// rea decompile_all
// name: FUN_0011dce0
// address: 0x11dce0
// body: available
// range: 0x11dce0-0x11dd20 inclusive
// range: 0x11dd28-0x11dd6e inclusive
// range: 0x11dd70-0x11ddf7 inclusive
// range: 0x11de00-0x11de60 inclusive
// range: 0x11de68-0x11df00 inclusive
// range: 0x11df08-0x11df30 inclusive
// range: 0x11df38-0x11df40 inclusive
// range: 0x11df48-0x11df7a inclusive
// range: 0x11df80-0x11dfeb inclusive
// range: 0x11dff0-0x11dff7 inclusive
// range: 0x11e000-0x11e014 inclusive
// range: 0x11e018-0x11e025 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011dce0(long *param_1,long param_2,undefined8 *param_3)

{
  bool bVar1;
  undefined8 *puVar2;
  ulong uVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  int *piVar9;
  ulong uVar10;
  long lVar11;
  
  piVar9 = (int *)*param_1;
  if (piVar9 == (int *)0x0) {
    uVar7 = *param_3;
    if ((param_1[2] != 0) && (param_2 == 0)) {
      FUN_0011d9f0(param_1,1,1);
      puVar2 = (undefined8 *)param_1[1];
      lVar8 = param_1[2];
      puVar4 = puVar2;
      goto LAB_0011de4f;
    }
LAB_0011dd83:
    FUN_0011d9f0(param_1,0,1);
    lVar8 = param_1[2];
    puVar4 = (undefined8 *)(param_1[1] + param_2 * 8);
  }
  else {
    lVar8 = param_1[2];
    if (1 < *piVar9) {
      uVar7 = *param_3;
      if ((lVar8 == 0) || (param_2 != 0)) {
        piVar9 = (int *)*param_1;
LAB_0011dd40:
        if (piVar9 == (int *)0x0) goto LAB_0011dd83;
        goto LAB_0011dd45;
      }
LAB_0011df54:
      if ((*piVar9 < 2) && (lVar5 = *param_1, lVar5 != 0)) {
        puVar2 = (undefined8 *)param_1[1];
        lVar6 = (long)puVar2 - (lVar5 + 0x1fU & 0xfffffffffffffff0);
        puVar4 = puVar2;
        if (0 < lVar6) goto LAB_0011de4f;
        lVar5 = *(long *)(lVar5 + 8);
        lVar6 = lVar6 >> 3;
        if ((lVar5 - lVar6 <= lVar8) || (lVar5 <= lVar8 * 3)) goto LAB_0011df5b;
        bVar1 = true;
        lVar5 = ((lVar5 - lVar8) + -1) / 2;
        if (lVar5 < 0) {
          lVar5 = 0;
        }
        lVar5 = lVar5 + 1;
LAB_0011dec5:
        puVar2 = puVar4 + (lVar5 - lVar6);
        FUN_0011dca0(puVar4,lVar8,puVar2);
        param_1[1] = (long)puVar2;
        puVar4 = puVar2 + param_2;
        if (!bVar1) goto LAB_0011dd68;
      }
      else {
LAB_0011df5b:
        FUN_0011d9f0(param_1,1,1);
        puVar2 = (undefined8 *)param_1[1];
        puVar4 = puVar2 + param_2;
      }
      lVar8 = param_1[2];
LAB_0011de4f:
      puVar4 = puVar4 + -1;
      param_1[1] = (long)(puVar2 + -1);
      goto LAB_0011dda9;
    }
    if (param_2 == lVar8) {
      uVar3 = param_1[1];
      uVar10 = (long)piVar9 + 0x1fU & 0xfffffffffffffff0;
      if (*(long *)(piVar9 + 2) - ((long)(uVar3 - uVar10) >> 3) != param_2) {
        *(undefined8 *)(uVar3 + param_2 * 8) = *param_3;
        param_1[2] = param_2 + 1;
        return;
      }
      if ((param_2 == 0) && (uVar3 != uVar10)) {
LAB_0011de16:
        *(undefined8 *)(uVar3 - 8) = *param_3;
        param_1[1] = (long)(uVar3 - 8);
        param_1[2] = lVar8 + 1;
        return;
      }
      uVar7 = *param_3;
    }
    else {
      if (param_2 != 0) {
        uVar7 = *param_3;
        goto LAB_0011dd40;
      }
      uVar3 = param_1[1];
      if (uVar3 != ((long)piVar9 + 0x1fU & 0xfffffffffffffff0)) goto LAB_0011de16;
      uVar7 = *param_3;
      if (lVar8 != 0) goto LAB_0011df54;
    }
LAB_0011dd45:
    if ((*piVar9 < 2) && (lVar5 = *param_1, lVar5 != 0)) {
      puVar4 = (undefined8 *)param_1[1];
      lVar8 = param_1[2];
      lVar11 = (long)puVar4 - (lVar5 + 0x1fU & 0xfffffffffffffff0);
      lVar6 = lVar11 >> 3;
      if (lVar8 < *(long *)(lVar5 + 8) - lVar6) {
        puVar4 = puVar4 + param_2;
        goto LAB_0011dda0;
      }
      if ((0 < lVar11) && (lVar8 * 3 < *(long *)(lVar5 + 8) * 2)) {
        bVar1 = false;
        lVar5 = 0;
        goto LAB_0011dec5;
      }
    }
    FUN_0011d9f0(param_1,0,1);
    puVar4 = (undefined8 *)(param_1[1] + param_2 * 8);
LAB_0011dd68:
    lVar8 = param_1[2];
  }
LAB_0011dda0:
  if (param_2 < lVar8) {
    memmove(puVar4 + 1,puVar4,(lVar8 - param_2) * 8);
    lVar8 = param_1[2];
  }
LAB_0011dda9:
  param_1[2] = lVar8 + 1;
  *puVar4 = uVar7;
  return;
}

