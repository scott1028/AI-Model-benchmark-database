// KMinesView — QGraphicsView hosting KMinesScene
// Reconstructed from FUN_00124500 (view ctor), FUN_001202b0 / FUN_00120550
// (scene attach, wheel zoom), FUN_001203f0 (resize), PLT sites:
// QGraphicsView::setScene, scale(), centerOn(), setTransformationAnchor,
// QPainter::setRenderHint. Confidence: B/C.
//
// Proven fragments:
//   V1 view renders background sprite "mainWidget" (key at 0x1a703 area)
//   V2 toolbar pixmaps "view-refresh" (new game) and settings icon "settings"
//   V3 zoom via scale() calls with QTransform (0x120517 etc.)
//   V4 KGameRenderer::spritePixmap("mainWidget", size) for central widget
//      background (observed QSize 345x23 in gdb PLT probe)

#include "minesview.h"

KMinesView::KMinesView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setAlignment(Qt::AlignCenter);
    setBackgroundBrush(Qt::black);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void KMinesView::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);   // scale()/centerOn()
}

void KMinesView::wheelEvent(QWheelEvent *e)
{
    // FUN_00120550: scale(1.25) / scale(0.8) zoom
}

void KMinesView::resizeEvent(QResizeEvent *e)
{
    // FUN_001203f0: re-fit board
}

// Main window builds the view + a background "mainWidget" sprite:
//   KGameRenderer("kmines").spritePixmap("mainWidget", QSize(w, h))   // V1/V4
