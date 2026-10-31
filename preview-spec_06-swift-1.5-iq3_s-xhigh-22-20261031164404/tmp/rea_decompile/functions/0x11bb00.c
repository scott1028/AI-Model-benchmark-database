// rea decompile_all
// name: FUN_0011bb00
// address: 0x11bb00
// body: available
// range: 0x11bb00-0x11bb60 inclusive
// range: 0x11bb68-0x11bb7c inclusive
// range: 0x11bb80-0x11bb9c inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11bb5c to 0x10f500 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011bb00(undefined8 *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)param_1[5];
  param_1[-2] = &PTR_FUN_00127828;
  *param_1 = &PTR_FUN_001278c8;
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      QArrayData::deallocate((QArrayData *)param_1[5],8,0x10);
    }
  }
  piVar1 = (int *)param_1[2];
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (*piVar1 == 0) {
      QArrayData::deallocate((QArrayData *)param_1[2],8,0x10);
    }
  }
  QGraphicsObject::~QGraphicsObject((QGraphicsObject *)(param_1 + -2));
  operator_delete((QGraphicsObject *)(param_1 + -2),0x98);
  return;
}

