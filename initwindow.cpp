#include "initwindow.h"
#include "ui_initwindow.h"

#include <steam/steam_api.h>
#include "isaac.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

QString Isaac_InstallPath = QString();
bool Isaac_IsRepentanceInstalled = false;

InitWindow::InitWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InitWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // FIXME remove this once we have a main window to open
    setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &InitWindow::doInit);
    timer->setSingleShot(true);
}

InitWindow::~InitWindow()
{
    delete ui;
}

void InitWindow::showEvent(QShowEvent *)
{
    ui->label->setText("...");
    timer->start(100);
}

void InitWindow::log(const QString &text)
{
    ui->pteLog->appendPlainText(text);
}

void InitWindow::doInit()
{
    // throw timer in the garbage
    timer->disconnect();
    timer->deleteLater();
    timer = nullptr;

    ui->label->setText(tr("Initializing Steam API..."));

    // check if steam_appid.txt exists, create it otherwise
    QFile steamAppidFile("steam_appid.txt");
    if (!steamAppidFile.exists()) {
        QString steamAppidStr;
        steamAppidStr.setNum(APPID);

        log(tr("Writing app ID %1 to %2").arg(steamAppidStr, steamAppidFile.fileName()));

        steamAppidFile.open(QFile::WriteOnly);
        steamAppidFile.write(steamAppidStr.toUtf8());
        steamAppidFile.close();
    }

    // SteamAPI_Init() takes a minute to execute, so let's update the UI before that happens
    qApp->processEvents();

    // spin up Steam API
    if (SteamAPI_Init()) {
        log(tr("SteamAPI_Init() returned true"));
        // make sure we shutdown Steam API on our way out
        connect(qApp, &QApplication::aboutToQuit, qApp, SteamAPI_Shutdown);

        ISteamApps *apps = SteamApps();

        // ensure AB+ is installed, otherwise we'd just be wasting everyone's time
        if (!apps->BIsDlcInstalled(DLCID_AFTERBIRTH_PLUS)) {
            QMessageBox::critical(this,
                                  tr("Afterbirth+ is not installed"),
                                  tr("The Afterbirth+ DLC is not installed!\n"
                                     "This DLC is required for mod support."));
            QApplication::exit(1);
        }

        // get Isaac's install path
        QByteArray installPathBuf(512, 0);
        quint32 installPathLen = apps->GetAppInstallDir(APPID, installPathBuf.data(), installPathBuf.size());
        Isaac_InstallPath = QDir::cleanPath(QString::fromLocal8Bit(installPathBuf.data(), installPathLen));

        // check Rep too, while we're here
        Isaac_IsRepentanceInstalled = apps->BIsDlcInstalled(DLCID_REPENTANCE);
    } else {
        log(tr("SteamAPI_Init() returned false"));
        QMessageBox::critical(this,
                              tr("Failed to initialize Steam API"),
                              tr("Could not initialize Steam API!\n"
                                 "Please ensure that:\n"
                                 " - the Steam client is running\n"
                                 " - it is logged into an account that owns The Binding of Isaac: Rebirth"));
        // can't be bothered to support non-Steam copies, sorry not sorry
        QApplication::exit(1);
        return;
    }

    log(tr("Game is installed to \"%1\"").arg(Isaac_InstallPath));
    log(Isaac_IsRepentanceInstalled ? tr("Repentance is installed") : tr("Repentance is NOT installed"));

    ui->label->setText("Extracting resources...");

    // check if resources have already been extracted
    QDir resourcesDir(Isaac_InstallPath);
    if (!resourcesDir.cd("resources")) {
        log(tr("Couldn't find resources directory at \"%1/resources\"").arg(resourcesDir.absolutePath()));
        QMessageBox::critical(this,
                              tr("Failed to extract resources"),
                              tr("The \"resources\" directory doesn't exist!\n"
                                 "Verify the integrity of the game files and try again."));
        QApplication::exit(1);
        return;
    }

    QStringList nameFilters;
    nameFilters << "!(packed)" << "!(scripts)";
    QStringList entries = resourcesDir.entryList(nameFilters, QDir::Dirs);

    if (entries.isEmpty()) {
        QMessageBox::information(this,
                                 tr("Extracting resources"),
                                 tr("doing the thing aaa"));
    }
}
