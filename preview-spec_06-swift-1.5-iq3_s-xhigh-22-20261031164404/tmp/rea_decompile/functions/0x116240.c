// rea decompile_all
// name: FUN_00116240
// address: 0x116240
// body: available
// range: 0x116240-0x116353 inclusive
// range: 0x116358-0x116424 inclusive
// range: 0x116428-0x116509 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x116505 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined1 (*) [16] FUN_00116240(undefined1 (*param_1) [16],long param_2,int param_3,int param_4)

{
  int iVar1;
  undefined8 uVar2;
  long in_FS_OFFSET;
  int local_48;
  int local_44;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  *(undefined8 *)param_1[1] = 0;
  *param_1 = (undefined1  [16])0x0;
  if (param_3 != 0) {
    iVar1 = param_3 + -1;
    uVar2 = 0;
    if (param_4 != 0) {
      local_44 = param_4 + -1;
      local_48 = iVar1;
      FUN_0011dce0(param_1,0,&local_48);
      if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
        FUN_0011d9f0(param_1,0,0);
      }
      uVar2 = *(undefined8 *)param_1[1];
    }
    local_48 = iVar1;
    local_44 = param_4;
    FUN_0011dce0(param_1,uVar2,&local_48);
    if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
      FUN_0011d9f0(param_1,0,0);
    }
    if (*(int *)(param_2 + 0x58) + -1 != param_4) {
      local_44 = param_4 + 1;
      local_48 = iVar1;
      FUN_0011dce0(param_1,*(undefined8 *)param_1[1],&local_48);
      if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
        FUN_0011d9f0(param_1,0,0);
      }
    }
  }
  if (param_4 == 0) {
    if (*(int *)(param_2 + 0x58) != 1) {
LAB_00116398:
      local_44 = param_4 + 1;
      local_48 = param_3;
      FUN_0011dce0(param_1,*(undefined8 *)param_1[1],&local_48);
      if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
        FUN_0011d9f0(param_1,0,0);
      }
      goto LAB_001163cc;
    }
  }
  else {
    local_44 = param_4 + -1;
    local_48 = param_3;
    FUN_0011dce0(param_1,*(undefined8 *)param_1[1],&local_48);
    if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
      FUN_0011d9f0(param_1,0,0);
    }
    if (*(int *)(param_2 + 0x58) + -1 != param_4) goto LAB_00116398;
LAB_001163cc:
    iVar1 = *(int *)(param_2 + 0x54) + -1;
    if ((iVar1 == param_3) || (param_4 == 0)) goto LAB_00116326;
    local_48 = param_3 + 1;
    local_44 = param_4 + -1;
    FUN_0011dce0(param_1,*(undefined8 *)param_1[1],&local_48);
    if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
      FUN_0011d9f0(param_1,0,0);
    }
  }
  iVar1 = *(int *)(param_2 + 0x54) + -1;
LAB_00116326:
  if (param_3 != iVar1) {
    local_48 = param_3 + 1;
    local_44 = param_4;
    FUN_0011dce0(param_1,*(undefined8 *)param_1[1],&local_48);
    if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
      FUN_0011d9f0(param_1,0,0);
    }
    if ((*(int *)(param_2 + 0x54) + -1 != param_3) && (*(int *)(param_2 + 0x58) + -1 != param_4)) {
      local_44 = param_4 + 1;
      local_48 = param_3 + 1;
      FUN_0011dce0(param_1,*(undefined8 *)param_1[1],&local_48);
      if ((*(int **)*param_1 == (int *)0x0) || (1 < **(int **)*param_1)) {
        FUN_0011d9f0(param_1,0,0);
      }
    }
  }
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return param_1;
}

