// rea decompile_all
// name: FUN_0011f660
// address: 0x11f660
// body: available
// range: 0x11f660-0x11f6fd inclusive
// range: 0x11f700-0x11f895 inclusive
// range: 0x11f8a0-0x11f8c7 inclusive
// range: 0x11f8d0-0x11f93a inclusive
// range: 0x11f940-0x11f9f5 inclusive
// range: 0x11fa00-0x11fa16 inclusive
// range: 0x11fa20-0x11fa5a inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11f86b to 0x10f780 <EXTERNAL>::QArrayData::deallocate (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11f936 to 0x119a50 FUN_00119a50 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11fa56 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0011f660(long param_1)

{
  long *plVar1;
  char cVar2;
  int iVar3;
  QTextStream *this;
  int iVar4;
  uint uVar5;
  long lVar6;
  ulong uVar7;
  long *plVar8;
  long lVar9;
  bool bVar10;
  int iVar11;
  int iVar12;
  long in_FS_OFFSET;
  double dVar13;
  double extraout_XMM1_Qa;
  QTextStream *local_90;
  QArrayData *local_88 [4];
  QArrayData *local_68;
  undefined1 local_60 [16];
  undefined8 local_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  QGraphicsSceneMouseEvent::pos();
  iVar11 = *(int *)(param_1 + 0x50);
  dVar13 = (double)QGraphicsSceneMouseEvent::pos();
  iVar11 = (int)(extraout_XMM1_Qa / (double)iVar11) + -1;
  if (((-1 < iVar11) &&
      (iVar12 = (int)(dVar13 / (double)*(int *)(param_1 + 0x50)) + -1,
      iVar11 < *(int *)(param_1 + 0x54) && -1 < iVar12)) && (iVar12 < *(int *)(param_1 + 0x58))) {
    lVar9 = *(long *)(*(long *)(param_1 + 0x28) +
                     (long)(*(int *)(param_1 + 0x58) * iVar11 + iVar12) * 8);
    if (lVar9 == 0) {
      lVar9 = FUN_0011f430();
      if (*(char *)(lVar9 + 0x10) != '\0') {
        local_50 = *(undefined8 *)(lVar9 + 8);
        local_60 = (undefined1  [16])0x0;
        local_68 = (QArrayData *)0x2;
        QMessageLogger::debug();
        this = local_90;
        QString::fromUtf8(local_88,0x20,"unexpected - no item under mouse");
        QTextStream::operator<<(this,(QString *)local_88);
        if (local_88[0] != (QArrayData *)0x0) {
          LOCK();
          *(int *)local_88[0] = *(int *)local_88[0] + -1;
          UNLOCK();
          if (*(int *)local_88[0] == 0) {
            QArrayData::deallocate(local_88[0],2,0x10);
          }
        }
        if (local_90[0x30] != (QTextStream)0x0) {
          QTextStream::operator<<(local_90,' ');
        }
        QDebug::~QDebug((QDebug *)&local_90);
      }
    }
    else {
      lVar6 = FUN_0011f4c0();
      cVar2 = *(char *)(lVar6 + 0x1b);
      lVar6 = FUN_0011f4c0();
      iVar3 = *(int *)(lVar6 + 0x1c);
      bVar10 = false;
      if (cVar2 == '\0') {
        uVar7 = QGraphicsSceneMouseEvent::buttons();
        if ((uVar7 & 1) != 0) {
          uVar5 = QGraphicsSceneMouseEvent::buttons();
          bVar10 = (bool)((byte)(uVar5 >> 1) & 1);
        }
      }
      else {
        uVar7 = QGraphicsSceneMouseEvent::buttons();
        if ((uVar7 & 1) != 0) {
          bVar10 = *(int *)(lVar9 + 0x28) == 2 || *(int *)(lVar9 + 0x28) == 5;
        }
      }
      *(bool *)(param_1 + 0x8a) = bVar10;
      iVar4 = QGraphicsSceneMouseEvent::button();
      if ((iVar4 == 4) || (*(char *)(param_1 + 0x8a) != '\0')) {
        if (*(int *)(lVar9 + 0x28) == 1) {
          *(undefined4 *)(lVar9 + 0x28) = 0;
          FUN_00117930(lVar9);
        }
        FUN_00119c40(&local_68,param_1,iVar11,iVar12);
        plVar1 = (long *)(local_60._0_8_ + local_60._8_8_ * 8);
        for (plVar8 = (long *)local_60._0_8_; plVar8 != plVar1; plVar8 = plVar8 + 1) {
          if (*(int *)(*plVar8 + 0x28) == 0) {
            *(undefined4 *)(*plVar8 + 0x28) = 1;
            FUN_00117930();
          }
          *(int *)(param_1 + 0x80) = iVar11;
          *(int *)(param_1 + 0x84) = iVar12;
          *(undefined8 *)(param_1 + 0x78) = 0xffffffffffffffff;
        }
        if (local_68 != (QArrayData *)0x0) {
          LOCK();
          *(int *)local_68 = *(int *)local_68 + -1;
          UNLOCK();
          if (*(int *)local_68 == 0) {
            if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
              QArrayData::deallocate(local_68,8,0x10);
              return;
            }
            goto LAB_0011fa56;
          }
        }
      }
      else {
        iVar4 = QGraphicsSceneMouseEvent::button();
        if (iVar4 == 1) {
          if (*(int *)(lVar9 + 0x28) == 0) {
            *(undefined4 *)(lVar9 + 0x28) = 1;
            FUN_00117930(lVar9);
          }
          *(int *)(param_1 + 0x78) = iVar11;
          *(int *)(param_1 + 0x7c) = iVar12;
        }
        else if (((iVar3 == 1) && (iVar11 = QGraphicsSceneMouseEvent::button(), iVar11 == 2)) &&
                (uVar7 = QGraphicsSceneMouseEvent::buttons(), (uVar7 & 1) == 0)) {
          if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
            FUN_00119a50(param_1,lVar9);
            return;
          }
          goto LAB_0011fa56;
        }
      }
    }
  }
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011fa56:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

