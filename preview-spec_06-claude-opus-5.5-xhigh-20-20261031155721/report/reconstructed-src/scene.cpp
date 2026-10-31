// Reconstructed KMinesScene. The constructor is inlined into
// KMinesMainWindow::KMinesMainWindow (0x11a610).
#include "scene.h"
#include "minefielditem.h"

#include <KGamePopupItem>
#include <KGameThemeProvider>
#include <KLocalizedString>

static KGameThemeProvider *createProvider()
{
    auto *provider = new KGameThemeProvider("Theme");          // rodata 0x122104
    provider->discoverThemes(QStringLiteral("themes"),          // QStandardPaths subdir
                             QStringLiteral("default"));        // default theme id
    return provider;
}

KMinesScene::KMinesScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_renderer(createProvider())
{
    setItemIndexMethod(NoIndex);
    m_fieldItem = new MineFieldItem(&m_renderer);
    connect(m_fieldItem, &MineFieldItem::flaggedMinesCountChanged, this, &KMinesScene::minesCountChanged);
    connect(m_fieldItem, &MineFieldItem::firstClickDone, this, &KMinesScene::firstClickDone);
    connect(m_fieldItem, &MineFieldItem::gameOver, this, &KMinesScene::onGameOver);
    connect(m_fieldItem, &MineFieldItem::gameOver, this, &KMinesScene::gameOver);
    addItem(m_fieldItem);

    m_messageItem = new KGamePopupItem;
    m_messageItem->setMessageOpacity(0.9);
    m_messageItem->setMessageTimeout(4000);
    addItem(m_messageItem);

    m_gamePausedMessageItem = new KGamePopupItem;
    m_gamePausedMessageItem->setMessageOpacity(0.9);
    m_gamePausedMessageItem->setMessageTimeout(0);
    m_gamePausedMessageItem->setHideOnMouseClick(false);
    addItem(m_gamePausedMessageItem);

    setBackgroundBrush(m_renderer.spritePixmap(QStringLiteral("mainWidget"), sceneRect().size().toSize()));
}

// 0x120020
void KMinesScene::resizeScene(int width, int height)
{
    setSceneRect(0, 0, width, height);
    setBackgroundBrush(m_renderer.spritePixmap(QStringLiteral("mainWidget"), QSize(width, height)));
    m_fieldItem->resizeToFitInRect(sceneRect());
    // Center the field and both popups.
    m_fieldItem->setPos(sceneRect().width() / 2 - m_fieldItem->boundingRect().width() / 2,
                        sceneRect().height() / 2 - m_fieldItem->boundingRect().height() / 2);
    m_gamePausedMessageItem->setPos(sceneRect().width() / 2 - m_gamePausedMessageItem->boundingRect().width() / 2,
                                    sceneRect().height() / 2 - m_gamePausedMessageItem->boundingRect().height() / 2);
    m_messageItem->setPos(sceneRect().width() / 2 - m_messageItem->boundingRect().width() / 2,
                          sceneRect().height() / 2 - m_messageItem->boundingRect().height() / 2);
}

// 0x1206d0
void KMinesScene::startNewGame(int rows, int cols, int numMines)
{
    m_messageItem->forceHide();
    m_fieldItem->initField(rows, cols, numMines);
    resizeScene(int(sceneRect().width()), int(sceneRect().height()));
}

int KMinesScene::totalMines() const
{
    return m_fieldItem->minesCount();
}

// 0x11c300
void KMinesScene::onGameOver(bool won)
{
    if (won)
        m_messageItem->showMessage(i18n("Congratulations! You have won!"), KGamePopupItem::Center);
    else
        m_messageItem->showMessage(i18n("You have lost."), KGamePopupItem::Center);
}

// 0x11f320
void KMinesScene::setGamePaused(bool paused)
{
    m_fieldItem->setGamePaused(paused); // field hidden while paused
    if (paused)
        m_gamePausedMessageItem->showMessage(i18n("Game is paused."), KGamePopupItem::Center);
    else
        m_gamePausedMessageItem->forceHide();
}
