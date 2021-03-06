QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    initwindow.cpp \
    main.cpp

HEADERS += \
    initwindow.h \
    isaac.h

FORMS += \
    initwindow.ui

TRANSLATIONS += \
    IsaacSpriteCatalog_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Steamworks SDK (Steam API)
CONFIG += file_copies
COPIES += copySteamApiRTLib
copySteamApiRTLib.path = $$OUT_PWD

STEAMWORKS_SDK_ROOT = C:/SteamworksSDK
INCLUDEPATH += $${STEAMWORKS_SDK_ROOT}/public
if (win32) {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    contains(QT_ARCH, x86_64) {
        # Windows, 64-bit
        message("Windows 64-bit - using steam_api64.dll")
        LIBS += -L"$${STEAMWORKS_SDK_ROOT}/redistributable_bin/win64" -lsteam_api64
        copySteamApiRTLib.files += $${STEAMWORKS_SDK_ROOT}/redistributable_bin/win64/steam_api64.dll
    } else {
        # Windows, 32-bit
        message("Windows 32-bit - using steam_api.dll")
        LIBS += -L"$${STEAMWORKS_SDK_ROOT}/redistributable_bin" -lsteam_api
        copySteamApiRTLib.files += $${STEAMWORKS_SDK_ROOT}/redistributable_bin/steam_api.dll
    }
} else {
    if (macx) {
        # Mac OSX
        message("Mac OSX - using libsteam_api.dylib")
        LIBS += -L"$${STEAMWORKS_SDK_ROOT}/redistributable_bin/macosx" -lsteam_api
        copySteamApiRTLib.files += $${STEAMWORKS_SDK_ROOT}/redistributable_bin/osx/libsteam_api.dylib)
    } else {
        if (unix) {
            contains(QT_ARCH, x86_64) {
                # Unix, 64-bit
                message("Unix 64-bit - using libsteam_api.so")
                LIBS += -L"$${STEAMWORKS_SDK_ROOT}/redistributable_bin/linux64" -lsteam_api
                copySteamApiRTLib.files += $${STEAMWORKS_SDK_ROOT}/redistributable_bin/linux64/libsteam_api.so
            } else {
                # Unix, 32-bit
                message("Unix 32-bit - using libsteam_api.so")
                LIBS += -L"$${STEAMWORKS_SDK_ROOT}/redistributable_bin/linux32" -lsteam_api
                copySteamApiRTLib.files += $${STEAMWORKS_SDK_ROOT}/redistributable_bin/linux32/libsteam_api.so
            }
        } else {
            error("Steam API isn't supported on this system!")
        }
    }
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
