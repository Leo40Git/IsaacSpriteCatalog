#include "steam_api_ext.h"
#include "utils.h"
#include "mainwindow.h"

#include <steam/steam_api.h>

#include <QObject>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

bool SteamAPI_IsAvailable = false;
bool IsRepentanceInstalled = false;

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

    if (SteamAPI_Init()) {
        SteamAPI_IsAvailable = true;
        QObject::connect(&a, &QCoreApplication::aboutToQuit, &a, SteamAPI_Shutdown);
        ISteamApps *apps = SteamApps();
        if (!apps->BIsDlcInstalled(DLCID_AFTERBIRTH_PLUS)) {
            QMessageBox::warning(nullptr,
                                 QMessageBox::tr("Afterbirth+ is not installed"),
                                 QMessageBox::tr("The Afterbirth+ DLC is not installed!\n"
                                                 "This DLC is required for mod support."));
        }
        IsRepentanceInstalled = apps->BIsDlcInstalled(DLCID_REPENTANCE);
    } else {
        QMessageBox::warning(nullptr,
                             QMessageBox::tr("Failed to initialize Steam API"),
                             QMessageBox::tr("Could not initialize Steam API!\n"
                                            "Please ensure that:\n"
                                            " - the Steam client is running\n"
                                            " - it is logged into an account that owns The Binding of Isaac: Rebirth\n"
                                            "\n"
                                            "The appliaction will still run, but all Workshop functionality will be disabled."));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
