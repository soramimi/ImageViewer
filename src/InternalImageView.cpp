#include "ImageWidget.h"
#include "InternalImageView.h"
#include "MainWindow.h"
#include <QWheelEvent>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QElapsedTimer>
#include <QMenu>

/**
 * @brief 内部状態保持用プライベート構造体。
 */
struct InternalImageView::Private {
	ImageWidget *parent;
	QScrollBar *hsb;
	QScrollBar *vsb;
	QImage image;
	double scale = 1;              //!< 拡大率
	double real_x = 0;             //!< 表示中心の実座標X
	double real_y = 0;             //!< 表示中心の実座標Y
	double real_w = 0;             //!< 画像実幅
	double real_h = 0;             //!< 画像実高
	QPoint last_mouse_pt = {-1, -1};
	QPoint zoom_view_pt;           //!< ズーム基準ビュー座標
	QPointF zoom_real_pt;          //!< ズーム基準実座標
	bool dragging = false;         //!< ドラッグ中フラグ
	QPointF scroll_org_pt;         //!< ドラッグ開始時の原点
	bool fit_image_to_view = false;//!< フィット表示フラグ
};

/**
 * @brief コンストラクタ。状態初期化とマウストラッキング有効化。
 */
InternalImageView::InternalImageView(ImageWidget *parent, QScrollBar *hsb, QScrollBar *vsb)
	: QWidget(parent)
	, m(new Private)
{
	m->parent = parent;
	m->hsb = hsb;
	m->vsb = vsb;
	setMouseTracking(true);
}

/**
 * @brief デストラクタ。状態解放。
 */
InternalImageView::~InternalImageView()
{
	delete m;
}

/**
 * @brief スクロール可能か判定。
 * フィット表示中はスクロール不可。
 */
bool InternalImageView::isScrollable() const
{
	if (m->fit_image_to_view) return false;
	return true;
}

/** @brief 現在の拡大率取得。 */
double InternalImageView::scale() const
{
	return m->scale;
}

/** @brief 実画像幅取得。 */
double InternalImageView::realWidth() const
{
	return m->real_w;
}

/** @brief 実画像高取得。 */
double InternalImageView::realHeight() const
{
	return m->real_h;
}

/**
 * @brief 指定位置を基準にズーム/スクロール位置を更新。
 * @param view_pt ビュー座標
 * @param real_pt 対応する実座標
 * @param scale 新しい拡大率 (クランプ 1/16～16)
 */
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

/**
 * @brief フィット表示を設定。ONの場合ビューサイズから最適スケール算出。
 */
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

/**
 * @brief 実画像サイズを設定し、表示位置を中心に再計算。
 */
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

/**
 * @brief スクロールバー値から表示中心実座標を更新。
 */
void InternalImageView::setPosition(double x, double y)
{
	m->real_x = x / m->scale;
	m->real_y = y / m->scale;
	update();
}

/**
 * @brief 拡大率を設定 (中心基準維持せず単純再描画)。
 */
void InternalImageView::setScale(double s)
{
	m->scale = s;
	update();
}

/** @brief ビュー座標→実座標変換。 */
QPointF InternalImageView::posRealFromView(QPointF const &view_pt)
{
	const int w = width();
	const int h = height();
	double x = (view_pt.x() - w / 2) / m->scale + m->real_x;
	double y = (view_pt.y() - h / 2) / m->scale + m->real_y;
	return {x, y};
}

/** @brief 実座標→ビュー座標変換。 */
QPointF InternalImageView::posViewFromReal(QPointF const &real_pt)
{
	const int w = width();
	const int h = height();
	double x = (real_pt.x() - m->real_x) * m->scale + w / 2;
	double y = (real_pt.y() - m->real_y) * m->scale + h / 2;
	return {x, y};
}

/** @brief リサイズ時。フィット中なら再計算。 */
void InternalImageView::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if (m->fit_image_to_view) {
		fitImageToView(true);
	}
}

/** @brief マウス押下: ドラッグ開始判定とカーソル変更。 */
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

/** @brief マウス解放: ドラッグ終了とカーソル刷新。 */
void InternalImageView::mouseReleaseEvent(QMouseEvent *event)
{
	bool done = false;

	if (!done) {
		setCursor(Qt::OpenHandCursor);
	}
	m->dragging = false;
}

/** @brief マウス移動: ドラッグ中はスクロール、非ドラッグ時はズーム基準更新。 */
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

/** @brief ホイール操作でのズーム処理 (フィット/ドラッグ中は無効)。 */
void InternalImageView::wheelEvent(QWheelEvent *event)
{
	if (m->dragging) return;
	if (m->fit_image_to_view) return;

	double delta = event->angleDelta().y();
	double scale = m->scale * pow(1.002, delta);

	zoomTo(m->zoom_view_pt, m->zoom_real_pt, scale);
}

/** @brief コンテキストメニューイベント: ドラッグ解除のみ。 */
void InternalImageView::contextMenuEvent(QContextMenuEvent *event)
{
	m->dragging = false;
}

/** @brief 現在保持画像取得。 */
QImage InternalImageView::image() const
{
	return m->image;
}

/** @brief 画像を設定し再描画。 */
void InternalImageView::setImage(const QImage &image)
{
	m->image = image;
	update();
}

/** @brief 描画イベント: 画像を中心基準で描画し簡易枠線を付与。 */
void InternalImageView::paintEvent(QPaintEvent *event)
{
	QPainter pr(this);
	{
		int x = width() / 2 - m->real_x * m->scale;
		int y = height() / 2 - m->real_y * m->scale;
		int w = m->image.width() * m->scale;
		int h = m->image.height() * m->scale;
		if (w > 0 && h > 0) {
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
}


