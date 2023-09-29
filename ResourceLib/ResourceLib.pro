TEMPLATE = lib
DEFINES += RESOURCELIB_LIBRARY

CONFIG += c++11
QT += core

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    feature.cpp \
    resource.cpp

HEADERS += \
    ResourceLib_global.h \
    feature.h \
    resource.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LockLib/release/ -lLockLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LockLib/debug/ -lLockLib
else:unix: LIBS += -L$$OUT_PWD/../LockLib/ -lLockLib

INCLUDEPATH += $$PWD/../LockLib
DEPENDPATH += $$PWD/../LockLib
