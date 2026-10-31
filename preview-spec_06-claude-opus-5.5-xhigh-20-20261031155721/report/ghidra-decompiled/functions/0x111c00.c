// rea decompile_all
// name: FUN_00111c00
// address: 0x111c00
// body: available
// range: 0x111c00-0x111d56 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x111d4d to 0x10f480 <EXTERNAL>::operator.delete[] (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00111c00(long param_1)

{
  undefined8 *puVar1;
  byte bVar2;
  int *piVar3;
  byte *pbVar4;
  long lVar5;
  undefined8 *puVar6;
  void *pvVar7;
  byte *pbVar8;
  byte *pbVar9;
  byte *local_40;
  
  pbVar8 = *(byte **)(param_1 + 0x20);
  if (pbVar8 == (byte *)0x0) {
    return;
  }
  local_40 = pbVar8 + *(long *)(pbVar8 + -8) * 0x90;
  if (pbVar8 == local_40) {
    lVar5 = 0;
  }
  else {
    pbVar8 = local_40 + -0x10;
    pbVar4 = local_40;
    do {
      local_40 = pbVar4;
      pvVar7 = *(void **)pbVar8;
      pbVar4 = local_40 + -0x90;
      if (pvVar7 != (void *)0x0) {
        pbVar9 = pbVar4;
        if (pbVar4 != pbVar8) {
          do {
            bVar2 = *pbVar9;
            if ((bVar2 != 0xff) &&
               (piVar3 = *(int **)((long)pvVar7 + (ulong)bVar2 * 0x20 + 8), piVar3 != (int *)0x0)) {
              LOCK();
              *piVar3 = *piVar3 + -1;
              UNLOCK();
              if (*piVar3 == 0) {
                puVar6 = *(undefined8 **)((long)pvVar7 + (ulong)bVar2 * 0x20 + 0x10);
                puVar1 = puVar6 + *(long *)((long)pvVar7 + (ulong)bVar2 * 0x20 + 0x18) * 3;
                for (; puVar6 != puVar1; puVar6 = puVar6 + 3) {
                  piVar3 = (int *)*puVar6;
                  if (piVar3 != (int *)0x0) {
                    LOCK();
                    *piVar3 = *piVar3 + -1;
                    UNLOCK();
                    if (*piVar3 == 0) {
                      QArrayData::deallocate((QArrayData *)*puVar6,2,0x10);
                    }
                  }
                }
                QArrayData::deallocate
                          (*(QArrayData **)((long)pvVar7 + (ulong)bVar2 * 0x20 + 8),0x18,0x10);
              }
              pvVar7 = *(void **)pbVar8;
            }
            pbVar9 = pbVar9 + 1;
          } while (pbVar9 != pbVar8);
          if (pvVar7 == (void *)0x0) goto LAB_00111d07;
        }
        operator_delete__(pvVar7);
      }
LAB_00111d07:
      pbVar8 = pbVar8 + -0x90;
    } while (*(byte **)(param_1 + 0x20) != pbVar4);
    lVar5 = *(long *)(local_40 + -0x98) * 0x90;
    local_40 = pbVar4;
  }
  operator_delete__(local_40 + -8,lVar5 + 8);
  return;
}

