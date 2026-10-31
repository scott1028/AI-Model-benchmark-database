// rea decompile_all
// name: FUN_0011fa60
// address: 0x11fa60
// body: available
// range: 0x11fa60-0x11fb1e inclusive
// range: 0x11fb20-0x11fb4a inclusive
// range: 0x11fb50-0x11fb76 inclusive
// range: 0x11fb80-0x11fc01 inclusive
// range: 0x11fc08-0x11fc8a inclusive
// range: 0x11fca0-0x11fcc3 inclusive
// range: 0x11fcc8-0x11fd9d inclusive
// range: 0x11fda0-0x11fdd0 inclusive
// range: 0x11fdd8-0x11fe52 inclusive
// range: 0x11fe58-0x11ff3e inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11fd99 to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11fe4e to 0x119a50 FUN_00119a50 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11fef6 to 0x1187f0 FUN_001187f0 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11ff3a to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011fa60(QObject *param_1)

{
  int iVar1;
  long lVar2;
  char cVar3;
  int iVar4;
  long lVar5;
  long *plVar6;
  ulong uVar7;
  int iVar8;
  long *plVar9;
  int iVar10;
  long in_FS_OFFSET;
  double dVar11;
  double extraout_XMM1_Qa;
  QArrayData *local_58;
  long *local_50;
  ulong local_48;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  QGraphicsSceneMouseEvent::pos();
  iVar8 = *(int *)(param_1 + 0x50);
  dVar11 = (double)QGraphicsSceneMouseEvent::pos();
  iVar8 = (int)(extraout_XMM1_Qa / (double)iVar8) + -1;
  if ((((iVar8 < 0) ||
       (iVar10 = (int)(dVar11 / (double)*(int *)(param_1 + 0x50)) + -1,
       *(int *)(param_1 + 0x54) <= iVar8)) || (iVar10 < 0)) || (*(int *)(param_1 + 0x58) <= iVar10))
  {
    if (*(int *)(param_1 + 0x80) == -1) {
LAB_0011faf3:
      iVar8 = *(int *)(param_1 + 0x78);
    }
    else {
      FUN_00119c40(&local_58,param_1,*(int *)(param_1 + 0x80),*(undefined4 *)(param_1 + 0x84));
      for (plVar9 = local_50; plVar9 != local_50 + local_48; plVar9 = plVar9 + 1) {
        while (*(int *)(*plVar9 + 0x28) != 1) {
          plVar9 = plVar9 + 1;
          if (local_50 + local_48 == plVar9) goto LAB_0011fbb0;
        }
        *(undefined4 *)(*plVar9 + 0x28) = 0;
        FUN_00117930();
      }
LAB_0011fbb0:
      *(undefined8 *)(param_1 + 0x80) = 0xffffffffffffffff;
      param_1[0x8a] = (QObject)0x0;
      if (local_58 == (QArrayData *)0x0) goto LAB_0011faf3;
      LOCK();
      *(int *)local_58 = *(int *)local_58 + -1;
      UNLOCK();
      if (*(int *)local_58 != 0) goto LAB_0011faf3;
      QArrayData::deallocate(local_58,8,0x10);
      iVar8 = *(int *)(param_1 + 0x78);
    }
    if (iVar8 != -1) {
      lVar2 = *(long *)(*(long *)(param_1 + 0x28) +
                       (long)(iVar8 * *(int *)(param_1 + 0x58) + *(int *)(param_1 + 0x7c)) * 8);
      if (*(int *)(lVar2 + 0x28) == 1) {
        *(undefined4 *)(lVar2 + 0x28) = 0;
        FUN_00117930();
      }
LAB_0011fb40:
      *(undefined8 *)(param_1 + 0x78) = 0xffffffffffffffff;
    }
  }
  else {
    lVar2 = *(long *)(*(long *)(param_1 + 0x28) +
                     (long)(*(int *)(param_1 + 0x58) * iVar8 + iVar10) * 8);
    lVar5 = FUN_0011f4c0();
    iVar1 = *(int *)(lVar5 + 0x1c);
    iVar4 = QGraphicsSceneMouseEvent::button();
    if ((iVar4 == 4) || (param_1[0x8a] != (QObject)0x0)) {
      *(undefined8 *)(param_1 + 0x80) = 0xffffffffffffffff;
      FUN_00119c40(&local_58,param_1,iVar8);
      plVar9 = local_50 + local_48;
      if ((*(int *)(lVar2 + 0x28) == 2) || (plVar6 = local_50, *(int *)(lVar2 + 0x28) == 5)) {
        if ((local_48 & 0x1fffffffffffffff) != 0) {
          iVar10 = 0;
          iVar8 = 0;
          plVar6 = local_50;
          do {
            lVar2 = *plVar6;
            plVar6 = plVar6 + 1;
            iVar8 = iVar8 + (uint)(*(int *)(lVar2 + 0x28) == 4);
            iVar10 = iVar10 + (uint)*(byte *)(lVar2 + 0x2c);
          } while (plVar6 != plVar9);
          plVar6 = local_50;
          if ((iVar10 == iVar8) && (iVar8 != 0)) {
            do {
              lVar2 = *plVar6;
              if ((*(int *)(lVar2 + 0x28) != 2) && (*(int *)(lVar2 + 0x28) != 5)) {
                FUN_00118790(lVar2,1);
                if (((*(int *)(lVar2 + 0x28) == 2) || (*(int *)(lVar2 + 0x28) == 5)) &&
                   (cVar3 = FUN_00119b00(param_1,lVar2), cVar3 != '\0')) break;
              }
              plVar6 = plVar6 + 1;
            } while (plVar9 != plVar6);
          }
          else {
            do {
              if (*(int *)(*plVar6 + 0x28) == 1) {
                *(undefined4 *)(*plVar6 + 0x28) = 0;
                FUN_00117930();
              }
              plVar6 = plVar6 + 1;
            } while (plVar9 != plVar6);
          }
        }
      }
      else {
        for (; plVar6 != plVar9; plVar6 = plVar6 + 1) {
          if (*(int *)(*plVar6 + 0x28) == 1) {
            *(undefined4 *)(*plVar6 + 0x28) = 0;
            FUN_00117930();
          }
        }
      }
      if (local_58 != (QArrayData *)0x0) {
        LOCK();
        *(int *)local_58 = *(int *)local_58 + -1;
        UNLOCK();
        if (*(int *)local_58 == 0) {
          if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
            QArrayData::deallocate(local_58,8,0x10);
            return;
          }
          goto LAB_0011ff3a;
        }
      }
    }
    else {
      iVar4 = QGraphicsSceneMouseEvent::button();
      if ((iVar4 == 1) && (uVar7 = QGraphicsSceneMouseEvent::buttons(), (uVar7 & 2) == 0)) {
        if (*(int *)(param_1 + 0x80) != -1) {
          if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
            FUN_001187f0(lVar2);
            return;
          }
          goto LAB_0011ff3a;
        }
        if (*(int *)(param_1 + 0x78) == -1) goto LAB_0011fafd;
        if ((*(int *)(lVar2 + 0x28) != 2) && (*(int *)(lVar2 + 0x28) != 5)) {
          if (param_1[0x88] != (QObject)0x0) {
            param_1[0x88] = (QObject)0x0;
            FUN_0011a0b0(param_1,*(int *)(param_1 + 0x58) * iVar8 + iVar10);
            QMetaObject::activate
                      (param_1,(QMetaObject *)&PTR_staticMetaObject_00127a00,1,(void **)0x0);
          }
          FUN_00118790(lVar2,0);
          if ((*(int *)(lVar2 + 0x28) == 2) || (*(int *)(lVar2 + 0x28) == 5)) {
            FUN_001197f0(param_1,iVar8,iVar10);
          }
        }
        goto LAB_0011fb40;
      }
      if (((iVar1 == 0) && (iVar8 = QGraphicsSceneMouseEvent::button(), iVar8 == 2)) &&
         (uVar7 = QGraphicsSceneMouseEvent::buttons(), (uVar7 & 1) == 0)) {
        if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
          FUN_00119a50(param_1,lVar2);
          return;
        }
        goto LAB_0011ff3a;
      }
    }
  }
LAB_0011fafd:
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011ff3a:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

