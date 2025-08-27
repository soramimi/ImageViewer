#include "SettingGeneralForm.h"
#include "ui_SettingGeneralForm.h"
#include "MySettings.h"
#include "main.h"

#include <QFileDialog>

SettingGeneralForm::SettingGeneralForm(QWidget *parent)
	: AbstractSettingForm(parent)
	, ui(new Ui::SettingGeneralForm)
{
	ui->setupUi(this);
}

SettingGeneralForm::~SettingGeneralForm()
{
	delete ui;
}

void SettingGeneralForm::exchange(bool save)
{
	if (save) {
		settings()->remember_and_restore_window_position = ui->checkBox_save_window_pos->isChecked();
	} else {
		ui->checkBox_save_window_pos->setChecked(settings()->remember_and_restore_window_position);
	}
}

