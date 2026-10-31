// KMinesMainWindow — game actions, status bar, dialogs
// Reconstructed from FUN_00121b50 (score/high-score dialogs), FUN_0011c300 /
// FUN_0011f320 / FUN_0011ff10 (messages), KGameDifficultyAction / KGameClock /
// KGameScore / KGameHighScoreDialog / KGameRulesDialog / KGameRenderer PLT sites.
// Confidence: structure B, names C.
//
// Proven facts:
//   M1 difficulty: KGameDifficultyAction::addStandardLevelRange(30, 50)  (0x12f8c)
//      → Easy / Medium / Hard (names not in binary; provided by libKDEGames6)
//      + addStandardLevel(KGCustomLevel=1000, i18n("Custom"))           (0x12fb5)
//      + setLevelIcon("kmines")                                         (0x12fd0)
//   M2 KGameClock ClockType = CountUp (ctor arg 0 at 0x12f34; enum proven in
//      libKDEGames6 strings: "CountUp","Pauseable")
//   M3 score dialog title: i18n("Time: %1").arg(seconds)                (0x12b8e/0x12c1a)
//   M4 high-score dialog: KGameHighScoreDialog::addScoreDialog(score)    (0x12c9e)
//   M5 rules dialog: KGameRulesDialog, rendered "logo" sprite, 320x…     (0x1305f-0x130a6)
//   M6 pause: meta-string slot pauseGame(bool)/paused(bool); "Game is paused."
//      text proven; exact clock call not import-visible (C)
//   M7 clock display strings: "Time: 00:00" (0x22198), "Mines: 0/0" (0x221c3)
//   M8 popup texts: "You have lost." (0x222b6),
//      "Congratulations! You have won!" (0x22b98), "Game is paused." (0x222c5),
//      "Reset the Game?" (0x221b3)
//   M9 theme name for KGameRenderer: "kmines" (0x1a703); main window sprite
//      "mainWidget"; renderer sprite keys "view-refresh", "settings", "themes"

#include "kminesmainwindow.h"

KMinesMainWindow::KMinesMainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
    auto *view  = new KMinesView(this);
    auto *scene = new KMinesScene(this);
    view->setScene(scene);                                   // FUN_001202b0/00120550 wiring

    m_difficulty = new KGameDifficultyAction(this);
    m_difficulty->addStandardLevelRange(30, 50);             // M1
    m_difficulty->addStandardLevel(KGCustomLevel, i18n("Custom"));
    m_difficulty->setLevelIcon(QStringLiteral("kmines"));

    m_clock = scene->clock();                                // CountUp (M2)
    connect(m_clock, &KGameClock::timeChanged, this, &KMinesMainWindow::advanceTime);
    connect(m_clock, &KGameClock::sig_paused,  this, &KMinesMainWindow::paused);

    m_score = scene->score();
    connect(scene, &KMinesScene::gameOver, this, &KMinesMainWindow::onGameOver);
    connect(scene, &KMinesScene::won,      this, &KMinesMainWindow::onGameOver);
    connect(scene, &KMinesScene::firstClickDone, this, &KMinesMainWindow::onFirstClick);

    setupActions();           // KStandardAction: New Game (KStandardGameAction::newGame),
                              // KStandardGameAction::pause, highscores, settings, help
    setupGUI();

    readHighScores();         // FUN_0011d430
    loadHighScores();         // FUN_0011d580 — scores live in KGameScore/KConfig
}

void KMinesMainWindow::advanceTime()
{
    // status bar: i18n("Time: %1").arg(clock->timeString())   (M7/M3)
}

void KMinesMainWindow::onFirstClick()
{
    // status bar: i18n("Mines: %1/%2").arg(flagged).arg(total)
}

void KMinesMainWindow::onGameOver(bool lost)
{
    // M8: KMessageWidget / status: lost -> i18n("You have lost.")
    //                       won  -> i18n("Congratulations! You have won!")
    // M3/M4: KGameScoreDialog title i18n("Time: %1").arg(score);
    KGameHighScoreDialog::addScoreDialog(m_score);            // M4
    saveHighScores();                                         // FUN_0011d370
}

void KMinesMainWindow::newGame()
{
    // KStandardGameAction::newGame -> scene->newField()
    // with "Reset the Game?" confirmation when AllowKminesReset semantics apply (M8)
}

void KMinesMainWindow::pauseGame()
{
    m_clock->pause();                                          // M6 (0x136c2)
    // status: i18n("Game is paused.")                         // M8
}

void KMinesMainWindow::showHighscores()
{
    KGameHighScoreDialog dlg(this);
    dlg.addScoreDialog(m_score);
    dlg.exec();
}

void KMinesMainWindow::configureSettings()
{
    KConfigDialog dlg(this, QStringLiteral("settings"), Settings::self());
    dlg.addPage(new GeneralOptsConfig(...));                   // FUN_00111a60
    dlg.addPage(new CustomGameConfig(...));                    // FUN_001128c0
    dlg.show();
}

void KMinesMainWindow::showRules()
{
    KGameRulesDialog dlg(this);
    dlg.setIconPixmap(KGameRenderer(QStringLiteral("kmines"))
                      .renderPixmap(QStringLiteral("logo"), 320, ...)); // M5
    dlg.exec();
}
