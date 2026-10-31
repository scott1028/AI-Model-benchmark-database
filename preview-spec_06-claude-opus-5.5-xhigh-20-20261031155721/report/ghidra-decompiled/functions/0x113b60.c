// rea decompile_all
// name: FUN_00113b60
// address: 0x113b60
// body: available
// range: 0x113b60-0x113cb5 inclusive
// range: 0x113cc0-0x113d41 inclusive
// range: 0x113d48-0x113d92 inclusive
// range: 0x113d98-0x113e04 inclusive
// range: 0x113e08-0x113e54 inclusive
// range: 0x113e58-0x113e6b inclusive
// range: 0x113e70-0x113ea7 inclusive
// range: 0x113eb0-0x113f55 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00113b60(long *param_1,long param_2,undefined1 (*param_3) [16])

{
  undefined8 *__src;
  int *piVar1;
  void *__dest;
  long lVar2;
  ulong uVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  bool bVar8;
  void *__src_00;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  
  piVar1 = (int *)*param_1;
  if (piVar1 == (int *)0x0) {
    uVar10 = *(undefined8 *)*param_3;
    uVar11 = *(undefined8 *)(*param_3 + 8);
    uVar9 = *(undefined8 *)param_3[1];
    *(undefined8 *)param_3[1] = 0;
    *param_3 = (undefined1  [16])0x0;
    if ((param_1[2] != 0) && (param_2 == 0)) goto LAB_00113d6b;
  }
  else {
    if (1 < *piVar1) {
LAB_00113b88:
      uVar10 = *(undefined8 *)*param_3;
      uVar11 = *(undefined8 *)(*param_3 + 8);
      uVar9 = *(undefined8 *)param_3[1];
      *(undefined8 *)param_3[1] = 0;
      *param_3 = (undefined1  [16])0x0;
      bVar8 = param_1[2] != 0 && param_2 == 0;
      if (param_1[2] == 0 || param_2 != 0) {
LAB_00113bb7:
        if (piVar1 == (int *)0x0) goto LAB_00113d98;
        goto LAB_00113bc0;
      }
      if (((piVar1 == (int *)0x0) || (1 < *piVar1)) || (lVar7 = *param_1, lVar7 == 0)) {
LAB_00113d6b:
        FUN_00113710(param_1,1,1);
        __dest = (void *)param_1[1];
        lVar2 = param_1[2];
        goto LAB_00113c96;
      }
      __dest = (void *)param_1[1];
      lVar5 = (long)__dest - (lVar7 + 0x1fU & 0xfffffffffffffff0);
      lVar2 = param_1[2];
      if (0 < lVar5) goto LAB_00113c96;
      lVar7 = *(long *)(lVar7 + 8);
      lVar5 = lVar5 >> 3;
      if ((lVar7 + lVar5 * 0x5555555555555555 <= lVar2) || (lVar7 <= lVar2 * 3)) goto LAB_00113d6b;
      lVar7 = (-1 - ((lVar7 - lVar2) + -1 >> 0x3f)) + (lVar7 - lVar2) >> 1;
      if (lVar7 < 0) {
        lVar7 = 0;
      }
      lVar7 = lVar7 + 1;
      __src_00 = __dest;
LAB_00113c37:
      __dest = (void *)((long)__src_00 + (lVar7 + lVar5 * 0x5555555555555555) * 0x18);
      if ((((lVar2 != 0) && (__dest != __src_00)) && (__src_00 != (void *)0x0)) &&
         (__dest != (void *)0x0)) {
        __dest = memmove(__dest,__src_00,lVar2 * 0x18);
        lVar2 = param_1[2];
      }
      param_1[1] = (long)__dest;
      if (bVar8) {
LAB_00113c96:
        *(undefined8 *)((long)__dest + -8) = uVar9;
        *(undefined8 *)((long)__dest + -0x18) = uVar10;
        *(undefined8 *)((long)__dest + -0x10) = uVar11;
        param_1[1] = (long)__dest + -0x18;
        param_1[2] = lVar2 + 1;
        return;
      }
      goto LAB_00113dc0;
    }
    uVar10 = *(undefined8 *)*param_3;
    uVar11 = *(undefined8 *)(*param_3 + 8);
    uVar9 = *(undefined8 *)param_3[1];
    if (param_1[2] != param_2) {
      if (param_2 == 0) {
        uVar6 = param_1[1];
        uVar3 = (long)piVar1 + 0x1fU & 0xfffffffffffffff0;
LAB_00113cf8:
        if (uVar6 != uVar3) {
          *(undefined8 *)(uVar6 - 8) = uVar9;
          *(undefined8 *)param_3[1] = 0;
          lVar7 = param_1[2];
          *(undefined8 *)(uVar6 - 0x18) = uVar10;
          *(undefined8 *)(uVar6 - 0x10) = uVar11;
          *(undefined8 *)*param_3 = 0;
          *(undefined8 *)(*param_3 + 8) = 0;
          param_1[1] = (long)(uVar6 - 0x18);
LAB_00113d35:
          param_1[2] = lVar7 + 1;
          return;
        }
        goto LAB_00113b88;
      }
      *(undefined8 *)param_3[1] = 0;
      *param_3 = (undefined1  [16])0x0;
      goto LAB_00113bb7;
    }
    uVar6 = param_1[1];
    uVar3 = (long)piVar1 + 0x1fU & 0xfffffffffffffff0;
    if (param_2 != *(long *)(piVar1 + 2) + ((long)(uVar6 - uVar3) >> 3) * 0x5555555555555555) {
      *(undefined8 *)param_3[1] = 0;
      *(undefined8 *)(uVar6 + 0x10 + param_2 * 0x18) = uVar9;
      *(undefined8 *)(uVar6 + param_2 * 0x18) = uVar10;
      *(undefined8 *)(uVar6 + 8 + param_2 * 0x18) = uVar11;
      lVar7 = param_1[2];
      *(undefined8 *)*param_3 = 0;
      *(undefined8 *)(*param_3 + 8) = 0;
      goto LAB_00113d35;
    }
    if (param_2 == 0) goto LAB_00113cf8;
    *(undefined8 *)param_3[1] = 0;
    *param_3 = (undefined1  [16])0x0;
LAB_00113bc0:
    if ((*piVar1 < 2) && (lVar7 = *param_1, lVar7 != 0)) {
      __dest = (void *)param_1[1];
      lVar4 = (long)__dest - (lVar7 + 0x1fU & 0xfffffffffffffff0);
      lVar5 = lVar4 >> 3;
      lVar2 = param_1[2];
      if (lVar2 < *(long *)(lVar7 + 8) + lVar5 * 0x5555555555555555) goto LAB_00113dc0;
      if ((0 < lVar4) && (lVar2 * 3 < *(long *)(lVar7 + 8) * 2)) {
        bVar8 = false;
        lVar7 = 0;
        __src_00 = __dest;
        goto LAB_00113c37;
      }
    }
  }
LAB_00113d98:
  FUN_00113710(param_1,0,1);
  __dest = (void *)param_1[1];
  lVar2 = param_1[2];
LAB_00113dc0:
  __src = (undefined8 *)((long)__dest + param_2 * 0x18);
  memmove(__src + 3,__src,(lVar2 - param_2) * 0x18);
  __src[2] = uVar9;
  *__src = uVar10;
  __src[1] = uVar11;
  param_1[2] = param_1[2] + 1;
  return;
}

