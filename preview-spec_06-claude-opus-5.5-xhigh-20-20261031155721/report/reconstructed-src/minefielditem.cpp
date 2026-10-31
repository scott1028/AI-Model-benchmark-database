// Reconstructed MineFieldItem implementation.
// Every function lists the Ghidra pseudocode address it was recovered from.
#include "minefielditem.h"
#include "borderitem.h"
#include "cellitem.h"
#include "settings.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTouchEvent>
#include <chrono>

using namespace KMinesState;

MineFieldItem::MineFieldItem(KGameRenderer *renderer)
    : m_renderer(renderer)
{
    // Inlined in KMinesMainWindow ctor (0x11a610): defaults rows=cols=cellSize=1,
    // all saved positions -1, accept touch, ItemIsFocusable-like flag 0x400.
    setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
    setAcceptTouchEvents(true);
}

// Inlined at the start of KMinesScene::startNewGame (0x1206d0).
void MineFieldItem::initField(int numRows, int numCols, int numMines)
{
    m_firstClick = true;
    m_gameOver = false;
    // Mines are clamped so the first click and its 8 neighbours fit.
    if (numMines >= numRows * numCols - 10)
        numMines = numRows * numCols - 10;

    const int newSize = numRows * numCols;
    const int newBorderSize = (numCols + 2) * 2 + (numRows + 2) * 2 - 4;
    // Shrinking: surplus CellItem/BorderItem objects are removed from the
    // scene and deleted. Growing: new items are created (see below).
    while (m_cells.size() > newSize) {
        CellItem *c = m_cells.takeLast();
        scene()->removeItem(c);
        delete c;
    }
    while (m_borders.size() > newBorderSize) {
        BorderItem *b = m_borders.takeLast();
        scene()->removeItem(b);
        delete b;
    }

    m_numUnrevealed = newSize;
    m_numRows = numRows;
    m_numCols = numCols;
    m_leftButtonPos = {-1, -1};
    m_midButtonPos = {-1, -1};
    m_minesCount = numMines;

    for (int i = 0; i < newSize; ++i) {
        if (i < m_cells.size())
            m_cells[i]->reset();
        else
            m_cells.append(new CellItem(m_renderer, this));
    }
    while (m_borders.size() < newBorderSize)
        m_borders.append(new BorderItem(m_renderer, this));

    // Border ring around a (rows+2) x (cols+2) grid.
    int idx = 0;
    for (int row = 0; row < numRows + 2; ++row) {
        for (int col = 0; col < numCols + 2; ++col) {
            BorderElement type;
            if (row == 0 && col == 0) type = BorderCornerNW;
            else if (row == 0 && col == numCols + 1) type = BorderCornerNE;
            else if (row == 0) type = BorderNorth;
            else if (row == numRows + 1 && col == 0) type = BorderCornerSW;
            else if (row == numRows + 1 && col == numCols + 1) type = BorderCornerSE;
            else if (row == numRows + 1) type = BorderSouth;
            else if (col == 0) type = BorderWest;
            else if (col == numCols + 1) type = BorderEast;
            else continue;
            m_borders[idx++]->setBorderType(type, row, col);
        }
    }

    adjustItemPositions();
    m_flaggedMinesCount = 0;
    Q_EMIT flaggedMinesCountChanged(m_flaggedMinesCount);
}

// Inlined in KMinesMainWindow::onGameOver (0x117ee0) when "Reset" is chosen.
// Mines and digits are kept: the same field is replayed.
void MineFieldItem::resetField()
{
    m_gameOver = false;
    m_numUnrevealed = m_numRows * m_numCols;
    for (CellItem *item : std::as_const(m_cells)) {
        item->setState(Released);
        item->setState(Released);
        // exploded flag cleared directly (+0x2d = 0)
    }
    m_flaggedMinesCount = 0;
    Q_EMIT flaggedMinesCountChanged(m_flaggedMinesCount);
}

// 0x11a0b0
void MineFieldItem::generateField(int clickedIdx)
{
    const QList<CellItem *> neighbours =
        adjacentItemsFor(clickedIdx / m_numCols, clickedIdx % m_numCols);
    // Fresh generator seeded from the global 64-bit generator.
    QRandomGenerator gen(QRandomGenerator64::global()->generate());

    QList<int> minePositions;
    int minesToPlace = m_minesCount;
    while (minesToPlace != 0) {
        const int idx = gen.bounded(m_numRows * m_numCols);
        CellItem *item = m_cells.at(idx);
        if (item->hasMine())
            continue;
        // Never on the first-clicked cell nor on any of its neighbours.
        if (neighbours.contains(item) || idx == clickedIdx)
            continue;
        item->setHasMine(true);
        minePositions.append(idx);
        --minesToPlace;
    }

    for (int pos : std::as_const(minePositions)) {
        const auto around = adjacentItemsFor(pos / m_numCols, pos % m_numCols);
        for (CellItem *item : around)
            if (!item->hasMine())
                item->setDigit(item->digit() + 1); // setDigit calls updatePixmap
    }
}

// 0x116240  order: NW, N, NE, W, E, SW, S, SE (bounds-checked)
QList<FieldPos> MineFieldItem::adjacentRowColsFor(int row, int col) const
{
    QList<FieldPos> out;
    if (row != 0) {
        if (col != 0) out.append({row - 1, col - 1});
        out.append({row - 1, col});
        if (col != m_numCols - 1) out.append({row - 1, col + 1});
    }
    if (col != 0) out.append({row, col - 1});
    if (col != m_numCols - 1) out.append({row, col + 1});
    if (row != m_numRows - 1) {
        if (col != 0) out.append({row + 1, col - 1});
        out.append({row + 1, col});
        if (col != m_numCols - 1) out.append({row + 1, col + 1});
    }
    return out;
}

// 0x119c40
QList<CellItem *> MineFieldItem::adjacentItemsFor(int row, int col) const
{
    QList<CellItem *> out;
    for (const FieldPos &p : adjacentRowColsFor(row, col))
        out.append(itemAt(p.first, p.second));
    return out;
}

// 0x1196e0
void MineFieldItem::adjustItemPositions()
{
    for (int row = 0; row < m_numRows; ++row)
        for (int col = 0; col < m_numCols; ++col)
            itemAt(row, col)->setPos((col + 1) * m_cellSize, (row + 1) * m_cellSize);
    for (BorderItem *b : std::as_const(m_borders))
        b->setPos(b->col() * m_cellSize, b->row() * m_cellSize);
}

// Inlined in KMinesScene::resizeScene (0x120020)
void MineFieldItem::resizeToFitInRect(const QRectF &rect)
{
    prepareGeometryChange();
    const qreal w = m_numCols + 2;
    const qreal h = m_numRows + 2;
    m_cellSize = (w / rect.width() > h / rect.height()) ? int(rect.width() / w)
                                                        : int(rect.height() / h);
    for (CellItem *c : std::as_const(m_cells)) {
        c->setRenderSize(QSize(m_cellSize, m_cellSize));
        for (QGraphicsItem *child : c->childItems())
            static_cast<KGameRenderedItem *>(child)->setRenderSize(QSize(m_cellSize, m_cellSize));
    }
    for (BorderItem *b : std::as_const(m_borders))
        b->setRenderSize(QSize(m_cellSize, m_cellSize));
    adjustItemPositions();
}

// 0x1108f0
QRectF MineFieldItem::boundingRect() const
{
    return QRectF(0, 0, m_cellSize * (m_numCols + 2), m_cellSize * (m_numRows + 2));
}

// 0x118690
void MineFieldItem::revealEmptySpace(int row, int col)
{
    for (const FieldPos &p : adjacentRowColsFor(row, col)) {
        CellItem *item = itemAt(p.first, p.second);
        const CellState s = item->state();
        // Only Released / Pressed / Hint cells are opened.
        if (s == Revealed || s == Questioned || s == Flagged || s == Error)
            continue;
        item->setState(Revealed);
        --m_numUnrevealed;
        if (item->digit() == 0)
            revealEmptySpace(p.first, p.second);
    }
}

// 0x1197f0  returns true when the game ended
bool MineFieldItem::onItemRevealed(int row, int col)
{
    --m_numUnrevealed;
    CellItem *revealed = itemAt(row, col);
    if (revealed->hasMine()) {
        // Lost: show every unflagged mine; wrongly placed flags become Error.
        for (CellItem *item : std::as_const(m_cells)) {
            const bool flagged = item->state() == Flagged;
            if (item->hasMine() == flagged)
                continue;
            if (item->state() != Revealed && item->state() != Error)
                item->setState((flagged && !item->hasMine()) ? Error : Revealed);
            --m_numUnrevealed;
        }
    } else if (revealed->digit() == 0) {
        revealEmptySpace(row, col);
    }

    for (CellItem *item : std::as_const(m_cells)) {
        if (item->isExploded()) {
            m_gameOver = true;
            Q_EMIT gameOver(false);
            return true;
        }
    }

    if (m_numUnrevealed == m_minesCount) {
        // Won: every remaining hidden cell gets a flag.
        for (CellItem *item : std::as_const(m_cells)) {
            if (item->state() == Questioned)
                item->mark(); // -> Released
            if (item->state() == Flagged || item->state() == Error)
                continue;
            if (item->state() != Revealed)
                item->mark(); // Released -> Flagged
        }
        m_gameOver = true;
        Q_EMIT flaggedMinesCountChanged(m_minesCount);
        Q_EMIT gameOver(true);
        return true;
    }
    return false;
}

// 0x119b00
bool MineFieldItem::onItemRevealed(CellItem *item)
{
    const int idx = m_cells.indexOf(item);
    if (idx == -1) {
        qCDebug(KMINES_LOG) << "really strange - item not found";
        return false;
    }
    return onItemRevealed(idx / m_numCols, idx % m_numCols);
}

// 0x119a50
void MineFieldItem::handleFlag(CellItem *item)
{
    const bool wasFlagged = item->isFlagged();
    item->mark();
    if (item->isFlagged() != wasFlagged) {
        m_flaggedMinesCount += item->isFlagged() ? 1 : -1;
        Q_EMIT flaggedMinesCountChanged(m_flaggedMinesCount);
    }
}

// Chord ("explore") shared by mouse release and short tap.
void MineFieldItem::revealAround(int row, int col)
{
    const QList<CellItem *> neighbours = adjacentItemsFor(row, col);
    int flags = 0, mines = 0;
    for (CellItem *n : neighbours) {
        flags += n->isFlagged();
        mines += n->hasMine();
    }
    if (flags == mines && flags != 0) {
        for (CellItem *n : neighbours) {
            if (n->isRevealed())
                continue;
            n->release(true);
            if (n->isRevealed() && onItemRevealed(n))
                break;
        }
    } else {
        for (CellItem *n : neighbours)
            n->undoPress();
    }
}

static inline FieldPos cellUnder(const QPointF &p, int cellSize)
{
    // Border ring occupies one cell, hence the -1.
    return {int(p.y() / cellSize) - 1, int(p.x() / cellSize) - 1};
}

// 0x11f660
void MineFieldItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    const FieldPos pos = cellUnder(ev->pos(), m_cellSize);
    const int row = pos.first, col = pos.second;
    if (row < 0 || row >= m_numRows || col < 0 || col >= m_numCols)
        return;
    CellItem *item = itemAt(row, col);
    if (!item) {
        qCDebug(KMINES_LOG) << "unexpected - no item under mouse";
        return;
    }

    const bool exploreWithLeft = Settings::exploreWithLeftClickOnNumberCells();
    const int placeFlagOn = Settings::placeFlagOn(); // 0 = MouseRelease, 1 = MousePress
    if (!exploreWithLeft)
        m_emulatingMidButton = (ev->buttons() & Qt::LeftButton) && (ev->buttons() & Qt::RightButton);
    else
        m_emulatingMidButton = (ev->buttons() & Qt::LeftButton) && item->isRevealed();

    if (ev->button() == Qt::MiddleButton || m_emulatingMidButton) {
        item->undoPress();
        for (CellItem *n : adjacentItemsFor(row, col)) {
            n->press();
            m_midButtonPos = {row, col};
            m_leftButtonPos = {-1, -1};
        }
    } else if (ev->button() == Qt::LeftButton) {
        item->press();
        m_leftButtonPos = {row, col};
    } else if (placeFlagOn == 1 && ev->button() == Qt::RightButton
               && !(ev->buttons() & Qt::LeftButton)) {
        handleFlag(item);
    }
}

// 0x11fa60
void MineFieldItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    const FieldPos pos = cellUnder(ev->pos(), m_cellSize);
    const int row = pos.first, col = pos.second;
    if (row < 0 || row >= m_numRows || col < 0 || col >= m_numCols) {
        // Released outside: cancel pending presses.
        if (m_midButtonPos.first != -1) {
            for (CellItem *n : adjacentItemsFor(m_midButtonPos.first, m_midButtonPos.second))
                n->undoPress();
            m_midButtonPos = {-1, -1};
            m_emulatingMidButton = false;
        }
        if (m_leftButtonPos.first != -1) {
            itemAt(m_leftButtonPos.first, m_leftButtonPos.second)->undoPress();
            m_leftButtonPos = {-1, -1};
        }
        return;
    }

    CellItem *item = itemAt(row, col);
    const int placeFlagOn = Settings::placeFlagOn();

    if (ev->button() == Qt::MiddleButton || m_emulatingMidButton) {
        m_midButtonPos = {-1, -1};
        if (item->isRevealed())
            revealAround(row, col);
        else
            for (CellItem *n : adjacentItemsFor(row, col))
                n->undoPress();
    } else if (ev->button() == Qt::LeftButton && !(ev->buttons() & Qt::RightButton)) {
        if (m_midButtonPos.first != -1) {
            item->undoPress();
            return;
        }
        if (m_leftButtonPos.first == -1)
            return;
        if (!item->isRevealed()) {
            if (m_firstClick) {
                m_firstClick = false;
                generateField(row * m_numCols + col);
                Q_EMIT firstClickDone();
            }
            item->release();
            if (item->isRevealed())
                onItemRevealed(row, col);
        }
        m_leftButtonPos = {-1, -1};
    } else if (placeFlagOn == 0 && ev->button() == Qt::RightButton
               && !(ev->buttons() & Qt::LeftButton)) {
        handleFlag(item);
    }
}

// 0x11eb40
void MineFieldItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    const FieldPos pos = cellUnder(ev->pos(), m_cellSize);
    const int row = pos.first, col = pos.second;
    if (row < 0 || row >= m_numRows || col < 0 || col >= m_numCols)
        return;

    if (!(ev->buttons() & Qt::MiddleButton) && !m_emulatingMidButton) {
        // Drag the single pressed cell along with the left button.
        if ((ev->buttons() & Qt::LeftButton) && m_leftButtonPos.first != -1
            && m_leftButtonPos.second != -1 && m_leftButtonPos != pos) {
            itemAt(m_leftButtonPos.first, m_leftButtonPos.second)->undoPress();
            itemAt(row, col)->press();
            m_leftButtonPos = pos;
        }
    } else if (m_midButtonPos.first != -1 && m_midButtonPos.second != -1 && m_midButtonPos != pos) {
        // Drag the pressed 3x3 ring.
        for (CellItem *n : adjacentItemsFor(m_midButtonPos.first, m_midButtonPos.second))
            n->undoPress();
        for (CellItem *n : adjacentItemsFor(row, col))
            n->press();
        m_midButtonPos = pos;
    }
}

// 0x11ee50  single-finger touch: short tap = flag / chord, long press (>=1s) = reveal
bool MineFieldItem::sceneEvent(QEvent *event)
{
    auto *te = static_cast<QTouchEvent *>(event);
    if (m_gameOver || te->points().size() != 1)
        return true;
    const QPointF p = te->point(0).position();
    const FieldPos pos = cellUnder(p, m_cellSize);
    const qint64 nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::system_clock::now().time_since_epoch()).count();
    const bool inside = pos.first >= 0 && pos.first < m_numRows && pos.second >= 0
                        && pos.second < m_numCols;
    CellItem *prev = (m_touchPos.first >= 0) ? itemAt(m_touchPos.first, m_touchPos.second) : nullptr;

    if (inside) {
        CellItem *item = itemAt(pos.first, pos.second);
        if (event->type() == QEvent::TouchBegin) {
            m_touchPos = pos;
            m_touchStartMs = nowMs;
            item->press();
            return true;
        }
        if (event->type() == QEvent::TouchEnd && pos == m_touchPos) {
            if (m_firstClick) {
                m_firstClick = false;
                generateField(pos.first * m_numCols + pos.second);
                Q_EMIT firstClickDone();
                item->release(true);
                if (item->isRevealed())
                    onItemRevealed(pos.first, pos.second);
            } else if (nowMs - m_touchStartMs < 1000) {
                if (item->isRevealed()) {
                    // chord: compares flag count with the cell digit
                    int flags = 0;
                    const auto ns = adjacentItemsFor(pos.first, pos.second);
                    for (CellItem *n : ns) flags += n->isFlagged();
                    if (flags != 0 && flags == item->digit())
                        for (CellItem *n : ns) {
                            if (n->isRevealed()) continue;
                            n->release(true);
                            if (n->isRevealed() && onItemRevealed(n)) break;
                        }
                } else {
                    item->undoPress();
                    handleFlag(item);
                }
            } else if (item->isRevealed()) {
                item->undoPress();
            } else {
                item->release();
                if (item->isRevealed())
                    onItemRevealed(item);
            }
            return true;
        }
        if (event->type() == QEvent::TouchCancel && prev)
            prev->undoPress();
    } else if (prev) {
        prev->undoPress();
    }
    m_touchPos = {-1, -1};
    return true;
}
