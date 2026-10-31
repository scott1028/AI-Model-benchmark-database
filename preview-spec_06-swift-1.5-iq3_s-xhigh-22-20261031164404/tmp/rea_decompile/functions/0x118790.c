// rea decompile_all
// name: FUN_00118790
// address: 0x118790
// body: available
// range: 0x118790-0x1187a1 inclusive
// range: 0x1187a8-0x1187d1 inclusive
// range: 0x1187d8-0x1187ea inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1187cd to 0x117930 FUN_00117930 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x1187e6 to 0x117930 FUN_00117930 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00118790(long param_1,char param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x28);
  if (param_2 == '\0') {
    if (iVar1 == 1) {
      *(undefined4 *)(param_1 + 0x28) = 2;
      *(undefined1 *)(param_1 + 0x2d) = *(undefined1 *)(param_1 + 0x2c);
      FUN_00117930();
      return;
    }
  }
  else if (1 < iVar1 - 3U) {
    *(undefined1 *)(param_1 + 0x2d) = *(undefined1 *)(param_1 + 0x2c);
    if ((iVar1 == 1) || ((iVar1 != 2 && (iVar1 != 5)))) {
      *(undefined4 *)(param_1 + 0x28) = 2;
      FUN_00117930();
      return;
    }
  }
  return;
}

