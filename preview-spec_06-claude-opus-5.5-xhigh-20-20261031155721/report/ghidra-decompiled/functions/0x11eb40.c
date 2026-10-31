// rea decompile_all
// name: FUN_0011eb40
// address: 0x11eb40
// body: available
// range: 0x11eb40-0x11ed35 inclusive
// range: 0x11ed40-0x11ed74 inclusive
// range: 0x11ed78-0x11ed9d inclusive
// range: 0x11eda0-0x11ee48 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11ed31 to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11ee44 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011eb40(long param_1)

{
  int iVar1;
  int iVar2;
  long lVar3;
  ulong uVar4;
  long lVar5;
  int iVar6;
  long *plVar7;
  int iVar8;
  long in_FS_OFFSET;
  double dVar9;
  double extraout_XMM1_Qa;
  double dVar10;
  undefined8 uVar11;
  QArrayData *local_68;
  long *local_60;
  long local_58;
  QArrayData *local_48;
  long *local_40;
  long local_38;
  long local_30;
  int iVar12;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  QGraphicsSceneMouseEvent::pos();
  dVar10 = extraout_XMM1_Qa / (double)*(int *)(param_1 + 0x50);
  dVar9 = (double)QGraphicsSceneMouseEvent::pos();
  iVar8 = (int)dVar10 + -1;
  if (-1 < iVar8) {
    dVar9 = dVar9 / (double)*(int *)(param_1 + 0x50);
    iVar12 = (int)dVar9 + -1;
    uVar11 = CONCAT44(iVar12,(int)dVar10 + -1);
    if (((iVar8 < *(int *)(param_1 + 0x54)) && (-1 < iVar12)) && (iVar12 < *(int *)(param_1 + 0x58))
       ) {
      iVar6 = (int)dVar9 + -1;
      uVar4 = QGraphicsSceneMouseEvent::buttons();
      if (((uVar4 & 4) == 0) && (*(char *)(param_1 + 0x8a) == '\0')) {
        uVar4 = QGraphicsSceneMouseEvent::buttons();
        if (((((uVar4 & 1) != 0) && (iVar1 = *(int *)(param_1 + 0x78), iVar1 != -1)) &&
            (iVar2 = *(int *)(param_1 + 0x7c), iVar2 != -1)) &&
           ((iVar8 != iVar1 || (iVar6 != iVar2)))) {
          iVar6 = *(int *)(param_1 + 0x58);
          lVar5 = *(long *)(param_1 + 0x28);
          lVar3 = *(long *)(lVar5 + (long)(iVar1 * iVar6 + iVar2) * 8);
          if (*(int *)(lVar3 + 0x28) == 1) {
            *(undefined4 *)(lVar3 + 0x28) = 0;
            FUN_00117930();
            iVar6 = *(int *)(param_1 + 0x58);
            lVar5 = *(long *)(param_1 + 0x28);
          }
          lVar5 = *(long *)(lVar5 + (long)(iVar8 * iVar6 + iVar12) * 8);
          if (*(int *)(lVar5 + 0x28) == 0) {
            *(undefined4 *)(lVar5 + 0x28) = 1;
            FUN_00117930();
          }
          *(undefined8 *)(param_1 + 0x78) = uVar11;
        }
      }
      else if (((*(int *)(param_1 + 0x80) != -1) && (*(int *)(param_1 + 0x84) != -1)) &&
              ((iVar8 != *(int *)(param_1 + 0x80) || (iVar6 != *(int *)(param_1 + 0x84))))) {
        FUN_00119c40(&local_48,param_1);
        for (plVar7 = local_40; plVar7 != local_40 + local_38; plVar7 = plVar7 + 1) {
          if (*(int *)(*plVar7 + 0x28) == 1) {
            *(undefined4 *)(*plVar7 + 0x28) = 0;
            FUN_00117930();
          }
        }
        FUN_00119c40(&local_68,param_1,iVar8,iVar12);
        for (plVar7 = local_60; plVar7 != local_60 + local_58; plVar7 = plVar7 + 1) {
          if (*(int *)(*plVar7 + 0x28) == 0) {
            *(undefined4 *)(*plVar7 + 0x28) = 1;
            FUN_00117930();
          }
        }
        *(undefined8 *)(param_1 + 0x80) = uVar11;
        if (local_68 != (QArrayData *)0x0) {
          LOCK();
          *(int *)local_68 = *(int *)local_68 + -1;
          UNLOCK();
          if (*(int *)local_68 == 0) {
            QArrayData::deallocate(local_68,8,0x10);
          }
        }
        if (local_48 != (QArrayData *)0x0) {
          LOCK();
          *(int *)local_48 = *(int *)local_48 + -1;
          UNLOCK();
          if (*(int *)local_48 == 0) {
            if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
              QArrayData::deallocate(local_48,8,0x10);
              return;
            }
            goto LAB_0011ee44;
          }
        }
      }
    }
  }
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011ee44:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

