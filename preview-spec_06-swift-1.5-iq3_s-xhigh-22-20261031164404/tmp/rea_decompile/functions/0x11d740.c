// rea decompile_all
// name: FUN_0011d740
// address: 0x11d740
// body: available
// range: 0x11d740-0x11d901 inclusive
// range: 0x11d908-0x11d91d inclusive
// range: 0x11d920-0x11d94b inclusive
// range: 0x11d950-0x11d95d inclusive
// range: 0x11d960-0x11d96c inclusive
// range: 0x11d970-0x11d9ad inclusive
// range: 0x11d9b0-0x11d9ed inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11d8fd to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11d9e9 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011d740(undefined1 (*param_1) [16],int param_2,ulong param_3)

{
  size_t __n;
  QArrayData *pQVar1;
  void *__dest;
  long lVar2;
  long lVar3;
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
LAB_0011d908:
      lVar3 = *(long *)param_1[1];
LAB_0011d90c:
      lVar2 = 0;
      if (lVar3 < 0) {
        lVar3 = lVar2;
      }
      lVar3 = lVar3 + param_3;
      goto LAB_0011d815;
    }
    if ((1 < *piVar5) || ((param_3 & 1) == 0)) {
      lVar3 = *(long *)param_1[1];
      if (piVar5 != (int *)0x0) {
        lVar2 = *(long *)(piVar5 + 2);
        lVar4 = lVar3;
        if (lVar3 <= lVar2) {
          lVar4 = lVar2;
        }
        lVar3 = (lVar4 + param_3) -
                ((lVar2 - ((long)(*(long *)(*param_1 + 8) -
                                 ((long)piVar5 + 0x1fU & 0xfffffffffffffff0)) >> 2)) - lVar3);
        goto LAB_0011d804;
      }
      goto LAB_0011d90c;
    }
    auVar6 = QArrayData::reallocateUnaligned
                       (piVar5,*(long *)(*param_1 + 8),4,
                        ((long)(*(long *)(*param_1 + 8) -
                               ((long)piVar5 + 0x1fU & 0xfffffffffffffff0)) >> 2) +
                        *(long *)param_1[1] + 1,0);
    *param_1 = auVar6;
  }
  else {
    piVar5 = *(int **)*param_1;
    if (piVar5 == (int *)0x0) goto LAB_0011d908;
    lVar2 = *(long *)(piVar5 + 2);
    lVar3 = *(long *)param_1[1];
    if (*(long *)param_1[1] <= lVar2) {
      lVar3 = lVar2;
    }
    lVar3 = (lVar3 + param_3) -
            ((long)(*(long *)(*param_1 + 8) - ((long)piVar5 + 0x1fU & 0xfffffffffffffff0)) >> 2);
LAB_0011d804:
    if (((*(byte *)(piVar5 + 1) & 1) != 0) && (lVar3 < *(long *)(piVar5 + 2))) {
      lVar3 = *(long *)(piVar5 + 2);
    }
LAB_0011d815:
    __dest = (void *)QArrayData::allocate(&local_38,4,0x10,lVar3,lVar3 <= lVar2);
    lVar2 = *(long *)param_1[1];
    if ((__dest != (void *)0x0) && (local_38 != 0)) {
      lVar3 = *(long *)*param_1;
      if (param_2 == 1) {
        lVar4 = (long)((*(long *)(local_38 + 8) - lVar2) - param_3) / 2;
        if (lVar4 < 0) {
          lVar4 = 0;
        }
        param_2 = 0;
        __dest = (void *)((long)__dest + (lVar4 + param_3) * 4);
        if (lVar3 != 0) goto LAB_0011d868;
      }
      else if (lVar3 != 0) {
        __dest = (void *)((long)__dest +
                         (*(long *)(*param_1 + 8) - (lVar3 + 0x1fU & 0xfffffffffffffff0)));
LAB_0011d868:
        param_2 = *(int *)(lVar3 + 4);
      }
      *(int *)(local_38 + 4) = param_2;
    }
    pQVar1 = *(QArrayData **)*param_1;
    if (lVar2 == 0) {
LAB_0011d920:
      *(long *)*param_1 = local_38;
      *(void **)(*param_1 + 8) = __dest;
      *(undefined8 *)param_1[1] = 0;
    }
    else {
      __n = lVar2 * 4;
      if (pQVar1 == (QArrayData *)0x0) {
        if (__n == 0) {
          *(undefined8 *)param_1[1] = 0;
          *(long *)*param_1 = local_38;
          *(void **)(*param_1 + 8) = __dest;
          goto LAB_0011d92e;
        }
        __src = *(void **)(*param_1 + 8);
      }
      else {
        __src = *(void **)(*param_1 + 8);
        if (__n == 0) goto LAB_0011d920;
      }
      memcpy(__dest,__src,__n);
      pQVar1 = *(QArrayData **)*param_1;
      *(long *)param_1[1] = (long)__n >> 2;
      *(long *)*param_1 = local_38;
      *(void **)(*param_1 + 8) = __dest;
    }
    if (pQVar1 != (QArrayData *)0x0) {
      LOCK();
      *(int *)pQVar1 = *(int *)pQVar1 + -1;
      UNLOCK();
      if (*(int *)pQVar1 == 0) {
        if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
          QArrayData::deallocate(pQVar1,4,0x10);
          return;
        }
        goto LAB_0011d9e9;
      }
    }
  }
LAB_0011d92e:
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011d9e9:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

