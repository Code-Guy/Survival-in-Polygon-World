QT += \
    phonon \
    sql

HEADERS += \
    startdialog.h \
    setdialog.h \
    scenetitleanim.h \
    scenemanager.h \
    player.h \
    pausewidget.h \
    nextscenedialog.h \
    menuwidget.h \
    leveldialog.h \
    item.h \
    globals.h \
    enemy.h \
    constants.h \
    bullet.h \
    boomanim.h \
    helpwidget.h \
    statswidget.h \
    gamewidget.h \
    winwidget.h \
    losewidget.h

SOURCES += \
    startdialog.cpp \
    setdialog.cpp \
    scenetitleanim.cpp \
    scenemanager.cpp \
    player.cpp \
    pausewidget.cpp \
    nextscenedialog.cpp \
    menuwidget.cpp \
    main.cpp \
    leveldialog.cpp \
    item.cpp \
    globals.cpp \
    enemy.cpp \
    bullet.cpp \
    boomanim.cpp \
    helpwidget.cpp \
    statswidget.cpp \
    gamewidget.cpp \
    winwidget.cpp \
    losewidget.cpp

FORMS += \
    startdialog.ui \
    setdialog.ui \
    pausewidget.ui \
    nextscenedialog.ui \
    menuwidget.ui \
    leveldialog.ui \
    helpwidget.ui \
    statswidget.ui \
    gamewidget.ui \
    winwidget.ui \
    losewidget.ui

RESOURCES += \
    sources.qrc

OTHER_FILES += \
    logo.rc

RC_FILE = \
    logo.rc
