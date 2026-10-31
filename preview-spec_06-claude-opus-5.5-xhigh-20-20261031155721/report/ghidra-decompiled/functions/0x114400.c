// rea decompile_all
// name: FUN_00114400
// address: 0x114400
// body: available
// range: 0x114400-0x1144bc inclusive
// range: 0x1144c0-0x1144d4 inclusive
// range: 0x1144d8-0x1144e4 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1144e0 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00114400(long param_1,undefined8 param_2)

{
  QString *pQVar1;
  long in_FS_OFFSET;
  KLocalizedString local_58 [8];
  KLocalizedString local_50 [8];
  QArrayData *local_48;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if ((*(long *)(param_1 + 0x70) == 0) || (*(int *)(*(long *)(param_1 + 0x70) + 4) == 0)) {
    pQVar1 = (QString *)0x0;
  }
  else {
    pQVar1 = *(QString **)(param_1 + 0x78);
  }
  ki18n((char *)local_50);
  KLocalizedString::subs(local_58,local_50,param_2,0,0x20);
  KLocalizedString::toString();
  KLocalizedString::~KLocalizedString(local_58);
  KLocalizedString::~KLocalizedString(local_50);
  QLabel::setText(pQVar1);
  if (local_48 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_48 = *(int *)local_48 + -1;
    UNLOCK();
    if (*(int *)local_48 == 0) {
      QArrayData::deallocate(local_48,2,0x10);
    }
  }
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

