// rea decompile_all
// name: FUN_00116ca0
// address: 0x116ca0
// body: available
// range: 0x116ca0-0x116e8a inclusive
// range: 0x116e90-0x117048 inclusive
// range: 0x117050-0x1170b4 inclusive
// range: 0x1170b8-0x11749e inclusive
// range: 0x1174a0-0x1175a2 inclusive
// range: 0x1175a8-0x1177e3 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 * FUN_00116ca0(undefined8 *param_1,uint *param_2)

{
  byte bVar1;
  int *piVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  ulong uVar5;
  uint *puVar6;
  long lVar7;
  ulong *puVar8;
  uint *puVar9;
  undefined8 *puVar10;
  void *pvVar11;
  ulong uVar12;
  ulong uVar13;
  byte bVar14;
  undefined1 uVar15;
  ulong uVar16;
  uint *puVar17;
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
  
  if (DAT_001292b8 == (uint *)0x0) {
    puVar9 = operator_new(0x28);
    *puVar9 = 1;
    puVar9[6] = 0;
    puVar9[7] = 0;
    puVar9[2] = 0;
    puVar9[3] = 0;
    puVar9[4] = 0x80;
    puVar9[5] = 0;
    puVar10 = operator_new__(0x98);
    *puVar10 = 1;
    puVar10[0x11] = 0;
    *(undefined2 *)(puVar10 + 0x12) = 0;
    *(undefined4 *)(puVar10 + 1) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0xc) = 0xffffffff;
    *(undefined4 *)(puVar10 + 2) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x14) = 0xffffffff;
    *(undefined4 *)(puVar10 + 3) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x1c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 4) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x24) = 0xffffffff;
    *(undefined4 *)(puVar10 + 5) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x2c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 6) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x34) = 0xffffffff;
    *(undefined4 *)(puVar10 + 7) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x3c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 8) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x44) = 0xffffffff;
    *(undefined4 *)(puVar10 + 9) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x4c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 10) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x54) = 0xffffffff;
    *(undefined4 *)(puVar10 + 0xb) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x5c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 0xc) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 100) = 0xffffffff;
    *(undefined4 *)(puVar10 + 0xd) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x6c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 0xe) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x74) = 0xffffffff;
    *(undefined4 *)(puVar10 + 0xf) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x7c) = 0xffffffff;
    *(undefined4 *)(puVar10 + 0x10) = 0xffffffff;
    *(undefined4 *)((long)puVar10 + 0x84) = 0xffffffff;
    *(undefined8 **)(puVar9 + 8) = puVar10 + 1;
    uVar5 = QHashSeed::globalSeed();
    DAT_001292b8 = puVar9;
    *(ulong *)(puVar9 + 6) = uVar5;
  }
  else {
    uVar5 = *(ulong *)(DAT_001292b8 + 6);
    puVar9 = DAT_001292b8;
  }
  uVar5 = (uVar5 >> 0x20 ^ *param_2 ^ uVar5) * -0x2917014799a6026d;
  lVar7 = *(long *)(puVar9 + 8);
  uVar5 = (uVar5 ^ uVar5 >> 0x20) * -0x2917014799a6026d;
  uVar5 = uVar5 ^ uVar5 >> 0x20;
  uVar19 = *(ulong *)(puVar9 + 4) - 1 & uVar5;
  uVar20 = (ulong)((uint)uVar19 & 0x7f);
  lVar22 = (uVar19 >> 7) * 0x90 + lVar7;
  while( true ) {
    bVar14 = *(byte *)(lVar22 + uVar20);
    if ((ulong)bVar14 == 0xff) break;
    if (*param_2 == *(uint *)((ulong)bVar14 * 0x20 + *(long *)(lVar22 + 0x80))) {
      if (*puVar9 < 2) {
        lVar7 = *(long *)(puVar9 + 8);
        *param_1 = puVar9;
        *(undefined1 *)(param_1 + 2) = 0;
        param_1[1] = (lVar22 - lVar7 >> 4) * 0x1c71c71c71c71c80 | uVar20;
        return param_1;
      }
      if (puVar9 == (uint *)0x0) {
        puVar27 = (uint *)0x0;
        uVar15 = 0;
        goto LAB_00116deb;
      }
      if (*puVar9 == 0xffffffff) {
        uVar15 = 0;
        puVar27 = puVar9;
        goto LAB_00116deb;
      }
      uVar15 = 0;
      goto LAB_0011709c;
    }
    uVar20 = uVar20 + 1;
    if (uVar20 == 0x80) {
      lVar22 = lVar22 + 0x90;
      uVar20 = 0;
      if ((lVar22 - lVar7 >> 4) * -0x71c71c71c71c71c7 - (*(ulong *)(puVar9 + 4) >> 7) == 0) {
        lVar22 = lVar7;
      }
    }
  }
  if (*puVar9 < 2) {
    if (*(ulong *)(puVar9 + 4) >> 1 <= *(ulong *)(puVar9 + 2)) goto LAB_00116dbe;
    uVar15 = 1;
    puVar9 = (uint *)0x0;
  }
  else {
    uVar15 = 1;
    puVar27 = (uint *)0x0;
    if (puVar9 == (uint *)0x0) {
LAB_00116dd2:
      uVar19 = *(ulong *)(puVar27 + 2);
      if (*(ulong *)(puVar27 + 4) >> 1 <= uVar19) {
        puVar6 = operator_new(0x28);
        *puVar6 = 1;
        *(ulong *)(puVar6 + 2) = uVar19;
        uVar3 = *(undefined8 *)(puVar27 + 6);
        puVar6[8] = 0;
        puVar6[9] = 0;
        *(undefined8 *)(puVar6 + 6) = uVar3;
        uVar20 = uVar19 + 1;
        if (uVar19 + 1 < uVar19) {
          uVar20 = uVar19;
        }
        if (uVar20 < 0x41) {
          puVar6[4] = 0x80;
          puVar6[5] = 0;
          lVar7 = 0;
          uVar19 = 0x98;
          uVar20 = 0x90;
          uVar13 = 1;
        }
        else {
          lVar7 = 0x3f;
          if (uVar20 != 0) {
            for (; uVar20 >> lVar7 == 0; lVar7 = lVar7 + -1) {
            }
          }
          uVar13 = 1L << ((char)lVar7 + 2U & 0x3f);
          *(ulong *)(puVar6 + 4) = uVar13;
          uVar13 = uVar13 >> 7;
          lVar7 = uVar13 - 1;
          uVar20 = uVar13 * 0x90;
          uVar19 = uVar20 + 8;
        }
        puVar8 = operator_new__(uVar19);
        *puVar8 = uVar13;
        puVar18 = puVar8 + 1;
        do {
          uVar13 = uVar20;
          if (uVar20 <= uVar19) {
            uVar13 = uVar19;
          }
          *(undefined2 *)(puVar18 + 0x11) = 0;
          puVar18[0x10] = 0;
          uVar16 = uVar13 - uVar20;
          uVar12 = uVar16;
          if (uVar16 <= uVar13) {
            uVar12 = uVar13;
          }
          lVar22 = __memset_chk(puVar18,0xff,0x80,uVar12 + (uVar20 - uVar13));
          uVar12 = uVar16 + 0x90;
          if (uVar16 + 0x90 < uVar13) {
            uVar12 = uVar13;
          }
          puVar18 = (ulong *)(lVar22 + 0x90);
          uVar20 = (uVar20 - uVar13) + -0x90 + uVar12;
        } while (puVar8 + lVar7 * 0x12 + 0x13 != puVar18);
        uVar19 = *(ulong *)(puVar27 + 4);
        *(ulong **)(puVar6 + 8) = puVar8 + 1;
        if (uVar19 >> 7 != 0) {
          lVar7 = 0;
          do {
            pbVar25 = (byte *)(*(long *)(puVar27 + 8) + lVar7);
            pbVar23 = pbVar25;
            do {
              if (*pbVar23 != 0xff) {
                pbVar26 = *(byte **)(puVar6 + 8);
                puVar17 = (uint *)((ulong)*pbVar23 * 0x20 + *(long *)(pbVar25 + 0x80));
                uVar20 = (*(ulong *)(puVar6 + 6) >> 0x20 ^ (ulong)*puVar17 ^ *(ulong *)(puVar6 + 6))
                         * -0x2917014799a6026d;
                uVar20 = (uVar20 ^ uVar20 >> 0x20) * -0x2917014799a6026d;
                uVar20 = (uVar20 ^ uVar20 >> 0x20) & *(ulong *)(puVar6 + 4) - 1;
                uVar13 = (ulong)((uint)uVar20 & 0x7f);
                pbVar21 = pbVar26 + (uVar20 >> 7) * 0x90;
                while (uVar20 = (ulong)pbVar21[uVar13], uVar20 != 0xff) {
                  while( true ) {
                    if (*puVar17 == *(uint *)(uVar20 * 0x20 + *(long *)(pbVar21 + 0x80)))
                    goto LAB_00117340;
                    uVar13 = uVar13 + 1;
                    if (uVar13 != 0x80) break;
                    pbVar21 = pbVar21 + 0x90;
                    uVar13 = 0;
                    if (((long)pbVar21 - (long)pbVar26 >> 4) * -0x71c71c71c71c71c7 -
                        (*(ulong *)(puVar6 + 4) >> 7) == 0) {
                      pbVar21 = pbVar26;
                    }
                    uVar20 = (ulong)*pbVar21;
                    if (uVar20 == 0xff) goto LAB_00117340;
                  }
                }
LAB_00117340:
                bVar14 = pbVar21[0x89];
                if (bVar14 == pbVar21[0x88]) {
                  if (bVar14 == 0) {
                    pvVar11 = operator_new__(0x600);
                    bVar1 = 0x30;
                    uVar20 = 0;
                    local_50 = 0x30;
                  }
                  else {
                    uVar20 = (ulong)bVar14;
                    if (bVar14 == 0x30) {
                      pvVar11 = operator_new__(0xa00);
                      bVar1 = 0x50;
                      uVar12 = 0xa00;
                      local_50 = 0x50;
                    }
                    else {
                      local_50 = uVar20 + 0x10;
                      uVar12 = local_50 * 0x20;
                      pvVar11 = operator_new__(uVar12);
                      bVar1 = bVar14 + 0x10;
                    }
                    pvVar11 = (void *)__memcpy_chk(pvVar11,*(undefined8 *)(pbVar21 + 0x80),
                                                   uVar20 << 5,uVar12);
                  }
                  do {
                    uVar12 = uVar20 + 1;
                    *(char *)((long)pvVar11 + uVar20 * 0x20) = (char)uVar20 + '\x01';
                    uVar20 = uVar12;
                  } while (uVar12 < local_50);
                  if (*(void **)(pbVar21 + 0x80) != (void *)0x0) {
                    operator_delete__(*(void **)(pbVar21 + 0x80));
                  }
                  *(void **)(pbVar21 + 0x80) = pvVar11;
                  bVar14 = pbVar21[0x89];
                  pbVar21[0x88] = bVar1;
                }
                else {
                  pvVar11 = *(void **)(pbVar21 + 0x80);
                }
                lVar22 = (ulong)bVar14 * 0x20;
                pbVar21[0x89] = *(byte *)((long)pvVar11 + lVar22);
                pbVar21[uVar13] = bVar14;
                uVar3 = *(undefined8 *)(puVar17 + 6);
                *(uint *)((long)pvVar11 + lVar22) = *puVar17;
                piVar2 = *(int **)(puVar17 + 2);
                uVar4 = *(undefined8 *)(puVar17 + 4);
                *(undefined8 *)((long)pvVar11 + lVar22 + 0x18) = uVar3;
                puVar10 = (undefined8 *)((long)pvVar11 + lVar22 + 8);
                *puVar10 = piVar2;
                puVar10[1] = uVar4;
                if (piVar2 != (int *)0x0) {
                  LOCK();
                  *piVar2 = *piVar2 + 1;
                  UNLOCK();
                }
              }
              pbVar23 = pbVar23 + 1;
            } while (pbVar25 + 0x80 != pbVar23);
            lVar7 = lVar7 + 0x90;
          } while ((uVar19 >> 7) * 0x90 != lVar7);
        }
        if (*puVar27 != 0xffffffff) {
          LOCK();
          *puVar27 = *puVar27 - 1;
          UNLOCK();
          if (*puVar27 == 0) {
            FUN_00111b10(puVar27);
            operator_delete(puVar27,0x28);
          }
        }
        lVar7 = *(long *)(puVar6 + 8);
        uVar5 = *(ulong *)(puVar6 + 4) - 1 & uVar5;
        uVar20 = (ulong)((uint)uVar5 & 0x7f);
        lVar22 = (uVar5 >> 7) * 0x90 + lVar7;
        while ((DAT_001292b8 = puVar6, (ulong)*(byte *)(lVar22 + uVar20) != 0xff &&
               (*param_2 !=
                *(uint *)((ulong)*(byte *)(lVar22 + uVar20) * 0x20 + *(long *)(lVar22 + 0x80))))) {
          uVar20 = uVar20 + 1;
          if (uVar20 == 0x80) {
            lVar22 = lVar22 + 0x90;
            uVar20 = 0;
            if ((lVar22 - lVar7 >> 4) * -0x71c71c71c71c71c7 - (*(ulong *)(puVar6 + 4) >> 7) == 0) {
              lVar22 = lVar7;
            }
          }
        }
        goto LAB_00116eac;
      }
      bVar14 = 0xff;
    }
    else {
LAB_00116dbe:
      if (*puVar9 == 0xffffffff) {
        uVar15 = 1;
        puVar27 = puVar9;
        goto LAB_00116dd2;
      }
      uVar15 = 1;
      bVar14 = 0xff;
LAB_0011709c:
      LOCK();
      *puVar9 = *puVar9 + 1;
      UNLOCK();
      puVar27 = DAT_001292b8;
      if (bVar14 == 0xff) goto LAB_00116dd2;
    }
LAB_00116deb:
    uVar5 = uVar20 | (lVar22 - *(long *)(puVar27 + 8) >> 4) * 0x1c71c71c71c71c80;
    puVar6 = operator_new(0x28);
    FUN_00111100(puVar6,puVar27);
    if (*puVar27 != 0xffffffff) {
      LOCK();
      *puVar27 = *puVar27 - 1;
      UNLOCK();
      if (*puVar27 == 0) {
        FUN_00111b10(puVar27);
        operator_delete(puVar27,0x28);
      }
    }
    uVar20 = (ulong)((uint)uVar5 & 0x7f);
    lVar22 = (uVar5 >> 7) * 0x90 + *(long *)(puVar6 + 8);
    DAT_001292b8 = puVar6;
    if (bVar14 != 0xff) goto LAB_00116f06;
  }
LAB_00116eac:
  bVar14 = *(byte *)(lVar22 + 0x89);
  if (bVar14 == *(byte *)(lVar22 + 0x88)) {
    if (bVar14 == 0) {
      cVar24 = '0';
      pvVar11 = operator_new__(0x600);
      uVar19 = 0;
      uVar5 = 0x30;
    }
    else {
      uVar19 = (ulong)bVar14;
      if (bVar14 == 0x30) {
        cVar24 = 'P';
        pvVar11 = operator_new__(0xa00);
        uVar13 = 0xa00;
        uVar5 = 0x50;
      }
      else {
        uVar5 = uVar19 + 0x10;
        cVar24 = bVar14 + 0x10;
        uVar13 = uVar5 * 0x20;
        pvVar11 = operator_new__(uVar13);
      }
      pvVar11 = (void *)__memcpy_chk(pvVar11,*(undefined8 *)(lVar22 + 0x80),uVar19 << 5,uVar13);
    }
    do {
      uVar13 = uVar19 + 1;
      *(char *)((long)pvVar11 + uVar19 * 0x20) = (char)uVar19 + '\x01';
      uVar19 = uVar13;
    } while (uVar13 < uVar5);
    if (*(void **)(lVar22 + 0x80) != (void *)0x0) {
      operator_delete__(*(void **)(lVar22 + 0x80));
    }
    *(char *)(lVar22 + 0x88) = cVar24;
    bVar14 = *(byte *)(lVar22 + 0x89);
    *(void **)(lVar22 + 0x80) = pvVar11;
  }
  else {
    pvVar11 = *(void **)(lVar22 + 0x80);
  }
  lVar7 = (ulong)bVar14 * 0x20;
  *(undefined1 *)(lVar22 + 0x89) = *(undefined1 *)((long)pvVar11 + lVar7);
  *(byte *)(lVar22 + uVar20) = bVar14;
  *(undefined1 (*) [16])((long)pvVar11 + lVar7) = (undefined1  [16])0x0;
  *(undefined1 (*) [16])((long)pvVar11 + lVar7 + 0x10) = (undefined1  [16])0x0;
  *(uint *)((long)pvVar11 + lVar7) = *param_2;
  puVar6 = DAT_001292b8;
  *(long *)(DAT_001292b8 + 2) = *(long *)(DAT_001292b8 + 2) + 1;
LAB_00116f06:
  lVar7 = *(long *)(puVar6 + 8);
  *param_1 = puVar6;
  *(undefined1 *)(param_1 + 2) = uVar15;
  param_1[1] = (lVar22 - lVar7 >> 4) * 0x1c71c71c71c71c80 | uVar20;
  if ((puVar9 != (uint *)0x0) && (*puVar9 != 0xffffffff)) {
    LOCK();
    *puVar9 = *puVar9 - 1;
    UNLOCK();
    if (*puVar9 == 0) {
      pbVar23 = *(byte **)(puVar9 + 8);
      if (pbVar23 != (byte *)0x0) {
        pbVar25 = pbVar23 + *(long *)(pbVar23 + -8) * 0x90;
        if (pbVar23 == pbVar25) {
          lVar7 = 0;
        }
        else {
          pbVar23 = pbVar25 + -0x10;
          do {
            pbVar26 = pbVar25;
            pvVar11 = *(void **)pbVar23;
            pbVar25 = pbVar26 + -0x90;
            if (pvVar11 != (void *)0x0) {
              pbVar21 = pbVar25;
              if (pbVar25 != pbVar23) {
                do {
                  bVar14 = *pbVar21;
                  if ((bVar14 != 0xff) &&
                     (piVar2 = *(int **)((long)pvVar11 + (ulong)bVar14 * 0x20 + 8),
                     piVar2 != (int *)0x0)) {
                    LOCK();
                    *piVar2 = *piVar2 + -1;
                    UNLOCK();
                    if (*piVar2 == 0) {
                      QArrayData::deallocate
                                (*(QArrayData **)((long)pvVar11 + (ulong)bVar14 * 0x20 + 8),2,0x10);
                    }
                    pvVar11 = *(void **)pbVar23;
                  }
                  pbVar21 = pbVar21 + 1;
                } while (pbVar23 != pbVar21);
                if (pvVar11 == (void *)0x0) goto LAB_00116fe9;
              }
              operator_delete__(pvVar11);
            }
LAB_00116fe9:
            pbVar23 = pbVar23 + -0x90;
          } while (*(byte **)(puVar9 + 8) != pbVar25);
          lVar7 = *(long *)(pbVar26 + -0x98) * 0x90;
        }
        operator_delete__(pbVar25 + -8,lVar7 + 8);
      }
      operator_delete(puVar9,0x28);
    }
  }
  return param_1;
}

