// rea decompile_all
// name: FUN_00115cc0
// address: 0x115cc0
// body: available
// range: 0x115cc0-0x115d51 inclusive
// range: 0x115d58-0x115d67 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x115d4d to 0x10f500 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00115cc0(KXmlGuiWindow *param_1)

{
  int *piVar1;
  
  *(undefined ***)param_1 = &PTR_FUN_00127df0;
  *(undefined ***)(param_1 + 0x10) = &PTR_FUN_00128010;
  *(undefined ***)(param_1 + 0x30) = &PTR_FUN_00128060;
  *(undefined ***)(param_1 + 0x80) = &PTR_FUN_00128128;
  piVar1 = *(int **)(param_1 + 0x70);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if ((*piVar1 == 0) && (*(void **)(param_1 + 0x70) != (void *)0x0)) {
      operator_delete(*(void **)(param_1 + 0x70));
    }
  }
  piVar1 = *(int **)(param_1 + 0x60);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if ((*piVar1 == 0) && (*(void **)(param_1 + 0x60) != (void *)0x0)) {
      operator_delete(*(void **)(param_1 + 0x60));
    }
  }
  KXmlGuiWindow::~KXmlGuiWindow(param_1);
  KXMLGUIClient::~KXMLGUIClient((KXMLGUIClient *)(param_1 + 0x80));
  operator_delete(param_1,0x90);
  return;
}

