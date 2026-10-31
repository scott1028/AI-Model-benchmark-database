// rea decompile_all
// name: FUN_001177f0
// address: 0x1177f0
// body: available
// range: 0x1177f0-0x11791b inclusive
// range: 0x117920-0x117924 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x117908 to 0x10f500 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001177f0(undefined8 *param_1)

{
  byte bVar1;
  int *piVar2;
  void *pvVar3;
  long lVar4;
  byte *pbVar5;
  byte *pbVar6;
  void *pvVar7;
  byte *pbVar8;
  byte *pbVar9;
  
  piVar2 = (int *)*param_1;
  if ((piVar2 != (int *)0x0) && (*piVar2 != -1)) {
    LOCK();
    *piVar2 = *piVar2 + -1;
    UNLOCK();
    if (*piVar2 == 0) {
      pvVar3 = (void *)*param_1;
      if (pvVar3 == (void *)0x0) {
        return;
      }
      pbVar9 = *(byte **)((long)pvVar3 + 0x20);
      if (pbVar9 != (byte *)0x0) {
        pbVar5 = pbVar9 + *(long *)(pbVar9 + -8) * 0x90;
        if (pbVar9 == pbVar5) {
          lVar4 = 0;
        }
        else {
          pbVar9 = pbVar5 + -0x10;
          do {
            pbVar6 = pbVar5;
            pvVar7 = *(void **)pbVar9;
            pbVar5 = pbVar6 + -0x90;
            if (pvVar7 != (void *)0x0) {
              pbVar8 = pbVar5;
              if (pbVar5 != pbVar9) {
                do {
                  bVar1 = *pbVar8;
                  if (bVar1 != 0xff) {
                    piVar2 = *(int **)((long)pvVar7 + (ulong)bVar1 * 0x20 + 8);
                    if (piVar2 != (int *)0x0) {
                      LOCK();
                      *piVar2 = *piVar2 + -1;
                      UNLOCK();
                      if (*piVar2 == 0) {
                        QArrayData::deallocate
                                  (*(QArrayData **)((long)pvVar7 + (ulong)bVar1 * 0x20 + 8),2,0x10);
                      }
                      pvVar7 = *(void **)pbVar9;
                    }
                  }
                  pbVar8 = pbVar8 + 1;
                } while (pbVar8 != pbVar9);
                if (pvVar7 == (void *)0x0) goto LAB_001178c8;
              }
              operator_delete__(pvVar7);
            }
LAB_001178c8:
            pbVar9 = pbVar9 + -0x90;
          } while (*(byte **)((long)pvVar3 + 0x20) != pbVar5);
          lVar4 = *(long *)(pbVar6 + -0x98) * 0x90;
        }
        operator_delete__(pbVar5 + -8,lVar4 + 8);
      }
      operator_delete(pvVar3,0x28);
      return;
    }
  }
  return;
}

