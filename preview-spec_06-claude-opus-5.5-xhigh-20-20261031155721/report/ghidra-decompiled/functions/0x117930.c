// rea decompile_all
// name: FUN_00117930
// address: 0x117930
// body: available
// range: 0x117930-0x117ac0 inclusive
// range: 0x117ac8-0x117c54 inclusive
// range: 0x117c58-0x117c6c inclusive
// range: 0x117c70-0x117d25 inclusive
// range: 0x117d30-0x117e62 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x117e5e to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00117930(QGraphicsItem *param_1)

{
  byte bVar1;
  int *piVar2;
  QArrayData *pQVar3;
  long lVar4;
  KGameRenderedItem *pKVar5;
  KGameRenderer *pKVar6;
  undefined8 uVar7;
  long lVar8;
  long *plVar9;
  long lVar10;
  undefined8 *puVar11;
  QArrayData *pQVar12;
  long in_FS_OFFSET;
  undefined8 *puVar13;
  undefined8 *local_d8;
  QArrayData *local_d0;
  undefined8 *local_c0;
  QArrayData *local_98;
  undefined8 *puStack_90;
  long local_88;
  QArrayData *local_78;
  long *local_70;
  long local_68;
  long local_58;
  wchar16 *pwStack_50;
  undefined8 local_48;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  QGraphicsItem::childItems();
  for (plVar9 = local_70; plVar9 != local_70 + local_68; plVar9 = plVar9 + 1) {
    if ((long *)*plVar9 != (long *)0x0) {
      (**(code **)(*(long *)*plVar9 + 8))();
    }
  }
  FUN_00111d60((QString *)&local_58,param_1 + 0x28);
  lVar8 = ((ulong)pwStack_50 >> 7) * 0x90 + *(long *)(local_58 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar8 + (ulong)((uint)pwStack_50 & 0x7f)) * 0x20 +
          *(long *)(lVar8 + 0x80);
  pQVar12 = *(QArrayData **)(lVar4 + 8);
  local_d8 = *(undefined8 **)(lVar4 + 0x10);
  lVar8 = *(long *)(lVar4 + 0x18);
  if (pQVar12 != (QArrayData *)0x0) {
    local_d0 = *(QArrayData **)(lVar4 + 8);
    puVar11 = *(undefined8 **)(lVar4 + 0x10);
    LOCK();
    *(int *)pQVar12 = *(int *)pQVar12 + 1;
    UNLOCK();
    if (*(int *)pQVar12 < 2) goto LAB_00117a60;
  }
  local_98 = pQVar12;
  puStack_90 = local_d8;
  local_88 = lVar8;
  FUN_00113710(&local_98,0,0);
  local_d0 = local_98;
  puVar11 = puStack_90;
  lVar8 = local_88;
  pQVar12 = local_98;
  local_d8 = puStack_90;
LAB_00117a60:
  KGameRendererClient::setSpriteKey((QString *)(param_1 + 0x10));
  pQVar3 = pQVar12;
  if (1 < lVar8) {
    lVar4 = 0;
    puVar13 = local_d8;
    local_c0 = local_d8;
    do {
      local_d8 = puVar11;
      if ((pQVar12 == (QArrayData *)0x0) || (1 < *(int *)pQVar12)) {
        local_98 = pQVar12;
        puStack_90 = puVar13;
        local_88 = lVar8;
        FUN_00113710(&local_98,0,0);
        local_d0 = local_98;
        local_c0 = puStack_90;
        local_d8 = puStack_90;
        lVar8 = local_88;
        pQVar12 = local_98;
        puVar13 = puStack_90;
      }
      lVar10 = lVar4 + 2;
      pKVar5 = operator_new(0x28);
      pKVar6 = (KGameRenderer *)KGameRendererClient::renderer();
      KGameRenderedItem::KGameRenderedItem
                (pKVar5,pKVar6,(QString *)(local_c0 + (lVar4 + 1) * 3),param_1);
      KGameRendererClient::renderSize();
      KGameRendererClient::setRenderSize(pKVar5 + 0x10);
      lVar4 = lVar4 + 1;
      puVar11 = local_d8;
      pQVar3 = local_d0;
    } while (lVar10 < lVar8);
  }
  local_d0 = pQVar3;
  if (*(int *)(param_1 + 0x28) == 2) {
    if (*(int *)(param_1 + 0x30) == 0) {
      if (param_1[0x2c] != (QGraphicsItem)0x0) {
        if (param_1[0x2d] != (QGraphicsItem)0x0) {
          local_48 = 9;
          local_58 = 0;
          pwStack_50 = L"explosion";
          pKVar5 = operator_new(0x28);
          pKVar6 = (KGameRenderer *)KGameRendererClient::renderer();
          KGameRenderedItem::KGameRenderedItem(pKVar5,pKVar6,(QString *)&local_58,param_1);
          uVar7 = KGameRendererClient::renderSize();
          KGameRendererClient::setRenderSize(pKVar5 + 0x10,uVar7);
          FUN_00116630((QString *)&local_58);
        }
        pwStack_50 = L"mine";
        local_58 = 0;
        local_48 = 4;
        pKVar5 = operator_new(0x28);
        pKVar6 = (KGameRenderer *)KGameRendererClient::renderer();
        KGameRenderedItem::KGameRenderedItem(pKVar5,pKVar6,(QString *)&local_58,param_1);
        uVar7 = KGameRendererClient::renderSize();
        KGameRendererClient::setRenderSize(pKVar5 + 0x10,uVar7);
        FUN_00116630((QString *)&local_58);
      }
    }
    else {
      FUN_00112a20((QString *)&local_58,param_1 + 0x30);
      lVar4 = ((ulong)pwStack_50 >> 7) * 0x90 + *(long *)(local_58 + 0x20);
      bVar1 = *(byte *)(lVar4 + (ulong)((uint)pwStack_50 & 0x7f));
      lVar4 = *(long *)(lVar4 + 0x80);
      pKVar5 = operator_new(0x28);
      pKVar6 = (KGameRenderer *)KGameRendererClient::renderer();
      KGameRenderedItem::KGameRenderedItem
                (pKVar5,pKVar6,(QString *)((ulong)bVar1 * 0x20 + lVar4 + 8),param_1);
      uVar7 = KGameRendererClient::renderSize();
      KGameRendererClient::setRenderSize(pKVar5 + 0x10,uVar7);
    }
  }
  if (local_d0 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_d0 = *(int *)local_d0 + -1;
    UNLOCK();
    if (*(int *)local_d0 == 0) {
      puVar11 = local_d8 + lVar8 * 3;
      for (; local_d8 != puVar11; local_d8 = local_d8 + 3) {
        piVar2 = (int *)*local_d8;
        if (piVar2 != (int *)0x0) {
          LOCK();
          *piVar2 = *piVar2 + -1;
          UNLOCK();
          if (*piVar2 == 0) {
            QArrayData::deallocate((QArrayData *)*local_d8,2,0x10);
          }
        }
      }
      QArrayData::deallocate(local_d0,0x18,0x10);
    }
  }
  if (local_78 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_78 = *(int *)local_78 + -1;
    UNLOCK();
    if (*(int *)local_78 == 0) {
      QArrayData::deallocate(local_78,8,0x10);
    }
  }
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

