#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MySettings.h"
#include "SettingsDialog.h"
#include "main.h"
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>

/**
 * @brief MainWindow のプライベート実装構造体。
 */
struct MainWindow::Private {
};

/**
 * @brief コンストラクタ。UI 初期化と設定値(フィット/ウィンドウ状態)復元を行う。
 * @param parent 親ウィジェット
 */
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m(new Private)
{
	ui->setupUi(this);
	
	// 画像フィット設定を復元
	{
		MySettings settings;
		settings.beginGroup("global");
		bool f = settings.value("FitImageToView", false).toBool();
		settings.endGroup();

		ui->widget_image_viewer->fitImageToView(f);
		ui->action_fit_image_to_view->setChecked(f);
	}

	// ウィンドウ位置・サイズの復元
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

/**
 * @brief デストラクタ。所有リソースを破棄。
 */
MainWindow::~MainWindow()
{
	delete m;
	delete ui;
}

/**
 * @brief 画像ファイルを開いて表示する。
 * @param path 画像ファイルパス
 * @return 成功時 true / 失敗時 false
 */
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

/**
 * @brief [ファイルを開く] アクション処理。ダイアログ表示後、選択画像を読み込む。
 */
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

/**
 * @brief ウィンドウクローズ時の処理。位置/サイズを必要に応じて保存。
 * @param event クローズイベント
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
	MySettings settings;

	if (global->appsettings.remember_and_restore_window_position) {
		setWindowOpacity(0); // フリッカー抑制
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

/**
 * @brief [フィット] チェック状態変更時の処理。内部ビューへ反映し設定保存。
 */
void MainWindow::on_action_fit_image_to_view_changed()
{
	bool f = ui->action_fit_image_to_view->isChecked();
	ui->widget_image_viewer->fitImageToView(f);

	MySettings s;
	s.beginGroup("global");
	s.setValue("FitImageToView", f);
}

/**
 * @brief 現在表示中画像をクリップボードへコピー。
 */
void MainWindow::on_action_copy_triggered()
{
	QImage img = ui->widget_image_viewer->image();
	qApp->clipboard()->setImage(img);
}

/**
 * @brief 設定ダイアログ表示。OK の場合は設定を反映。
 */
void MainWindow::on_action_settings_triggered()
{
	SettingsDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		ApplicationSettings const &newsettings = dlg.settings();
		global->appsettings = newsettings;
	}
}



