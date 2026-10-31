// rea decompile_all
// name: FUN_0011c770
// address: 0x11c770
// body: available
// range: 0x11c770-0x11c808 inclusive
// range: 0x11c810-0x11c91a inclusive
// range: 0x11c920-0x11c935 inclusive
// range: 0x11c940-0x11cb83 inclusive
// range: 0x11cb88-0x11cbfd inclusive
// range: 0x11cc00-0x11cc08 inclusive
// range: 0x11cc10-0x11cc1d inclusive
// range: 0x11cc20-0x11cc5e inclusive
// range: 0x11cc60-0x11cd3e inclusive
// range: 0x11cd40-0x11cd48 inclusive
// range: 0x11cd50-0x11cd5b inclusive
// range: 0x11cd60-0x11cd68 inclusive
// range: 0x11cd70-0x11cd93 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11cb7f to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11cd8f to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011c770(undefined1 (*param_1) [16],int param_2,ulong param_3)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 *puVar6;
  long lVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  int *piVar10;
  long lVar11;
  QArrayData *pQVar12;
  long in_FS_OFFSET;
  undefined1 auVar13 [16];
  long local_38;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_2 == 0) {
    piVar10 = *(int **)*param_1;
    if (piVar10 == (int *)0x0) {
LAB_0011c920:
      lVar5 = *(long *)param_1[1];
LAB_0011c924:
      lVar7 = 0;
      if (lVar5 < 0) {
        lVar5 = lVar7;
      }
      lVar5 = lVar5 + param_3;
      goto LAB_0011c863;
    }
    if ((1 < *piVar10) || ((param_3 & 1) == 0)) {
      lVar5 = *(long *)param_1[1];
      if (piVar10 != (int *)0x0) {
        lVar7 = *(long *)(piVar10 + 2);
        lVar11 = lVar5;
        if (lVar5 <= lVar7) {
          lVar11 = lVar7;
        }
        lVar5 = (lVar11 + param_3) -
                ((lVar7 + ((long)(*(long *)(*param_1 + 8) -
                                 ((long)piVar10 + 0x1fU & 0xfffffffffffffff0)) >> 3) *
                          0x1111111111111111) - lVar5);
        goto LAB_0011c852;
      }
      goto LAB_0011c924;
    }
    auVar13 = QArrayData::reallocateUnaligned
                        (piVar10,*(long *)(*param_1 + 8),0x78,
                         ((long)(*(long *)(*param_1 + 8) -
                                ((long)piVar10 + 0x1fU & 0xfffffffffffffff0)) >> 3) *
                         -0x1111111111111111 + *(long *)param_1[1] + 1,0);
    *param_1 = auVar13;
  }
  else {
    piVar10 = *(int **)*param_1;
    if (piVar10 == (int *)0x0) goto LAB_0011c920;
    lVar7 = *(long *)(piVar10 + 2);
    lVar5 = *(long *)param_1[1];
    if (*(long *)param_1[1] <= lVar7) {
      lVar5 = lVar7;
    }
    lVar5 = lVar5 + param_3 +
            ((long)(*(long *)(*param_1 + 8) - ((long)piVar10 + 0x1fU & 0xfffffffffffffff0)) >> 3) *
            0x1111111111111111;
LAB_0011c852:
    if (((*(byte *)(piVar10 + 1) & 1) != 0) && (lVar5 < *(long *)(piVar10 + 2))) {
      lVar5 = *(long *)(piVar10 + 2);
    }
LAB_0011c863:
    puVar2 = (undefined8 *)QArrayData::allocate(&local_38,0x78,0x10,lVar5,lVar5 <= lVar7);
    lVar7 = *(long *)param_1[1];
    if ((local_38 == 0) || (puVar2 == (undefined8 *)0x0)) {
      puVar9 = *(undefined8 **)(*param_1 + 8);
    }
    else {
      lVar5 = *(long *)*param_1;
      if (param_2 == 1) {
        lVar11 = (long)((*(long *)(local_38 + 8) - lVar7) - param_3) / 2;
        if (lVar11 < 0) {
          lVar11 = 0;
        }
        puVar2 = puVar2 + (lVar11 + param_3) * 0xf;
        if (lVar5 != 0) {
          puVar9 = *(undefined8 **)(*param_1 + 8);
          goto LAB_0011c8c8;
        }
        puVar9 = *(undefined8 **)(*param_1 + 8);
        param_2 = 0;
      }
      else if (lVar5 == 0) {
        puVar9 = *(undefined8 **)(*param_1 + 8);
      }
      else {
        puVar9 = *(undefined8 **)(*param_1 + 8);
        puVar2 = (undefined8 *)
                 ((long)puVar2 + ((long)puVar9 - (lVar5 + 0x1fU & 0xfffffffffffffff0)));
LAB_0011c8c8:
        param_2 = *(int *)(lVar5 + 4);
      }
      *(int *)(local_38 + 4) = param_2;
    }
    if (lVar7 == 0) {
      pQVar12 = *(QArrayData **)*param_1;
      lVar7 = 0;
      lVar5 = 0;
    }
    else {
      pQVar12 = *(QArrayData **)*param_1;
      if (pQVar12 == (QArrayData *)0x0) {
        if (puVar9 == puVar9 + lVar7 * 0xf) {
          *(undefined8 *)param_1[1] = 0;
          *(long *)*param_1 = local_38;
          *(undefined8 **)(*param_1 + 8) = puVar2;
          goto LAB_0011cbe0;
        }
LAB_0011cc70:
        puVar3 = puVar9 + lVar7 * 0xf;
        lVar5 = 0;
        puVar8 = puVar2;
        if (puVar9 < puVar3) {
          do {
            piVar10 = (int *)*puVar9;
            uVar1 = puVar9[1];
            puVar8[2] = puVar9[2];
            *puVar8 = piVar10;
            puVar8[1] = uVar1;
            if (piVar10 != (int *)0x0) {
              LOCK();
              *piVar10 = *piVar10 + 1;
              UNLOCK();
            }
            piVar10 = (int *)puVar9[3];
            uVar1 = puVar9[4];
            puVar8[5] = puVar9[5];
            puVar8[3] = piVar10;
            puVar8[4] = uVar1;
            if (piVar10 != (int *)0x0) {
              LOCK();
              *piVar10 = *piVar10 + 1;
              UNLOCK();
            }
            piVar10 = (int *)puVar9[6];
            uVar1 = puVar9[7];
            puVar8[8] = puVar9[8];
            puVar8[6] = piVar10;
            puVar8[7] = uVar1;
            if (piVar10 != (int *)0x0) {
              LOCK();
              *piVar10 = *piVar10 + 1;
              UNLOCK();
            }
            piVar10 = (int *)puVar9[9];
            uVar1 = puVar9[10];
            puVar8[0xb] = puVar9[0xb];
            puVar8[9] = piVar10;
            puVar8[10] = uVar1;
            if (piVar10 != (int *)0x0) {
              LOCK();
              *piVar10 = *piVar10 + 1;
              UNLOCK();
            }
            piVar10 = (int *)puVar9[0xc];
            uVar1 = puVar9[0xd];
            puVar8[0xe] = puVar9[0xe];
            puVar8[0xc] = piVar10;
            puVar8[0xd] = uVar1;
            if (piVar10 != (int *)0x0) {
              LOCK();
              *piVar10 = *piVar10 + 1;
              UNLOCK();
            }
            puVar9 = puVar9 + 0xf;
            lVar5 = lVar5 + 1;
            puVar8 = puVar8 + 0xf;
          } while (puVar9 < puVar3);
          lVar7 = *(long *)param_1[1];
          pQVar12 = *(QArrayData **)*param_1;
          puVar9 = *(undefined8 **)(*param_1 + 8);
        }
        else {
          lVar7 = *(long *)param_1[1];
          puVar9 = *(undefined8 **)(*param_1 + 8);
        }
      }
      else {
        puVar8 = puVar9 + lVar7 * 0xf;
        if (*(int *)pQVar12 < 2) {
          if (lVar7 * 0x78 == 0) {
            lVar7 = *(long *)param_1[1];
            lVar5 = 0;
            puVar9 = puVar8;
          }
          else {
            puVar3 = puVar9;
            lVar7 = 1;
            puVar6 = puVar2;
            if (puVar8 <= puVar9) goto LAB_0011c910;
            do {
              lVar5 = lVar7;
              uVar1 = *puVar3;
              *puVar3 = 0;
              puVar4 = puVar3 + 0xf;
              *puVar6 = uVar1;
              uVar1 = puVar3[1];
              puVar3[1] = 0;
              puVar6[1] = uVar1;
              uVar1 = puVar3[2];
              puVar3[2] = 0;
              puVar6[2] = uVar1;
              uVar1 = puVar3[3];
              puVar3[3] = 0;
              puVar6[3] = uVar1;
              uVar1 = puVar3[4];
              puVar3[4] = 0;
              puVar6[4] = uVar1;
              uVar1 = puVar3[5];
              puVar3[5] = 0;
              puVar6[5] = uVar1;
              uVar1 = puVar3[6];
              puVar3[6] = 0;
              puVar6[6] = uVar1;
              uVar1 = puVar3[7];
              puVar3[7] = 0;
              puVar6[7] = uVar1;
              uVar1 = puVar3[8];
              puVar3[8] = 0;
              puVar6[8] = uVar1;
              uVar1 = puVar3[9];
              puVar3[9] = 0;
              puVar6[9] = uVar1;
              uVar1 = puVar3[10];
              puVar3[10] = 0;
              puVar6[10] = uVar1;
              uVar1 = puVar3[0xb];
              puVar3[0xb] = 0;
              puVar6[0xb] = uVar1;
              uVar1 = puVar3[0xc];
              puVar3[0xc] = 0;
              puVar6[0xc] = uVar1;
              uVar1 = puVar3[0xd];
              puVar3[0xd] = 0;
              puVar6[0xd] = uVar1;
              uVar1 = puVar3[0xe];
              puVar3[0xe] = 0;
              puVar6[0xe] = uVar1;
              puVar3 = puVar4;
              lVar7 = lVar5 + 1;
              puVar6 = puVar6 + 0xf;
            } while (puVar4 < puVar8);
            lVar7 = *(long *)param_1[1];
          }
        }
        else {
          if (puVar9 != puVar8) goto LAB_0011cc70;
LAB_0011c910:
          lVar7 = *(long *)param_1[1];
          lVar5 = 0;
        }
      }
    }
    *(long *)param_1[1] = lVar5;
    *(long *)*param_1 = local_38;
    *(undefined8 **)(*param_1 + 8) = puVar2;
    if (pQVar12 != (QArrayData *)0x0) {
      LOCK();
      *(int *)pQVar12 = *(int *)pQVar12 + -1;
      UNLOCK();
      if (*(int *)pQVar12 == 0) {
        puVar2 = puVar9 + lVar7 * 0xf;
        for (; puVar9 != puVar2; puVar9 = puVar9 + 0xf) {
          piVar10 = (int *)puVar9[0xc];
          if (piVar10 != (int *)0x0) {
            LOCK();
            *piVar10 = *piVar10 + -1;
            UNLOCK();
            if (*piVar10 == 0) {
              QArrayData::deallocate((QArrayData *)puVar9[0xc],2,0x10);
            }
          }
          piVar10 = (int *)puVar9[9];
          if (piVar10 != (int *)0x0) {
            LOCK();
            *piVar10 = *piVar10 + -1;
            UNLOCK();
            if (*piVar10 == 0) {
              QArrayData::deallocate((QArrayData *)puVar9[9],2,0x10);
            }
          }
          piVar10 = (int *)puVar9[6];
          if (piVar10 != (int *)0x0) {
            LOCK();
            *piVar10 = *piVar10 + -1;
            UNLOCK();
            if (*piVar10 == 0) {
              QArrayData::deallocate((QArrayData *)puVar9[6],2,0x10);
            }
          }
          piVar10 = (int *)puVar9[3];
          if (piVar10 != (int *)0x0) {
            LOCK();
            *piVar10 = *piVar10 + -1;
            UNLOCK();
            if (*piVar10 == 0) {
              QArrayData::deallocate((QArrayData *)puVar9[3],2,0x10);
            }
          }
          piVar10 = (int *)*puVar9;
          if (piVar10 != (int *)0x0) {
            LOCK();
            *piVar10 = *piVar10 + -1;
            UNLOCK();
            if (*piVar10 == 0) {
              QArrayData::deallocate((QArrayData *)*puVar9,2,0x10);
            }
          }
        }
        if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
          QArrayData::deallocate(pQVar12,0x78,0x10);
          return;
        }
        goto LAB_0011cd8f;
      }
    }
  }
LAB_0011cbe0:
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011cd8f:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

