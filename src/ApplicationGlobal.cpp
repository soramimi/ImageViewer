#include "main.h"
#include "MainWindow.h"
#include <QBuffer>
#include <QCoreApplication>
#include <QFileIconProvider>
#include <QThread>
#include <memory>

struct ApplicationGlobal::Private {
};

ApplicationGlobal::ApplicationGlobal()
	: m(new Private)
{
}

ApplicationGlobal::~ApplicationGlobal()
{
	delete m;
}

bool ApplicationGlobal::isMainThread()
{
	return QThread::currentThread() == QCoreApplication::instance()->thread();
}


