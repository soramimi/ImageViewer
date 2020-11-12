#ifndef MAIN_H
#define MAIN_H

#include <QString>

#define ORGANIZATION_NAME "example.com"
#define APPLICATION_NAME "MyApp"

struct Global {
	QString organization_name = ORGANIZATION_NAME;
	QString application_name = APPLICATION_NAME;
	QString generic_config_dir;
	QString app_config_dir;
	QString config_file_path;
};

extern Global *global;

#endif // MAIN_H
