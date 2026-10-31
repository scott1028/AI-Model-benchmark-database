// Reconstructed CellItem implementation. Address comments point at Ghidra
// pseudocode in ../ghidra-decompiled/functions/<addr>.c
#include "cellitem.h"
#include "settings.h"

#include <KGameRenderer>

QHash<KMinesState::CellState, QList<QString>> CellItem::s_stateNames;
QHash<int, QString> CellItem::s_digitNames;

CellItem::CellItem(KGameRenderer *renderer, QGraphicsItem *parent)
    : KGameRenderedItem(renderer, QString(), parent)
{
    // Inlined in KMinesScene::startNewGame (0x1206d0): new CellItem, then
    // fillNameHashes() when the hash is still empty, then setShapeMode(Bounding).
    if (s_stateNames.isEmpty())
        fillNameHashes();
    setShapeMode(BoundingRectShape);
    reset();
}

// 0x118810
void CellItem::fillNameHashes()
{
    s_digitNames[1] = QStringLiteral("arabicOne");
    s_digitNames[2] = QStringLiteral("arabicTwo");
    s_digitNames[3] = QStringLiteral("arabicThree");
    s_digitNames[4] = QStringLiteral("arabicFour");
    s_digitNames[5] = QStringLiteral("arabicFive");
    s_digitNames[6] = QStringLiteral("arabicSix");
    s_digitNames[7] = QStringLiteral("arabicSeven");
    s_digitNames[8] = QStringLiteral("arabicEight");

    // rodata: 0x12232a "cell_up", 0x122346 "cell_down", 0x122364 "question",
    //         0x12235a "flag", 0x1222e8 "mine", 0x12233a "error"
    s_stateNames[KMinesState::Released] << QStringLiteral("cell_up");
    s_stateNames[KMinesState::Pressed] << QStringLiteral("cell_down");
    s_stateNames[KMinesState::Revealed] << QStringLiteral("cell_down");
    s_stateNames[KMinesState::Questioned] << QStringLiteral("cell_up");
    s_stateNames[KMinesState::Questioned] << QStringLiteral("question");
    s_stateNames[KMinesState::Flagged] << QStringLiteral("cell_up");
    s_stateNames[KMinesState::Flagged] << QStringLiteral("flag");
    s_stateNames[KMinesState::Error] << QStringLiteral("cell_down");
    s_stateNames[KMinesState::Error] << QStringLiteral("mine");
    s_stateNames[KMinesState::Error] << QStringLiteral("error");
    s_stateNames[KMinesState::Hint] << QStringLiteral("cell_up");
    s_stateNames[KMinesState::Hint] << QStringLiteral("hint");
}

void CellItem::reset()
{
    m_state = KMinesState::Released;
    m_hasMine = false;
    m_exploded = false;
    m_digit = 0;
    updatePixmap();
}

// 0x117930
void CellItem::updatePixmap()
{
    // Drop previous overlay children.
    qDeleteAll(childItems());

    const QList<QString> keys = s_stateNames[m_state];
    setSpriteKey(keys.at(0));
    // Extra keys become child overlays with the same render size.
    for (int i = 1; i < keys.size(); ++i) {
        auto *child = new KGameRenderedItem(renderer(), keys.at(i), this);
        child->setRenderSize(renderSize());
    }

    if (m_state == KMinesState::Revealed) {
        if (m_digit == 0) {
            if (m_hasMine) {
                if (m_exploded) {
                    auto *e = new KGameRenderedItem(renderer(), QStringLiteral("explosion"), this);
                    e->setRenderSize(renderSize());
                }
                auto *m = new KGameRenderedItem(renderer(), QStringLiteral("mine"), this);
                m->setRenderSize(renderSize());
            }
        } else {
            auto *d = new KGameRenderedItem(renderer(), s_digitNames[m_digit], this);
            d->setRenderSize(renderSize());
        }
    }
}

void CellItem::press()
{
    if (m_state == KMinesState::Released) {
        m_state = KMinesState::Pressed;
        updatePixmap();
    }
}

// 0x118790
void CellItem::release(bool force)
{
    if (!force) {
        if (m_state == KMinesState::Pressed) {
            m_state = KMinesState::Revealed;
            m_exploded = m_hasMine;
            updatePixmap();
        }
        return;
    }
    // force: reveal anything that is not flagged / question-marked.
    if (m_state == KMinesState::Questioned || m_state == KMinesState::Flagged)
        return;
    m_exploded = m_hasMine;
    if (m_state == KMinesState::Pressed
        || (m_state != KMinesState::Revealed && m_state != KMinesState::Error)) {
        m_state = KMinesState::Revealed;
        updatePixmap();
    }
}

// 0x1187f0
void CellItem::undoPress()
{
    if (m_state == KMinesState::Pressed) {
        m_state = KMinesState::Released;
        updatePixmap();
    }
}

// 0x117e70  Settings::self() at 0x11f4c0, useQuestionMarks at Settings+0x18
void CellItem::mark()
{
    switch (m_state) {
    case KMinesState::Released:
        m_state = KMinesState::Flagged;
        break;
    case KMinesState::Flagged:
        m_state = Settings::useQuestionMarks() ? KMinesState::Questioned : KMinesState::Released;
        break;
    case KMinesState::Questioned:
        m_state = KMinesState::Released;
        break;
    default:
        break; // Pressed / Revealed / Error / Hint unchanged
    }
    updatePixmap();
}
