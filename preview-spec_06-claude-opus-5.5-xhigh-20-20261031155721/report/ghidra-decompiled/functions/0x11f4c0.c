// rea decompile_all
// name: FUN_0011f4c0
// address: 0x11f4c0
// body: available
// range: 0x10fbe0-0x10fbf3 inclusive
// range: 0x11f4c0-0x11f522 inclusive
// range: 0x11f528-0x11f577 inclusive
// range: 0x11f580-0x11f5c8 inclusive
// range: 0x11f5d0-0x11f60c inclusive
// range: 0x11f610-0x11f655 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


long FUN_0011f4c0(void)

{
  code *pcVar1;
  int iVar2;
  void *pvVar3;
  
  if (DAT_001292d8 < -1) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)invalidInstructionException();
    (*pcVar1)();
  }
  if (DAT_001292d0 == '\0') {
    iVar2 = __cxa_guard_acquire(&DAT_001292d0);
    if (iVar2 != 0) {
      DAT_001292c8 = 0;
      DAT_001292d8 = -1;
      __cxa_atexit(FUN_0011c700,&DAT_001292c8,&PTR_LOOP_00129008);
      __cxa_guard_release(&DAT_001292d0);
    }
  }
  if (DAT_001292c8 == 0) {
    pvVar3 = operator_new(0x30);
    FUN_0011e030(pvVar3);
    if (DAT_001292d8 < -1) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)invalidInstructionException();
      (*pcVar1)();
    }
    if (DAT_001292d0 == '\0') {
      iVar2 = __cxa_guard_acquire(&DAT_001292d0);
      if (iVar2 != 0) {
        DAT_001292c8 = 0;
        DAT_001292d8 = -1;
        __cxa_atexit(FUN_0011c700,&DAT_001292c8,&PTR_LOOP_00129008);
        __cxa_guard_release(&DAT_001292d0);
      }
    }
    KCoreConfigSkeleton::read();
  }
  if (-2 < DAT_001292d8) {
    if (DAT_001292d0 == '\0') {
      iVar2 = __cxa_guard_acquire(&DAT_001292d0);
      if (iVar2 != 0) {
        DAT_001292c8 = 0;
        DAT_001292d8 = -1;
        __cxa_atexit(FUN_0011c700,&DAT_001292c8,&PTR_LOOP_00129008);
        __cxa_guard_release(&DAT_001292d0);
      }
    }
    return DAT_001292c8;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)invalidInstructionException();
  (*pcVar1)();
}

