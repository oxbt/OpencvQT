#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T18:59:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpencvDevelop
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

# OpenCV
win32:CONFIG(release, debug|release): LIBS += -LD:/dev/opencv/x64/vc16/lib/ -lopencv_world470
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/dev/opencv/x64/vc16/lib/ -lopencv_world470d
INCLUDEPATH += D:/dev/opencv/include
DEPENDPATH += D:/dev/opencv/include

SOURCES += main.cpp\
        mainwindow.cpp \
    imagelook.cpp \
    mypixitem.cpp \
    myimagelist.cpp \
    codewidget.cpp \
    variatedialog.cpp \
    histogtamdlg.cpp \
    showhistogtamresult.cpp

HEADERS  += mainwindow.h \
    imagelook.h \
    mypixitem.h \
    myimagelist.h \
    codewidget.h \
    variatedialog.h \
    histogtamdlg.h \
    showhistogtamresult.h

FORMS    += mainwindow.ui \
    imagelook.ui \
    myimagelist.ui \
    codewidget.ui \
    histogtamdlg.ui \
    showhistogtamresult.ui

RESOURCES += \
    rc.qrc
