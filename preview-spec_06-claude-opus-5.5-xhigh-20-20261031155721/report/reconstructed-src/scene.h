// Reconstructed KMinesScene (RTTI "11KMinesScene").
// Signals (moc): minesCountChanged(int), gameOver(bool), firstClickDone().
#ifndef KMINESSCENE_H
#define KMINESSCENE_H

#include <KGameRenderer>
#include <QGraphicsScene>

class MineFieldItem;
class KGamePopupItem;

class KMinesScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit KMinesScene(QObject *parent);

    void resizeScene(int width, int height);            // 0x120020
    void startNewGame(int rows, int cols, int numMines); // 0x1206d0
    void setGamePaused(bool paused);                    // 0x11f320
    int totalMines() const;
    KGameRenderer &renderer() { return m_renderer; }
    // The binary touches these members directly from KMinesMainWindow::onGameOver.
    MineFieldItem *fieldItem() const { return m_fieldItem; }
    KGamePopupItem *messageItem() const { return m_messageItem; }

    bool m_gameCanScore;  // +0x10 (written only by KMinesMainWindow::onGameOver)

Q_SIGNALS:
    void minesCountChanged(int count);
    void gameOver(bool won);
    void firstClickDone();

private Q_SLOTS:
    void onGameOver(bool won); // 0x11c300

private:
    KGameRenderer m_renderer;          // +0x18 (theme provider config key "Theme", default "default")
    MineFieldItem *m_fieldItem;        // +0x30
    KGamePopupItem *m_messageItem;     // +0x38 (win/lose, timeout 4000 ms, opacity 0.9)
    KGamePopupItem *m_gamePausedMessageItem; // +0x40 (no timeout, not hidden on click)
};

#endif
