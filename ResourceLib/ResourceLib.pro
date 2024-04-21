QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    resourcelib.cpp \
    feature.cpp \
    resource.cpp

HEADERS += \
    resourcelib.h \
    feature.h \
    resource.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

# Depend on LockLib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LockLib/release/ -lLockLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LockLib/debug/ -lLockLib
else:unix: LIBS += -L$$OUT_PWD/../LockLib/ -lLockLib

INCLUDEPATH += $$PWD/../LockLib
DEPENDPATH += $$PWD/../LockLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LockLib/release/libLockLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LockLib/debug/libLockLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LockLib/release/LockLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LockLib/debug/LockLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../LockLib/libLockLib.a
