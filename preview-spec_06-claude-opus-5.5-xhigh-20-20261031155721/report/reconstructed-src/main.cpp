// Reconstructed main() (0x10fc00). Strings and credits are read from .rodata.
#include "mainwindow.h"

#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KIconTheme>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kmines");

    KAboutData aboutData(QStringLiteral("kmines"), i18n("KMines"),
                         QStringLiteral("4.0.25123"), // UTF-16 literal in .rodata
                         i18n("KMines is a classic minesweeper game"),
                         KAboutLicense::GPL,
                         i18n("(c) 1996-2005, Nicolas Hadacek\n(c) 2001, Mikhail Kourinny\n"
                              "(c) 2006-2007, Mauricio Piacentini\n(c) 2007, Dmitry Suzdalev"),
                         QString(), QStringLiteral("https://apps.kde.org/kmines"));
    aboutData.addAuthor(i18n("Nicolas Hadacek"), i18n("Original author"), QStringLiteral("hadacek@kde.org"));
    aboutData.addAuthor(i18n("Mauricio Piacentini"), i18n("Code refactoring and SVG support. Current maintainer"),
                        QStringLiteral("mauricio@tabuleiro.com"));
    aboutData.addAuthor(i18n("Dmitry Suzdalev"), i18n("Rewrite to use QGraphicsView framework. Current maintainer"),
                        QStringLiteral("dimsuz@gmail.com"));
    aboutData.addCredit(i18n("Andreas Zehender"), i18n("Smiley pixmaps"));
    aboutData.addCredit(i18n("Mikhail Kourinny"), i18n("Solver/Adviser"));
    aboutData.addCredit(i18n("Thomas Capricelli"), i18n("Magic reveal mode"));
    aboutData.addCredit(i18n("Brian Croom"), i18n("Port to use KGameRenderer"));
    KAboutData::setApplicationData(aboutData);
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kmines")));

    KCrash::initialize();
    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KDBusService service;
    auto *window = new KMinesMainWindow;
    window->show();
    return app.exec();
}
