#-------------------------------------------------
#
# Project created by QtCreator 2016-08-04T22:12:00
#
#-------------------------------------------------

QT       += widgets network

TARGET = $$qtLibraryTarget(QtUtils)
VERSION = 2.0.0
TEMPLATE = lib
CONFIG += staticlib

win32:CONFIG += skip_target_version_ext

include(../qtutils.pri)

HEADERS += qtutilslibrary.h

SOURCES += qtutilslibrary.cpp

DISTFILES += ../QtUtils
