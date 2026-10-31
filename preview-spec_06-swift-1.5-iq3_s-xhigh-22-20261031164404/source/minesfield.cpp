// MineField — board model (grid of MineFieldItem cells)
// Reconstructed from FUN_001191b0 (ctor), FUN_001193b0 (setSize: builds the
// 4-neighbour QHash keyed by 1-based coords x+1+(y+1)*w), FUN_00119290
// (createCells: cells are 0x38 = 56 bytes, row-major), FUN_00119c40
// (getNeighbours: 8-neighbour gather), FUN_00119620/FUN_001198b0 (accessors).
// Confidence: structure A/B, layout inferred (C).
//
// Layout facts (proven A):
//   - cells stored row-major: index = x + y*width   (0x1a244: imul rax,rbp; add rax,rbx)
//   - cell struct 56 bytes (imul 0x38 at 0x11929d); state int at +0x28; flag
//     bytes at +0x2c..0x31 (newField clears them); minesAround counter at
//     +0x30 (FUN_00119b60 increments from cell+0x30; call site 0x19db5)
//   - getNeighbours returns at most 8 items, skipping edges
//   - setSize() re-emits Qt signal "minesCountChanged" (QMetaSignal: FUN_0011d1b0)

#include "minefield.h"

MineField::MineField(QObject *parent) : QObject(parent)
{
    // FUN_001191b0: members
    m_width  = 0;
    m_height = 0;
    m_cells.clear();
    m_neighbourLists.clear();      // QHash<int, QList<MineFieldItem*>>
}

void MineField::setSize(int width, int height)
{
    // FUN_001193b0
    m_width  = width;
    m_height = height;

    // build 4-directional neighbour lists per cell (north/south/east/west,
    // 0x11948c-0x11954b; QHash insert key is the 1-based padded index
    // (long)y*width + x + width; FUN_00119a80 appends into the cell's list)
    m_neighbourLists.clear();
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            QList<MineFieldItem *> list;
            if (y > 0)          list.append(cellAt(x, y - 1)); // north
            if (y < m_height-1) list.append(cellAt(x, y + 1)); // south
            if (x > 0)          list.append(cellAt(x - 1, y)); // west
            if (x < m_width-1)  list.append(cellAt(x + 1, y)); // east
            m_neighbourLists.insert(indexOf(x, y), list);
        }
    }
    emit minesCountChanged(mineCount());
}

MineFieldItem *MineField::cellAt(int x, int y)
{
    // FUN_00119620 — bounds-checked: x < width (cmp at 0x1a000), y < height
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
        return nullptr;
    return &m_cells[x + y * m_width];
}

int MineField::mineCount() const
{
    // FUN_001198b0 — counts cells with isMine
    int count = 0;
    for (const MineFieldItem &cell : m_cells)
        if (cell.isMine())
            ++count;
    return count;
}

const QList<MineFieldItem *> &MineField::neighbours(int x, int y)
{
    // FUN_00119c40 — 8 neighbours: (x-1,y-1) (x,y-1) (x+1,y-1)
    //                                  (x-1,y)         (x+1,y)
    //                                  (x-1,y+1) (x,y+1) (x+1,y+1)
    // each via cellAt(); edge cells silently skipped
    return m_fullNeighbourCache[indexOf(x, y)];
}

void MineField::countMinesAround()
{
    // FUN_00119a80 is invoked for every cell; it increments the
    // "minesAround" counter of all neighbours of mine cells.
    for (auto &cell : m_cells)
        cell.setMinesAround(0);
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            if (cellAt(x, y)->isMine())
                for (MineFieldItem *n : neighbours(x, y))
                    n->setMinesAround(n->minesAround() + 1);
}

// ---------------------------------------------------------------------------

// FUN_00119290 — MineFieldItem model-side ctor (x, y, w, h)
MineFieldItem::MineFieldItem(int x, int y, int width, int height)
{
    m_x = x; m_y = y;
    m_width = width; m_height = height;
    m_isMine       = false;   // proven: cleared at 0x18e2d movword ptr [rsi+0x2c],0
    m_opened       = false;   // cleared at 0x18e41
    m_shineOn      = false;   // cleared at 0x18e35
    m_questionMark = false;   // cleared at 0x18e3b
    m_flagged      = false;   // cleared at 0x18e4b
    m_minesAround  = 0;       // cleared at 0x18e44
    // (flag/mine/marks booleans packed in bytes 0x2c..0x30 — layout C)
}
