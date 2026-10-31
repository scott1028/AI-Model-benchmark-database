// rea decompile_all
// name: FUN_0011d400
// address: 0x11d400
// body: available
// range: 0x11d400-0x11d48e inclusive
// range: 0x11d490-0x11d594 inclusive
// range: 0x11d598-0x11d5ad inclusive
// range: 0x11d5b0-0x11d60d inclusive
// range: 0x11d610-0x11d659 inclusive
// range: 0x11d660-0x11d66a inclusive
// range: 0x11d670-0x11d6bd inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11d590 to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11d6b9 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011d400(undefined1 (*param_1) [16],int param_2,long param_3)

{
  size_t __n;
  void *__dest;
  long lVar1;
  long lVar2;
  QArrayData *pQVar3;
  long lVar4;
  int *piVar5;
  void *__src;
  long in_FS_OFFSET;
  undefined1 auVar6 [16];
  long local_38;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_2 == 0) {
    piVar5 = *(int **)*param_1;
    if (piVar5 == (int *)0x0) {
LAB_0011d598:
      lVar2 = *(long *)param_1[1];
LAB_0011d59c:
      lVar1 = 0;
      if (lVar2 < 0) {
        lVar2 = lVar1;
      }
      lVar2 = lVar2 + param_3;
      goto LAB_0011d4d5;
    }
    if ((1 < *piVar5) || (param_3 < 1)) {
      lVar2 = *(long *)param_1[1];
      if (piVar5 != (int *)0x0) {
        lVar1 = *(long *)(piVar5 + 2);
        lVar4 = lVar2;
        if (lVar2 <= lVar1) {
          lVar4 = lVar1;
        }
        lVar2 = (lVar4 + param_3) -
                ((lVar1 - ((long)(*(long *)(*param_1 + 8) -
                                 ((long)piVar5 + 0x1fU & 0xfffffffffffffff0)) >> 3)) - lVar2);
        goto LAB_0011d4c4;
      }
      goto LAB_0011d59c;
    }
    auVar6 = QArrayData::reallocateUnaligned
                       (piVar5,*(long *)(*param_1 + 8),8,
                        param_3 + ((long)(*(long *)(*param_1 + 8) -
                                         ((long)piVar5 + 0x1fU & 0xfffffffffffffff0)) >> 3) +
                                  *(long *)param_1[1],0);
    *param_1 = auVar6;
  }
  else {
    piVar5 = *(int **)*param_1;
    if (piVar5 == (int *)0x0) goto LAB_0011d598;
    lVar1 = *(long *)(piVar5 + 2);
    lVar2 = *(long *)param_1[1];
    if (*(long *)param_1[1] <= lVar1) {
      lVar2 = lVar1;
    }
    lVar2 = (lVar2 + param_3) -
            ((long)(*(long *)(*param_1 + 8) - ((long)piVar5 + 0x1fU & 0xfffffffffffffff0)) >> 3);
LAB_0011d4c4:
    if (((*(byte *)(piVar5 + 1) & 1) != 0) && (lVar2 < *(long *)(piVar5 + 2))) {
      lVar2 = *(long *)(piVar5 + 2);
    }
LAB_0011d4d5:
    __dest = (void *)QArrayData::allocate(&local_38,8,0x10,lVar2,lVar2 <= lVar1);
    lVar1 = *(long *)param_1[1];
    if ((local_38 != 0) && (__dest != (void *)0x0)) {
      lVar2 = *(long *)*param_1;
      if (param_2 == 1) {
        lVar4 = ((*(long *)(local_38 + 8) - lVar1) - param_3) / 2;
        if (lVar4 < 0) {
          lVar4 = 0;
        }
        param_2 = 0;
        __dest = (void *)((long)__dest + (lVar4 + param_3) * 8);
        if (lVar2 != 0) goto LAB_0011d528;
      }
      else if (lVar2 != 0) {
        __dest = (void *)((long)__dest +
                         (*(long *)(*param_1 + 8) - (lVar2 + 0x1fU & 0xfffffffffffffff0)));
LAB_0011d528:
        param_2 = *(int *)(lVar2 + 4);
      }
      *(int *)(local_38 + 4) = param_2;
    }
    if (lVar1 == 0) {
      pQVar3 = *(QArrayData **)*param_1;
LAB_0011d54a:
      lVar1 = 0;
    }
    else {
      pQVar3 = *(QArrayData **)*param_1;
      if (param_3 < 0) {
        lVar1 = lVar1 + param_3;
      }
      __n = lVar1 * 8;
      if (pQVar3 == (QArrayData *)0x0) {
        if (__n == 0) {
          *(undefined8 *)param_1[1] = 0;
          *(long *)*param_1 = local_38;
          *(void **)(*param_1 + 8) = __dest;
          goto LAB_0011d5f0;
        }
        __src = *(void **)(*param_1 + 8);
      }
      else {
        __src = *(void **)(*param_1 + 8);
        if (__n == 0) goto LAB_0011d54a;
      }
      memcpy(__dest,__src,__n);
      pQVar3 = *(QArrayData **)*param_1;
      lVar1 = (long)__n >> 3;
    }
    *(long *)param_1[1] = lVar1;
    *(long *)*param_1 = local_38;
    *(void **)(*param_1 + 8) = __dest;
    if (pQVar3 != (QArrayData *)0x0) {
      LOCK();
      *(int *)pQVar3 = *(int *)pQVar3 + -1;
      UNLOCK();
      if (*(int *)pQVar3 == 0) {
        if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
          QArrayData::deallocate(pQVar3,8,0x10);
          return;
        }
        goto LAB_0011d6b9;
      }
    }
  }
LAB_0011d5f0:
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011d6b9:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

