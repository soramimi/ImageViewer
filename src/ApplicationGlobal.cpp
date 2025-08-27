#include "main.h"
#include "MainWindow.h"
#include <QBuffer>
#include <QFileIconProvider>
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

