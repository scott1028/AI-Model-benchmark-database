// rea decompile_all
// name: FUN_0011f430
// address: 0x11f430
// body: available
// range: 0x11f430-0x11f449 inclusive
// range: 0x11f450-0x11f473 inclusive
// range: 0x11f478-0x11f4bb inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined * FUN_0011f430(void)

{
  int iVar1;
  
  if (DAT_001292f8 != '\0') {
    return &DAT_001292e0;
  }
  iVar1 = __cxa_guard_acquire(&DAT_001292f8);
  if (iVar1 == 0) {
    return &DAT_001292e0;
  }
  QLoggingCategory::QLoggingCategory((QLoggingCategory *)&DAT_001292e0,"kmines",4);
  __cxa_atexit(QLoggingCategory::~QLoggingCategory,&DAT_001292e0,&PTR_LOOP_00129008);
  __cxa_guard_release(&DAT_001292f8);
  return &DAT_001292e0;
}

