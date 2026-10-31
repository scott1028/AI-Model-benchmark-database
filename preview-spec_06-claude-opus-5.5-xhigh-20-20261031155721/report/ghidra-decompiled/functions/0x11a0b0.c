// rea decompile_all
// name: FUN_0011a0b0
// address: 0x11a0b0
// body: available
// range: 0x11a0b0-0x11a1cf inclusive
// range: 0x11a1e0-0x11a347 inclusive
// range: 0x11a350-0x11a3c6 inclusive
// range: 0x11a3e0-0x11a603 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11a5ff to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011a0b0(long param_1,int param_2)

{
  int *piVar1;
  size_t __n;
  int iVar2;
  long *plVar3;
  uint uVar4;
  void *pvVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  QArrayData *pQVar9;
  long *plVar10;
  QArrayData *__src;
  QArrayData *pQVar11;
  long in_FS_OFFSET;
  QArrayData **local_ab0;
  int local_a8c;
  long local_a88;
  QArrayData *local_a78;
  long *local_a70;
  long local_a68;
  QArrayData *local_a58;
  QArrayData *local_a50;
  long local_a48;
  uint local_a38;
  uint uStack_a34;
  long *local_a30;
  long local_a28;
  QRandomGenerator local_a18 [2520];
  long local_40;
  
  pQVar11 = (QArrayData *)0x0;
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  local_a8c = *(int *)(param_1 + 0x5c);
  FUN_00119c40(&local_a78,param_1,(long)param_2 / (long)*(int *)(param_1 + 0x58) & 0xffffffff,
               (long)param_2 % (long)*(int *)(param_1 + 0x58) & 0xffffffff);
  pvVar5 = (void *)QRandomGenerator64::global();
  local_a38 = QRandomGenerator::_fillRange(pvVar5,0);
  QRandomGenerator::QRandomGenerator(local_a18,&local_a38,&uStack_a34);
  local_a88 = 0;
  __src = pQVar11;
joined_r0x0011a173:
  if (local_a8c == 0) {
    pQVar9 = __src + local_a88 * 4;
    do {
      if (pQVar9 == __src) {
        if (local_a78 != (QArrayData *)0x0) {
          LOCK();
          *(int *)local_a78 = *(int *)local_a78 + -1;
          UNLOCK();
          if (*(int *)local_a78 == 0) {
            QArrayData::deallocate(local_a78,8,0x10);
          }
        }
        if (pQVar11 != (QArrayData *)0x0) {
          LOCK();
          *(int *)pQVar11 = *(int *)pQVar11 + -1;
          UNLOCK();
          if (*(int *)pQVar11 == 0) {
            QArrayData::deallocate(pQVar11,4,0x10);
          }
        }
        if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
          __stack_chk_fail();
        }
        return;
      }
      FUN_00119c40(&local_a38,param_1,
                   (long)*(int *)__src / (long)*(int *)(param_1 + 0x58) & 0xffffffff,
                   (long)*(int *)__src % (long)*(int *)(param_1 + 0x58) & 0xffffffff);
      plVar3 = local_a30 + local_a28;
      for (plVar10 = local_a30; plVar3 != plVar10; plVar10 = plVar10 + 1) {
        while (*(char *)(*plVar10 + 0x2c) == '\0') {
          piVar1 = (int *)(*plVar10 + 0x30);
          *piVar1 = *piVar1 + 1;
          plVar10 = plVar10 + 1;
          FUN_00117930();
          if (plVar3 == plVar10) goto LAB_0011a405;
        }
      }
LAB_0011a405:
      piVar1 = (int *)CONCAT44(uStack_a34,local_a38);
      if (piVar1 != (int *)0x0) {
        LOCK();
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (*piVar1 == 0) {
          QArrayData::deallocate((QArrayData *)CONCAT44(uStack_a34,local_a38),8,0x10);
        }
      }
      __src = __src + 4;
    } while( true );
  }
  do {
    iVar8 = *(int *)(param_1 + 0x54);
    iVar2 = *(int *)(param_1 + 0x58);
    uVar4 = QRandomGenerator::_fillRange(local_a18,0);
    lVar6 = (ulong)uVar4 * (ulong)(uint)(iVar8 * iVar2);
    lVar7 = *(long *)(*(long *)(param_1 + 0x28) + (lVar6 >> 0x20) * 8);
  } while (*(char *)(lVar7 + 0x2c) != '\0');
  if (0 < local_a68) goto code_r0x0011a1c0;
  goto LAB_0011a1f6;
code_r0x0011a1c0:
  plVar3 = local_a70;
  do {
    plVar10 = plVar3;
    if (local_a70 + local_a68 == plVar10) goto LAB_0011a1f6;
    plVar3 = plVar10 + 1;
  } while (lVar7 != *plVar10);
  if ((long)plVar10 - (long)local_a70 != -8) goto joined_r0x0011a173;
LAB_0011a1f6:
  iVar8 = (int)((ulong)lVar6 >> 0x20);
  if (param_2 == iVar8) goto joined_r0x0011a173;
  *(undefined1 *)(lVar7 + 0x2c) = 1;
  if (pQVar11 == (QArrayData *)0x0) {
LAB_0011a4a1:
    local_a48 = local_a88;
    local_a58 = pQVar11;
    local_a50 = __src;
    FUN_0011d740(&local_a58,0,1);
    lVar7 = local_a48;
    __src = local_a50;
    pQVar11 = local_a58;
    pQVar9 = local_a50 + local_a88 * 4;
    if (local_a88 < local_a48) {
      memmove(pQVar9 + 4,pQVar9,(local_a48 - local_a88) * 4);
    }
    local_a88 = lVar7 + 1;
    *(int *)pQVar9 = iVar8;
    if (pQVar11 != (QArrayData *)0x0) {
LAB_0011a2c9:
      iVar8 = *(int *)pQVar11;
      goto joined_r0x0011a5f7;
    }
  }
  else {
    if (1 < *(int *)pQVar11) {
LAB_0011a21b:
      __n = local_a88 * 4;
      if (*(int *)pQVar11 < 2) {
        lVar7 = (long)__src - ((ulong)(pQVar11 + 0x1f) & 0xfffffffffffffff0);
        if (local_a88 < *(long *)(pQVar11 + 8) - (lVar7 >> 2)) {
          local_a88 = local_a88 + 1;
          *(int *)(__src + __n) = iVar8;
          iVar8 = *(int *)pQVar11;
          goto joined_r0x0011a5f7;
        }
        if ((0 < lVar7) && (local_a88 * 3 < *(long *)(pQVar11 + 8) * 2)) {
          pQVar9 = __src + (((long)__src << 0x3e) - lVar7);
          if ((local_a88 != 0) &&
             (((pQVar9 != __src && (__src != (QArrayData *)0x0)) && (pQVar9 != (QArrayData *)0x0))))
          {
            pQVar9 = memmove(pQVar9,__src,__n);
          }
          local_a88 = local_a88 + 1;
          *(int *)(pQVar9 + __n) = iVar8;
          __src = pQVar9;
          goto LAB_0011a2c9;
        }
      }
      goto LAB_0011a4a1;
    }
    if (*(long *)(pQVar11 + 8) -
        ((long)__src - (long)((ulong)(pQVar11 + 0x1f) & 0xfffffffffffffff0) >> 2) != local_a88) {
      *(int *)(__src + local_a88 * 4) = iVar8;
      local_a88 = local_a88 + 1;
      goto LAB_0011a2c9;
    }
    if ((local_a88 != 0) || ((QArrayData *)((ulong)(pQVar11 + 0x1f) & 0xfffffffffffffff0) == __src))
    goto LAB_0011a21b;
    *(int *)(__src + -4) = iVar8;
    __src = __src + -4;
    local_a88 = 1;
    iVar8 = *(int *)pQVar11;
joined_r0x0011a5f7:
    if (iVar8 < 2) goto LAB_0011a592;
  }
  local_ab0 = &local_a58;
  local_a48 = local_a88;
  local_a58 = pQVar11;
  local_a50 = __src;
  FUN_0011d740(local_ab0,0,0);
  local_a88 = local_a48;
  __src = local_a50;
  pQVar11 = local_a58;
LAB_0011a592:
  local_a8c = local_a8c + -1;
  goto joined_r0x0011a173;
}

