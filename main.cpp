#include <QApplication>
#include <QSettings>
#include <QIcon>

#include "analogclock.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    app.setOrganizationName("Post Meridian");
    app.setApplicationName("Analog Clock Widget");
    app.setOrganizationDomain("pmify.com");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    app.setWindowIcon(QIcon(":/img/clock.ico"));

    AnalogClock clock;
    clock.show();

    return app.exec();
}
