// rea decompile_all
// name: FUN_00120020
// address: 0x120020
// body: available
// range: 0x120020-0x1206a3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x12069f to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00120020(QRectF *param_1,int param_2,int param_3)

{
  wchar16 *pwVar1;
  long *plVar2;
  int *piVar3;
  long lVar4;
  int iVar5;
  long lVar6;
  long *plVar7;
  wchar16 *pwVar8;
  long in_FS_OFFSET;
  ulong uVar9;
  ulong uVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double local_d8;
  double dStack_d0;
  int *local_c8;
  double local_b0;
  QPixmap local_a8 [24];
  double local_90;
  QArrayData *local_88;
  wchar16 *pwStack_80;
  double local_78;
  undefined1 local_68 [16];
  double local_58;
  double dStack_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  local_68 = (undefined1  [16])0x0;
  local_58 = (double)param_2;
  dStack_50 = (double)param_3;
  QGraphicsScene::setSceneRect(param_1);
  local_c8 = (int *)0x0;
  QGraphicsScene::sceneRect();
  local_78 = 4.94065645841247e-323;
  dVar12 = (double)((ulong)local_58 & 0x8000000000000000 | 0x3fe0000000000000) + local_58;
  dVar13 = (double)((ulong)dStack_50 & 0x8000000000000000 | 0x3fe0000000000000) + dStack_50;
  uVar9 = -(ulong)(2147483647.0 < dVar12);
  uVar10 = -(ulong)(2147483647.0 < dVar13);
  dVar12 = (double)(~uVar9 & (ulong)dVar12 | uVar9 & 0x41dfffffffc00000);
  dVar13 = (double)(~uVar10 & (ulong)dVar13 | uVar10 & 0x41dfffffffc00000);
  uVar9 = -(ulong)(-2147483648.0 < dVar12);
  uVar10 = -(ulong)(-2147483648.0 < dVar13);
  local_88 = (QArrayData *)0x0;
  pwStack_80 = L"mainWidget";
  KGameRenderer::spritePixmap
            (local_a8,param_1 + 0x18,&local_88,
             CONCAT44((int)(double)(~uVar10 & 0xc1e0000000000000 | (ulong)dVar13 & uVar10),
                      (int)(double)(~uVar9 & 0xc1e0000000000000 | (ulong)dVar12 & uVar9)),0xffffffff
            );
  QBrush::QBrush((QBrush *)&local_d8,local_a8);
  QGraphicsScene::setBackgroundBrush((QBrush *)param_1);
  QBrush::~QBrush((QBrush *)&local_d8);
  QPixmap::~QPixmap(local_a8);
  if (local_88 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_88 = *(int *)local_88 + -1;
    UNLOCK();
    if (*(int *)local_88 == 0) {
      QArrayData::deallocate(local_88,2,0x10);
    }
  }
  piVar3 = local_c8;
  if ((local_c8 != (int *)0x0) && (*local_c8 != -1)) {
    LOCK();
    *local_c8 = *local_c8 + -1;
    UNLOCK();
    if ((*local_c8 == 0) && (local_c8 != (int *)0x0)) {
      lVar4 = *(long *)(local_c8 + 8);
      if (lVar4 != 0) {
        lVar6 = *(long *)(lVar4 + -8) * 0x90 + lVar4;
        if (lVar4 == lVar6) {
          lVar4 = 0;
        }
        else {
          do {
            lVar4 = lVar6;
            lVar6 = lVar4 + -0x90;
            if (*(void **)(lVar4 + -0x10) != (void *)0x0) {
              operator_delete__(*(void **)(lVar4 + -0x10));
            }
          } while (*(long *)(piVar3 + 8) != lVar6);
          lVar4 = *(long *)(lVar4 + -0x98) * 0x90;
        }
        operator_delete__((void *)(lVar6 + -8),lVar4 + 8);
      }
      operator_delete(piVar3,0x28);
    }
  }
  lVar4 = *(long *)(param_1 + 0x30);
  QGraphicsScene::sceneRect();
  QGraphicsItem::prepareGeometryChange();
  dVar12 = (double)(*(int *)(lVar4 + 0x58) + 2);
  dVar13 = (double)(*(int *)(lVar4 + 0x54) + 2);
  if (dVar12 / local_58 <= dVar13 / dStack_50) {
    dVar12 = dStack_50 / dVar13;
  }
  else {
    dVar12 = local_58 / dVar12;
  }
  iVar5 = (int)dVar12;
  plVar7 = *(long **)(lVar4 + 0x28);
  plVar2 = plVar7 + *(long *)(lVar4 + 0x30);
  *(int *)(lVar4 + 0x50) = iVar5;
  if (plVar7 != plVar2) {
    while( true ) {
      KGameRendererClient::setRenderSize(*plVar7 + 0x10,CONCAT44(iVar5,iVar5));
      QGraphicsItem::childItems();
      pwVar1 = pwStack_80 + (long)local_78 * 4;
      for (pwVar8 = pwStack_80; pwVar8 != pwVar1; pwVar8 = pwVar8 + 4) {
        KGameRendererClient::setRenderSize(*(long *)pwVar8 + 0x10,CONCAT44(iVar5,iVar5));
      }
      if (local_88 != (QArrayData *)0x0) {
        LOCK();
        *(int *)local_88 = *(int *)local_88 + -1;
        UNLOCK();
        if (*(int *)local_88 == 0) {
          QArrayData::deallocate(local_88,8,0x10);
        }
      }
      plVar7 = plVar7 + 1;
      if (plVar2 == plVar7) break;
      iVar5 = *(int *)(lVar4 + 0x50);
    }
  }
  plVar7 = *(long **)(lVar4 + 0x40);
  plVar2 = plVar7 + *(long *)(lVar4 + 0x48);
  for (; plVar7 != plVar2; plVar7 = plVar7 + 1) {
    KGameRendererClient::setRenderSize
              (*plVar7 + 0x10,CONCAT44(*(undefined4 *)(lVar4 + 0x50),*(undefined4 *)(lVar4 + 0x50)))
    ;
  }
  FUN_001196e0(lVar4);
  lVar4 = *(long *)(param_1 + 0x30);
  QGraphicsScene::sceneRect();
  plVar2 = *(long **)(param_1 + 0x30);
  dVar12 = local_b0 * 0.5;
  if (*(code **)(*plVar2 + 0x60) == FUN_001108f0) {
    dVar13 = (double)((*(int *)((long)plVar2 + 0x54) + 2) * (int)plVar2[10]);
  }
  else {
    (**(code **)(*plVar2 + 0x60))(local_a8);
    dVar13 = local_90;
  }
  QGraphicsScene::sceneRect();
  plVar2 = *(long **)(param_1 + 0x30);
  local_d8 = local_78 * 0.5;
  if (*(code **)(*plVar2 + 0x60) == FUN_001108f0) {
    dVar11 = (double)(((int)plVar2[0xb] + 2) * (int)plVar2[10]);
  }
  else {
    (**(code **)(*plVar2 + 0x60))(local_68);
    dVar11 = local_58;
  }
  local_d8 = local_d8 - dVar11 * 0.5;
  dStack_d0 = dVar12 - dVar13 * 0.5;
  QGraphicsItem::setPos((QPointF *)(lVar4 + 0x10));
  lVar4 = *(long *)(param_1 + 0x40);
  QGraphicsScene::sceneRect();
  dVar12 = local_b0;
  (**(code **)(**(long **)(param_1 + 0x40) + 0x60))(local_a8);
  dVar13 = local_90;
  QGraphicsScene::sceneRect();
  dVar11 = local_78 * 0.5;
  (**(code **)(**(long **)(param_1 + 0x40) + 0x60))(local_68);
  local_d8 = dVar11 - local_58 * 0.5;
  dStack_d0 = dVar12 * 0.5 - dVar13 * 0.5;
  QGraphicsItem::setPos((QPointF *)(lVar4 + 0x10));
  lVar4 = *(long *)(param_1 + 0x38);
  QGraphicsScene::sceneRect();
  (**(code **)(**(long **)(param_1 + 0x38) + 0x60))(local_a8);
  QGraphicsScene::sceneRect();
  dVar12 = local_78 * 0.5;
  (**(code **)(**(long **)(param_1 + 0x38) + 0x60))(local_68);
  local_d8 = dVar12 - local_58 * 0.5;
  dStack_d0 = local_b0 * 0.5 - local_90 * 0.5;
  QGraphicsItem::setPos((QPointF *)(lVar4 + 0x10));
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

