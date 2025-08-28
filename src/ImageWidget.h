#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "InternalImageView.h"
#include <QScrollBar>
#include <QWidget>

class MainWindow;

/**
 * @brief 画像表示用の複合ウィジェット。
 *
 * 内部にスクロールバーと `InternalImageView` を持ち、
 * ズーム・パンの制御を仲介します。
 */
class ImageWidget : public QWidget {
    Q_OBJECT
	friend class MainWindow;
	friend class InternalImageView;
private:
	InternalImageView *view_;
	QScrollBar *hsb_;
	QScrollBar *vsb_;
	MainWindow *mainwindow();
	const MainWindow *mainwindow() const;
public:
	/** @brief コンストラクタ。内部レイアウト/スクロールバー構築。 */
	explicit ImageWidget(QWidget *parent = nullptr);
	/** @brief デストラクタ。 */
	~ImageWidget();
	/** @brief フィット表示ON/OFF。 */
	void fitImageToView(bool fit);
    /** @brief 表示画像を設定し、必要なら内部サイズ更新。 */
	void setImage(QImage const &image);
	/** @brief スケール値を直接設定。 */
	void setScale(double scale);
	/** @brief 現在表示中の画像を取得。 */
	QImage image() const;
private slots:
	/** @brief 水平スクロールバー変更ハンドラ。 */
	void hsbValueChanged(int value);
	/** @brief 垂直スクロールバー変更ハンドラ。 */
	void vsbValueChanged(int value);
};

#endif // IMAGEWIDGET_H
