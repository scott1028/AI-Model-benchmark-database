// Reconstructed MineFieldItem (RTTI "13MineFieldItem").
// Signals from moc string table: flaggedMinesCountChanged(int)=0,
// firstClickDone()=1, gameOver(bool)=2 (QMetaObject::activate index).
#ifndef MINEFIELDITEM_H
#define MINEFIELDITEM_H

#include <QGraphicsObject>
#include <QList>
#include <QPair>

class CellItem;
class BorderItem;
class KGameRenderer;
class QGraphicsSceneMouseEvent;
using FieldPos = QPair<int, int>; // (row, col)

class MineFieldItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MineFieldItem(KGameRenderer *renderer);

    void initField(int numRows, int numCols, int numMines); // inlined in 0x1206d0
    void resetField();                                      // inlined in 0x117ee0 (KMines reset)
    void resizeToFitInRect(const QRectF &rect);             // inlined in 0x120020
    QRectF boundingRect() const override;                   // 0x1108f0
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override {}

    int minesCount() const { return m_minesCount; }
    bool isGameOver() const { return m_gameOver; }
    void setGamePaused(bool paused) { setVisible(!paused); }

Q_SIGNALS:
    void flaggedMinesCountChanged(int);
    void firstClickDone();
    void gameOver(bool won);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;   // 0x11f660
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override; // 0x11fa60
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;    // 0x11eb40
    bool sceneEvent(QEvent *) override;                          // 0x11ee50 (touch)

    void generateField(int clickedIdx);                          // 0x11a0b0
    void adjustItemPositions();                                  // 0x1196e0
    void revealEmptySpace(int row, int col);                     // 0x118690
    bool onItemRevealed(int row, int col);                       // 0x1197f0
    bool onItemRevealed(CellItem *item);                         // 0x119b00
    void handleFlag(CellItem *item);                             // 0x119a50
    QList<FieldPos> adjacentRowColsFor(int row, int col) const;  // 0x116240
    QList<CellItem *> adjacentItemsFor(int row, int col) const;  // 0x119c40
    CellItem *itemAt(int row, int col) const { return m_cells.at(row * m_numCols + col); }
    void revealAround(int row, int col);                         // chord helper (inlined)

    QList<CellItem *> m_cells;      // +0x20/+0x28/+0x30
    QList<BorderItem *> m_borders;  // +0x38/+0x40/+0x48
    int m_cellSize = 1;             // +0x50
    int m_numRows = 1;              // +0x54
    int m_numCols = 1;              // +0x58
    int m_minesCount = 0;           // +0x5c
    int m_flaggedMinesCount = 0;    // +0x60
    FieldPos m_touchPos{-1, -1};    // +0x64
    qint64 m_touchStartMs = 0;      // +0x70
    FieldPos m_leftButtonPos{-1, -1}; // +0x78
    FieldPos m_midButtonPos{-1, -1};  // +0x80
    bool m_firstClick = true;       // +0x88
    bool m_gameOver = false;        // +0x89
    bool m_emulatingMidButton = false; // +0x8a
    int m_numUnrevealed = 0;        // +0x8c
    KGameRenderer *m_renderer;      // +0x90
};

#endif
