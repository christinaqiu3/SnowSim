QT += core widgets openglwidgets

TARGET = MicroMaya
TEMPLATE = app
CONFIG += console
CONFIG += c++1z
win32 {
    LIBS += -lopengl32
#    LIBS += -lglut32
    LIBS += -lglu32
    # LIBS += -LC:/opt/USD/lib
    # LIBS += -lusd -lpxr_usd -lpxr_base -lpxr_usdGeom
    # LIBS += -LC:/Python311/libs -lpython311
    # LIBS += -LC:/Python313/libs -lpython313
}
CONFIG += warn_on
CONFIG += debug

INCLUDEPATH += include
#INCLUDEPATH += C:/opt/USD/include
#INCLUDEPATH += C:/opt/USD/include/pxr
# INCLUDEPATH += C:/Users/Christina Qiu/AppData/Local/Programs/Python/Python311/include
# INCLUDEPATH += C:/Users/Christina Qiu/AppData/Local/Programs/Python/Python313/include


include(src/src.pri)

FORMS += forms/mainwindow.ui \
    forms/cameracontrolshelp.ui

RESOURCES += glsl.qrc

*-clang*|*-g++* {
    message("Enabling additional warnings")
    CONFIG -= warn_on
    QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -Winit-self
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer
}
linux-clang*|linux-g++*|macx-clang*|macx-g++* {
    message("Enabling stack protector")
    QMAKE_CXXFLAGS += -fstack-protector-all
}

# FOR LINUX & MAC USERS INTERESTED IN ADDITIONAL BUILD TOOLS
# ----------------------------------------------------------
# This conditional exists to enable Address Sanitizer (ASAN) during
# the automated build. ASAN is a compiled-in tool which checks for
# memory errors (like Valgrind). You may enable it for yourself;
# check the hidden `.build.sh` file for info. But be aware: ASAN may
# trigger a lot of false-positive leak warnings for the Qt libraries.
# (See `.run.sh` for how to disable leak checking.)
address_sanitizer {
    message("Enabling Address Sanitizer")
    QMAKE_CXXFLAGS += -fsanitize=address
    QMAKE_LFLAGS += -fsanitize=address
}

HEADERS += \
src/MPM/mpmparticle.h \
src/MPM/mpmsolver.h

SOURCES += \
src/MPM/mpmparticle.cpp \
src/MPM/mpmsolver.cpp \
src/mygl.cpp
