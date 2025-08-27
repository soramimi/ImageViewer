#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MySettings.h"
#include "SettingsDialog.h"
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
		MySettings s;
		s.beginGroup("global");
		bool f = s.value("FitImageToView", false).toBool();
		ui->widget_image_viewer->fitImageToView(f);
		ui->action_fit_image_to_view->setChecked(f);
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
	dlg.exec();
}

