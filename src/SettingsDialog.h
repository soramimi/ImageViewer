#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "ApplicationSettings.h"
#include "MainWindow.h"
#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class QTreeWidgetItem;

/**
 * @brief アプリ設定を編集するダイアログ。ツリー + スタック形式でページを切替。
 */
class SettingsDialog : public QDialog
{
	Q_OBJECT
public:
	ApplicationSettings settings_; //!< 編集対象設定
private:
	Ui::SettingsDialog *ui;
	MainWindow *mainwindow_;

	void exchange(bool save);

	void loadSettings();
	void saveSettings();

public:
	/** @brief コンストラクタ。ページ生成と前回選択ページ復元。 */
	explicit SettingsDialog(MainWindow *parent);
	/** @brief デストラクタ。 */
	~SettingsDialog() override;

	MainWindow *mainwindow()
	{
		return mainwindow_;
	}

	SettingsDialog *dialog()
	{
		return this;
	}

	/** @brief 編集後の設定値取得。 */
	ApplicationSettings const &settings() const
	{
		return settings_;
	}

private slots:
	/** @brief ツリー選択変更時: 対応ページへ切替。 */
	void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
public slots:
	/** @brief OK (accept) 処理: 値を保存。 */
	void accept() override;
	/** @brief 完了処理: ページ番号保存。 */
	void done(int) override;
};

#endif // SETTINGSDIALOG_H
