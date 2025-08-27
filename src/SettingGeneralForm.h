#ifndef SETTINGGENERALFORM_H
#define SETTINGGENERALFORM_H

#include "AbstractSettingForm.h"

#include <QWidget>

namespace Ui {
class SettingGeneralForm;
}

class SettingGeneralForm : public AbstractSettingForm {
	Q_OBJECT
private:
	Ui::SettingGeneralForm *ui;

public:
	explicit SettingGeneralForm(QWidget *parent = nullptr);
	~SettingGeneralForm() override;

	void exchange(bool save) override;
private slots:
};

#endif // SETTINGGENERALFORM_H
