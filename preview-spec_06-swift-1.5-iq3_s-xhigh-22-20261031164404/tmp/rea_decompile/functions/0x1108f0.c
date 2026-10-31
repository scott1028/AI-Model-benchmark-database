// rea decompile_all
// name: FUN_001108f0
// address: 0x1108f0
// body: available
// range: 0x1108f0-0x110927 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined1 (*) [16] FUN_001108f0(undefined1 (*param_1) [16],long param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = *(int *)(param_2 + 0x58);
  iVar2 = *(int *)(param_2 + 0x50);
  iVar3 = *(int *)(param_2 + 0x54);
  *param_1 = (undefined1  [16])0x0;
  *(double *)param_1[1] = (double)((iVar1 + 2) * iVar2);
  *(double *)(param_1[1] + 8) = (double)((iVar3 + 2) * iVar2);
  return param_1;
}

