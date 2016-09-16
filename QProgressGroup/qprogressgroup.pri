HEADERS += \
    $$PWD/progressbaradapter.h \
	$$PWD/qprogressgroup.h \
    $$PWD/systemtrayprogressadapter.h

SOURCES += \
    $$PWD/progressbaradapter.cpp \
	$$PWD/qprogressgroup.cpp \
    $$PWD/systemtrayprogressadapter.cpp

win32 {
	QT *= winextras

	HEADERS += \
		$$PWD/wintaskbarprogressadapter.h

	SOURCES += \
		$$PWD/wintaskbarprogressadapter.cpp
}

INCLUDEPATH += $$PWD
