QT += widgets svg

HEADERS       = analogclock.h
SOURCES       = analogclock.cpp \
                main.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS = clock.ico

RC_FILE = myapp.rc

DISTFILES += \
    myapp.rc
