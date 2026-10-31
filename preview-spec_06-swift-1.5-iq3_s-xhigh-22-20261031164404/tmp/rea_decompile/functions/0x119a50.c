// rea decompile_all
// name: FUN_00119a50
// address: 0x119a50
// body: available
// range: 0x119a50-0x119afb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x119af7 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00119a50(QObject *param_1,long param_2)

{
  int iVar1;
  long in_FS_OFFSET;
  int local_4c;
  void *local_48;
  int *piStack_40;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  iVar1 = *(int *)(param_2 + 0x28);
  FUN_00117e70(param_2);
  if ((*(int *)(param_2 + 0x28) == 4) != (iVar1 == 4)) {
    piStack_40 = &local_4c;
    local_48 = (void *)0x0;
    local_4c = *(int *)(param_1 + 0x60) + -1;
    if (*(int *)(param_2 + 0x28) == 4) {
      local_4c = *(int *)(param_1 + 0x60) + 1;
    }
    *(int *)(param_1 + 0x60) = local_4c;
    QMetaObject::activate(param_1,(QMetaObject *)&PTR_staticMetaObject_00127a00,0,&local_48);
  }
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

