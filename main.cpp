#include "smssendermainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("itQuasar");
    QApplication::setOrganizationDomain("itquasar.ru");
    QApplication::setApplicationName("SMSSender");
    QApplication::setApplicationVersion("1.0");

    QTranslator qtTranslator;
    qtTranslator.load(QCoreApplication::applicationDirPath() + "/qt_" + QLocale::system().name() + ".qm");

    QTranslator qtBaseTranslator;
    qtBaseTranslator.load(QCoreApplication::applicationDirPath() + "/qtbase_" + QLocale::system().name() + ".qm");


    QTranslator appTranslator;
    appTranslator.load(QCoreApplication::applicationDirPath() + "/smssender_" + QLocale::system().name() + ".qm");

    a.installTranslator(&qtTranslator);
    a.installTranslator(&qtBaseTranslator);
    a.installTranslator(&appTranslator);

    SMSSenderMainWindow w;

    if (a.arguments().length() > 0 && (a.arguments().contains("--autoStart") || a.arguments().contains("-s")))
        w.start();

    w.show();
    
    return a.exec();
}
