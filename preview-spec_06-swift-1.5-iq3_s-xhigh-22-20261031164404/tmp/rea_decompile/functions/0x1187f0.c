// rea decompile_all
// name: FUN_001187f0
// address: 0x1187f0
// body: available
// range: 0x1187f0-0x1187fa inclusive
// range: 0x118800-0x11880b inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x118807 to 0x117930 FUN_00117930 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001187f0(long param_1)

{
  if (*(int *)(param_1 + 0x28) != 1) {
    return;
  }
  *(undefined4 *)(param_1 + 0x28) = 0;
  FUN_00117930();
  return;
}

