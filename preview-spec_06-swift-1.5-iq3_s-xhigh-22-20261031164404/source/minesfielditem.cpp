// MineFieldItem — view item per cell; KGameRendererClient sprite binding
// Reconstructed from FUN_001173a0 / FUN_001176b0 / FUN_00117510 (ctors with
// x,y,w,h), FUN_00118810 (static state→key QHash, vaddr 0x18810), FUN_00117930
// (setSpriteKey caller), FUN_00117b20/FUN_00117d80 (paint path, renderPixmap PLT 0xf590).
// Confidence: A for keys/flow, C for class spelling.
//
// Proven:
//   I1 sprite key strings in static pool (objdump of .rodata @0x18810, 65-byte
//      init FUN_00118810): "hint","cell_up","error","mine","cell_down","flag",
//      "question" (+ "arabicOne".."arabicEight" built at 0x17b50 by "arabic"+digit word
//      concatenation: 0x636162617261 = "arabic" pattern, key len 9 + digit)
//   I2 spritePixmap QSize args observed at runtime via gdb on PLT 0xf590:
//      e.g. QSize(65,40) for number sprites, QSize(345,23) for wide sprites
//      (r8d = size-1: 0x27=39→40, 0x15=21→22)
//   I3 cell paint: base "cell_up" / "cell_down" + overlay sprite
//      (two renderPixmap calls at 0x18092 / 0x1829a then QImage composition)

#include "minesfielditem.h"

MineFieldItem::MineFieldItem(int x, int y, int w, int h)
    : QGraphicsItem()
    , m_client(new KGameRendererClient(g_renderer))
{
    m_x = x; m_y = y; m_w = w; m_h = h;
    m_state = CellUp;
}

void MineFieldItem::setState(State s)
{
    m_state = s;
    m_client->setSpriteKey(stateKeyTable().value(s));   // FUN_00117930
}

// FUN_00118810 — state key table (static initializer)
QHash<State, QString> &stateKeyTable()
{
    static const QHash<State, QString> table = {
        { Hint,          u"hint" },          // 4 bytes @pool+0x00
        { CellUp,        u"cell_up" },       // 9 bytes @pool+0x04
        { Error,         u"error" },         // 6 bytes @pool+0x0e
        { Mine,          u"mine" },          // 5 bytes @pool+0x15
        { CellDown,      u"cell_down" },     // 10 bytes @pool+0x1b
        { Flag,          u"flag" },          // 5 bytes @pool+0x26
        { Question,      u"question" },      // 9 bytes @pool+0x2c
        { NumberOne,     u"arabicOne" },     // digits appended to "arabic"
        ...
        { NumberEight,   u"arabicEight" },
    };
    return table;
}

void MineFieldItem::paint(QPainter *p)
{
    // FUN_00117b20: renderer->renderPixmap("cell_up"|"cell_down")  (PLT 0xf590)
    // + overlay sprite (number/mine/flag/question/explosion)
}
