#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Video;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
	friend class ImageWidget;
	friend class InternalImageView;
	friend class ThumbnailCache;
private:
	Ui::MainWindow *ui;
    struct Private;
	Private *m;
	bool openFile(const QString &path);
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	QImage thumbnail(int64_t index, bool add);
private slots:
	void on_action_file_open_triggered();
	void on_action_fit_image_to_view_changed();
	void on_action_copy_triggered();
	void on_action_settings_triggered();
};
#endif // MAINWINDOW_H
