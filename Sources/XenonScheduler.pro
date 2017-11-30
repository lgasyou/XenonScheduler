#-------------------------------------------------
#
# Project created by QtCreator 2017-11-28T20:58:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XenonScheduler
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \ 
    Application/CreateOperationDialog.cpp \
    Application/CreateTaskWizard.cpp \
    Application/CreateTriggerDialog.cpp \
    Application/EditOperationDialog.cpp \
    Application/EditTriggerDialog.cpp \
    Application/GeneralSettingDialog.cpp \
    Application/main.cpp \
    Application/Scheduler.cpp \
    Application/TaskOptionDialog.cpp \
    Application/TaskTableWidget.cpp \
    Plugins/TaskManagement/Operation.cpp \
    Plugins/TaskManagement/Task.cpp \
    Plugins/TaskManagement/TaskManager.cpp \
    Plugins/TaskManagement/Trigger.cpp

HEADERS += \ 
    Application/CreateOperationDialog.h \
    Application/CreateTaskWizard.h \
    Application/CreateTriggerDialog.h \
    Application/EditOperationDialog.h \
    Application/EditTriggerDialog.h \
    Application/GeneralSettingDialog.h \
    Application/Scheduler.h \
    Application/TaskOptionDialog.h \
    Application/TaskTableWidget.h \
    Plugins/TaskManagement/Operation.h \
    Plugins/TaskManagement/Task.h \
    Plugins/TaskManagement/TaskManager.h \
    Plugins/TaskManagement/Trigger.h

FORMS += \ 
    Application/CreateOperationDialog.ui \
    Application/CreateTriggerDialog.ui \
    Application/EditOperationDialog.ui \
    Application/EditTriggerDialog.ui \
    Application/GeneralSettingDialog.ui \
    Application/TaskOptionDialog.ui

RESOURCES += \
    Application/Resources/AppResource.qrc
