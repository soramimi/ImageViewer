#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "MySettings.h"

#include "SettingGeneralForm.h"

#include <QFileDialog>

static int page_number = 0;

SettingsDialog::SettingsDialog(MainWindow *parent)
	: QDialog(parent)
	, ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
	Qt::WindowFlags flags = windowFlags();
	flags &= ~Qt::WindowContextHelpButtonHint;
	setWindowFlags(flags);

	mainwindow_ = parent;

	auto AddPage = [&](AbstractSettingForm *page, bool enabled = true){
		ui->stackedWidget->addWidget(page);
		auto *l = page->layout();
		if (l) {
			l->setContentsMargins(0, 0, 0, 0);
		}
		page->reset(mainwindow_, &settings_);
		if (enabled) {
			QString name = page->windowTitle();
			QTreeWidgetItem *item = new QTreeWidgetItem();
			item->setText(0, name);
			item->setData(0, Qt::UserRole, QVariant::fromValue((uintptr_t)(QWidget *)page));
			ui->treeWidget->addTopLevelItem(item);
		}
	};

	AddPage(new SettingGeneralForm(this));

	loadSettings();

	ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(page_number));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::saveSettings()
{
	settings_.saveSettings();
}

void SettingsDialog::exchange(bool save)
{
	QList<AbstractSettingForm *> forms = ui->stackedWidget->findChildren<AbstractSettingForm *>();
	for (AbstractSettingForm *form : forms) {
		form->exchange(save);
	}
}

void SettingsDialog::loadSettings()
{
	settings_ = ApplicationSettings::loadSettings();
	exchange(false);
}

void SettingsDialog::done(int r)
{
	page_number = ui->treeWidget->currentIndex().row();
	QDialog::done(r);
}

void SettingsDialog::accept()
{
	exchange(true);
	saveSettings();
	done(QDialog::Accepted);
}

void SettingsDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	(void)previous;
	if (current) {
		uintptr_t p = current->data(0, Qt::UserRole).value<uintptr_t>();
		QWidget *w = reinterpret_cast<QWidget *>(p);
		Q_ASSERT(w);
		ui->stackedWidget->setCurrentWidget(w);
	}
}

