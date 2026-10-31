// Reconstructed from /usr/games/kmines (kmines 4:25.12.3, sha256 cc51df42...)
// Class name "CellItem" comes from RTTI string "8CellItem".
// Field offsets are from Ghidra pseudocode; names are inferred from behavior.
#ifndef CELLITEM_H
#define CELLITEM_H

#include <KGameRenderedItem>
#include <QHash>

namespace KMinesState {
// Values observed as ints stored at CellItem+0x28.
enum CellState {
    Released = 0,   // sprite: cell_up
    Pressed = 1,    // sprite: cell_down
    Revealed = 2,   // sprite: cell_down (+ digit / mine / explosion children)
    Questioned = 3, // sprite: cell_up + question
    Flagged = 4,    // sprite: cell_up + flag
    Error = 5,      // sprite: cell_down + mine + error  (wrong flag after loss)
    Hint = 6        // sprite: cell_up + hint (table entry exists, never assigned)
};

// BorderItem type values observed at BorderItem+0x28.
enum BorderElement {
    BorderNorth = 0, BorderSouth, BorderEast, BorderWest,
    BorderCornerNW, BorderCornerSW, BorderCornerNE, BorderCornerSE
};
}

class CellItem : public KGameRenderedItem
{
public:
    CellItem(KGameRenderer *renderer, QGraphicsItem *parent);

    void reset();                       // state=Released, mine/exploded/digit cleared (inlined)
    void setHasMine(bool hasMine) { m_hasMine = hasMine; }
    bool hasMine() const { return m_hasMine; }
    void setDigit(int digit) { m_digit = digit; updatePixmap(); }
    int digit() const { return m_digit; }

    void press();                       // Released -> Pressed (inlined at call sites)
    void release(bool force = false);   // 0x118790
    void undoPress();                   // 0x1187f0
    void mark();                        // 0x117e70  flag / question cycle

    bool isRevealed() const { return m_state == KMinesState::Revealed || m_state == KMinesState::Error; }
    bool isFlagged() const { return m_state == KMinesState::Flagged; }
    bool isExploded() const { return m_exploded; }
    KMinesState::CellState state() const { return m_state; }
    void setState(KMinesState::CellState s) { m_state = s; updatePixmap(); }

    void updatePixmap();                // 0x117930

private:
    static void fillNameHashes();       // 0x118810, lazily called (DAT_001292a8 guard)
    static QHash<KMinesState::CellState, QList<QString>> s_stateNames; // 0x111d60 = operator[]
    static QHash<int, QString> s_digitNames;                           // 0x112a20 = operator[]

    KMinesState::CellState m_state = KMinesState::Released; // +0x28
    bool m_hasMine = false;                                  // +0x2c
    bool m_exploded = false;                                 // +0x2d
    int m_digit = 0;                                         // +0x30
};

#endif
