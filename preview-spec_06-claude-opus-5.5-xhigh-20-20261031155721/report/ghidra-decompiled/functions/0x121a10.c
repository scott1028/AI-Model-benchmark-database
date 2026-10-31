// rea decompile_all
// name: FUN_00121a10
// address: 0x121a10
// body: available
// range: 0x121a10-0x121a40 inclusive
// range: 0x121a48-0x121a56 inclusive
// range: 0x121a60-0x121a62 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x121a52 to 0x10fb80 <EXTERNAL>::QGraphicsView::qt_metacast (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


char * FUN_00121a10(char *param_1,char *param_2)

{
  int iVar1;
  char *pcVar2;
  
  if (param_2 == (char *)0x0) {
    return (char *)0x0;
  }
  iVar1 = strcmp(param_2,"KMinesView");
  if (iVar1 == 0) {
    return param_1;
  }
  pcVar2 = (char *)QGraphicsView::qt_metacast(param_1);
  return pcVar2;
}

