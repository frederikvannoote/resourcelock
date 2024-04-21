QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# LockLib dependency
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

# ResourceLib dependency
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ResourceLib/release/ -lResourceLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ResourceLib/debug/ -lResourceLib
else:unix: LIBS += -L$$OUT_PWD/../ResourceLib/ -lResourceLib

INCLUDEPATH += $$PWD/../ResourceLib
DEPENDPATH += $$PWD/../ResourceLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ResourceLib/release/libResourceLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ResourceLib/debug/libResourceLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ResourceLib/release/ResourceLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ResourceLib/debug/ResourceLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../ResourceLib/libResourceLib.a
