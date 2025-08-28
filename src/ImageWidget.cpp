#include "ImageWidget.h"
#include "MainWindow.h"
#include <QGridLayout>
#include <QPainter>

/**
 * @brief コンストラクタ。内部ビューとスクロールバーを生成しレイアウトへ配置する。
 * @param parent 親ウィジェット
 */
ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
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

/**
 * @brief デストラクタ。
 */
ImageWidget::~ImageWidget()
{
}

/**
 * @brief フィット表示の ON/OFF を内部ビューへ委譲。
 * @param fit true でフィット表示
 */
void ImageWidget::fitImageToView(bool fit)
{
	view_->fitImageToView(fit);
}

/**
 * @brief 現在の表示画像を取得。
 * @return 画像
 */
QImage ImageWidget::image() const
{
	return view_->image();
}

/**
 * @brief 画像を設定し、サイズが変わる場合は内部ビューの実サイズも更新。
 * @param image 新しい画像
 */
void ImageWidget::setImage(const QImage &image)
{
	double w = image.width();
	double h = image.height();
	if (w != view_->realWidth() || h != view_->realHeight()) {
		view_->setSize(w, h);
	}
	view_->setImage(image);
}

/**
 * @brief スケール値を直接設定。
 * @param scale 新しいスケール
 */
void ImageWidget::setScale(double scale)
{
	view_->setScale(scale);
}

/**
 * @brief 水平スクロールバー変更通知。内部ビュー位置を更新。
 * @param value スクロール値
 */
void ImageWidget::hsbValueChanged(int value)
{
	int x = hsb_->value();
	int y = vsb_->value();
	view_->setPosition(x, y);
}

/**
 * @brief 垂直スクロールバー変更通知。内部ビュー位置を更新。
 * @param value スクロール値
 */
void ImageWidget::vsbValueChanged(int value)
{
	int x = hsb_->value();
	int y = vsb_->value();
	view_->setPosition(x, y);
}
