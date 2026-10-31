// rea decompile_all
// name: FUN_00115e10
// address: 0x115e10
// body: available
// range: 0x115e10-0x115e9f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x115e8b to 0x10f7c0 <EXTERNAL>::KXMLGUIClient::~KXMLGUIClient (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00115e10(undefined8 *param_1)

{
  int *piVar1;
  
  param_1[-6] = &PTR_FUN_00127df0;
  param_1[-4] = &PTR_FUN_00128010;
  *param_1 = &PTR_FUN_00128060;
  param_1[10] = &PTR_FUN_00128128;
  piVar1 = (int *)param_1[8];
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if ((*piVar1 == 0) && ((void *)param_1[8] != (void *)0x0)) {
      operator_delete((void *)param_1[8]);
    }
  }
  piVar1 = (int *)param_1[6];
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if ((*piVar1 == 0) && ((void *)param_1[6] != (void *)0x0)) {
      operator_delete((void *)param_1[6]);
    }
  }
  KXmlGuiWindow::~KXmlGuiWindow((KXmlGuiWindow *)(param_1 + -6));
  KXMLGUIClient::~KXMLGUIClient((KXMLGUIClient *)(param_1 + 10));
  return;
}

