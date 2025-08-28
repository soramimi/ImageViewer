#include "MainWindow.h"
#include "joinpath.h"
#include "main.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

/** @brief アプリケーショングローバル状態シングルトン的ポインタ。 */
ApplicationGlobal *global = nullptr;

/**
 * @brief エントリポイント。
 * 設定ディレクトリの生成・設定ロード・メインウィンドウ表示を行う。
 */
int main(int argc, char *argv[])
{
	ApplicationGlobal g;
	global = &g;

	global->organization_name = ORGANIZATION_NAME;
	global->application_name = APPLICATION_NAME;
	global->generic_config_dir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
	global->app_config_dir = global->generic_config_dir / global->organization_name / global->application_name;
	global->log_dir = global->app_config_dir / "log";
	global->config_file_path = joinpath(global->app_config_dir, "settings.ini");

	auto MKPATH = [&](const QString &path) {
		if (!QFileInfo(path).isDir()) {
			if (!QDir().mkpath(path)) {
				qDebug() << "Failed to create directory:" << path;
			}
		}
	};

	MKPATH(global->app_config_dir);
	MKPATH(global->log_dir);

	global->appsettings = ApplicationSettings::loadSettings();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
