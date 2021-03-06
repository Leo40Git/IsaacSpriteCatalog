#include <QObject>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "initwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "IsaacSpriteCatalog_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    InitWindow w;
    w.show();
    return a.exec();
}
