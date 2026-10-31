// rea decompile_all
// name: FUN_00115bc0
// address: 0x115bc0
// body: available
// range: 0x115bc0-0x115bf0 inclusive
// range: 0x115bf8-0x115c06 inclusive
// range: 0x115c10-0x115c12 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x115c02 to 0x10f620 <EXTERNAL>::KXmlGuiWindow::qt_metacast (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


char * FUN_00115bc0(char *param_1,char *param_2)

{
  int iVar1;
  char *pcVar2;
  
  if (param_2 == (char *)0x0) {
    return (char *)0x0;
  }
  iVar1 = strcmp(param_2,"KMinesMainWindow");
  if (iVar1 == 0) {
    return param_1;
  }
  pcVar2 = (char *)KXmlGuiWindow::qt_metacast(param_1);
  return pcVar2;
}

