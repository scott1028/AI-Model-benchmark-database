// rea decompile_all
// name: FUN_00112a20
// address: 0x112a20
// body: available
// range: 0x112a20-0x112c0a inclusive
// range: 0x112c10-0x112dc8 inclusive
// range: 0x112dd0-0x112e34 inclusive
// range: 0x112e38-0x11322e inclusive
// range: 0x113230-0x113322 inclusive
// range: 0x113328-0x113563 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 * FUN_00112a20(undefined8 *param_1,int *param_2)

{
  byte bVar1;
  undefined8 uVar2;
  int *piVar3;
  undefined8 uVar4;
  uint *puVar5;
  long lVar6;
  ulong *puVar7;
  uint *puVar8;
  undefined8 *puVar9;
  void *pvVar10;
  ulong uVar11;
  ulong uVar12;
  byte bVar13;
  ulong uVar14;
  undefined1 uVar15;
  ulong uVar16;
  int *piVar17;
  ulong *puVar18;
  ulong uVar19;
  ulong uVar20;
  byte *pbVar21;
  long lVar22;
  byte *pbVar23;
  char cVar24;
  byte *pbVar25;
  byte *pbVar26;
  uint *puVar27;
  ulong local_50;
  
  if (DAT_001292a8 == (uint *)0x0) {
    puVar8 = operator_new(0x28);
    *puVar8 = 1;
    puVar8[6] = 0;
    puVar8[7] = 0;
    puVar8[2] = 0;
    puVar8[3] = 0;
    puVar8[4] = 0x80;
    puVar8[5] = 0;
    puVar9 = operator_new__(0x98);
    *puVar9 = 1;
    puVar9[0x11] = 0;
    *(undefined2 *)(puVar9 + 0x12) = 0;
    *(undefined4 *)(puVar9 + 1) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0xc) = 0xffffffff;
    *(undefined4 *)(puVar9 + 2) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x14) = 0xffffffff;
    *(undefined4 *)(puVar9 + 3) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x1c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 4) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x24) = 0xffffffff;
    *(undefined4 *)(puVar9 + 5) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x2c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 6) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x34) = 0xffffffff;
    *(undefined4 *)(puVar9 + 7) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x3c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 8) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x44) = 0xffffffff;
    *(undefined4 *)(puVar9 + 9) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x4c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 10) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x54) = 0xffffffff;
    *(undefined4 *)(puVar9 + 0xb) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x5c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 0xc) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 100) = 0xffffffff;
    *(undefined4 *)(puVar9 + 0xd) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x6c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 0xe) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x74) = 0xffffffff;
    *(undefined4 *)(puVar9 + 0xf) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x7c) = 0xffffffff;
    *(undefined4 *)(puVar9 + 0x10) = 0xffffffff;
    *(undefined4 *)((long)puVar9 + 0x84) = 0xffffffff;
    *(undefined8 **)(puVar8 + 8) = puVar9 + 1;
    uVar14 = QHashSeed::globalSeed();
    DAT_001292a8 = puVar8;
    *(ulong *)(puVar8 + 6) = uVar14;
  }
  else {
    uVar14 = *(ulong *)(DAT_001292a8 + 6);
    puVar8 = DAT_001292a8;
  }
  uVar14 = (long)*param_2 ^ uVar14;
  uVar14 = (uVar14 >> 0x20 ^ uVar14) * -0x2917014799a6026d;
  lVar6 = *(long *)(puVar8 + 8);
  uVar14 = (uVar14 ^ uVar14 >> 0x20) * -0x2917014799a6026d;
  uVar14 = uVar14 ^ uVar14 >> 0x20;
  uVar19 = *(ulong *)(puVar8 + 4) - 1 & uVar14;
  uVar20 = (ulong)((uint)uVar19 & 0x7f);
  lVar22 = (uVar19 >> 7) * 0x90 + lVar6;
  while( true ) {
    bVar13 = *(byte *)(lVar22 + uVar20);
    if ((ulong)bVar13 == 0xff) break;
    if (*param_2 == *(int *)((ulong)bVar13 * 0x20 + *(long *)(lVar22 + 0x80))) {
      if (*puVar8 < 2) {
        lVar6 = *(long *)(puVar8 + 8);
        *param_1 = puVar8;
        *(undefined1 *)(param_1 + 2) = 0;
        param_1[1] = (lVar22 - lVar6 >> 4) * 0x1c71c71c71c71c80 | uVar20;
        return param_1;
      }
      if (puVar8 == (uint *)0x0) {
        puVar27 = (uint *)0x0;
        uVar15 = 0;
        goto LAB_00112b6b;
      }
      if (*puVar8 == 0xffffffff) {
        uVar15 = 0;
        puVar27 = puVar8;
        goto LAB_00112b6b;
      }
      uVar15 = 0;
      goto LAB_00112e1c;
    }
    uVar20 = uVar20 + 1;
    if (uVar20 == 0x80) {
      lVar22 = lVar22 + 0x90;
      uVar20 = 0;
      if ((lVar22 - lVar6 >> 4) * -0x71c71c71c71c71c7 - (*(ulong *)(puVar8 + 4) >> 7) == 0) {
        lVar22 = lVar6;
      }
    }
  }
  if (*puVar8 < 2) {
    if (*(ulong *)(puVar8 + 4) >> 1 <= *(ulong *)(puVar8 + 2)) goto LAB_00112b3e;
    uVar15 = 1;
    puVar8 = (uint *)0x0;
  }
  else {
    uVar15 = 1;
    puVar27 = (uint *)0x0;
    if (puVar8 == (uint *)0x0) {
LAB_00112b52:
      uVar19 = *(ulong *)(puVar27 + 2);
      if (*(ulong *)(puVar27 + 4) >> 1 <= uVar19) {
        puVar5 = operator_new(0x28);
        *puVar5 = 1;
        *(ulong *)(puVar5 + 2) = uVar19;
        uVar2 = *(undefined8 *)(puVar27 + 6);
        puVar5[8] = 0;
        puVar5[9] = 0;
        *(undefined8 *)(puVar5 + 6) = uVar2;
        uVar20 = uVar19 + 1;
        if (uVar19 + 1 < uVar19) {
          uVar20 = uVar19;
        }
        if (uVar20 < 0x41) {
          puVar5[4] = 0x80;
          puVar5[5] = 0;
          lVar6 = 0;
          uVar19 = 0x98;
          uVar20 = 0x90;
          uVar12 = 1;
        }
        else {
          lVar6 = 0x3f;
          if (uVar20 != 0) {
            for (; uVar20 >> lVar6 == 0; lVar6 = lVar6 + -1) {
            }
          }
          uVar12 = 1L << ((char)lVar6 + 2U & 0x3f);
          *(ulong *)(puVar5 + 4) = uVar12;
          uVar12 = uVar12 >> 7;
          lVar6 = uVar12 - 1;
          uVar20 = uVar12 * 0x90;
          uVar19 = uVar20 + 8;
        }
        puVar7 = operator_new__(uVar19);
        *puVar7 = uVar12;
        puVar18 = puVar7 + 1;
        do {
          uVar12 = uVar20;
          if (uVar20 <= uVar19) {
            uVar12 = uVar19;
          }
          *(undefined2 *)(puVar18 + 0x11) = 0;
          puVar18[0x10] = 0;
          uVar16 = uVar12 - uVar20;
          uVar11 = uVar16;
          if (uVar16 <= uVar12) {
            uVar11 = uVar12;
          }
          lVar22 = __memset_chk(puVar18,0xff,0x80,uVar11 + (uVar20 - uVar12));
          uVar11 = uVar16 + 0x90;
          if (uVar16 + 0x90 < uVar12) {
            uVar11 = uVar12;
          }
          puVar18 = (ulong *)(lVar22 + 0x90);
          uVar20 = (uVar20 - uVar12) + -0x90 + uVar11;
        } while (puVar7 + lVar6 * 0x12 + 0x13 != puVar18);
        uVar19 = *(ulong *)(puVar27 + 4);
        *(ulong **)(puVar5 + 8) = puVar7 + 1;
        if (uVar19 >> 7 != 0) {
          lVar6 = 0;
          do {
            pbVar25 = (byte *)(*(long *)(puVar27 + 8) + lVar6);
            pbVar23 = pbVar25;
            do {
              if (*pbVar23 != 0xff) {
                pbVar26 = *(byte **)(puVar5 + 8);
                piVar17 = (int *)((ulong)*pbVar23 * 0x20 + *(long *)(pbVar25 + 0x80));
                uVar20 = (long)*piVar17 ^ *(ulong *)(puVar5 + 6);
                uVar20 = (uVar20 >> 0x20 ^ uVar20) * -0x2917014799a6026d;
                uVar20 = (uVar20 ^ uVar20 >> 0x20) * -0x2917014799a6026d;
                uVar20 = (uVar20 ^ uVar20 >> 0x20) & *(ulong *)(puVar5 + 4) - 1;
                uVar12 = (ulong)((uint)uVar20 & 0x7f);
                pbVar21 = pbVar26 + (uVar20 >> 7) * 0x90;
                while (uVar20 = (ulong)pbVar21[uVar12], uVar20 != 0xff) {
                  while( true ) {
                    if (*piVar17 == *(int *)(uVar20 * 0x20 + *(long *)(pbVar21 + 0x80)))
                    goto LAB_001130d0;
                    uVar12 = uVar12 + 1;
                    if (uVar12 != 0x80) break;
                    pbVar21 = pbVar21 + 0x90;
                    uVar12 = 0;
                    if (((long)pbVar21 - (long)pbVar26 >> 4) * -0x71c71c71c71c71c7 -
                        (*(ulong *)(puVar5 + 4) >> 7) == 0) {
                      pbVar21 = pbVar26;
                    }
                    uVar20 = (ulong)*pbVar21;
                    if (uVar20 == 0xff) goto LAB_001130d0;
                  }
                }
LAB_001130d0:
                bVar13 = pbVar21[0x89];
                if (bVar13 == pbVar21[0x88]) {
                  if (bVar13 == 0) {
                    pvVar10 = operator_new__(0x600);
                    bVar1 = 0x30;
                    uVar20 = 0;
                    local_50 = 0x30;
                  }
                  else {
                    uVar20 = (ulong)bVar13;
                    if (bVar13 == 0x30) {
                      pvVar10 = operator_new__(0xa00);
                      bVar1 = 0x50;
                      uVar11 = 0xa00;
                      local_50 = 0x50;
                    }
                    else {
                      local_50 = uVar20 + 0x10;
                      uVar11 = local_50 * 0x20;
                      pvVar10 = operator_new__(uVar11);
                      bVar1 = bVar13 + 0x10;
                    }
                    pvVar10 = (void *)__memcpy_chk(pvVar10,*(undefined8 *)(pbVar21 + 0x80),
                                                   uVar20 << 5,uVar11);
                  }
                  do {
                    uVar11 = uVar20 + 1;
                    *(char *)((long)pvVar10 + uVar20 * 0x20) = (char)uVar20 + '\x01';
                    uVar20 = uVar11;
                  } while (uVar11 < local_50);
                  if (*(void **)(pbVar21 + 0x80) != (void *)0x0) {
                    operator_delete__(*(void **)(pbVar21 + 0x80));
                  }
                  *(void **)(pbVar21 + 0x80) = pvVar10;
                  bVar13 = pbVar21[0x89];
                  pbVar21[0x88] = bVar1;
                }
                else {
                  pvVar10 = *(void **)(pbVar21 + 0x80);
                }
                lVar22 = (ulong)bVar13 * 0x20;
                pbVar21[0x89] = *(byte *)((long)pvVar10 + lVar22);
                pbVar21[uVar12] = bVar13;
                uVar2 = *(undefined8 *)(piVar17 + 6);
                *(int *)((long)pvVar10 + lVar22) = *piVar17;
                piVar3 = *(int **)(piVar17 + 2);
                uVar4 = *(undefined8 *)(piVar17 + 4);
                *(undefined8 *)((long)pvVar10 + lVar22 + 0x18) = uVar2;
                puVar9 = (undefined8 *)((long)pvVar10 + lVar22 + 8);
                *puVar9 = piVar3;
                puVar9[1] = uVar4;
                if (piVar3 != (int *)0x0) {
                  LOCK();
                  *piVar3 = *piVar3 + 1;
                  UNLOCK();
                }
              }
              pbVar23 = pbVar23 + 1;
            } while (pbVar25 + 0x80 != pbVar23);
            lVar6 = lVar6 + 0x90;
          } while ((uVar19 >> 7) * 0x90 != lVar6);
        }
        if (*puVar27 != 0xffffffff) {
          LOCK();
          *puVar27 = *puVar27 - 1;
          UNLOCK();
          if (*puVar27 == 0) {
            FUN_00112930(puVar27);
            operator_delete(puVar27,0x28);
          }
        }
        lVar6 = *(long *)(puVar5 + 8);
        uVar14 = *(ulong *)(puVar5 + 4) - 1 & uVar14;
        uVar20 = (ulong)((uint)uVar14 & 0x7f);
        lVar22 = (uVar14 >> 7) * 0x90 + lVar6;
        while ((DAT_001292a8 = puVar5, (ulong)*(byte *)(lVar22 + uVar20) != 0xff &&
               (*param_2 !=
                *(int *)((ulong)*(byte *)(lVar22 + uVar20) * 0x20 + *(long *)(lVar22 + 0x80))))) {
          uVar20 = uVar20 + 1;
          if (uVar20 == 0x80) {
            lVar22 = lVar22 + 0x90;
            uVar20 = 0;
            if ((lVar22 - lVar6 >> 4) * -0x71c71c71c71c71c7 - (*(ulong *)(puVar5 + 4) >> 7) == 0) {
              lVar22 = lVar6;
            }
          }
        }
        goto LAB_00112c2c;
      }
      bVar13 = 0xff;
    }
    else {
LAB_00112b3e:
      if (*puVar8 == 0xffffffff) {
        uVar15 = 1;
        puVar27 = puVar8;
        goto LAB_00112b52;
      }
      uVar15 = 1;
      bVar13 = 0xff;
LAB_00112e1c:
      LOCK();
      *puVar8 = *puVar8 + 1;
      UNLOCK();
      puVar27 = DAT_001292a8;
      if (bVar13 == 0xff) goto LAB_00112b52;
    }
LAB_00112b6b:
    uVar14 = uVar20 | (lVar22 - *(long *)(puVar27 + 8) >> 4) * 0x1c71c71c71c71c80;
    puVar5 = operator_new(0x28);
    FUN_001117b0(puVar5,puVar27);
    if (*puVar27 != 0xffffffff) {
      LOCK();
      *puVar27 = *puVar27 - 1;
      UNLOCK();
      if (*puVar27 == 0) {
        FUN_00112930(puVar27);
        operator_delete(puVar27,0x28);
      }
    }
    uVar20 = (ulong)((uint)uVar14 & 0x7f);
    lVar22 = (uVar14 >> 7) * 0x90 + *(long *)(puVar5 + 8);
    DAT_001292a8 = puVar5;
    if (bVar13 != 0xff) goto LAB_00112c86;
  }
LAB_00112c2c:
  bVar13 = *(byte *)(lVar22 + 0x89);
  if (bVar13 == *(byte *)(lVar22 + 0x88)) {
    if (bVar13 == 0) {
      cVar24 = '0';
      pvVar10 = operator_new__(0x600);
      uVar19 = 0;
      uVar14 = 0x30;
    }
    else {
      uVar19 = (ulong)bVar13;
      if (bVar13 == 0x30) {
        cVar24 = 'P';
        pvVar10 = operator_new__(0xa00);
        uVar12 = 0xa00;
        uVar14 = 0x50;
      }
      else {
        uVar14 = uVar19 + 0x10;
        cVar24 = bVar13 + 0x10;
        uVar12 = uVar14 * 0x20;
        pvVar10 = operator_new__(uVar12);
      }
      pvVar10 = (void *)__memcpy_chk(pvVar10,*(undefined8 *)(lVar22 + 0x80),uVar19 << 5,uVar12);
    }
    do {
      uVar12 = uVar19 + 1;
      *(char *)((long)pvVar10 + uVar19 * 0x20) = (char)uVar19 + '\x01';
      uVar19 = uVar12;
    } while (uVar12 < uVar14);
    if (*(void **)(lVar22 + 0x80) != (void *)0x0) {
      operator_delete__(*(void **)(lVar22 + 0x80));
    }
    *(char *)(lVar22 + 0x88) = cVar24;
    bVar13 = *(byte *)(lVar22 + 0x89);
    *(void **)(lVar22 + 0x80) = pvVar10;
  }
  else {
    pvVar10 = *(void **)(lVar22 + 0x80);
  }
  lVar6 = (ulong)bVar13 * 0x20;
  *(undefined1 *)(lVar22 + 0x89) = *(undefined1 *)((long)pvVar10 + lVar6);
  *(byte *)(lVar22 + uVar20) = bVar13;
  *(undefined1 (*) [16])((long)pvVar10 + lVar6) = (undefined1  [16])0x0;
  *(undefined1 (*) [16])((long)pvVar10 + lVar6 + 0x10) = (undefined1  [16])0x0;
  *(int *)((long)pvVar10 + lVar6) = *param_2;
  puVar5 = DAT_001292a8;
  *(long *)(DAT_001292a8 + 2) = *(long *)(DAT_001292a8 + 2) + 1;
LAB_00112c86:
  lVar6 = *(long *)(puVar5 + 8);
  *param_1 = puVar5;
  *(undefined1 *)(param_1 + 2) = uVar15;
  param_1[1] = (lVar22 - lVar6 >> 4) * 0x1c71c71c71c71c80 | uVar20;
  if ((puVar8 != (uint *)0x0) && (*puVar8 != 0xffffffff)) {
    LOCK();
    *puVar8 = *puVar8 - 1;
    UNLOCK();
    if (*puVar8 == 0) {
      pbVar23 = *(byte **)(puVar8 + 8);
      if (pbVar23 != (byte *)0x0) {
        pbVar25 = pbVar23 + *(long *)(pbVar23 + -8) * 0x90;
        if (pbVar23 == pbVar25) {
          lVar6 = 0;
        }
        else {
          pbVar23 = pbVar25 + -0x10;
          do {
            pbVar26 = pbVar25;
            pvVar10 = *(void **)pbVar23;
            pbVar25 = pbVar26 + -0x90;
            if (pvVar10 != (void *)0x0) {
              pbVar21 = pbVar25;
              if (pbVar25 != pbVar23) {
                do {
                  bVar13 = *pbVar21;
                  if ((bVar13 != 0xff) &&
                     (piVar17 = *(int **)((long)pvVar10 + (ulong)bVar13 * 0x20 + 8),
                     piVar17 != (int *)0x0)) {
                    LOCK();
                    *piVar17 = *piVar17 + -1;
                    UNLOCK();
                    if (*piVar17 == 0) {
                      QArrayData::deallocate
                                (*(QArrayData **)((long)pvVar10 + (ulong)bVar13 * 0x20 + 8),2,0x10);
                    }
                    pvVar10 = *(void **)pbVar23;
                  }
                  pbVar21 = pbVar21 + 1;
                } while (pbVar23 != pbVar21);
                if (pvVar10 == (void *)0x0) goto LAB_00112d69;
              }
              operator_delete__(pvVar10);
            }
LAB_00112d69:
            pbVar23 = pbVar23 + -0x90;
          } while (*(byte **)(puVar8 + 8) != pbVar25);
          lVar6 = *(long *)(pbVar26 + -0x98) * 0x90;
        }
        operator_delete__(pbVar25 + -8,lVar6 + 8);
      }
      operator_delete(puVar8,0x28);
    }
  }
  return param_1;
}

