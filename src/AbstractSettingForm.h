#ifndef ABSTRACTSETTINGFORM_H
#define ABSTRACTSETTINGFORM_H

#include "SettingsDialog.h"
#include <QWidget>

class ApplicationSettings;
class MainWindow;

/**
 * @brief 設定ページ用抽象基底フォーム。
 * exchange() で UI と設定値を相互変換する。
 */
class AbstractSettingForm : public QWidget {
	Q_OBJECT
private:
	MainWindow *mainwindow_ = nullptr; /**< @brief 関連メインウィンドウ */
	ApplicationSettings *settings_ = nullptr; /**< @brief 関連設定オブジェクト */
protected:
	/** @brief 関連メインウィンドウ取得。 */
	MainWindow *mainwindow();
	/** @brief 関連設定オブジェクト取得。 */
	ApplicationSettings *settings();
public:
	/** @brief コンストラクタ。 */
	AbstractSettingForm(QWidget *parent = nullptr);
	/** @brief 親ウィンドウと設定オブジェクトを関連付け。 */
	void reset(MainWindow *mw, ApplicationSettings *s)
	{
		mainwindow_ = mw;
		settings_ = s;
	}
	/** @brief UI と設定の交換。save=true で UI→設定, false で設定→UI。 */
	virtual void exchange(bool save) = 0;
};

#endif // ABSTRACTSETTINGFORM_H
