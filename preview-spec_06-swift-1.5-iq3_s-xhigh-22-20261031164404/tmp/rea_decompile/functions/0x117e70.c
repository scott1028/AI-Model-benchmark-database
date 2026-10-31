// rea decompile_all
// name: FUN_00117e70
// address: 0x117e70
// body: available
// range: 0x117e70-0x117ea5 inclusive
// range: 0x117eb0-0x117ec3 inclusive
// range: 0x117ec8-0x117ed4 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x117ea1 to 0x117930 FUN_00117930 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x117ebf to 0x117930 FUN_00117930 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x117ed0 to 0x117930 FUN_00117930 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00117e70(long param_1)

{
  int iVar1;
  long lVar2;
  
  lVar2 = FUN_0011f4c0();
  iVar1 = *(int *)(param_1 + 0x28);
  if (iVar1 == 3) {
    *(undefined4 *)(param_1 + 0x28) = 0;
    FUN_00117930();
    return;
  }
  if (iVar1 != 4) {
    if (iVar1 == 0) {
      *(undefined4 *)(param_1 + 0x28) = 4;
    }
    FUN_00117930();
    return;
  }
  *(uint *)(param_1 + 0x28) = -(uint)(*(char *)(lVar2 + 0x18) != '\0') & 3;
  FUN_00117930();
  return;
}

