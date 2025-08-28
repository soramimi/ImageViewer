#include "main.h"
#include "MainWindow.h"
#include <QBuffer>
#include <QCoreApplication>
#include <QFileIconProvider>
#include <QThread>
#include <memory>

/** @brief 内部実装用プライベート構造体。 */
struct ApplicationGlobal::Private {
};

/** @brief コンストラクタ。 */
ApplicationGlobal::ApplicationGlobal()
	: m(new Private)
{
}

/** @brief デストラクタ。 */
ApplicationGlobal::~ApplicationGlobal()
{
	delete m;
}

/** @brief 呼び出しスレッドが GUI メインスレッドか判定。 */
bool ApplicationGlobal::isMainThread()
{
	return QThread::currentThread() == QCoreApplication::instance()->thread();
}


