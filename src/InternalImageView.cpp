#include "ImageWidget.h"
#include "InternalImageView.h"
#include "MainWindow.h"
#include <QWheelEvent>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QElapsedTimer>
#include <QMenu>

struct InternalImageView::Private {
	ImageWidget *parent;
	QScrollBar *hsb;
	QScrollBar *vsb;
	QImage image;
	double scale = 1;
	double real_x = 0;
	double real_y = 0;
	double real_w = 0;
	double real_h = 0;
	QPoint last_mouse_pt = {-1, -1};
	QPoint zoom_view_pt;
	QPointF zoom_real_pt;
	bool dragging = false;
	QPointF scroll_org_pt;
	bool fit_image_to_view = false;
};

InternalImageView::InternalImageView(ImageWidget *parent, QScrollBar *hsb, QScrollBar *vsb)
	: QWidget(parent)
	, m(new Private)
{
	m->parent = parent;
	m->hsb = hsb;
	m->vsb = vsb;
	setMouseTracking(true);
}

InternalImageView::~InternalImageView()
{
	delete m;
}

bool InternalImageView::isScrollable() const
{
	if (m->fit_image_to_view) return false;
	return true;
}

double InternalImageView::scale() const
{
	return m->scale;
}

double InternalImageView::realWidth() const
{
	return m->real_w;
}

double InternalImageView::realHeight() const
{
	return m->real_h;
}

void InternalImageView::zoomTo(QPoint view_pt, QPointF real_pt, double scale)
{
	const int w = width();
	const int h = height();
	scale = std::max(1 / 16.0, std::min(scale, 16.0));
	m->real_x = real_pt.x() - (view_pt.x() - w / 2) / scale;
	m->real_y = real_pt.y() - (view_pt.y() - h / 2) / scale;
	m->real_x = std::max(0.0, std::min(m->real_x, m->real_w));
	m->real_y = std::max(0.0, std::min(m->real_y, m->real_h));
	m->scroll_org_pt = {m->real_x, m->real_y};
	m->scale = scale;
	{
		bool b = m->hsb->blockSignals(true);
		m->hsb->setRange(0, m->real_w * m->scale);
		m->hsb->setValue(m->real_x * m->scale);
		m->hsb->setPageStep(width());
		m->hsb->blockSignals(b);
	}
	{
		bool b = m->vsb->blockSignals(true);
		m->vsb->setRange(0, m->real_h * m->scale);
		m->vsb->setValue(m->real_y * m->scale);
		m->vsb->setPageStep(height());
		m->vsb->blockSignals(b);
	}
	update();
}

void InternalImageView::fitImageToView(bool fit)
{
	m->fit_image_to_view = fit;
	if (m->fit_image_to_view) {
		double scale0 = width() / m->real_w;
		double scale1 = height() / m->real_h;
		double scale = std::min(scale0, scale1) * 0.95;
		zoomTo({width() / 2, height() / 2}, {m->real_w / 2, m->real_h / 2}, scale);
	}
}

void InternalImageView::setSize(double w, double h)
{
	m->real_w = w;
	m->real_h = h;
	if (m->fit_image_to_view) {
		fitImageToView(m->fit_image_to_view);
	} else {
		zoomTo({width() / 2, height() / 2}, {w / 2, h / 2}, m->scale);
	}
}

void InternalImageView::setPosition(double x, double y)
{
	m->real_x = x / m->scale;
	m->real_y = y / m->scale;
	update();
}

void InternalImageView::setScale(double s)
{
	m->scale = s;
	update();
}

QPointF InternalImageView::posRealFromView(QPointF const &view_pt)
{
	const int w = width();
	const int h = height();
	double x = (view_pt.x() - w / 2) / m->scale + m->real_x;
	double y = (view_pt.y() - h / 2) / m->scale + m->real_y;
	return {x, y};
}

QPointF InternalImageView::posViewFromReal(QPointF const &real_pt)
{
	const int w = width();
	const int h = height();
	double x = (real_pt.x() - m->real_x) * m->scale + w / 2;
	double y = (real_pt.y() - m->real_y) * m->scale + h / 2;
	return {x, y};
}

void InternalImageView::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if (m->fit_image_to_view) {
		fitImageToView(true);
	}
}

void InternalImageView::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		m->dragging = true;
	}

	bool done = false;

	if (!done) {
		setCursor(Qt::ClosedHandCursor);
	}
}

void InternalImageView::mouseReleaseEvent(QMouseEvent *event)
{
	bool done = false;

	if (!done) {
		setCursor(Qt::OpenHandCursor);
	}
	m->dragging = false;
}

void InternalImageView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pt = QCursor::pos();
	pt = mapFromGlobal(pt);
	if (m->last_mouse_pt != pt) {
		m->last_mouse_pt = pt;

		bool done = false;

		if (!done) {
			if (!m->dragging) {
				setCursor(Qt::OpenHandCursor);
			}
		}

		if (!done && m->dragging && isScrollable()) {
			if (event->buttons() & Qt::LeftButton) {
				QPointF delta = QPointF(pt - m->zoom_view_pt) / m->scale;
				m->real_x = m->scroll_org_pt.x() - delta.x();
				m->real_y = m->scroll_org_pt.y() - delta.y();
				m->real_x = std::max(0.0, std::min(m->real_x, m->real_w));
				m->real_y = std::max(0.0, std::min(m->real_y, m->real_h));
				{
					bool b = m->hsb->blockSignals(true);
					m->hsb->setValue(m->real_x * m->scale);
					m->hsb->blockSignals(b);
				}
				{
					bool b = m->vsb->blockSignals(true);
					m->vsb->setValue(m->real_y * m->scale);
					m->vsb->blockSignals(b);
				}
				update();
			}
		} else {
			m->zoom_view_pt = pt;
			m->zoom_real_pt = posRealFromView(m->zoom_view_pt);
			m->scroll_org_pt = {m->real_x, m->real_y};
		}
	}
}

void InternalImageView::wheelEvent(QWheelEvent *event)
{
	if (m->dragging) return;
	if (m->fit_image_to_view) return;

	double delta = event->angleDelta().y();
	double scale = m->scale * pow(1.002, delta);

	zoomTo(m->zoom_view_pt, m->zoom_real_pt, scale);
}

void InternalImageView::contextMenuEvent(QContextMenuEvent *event)
{
	m->dragging = false;
}

QImage InternalImageView::image() const
{
	return m->image;
}

void InternalImageView::setImage(const QImage &image)
{
	m->image = image;
	update();
}

void InternalImageView::paintEvent(QPaintEvent *event)
{
	QPainter pr(this);
	{
		int x = width() / 2 - m->real_x * m->scale;
		int y = height() / 2 - m->real_y * m->scale;
		int w = m->image.width() * m->scale;
		int h = m->image.height() * m->scale;
		pr.drawImage(QRect(x, y, w, h), m->image);
		QColor black(0, 0, 0);
		QColor gray(128, 128, 128);
		QColor white(255, 255, 255);
		pr.fillRect(x - 1, y - 1, w + 2, 1, black);
		pr.fillRect(x - 1, y - 1, 1, h + 2, black);
		pr.fillRect(x - 1, y + h, w + 2, 1, black);
		pr.fillRect(x + w, y - 1, 1, h + 2, black);
		pr.fillRect(x - 1, y - 2, w + 2, 1, gray);
		pr.fillRect(x - 2, y - 1, 1, h + 2, gray);
		pr.fillRect(x - 1, y + h + 1, w + 2, 1, white);
		pr.fillRect(x + w + 1, y - 1, 1, h + 2, white);
	}
}


