// rea decompile_all
// name: FUN_00112930
// address: 0x112930
// body: available
// range: 0x112930-0x112a13 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x112a0a to 0x10f480 <EXTERNAL>::operator.delete[] (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00112930(long param_1)

{
  byte bVar1;
  int *piVar2;
  long lVar3;
  byte *pbVar4;
  byte *pbVar5;
  void *pvVar6;
  byte *pbVar7;
  byte *pbVar8;
  
  pbVar8 = *(byte **)(param_1 + 0x20);
  if (pbVar8 == (byte *)0x0) {
    return;
  }
  pbVar4 = pbVar8 + *(long *)(pbVar8 + -8) * 0x90;
  if (pbVar8 == pbVar4) {
    lVar3 = 0;
  }
  else {
    pbVar8 = pbVar4 + -0x10;
    do {
      pbVar5 = pbVar4;
      pvVar6 = *(void **)pbVar8;
      pbVar4 = pbVar5 + -0x90;
      if (pvVar6 != (void *)0x0) {
        pbVar7 = pbVar4;
        if (pbVar4 != pbVar8) {
          do {
            bVar1 = *pbVar7;
            if (bVar1 != 0xff) {
              piVar2 = *(int **)((long)pvVar6 + (ulong)bVar1 * 0x20 + 8);
              if (piVar2 != (int *)0x0) {
                LOCK();
                *piVar2 = *piVar2 + -1;
                UNLOCK();
                if (*piVar2 == 0) {
                  QArrayData::deallocate
                            (*(QArrayData **)((long)pvVar6 + (ulong)bVar1 * 0x20 + 8),2,0x10);
                }
                pvVar6 = *(void **)pbVar8;
              }
            }
            pbVar7 = pbVar7 + 1;
          } while (pbVar7 != pbVar8);
          if (pvVar6 == (void *)0x0) goto LAB_001129d7;
        }
        operator_delete__(pvVar6);
      }
LAB_001129d7:
      pbVar8 = pbVar8 + -0x90;
    } while (*(byte **)(param_1 + 0x20) != pbVar4);
    lVar3 = *(long *)(pbVar5 + -0x98) * 0x90;
  }
  operator_delete__(pbVar4 + -8,lVar3 + 8);
  return;
}

