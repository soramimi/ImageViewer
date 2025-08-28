#ifndef SETTINGGENERALFORM_H
#define SETTINGGENERALFORM_H

#include "AbstractSettingForm.h"

#include <QWidget>

namespace Ui {
class SettingGeneralForm;
}

/**
 * @brief 一般設定ページ (ウィンドウ位置保存など)。
 */
class SettingGeneralForm : public AbstractSettingForm {
	Q_OBJECT
private:
	Ui::SettingGeneralForm *ui;

public:
	/** @brief コンストラクタ。 */
	explicit SettingGeneralForm(QWidget *parent = nullptr);
	/** @brief デストラクタ。 */
	~SettingGeneralForm() override;

	/** @brief UI と設定の交換。 */
	void exchange(bool save) override;
private slots:
};

#endif // SETTINGGENERALFORM_H
