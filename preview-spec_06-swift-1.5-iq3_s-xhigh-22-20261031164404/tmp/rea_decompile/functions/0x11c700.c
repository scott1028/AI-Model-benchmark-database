// rea decompile_all
// name: FUN_0011c700
// address: 0x11c700
// body: available
// range: 0x11c700-0x11c746 inclusive
// range: 0x11c750-0x11c757 inclusive
// range: 0x11c760-0x11c763 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011c700(undefined8 *param_1)

{
  long *plVar1;
  
  plVar1 = (long *)*param_1;
  if (plVar1 != (long *)0x0) {
    if (*(code **)(*plVar1 + 0x20) == FUN_0011c6d0) {
      FUN_0011c600();
      operator_delete(plVar1,0x30);
    }
    else {
      (**(code **)(*plVar1 + 0x20))();
    }
    DAT_001292d8 = 0xfe;
    return;
  }
  DAT_001292d8 = 0xfe;
  return;
}

