// rea decompile_all
// name: FUN_0011ee50
// address: 0x11ee50
// body: available
// range: 0x11ee50-0x11eea2 inclusive
// range: 0x11eea8-0x11ef89 inclusive
// range: 0x11ef90-0x11efa9 inclusive
// range: 0x11efb0-0x11f034 inclusive
// range: 0x11f038-0x11f090 inclusive
// range: 0x11f098-0x11f10a inclusive
// range: 0x11f110-0x11f31a inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11f1e2 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 FUN_0011ee50(QObject *param_1,long param_2)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  long lVar6;
  long *plVar7;
  long *plVar8;
  int iVar9;
  long lVar10;
  long lVar11;
  long in_FS_OFFSET;
  undefined8 uVar12;
  uint uVar13;
  undefined8 extraout_XMM1_Qa;
  uint uVar16;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  QArrayData *local_48;
  long *local_40;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if ((param_1[0x89] != (QObject)0x0) || (*(long *)(param_2 + 0x38) != 1)) goto LAB_0011ee80;
  QPointerEvent::point(param_2);
  QEventPoint::position();
  iVar9 = *(int *)(param_1 + 0x50);
  QPointerEvent::point(param_2);
  uVar12 = QEventPoint::position();
  auVar14._8_8_ = uVar12;
  auVar14._0_8_ = extraout_XMM1_Qa;
  auVar15._0_8_ = (double)iVar9;
  auVar15._8_8_ = (double)*(int *)(param_1 + 0x50);
  auVar15 = divpd(auVar14,auVar15);
  uVar13 = (int)auVar15._0_8_ - 1;
  uVar16 = (int)auVar15._8_8_ - 1;
  lVar6 = std::chrono::_V2::system_clock::now();
  uVar2 = *(uint *)(param_1 + 0x68);
  if ((int)uVar2 < 0) {
    if (((-1 < (int)uVar13) && ((int)uVar13 < *(int *)(param_1 + 0x54))) && (-1 < (int)uVar16)) {
      iVar9 = *(int *)(param_1 + 0x58);
joined_r0x0011efa6:
      if ((int)uVar16 < iVar9) {
        lVar10 = *(long *)(param_1 + 0x28);
        lVar11 = 0;
LAB_0011efcc:
        lVar10 = *(long *)(lVar10 + (long)(int)(iVar9 * uVar13 + uVar16) * 8);
        sVar1 = *(short *)(param_2 + 8);
        if (sVar1 == 0xc2) {
          *(ulong *)(param_1 + 100) = CONCAT44(uVar16,uVar13);
          iVar9 = *(int *)(lVar10 + 0x28);
          *(long *)(param_1 + 0x70) = lVar6 / 1000000;
          if (iVar9 == 0) {
            *(undefined4 *)(lVar10 + 0x28) = 1;
            FUN_00117930(lVar10);
          }
          goto LAB_0011ee80;
        }
        if (sVar1 == 0xc4) {
          if ((uVar2 == uVar16) && (*(uint *)(param_1 + 100) == uVar13)) {
            if (param_1[0x88] == (QObject)0x0) {
              iVar9 = *(int *)(lVar10 + 0x28);
              if (lVar6 / 1000000 - *(long *)(param_1 + 0x70) < 1000) {
                if (iVar9 == 5 || iVar9 == 2) {
                  FUN_00119c40(&local_48,param_1,uVar13,uVar2);
                  FUN_0011d6c0(&local_48);
                  plVar8 = local_40;
                  plVar7 = (long *)FUN_0011d6f0(&local_48);
                  if (plVar7 != plVar8) {
                    iVar9 = 0;
                    do {
                      lVar6 = *plVar8;
                      plVar8 = plVar8 + 1;
                      iVar9 = iVar9 + (uint)(*(int *)(lVar6 + 0x28) == 4);
                    } while (plVar7 != plVar8);
                    if ((iVar9 != 0) && (iVar9 == *(int *)(lVar10 + 0x30))) {
                      FUN_0011d6c0(&local_48);
                      plVar7 = (long *)FUN_0011d6f0(&local_48);
                      for (plVar8 = local_40; plVar8 != plVar7; plVar8 = plVar8 + 1) {
                        lVar6 = *plVar8;
                        if ((*(int *)(lVar6 + 0x28) != 2) && (*(int *)(lVar6 + 0x28) != 5)) {
                          FUN_00118790(lVar6,1);
                          if (((*(int *)(lVar6 + 0x28) == 2) || (*(int *)(lVar6 + 0x28) == 5)) &&
                             (cVar5 = FUN_00119b00(param_1,lVar6), cVar5 != '\0')) break;
                        }
                      }
                    }
                  }
                  if (local_48 != (QArrayData *)0x0) {
                    LOCK();
                    *(int *)local_48 = *(int *)local_48 + -1;
                    UNLOCK();
                    if (*(int *)local_48 == 0) {
                      QArrayData::deallocate(local_48,8,0x10);
                    }
                  }
                }
                else {
                  FUN_001187f0(lVar10);
                  FUN_00119a50(param_1,lVar10);
                }
              }
              else if (iVar9 == 5 || iVar9 == 2) {
                FUN_001187f0(lVar10);
              }
              else {
                FUN_00118790(lVar10,0);
                if ((*(int *)(lVar10 + 0x28) == 2) || (*(int *)(lVar10 + 0x28) == 5)) {
                  FUN_00119b00(param_1,lVar10);
                }
              }
            }
            else {
              param_1[0x88] = (QObject)0x0;
              FUN_0011a0b0(param_1);
              QMetaObject::activate
                        (param_1,(QMetaObject *)&PTR_staticMetaObject_00127a00,1,(void **)0x0);
              FUN_00118790(*(undefined8 *)
                            (*(long *)(param_1 + 0x28) +
                            (long)(int)(uVar13 * *(int *)(param_1 + 0x58) + uVar16) * 8),1);
              iVar9 = *(int *)(*(long *)(*(long *)(param_1 + 0x28) +
                                        (long)(int)(*(int *)(param_1 + 0x58) * uVar13 + uVar16) * 8)
                              + 0x28);
              if ((iVar9 == 2) || (iVar9 == 5)) {
                FUN_001197f0(param_1,uVar13,uVar16);
              }
            }
            goto LAB_0011ee80;
          }
          goto LAB_0011f060;
        }
        if (sVar1 != 0xd1) goto LAB_0011ee80;
        if (lVar11 != 0) {
          FUN_001187f0(lVar11);
        }
      }
    }
  }
  else {
    iVar9 = *(int *)(param_1 + 0x58);
    if (((int)uVar2 < iVar9) && (iVar3 = *(int *)(param_1 + 100), -1 < iVar3)) {
      iVar4 = *(int *)(param_1 + 0x54);
      if (iVar3 < iVar4) {
        lVar10 = *(long *)(param_1 + 0x28);
        lVar11 = *(long *)(lVar10 + (long)(int)(iVar3 * iVar9 + uVar2) * 8);
        if (((-1 < (int)(uVar16 | uVar13)) && ((int)uVar13 < iVar4)) && ((int)uVar16 < iVar9))
        goto LAB_0011efcc;
LAB_0011f060:
        if ((lVar11 != 0) && (*(int *)(lVar11 + 0x28) == 1)) {
          *(undefined4 *)(lVar11 + 0x28) = 0;
          FUN_00117930(lVar11);
        }
      }
      else if ((-1 < (int)(uVar16 | uVar13)) && ((int)uVar13 < iVar4)) goto joined_r0x0011efa6;
    }
    else if ((-1 < (int)uVar13) && (((int)uVar13 < *(int *)(param_1 + 0x54) && (-1 < (int)uVar16))))
    goto joined_r0x0011efa6;
  }
  *(undefined8 *)(param_1 + 100) = 0xffffffffffffffff;
LAB_0011ee80:
  if (local_30 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 1;
}

