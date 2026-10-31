// rea decompile_all
// name: FUN_0011bca0
// address: 0x11bca0
// body: available
// range: 0x11bca0-0x11bcc6 inclusive
// range: 0x11bcd0-0x11bcde inclusive
// range: 0x11bce0-0x11bd02 inclusive
// range: 0x11bd08-0x11bd1c inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11bcc2 to 0x10f500 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011bca0(int param_1,void *param_2,long param_3,long *param_4,undefined1 *param_5)

{
  bool bVar1;
  long *plVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  if (param_1 == 1) {
    UNRECOVERED_JUMPTABLE = *(code **)((long)param_2 + 0x10);
    plVar2 = (long *)(param_3 + *(long *)((long)param_2 + 0x18));
    if (((ulong)UNRECOVERED_JUMPTABLE & 1) != 0) {
      UNRECOVERED_JUMPTABLE = *(code **)(UNRECOVERED_JUMPTABLE + *plVar2 + -1);
    }
                    /* WARNING: Could not recover jumptable at 0x0011bd00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(plVar2,*(undefined1 *)param_4[1],plVar2,param_4,UNRECOVERED_JUMPTABLE);
    return;
  }
  if (param_1 == 2) {
    bVar1 = false;
    if (*param_4 == *(long *)((long)param_2 + 0x10)) {
      bVar1 = *(long *)((long)param_2 + 0x18) == param_4[1] || *param_4 == 0;
    }
    *param_5 = bVar1;
  }
  else if ((param_1 == 0) && (param_2 != (void *)0x0)) {
    operator_delete(param_2,0x20);
    return;
  }
  return;
}

