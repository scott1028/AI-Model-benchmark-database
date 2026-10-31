// Reconstructed KMinesMainWindow (RTTI "16KMinesMainWindow").
// Slot indices from qt_static_metacall 0x118530:
//  0 onMinesCountChanged(int) 1 newGame() 2 onGameOver(bool) 3 advanceTime(QString)
//  4 onFirstClick() 5 showHighscores() 6 configureSettings() 7 pauseGame(bool)
//  8 loadSettings()
#ifndef KMINESMAINWINDOW_H
#define KMINESMAINWINDOW_H

#include <KXmlGuiWindow>
#include <QPointer>

class KMinesScene;
class KMinesView;
class KGameClock;
class KToggleAction;
class QLabel;

class KMinesMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    KMinesMainWindow();

private Q_SLOTS:
    void onMinesCountChanged(int count); // 0x1141a0
    void newGame();                      // 0x116660
    void onGameOver(bool won);           // 0x117ee0
    void advanceTime(const QString &t);  // 0x114400
    void onFirstClick();                 // 0x114110
    void showHighscores();               // 0x1142d0
    void configureSettings();            // 0x114570
    void pauseGame(bool paused);         // 0x114150
    void loadSettings();                 // 0x1144f0

private:
    KMinesScene *m_scene;          // +0x40
    KMinesView *m_view;            // +0x48
    KGameClock *m_gameClock;       // +0x50 (KGameClock::FlexibleHourMinSec)
    KToggleAction *m_actionPause;  // +0x58
    QPointer<QLabel> m_minesDisplay; // +0x60/+0x68
    QPointer<QLabel> m_timeDisplay;  // +0x70/+0x78
};

#endif
