#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "InternalImageView.h"
#include <QScrollBar>
#include <QWidget>
#include "main.h"

class MainWindow;

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
	explicit ImageWidget(QWidget *parent = nullptr);
	~ImageWidget();
	void fitImageToView(bool fit);
    void setImage(QImage const &image);
	void setScale(double scale);
	QImage image() const;
private slots:
	void hsbValueChanged(int value);
	void vsbValueChanged(int value);
};

#endif // IMAGEWIDGET_H
