// rea decompile_all
// name: FUN_00111d60
// address: 0x111d60
// body: available
// range: 0x111d60-0x111e9b inclusive
// range: 0x111ea0-0x1120d6 inclusive
// range: 0x1120e0-0x1121f0 inclusive
// range: 0x1121f8-0x1125cc inclusive
// range: 0x1125d0-0x1126c2 inclusive
// range: 0x1126c8-0x1127e3 inclusive
// range: 0x1127e8-0x112923 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 * FUN_00111d60(undefined8 *param_1,uint *param_2)

{
  byte bVar1;
  int *piVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  ulong uVar5;
  long lVar6;
  uint *puVar7;
  ulong *puVar8;
  undefined8 *puVar9;
  void *pvVar10;
  ulong uVar11;
  ulong uVar12;
  byte *pbVar13;
  undefined1 uVar14;
  undefined8 *puVar15;
  uint *puVar16;
  ulong *puVar17;
  ulong uVar18;
  ulong uVar19;
  long lVar20;
  byte *pbVar21;
  ulong uVar22;
  byte *pbVar23;
  byte bVar24;
  char cVar25;
  byte *pbVar26;
  uint *puVar27;
  uint *local_70;
  ulong local_50;
  byte *local_40;
  
  if (DAT_001292b0 == (uint *)0x0) {
    local_70 = operator_new(0x28);
    *local_70 = 1;
    local_70[6] = 0;
    local_70[7] = 0;
    local_70[2] = 0;
    local_70[3] = 0;
    local_70[4] = 0x80;
    local_70[5] = 0;
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
    *(undefined8 **)(local_70 + 8) = puVar9 + 1;
    uVar5 = QHashSeed::globalSeed();
    DAT_001292b0 = local_70;
    *(ulong *)(local_70 + 6) = uVar5;
  }
  else {
    uVar5 = *(ulong *)(DAT_001292b0 + 6);
    local_70 = DAT_001292b0;
  }
  uVar5 = (uVar5 >> 0x20 ^ *param_2 ^ uVar5) * -0x2917014799a6026d;
  lVar6 = *(long *)(local_70 + 8);
  uVar5 = (uVar5 ^ uVar5 >> 0x20) * -0x2917014799a6026d;
  uVar5 = uVar5 ^ uVar5 >> 0x20;
  uVar18 = *(ulong *)(local_70 + 4) - 1 & uVar5;
  uVar19 = (ulong)((uint)uVar18 & 0x7f);
  lVar20 = (uVar18 >> 7) * 0x90 + lVar6;
  while( true ) {
    bVar24 = *(byte *)(lVar20 + uVar19);
    puVar27 = local_70;
    if ((ulong)bVar24 == 0xff) break;
    if (*param_2 == *(uint *)((ulong)bVar24 * 0x20 + *(long *)(lVar20 + 0x80))) {
      if (*local_70 < 2) {
        lVar6 = *(long *)(local_70 + 8);
        *param_1 = local_70;
        *(undefined1 *)(param_1 + 2) = 0;
        param_1[1] = (lVar20 - lVar6 >> 4) * 0x1c71c71c71c71c80 | uVar19;
        return param_1;
      }
      if (local_70 == (uint *)0x0) {
        puVar27 = (uint *)0x0;
        uVar14 = 0;
        goto LAB_0011214e;
      }
      uVar14 = 0;
      if (*local_70 != 0xffffffff) goto LAB_001127cb;
      goto LAB_0011214e;
    }
    uVar19 = uVar19 + 1;
    if (uVar19 == 0x80) {
      lVar20 = lVar20 + 0x90;
      uVar19 = 0;
      if ((lVar20 - lVar6 >> 4) * -0x71c71c71c71c71c7 - (*(ulong *)(local_70 + 4) >> 7) == 0) {
        lVar20 = lVar6;
      }
    }
  }
  if (*local_70 < 2) {
    if (*(ulong *)(local_70 + 4) >> 1 <= *(ulong *)(local_70 + 2)) {
      if (*local_70 != 0xffffffff) {
        uVar14 = 1;
        bVar24 = 0xff;
        goto LAB_001127cb;
      }
      uVar14 = 1;
      goto LAB_00112135;
    }
    local_70 = (uint *)0x0;
    uVar14 = 1;
  }
  else {
    uVar14 = 1;
    puVar27 = (uint *)0x0;
    if ((local_70 == (uint *)0x0) || (uVar14 = 1, puVar27 = local_70, *local_70 == 0xffffffff)) {
LAB_00112135:
      uVar18 = *(ulong *)(puVar27 + 2);
      if (*(ulong *)(puVar27 + 4) >> 1 <= uVar18) {
        puVar7 = operator_new(0x28);
        *puVar7 = 1;
        *(ulong *)(puVar7 + 2) = uVar18;
        uVar3 = *(undefined8 *)(puVar27 + 6);
        puVar7[8] = 0;
        puVar7[9] = 0;
        *(undefined8 *)(puVar7 + 6) = uVar3;
        uVar19 = uVar18 + 1;
        if (uVar18 + 1 < uVar18) {
          uVar19 = uVar18;
        }
        if (uVar19 < 0x41) {
          puVar7[4] = 0x80;
          puVar7[5] = 0;
          lVar6 = 0;
          uVar18 = 0x98;
          uVar19 = 0x90;
          uVar12 = 1;
        }
        else {
          lVar6 = 0x3f;
          if (uVar19 != 0) {
            for (; uVar19 >> lVar6 == 0; lVar6 = lVar6 + -1) {
            }
          }
          uVar12 = 1L << ((char)lVar6 + 2U & 0x3f);
          *(ulong *)(puVar7 + 4) = uVar12;
          uVar12 = uVar12 >> 7;
          lVar6 = uVar12 - 1;
          uVar19 = uVar12 * 0x90;
          uVar18 = uVar19 + 8;
        }
        puVar8 = operator_new__(uVar18);
        *puVar8 = uVar12;
        puVar17 = puVar8 + 1;
        do {
          uVar12 = uVar19;
          if (uVar19 <= uVar18) {
            uVar12 = uVar18;
          }
          *(undefined2 *)(puVar17 + 0x11) = 0;
          puVar17[0x10] = 0;
          uVar22 = uVar12 - uVar19;
          uVar11 = uVar12;
          if (uVar12 <= uVar22) {
            uVar11 = uVar22;
          }
          lVar20 = __memset_chk(puVar17,0xff,0x80,uVar11 + (uVar19 - uVar12));
          uVar11 = uVar22 + 0x90;
          if (uVar22 + 0x90 < uVar12) {
            uVar11 = uVar12;
          }
          puVar17 = (ulong *)(lVar20 + 0x90);
          uVar19 = (uVar19 - uVar12) + -0x90 + uVar11;
        } while (puVar8 + lVar6 * 0x12 + 0x13 != puVar17);
        *(ulong **)(puVar7 + 8) = puVar8 + 1;
        uVar18 = *(ulong *)(puVar27 + 4);
        if (uVar18 >> 7 != 0) {
          lVar6 = 0;
          do {
            pbVar13 = (byte *)(*(long *)(puVar27 + 8) + lVar6);
            pbVar21 = pbVar13;
            do {
              if (*pbVar21 != 0xff) {
                pbVar26 = *(byte **)(puVar7 + 8);
                puVar16 = (uint *)((ulong)*pbVar21 * 0x20 + *(long *)(pbVar13 + 0x80));
                uVar19 = (*(ulong *)(puVar7 + 6) >> 0x20 ^ (ulong)*puVar16 ^ *(ulong *)(puVar7 + 6))
                         * -0x2917014799a6026d;
                uVar19 = (uVar19 ^ uVar19 >> 0x20) * -0x2917014799a6026d;
                uVar19 = (uVar19 ^ uVar19 >> 0x20) & *(ulong *)(puVar7 + 4) - 1;
                uVar12 = (ulong)((uint)uVar19 & 0x7f);
                pbVar23 = pbVar26 + (uVar19 >> 7) * 0x90;
                while (uVar19 = (ulong)pbVar23[uVar12], uVar19 != 0xff) {
                  while( true ) {
                    if (*puVar16 == *(uint *)(uVar19 * 0x20 + *(long *)(pbVar23 + 0x80)))
                    goto LAB_00112470;
                    uVar12 = uVar12 + 1;
                    if (uVar12 != 0x80) break;
                    pbVar23 = pbVar23 + 0x90;
                    uVar12 = 0;
                    if (((long)pbVar23 - (long)pbVar26 >> 4) * -0x71c71c71c71c71c7 -
                        (*(ulong *)(puVar7 + 4) >> 7) == 0) {
                      pbVar23 = pbVar26;
                    }
                    uVar19 = (ulong)*pbVar23;
                    if (uVar19 == 0xff) goto LAB_00112470;
                  }
                }
LAB_00112470:
                bVar24 = pbVar23[0x89];
                if (bVar24 == pbVar23[0x88]) {
                  if (bVar24 == 0) {
                    pvVar10 = operator_new__(0x600);
                    bVar1 = 0x30;
                    uVar19 = 0;
                    local_50 = 0x30;
                  }
                  else {
                    uVar19 = (ulong)bVar24;
                    if (bVar24 == 0x30) {
                      pvVar10 = operator_new__(0xa00);
                      bVar1 = 0x50;
                      uVar11 = 0xa00;
                      local_50 = 0x50;
                    }
                    else {
                      local_50 = uVar19 + 0x10;
                      uVar11 = local_50 * 0x20;
                      pvVar10 = operator_new__(uVar11);
                      bVar1 = bVar24 + 0x10;
                    }
                    pvVar10 = (void *)__memcpy_chk(pvVar10,*(undefined8 *)(pbVar23 + 0x80),
                                                   uVar19 << 5,uVar11);
                  }
                  do {
                    uVar11 = uVar19 + 1;
                    *(char *)((long)pvVar10 + uVar19 * 0x20) = (char)uVar19 + '\x01';
                    uVar19 = uVar11;
                  } while (uVar11 < local_50);
                  if (*(void **)(pbVar23 + 0x80) != (void *)0x0) {
                    operator_delete__(*(void **)(pbVar23 + 0x80));
                  }
                  *(void **)(pbVar23 + 0x80) = pvVar10;
                  bVar24 = pbVar23[0x89];
                  pbVar23[0x88] = bVar1;
                }
                else {
                  pvVar10 = *(void **)(pbVar23 + 0x80);
                }
                lVar20 = (ulong)bVar24 * 0x20;
                pbVar23[0x89] = *(byte *)((long)pvVar10 + lVar20);
                pbVar23[uVar12] = bVar24;
                uVar3 = *(undefined8 *)(puVar16 + 6);
                *(uint *)((long)pvVar10 + lVar20) = *puVar16;
                piVar2 = *(int **)(puVar16 + 2);
                uVar4 = *(undefined8 *)(puVar16 + 4);
                *(undefined8 *)((long)pvVar10 + lVar20 + 0x18) = uVar3;
                puVar9 = (undefined8 *)((long)pvVar10 + lVar20 + 8);
                *puVar9 = piVar2;
                puVar9[1] = uVar4;
                if (piVar2 != (int *)0x0) {
                  LOCK();
                  *piVar2 = *piVar2 + 1;
                  UNLOCK();
                }
              }
              pbVar21 = pbVar21 + 1;
            } while (pbVar13 + 0x80 != pbVar21);
            lVar6 = lVar6 + 0x90;
          } while ((uVar18 >> 7) * 0x90 != lVar6);
        }
        if (*puVar27 != 0xffffffff) {
          LOCK();
          *puVar27 = *puVar27 - 1;
          UNLOCK();
          if (*puVar27 == 0) {
            FUN_00111c00(puVar27);
            operator_delete(puVar27,0x28);
          }
        }
        lVar6 = *(long *)(puVar7 + 8);
        uVar5 = *(ulong *)(puVar7 + 4) - 1 & uVar5;
        uVar19 = (ulong)((uint)uVar5 & 0x7f);
        lVar20 = (uVar5 >> 7) * 0x90 + lVar6;
        while ((DAT_001292b0 = puVar7, (ulong)*(byte *)(lVar20 + uVar19) != 0xff &&
               (*param_2 !=
                *(uint *)((ulong)*(byte *)(lVar20 + uVar19) * 0x20 + *(long *)(lVar20 + 0x80))))) {
          uVar19 = uVar19 + 1;
          if (uVar19 == 0x80) {
            lVar20 = lVar20 + 0x90;
            uVar19 = 0;
            if ((lVar20 - lVar6 >> 4) * -0x71c71c71c71c71c7 - (*(ulong *)(puVar7 + 4) >> 7) == 0) {
              lVar20 = lVar6;
            }
          }
        }
        goto LAB_00111ec6;
      }
      bVar24 = 0xff;
    }
    else {
      bVar24 = 0xff;
LAB_001127cb:
      LOCK();
      *local_70 = *local_70 + 1;
      UNLOCK();
      puVar27 = DAT_001292b0;
      if (bVar24 == 0xff) goto LAB_00112135;
    }
LAB_0011214e:
    uVar5 = uVar19 | (lVar20 - *(long *)(puVar27 + 8) >> 4) * 0x1c71c71c71c71c80;
    puVar7 = operator_new(0x28);
    FUN_00111450(puVar7,puVar27);
    if (*puVar27 != 0xffffffff) {
      LOCK();
      *puVar27 = *puVar27 - 1;
      UNLOCK();
      if (*puVar27 == 0) {
        FUN_00111c00(puVar27);
        operator_delete(puVar27,0x28);
      }
    }
    uVar19 = (ulong)((uint)uVar5 & 0x7f);
    lVar20 = (uVar5 >> 7) * 0x90 + *(long *)(puVar7 + 8);
    DAT_001292b0 = puVar7;
    if (bVar24 != 0xff) goto LAB_00111f20;
  }
LAB_00111ec6:
  bVar24 = *(byte *)(lVar20 + 0x89);
  if (bVar24 == *(byte *)(lVar20 + 0x88)) {
    if (bVar24 == 0) {
      cVar25 = '0';
      pvVar10 = operator_new__(0x600);
      uVar18 = 0;
      uVar5 = 0x30;
    }
    else {
      uVar18 = (ulong)bVar24;
      if (bVar24 == 0x30) {
        cVar25 = 'P';
        pvVar10 = operator_new__(0xa00);
        uVar12 = 0xa00;
        uVar5 = 0x50;
      }
      else {
        uVar5 = uVar18 + 0x10;
        cVar25 = bVar24 + 0x10;
        uVar12 = uVar5 * 0x20;
        pvVar10 = operator_new__(uVar12);
      }
      pvVar10 = (void *)__memcpy_chk(pvVar10,*(undefined8 *)(lVar20 + 0x80),uVar18 << 5,uVar12);
    }
    do {
      uVar12 = uVar18 + 1;
      *(char *)((long)pvVar10 + uVar18 * 0x20) = (char)uVar18 + '\x01';
      uVar18 = uVar12;
    } while (uVar12 < uVar5);
    if (*(void **)(lVar20 + 0x80) != (void *)0x0) {
      operator_delete__(*(void **)(lVar20 + 0x80));
    }
    *(char *)(lVar20 + 0x88) = cVar25;
    bVar24 = *(byte *)(lVar20 + 0x89);
    *(void **)(lVar20 + 0x80) = pvVar10;
  }
  else {
    pvVar10 = *(void **)(lVar20 + 0x80);
  }
  lVar6 = (ulong)bVar24 * 0x20;
  *(undefined1 *)(lVar20 + 0x89) = *(undefined1 *)((long)pvVar10 + lVar6);
  *(byte *)(lVar20 + uVar19) = bVar24;
  *(undefined1 (*) [16])((long)pvVar10 + lVar6) = (undefined1  [16])0x0;
  *(undefined1 (*) [16])((long)pvVar10 + lVar6 + 0x10) = (undefined1  [16])0x0;
  *(uint *)((long)pvVar10 + lVar6) = *param_2;
  puVar7 = DAT_001292b0;
  *(long *)(DAT_001292b0 + 2) = *(long *)(DAT_001292b0 + 2) + 1;
LAB_00111f20:
  lVar6 = *(long *)(puVar7 + 8);
  *param_1 = puVar7;
  *(undefined1 *)(param_1 + 2) = uVar14;
  param_1[1] = (lVar20 - lVar6 >> 4) * 0x1c71c71c71c71c80 | uVar19;
  if ((local_70 != (uint *)0x0) && (*local_70 != 0xffffffff)) {
    LOCK();
    *local_70 = *local_70 - 1;
    UNLOCK();
    if (*local_70 == 0) {
      pbVar21 = *(byte **)(local_70 + 8);
      if (pbVar21 != (byte *)0x0) {
        pbVar13 = pbVar21 + *(long *)(pbVar21 + -8) * 0x90;
        if (pbVar21 == pbVar13) {
          lVar6 = 0;
        }
        else {
          pbVar21 = pbVar13 + -0x10;
          do {
            local_40 = pbVar13;
            pvVar10 = *(void **)pbVar21;
            pbVar13 = local_40 + -0x90;
            if (pvVar10 != (void *)0x0) {
              pbVar26 = pbVar13;
              if (pbVar13 != pbVar21) {
                do {
                  bVar24 = *pbVar26;
                  if ((bVar24 != 0xff) &&
                     (piVar2 = *(int **)((long)pvVar10 + (ulong)bVar24 * 0x20 + 8),
                     piVar2 != (int *)0x0)) {
                    LOCK();
                    *piVar2 = *piVar2 + -1;
                    UNLOCK();
                    if (*piVar2 == 0) {
                      puVar15 = *(undefined8 **)((long)pvVar10 + (ulong)bVar24 * 0x20 + 0x10);
                      puVar9 = puVar15 + *(long *)((long)pvVar10 + (ulong)bVar24 * 0x20 + 0x18) * 3;
                      for (; puVar15 != puVar9; puVar15 = puVar15 + 3) {
                        piVar2 = (int *)*puVar15;
                        if (piVar2 != (int *)0x0) {
                          LOCK();
                          *piVar2 = *piVar2 + -1;
                          UNLOCK();
                          if (*piVar2 == 0) {
                            QArrayData::deallocate((QArrayData *)*puVar15,2,0x10);
                          }
                        }
                      }
                      QArrayData::deallocate
                                (*(QArrayData **)((long)pvVar10 + (ulong)bVar24 * 0x20 + 8),0x18,
                                 0x10);
                    }
                    pvVar10 = *(void **)pbVar21;
                  }
                  pbVar26 = pbVar26 + 1;
                } while (pbVar21 != pbVar26);
                if (pvVar10 == (void *)0x0) goto LAB_00112066;
              }
              operator_delete__(pvVar10);
            }
LAB_00112066:
            pbVar21 = pbVar21 + -0x90;
          } while (*(byte **)(local_70 + 8) != pbVar13);
          lVar6 = *(long *)(local_40 + -0x98) * 0x90;
        }
        operator_delete__(pbVar13 + -8,lVar6 + 8);
      }
      operator_delete(local_70,0x28);
    }
  }
  return param_1;
}

