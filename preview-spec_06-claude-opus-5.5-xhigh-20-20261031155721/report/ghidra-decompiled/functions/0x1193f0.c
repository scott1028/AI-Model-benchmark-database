// rea decompile_all
// name: FUN_001193f0
// address: 0x1193f0
// body: available
// range: 0x1193f0-0x119593 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11957b to 0x10f500 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001193f0(undefined8 *param_1)

{
  undefined8 *puVar1;
  byte bVar2;
  int *piVar3;
  void *pvVar4;
  byte *pbVar5;
  long lVar6;
  undefined8 *puVar7;
  void *pvVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte *local_40;
  
  piVar3 = (int *)*param_1;
  if ((piVar3 != (int *)0x0) && (*piVar3 != -1)) {
    LOCK();
    *piVar3 = *piVar3 + -1;
    UNLOCK();
    if (*piVar3 == 0) {
      pvVar4 = (void *)*param_1;
      if (pvVar4 == (void *)0x0) {
        return;
      }
      pbVar9 = *(byte **)((long)pvVar4 + 0x20);
      if (pbVar9 != (byte *)0x0) {
        local_40 = pbVar9 + *(long *)(pbVar9 + -8) * 0x90;
        if (pbVar9 == local_40) {
          lVar6 = 0;
        }
        else {
          pbVar9 = local_40 + -0x10;
          pbVar5 = local_40;
          do {
            local_40 = pbVar5;
            pvVar8 = *(void **)pbVar9;
            pbVar5 = local_40 + -0x90;
            if (pvVar8 != (void *)0x0) {
              pbVar10 = pbVar5;
              if (pbVar9 != pbVar5) {
                do {
                  bVar2 = *pbVar10;
                  if ((bVar2 != 0xff) &&
                     (piVar3 = *(int **)((long)pvVar8 + (ulong)bVar2 * 0x20 + 8),
                     piVar3 != (int *)0x0)) {
                    LOCK();
                    *piVar3 = *piVar3 + -1;
                    UNLOCK();
                    if (*piVar3 == 0) {
                      puVar7 = *(undefined8 **)((long)pvVar8 + (ulong)bVar2 * 0x20 + 0x10);
                      puVar1 = puVar7 + *(long *)((long)pvVar8 + (ulong)bVar2 * 0x20 + 0x18) * 3;
                      for (; puVar7 != puVar1; puVar7 = puVar7 + 3) {
                        piVar3 = (int *)*puVar7;
                        if (piVar3 != (int *)0x0) {
                          LOCK();
                          *piVar3 = *piVar3 + -1;
                          UNLOCK();
                          if (*piVar3 == 0) {
                            QArrayData::deallocate((QArrayData *)*puVar7,2,0x10);
                          }
                        }
                      }
                      QArrayData::deallocate
                                (*(QArrayData **)((long)pvVar8 + (ulong)bVar2 * 0x20 + 8),0x18,0x10)
                      ;
                    }
                    pvVar8 = *(void **)pbVar9;
                  }
                  pbVar10 = pbVar10 + 1;
                } while (pbVar9 != pbVar10);
                if (pvVar8 == (void *)0x0) goto LAB_00119527;
              }
              operator_delete__(pvVar8);
            }
LAB_00119527:
            pbVar9 = pbVar9 + -0x90;
          } while (*(byte **)((long)pvVar4 + 0x20) != pbVar5);
          lVar6 = *(long *)(local_40 + -0x98) * 0x90;
          local_40 = pbVar5;
        }
        operator_delete__(local_40 + -8,lVar6 + 8);
      }
      operator_delete(pvVar4,0x28);
      return;
    }
  }
  return;
}

