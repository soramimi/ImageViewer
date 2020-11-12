#include "ImageWidget.h"
#include "MainWindow.h"
#include "main.h"
#include <QGridLayout>
#include <QPainter>


ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	hsb_ = new QScrollBar(Qt::Horizontal, this);
	vsb_ = new QScrollBar(Qt::Vertical, this);
	view_ = new InternalImageView(this, hsb_, vsb_);
	view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(view_, 0, 0);
	layout->addWidget(hsb_, 1, 0);
	layout->addWidget(vsb_, 0, 1);
	connect(hsb_, &QScrollBar::valueChanged, this, &ImageWidget::hsbValueChanged);
	connect(vsb_, &QScrollBar::valueChanged, this, &ImageWidget::vsbValueChanged);
}

MainWindow *ImageWidget::mainwindow()
{
	return qobject_cast<MainWindow *>(window());
}

MainWindow const *ImageWidget::mainwindow() const
{
	return qobject_cast<MainWindow *>(window());
}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::fitImageToView(bool fit)
{
	view_->fitImageToView(fit);
}

QImage ImageWidget::image() const
{
	return view_->image();
}

void ImageWidget::setImage(const QImage &image)
{
	double w = image.width();
	double h = image.height();
	if (w != view_->realWidth() || h != view_->realHeight()) {
		view_->setSize(w, h);
	}
	view_->setImage(image);
}

void ImageWidget::setScale(double scale)
{
	view_->setScale(scale);
}

void ImageWidget::hsbValueChanged(int value)
{
	int x = hsb_->value();
	int y = vsb_->value();
	view_->setPosition(x, y);
}

void ImageWidget::vsbValueChanged(int value)
{
	int x = hsb_->value();
	int y = vsb_->value();
	view_->setPosition(x, y);
}
