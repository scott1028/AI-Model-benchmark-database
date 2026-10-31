// rea decompile_all
// name: <EXTERNAL>::qRegisterResourceData
// address: 0x10f2d0
// body: available
// range: 0x10f2d0-0x10f2d9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x10f2d4 to 0x12a498 <EXTERNAL>::qRegisterResourceData (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void qRegisterResourceData(int param_1,uchar *param_2,uchar *param_3,uchar *param_4)

{
  qRegisterResourceData(param_1,param_2,param_3,param_4);
  return;
}

