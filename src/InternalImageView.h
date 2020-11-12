#ifndef INTERNALIMAGEVIEW_H
#define INTERNALIMAGEVIEW_H

#include <QWidget>

class ImageWidget;
class QScrollBar;

class InternalImageView : public QWidget {
	Q_OBJECT
	friend class ImageWidget;
private:
	struct Private;
	Private *m;
	void zoomTo(QPoint view_pt, QPointF real_pt, double scale);
	void fitImageToView(bool fit);
	bool isScrollable() const;
protected:
	void paintEvent(QPaintEvent *event);
public:
	explicit InternalImageView(ImageWidget *parent, QScrollBar *hsb, QScrollBar *vsb);
	~InternalImageView();
	double scale() const;
	double realWidth() const;
	double realHeight() const;
	void setSize(double w, double h);
	void setImage(QImage const &image);
	void setPosition(double x, double y);
	void setScale(double s);
	QPointF posRealFromView(const QPointF &view_pt);
	QPointF posViewFromReal(const QPointF &real_pt);
	QImage image() const;
protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
};

#endif // INTERNALIMAGEVIEW_H
