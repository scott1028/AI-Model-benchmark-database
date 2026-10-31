// rea decompile_all
// name: FUN_001144f0
// address: 0x1144f0
// body: available
// range: 0x1144f0-0x11456a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x114561 to 0x120020 FUN_00120020 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x114566 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001144f0(long param_1)

{
  long lVar1;
  undefined8 uVar2;
  long in_FS_OFFSET;
  undefined8 local_60;
  undefined8 local_48;
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  QGraphicsView::resetCachedContent();
  uVar2 = *(undefined8 *)(param_1 + 0x40);
  QGraphicsScene::sceneRect();
  QGraphicsScene::sceneRect();
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    FUN_00120020(uVar2,(int)local_48,(int)local_60);
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

