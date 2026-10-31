// rea decompile_all
// name: FUN_0011f320
// address: 0x11f320
// body: available
// range: 0x11f320-0x11f3c1 inclusive
// range: 0x11f3c8-0x11f409 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11f3eb to 0x10f400 <EXTERNAL>::KGamePopupItem::forceHide (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11f405 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011f320(long param_1,char param_2)

{
  undefined8 uVar1;
  long in_FS_OFFSET;
  KLocalizedString local_50 [8];
  QArrayData *local_48 [3];
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  QGraphicsItem::setVisible((bool)((char)*(undefined8 *)(param_1 + 0x30) + '\x10'));
  if (param_2 == '\0') {
    if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
      KGamePopupItem::forceHide(*(undefined8 *)(param_1 + 0x40),1);
      return;
    }
  }
  else {
    uVar1 = *(undefined8 *)(param_1 + 0x40);
    ki18n((char *)local_50);
    KLocalizedString::toString();
    KLocalizedString::~KLocalizedString(local_50);
    KGamePopupItem::showMessage(uVar1,local_48,4,0);
    if (local_48[0] != (QArrayData *)0x0) {
      LOCK();
      *(int *)local_48[0] = *(int *)local_48[0] + -1;
      UNLOCK();
      if (*(int *)local_48[0] == 0) {
        QArrayData::deallocate(local_48[0],2,0x10);
      }
    }
    if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

