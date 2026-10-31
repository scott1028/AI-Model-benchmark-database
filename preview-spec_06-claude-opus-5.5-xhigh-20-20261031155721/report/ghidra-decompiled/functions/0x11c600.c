// rea decompile_all
// name: FUN_0011c600
// address: 0x11c600
// body: available
// range: 0x11c600-0x11c636 inclusive
// range: 0x11c640-0x11c671 inclusive
// range: 0x11c678-0x11c6cd inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11c632 to 0x10f9f0 <EXTERNAL>::KCoreConfigSkeleton::~KCoreConfigSkeleton (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Removing unreachable block (ram,0x0010fbc0) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0011c600(KCoreConfigSkeleton *param_1)

{
  int iVar1;
  
  *(undefined ***)param_1 = &PTR_metaObject_001284e8;
  if (DAT_001292d8 == -1) {
    if ((DAT_001292d0 == '\0') && (iVar1 = __cxa_guard_acquire(&DAT_001292d0), iVar1 != 0)) {
      DAT_001292c8 = 0;
      DAT_001292d8 = -1;
      __cxa_atexit(FUN_0011c700,&DAT_001292c8,&PTR_LOOP_00129008);
      __cxa_guard_release(&DAT_001292d0);
    }
    DAT_001292c8 = 0;
  }
  *(undefined8 *)param_1 = 0x126ce8;
  KCoreConfigSkeleton::~KCoreConfigSkeleton(param_1);
  return;
}

