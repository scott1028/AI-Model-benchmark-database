// rea decompile_all
// name: FUN_00114070
// address: 0x114070
// body: available
// range: 0x114070-0x1140ab inclusive
// range: 0x1140b0-0x1140c5 inclusive
// range: 0x1140d0-0x114107 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_00114070(long param_1,int param_2,undefined8 param_3,undefined8 *param_4)

{
  int iVar1;
  
  iVar1 = QWidget::qt_metacall();
  if (-1 < iVar1) {
    if (param_2 == 0) {
      if (iVar1 == 0) {
        QSpinBox::value();
        QSpinBox::value();
        QSpinBox::setMaximum((int)*(undefined8 *)(param_1 + 0x58));
      }
    }
    else {
      if (param_2 != 7) {
        return iVar1;
      }
      if (iVar1 == 0) {
        *(undefined8 *)*param_4 = 0;
      }
    }
    iVar1 = iVar1 + -1;
  }
  return iVar1;
}

