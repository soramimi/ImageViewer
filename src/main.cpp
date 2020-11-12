#include "main.h"
#include "MainWindow.h"
#include "joinpath.h"
#include <QApplication>
#include <QStandardPaths>

Global *global = nullptr;

int main(int argc, char *argv[])
{
	Global g;
	global = &g;

	global->organization_name = ORGANIZATION_NAME;
	global->application_name = APPLICATION_NAME;
	global->generic_config_dir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
	global->app_config_dir = global->generic_config_dir / global->organization_name / global->application_name;
	global->config_file_path = joinpath(global->app_config_dir, "settings.ini");

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
