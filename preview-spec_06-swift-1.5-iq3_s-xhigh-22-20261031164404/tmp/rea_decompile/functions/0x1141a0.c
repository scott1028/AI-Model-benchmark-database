// rea decompile_all
// name: FUN_001141a0
// address: 0x1141a0
// body: available
// range: 0x1141a0-0x114299 inclusive
// range: 0x1142a0-0x1142b4 inclusive
// range: 0x1142b8-0x1142c4 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1142c0 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001141a0(long param_1,undefined4 param_2)

{
  undefined4 uVar1;
  QString *pQVar2;
  long in_FS_OFFSET;
  KLocalizedString local_70 [8];
  KLocalizedString local_68 [8];
  KLocalizedString local_60 [8];
  QArrayData *local_58;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  if ((*(long *)(param_1 + 0x60) == 0) || (*(int *)(*(long *)(param_1 + 0x60) + 4) == 0)) {
    pQVar2 = (QString *)0x0;
  }
  else {
    pQVar2 = *(QString **)(param_1 + 0x68);
  }
  uVar1 = *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x40) + 0x30) + 0x5c);
  ki18n((char *)local_60);
  KLocalizedString::subs(local_68,local_60,param_2,0,10,0x20);
  KLocalizedString::subs(local_70,local_68,uVar1,0,10,0x20);
  KLocalizedString::toString();
  KLocalizedString::~KLocalizedString(local_70);
  KLocalizedString::~KLocalizedString(local_68);
  KLocalizedString::~KLocalizedString(local_60);
  QLabel::setText(pQVar2);
  if (local_58 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_58 = *(int *)local_58 + -1;
    UNLOCK();
    if (*(int *)local_58 == 0) {
      QArrayData::deallocate(local_58,2,0x10);
    }
  }
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

