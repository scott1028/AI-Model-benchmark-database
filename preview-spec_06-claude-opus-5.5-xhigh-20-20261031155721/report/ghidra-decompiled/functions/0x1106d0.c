// rea decompile_all
// name: _INIT_1
// address: 0x1106d0
// body: available
// range: 0x1106d0-0x11075f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11075b to 0x10f080 <EXTERNAL>::__cxa_atexit (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void _INIT_1(void)

{
  qRegisterResourceData(3,"","","");
  __cxa_atexit(FUN_0011d3d0,&DAT_001292c0,&PTR_LOOP_00129008);
  __cxa_atexit(FUN_001195a0,&DAT_001292a8,&PTR_LOOP_00129008);
  __cxa_atexit(FUN_001193f0,&DAT_001292b0,&PTR_LOOP_00129008);
  __cxa_atexit(FUN_001177f0,&DAT_001292b8,&PTR_LOOP_00129008);
  return;
}

