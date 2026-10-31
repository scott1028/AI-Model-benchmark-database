// rea decompile_all
// name: FUN_00118530
// address: 0x118530
// body: available
// range: 0x118530-0x11854e inclusive
// range: 0x118550-0x11855b inclusive
// range: 0x118560-0x118564 inclusive
// range: 0x118568-0x118572 inclusive
// range: 0x118578-0x11857c inclusive
// range: 0x118580-0x11858b inclusive
// range: 0x118590-0x118598 inclusive
// range: 0x1185a0-0x1185dd inclusive
// range: 0x1185e0-0x1185e4 inclusive
// range: 0x1185e8-0x1185ed inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x118557 to 0x114150 FUN_00114150 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x118560 to 0x1144f0 FUN_001144f0 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11856e to 0x1141a0 FUN_001141a0 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x118578 to 0x116660 FUN_00116660 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x118587 to 0x117ee0 FUN_00117ee0 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x118594 to 0x114400 FUN_00114400 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x1185d9 to 0x10f900 <EXTERNAL>::KGameDifficulty::setGameRunning (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x1185e0 to 0x1142d0 FUN_001142d0 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x1185e8 to 0x114570 FUN_00114570 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00118530(long param_1,undefined4 param_2,long param_3)

{
  bool bVar1;
  
  switch(param_2) {
  case 0:
    FUN_001141a0(param_1,**(undefined4 **)(param_3 + 8));
    return;
  case 1:
    FUN_00116660();
    return;
  case 2:
    FUN_00117ee0(param_1,**(undefined1 **)(param_3 + 8));
    return;
  case 3:
    FUN_00114400(param_1,*(undefined8 *)(param_3 + 8));
    return;
  case 4:
    QAction::setEnabled(SUB81(*(undefined8 *)(param_1 + 0x58),0));
    (**(code **)(**(long **)(param_1 + 0x50) + 0x70))();
    bVar1 = (bool)KGameDifficulty::global();
    KGameDifficulty::setGameRunning(bVar1);
    return;
  case 5:
    FUN_001142d0();
    return;
  case 6:
    FUN_00114570();
    return;
  case 7:
    FUN_00114150(param_1,**(undefined1 **)(param_3 + 8));
    return;
  case 8:
    FUN_001144f0();
    return;
  default:
    return;
  }
}

