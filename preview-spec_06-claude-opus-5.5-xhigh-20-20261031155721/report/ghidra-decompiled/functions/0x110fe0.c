// rea decompile_all
// name: FUN_00110fe0
// address: 0x110fe0
// body: available
// range: 0x110fe0-0x1110e4 inclusive
// range: 0x1110e8-0x1110f7 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 * FUN_00110fe0(undefined4 *param_1,undefined8 param_2)

{
  undefined4 uVar1;
  long lVar2;
  undefined4 *puVar3;
  int *piVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined8 uVar7;
  undefined4 *puVar8;
  
  puVar6 = operator_new(0x40);
  piVar4 = *(int **)(param_1 + 10);
  uVar7 = *(undefined8 *)(param_1 + 0xc);
  uVar1 = param_1[8];
  *(undefined8 *)(puVar6 + 0xe) = *(undefined8 *)(param_1 + 0xe);
  puVar6[8] = uVar1;
  *(int **)(puVar6 + 10) = piVar4;
  *(undefined8 *)(puVar6 + 0xc) = uVar7;
  if (piVar4 != (int *)0x0) {
    LOCK();
    *piVar4 = *piVar4 + 1;
    UNLOCK();
  }
  uVar1 = *param_1;
  lVar2 = *(long *)(param_1 + 6);
  *(undefined8 *)(puVar6 + 2) = param_2;
  *(undefined8 *)(puVar6 + 4) = 0;
  *(undefined8 *)(puVar6 + 6) = 0;
  *puVar6 = uVar1;
  if (lVar2 != 0) {
    uVar7 = FUN_00110fe0(lVar2,puVar6);
    *(undefined8 *)(puVar6 + 6) = uVar7;
  }
  puVar5 = puVar6;
  for (puVar3 = *(undefined4 **)(param_1 + 4); puVar3 != (undefined4 *)0x0;
      puVar3 = *(undefined4 **)(puVar3 + 4)) {
    puVar8 = operator_new(0x40);
    uVar1 = puVar3[8];
    *(undefined8 *)(puVar8 + 0xe) = *(undefined8 *)(puVar3 + 0xe);
    puVar8[8] = uVar1;
    piVar4 = *(int **)(puVar3 + 10);
    uVar7 = *(undefined8 *)(puVar3 + 0xc);
    *(int **)(puVar8 + 10) = piVar4;
    *(undefined8 *)(puVar8 + 0xc) = uVar7;
    if (piVar4 != (int *)0x0) {
      LOCK();
      *piVar4 = *piVar4 + 1;
      UNLOCK();
    }
    uVar1 = *puVar3;
    *(undefined8 *)(puVar8 + 6) = 0;
    *(undefined8 *)(puVar8 + 4) = 0;
    *puVar8 = uVar1;
    *(undefined4 **)(puVar5 + 4) = puVar8;
    *(undefined4 **)(puVar8 + 2) = puVar5;
    if (*(long *)(puVar3 + 6) != 0) {
      uVar7 = FUN_00110fe0(*(long *)(puVar3 + 6),puVar8);
      *(undefined8 *)(puVar8 + 6) = uVar7;
    }
    puVar5 = puVar8;
  }
  return puVar6;
}

