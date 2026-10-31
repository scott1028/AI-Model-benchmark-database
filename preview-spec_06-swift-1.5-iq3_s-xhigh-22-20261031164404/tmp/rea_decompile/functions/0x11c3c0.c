// rea decompile_all
// name: FUN_0011c3c0
// address: 0x11c3c0
// body: available
// range: 0x11c3c0-0x11c444 inclusive
// range: 0x11c448-0x11c47d inclusive
// range: 0x11c480-0x11c48e inclusive
// range: 0x11c490-0x11c4aa inclusive
// range: 0x11c4b0-0x11c4e5 inclusive
// range: 0x11c4f0-0x11c512 inclusive
// range: 0x11c518-0x11c541 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11c50e to 0x10f650 <EXTERNAL>::QMetaObject::activate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11c538 to 0x11c300 FUN_0011c300 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11c53d to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011c3c0(QObject *param_1,int param_2,int param_3,undefined8 *param_4)

{
  long *plVar1;
  undefined4 *puVar2;
  code *pcVar3;
  long in_FS_OFFSET;
  undefined4 local_2c;
  void *local_28;
  undefined4 *puStack_20;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_2 == 0) {
    if (param_3 == 2) {
      if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
        QMetaObject::activate(param_1,(QMetaObject *)&PTR_staticMetaObject_00127b60,2,(void **)0x0);
        return;
      }
      goto LAB_0011c53d;
    }
    if (param_3 < 3) {
      if (param_3 == 0) {
        local_2c = *(undefined4 *)param_4[1];
        puStack_20 = &local_2c;
        local_28 = (void *)0x0;
        QMetaObject::activate(param_1,(QMetaObject *)&PTR_staticMetaObject_00127b60,0,&local_28);
      }
      else if (param_3 == 1) {
        local_2c = CONCAT31(local_2c._1_3_,*(undefined1 *)param_4[1]);
        puStack_20 = &local_2c;
        local_28 = (void *)0x0;
        QMetaObject::activate(param_1,(QMetaObject *)&PTR_staticMetaObject_00127b60,1,&local_28);
      }
    }
    else if (param_3 == 3) {
      if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
        FUN_0011c300(param_1,*(undefined1 *)param_4[1]);
        return;
      }
      goto LAB_0011c53d;
    }
  }
  else if (param_2 == 5) {
    plVar1 = (long *)param_4[1];
    puVar2 = (undefined4 *)*param_4;
    pcVar3 = (code *)*plVar1;
    if (pcVar3 == FUN_0011bf80) {
      if (plVar1[1] == 0) {
        *puVar2 = 0;
      }
    }
    else if (pcVar3 == FUN_0011bfe0) {
      if (plVar1[1] == 0) {
        *puVar2 = 1;
      }
    }
    else if ((pcVar3 == FUN_0011c040) && (plVar1[1] == 0)) {
      *puVar2 = 2;
    }
  }
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011c53d:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

