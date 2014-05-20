#-------------------------------------------------
#
# Project created by QtCreator 2014-05-20T15:28:29
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = db_api_console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    json.cpp \
    user.cpp \
    forum.cpp \
    thread.cpp \
    post.cpp \
    forumdao.cpp \
    databaseconnection.cpp \
    threaddao.cpp \
    userdao.cpp \
    postdao.cpp \
    mainwindow.cpp

HEADERS += \
    json.h \
    forum.h \
    databaseconnection.h \
    userdao.h \
    user.h \
    forumdao.h \
    post.h \
    thread.h \
    threaddao.h \
    postdao.h \
    mainwindow.h
