#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

/**
 * @brief アプリ固有の設定ファイル (INI) へアクセスする QSettings 拡張。
 */
class MySettings : public QSettings {
	Q_OBJECT
public:
	/** @brief コンストラクタ。既定 INI パスで初期化。 */
	explicit MySettings(QObject *parent = nullptr);
};

#endif // MYSETTINGS_H
