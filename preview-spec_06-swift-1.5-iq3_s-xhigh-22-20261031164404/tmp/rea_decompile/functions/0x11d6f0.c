// rea decompile_all
// name: FUN_0011d6f0
// address: 0x11d6f0
// body: available
// range: 0x11d6f0-0x11d725 inclusive
// range: 0x11d730-0x11d73c inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


long FUN_0011d6f0(undefined8 *param_1)

{
  if (((int *)*param_1 != (int *)0x0) && (*(int *)*param_1 < 2)) {
    return param_1[1] + param_1[2] * 8;
  }
  FUN_0011d400(param_1,0,0);
  return param_1[1] + param_1[2] * 8;
}

