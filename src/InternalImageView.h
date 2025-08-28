#ifndef INTERNALIMAGEVIEW_H
#define INTERNALIMAGEVIEW_H

#include <QWidget>

class ImageWidget;
class QScrollBar;

/**
 * @brief 実際の画像描画とズーム・パン処理を担当する内部ビュー。
 *
 * 中心座標とスケールで表示を管理し、ドラッグによるパン、ホイールによるズーム、
 * フィット表示などの操作を提供します。
 */
class InternalImageView : public QWidget {
	Q_OBJECT
	friend class ImageWidget;
private:
	struct Private;
	Private *m;
	/** @brief 指定ビュー座標と実座標を基準にズーム位置を更新。 */
	void zoomTo(QPoint view_pt, QPointF real_pt, double scale);
	/** @brief フィット表示切替処理。 */
	void fitImageToView(bool fit);
	/** @brief スクロール可能か判定。 */
	bool isScrollable() const;
protected:
	void paintEvent(QPaintEvent *event) override;
public:
	/** @brief コンストラクタ。スクロールバー参照を保持。 */
	explicit InternalImageView(ImageWidget *parent, QScrollBar *hsb, QScrollBar *vsb);
	/** @brief デストラクタ。 */
	~InternalImageView();
	/** @brief 現在の拡大率取得。 */
	double scale() const;
	/** @brief 実画像の幅取得。 */
	double realWidth() const;
	/** @brief 実画像の高さ取得。 */
	double realHeight() const;
	/** @brief 実画像サイズ設定。 */
	void setSize(double w, double h);
	/** @brief 表示画像設定。 */
	void setImage(QImage const &image);
	/** @brief 表示オフセット (実座標) を設定。 */
	void setPosition(double x, double y);
	/** @brief 拡大率設定。 */
	void setScale(double s);
	/** @brief ビュー座標→実座標変換。 */
	QPointF posRealFromView(const QPointF &view_pt);
	/** @brief 実座標→ビュー座標変換。 */
	QPointF posViewFromReal(const QPointF &real_pt);
	/** @brief 現在画像取得。 */
	QImage image() const;
protected:
	void resizeEvent(QResizeEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // INTERNALIMAGEVIEW_H
