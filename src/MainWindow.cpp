#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MySettings.h"
#include "SettingsDialog.h"
#include "main.h"
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>

struct MainWindow::Private {
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m(new Private)
{
	ui->setupUi(this);
	
	{
		MySettings settings;
		settings.beginGroup("global");
		bool f = settings.value("FitImageToView", false).toBool();
		settings.endGroup();

		ui->widget_image_viewer->fitImageToView(f);
		ui->action_fit_image_to_view->setChecked(f);
	}

	{
		if (global->appsettings.remember_and_restore_window_position) {
			Qt::WindowStates state = windowState();

			MySettings settings;
			settings.beginGroup("MainWindow");
			bool maximized = settings.value("Maximized").toBool();
			restoreGeometry(settings.value("Geometry").toByteArray());
			settings.endGroup();

			if (maximized) {
				state |= Qt::WindowMaximized;
				setWindowState(state);
			}
		}
	}
}

MainWindow::~MainWindow()
{
	delete m;
	delete ui;
}

bool MainWindow::openFile(QString const &path)
{
	QImage image;
	image.load(path);
	if (!image.isNull()) {
		ui->widget_image_viewer->setImage(image);
		return true;
	}
	return false;
}

void MainWindow::on_action_file_open_triggered()
{
	QString path;

	MySettings s;
	s.beginGroup("global");
	path = s.value("RecentFile").toString();

	path = QFileDialog::getOpenFileName(this, "Open", path, "Image Files (*.jpg *.jpeg *.png);;All Files (*.*)");

	if (!path.isEmpty()) {
		s.setValue("RecentFile", path);

		if (!openFile(path)) {
			QMessageBox::critical(this, tr("File Open Error"), tr("Failed to open the file."));
			return;
		}
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	MySettings settings;

	if (global->appsettings.remember_and_restore_window_position) {
		setWindowOpacity(0);
		Qt::WindowStates state = windowState();
		bool maximized = (state & Qt::WindowMaximized) != 0;
		if (maximized) {
			state &= ~Qt::WindowMaximized;
			setWindowState(state);
		}
		{
			settings.beginGroup("MainWindow");
			settings.setValue("Maximized", maximized);
			settings.setValue("Geometry", saveGeometry());
			settings.endGroup();
		}
	}

	QMainWindow::closeEvent(event);
}

void MainWindow::on_action_fit_image_to_view_changed()
{
	bool f = ui->action_fit_image_to_view->isChecked();
	ui->widget_image_viewer->fitImageToView(f);

	MySettings s;
	s.beginGroup("global");
	s.setValue("FitImageToView", f);
}

void MainWindow::on_action_copy_triggered()
{
	QImage img = ui->widget_image_viewer->image();
	qApp->clipboard()->setImage(img);
}

void MainWindow::on_action_settings_triggered()
{
	SettingsDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		ApplicationSettings const &newsettings = dlg.settings();
		global->appsettings = newsettings;
	}
}



