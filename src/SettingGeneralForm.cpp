#include "SettingGeneralForm.h"
#include "ui_SettingGeneralForm.h"
#include "MySettings.h"
#include "main.h"

#include <QFileDialog>

/** @brief コンストラクタ: UI 初期化。 */
SettingGeneralForm::SettingGeneralForm(QWidget *parent)
	: AbstractSettingForm(parent)
	, ui(new Ui::SettingGeneralForm)
{
	ui->setupUi(this);
}

/** @brief デストラクタ。 */
SettingGeneralForm::~SettingGeneralForm()
{
	delete ui;
}

/**
 * @brief UI と設定の交換。
 * @param save true で UI→設定, false で設定→UI
 */
void SettingGeneralForm::exchange(bool save)
{
	if (save) {
		settings()->remember_and_restore_window_position = ui->checkBox_save_window_pos->isChecked();
	} else {
		ui->checkBox_save_window_pos->setChecked(settings()->remember_and_restore_window_position);
	}
}

