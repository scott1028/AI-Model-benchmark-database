// rea decompile_all
// name: FUN_0011dca0
// address: 0x11dca0
// body: available
// range: 0x11dca0-0x11dccd inclusive
// range: 0x11dcd0-0x11dcd0 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11dcc9 to 0x10f530 <EXTERNAL>::memmove (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011dca0(void *param_1,long param_2,void *param_3)

{
  if ((((param_2 != 0) && (param_1 != param_3)) && (param_1 != (void *)0x0)) &&
     (param_3 != (void *)0x0)) {
    memmove(param_3,param_1,param_2 * 8);
    return;
  }
  return;
}

