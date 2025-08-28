#include "ApplicationSettings.h"
#include "MySettings.h"
#include "joinpath.h"
#include "misc.h"
#include <QStandardPaths>

namespace {

// 汎用テンプレート: 設定値読取/書込用ヘルパ。
template <typename T> class GetValue {
public:
	MySettings &settings; QString name;
	GetValue(MySettings &s, QString const &name) : settings(s), name(name) {}
};

template <typename T> void operator >> (GetValue<T> const &l, T &r)
{ r = l.settings.value(l.name, r).template value<T>(); }

template <> void operator >> (GetValue<QColor> const &l, QColor &r)
{
	QString s = l.settings.value(l.name, QString()).template value<QString>();
	if (s.startsWith('#')) { r = s; }
}

template <> void operator >> (GetValue<std::string> const &l, std::string &r)
{ r = l.settings.value(l.name, QString::fromStdString(r)).toString().toStdString(); }

template <typename T> class SetValue {
public:
	MySettings &settings; QString name;
	SetValue(MySettings &s, QString const &name) : settings(s), name(name) {}
};

template <typename T> void operator << (SetValue<T> &&l, T const &r)
{ l.settings.setValue(l.name, r); }

template <> void operator << (SetValue<QColor> &&l, QColor const &r)
{ QString s = QString::asprintf("#%02x%02x%02x", r.red(), r.green(), r.blue()); l.settings.setValue(l.name, s); }

template <> void operator << (SetValue<std::string> &&l, std::string const &r)
{ l.settings.setValue(l.name, QString::fromStdString(r)); }

} // namespace

/**
 * @brief 設定ファイルからアプリ設定を読み込む。
 * 未存在項目はデフォルト値を使用。
 */
ApplicationSettings ApplicationSettings::loadSettings()
{
	ApplicationSettings as(defaultSettings());

	MySettings s;

	s.beginGroup("Global");
	GetValue<bool>(s, "SaveWindowPosition")                  >> as.remember_and_restore_window_position;
	s.endGroup();

	return as;
}

/**
 * @brief 現在の設定値を永続化する。
 */
void ApplicationSettings::saveSettings() const
{
	MySettings s;

	s.beginGroup("Global");
	SetValue<bool>(s, "SaveWindowPosition")                  << this->remember_and_restore_window_position;
	s.endGroup();
}

/**
 * @brief デフォルト設定値を返す。
 */
ApplicationSettings ApplicationSettings::defaultSettings()
{
	return {};
}
