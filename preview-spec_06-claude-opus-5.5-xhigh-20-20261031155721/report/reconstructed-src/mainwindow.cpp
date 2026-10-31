// Reconstructed KMinesMainWindow (constructor 0x11a610 and slots).
#include "mainwindow.h"
#include "minefielditem.h"
#include "scene.h"
#include "settings.h"
#include "ui_customgame.h"
#include "ui_generalopts.h"

#include <KConfigDialog>
#include <KGameClock>
#include <KGameDifficulty>
#include <KGameHighScoreDialog>
#include <KGameStandardAction>
#include <KGameThemeSelector>
#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardAction>
#include <KToggleAction>
#include <QGraphicsView>
#include <QLabel>
#include <QScreen>
#include <QStatusBar>

class KMinesView : public QGraphicsView
{
public:
    KMinesView(KMinesScene *scene, QWidget *parent)
        : QGraphicsView(scene, parent), m_scene(scene) {}
private:
    void resizeEvent(QResizeEvent *ev) override
    {
        m_scene->resizeScene(ev->size().width(), ev->size().height());
    }
    KMinesScene *m_scene;
};

KMinesMainWindow::KMinesMainWindow()
{
    m_minesDisplay = new QLabel;
    m_timeDisplay = new QLabel;
    m_scene = new KMinesScene(this);
    connect(m_scene, &KMinesScene::minesCountChanged, this, &KMinesMainWindow::onMinesCountChanged);
    connect(m_scene, &KMinesScene::gameOver, this, &KMinesMainWindow::onGameOver);
    connect(m_scene, &KMinesScene::firstClickDone, this, &KMinesMainWindow::onFirstClick);

    m_view = new KMinesView(m_scene, this);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameStyle(QFrame::NoFrame);
    m_view->setOptimizationFlags(QGraphicsView::DontClipPainter | QGraphicsView::DontSavePainterState);

    m_gameClock = new KGameClock(this, KGameClock::FlexibleHourMinSec);
    connect(m_gameClock, &KGameClock::timeChanged, this, &KMinesMainWindow::advanceTime);

    m_minesDisplay->setText(i18n("Mines: 0/0"));
    m_timeDisplay->setText(i18n("Time: 00:00"));
    statusBar()->insertPermanentWidget(0, m_minesDisplay);
    statusBar()->insertPermanentWidget(1, m_timeDisplay);
    setCentralWidget(m_view);

    KGameStandardAction::gameNew(this, &KMinesMainWindow::newGame, actionCollection());
    KGameStandardAction::highscores(this, &KMinesMainWindow::showHighscores, actionCollection());
    KGameStandardAction::quit(this, &QWidget::close, actionCollection());
    KStandardAction::preferences(this, &KMinesMainWindow::configureSettings, actionCollection());
    m_actionPause = KGameStandardAction::pause(this, &KMinesMainWindow::pauseGame, actionCollection());

    // Levels: Easy(30) .. Hard(50) + Custom (value 1000, key "Custom").
    KGameDifficulty::global()->addStandardLevelRange(KGameDifficultyLevel::Easy, KGameDifficultyLevel::Hard);
    KGameDifficulty::global()->addLevel(new KGameDifficultyLevel(1000, "Custom", i18n("Custom")));
    KGameDifficultyGUI::init(this);
    connect(KGameDifficulty::global(), &KGameDifficulty::currentLevelChanged, this, &KMinesMainWindow::newGame);

    // 40% of the available screen.
    setupGUI(screen()->availableGeometry().size() * 0.4);
    newGame();
}

// 0x1141a0
void KMinesMainWindow::onMinesCountChanged(int count)
{
    m_minesDisplay->setText(i18n("Mines: %1/%2", count, m_scene->totalMines()));
}

// 0x116660
void KMinesMainWindow::newGame()
{
    qCDebug(KMINES_LOG) << "Inside game";
    m_gameClock->restart();
    m_gameClock->pause(); // starts only on the first click
    if (m_actionPause->isChecked()) {
        m_scene->setGamePaused(false);
        m_actionPause->setChecked(false);
    }
    m_actionPause->setEnabled(false);
    KGameDifficulty::global()->setGameRunning(false);

    switch (KGameDifficulty::globalLevel()) {
    case KGameDifficultyLevel::Easy:   m_scene->startNewGame(9, 9, 10); break;
    case KGameDifficultyLevel::Medium: m_scene->startNewGame(16, 16, 40); break;
    case KGameDifficultyLevel::Hard:   m_scene->startNewGame(16, 30, 99); break;
    case KGameDifficultyLevel::Custom:
        m_scene->startNewGame(Settings::customHeight(), Settings::customWidth(), Settings::customMines());
        break;
    default: break;
    }
    m_timeDisplay->setText(i18n("Time: 00:00"));
}

// 0x114110
void KMinesMainWindow::onFirstClick()
{
    m_actionPause->setEnabled(true);
    m_gameClock->resume();
    KGameDifficulty::global()->setGameRunning(true);
}

// 0x114400
void KMinesMainWindow::advanceTime(const QString &timeStr)
{
    m_timeDisplay->setText(i18n("Time: %1", timeStr));
}

// 0x117ee0
void KMinesMainWindow::onGameOver(bool won)
{
    m_gameClock->pause();
    m_actionPause->setEnabled(false);
    KGameDifficulty::global()->setGameRunning(false);

    if (!won) {
        if (Settings::allowKminesReset()) {
            const int answer = KMessageBox::questionTwoActions(
                this, i18n("Reset the Game?"), QString(),
                KGuiItem(i18nc("@action;button", "Reset"), QStringLiteral("view-refresh")),
                KStandardGuiItem::cancel());
            if (answer == KMessageBox::PrimaryAction) {
                m_scene->fieldItem()->resetField(); // same mines, all cells Released
                m_scene->messageItem()->forceHide();
                m_gameClock->resume();               // time continues
                m_actionPause->setEnabled(true);
                m_scene->m_gameCanScore = !Settings::disableScoreOnReset();
            }
        }
        return;
    }

    if (m_scene->m_gameCanScore) {
        QPointer<KGameHighScoreDialog> dlg =
            new KGameHighScoreDialog(KGameHighScoreDialog::Name | KGameHighScoreDialog::Time, this);
        dlg->initFromDifficulty(KGameDifficulty::global());
        dlg->hideField(KGameHighScoreDialog::Score);
        KGameHighScoreDialog::FieldInfo scoreInfo;
        scoreInfo[KGameHighScoreDialog::Score].setNum(m_gameClock->seconds());
        scoreInfo[KGameHighScoreDialog::Time] = m_gameClock->timeString();
        if (dlg->addScore(scoreInfo, KGameHighScoreDialog::LessIsMore) != 0)
            dlg->exec();
        delete dlg;
    }
}

// 0x1142d0
void KMinesMainWindow::showHighscores()
{
    QPointer<KGameHighScoreDialog> dlg =
        new KGameHighScoreDialog(KGameHighScoreDialog::Name | KGameHighScoreDialog::Time, this);
    dlg->initFromDifficulty(KGameDifficulty::global());
    dlg->hideField(KGameHighScoreDialog::Score);
    dlg->exec();
    delete dlg;
}

// 0x114150
void KMinesMainWindow::pauseGame(bool paused)
{
    m_scene->setGamePaused(paused);
    if (paused)
        m_gameClock->pause();
    else
        m_gameClock->resume();
}

// 0x114570
void KMinesMainWindow::configureSettings()
{
    if (KConfigDialog::showDialog(QStringLiteral("settings")))
        return;
    auto *dialog = new KConfigDialog(this, QStringLiteral("settings"), Settings::self());
    dialog->addPage(new GeneralOptsConfig(dialog), i18nc("@title:tab", "General"),
                    QStringLiteral("games-config-options"));
    dialog->addPage(new KGameThemeSelector(m_scene->renderer().themeProvider()),
                    i18nc("@title:tab", "Theme"), QStringLiteral("games-config-theme"));
    dialog->addPage(new CustomGameConfig(dialog), i18nc("@title:tab", "Custom Game"),
                    QStringLiteral("games-config-custom"));
    connect(m_scene->renderer().themeProvider(), &KGameThemeProvider::currentThemeChanged,
            this, &KMinesMainWindow::loadSettings);
    connect(dialog, &KConfigDialog::settingsChanged, this, &KMinesMainWindow::loadSettings);
    dialog->show();
}

// 0x1144f0
void KMinesMainWindow::loadSettings()
{
    m_view->resetCachedContent();
    m_scene->resizeScene(int(m_scene->sceneRect().width()), int(m_scene->sceneRect().height()));
}

// CustomGameConfig::updateMaxMines (0x113fc0 / 0x114010)
void CustomGameConfig::updateMaxMines()
{
    const int max = ui.kcfg_CustomWidth->value() * ui.kcfg_CustomHeight->value() - 10;
    ui.kcfg_CustomMines->setMaximum(max > 0 ? max : 1);
}
