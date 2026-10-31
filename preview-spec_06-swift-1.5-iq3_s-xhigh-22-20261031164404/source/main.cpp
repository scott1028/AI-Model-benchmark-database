// main.cpp — application bootstrap
// Reconstructed from FUN_00122d80 (main). Confidence: A (decompiled fully).

#include <KCrash>
#include <KDEApplication>
#include <KDBusService>
#include "kminesmainwindow.h"

int main(int argc, char **argv)
{
    KCrash::initialize();
    KDEApplication app(argc, argv);

    KMinesMainWindow window;
    window.show();

    KDBusService registration;         // unique D-Bus service org.kde.kmines
    return app.exec();
}
