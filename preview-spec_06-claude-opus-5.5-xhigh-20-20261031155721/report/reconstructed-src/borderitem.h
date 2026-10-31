// Reconstructed BorderItem (RTTI "10BorderItem"). Sprite keys from 0x1206d0
// (static QHash filled once, guard DAT_001292b8; 0x116ca0 = operator[]).
#ifndef BORDERITEM_H
#define BORDERITEM_H

#include "cellitem.h"
#include <KGameRenderedItem>

class BorderItem : public KGameRenderedItem
{
public:
    BorderItem(KGameRenderer *renderer, QGraphicsItem *parent)
        : KGameRenderedItem(renderer, QString(), parent)
    {
        if (s_elementNames.isEmpty()) {
            s_elementNames[KMinesState::BorderNorth] = QStringLiteral("border.edge.north");
            s_elementNames[KMinesState::BorderSouth] = QStringLiteral("border.edge.south");
            s_elementNames[KMinesState::BorderEast] = QStringLiteral("border.edge.east");
            s_elementNames[KMinesState::BorderWest] = QStringLiteral("border.edge.west");
            s_elementNames[KMinesState::BorderCornerNE] = QStringLiteral("border.outsideCorner.ne");
            s_elementNames[KMinesState::BorderCornerNW] = QStringLiteral("border.outsideCorner.nw");
            s_elementNames[KMinesState::BorderCornerSW] = QStringLiteral("border.outsideCorner.sw");
            s_elementNames[KMinesState::BorderCornerSE] = QStringLiteral("border.outsideCorner.se");
        }
    }

    void setBorderType(KMinesState::BorderElement e, int row, int col)
    {
        m_element = e; // +0x28
        m_row = row;   // +0x2c
        m_col = col;   // +0x30
        setSpriteKey(s_elementNames[e]);
    }
    int row() const { return m_row; }
    int col() const { return m_col; }

private:
    static QHash<KMinesState::BorderElement, QString> s_elementNames;
    KMinesState::BorderElement m_element = KMinesState::BorderEast; // ctor writes 2
    int m_row = -1;
    int m_col = -1;
};

#endif
