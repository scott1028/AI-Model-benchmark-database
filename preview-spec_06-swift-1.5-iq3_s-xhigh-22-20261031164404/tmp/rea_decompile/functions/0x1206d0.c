// rea decompile_all
// name: FUN_001206d0
// address: 0x1206d0
// body: available
// range: 0x1206d0-0x120a2a inclusive
// range: 0x120a30-0x121376 inclusive
// range: 0x121380-0x121394 inclusive
// range: 0x121398-0x1215b2 inclusive
// range: 0x1215b8-0x1215e5 inclusive
// range: 0x1215f0-0x121608 inclusive
// range: 0x121610-0x121869 inclusive
// range: 0x121870-0x1218a0 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x12152b to 0x120020 FUN_00120020 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x12189c to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001206d0(long param_1,int param_2,int param_3,int param_4)

{
  void *__s;
  QObject *pQVar1;
  QArrayData *pQVar2;
  long lVar3;
  int iVar4;
  QGraphicsItem *pQVar5;
  KGameRenderedItem *pKVar6;
  int *piVar7;
  undefined8 *puVar8;
  long lVar9;
  int iVar10;
  undefined8 *puVar11;
  long lVar12;
  int iVar13;
  long lVar14;
  int iVar15;
  long lVar16;
  int iVar17;
  long lVar18;
  long lVar19;
  long in_FS_OFFSET;
  undefined4 local_9c;
  undefined4 local_98 [6];
  double local_80;
  undefined1 local_78 [16];
  double local_68;
  void *local_58;
  undefined4 *puStack_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  KGamePopupItem::forceHide(*(undefined8 *)(param_1 + 0x38),1);
  pQVar1 = *(QObject **)(param_1 + 0x30);
  iVar17 = param_2 * param_3;
  *(undefined2 *)(pQVar1 + 0x88) = 1;
  lVar16 = (long)iVar17;
  if (iVar17 + -10 <= param_4) {
    param_4 = iVar17 + -10;
  }
  iVar13 = param_3 + 4 + param_2;
  iVar15 = iVar13 * 2 + -4;
  iVar4 = (int)*(undefined8 *)(pQVar1 + 0x30);
  iVar10 = (int)*(undefined8 *)(pQVar1 + 0x48);
  if (iVar17 < iVar4) {
    lVar14 = lVar16 * 8;
    do {
      pQVar5 = (QGraphicsItem *)QGraphicsItem::scene();
      if ((*(int **)(pQVar1 + 0x20) == (int *)0x0) || (1 < **(int **)(pQVar1 + 0x20))) {
        FUN_0011d400(pQVar1 + 0x20,0,0);
      }
      QGraphicsScene::removeItem(pQVar5);
      if ((*(int **)(pQVar1 + 0x20) == (int *)0x0) || (1 < **(int **)(pQVar1 + 0x20))) {
        FUN_0011d400(pQVar1 + 0x20,0,0);
      }
      if (*(long **)(*(long *)(pQVar1 + 0x28) + lVar14) != (long *)0x0) {
        (**(code **)(**(long **)(*(long *)(pQVar1 + 0x28) + lVar14) + 8))();
      }
      lVar14 = lVar14 + 8;
    } while (lVar14 != ((ulong)((iVar4 - iVar17) - 1) + 1 + lVar16) * 8);
    if (iVar15 < iVar10) {
      lVar14 = ((long)iVar13 + -2) * 0x10;
      do {
        pQVar5 = (QGraphicsItem *)QGraphicsItem::scene();
        if ((*(int **)(pQVar1 + 0x38) == (int *)0x0) || (1 < **(int **)(pQVar1 + 0x38))) {
          FUN_0011c0c0(pQVar1 + 0x38,0);
        }
        QGraphicsScene::removeItem(pQVar5);
        if ((*(int **)(pQVar1 + 0x38) == (int *)0x0) || (1 < **(int **)(pQVar1 + 0x38))) {
          FUN_0011c0c0(pQVar1 + 0x38,0);
        }
        if (*(long **)(*(long *)(pQVar1 + 0x40) + lVar14) != (long *)0x0) {
          (**(code **)(**(long **)(*(long *)(pQVar1 + 0x40) + lVar14) + 8))();
        }
        lVar14 = lVar14 + 8;
      } while ((((ulong)(uint)(iVar10 + 3 + iVar13 * -2) - 3) + (long)iVar13 * 2) * 8 != lVar14);
    }
  }
  piVar7 = *(int **)(pQVar1 + 0x20);
  if (piVar7 == (int *)0x0) {
    lVar14 = lVar16 - *(long *)(pQVar1 + 0x30);
LAB_0012094b:
    FUN_0011d400(pQVar1 + 0x20,0,lVar14);
LAB_0012096e:
    lVar14 = *(long *)(pQVar1 + 0x30);
    if (lVar16 <= lVar14) goto LAB_0012097b;
LAB_00120be3:
    if (lVar14 * 8 != lVar16 * 8) {
      puVar8 = (undefined8 *)(lVar14 * 8 + *(long *)(pQVar1 + 0x28));
      puVar11 = (undefined8 *)(*(long *)(pQVar1 + 0x28) + lVar16 * 8);
      *puVar8 = 0;
      if (puVar11 != puVar8 + 1) {
        memset(puVar8 + 1,0,(long)puVar11 + (-8 - (long)puVar8));
      }
    }
    piVar7 = *(int **)(pQVar1 + 0x38);
    *(long *)(pQVar1 + 0x30) = lVar16;
    if (piVar7 == (int *)0x0) goto LAB_00120c50;
LAB_0012098f:
    lVar14 = (long)iVar15;
    if ((1 < *piVar7) ||
       (*(long *)(piVar7 + 2) -
        ((long)(*(long *)(pQVar1 + 0x40) - ((long)piVar7 + 0x1fU & 0xfffffffffffffff0)) >> 3) <
        lVar14)) {
      lVar14 = lVar14 - *(long *)(pQVar1 + 0x48);
      if (1 < *piVar7) goto LAB_001209b0;
      if (lVar14 != 0) {
        lVar18 = *(long *)(pQVar1 + 0x38);
        if (lVar18 == 0) {
          if (0 < lVar14) goto LAB_001209b0;
        }
        else {
          lVar9 = *(long *)(pQVar1 + 0x40);
          lVar3 = *(long *)(pQVar1 + 0x48);
          lVar19 = lVar9 - (lVar18 + 0x1fU & 0xfffffffffffffff0);
          lVar12 = lVar19 >> 3;
          if ((*(long *)(lVar18 + 8) - lVar12) - lVar3 < lVar14) {
            if ((lVar12 < lVar14) || (*(long *)(lVar18 + 8) * 2 <= lVar3 * 3)) goto LAB_001209b0;
            lVar14 = ((lVar9 << 0x3d) - lVar19) + lVar9;
            FUN_0011dca0(lVar9,lVar3,lVar14);
            *(long *)(pQVar1 + 0x40) = lVar14;
          }
        }
      }
      goto LAB_001209d1;
    }
    if (*(long *)(pQVar1 + 0x48) <= lVar14) goto LAB_001209d1;
    *(long *)(pQVar1 + 0x48) = lVar14;
    lVar18 = *(long *)(pQVar1 + 0x48);
    if (lVar18 < lVar14) goto LAB_001216c9;
  }
  else {
    if ((1 < *piVar7) ||
       (*(long *)(piVar7 + 2) -
        ((long)(*(long *)(pQVar1 + 0x28) - ((long)piVar7 + 0x1fU & 0xfffffffffffffff0)) >> 3) <
        lVar16)) {
      lVar14 = lVar16 - *(long *)(pQVar1 + 0x30);
      if (1 < *piVar7) goto LAB_0012094b;
      if (lVar14 != 0) {
        lVar18 = *(long *)(pQVar1 + 0x20);
        if (lVar18 == 0) {
          if (0 < lVar14) goto LAB_0012094b;
        }
        else {
          lVar9 = *(long *)(pQVar1 + 0x28);
          lVar3 = *(long *)(pQVar1 + 0x30);
          lVar19 = lVar9 - (lVar18 + 0x1fU & 0xfffffffffffffff0);
          lVar12 = lVar19 >> 3;
          if ((*(long *)(lVar18 + 8) - lVar12) - lVar3 < lVar14) {
            if ((lVar12 < lVar14) || (*(long *)(lVar18 + 8) * 2 <= lVar3 * 3)) goto LAB_0012094b;
            lVar14 = ((lVar9 << 0x3d) - lVar19) + lVar9;
            FUN_0011dca0(lVar9,lVar3,lVar14);
            *(long *)(pQVar1 + 0x28) = lVar14;
          }
        }
      }
      goto LAB_0012096e;
    }
    if (*(long *)(pQVar1 + 0x30) <= lVar16) goto LAB_0012096e;
    *(long *)(pQVar1 + 0x30) = lVar16;
    lVar14 = *(long *)(pQVar1 + 0x30);
    if (lVar14 < lVar16) goto LAB_00120be3;
LAB_0012097b:
    piVar7 = *(int **)(pQVar1 + 0x38);
    if (piVar7 != (int *)0x0) goto LAB_0012098f;
LAB_00120c50:
    lVar14 = (long)iVar15 - *(long *)(pQVar1 + 0x48);
LAB_001209b0:
    FUN_0011c0c0(pQVar1 + 0x38,lVar14);
LAB_001209d1:
    lVar18 = *(long *)(pQVar1 + 0x48);
    if (lVar18 < iVar15) {
LAB_001216c9:
      lVar9 = lVar18 * 8;
      lVar14 = (long)iVar15 * 8;
      if (lVar9 != lVar14) {
        lVar3 = *(long *)(pQVar1 + 0x40);
        *(undefined8 *)(lVar3 + lVar9) = 0;
        __s = (void *)(lVar3 + 8 + lVar9);
        if (__s != (void *)(lVar3 + lVar14)) {
          memset(__s,0,(lVar14 + lVar18 * -8) - 8);
        }
      }
      *(long *)(pQVar1 + 0x48) = (long)iVar15;
    }
  }
  lVar14 = 0;
  *(int *)(pQVar1 + 0x8c) = iVar17;
  *(int *)(pQVar1 + 0x54) = param_2;
  *(undefined4 *)(pQVar1 + 0x78) = 0xffffffff;
  *(undefined4 *)(pQVar1 + 0x7c) = 0xffffffff;
  *(undefined4 *)(pQVar1 + 0x80) = 0xffffffff;
  *(undefined4 *)(pQVar1 + 0x84) = 0xffffffff;
  *(int *)(pQVar1 + 0x58) = param_3;
  *(int *)(pQVar1 + 0x5c) = param_4;
  if (0 < iVar17) {
    do {
      if ((int)lVar14 < iVar4) {
        if ((*(int **)(pQVar1 + 0x20) == (int *)0x0) || (1 < **(int **)(pQVar1 + 0x20))) {
          FUN_0011d400(pQVar1 + 0x20,0,0);
        }
        lVar18 = *(long *)(*(long *)(pQVar1 + 0x28) + lVar14 * 8);
        *(undefined4 *)(lVar18 + 0x28) = 0;
        *(undefined2 *)(lVar18 + 0x2c) = 0;
        *(undefined4 *)(lVar18 + 0x30) = 0;
        FUN_00117930();
        piVar7 = *(int **)(pQVar1 + 0x20);
      }
      else {
        pKVar6 = operator_new(0x38);
        local_78 = (undefined1  [16])0x0;
        local_68 = 0.0;
        KGameRenderedItem::KGameRenderedItem
                  (pKVar6,*(KGameRenderer **)(pQVar1 + 0x90),(QString *)local_78,
                   (QGraphicsItem *)(pQVar1 + 0x10));
        if ((QArrayData *)local_78._0_8_ != (QArrayData *)0x0) {
          LOCK();
          *(int *)local_78._0_8_ = *(int *)local_78._0_8_ + -1;
          UNLOCK();
          if (*(int *)local_78._0_8_ == 0) {
            QArrayData::deallocate((QArrayData *)local_78._0_8_,2,0x10);
          }
        }
        *(undefined ***)pKVar6 = &PTR_FUN_001281e8;
        *(undefined ***)(pKVar6 + 0x10) = &PTR_FUN_00128328;
        if ((DAT_001292a8 == 0) || (*(long *)(DAT_001292a8 + 8) == 0)) {
          FUN_00118810();
        }
        QGraphicsPixmapItem::setShapeMode(pKVar6);
        *(undefined4 *)(pKVar6 + 0x28) = 0;
        *(undefined2 *)(pKVar6 + 0x2c) = 0;
        *(undefined4 *)(pKVar6 + 0x30) = 0;
        FUN_00117930(pKVar6);
        piVar7 = *(int **)(pQVar1 + 0x20);
        if ((piVar7 == (int *)0x0) || (1 < *piVar7)) {
          FUN_0011d400(pQVar1 + 0x20,0,0);
          piVar7 = *(int **)(pQVar1 + 0x20);
        }
        *(KGameRenderedItem **)(*(long *)(pQVar1 + 0x28) + lVar14 * 8) = pKVar6;
      }
      lVar18 = lVar14 * 8;
      if ((piVar7 == (int *)0x0) || (1 < *piVar7)) {
        FUN_0011d400(pQVar1 + 0x20,0,0);
      }
      lVar14 = lVar14 + 1;
      *(undefined4 *)(*(long *)(*(long *)(pQVar1 + 0x28) + lVar18) + 0x30) = 0;
      FUN_00117930();
    } while (lVar14 != lVar16);
  }
  if (iVar10 < iVar15) {
    lVar16 = (long)iVar10 * 8;
    do {
      pKVar6 = operator_new(0x38);
      local_68 = 0.0;
      local_78 = (undefined1  [16])0x0;
      KGameRenderedItem::KGameRenderedItem
                (pKVar6,*(KGameRenderer **)(pQVar1 + 0x90),(QString *)local_78,
                 (QGraphicsItem *)(pQVar1 + 0x10));
      if ((QArrayData *)local_78._0_8_ != (QArrayData *)0x0) {
        LOCK();
        *(int *)local_78._0_8_ = *(int *)local_78._0_8_ + -1;
        UNLOCK();
        if (*(int *)local_78._0_8_ == 0) {
          QArrayData::deallocate((QArrayData *)local_78._0_8_,2,0x10);
        }
      }
      *(undefined4 *)(pKVar6 + 0x28) = 2;
      *(undefined ***)pKVar6 = &PTR_FUN_00128350;
      *(undefined ***)(pKVar6 + 0x10) = &PTR_FUN_00128490;
      *(undefined8 *)(pKVar6 + 0x2c) = 0xffffffffffffffff;
      if ((DAT_001292b8 == 0) || (*(long *)(DAT_001292b8 + 8) == 0)) {
        local_9c = 0;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x11;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.edge.north";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 1;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x11;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.edge.south";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 2;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x10;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.edge.east";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 3;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x10;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.edge.west";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 6;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x17;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.outsideCorner.ne";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 4;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x17;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.outsideCorner.nw";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 5;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x17;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.outsideCorner.sw";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
        local_9c = 7;
        FUN_00116ca0((QString *)local_78,&local_9c);
        lVar14 = ((ulong)local_78._8_8_ >> 7) * 0x90 + *(long *)(local_78._0_8_ + 0x20);
        lVar14 = (ulong)*(byte *)(lVar14 + (ulong)(local_78._8_4_ & 0x7f)) * 0x20 +
                 *(long *)(lVar14 + 0x80);
        pQVar2 = *(QArrayData **)(lVar14 + 8);
        *(undefined8 *)(lVar14 + 0x18) = 0x17;
        *(undefined8 *)(lVar14 + 8) = 0;
        *(wchar16 **)(lVar14 + 0x10) = L"border.outsideCorner.se";
        if (pQVar2 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar2 = *(int *)pQVar2 + -1;
          UNLOCK();
          if (*(int *)pQVar2 == 0) {
            QArrayData::deallocate(pQVar2,2,0x10);
          }
        }
      }
      QGraphicsPixmapItem::setShapeMode(pKVar6,1);
      if ((*(int **)(pQVar1 + 0x38) == (int *)0x0) || (1 < **(int **)(pQVar1 + 0x38))) {
        FUN_0011c0c0(pQVar1 + 0x38,0);
      }
      *(KGameRenderedItem **)(*(long *)(pQVar1 + 0x40) + lVar16) = pKVar6;
      lVar16 = lVar16 + 8;
    } while (((long)iVar10 + 1 + (ulong)(uint)((iVar13 * 2 + -5) - iVar10)) * 8 != lVar16);
  }
  if (-2 < *(int *)(pQVar1 + 0x54)) {
    iVar15 = 0;
    iVar17 = 0;
    if (*(int *)(pQVar1 + 0x58) < -1) goto LAB_001212d4;
    iVar17 = 1;
    lVar16 = **(long **)(pQVar1 + 0x40);
    *(undefined8 *)(lVar16 + 0x2c) = 0;
    *(undefined4 *)(lVar16 + 0x28) = 4;
    FUN_00116ca0((QString *)local_78,lVar16 + 0x28);
    KGameRendererClient::setSpriteKey((QString *)(lVar16 + 0x10));
    iVar15 = 0;
    do {
      iVar4 = *(int *)(pQVar1 + 0x58);
      iVar10 = 0;
      if (-1 < iVar4) {
        do {
          iVar13 = iVar10 + 1;
          if (iVar15 == 0) goto LAB_00121408;
          if (iVar15 == *(int *)(pQVar1 + 0x54) + 1) {
            if (iVar10 == iVar4) {
              lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
              *(int *)(lVar16 + 0x2c) = iVar15;
              *(int *)(lVar16 + 0x30) = iVar13;
              *(undefined4 *)(lVar16 + 0x28) = 7;
            }
            else {
              lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
              *(int *)(lVar16 + 0x2c) = iVar15;
              *(int *)(lVar16 + 0x30) = iVar13;
              *(undefined4 *)(lVar16 + 0x28) = 1;
            }
            FUN_00116ca0((QString *)local_78,lVar16 + 0x28);
            KGameRendererClient::setSpriteKey((QString *)(lVar16 + 0x10));
            iVar4 = *(int *)(pQVar1 + 0x58);
            iVar17 = iVar17 + 1;
          }
          else if (iVar10 == iVar4) {
            lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
            *(int *)(lVar16 + 0x2c) = iVar15;
            *(int *)(lVar16 + 0x30) = iVar13;
            *(undefined4 *)(lVar16 + 0x28) = 2;
            FUN_00116ca0((QString *)local_78,lVar16 + 0x28);
            KGameRendererClient::setSpriteKey((QString *)(lVar16 + 0x10));
            iVar4 = *(int *)(pQVar1 + 0x58);
            iVar17 = iVar17 + 1;
          }
          iVar10 = iVar13;
        } while (iVar13 <= iVar4);
      }
LAB_001212d4:
      do {
        iVar4 = iVar15;
        if (*(int *)(pQVar1 + 0x54) < iVar15) goto LAB_00121496;
LAB_001212e1:
        iVar15 = iVar4 + 1;
      } while (*(int *)(pQVar1 + 0x58) < -1);
      if (iVar4 == *(int *)(pQVar1 + 0x54)) {
        lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
        *(int *)(lVar16 + 0x2c) = iVar15;
        *(undefined4 *)(lVar16 + 0x30) = 0;
        *(undefined4 *)(lVar16 + 0x28) = 5;
      }
      else {
        lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
        *(int *)(lVar16 + 0x2c) = iVar15;
        *(undefined4 *)(lVar16 + 0x30) = 0;
        *(undefined4 *)(lVar16 + 0x28) = 3;
      }
      iVar17 = iVar17 + 1;
      FUN_00116ca0((QString *)local_78,lVar16 + 0x28);
      KGameRendererClient::setSpriteKey((QString *)(lVar16 + 0x10));
    } while( true );
  }
LAB_00121496:
  FUN_001196e0(pQVar1);
  *(undefined4 *)(pQVar1 + 0x60) = 0;
  local_98[0] = 0;
  local_58 = (void *)0x0;
  puStack_50 = local_98;
  QMetaObject::activate(pQVar1,(QMetaObject *)&PTR_staticMetaObject_00127a00,0,&local_58);
  QGraphicsScene::sceneRect();
  QGraphicsScene::sceneRect();
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    FUN_00120020(param_1,(int)local_68,(int)local_80);
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
LAB_00121408:
  while( true ) {
    if (iVar4 + 1 == iVar13) {
      lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
      *(int *)(lVar16 + 0x30) = iVar13;
      *(undefined8 *)(lVar16 + 0x28) = 6;
      FUN_00116ca0((QString *)local_78,lVar16 + 0x28);
      KGameRendererClient::setSpriteKey((QString *)(lVar16 + 0x10));
      iVar4 = *(int *)(pQVar1 + 0x58);
    }
    else {
      lVar16 = *(long *)(*(long *)(pQVar1 + 0x40) + (long)iVar17 * 8);
      *(int *)(lVar16 + 0x30) = iVar13;
      *(undefined8 *)(lVar16 + 0x28) = 0;
      FUN_00116ca0((QString *)local_78,lVar16 + 0x28);
      KGameRendererClient::setSpriteKey((QString *)(lVar16 + 0x10));
      iVar4 = *(int *)(pQVar1 + 0x58);
    }
    if (iVar4 < iVar13) break;
    iVar17 = iVar17 + 1;
    iVar13 = iVar13 + 1;
  }
  iVar17 = iVar17 + 1;
  iVar4 = iVar15;
  if (-1 < *(int *)(pQVar1 + 0x54)) goto LAB_001212e1;
  goto LAB_00121496;
}

