#ifndef MAIN_H
#define MAIN_H

#include "ApplicationSettings.h"
#include <QColor>
#include <QString>

class MainWindow;
class IncrementalSearch;

/**
 * @brief プロセス全体で共有するグローバル状態を保持する構造。
 */
class ApplicationGlobal : public ApplicationBasicData {
private:
	struct Private;
	Private *m;
public:
	ApplicationGlobal();
	~ApplicationGlobal();

	MainWindow *mainwindow = nullptr; //!< メインウィンドウポインタ
	bool start_with_shift_key = false; //!< Shift 起動フラグ

	ApplicationSettings appsettings;   //!< アプリ設定

	/** @brief 現在スレッドがメインスレッドか判定。 */
	static bool isMainThread();

};

#define ASSERT_MAIN_THREAD() Q_ASSERT(ApplicationGlobal::isMainThread())

/** @brief グローバル状態アクセス用外部ポインタ。 */
extern ApplicationGlobal *global;

#endif // MAIN_H
