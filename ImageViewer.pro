QMAKE_PROJECT_DEPTH = 0

QT       += core gui widgets svg

CONFIG += c++1z
CONFIG += nostrip debug_info

DESTDIR = $$PWD/_bin

SOURCES += \
	src/AbstractSettingForm.cpp \
	src/ApplicationGlobal.cpp \
	src/ApplicationSettings.cpp \
	src/ImageWidget.cpp \
	src/InternalImageView.cpp \
	src/MainWindow.cpp \
	src/MySettings.cpp \
	src/ReadOnlyLineEdit.cpp \
	src/SettingGeneralForm.cpp \
	src/SettingsDialog.cpp \
	src/joinpath.cpp \
	src/main.cpp \
	src/misc.cpp

HEADERS += \
	src/AbstractSettingForm.h \
	src/ApplicationGlobal.h \
	src/ApplicationSettings.h \
	src/ImageWidget.h \
	src/InternalImageView.h \
	src/MainWindow.h \
	src/MySettings.h \
	src/ReadOnlyLineEdit.h \
	src/SettingGeneralForm.h \
	src/SettingsDialog.h \
	src/joinpath.h \
	src/main.h \
	src/misc.h

FORMS += \
	src/MainWindow.ui \
	src/SettingGeneralForm.ui \
	src/SettingsDialog.ui

