// rea decompile_all
// name: FUN_00113710
// address: 0x113710
// body: available
// range: 0x113710-0x1137a9 inclusive
// range: 0x1137b0-0x1138ba inclusive
// range: 0x1138c0-0x1138d5 inclusive
// range: 0x1138e0-0x1139c9 inclusive
// range: 0x1139d0-0x113a3d inclusive
// range: 0x113a40-0x113a48 inclusive
// range: 0x113a50-0x113a5c inclusive
// range: 0x113a60-0x113a9c inclusive
// range: 0x113aa0-0x113b00 inclusive
// range: 0x113b08-0x113b10 inclusive
// range: 0x113b18-0x113b25 inclusive
// range: 0x113b30-0x113b38 inclusive
// range: 0x113b40-0x113b5d inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1139c5 to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x113b59 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00113710(undefined1 (*param_1) [16],int param_2,ulong param_3)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  undefined8 *puVar7;
  long lVar8;
  undefined8 *puVar9;
  int *piVar10;
  undefined8 *puVar11;
  QArrayData *pQVar12;
  long in_FS_OFFSET;
  undefined1 auVar13 [16];
  long local_38;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if (param_2 == 0) {
    piVar10 = *(int **)*param_1;
    if (piVar10 == (int *)0x0) {
LAB_001138c0:
      lVar6 = *(long *)param_1[1];
LAB_001138c4:
      lVar5 = 0;
      if (lVar6 < 0) {
        lVar6 = lVar5;
      }
      lVar6 = lVar6 + param_3;
      goto LAB_00113803;
    }
    if ((1 < *piVar10) || ((param_3 & 1) == 0)) {
      lVar6 = *(long *)param_1[1];
      if (piVar10 != (int *)0x0) {
        lVar5 = *(long *)(piVar10 + 2);
        lVar8 = lVar5;
        if (lVar5 <= lVar6) {
          lVar8 = lVar6;
        }
        lVar6 = (lVar8 + param_3) -
                ((lVar5 + ((long)(*(long *)(*param_1 + 8) -
                                 ((long)piVar10 + 0x1fU & 0xfffffffffffffff0)) >> 3) *
                          0x5555555555555555) - lVar6);
        goto LAB_001137f2;
      }
      goto LAB_001138c4;
    }
    auVar13 = QArrayData::reallocateUnaligned
                        (piVar10,*(long *)(*param_1 + 8),0x18,
                         ((long)(*(long *)(*param_1 + 8) -
                                ((long)piVar10 + 0x1fU & 0xfffffffffffffff0)) >> 3) *
                         -0x5555555555555555 + *(long *)param_1[1] + 1,0);
    *param_1 = auVar13;
  }
  else {
    piVar10 = *(int **)*param_1;
    if (piVar10 == (int *)0x0) goto LAB_001138c0;
    lVar5 = *(long *)(piVar10 + 2);
    lVar6 = *(long *)param_1[1];
    if (*(long *)param_1[1] <= lVar5) {
      lVar6 = lVar5;
    }
    lVar6 = lVar6 + param_3 +
            ((long)(*(long *)(*param_1 + 8) - ((long)piVar10 + 0x1fU & 0xfffffffffffffff0)) >> 3) *
            0x5555555555555555;
LAB_001137f2:
    if (((*(byte *)(piVar10 + 1) & 1) != 0) && (lVar6 < *(long *)(piVar10 + 2))) {
      lVar6 = *(long *)(piVar10 + 2);
    }
LAB_00113803:
    puVar2 = (undefined8 *)QArrayData::allocate(&local_38,0x18,0x10,lVar6,lVar6 <= lVar5);
    lVar5 = *(long *)param_1[1];
    if ((local_38 == 0) || (puVar2 == (undefined8 *)0x0)) {
      puVar7 = *(undefined8 **)(*param_1 + 8);
    }
    else {
      lVar6 = *(long *)*param_1;
      if (param_2 == 1) {
        lVar8 = (long)((*(long *)(local_38 + 8) - lVar5) - param_3) / 2;
        if (lVar8 < 0) {
          lVar8 = 0;
        }
        puVar2 = puVar2 + (lVar8 + param_3) * 3;
        if (lVar6 != 0) {
          puVar7 = *(undefined8 **)(*param_1 + 8);
          goto LAB_00113868;
        }
        puVar7 = *(undefined8 **)(*param_1 + 8);
        param_2 = 0;
      }
      else if (lVar6 == 0) {
        puVar7 = *(undefined8 **)(*param_1 + 8);
      }
      else {
        puVar7 = *(undefined8 **)(*param_1 + 8);
        puVar2 = (undefined8 *)
                 ((long)puVar2 + ((long)puVar7 - (lVar6 + 0x1fU & 0xfffffffffffffff0)));
LAB_00113868:
        param_2 = *(int *)(lVar6 + 4);
      }
      *(int *)(local_38 + 4) = param_2;
    }
    if (lVar5 == 0) {
      pQVar12 = *(QArrayData **)*param_1;
      lVar5 = 0;
      lVar6 = 0;
    }
    else {
      pQVar12 = *(QArrayData **)*param_1;
      if (pQVar12 == (QArrayData *)0x0) {
        puVar11 = puVar7 + lVar5 * 3;
        if (puVar11 == puVar7) {
          *(undefined8 *)param_1[1] = 0;
          *(long *)*param_1 = local_38;
          *(undefined8 **)(*param_1 + 8) = puVar2;
          goto LAB_00113a20;
        }
LAB_00113ab0:
        lVar6 = 0;
        puVar3 = puVar2;
        if (puVar7 < puVar11) {
          do {
            piVar10 = (int *)*puVar7;
            uVar1 = puVar7[1];
            puVar3[2] = puVar7[2];
            *puVar3 = piVar10;
            puVar3[1] = uVar1;
            if (piVar10 != (int *)0x0) {
              LOCK();
              *piVar10 = *piVar10 + 1;
              UNLOCK();
            }
            puVar7 = puVar7 + 3;
            lVar6 = lVar6 + 1;
            puVar3 = puVar3 + 3;
          } while (puVar7 < puVar11);
          lVar5 = *(long *)param_1[1];
          pQVar12 = *(QArrayData **)*param_1;
          puVar7 = *(undefined8 **)(*param_1 + 8);
        }
        else {
          puVar7 = *(undefined8 **)(*param_1 + 8);
LAB_00113942:
          lVar5 = *(long *)param_1[1];
        }
      }
      else {
        puVar11 = puVar7 + lVar5 * 3;
        if (*(int *)pQVar12 < 2) {
          if ((lVar5 * 3 & 0x1fffffffffffffffU) != 0) {
            puVar3 = puVar7;
            lVar5 = 1;
            puVar9 = puVar2;
            if (puVar11 <= puVar7) goto LAB_001138b0;
            do {
              lVar6 = lVar5;
              uVar1 = *puVar3;
              *puVar3 = 0;
              puVar4 = puVar3 + 3;
              *puVar9 = uVar1;
              uVar1 = puVar3[1];
              puVar3[1] = 0;
              puVar9[1] = uVar1;
              uVar1 = puVar3[2];
              puVar3[2] = 0;
              puVar9[2] = uVar1;
              puVar3 = puVar4;
              lVar5 = lVar6 + 1;
              puVar9 = puVar9 + 3;
            } while (puVar4 < puVar11);
            goto LAB_00113942;
          }
          lVar5 = *(long *)param_1[1];
          lVar6 = 0;
          puVar7 = puVar11;
        }
        else {
          if (puVar7 != puVar11) goto LAB_00113ab0;
LAB_001138b0:
          lVar5 = *(long *)param_1[1];
          lVar6 = 0;
        }
      }
    }
    *(long *)param_1[1] = lVar6;
    *(long *)*param_1 = local_38;
    *(undefined8 **)(*param_1 + 8) = puVar2;
    if (pQVar12 != (QArrayData *)0x0) {
      LOCK();
      *(int *)pQVar12 = *(int *)pQVar12 + -1;
      UNLOCK();
      if (*(int *)pQVar12 == 0) {
        puVar2 = puVar7 + lVar5 * 3;
        for (; puVar7 != puVar2; puVar7 = puVar7 + 3) {
          piVar10 = (int *)*puVar7;
          if (piVar10 != (int *)0x0) {
            LOCK();
            *piVar10 = *piVar10 + -1;
            UNLOCK();
            if (*piVar10 == 0) {
              QArrayData::deallocate((QArrayData *)*puVar7,2,0x10);
            }
          }
        }
        if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
          QArrayData::deallocate(pQVar12,0x18,0x10);
          return;
        }
        goto LAB_00113b59;
      }
    }
  }
LAB_00113a20:
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_00113b59:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

