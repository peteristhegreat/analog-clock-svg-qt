#include <QApplication>
#include <QSettings>

#include "analogclock.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    app.setOrganizationName("Post Meridian");
    app.setApplicationName("Analog Clock Widget");
    app.setOrganizationDomain("pmify.com");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    AnalogClock clock;
    clock.show();

    return app.exec();
}
