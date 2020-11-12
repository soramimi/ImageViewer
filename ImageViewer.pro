QT       += core gui widgets svg

CONFIG += c++1z
CONFIG += nostrip debug_info

DESTDIR = $$PWD/_bin

SOURCES += \
	src/ImageWidget.cpp \
	src/InternalImageView.cpp \
	src/MainWindow.cpp \
	src/MySettings.cpp \
	src/joinpath.cpp \
	src/main.cpp

HEADERS += \
	src/ImageWidget.h \
	src/InternalImageView.h \
	src/MainWindow.h \
	src/MySettings.h \
	src/joinpath.h \
	src/main.h

FORMS += \
	src/MainWindow.ui

