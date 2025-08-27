#ifndef MAIN_H
#define MAIN_H

#include "ApplicationSettings.h"
#include <QColor>
#include <QString>

class MainWindow;
class IncrementalSearch;

class ApplicationGlobal : public ApplicationBasicData {
private:
	struct Private;
	Private *m;
public:
	ApplicationGlobal();
	~ApplicationGlobal();

	MainWindow *mainwindow = nullptr;
	bool start_with_shift_key = false;

	ApplicationSettings appsettings;

	static bool isMainThread();

};

#define ASSERT_MAIN_THREAD() Q_ASSERT(ApplicationGlobal::isMainThread())

extern ApplicationGlobal *global;

#endif // MAIN_H
