// rea decompile_all
// name: FUN_00116510
// address: 0x116510
// body: available
// range: 0x116510-0x11655e inclusive
// range: 0x116560-0x116594 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11655a to 0x10f6b0 <EXTERNAL>::QGraphicsObject::~QGraphicsObject (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11657b to 0x10f6b0 <EXTERNAL>::QGraphicsObject::~QGraphicsObject (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00116510(QGraphicsObject *param_1)

{
  int *piVar1;
  
  *(undefined ***)param_1 = &PTR_FUN_00127828;
  *(undefined ***)(param_1 + 0x10) = &PTR_FUN_001278c8;
  piVar1 = *(int **)(param_1 + 0x38);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      QArrayData::deallocate(*(QArrayData **)(param_1 + 0x38),8,0x10);
    }
  }
  piVar1 = *(int **)(param_1 + 0x20);
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      QArrayData::deallocate(*(QArrayData **)(param_1 + 0x20),8,0x10);
      QGraphicsObject::~QGraphicsObject(param_1);
      return;
    }
  }
  QGraphicsObject::~QGraphicsObject(param_1);
  return;
}

