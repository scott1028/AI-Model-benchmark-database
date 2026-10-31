// rea decompile_all
// name: FUN_0011c0c0
// address: 0x11c0c0
// body: available
// range: 0x11c0c0-0x11c1f6 inclusive
// range: 0x11c200-0x11c243 inclusive
// range: 0x11c248-0x11c29b inclusive
// range: 0x11c2a0-0x11c2b8 inclusive
// range: 0x11c2c0-0x11c2cd inclusive
// range: 0x11c2d0-0x11c2d6 inclusive
// range: 0x11c2e0-0x11c2f1 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11c23f to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11c2ed to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011c0c0(undefined1 (*param_1) [16],long param_2)

{
  size_t __n;
  int *piVar1;
  long lVar2;
  void *__dest;
  long lVar3;
  QArrayData *pQVar4;
  undefined4 uVar5;
  void *__src;
  long lVar6;
  long in_FS_OFFSET;
  undefined1 auVar7 [16];
  long local_28;
  long local_20;
  
  piVar1 = *(int **)*param_1;
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  if (piVar1 == (int *)0x0) {
    lVar3 = *(long *)param_1[1];
LAB_0011c2a4:
    if (lVar3 < 0) {
      lVar3 = 0;
    }
    lVar6 = 0;
    lVar3 = lVar3 + param_2;
LAB_0011c149:
    __dest = (void *)QArrayData::allocate(&local_28,8,0x10,lVar3,lVar3 <= lVar6);
    if ((__dest != (void *)0x0) && (local_28 != 0)) {
      lVar3 = *(long *)*param_1;
      if (lVar3 == 0) {
        uVar5 = 0;
      }
      else {
        uVar5 = *(undefined4 *)(lVar3 + 4);
        __dest = (void *)((long)__dest +
                         (*(long *)(*param_1 + 8) - (lVar3 + 0x1fU & 0xfffffffffffffff0)));
      }
      *(undefined4 *)(local_28 + 4) = uVar5;
    }
    lVar3 = *(long *)param_1[1];
    if (lVar3 == 0) {
      pQVar4 = *(QArrayData **)*param_1;
LAB_0011c203:
      lVar3 = 0;
    }
    else {
      pQVar4 = *(QArrayData **)*param_1;
      if (param_2 < 0) {
        lVar3 = lVar3 + param_2;
      }
      __n = lVar3 * 8;
      if (pQVar4 == (QArrayData *)0x0) {
        if (__n == 0) {
          *(undefined8 *)param_1[1] = 0;
          *(long *)*param_1 = local_28;
          *(void **)(*param_1 + 8) = __dest;
          goto LAB_0011c284;
        }
        __src = *(void **)(*param_1 + 8);
      }
      else {
        __src = *(void **)(*param_1 + 8);
        if (__n == 0) goto LAB_0011c203;
      }
      memcpy(__dest,__src,__n);
      pQVar4 = *(QArrayData **)*param_1;
      lVar3 = (long)__n >> 3;
    }
    *(long *)param_1[1] = lVar3;
    *(long *)*param_1 = local_28;
    *(void **)(*param_1 + 8) = __dest;
    if (pQVar4 != (QArrayData *)0x0) {
      LOCK();
      *(int *)pQVar4 = *(int *)pQVar4 + -1;
      UNLOCK();
      if (*(int *)pQVar4 == 0) {
        if (local_20 == *(long *)(in_FS_OFFSET + 0x28)) {
          QArrayData::deallocate(pQVar4,8,0x10);
          return;
        }
        goto LAB_0011c2ed;
      }
    }
  }
  else {
    if ((1 < *piVar1) || (param_2 < 1)) {
      lVar3 = *(long *)param_1[1];
      if (piVar1 == (int *)0x0) goto LAB_0011c2a4;
      lVar6 = *(long *)(piVar1 + 2);
      lVar2 = lVar3;
      if (lVar3 <= lVar6) {
        lVar2 = lVar6;
      }
      lVar3 = (lVar2 + param_2) -
              ((lVar6 - ((long)(*(long *)(*param_1 + 8) -
                               ((long)piVar1 + 0x1fU & 0xfffffffffffffff0)) >> 3)) - lVar3);
      lVar2 = lVar6;
      if (lVar6 <= lVar3) {
        lVar2 = lVar3;
      }
      if ((*(byte *)(piVar1 + 1) & 1) != 0) {
        lVar3 = lVar2;
      }
      goto LAB_0011c149;
    }
    auVar7 = QArrayData::reallocateUnaligned
                       (piVar1,*(long *)(*param_1 + 8),8,
                        param_2 + ((long)(*(long *)(*param_1 + 8) -
                                         ((long)piVar1 + 0x1fU & 0xfffffffffffffff0)) >> 3) +
                                  *(long *)param_1[1],0);
    *param_1 = auVar7;
  }
LAB_0011c284:
  if (local_20 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011c2ed:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

