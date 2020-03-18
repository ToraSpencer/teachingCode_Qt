#-------------------------------------------------
#
# Project created by QtCreator 2020-03-18T18:07:23
#
#-------------------------------------------------

QT       += core gui

# ！！！注意
QT       += opengl



# ！！！WINDOWS系统下需要手动添加静态库，否则无法正常使用功能。
win32-g++ {
    LIBS += -lopengl32
    LIBS += -lglu32
}
win32-msvc*{
    LIBS += opengl32.lib
    LIBS += glu32.lib
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = teachingCode-opengl
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
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h

FORMS += \
        mainwindow.ui \
    glwidget.ui
