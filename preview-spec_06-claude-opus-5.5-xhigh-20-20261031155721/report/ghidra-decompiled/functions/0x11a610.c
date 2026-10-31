// rea decompile_all
// name: FUN_0011a610
// address: 0x11a610
// body: available
// range: 0x11a610-0x11b850 inclusive
// range: 0x11b858-0x11b872 inclusive
// range: 0x11b878-0x11b8a7 inclusive
// range: 0x11b8b0-0x11b8c7 inclusive
// range: 0x11b8d0-0x11b8e7 inclusive
// range: 0x11b8f0-0x11b907 inclusive
// range: 0x11b910-0x11b927 inclusive
// range: 0x11b930-0x11b947 inclusive
// range: 0x11b950-0x11b977 inclusive
// range: 0x11b980-0x11b987 inclusive
// range: 0x11b990-0x11b996 inclusive
// range: 0x11b9a0-0x11b9a6 inclusive
// range: 0x11b9b0-0x11b9bb inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11b84c to 0x116660 FUN_00116660 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11b9b7 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011a610(QObject *param_1)

{
  KGameRenderer *this;
  int *piVar1;
  QArrayData *pQVar2;
  wchar16 *pwVar3;
  int iVar4;
  QObject *pQVar5;
  undefined8 uVar6;
  QGraphicsScene *pQVar7;
  KGameThemeProvider *this_00;
  QGraphicsObject *this_01;
  undefined4 *puVar8;
  KGamePopupItem *pKVar9;
  QGraphicsView *this_02;
  KGameClock *pKVar10;
  KGameDifficultyLevel *pKVar11;
  KGameDifficultyLevel *this_03;
  int iVar12;
  long lVar13;
  QString *pQVar14;
  long lVar15;
  long in_FS_OFFSET;
  ulong uVar16;
  ulong uVar17;
  double dVar18;
  double dVar19;
  undefined1 auVar20 [16];
  int *local_b8;
  QBrush local_b0 [8];
  QArrayData *local_a8;
  wchar16 *pwStack_a0;
  undefined8 local_98;
  QArrayData *local_88;
  wchar16 *pwStack_80;
  undefined8 local_78;
  undefined1 local_68 [16];
  double local_58;
  double dStack_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  KXMLGUIClient::KXMLGUIClient((KXMLGUIClient *)(param_1 + 0x80));
  KXmlGuiWindow::KXmlGuiWindow((KXmlGuiWindow *)param_1,&PTR_PTR_00126bb8,0,0);
  *(undefined ***)param_1 = &PTR_FUN_00127df0;
  *(undefined ***)(param_1 + 0x10) = &PTR_FUN_00128010;
  *(undefined ***)(param_1 + 0x30) = &PTR_FUN_00128060;
  *(undefined ***)(param_1 + 0x80) = &PTR_FUN_00128128;
  *(undefined1 (*) [16])(param_1 + 0x40) = (undefined1  [16])0x0;
  *(undefined1 (*) [16])(param_1 + 0x50) = (undefined1  [16])0x0;
  pQVar5 = operator_new(0x28);
  QLabel::QLabel((QLabel *)pQVar5,0,0);
  uVar6 = QtSharedPointer::ExternalRefCountData::getAndRef(pQVar5);
  *(QObject **)(param_1 + 0x68) = pQVar5;
  *(undefined8 *)(param_1 + 0x60) = uVar6;
  pQVar5 = operator_new(0x28);
  QLabel::QLabel((QLabel *)pQVar5,0,0);
  uVar6 = QtSharedPointer::ExternalRefCountData::getAndRef(pQVar5);
  *(QObject **)(param_1 + 0x78) = pQVar5;
  *(undefined8 *)(param_1 + 0x70) = uVar6;
  pQVar7 = operator_new(0x48);
  QGraphicsScene::QGraphicsScene(pQVar7,param_1);
  *(undefined ***)pQVar7 = &PTR_FUN_00127a48;
  this = (KGameRenderer *)(pQVar7 + 0x18);
  this_00 = operator_new(0x18);
  local_58 = 2.47032822920623e-323;
  local_68 = ZEXT816(0x122104) << 0x40;
  KGameThemeProvider::KGameThemeProvider(this_00,(QByteArray *)local_68,(QObject *)0x0);
  if ((QArrayData *)local_68._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68._0_8_ = *(int *)local_68._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_68._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_68._0_8_,1,0x10);
    }
  }
  local_58 = 3.45845952088873e-323;
  local_78 = 6;
  local_68 = ZEXT816(0x12243c) << 0x40;
  local_88 = (QArrayData *)0x0;
  pwStack_80 = L"themes";
  KGameThemeProvider::discoverThemes
            ((QString *)this_00,(QString *)&local_88,(QMetaObject *)local_68);
  if (local_88 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_88 = *(int *)local_88 + -1;
    UNLOCK();
    if (*(int *)local_88 == 0) {
      QArrayData::deallocate(local_88,2,0x10);
    }
  }
  if ((QArrayData *)local_68._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68._0_8_ = *(int *)local_68._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_68._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_68._0_8_,2,0x10);
    }
  }
  KGameRenderer::KGameRenderer(this,this_00,0);
  *(undefined8 *)(pQVar7 + 0x40) = 0;
  *(undefined1 (*) [16])(pQVar7 + 0x30) = (undefined1  [16])0x0;
  QGraphicsScene::setItemIndexMethod(pQVar7);
  this_01 = operator_new(0x98);
  QGraphicsObject::QGraphicsObject(this_01,(QGraphicsItem *)0x0);
  *(KGameRenderer **)(this_01 + 0x90) = this;
  *(undefined ***)this_01 = &PTR_FUN_00127828;
  *(undefined ***)(this_01 + 0x10) = &PTR_FUN_001278c8;
  *(undefined8 *)(this_01 + 0x30) = 0;
  *(undefined8 *)(this_01 + 0x50) = 0x100000001;
  *(undefined2 *)(this_01 + 0x89) = 0;
  *(undefined8 *)(this_01 + 0x48) = 0;
  *(undefined4 *)(this_01 + 0x58) = 1;
  *(undefined8 *)(this_01 + 100) = 0xffffffffffffffff;
  *(undefined8 *)(this_01 + 0x70) = 0;
  *(undefined1 (*) [16])(this_01 + 0x20) = (undefined1  [16])0x0;
  *(undefined1 (*) [16])(this_01 + 0x38) = (undefined1  [16])0x0;
  *(undefined4 *)(this_01 + 0x78) = 0xffffffff;
  *(undefined4 *)(this_01 + 0x7c) = 0xffffffff;
  *(undefined4 *)(this_01 + 0x80) = 0xffffffff;
  *(undefined4 *)(this_01 + 0x84) = 0xffffffff;
  QGraphicsItem::setFlag(this_01 + 0x10,0x400,1);
  QGraphicsItem::setAcceptTouchEvents((bool)((char)this_01 + '\x10'));
  *(QGraphicsObject **)(pQVar7 + 0x30) = this_01;
  local_88 = (QArrayData *)FUN_0011bf80;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11bd20);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(QArrayData **)(puVar8 + 4) = local_88;
  *(wchar16 **)(puVar8 + 6) = pwStack_80;
  *(code **)(puVar8 + 2) = FUN_0011bba0;
  QObject::connectImpl
            ((Connection *)&local_a8,this_01,(QByteArray *)local_68,pQVar7,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127a00);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = *(undefined8 *)(pQVar7 + 0x30);
  local_88 = (QArrayData *)FUN_0011c040;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11bd80);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(code **)(puVar8 + 2) = FUN_0011bc20;
  *(QArrayData **)(puVar8 + 4) = local_88;
  *(wchar16 **)(puVar8 + 6) = pwStack_80;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,pQVar7,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127a00);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = *(undefined8 *)(pQVar7 + 0x30);
  local_88 = (QArrayData *)FUN_0011c300;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11bda0);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(undefined8 *)(puVar8 + 6) = 0;
  *(code **)(puVar8 + 2) = FUN_0011bca0;
  *(code **)(puVar8 + 4) = FUN_0011c300;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,pQVar7,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127a00);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = *(undefined8 *)(pQVar7 + 0x30);
  local_88 = (QArrayData *)FUN_0011bfe0;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11bda0);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(undefined8 *)(puVar8 + 6) = 0;
  *(code **)(puVar8 + 2) = FUN_0011bca0;
  *(code **)(puVar8 + 4) = FUN_0011bfe0;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,pQVar7,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127a00);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  QGraphicsScene::addItem((QGraphicsItem *)pQVar7);
  pKVar9 = operator_new(0x28);
  KGamePopupItem::KGamePopupItem(pKVar9,(QGraphicsItem *)0x0);
  *(KGamePopupItem **)(pQVar7 + 0x38) = pKVar9;
  KGamePopupItem::setMessageOpacity(0.9);
  KGamePopupItem::setMessageTimeout((int)*(undefined8 *)(pQVar7 + 0x38));
  QGraphicsScene::addItem((QGraphicsItem *)pQVar7);
  pKVar9 = operator_new(0x28);
  KGamePopupItem::KGamePopupItem(pKVar9,(QGraphicsItem *)0x0);
  *(KGamePopupItem **)(pQVar7 + 0x40) = pKVar9;
  KGamePopupItem::setMessageOpacity(0.9);
  KGamePopupItem::setMessageTimeout((int)*(undefined8 *)(pQVar7 + 0x40));
  KGamePopupItem::setHideOnMouseClick(SUB81(*(undefined8 *)(pQVar7 + 0x40),0));
  QGraphicsScene::addItem((QGraphicsItem *)pQVar7);
  local_b8 = (int *)0x0;
  QGraphicsScene::sceneRect();
  local_a8 = (QArrayData *)0x0;
  pwStack_a0 = L"mainWidget";
  local_98 = 10;
  dVar19 = (double)((ulong)local_58 & 0x8000000000000000 | 0x3fe0000000000000) + local_58;
  dStack_50 = (double)((ulong)dStack_50 & 0x8000000000000000 | 0x3fe0000000000000) + dStack_50;
  uVar16 = -(ulong)(2147483647.0 < dVar19);
  uVar17 = -(ulong)(2147483647.0 < dStack_50);
  dVar19 = (double)(~uVar16 & (ulong)dVar19 | uVar16 & 0x41dfffffffc00000);
  dVar18 = (double)(~uVar17 & (ulong)dStack_50 | uVar17 & 0x41dfffffffc00000);
  uVar16 = -(ulong)(-2147483648.0 < dVar19);
  uVar17 = -(ulong)(-2147483648.0 < dVar18);
  KGameRenderer::spritePixmap
            (&local_88,this,(Connection *)&local_a8,
             CONCAT44((int)(double)(~uVar17 & 0xc1e0000000000000 | (ulong)dVar18 & uVar17),
                      (int)(double)(~uVar16 & 0xc1e0000000000000 | (ulong)dVar19 & uVar16)),
             0xffffffff,&local_b8);
  QBrush::QBrush(local_b0,(QPixmap *)&local_88);
  QGraphicsScene::setBackgroundBrush((QBrush *)pQVar7);
  QBrush::~QBrush(local_b0);
  QPixmap::~QPixmap((QPixmap *)&local_88);
  if (local_a8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_a8 = *(int *)local_a8 + -1;
    UNLOCK();
    if (*(int *)local_a8 == 0) {
      QArrayData::deallocate(local_a8,2,0x10);
    }
  }
  piVar1 = local_b8;
  if ((local_b8 != (int *)0x0) && (*local_b8 != -1)) {
    LOCK();
    *local_b8 = *local_b8 + -1;
    UNLOCK();
    if ((*local_b8 == 0) && (local_b8 != (int *)0x0)) {
      lVar13 = *(long *)(local_b8 + 8);
      if (lVar13 != 0) {
        lVar15 = *(long *)(lVar13 + -8) * 0x90 + lVar13;
        if (lVar13 == lVar15) {
          lVar13 = 0;
        }
        else {
          do {
            lVar13 = lVar15;
            lVar15 = lVar13 + -0x90;
            if (*(void **)(lVar13 + -0x10) != (void *)0x0) {
              operator_delete__(*(void **)(lVar13 + -0x10));
            }
          } while (*(long *)(piVar1 + 8) != lVar15);
          lVar13 = *(long *)(lVar13 + -0x98) * 0x90;
        }
        operator_delete__((void *)(lVar15 + -8),lVar13 + 8);
      }
      operator_delete(piVar1,0x28);
    }
  }
  *(QGraphicsScene **)(param_1 + 0x40) = pQVar7;
  local_88 = (QArrayData *)FUN_001141a0;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11bf80);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(QArrayData **)(puVar8 + 4) = local_88;
  *(wchar16 **)(puVar8 + 6) = pwStack_80;
  *(code **)(puVar8 + 2) = FUN_00110b90;
  QObject::connectImpl
            ((Connection *)&local_a8,pQVar7,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127b60);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = *(undefined8 *)(param_1 + 0x40);
  local_88 = (QArrayData *)FUN_00117ee0;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11bfe0);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(undefined8 *)(puVar8 + 6) = 0;
  *(code **)(puVar8 + 2) = FUN_00110a10;
  *(code **)(puVar8 + 4) = FUN_00117ee0;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127b60);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = *(undefined8 *)(param_1 + 0x40);
  local_88 = (QArrayData *)FUN_00114110;
  pwStack_80 = (wchar16 *)0x0;
  local_68 = ZEXT816(0x11c040);
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(undefined8 *)(puVar8 + 6) = 0;
  *(code **)(puVar8 + 2) = FUN_00110990;
  *(code **)(puVar8 + 4) = FUN_00114110;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             &PTR_staticMetaObject_00127b60);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  this_02 = operator_new(0x30);
  pQVar7 = *(QGraphicsScene **)(param_1 + 0x40);
  QGraphicsView::QGraphicsView(this_02,pQVar7,(QWidget *)param_1);
  *(QGraphicsScene **)(this_02 + 0x28) = pQVar7;
  *(undefined ***)this_02 = &PTR_FUN_00127ba8;
  *(undefined ***)(this_02 + 0x10) = &PTR_FUN_00127d98;
  *(QGraphicsView **)(param_1 + 0x48) = this_02;
  QGraphicsView::setCacheMode(this_02,1);
  QAbstractScrollArea::setVerticalScrollBarPolicy(*(undefined8 *)(param_1 + 0x48),1);
  QAbstractScrollArea::setHorizontalScrollBarPolicy(*(undefined8 *)(param_1 + 0x48),1);
  QFrame::setFrameStyle((int)*(undefined8 *)(param_1 + 0x48));
  QGraphicsView::setOptimizationFlags(*(undefined8 *)(param_1 + 0x48),3);
  pKVar10 = operator_new(0x18);
  KGameClock::KGameClock(pKVar10,param_1,2);
  *(KGameClock **)(param_1 + 0x50) = pKVar10;
  local_88 = (QArrayData *)FUN_00114400;
  pwStack_80 = (wchar16 *)0x0;
  local_68._8_8_ = 0;
  local_68._0_8_ = KGameClock::timeChanged;
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(QArrayData **)(puVar8 + 4) = local_88;
  *(wchar16 **)(puVar8 + 6) = pwStack_80;
  *(code **)(puVar8 + 2) = FUN_00110b10;
  QObject::connectImpl
            ((Connection *)&local_a8,pKVar10,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             KGameClock::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  if ((*(long *)(param_1 + 0x60) == 0) || (*(int *)(*(long *)(param_1 + 0x60) + 4) == 0)) {
    pQVar14 = (QString *)0x0;
  }
  else {
    pQVar14 = *(QString **)(param_1 + 0x68);
  }
  ki18n((char *)&local_88);
  KLocalizedString::toString();
  KLocalizedString::~KLocalizedString((KLocalizedString *)&local_88);
  QLabel::setText(pQVar14);
  if ((QArrayData *)local_68._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68._0_8_ = *(int *)local_68._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_68._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_68._0_8_,2,0x10);
      lVar13 = *(long *)(param_1 + 0x70);
      goto joined_r0x0011b96a;
    }
  }
  lVar13 = *(long *)(param_1 + 0x70);
joined_r0x0011b96a:
  if ((lVar13 == 0) || (*(int *)(lVar13 + 4) == 0)) {
    pQVar14 = (QString *)0x0;
  }
  else {
    pQVar14 = *(QString **)(param_1 + 0x78);
  }
  ki18n((char *)&local_88);
  KLocalizedString::toString();
  KLocalizedString::~KLocalizedString((KLocalizedString *)&local_88);
  QLabel::setText(pQVar14);
  if ((QArrayData *)local_68._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68._0_8_ = *(int *)local_68._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_68._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_68._0_8_,2,0x10);
    }
  }
  iVar4 = QMainWindow::statusBar();
  if ((*(long *)(param_1 + 0x60) == 0) || (*(int *)(*(long *)(param_1 + 0x60) + 4) == 0)) {
    iVar12 = 0;
  }
  else {
    iVar12 = (int)*(undefined8 *)(param_1 + 0x68);
  }
  QStatusBar::insertPermanentWidget(iVar4,(QWidget *)0x0,iVar12);
  iVar4 = QMainWindow::statusBar();
  if ((*(long *)(param_1 + 0x70) == 0) || (*(int *)(*(long *)(param_1 + 0x70) + 4) == 0)) {
    iVar12 = 0;
  }
  else {
    iVar12 = (int)*(undefined8 *)(param_1 + 0x78);
  }
  QStatusBar::insertPermanentWidget(iVar4,(QWidget *)0x1,iVar12);
  QMainWindow::setCentralWidget((QWidget *)param_1);
  uVar6 = (**(code **)(*(long *)(param_1 + *(long *)(*(long *)param_1 + -0x18)) + 0x18))();
  pwStack_80 = (wchar16 *)0x0;
  local_88 = (QArrayData *)FUN_00116660;
  uVar6 = KGameStandardAction::_k_createInternal(1,uVar6);
  pwVar3 = pwStack_80;
  pQVar2 = local_88;
  local_68._8_8_ = 0;
  local_68._0_8_ = QAction::triggered;
  puVar8 = operator_new(0x20);
  *(wchar16 **)(puVar8 + 6) = pwVar3;
  *(code **)(puVar8 + 2) = FUN_00110990;
  *(QArrayData **)(puVar8 + 4) = pQVar2;
  *puVar8 = 1;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             QAction::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = (**(code **)(*(long *)(param_1 + *(long *)(*(long *)param_1 + -0x18)) + 0x18))();
  pwStack_80 = (wchar16 *)0x0;
  local_88 = (QArrayData *)FUN_001142d0;
  uVar6 = KGameStandardAction::_k_createInternal(8,uVar6);
  pwVar3 = pwStack_80;
  pQVar2 = local_88;
  local_68._8_8_ = 0;
  local_68._0_8_ = QAction::triggered;
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(code **)(puVar8 + 2) = FUN_00110990;
  *(wchar16 **)(puVar8 + 6) = pwVar3;
  *(QArrayData **)(puVar8 + 4) = pQVar2;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             QAction::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = (**(code **)(*(long *)(param_1 + *(long *)(*(long *)param_1 + -0x18)) + 0x18))();
  pwStack_80 = (wchar16 *)0x0;
  local_88 = (QArrayData *)QWidget::close;
  uVar6 = KGameStandardAction::_k_createInternal(0xb,uVar6);
  pwVar3 = pwStack_80;
  pQVar2 = local_88;
  local_68._8_8_ = 0;
  local_68._0_8_ = QAction::triggered;
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(code **)(puVar8 + 2) = FUN_00110a90;
  *(QArrayData **)(puVar8 + 4) = pQVar2;
  *(wchar16 **)(puVar8 + 6) = pwVar3;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             QAction::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = (**(code **)(*(long *)(param_1 + *(long *)(*(long *)param_1 + -0x18)) + 0x18))();
  pwStack_80 = (wchar16 *)0x0;
  local_88 = (QArrayData *)FUN_00114570;
  uVar6 = KStandardAction::_k_createInternal(0x33,uVar6);
  pwVar3 = pwStack_80;
  pQVar2 = local_88;
  local_68._8_8_ = 0;
  local_68._0_8_ = QAction::triggered;
  puVar8 = operator_new(0x20);
  *puVar8 = 1;
  *(code **)(puVar8 + 2) = FUN_00110990;
  *(wchar16 **)(puVar8 + 6) = pwVar3;
  *(QArrayData **)(puVar8 + 4) = pQVar2;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             QAction::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  uVar6 = (**(code **)(*(long *)(param_1 + *(long *)(*(long *)param_1 + -0x18)) + 0x18))();
  pwStack_80 = (wchar16 *)0x0;
  local_88 = (QArrayData *)FUN_00114150;
  uVar6 = KGameStandardAction::_k_createInternal(7,uVar6);
  pwVar3 = pwStack_80;
  pQVar2 = local_88;
  local_68._8_8_ = 0;
  local_68._0_8_ = QAction::triggered;
  puVar8 = operator_new(0x20);
  *(wchar16 **)(puVar8 + 6) = pwVar3;
  *(code **)(puVar8 + 2) = FUN_00110a10;
  *(QArrayData **)(puVar8 + 4) = pQVar2;
  *puVar8 = 1;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             QAction::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  *(undefined8 *)(param_1 + 0x58) = uVar6;
  uVar6 = KGameDifficulty::global();
  KGameDifficulty::addStandardLevelRange(uVar6,0x1e,0x32);
  pKVar11 = (KGameDifficultyLevel *)KGameDifficulty::global();
  this_03 = operator_new(0x18);
  ki18n((char *)&local_88);
  KLocalizedString::toString();
  KLocalizedString::~KLocalizedString((KLocalizedString *)&local_88);
  QByteArray::QByteArray((QByteArray *)&local_88,"Custom",-1);
  KGameDifficultyLevel::KGameDifficultyLevel
            (this_03,1000,(QByteArray *)&local_88,(QString *)local_68,false);
  KGameDifficulty::addLevel(pKVar11);
  if (local_88 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_88 = *(int *)local_88 + -1;
    UNLOCK();
    if (*(int *)local_88 == 0) {
      QArrayData::deallocate(local_88,1,0x10);
    }
  }
  if ((QArrayData *)local_68._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68._0_8_ = *(int *)local_68._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_68._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_68._0_8_,2,0x10);
    }
  }
  KGameDifficultyGUI::init((KXmlGuiWindow *)param_1,(KGameDifficulty *)0x0);
  pwStack_80 = (wchar16 *)0x0;
  local_88 = (QArrayData *)FUN_00116660;
  uVar6 = KGameDifficulty::global();
  pwVar3 = pwStack_80;
  pQVar2 = local_88;
  local_68._8_8_ = 0;
  local_68._0_8_ = KGameDifficulty::currentLevelChanged;
  puVar8 = operator_new(0x20);
  *(wchar16 **)(puVar8 + 6) = pwVar3;
  *(code **)(puVar8 + 2) = FUN_00110990;
  *puVar8 = 1;
  *(QArrayData **)(puVar8 + 4) = pQVar2;
  QObject::connectImpl
            ((Connection *)&local_a8,uVar6,(QByteArray *)local_68,param_1,&local_88,puVar8,0,0,
             KGameDifficulty::staticMetaObject);
  QMetaObject::Connection::~Connection((Connection *)&local_a8);
  local_58 = 0.0;
  local_68 = (undefined1  [16])0x0;
  QWidget::screen();
  auVar20 = QScreen::availableGeometry();
  dVar19 = (double)((auVar20._8_4_ + 1) - auVar20._0_4_) * 0.4;
  dVar18 = (double)((auVar20._12_4_ + 1) - auVar20._4_4_) * 0.4;
  dVar19 = (double)((ulong)dVar19 & 0x8000000000000000 | 0x3fe0000000000000) + dVar19;
  dVar18 = (double)((ulong)dVar18 & 0x8000000000000000 | 0x3fe0000000000000) + dVar18;
  uVar16 = -(ulong)(2147483647.0 < dVar19);
  uVar17 = -(ulong)(2147483647.0 < dVar18);
  dVar19 = (double)(~uVar16 & (ulong)dVar19 | uVar16 & 0x41dfffffffc00000);
  dVar18 = (double)(~uVar17 & (ulong)dVar18 | uVar17 & 0x41dfffffffc00000);
  uVar16 = -(ulong)(-2147483648.0 < dVar19);
  uVar17 = -(ulong)(-2147483648.0 < dVar18);
  local_88 = (QArrayData *)
             CONCAT44((int)(double)(~uVar17 & 0xc1e0000000000000 | (ulong)dVar18 & uVar17),
                      (int)(double)(~uVar16 & 0xc1e0000000000000 | (ulong)dVar19 & uVar16));
  KXmlGuiWindow::setupGUI(param_1,&local_88,0x1f,(QByteArray *)local_68);
  if ((QArrayData *)local_68._0_8_ != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68._0_8_ = *(int *)local_68._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_68._0_8_ == 0) {
      QArrayData::deallocate((QArrayData *)local_68._0_8_,2,0x10);
    }
  }
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  FUN_00116660(param_1);
  return;
}

