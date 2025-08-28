#include "AbstractSettingForm.h"

/** @brief コンストラクタ。 */
AbstractSettingForm::AbstractSettingForm(QWidget *parent)
	: QWidget(parent)
{
}

/** @brief 関連メインウィンドウ取得。 */
MainWindow *AbstractSettingForm::mainwindow()
{
#if 0
	auto *w = qobject_cast<SettingsDialog *>(window());
	Q_ASSERT(w);
	return w->mainwindow();
#else
	return mainwindow_;
#endif
}

/** @brief 関連設定オブジェクト取得。 */
ApplicationSettings *AbstractSettingForm::settings()
{
#if 0
	auto *w = qobject_cast<SettingsDialog *>(window());
	Q_ASSERT(w);
	return &w->set;
#else
	return settings_;
#endif
}
