// rea decompile_all
// name: FUN_0011ffa0
// address: 0x11ffa0
// body: available
// range: 0x11ffa0-0x11ffd8 inclusive
// range: 0x11ffe0-0x11fff6 inclusive
// range: 0x120000-0x120016 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


ulong FUN_0011ffa0(undefined8 param_1,int param_2,undefined8 param_3,undefined8 *param_4)

{
  int iVar1;
  ulong uVar2;
  
  uVar2 = QGraphicsObject::qt_metacall();
  iVar1 = (int)uVar2;
  if (-1 < iVar1) {
    if (param_2 == 0) {
      if (iVar1 < 3) {
        FUN_0011be00(param_1,0,uVar2 & 0xffffffff,param_4);
        uVar2 = uVar2 & 0xffffffff;
      }
    }
    else {
      if (param_2 != 7) {
        return uVar2;
      }
      if (iVar1 < 3) {
        *(undefined8 *)*param_4 = 0;
      }
    }
    uVar2 = (ulong)((int)uVar2 - 3);
  }
  return uVar2;
}

