// rea decompile_all
// name: FUN_0011cda0
// address: 0x11cda0
// body: available
// range: 0x11cda0-0x11cf5c inclusive
// range: 0x11cf60-0x11d033 inclusive
// range: 0x11d038-0x11d079 inclusive
// range: 0x11d080-0x11d169 inclusive
// range: 0x11d170-0x11d1b2 inclusive
// range: 0x11d1b8-0x11d24b inclusive
// range: 0x11d250-0x11d2c7 inclusive
// range: 0x11d2d0-0x11d3ca inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011cda0(undefined8 *param_1,long param_2,undefined8 *param_3)

{
  int *piVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  int *piVar7;
  undefined8 uVar8;
  int *piVar9;
  undefined8 uVar10;
  int *piVar11;
  undefined8 uVar12;
  int *piVar13;
  undefined8 uVar14;
  int *piVar15;
  undefined8 uVar16;
  long lVar17;
  void *__dest;
  undefined8 *puVar18;
  ulong uVar19;
  long lVar20;
  void *__src;
  long lVar21;
  ulong uVar22;
  bool bVar23;
  
  piVar1 = (int *)*param_1;
  if ((piVar1 != (int *)0x0) && (*piVar1 < 2)) {
    if (param_1[2] == param_2) {
      uVar19 = param_1[1];
      uVar22 = (long)piVar1 + 0x1fU & 0xfffffffffffffff0;
      if (param_2 != *(long *)(piVar1 + 2) + ((long)(uVar19 - uVar22) >> 3) * 0x1111111111111111) {
        piVar1 = (int *)*param_3;
        uVar8 = param_3[1];
        puVar18 = (undefined8 *)(param_2 * 0x78 + uVar19);
        uVar2 = param_3[2];
        *puVar18 = piVar1;
        puVar18[1] = uVar8;
        puVar18[2] = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[3];
        uVar2 = param_3[4];
        puVar18[5] = param_3[5];
        puVar18[3] = piVar1;
        puVar18[4] = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[6];
        uVar2 = param_3[7];
        puVar18[8] = param_3[8];
        puVar18[6] = piVar1;
        puVar18[7] = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[9];
        uVar2 = param_3[10];
        puVar18[0xb] = param_3[0xb];
        puVar18[9] = piVar1;
        puVar18[10] = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[0xc];
        uVar2 = param_3[0xd];
        puVar18[0xe] = param_3[0xe];
        puVar18[0xc] = piVar1;
        puVar18[0xd] = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        goto LAB_0011d027;
      }
      if (param_2 == 0) goto LAB_0011cf7f;
    }
    else if (param_2 == 0) {
      uVar19 = param_1[1];
      uVar22 = (long)piVar1 + 0x1fU & 0xfffffffffffffff0;
LAB_0011cf7f:
      if (uVar19 != uVar22) {
        piVar1 = (int *)*param_3;
        uVar2 = param_3[1];
        *(undefined8 *)(uVar19 - 0x68) = param_3[2];
        *(int **)(uVar19 - 0x78) = piVar1;
        *(undefined8 *)(uVar19 - 0x70) = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[3];
        uVar2 = param_3[4];
        *(undefined8 *)(uVar19 - 0x50) = param_3[5];
        *(int **)(uVar19 - 0x60) = piVar1;
        *(undefined8 *)(uVar19 - 0x58) = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[6];
        uVar2 = param_3[7];
        *(undefined8 *)(uVar19 - 0x38) = param_3[8];
        *(int **)(uVar19 - 0x48) = piVar1;
        *(undefined8 *)(uVar19 - 0x40) = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[9];
        uVar2 = param_3[10];
        *(undefined8 *)(uVar19 - 0x20) = param_3[0xb];
        *(int **)(uVar19 - 0x30) = piVar1;
        *(undefined8 *)(uVar19 - 0x28) = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        piVar1 = (int *)param_3[0xc];
        uVar2 = param_3[0xd];
        *(undefined8 *)(uVar19 - 8) = param_3[0xe];
        *(int **)(uVar19 - 0x18) = piVar1;
        *(undefined8 *)(uVar19 - 0x10) = uVar2;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + 1;
          UNLOCK();
        }
        param_1[1] = param_1[1] + -0x78;
LAB_0011d027:
        param_1[2] = param_1[2] + 1;
        return;
      }
    }
  }
  piVar1 = (int *)*param_3;
  uVar8 = param_3[1];
  uVar2 = param_3[2];
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  piVar9 = (int *)param_3[3];
  uVar10 = param_3[4];
  uVar3 = param_3[5];
  if (piVar9 != (int *)0x0) {
    LOCK();
    *piVar9 = *piVar9 + 1;
    UNLOCK();
  }
  piVar11 = (int *)param_3[6];
  uVar12 = param_3[7];
  uVar4 = param_3[8];
  if (piVar11 != (int *)0x0) {
    LOCK();
    *piVar11 = *piVar11 + 1;
    UNLOCK();
  }
  piVar13 = (int *)param_3[9];
  uVar14 = param_3[10];
  uVar5 = param_3[0xb];
  if (piVar13 != (int *)0x0) {
    LOCK();
    *piVar13 = *piVar13 + 1;
    UNLOCK();
  }
  piVar15 = (int *)param_3[0xc];
  uVar16 = param_3[0xd];
  uVar6 = param_3[0xe];
  if (piVar15 != (int *)0x0) {
    LOCK();
    *piVar15 = *piVar15 + 1;
    UNLOCK();
  }
  piVar7 = (int *)*param_1;
  bVar23 = param_1[2] != 0 && param_2 == 0;
  if (bVar23) {
    if (piVar7 == (int *)0x0) {
      FUN_0011c770(param_1,1,1);
      lVar20 = param_1[2];
      __dest = (void *)param_1[1];
      goto LAB_0011d202;
    }
    if (*piVar7 < 2) {
      __dest = (void *)param_1[1];
      lVar17 = (long)__dest - ((long)piVar7 + 0x1fU & 0xfffffffffffffff0);
      lVar20 = param_1[2];
      if (0 < lVar17) goto LAB_0011d202;
      lVar21 = *(long *)(piVar7 + 2);
      lVar17 = lVar17 >> 3;
      if (lVar21 + lVar17 * 0x1111111111111111 <= lVar20) {
        FUN_0011c770(param_1,1,1);
        __dest = (void *)param_1[1];
        lVar20 = param_1[2];
        goto LAB_0011d202;
      }
      if (lVar20 * 3 < lVar21) {
        lVar21 = ((lVar21 - lVar20) + -1) - ((lVar21 - lVar20) + -1 >> 0x3f) >> 1;
        if (lVar21 < 0) {
          lVar21 = 0;
        }
        lVar21 = lVar21 + 1;
        __src = __dest;
        goto LAB_0011d0d9;
      }
    }
    FUN_0011c770(param_1,1,1);
    __dest = (void *)param_1[1];
    lVar20 = param_1[2];
LAB_0011d202:
    *(undefined8 *)((long)__dest + -0x68) = uVar2;
    *(undefined8 *)((long)__dest + -0x78) = piVar1;
    *(undefined8 *)((long)__dest + -0x70) = uVar8;
    *(int **)((long)__dest + -0x60) = piVar9;
    *(undefined8 *)((long)__dest + -0x58) = uVar10;
    *(undefined8 *)((long)__dest + -0x50) = uVar3;
    *(int **)((long)__dest + -0x48) = piVar11;
    *(undefined8 *)((long)__dest + -0x40) = uVar12;
    *(undefined8 *)((long)__dest + -0x38) = uVar4;
    *(int **)((long)__dest + -0x30) = piVar13;
    *(undefined8 *)((long)__dest + -0x28) = uVar14;
    *(undefined8 *)((long)__dest + -0x20) = uVar5;
    *(int **)((long)__dest + -0x18) = piVar15;
    *(undefined8 *)((long)__dest + -0x10) = uVar16;
    *(undefined8 *)((long)__dest + -8) = uVar6;
    param_1[1] = (undefined8 *)((long)__dest + -0x78);
    param_1[2] = lVar20 + 1;
    return;
  }
  if ((piVar7 != (int *)0x0) && (*piVar7 < 2)) {
    __dest = (void *)param_1[1];
    lVar21 = (long)__dest - ((long)piVar7 + 0x1fU & 0xfffffffffffffff0);
    lVar17 = lVar21 >> 3;
    lVar20 = param_1[2];
    if (lVar20 < *(long *)(piVar7 + 2) + lVar17 * 0x1111111111111111) goto LAB_0011cec3;
    if ((0 < lVar21) && (lVar20 * 3 < *(long *)(piVar7 + 2) * 2)) {
      lVar21 = 0;
      __src = __dest;
LAB_0011d0d9:
      __dest = (void *)((long)__src + (lVar21 + lVar17 * 0x1111111111111111) * 0x78);
      if ((((lVar20 != 0) && (__dest != __src)) && (__src != (void *)0x0)) &&
         (__dest != (void *)0x0)) {
        __dest = memmove(__dest,__src,lVar20 * 0x78);
        lVar20 = param_1[2];
      }
      param_1[1] = __dest;
      if (bVar23) goto LAB_0011d202;
      goto LAB_0011cec3;
    }
  }
  FUN_0011c770(param_1,0,1);
  __dest = (void *)param_1[1];
  lVar20 = param_1[2];
LAB_0011cec3:
  puVar18 = (undefined8 *)(param_2 * 0x78 + (long)__dest);
  memmove(puVar18 + 0xf,puVar18,(lVar20 - param_2) * 0x78);
  puVar18[2] = uVar2;
  puVar18[5] = uVar3;
  puVar18[8] = uVar4;
  puVar18[0xb] = uVar5;
  puVar18[0xe] = uVar6;
  *puVar18 = piVar1;
  puVar18[1] = uVar8;
  puVar18[3] = piVar9;
  puVar18[4] = uVar10;
  puVar18[6] = piVar11;
  puVar18[7] = uVar12;
  puVar18[9] = piVar13;
  puVar18[10] = uVar14;
  puVar18[0xc] = piVar15;
  puVar18[0xd] = uVar16;
  param_1[2] = param_1[2] + 1;
  return;
}

