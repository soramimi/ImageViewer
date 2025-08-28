#include "MySettings.h"
#include "main.h"

/**
 * @brief settings.ini (INI 形式) をベースに QSettings を初期化する。
 */
MySettings::MySettings(QObject *)
	: QSettings(global->config_file_path, QSettings::IniFormat)
{
}

