// KMinesScene — game logic + KGameRenderer view bridge
// Reconstructed from FUN_0011a610 (ctor), FUN_0011a0b0 (placeMines),
// FUN_0011a8f0 (openCell), FUN_0011b270 (openNeighbours/cascade),
// FUN_00117930 (updateItem), FUN_00117ee0 (gameOver/score),
// FUN_001206d0 (newField), FUN_0011f660 (mousePress), FUN_0011fa60 (mouseRelease),
// FUN_0011eb40 (setShineOnNeighbours), FUN_0011ee50 (hover), FUN_00119f20 (start),
// FUN_00119570 (revealCell), FUN_00118810 (state→sprite-key table).
// Confidence: logic A/B, names & signatures C.
//
// Proven facts encoded below:
//   A1 placeMines RNG:  `mine = (qulonglong(rand()) * (w*h)) >> 32`   (0x1ab2c-0x1ab38)
//   A2 first click: mines placed AFTER the first click, excluding the clicked cell
//      only (loop breaks when index==clicked; NO exclusion of neighbours) (0x1aadc-0x1ab98)
//   A3 on the very first click the clock starts and signal firstClickDone fires (0x1ac05-0x1ac44)
//   A4 updateItem() = KGameRendererClient::setSpriteKey (PLT 0x1fa30 -> _ZN18KGameRendererClient12setSpriteKeyRK7QString)
//   A5 sprite keys from static hash at vaddr 0x18810 / FUN_00118810:
//      keys "hint","cell_up","error","mine","cell_down","flag","question" + "arabicOne".."arabicEight"
//      (string pool bytes decoded at file 0x8810+; 32-entry table max)
//   A6 middle-button chord: if flagsAround == minesAround -> openNeighbours (0x20170-0x20198)
//   A7 right-button cycles marks when settings()->useQuestionMarks() (0x2019c-0x201e8);
//      first right click (no mine field yet) places mines excluding clicked cell (0x201f2-0x2022d)
//   A8 PlaceFlagOn: 0 = Released, 1 = Pressed (ItemEnum values proven in FUN_00111580)
//   A9 ExploreWithLeftClickOnNumberCells gates left-click on opened number cells (0x1fd3b)

#include "minesscene.h"
#include <cstdlib>

KMinesScene::KMinesScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_renderer(new KGameRenderer(QStringLiteral("kmines")))   // theme name "kmines" (0x1a703)
{
    // FUN_0011a610
    m_clock   = new KGameClock(this, KGameClock::Pauseable);  // scene member @this+0x60, ctor arg 1 (0x11b764); Pauseable=1 (enum CountUp=0, Pauseable=1 proven via libKDEGames6 QMetaEnum strings). The main window creates its own clock with CountUp (0x12f34).
    // Clock control via QMetaObject::metacall slot ids [name mapping B,
    // kdegames KGameClock public-slot order]:
    //   slot 4 (max(mines,499)) = setMaxSeconds   @0x11b690 (updateMaxMines)
    //   slot 5 (5)              = setPenalty(5)   @0x11b690/0x1ad46 (+5 s per flag)
    //   slot 7 (no args)        = start()         @0x1ac2e (first click)
    //   slot 9 (no args)        = stopAndReset()  @0x17d1d gameOver, 0x2119b newField
    // KGameScore slots via metacall: 5 setValue(seconds), 6 disable()
    //   @0x17f52 (DisableScoreOnReset), @0x211b0 (newField), 8 save, 9 load
    // KGameClock API proven by imports: ctor, timeString(), seconds(),
    // timeChanged(QString). Status text i18n("Time: %1"); score = seconds();
    // score dialog title i18n("Time: %1").arg(score) (0x12b8e).
    m_field   = new MineField(this);
    m_score   = new KGameScore(this);
    m_firstClickDone = false;
    m_gameOver = false;

    connect(m_clock, &KGameClock::timeChanged, this, &KMinesScene::advanceTime);
    connect(m_clock, &KGameClock::sig_paused,  this, &KMinesScene::paused);
    connect(m_field, &MineField::minesCountChanged, this, &KMinesScene::minesCountChanged);
    // QMetaSignal strings proven: firstClickDone, gameOver, won,
    // flaggedMinesCountChanged, minesCountChanged (inventory.md)
}

void KMinesScene::setDifficulty(int level, int width, int height, int mines)
{
    // FUN_0011be70 (custom sizes) / FUN_001206d0 (newField)
    m_field->setSize(width, height);
    m_totalMines = mines;
    newField();
}

void KMinesScene::newField()
{
    // FUN_001206d0
    for (cell : all cells) {
        cell->setOpened(false);
        cell->setFlagged(false);
        cell->setQuestionMark(false);
        cell->setMine(false);
        cell->setShineOn(false);
        updateItem(cell);                       // -> "cell_up"
    }
    m_clock->stopAndReset();                    // metacall slot @0x2119b
    emit minesCountChanged(0);
    emit flaggedMinesCountChanged(0);
    m_firstClickDone = false;
    m_gameOver = false;
}

void KMinesScene::placeMines(int forbiddenX, int forbiddenY)
{
    // FUN_0011a0b0 — proven A1/A2
    const int w = m_field->width(), h = m_field->height();
    int placed = 0;
    while (placed < m_totalMines) {
        const int index = (int)(((qulonglong)std::rand() * (qulonglong)(w * h)) >> 32);
        if (index == forbiddenX + forbiddenY * w)   // only the clicked cell is skipped
            continue;
        MineFieldItem *cell = m_field->cellByIndex(index);
        if (cell->isMine())
            continue;
        cell->setMine(true);
        ++placed;
    }
    m_field->countMinesAround();
}

void KMinesScene::openCell(int x, int y)         // FUN_0011a8f0
{
    MineFieldItem *cell = m_field->cellAt(x, y);
    if (m_gameOver || !cell || cell->opened() || cell->flagged())
        return;

    if (!m_firstClickDone) {                      // 0x1aadc: firstClickDone check
        placeMines(x, y);                         // A2
        m_clock->start();                         // A3
        m_firstClickDone = true;
        emit firstClickDone();
    }

    if (cell->isMine()) {                         // 0x1af2d
        revealCell(cell, true);                   // FUN_00119570: "explosion" variant
        gameOver(/*lost=*/true);
        return;
    }

    cell->setOpened(true);
    cell->setFlagged(false);
    cell->setQuestionMark(false);

    if (cell->minesAround() == 0)                 // 0x1b18e: cascade
        openNeighbours(cell);                     // FUN_0011b270 flood fill
    else
        updateItem(cell);                         // "arabicOne".."arabicEight"

    if (openedCount() == m_field->width() * m_field->height() - m_totalMines)
        gameOver(/*won=*/true);                   // emit won + score
}

void KMinesScene::openNeighbours(MineFieldItem *cell)  // FUN_0011b270 (iterative queue)
{
    QQueue<MineFieldItem *> queue;
    queue.enqueue(cell);
    while (!queue.isEmpty()) {
        MineFieldItem *c = queue.dequeue();
        for (MineFieldItem *n : m_field->neighbours(c->x(), c->y())) {
            if (n->opened() || n->isMine() || n->flagged())
                continue;
            n->setOpened(true);
            updateItem(n);
            if (n->minesAround() == 0)
                queue.enqueue(n);
        }
    }
}

void KMinesScene::setShineOnNeighbours(int x, int y, bool on)
{
    // FUN_0011eb40 — proven: sets shine on the 4 orthogonal neighbours
    // (index-1, index+1, index-width, index+width with edge checks)
    // 0x1f0a9-0x1f130; each -> updateItem()
}

void KMinesScene::updateItem(MineFieldItem *item)
{
    // FUN_00117930 — state -> sprite key via static QHash (FUN_00118810)
    static const QHash<State, QString> keys {
        { CellUp,       QStringLiteral("cell_up") },
        { CellDown,     QStringLiteral("cell_down") },
        { Number,       QStringLiteral("arabicOne") /* .. arabicEight, + offset */ },
        { Mine,         QStringLiteral("mine") },
        { Explosion,    QStringLiteral("explosion") },  // seen at PLT sprite calls
        { Flag,         QStringLiteral("flag") },
        { Question,     QStringLiteral("question") },
        { Hint,         QStringLiteral("hint") },
        { Error,        QStringLiteral("error") },
    };
    item->client()->setSpriteKey(keys.value(item->state()));   // PLT 0xfa30
}

void KMinesScene::gameOver(bool lost)
{
    // FUN_00117ee0 + FUN_00118470
    m_gameOver = true;
    m_clock->stopAndReset();                      // 0x17d1d

    for (cell : all cells)
        if (cell->isMine())
            revealCell(cell, /*asMine=*/false);   // FUN_00119570 "mine" (0x199b2 site)

    emit gameOver(lost);
    if (!lost) {
        emit won();
        m_score->setValue(m_clock->secondsElapsed());
        if (KConfig("kmines").group("General").readEntry("DisableScoreOnReset", true)
            && m_clock->wasReset())
            m_score->disable();                   // 0x17f35-0x17f4d
    }
}

// ---- input (KMinesScene event handlers, installed by view) ------------------

void KMinesScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    // FUN_0011f660
    MineFieldItem *cell = cellAtScenePos(e->scenePos());   // FUN_0011a280
    if (!cell) return;

    if (e->buttons().testFlag(Qt::LeftButton)) {
        if (cell->opened() && cell->minesAround() > 0 && !exploreWithLeftClickOnNumbers())
            return;                                       // A8 gate at 0x1fd3b
        m_pendingOpen = cell;                             // 0x1fe1c: [this+0x118]
        cell->setShineOn(true);
        setShineOnNeighbours(cell->x(), cell->y(), true); // 0x1fe35 FUN_0011eb40
        update();
    }
    if (e->buttons().testFlag(Qt::RightButton) && settings().useQuestionMarks()) {
        // A7: right-click on closed cell cycles flag/question
    }
}

void KMinesScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    // FUN_0011fa60
    switch (e->button()) {
    case Qt::LeftButton: {
        MineFieldItem *cell = cellAtScenePos(e->scenePos());
        if (!cell) break;
        if (cell->opened() && cell->minesAround() > 0) {
            // chord on number: flags around == number -> open rest (A6)
            int flags = countFlagsAround(cell);
            if (flags == cell->minesAround())
                openNeighbours(cell);
        } else if (m_pendingOpen == cell && settings().placeFlagOn() == PlaceFlag_Released) {
            openCell(cell->x(), cell->y());               // default: Released (0)
        }
        cell->setShineOn(false);
        setShineOnNeighbours(cell->x(), cell->y(), false);
        m_pendingOpen = nullptr;
        update();
        break;
    }
    case Qt::RightButton:
        if (settings().placeFlagOn() == PlaceFlag_Pressed)
            toggleMark(cellAtScenePos(e->scenePos()));
        break;
    case Qt::MiddleButton:
        // A6: middle-click chord, proven at 0x20170-0x20198
        break;
    }
}

void KMinesScene::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    // FUN_0011ee50: un-shine previous cell, shine cell under cursor and
    // its neighbours (0x1f27e, 0x1f2a9)
}

void KMinesScene::revealCell(MineFieldItem *cell, bool exploded)
{
    // FUN_00119570: marks cell opened and updates sprite; exploded -> "explosion"
}

void KMinesScene::start()
{
    // FUN_00119f20
}
