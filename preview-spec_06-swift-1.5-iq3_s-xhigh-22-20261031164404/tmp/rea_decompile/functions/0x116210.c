// rea decompile_all
// name: FUN_00116210
// address: 0x116210
// body: available
// range: 0x116210-0x11622c inclusive
// range: 0x116230-0x11623a inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x116228 to 0x11ee50 FUN_0011ee50 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x116236 to 0x10f6c0 <EXTERNAL>::QGraphicsItem::sceneEvent (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00116210(QEvent *param_1,long param_2)

{
  ushort uVar1;
  
  uVar1 = *(ushort *)(param_2 + 8);
  if (uVar1 < 0xc5) {
    if (uVar1 < 0xc2) {
LAB_00116236:
      QGraphicsItem::sceneEvent(param_1);
      return;
    }
  }
  else if (uVar1 != 0xd1) goto LAB_00116236;
  FUN_0011ee50(param_1 + -0x10);
  return;
}

