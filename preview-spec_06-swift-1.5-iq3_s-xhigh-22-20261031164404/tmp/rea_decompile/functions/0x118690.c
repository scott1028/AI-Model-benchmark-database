// rea decompile_all
// name: FUN_00118690
// address: 0x118690
// body: available
// range: 0x118690-0x11871c inclusive
// range: 0x118720-0x118734 inclusive
// range: 0x118738-0x11876a inclusive
// range: 0x118770-0x11878b inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x118787 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00118690(long param_1,undefined4 param_2,undefined4 param_3)

{
  long lVar1;
  int *piVar2;
  long in_FS_OFFSET;
  QArrayData *local_38;
  int *local_30;
  long local_28;
  long local_20;
  
  local_20 = *(long *)(in_FS_OFFSET + 0x28);
  FUN_00116240(&local_38,param_1,param_2,param_3);
  for (piVar2 = local_30; local_30 + local_28 * 2 != piVar2; piVar2 = piVar2 + 2) {
    lVar1 = *(long *)(*(long *)(param_1 + 0x28) +
                     (long)(*piVar2 * *(int *)(param_1 + 0x58) + piVar2[1]) * 8);
    if (3 < *(int *)(lVar1 + 0x28) - 2U) {
      *(undefined4 *)(lVar1 + 0x28) = 2;
      if (*(int *)(lVar1 + 0x30) == 0) {
        FUN_00117930();
        *(int *)(param_1 + 0x8c) = *(int *)(param_1 + 0x8c) + -1;
        FUN_00118690(param_1,*piVar2,piVar2[1]);
      }
      else {
        FUN_00117930();
        *(int *)(param_1 + 0x8c) = *(int *)(param_1 + 0x8c) + -1;
      }
    }
  }
  if (local_38 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_38 = *(int *)local_38 + -1;
    UNLOCK();
    if (*(int *)local_38 == 0) {
      QArrayData::deallocate(local_38,8,0x10);
    }
  }
  if (local_20 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

