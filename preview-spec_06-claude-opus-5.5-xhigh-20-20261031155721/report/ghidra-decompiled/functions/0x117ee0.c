// rea decompile_all
// name: FUN_00117ee0
// address: 0x117ee0
// body: available
// range: 0x117ee0-0x117f69 inclusive
// range: 0x117f70-0x118143 inclusive
// range: 0x118148-0x11845a inclusive
// range: 0x118460-0x1184da inclusive
// range: 0x1184e0-0x1184fd inclusive
// range: 0x118500-0x118518 inclusive
// range: 0x118520-0x11852e inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11813f to 0x10ef40 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x11852a to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00117ee0(QWidget *param_1,char param_2)

{
  long *plVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  QObject *pQVar5;
  code *pcVar6;
  undefined1 auVar7 [16];
  bool bVar8;
  int iVar9;
  long lVar10;
  KGameHighScoreDialog *this;
  int *piVar11;
  ulonglong uVar12;
  undefined8 *puVar13;
  long lVar14;
  long *plVar15;
  KGameHighScoreDialog *pKVar16;
  long in_FS_OFFSET;
  undefined4 local_174;
  KGuiItem local_170 [8];
  KGuiItem local_168 [8];
  KLocalizedString local_160 [8];
  undefined1 local_158 [16];
  undefined8 local_148;
  QString local_138 [16];
  undefined8 local_128;
  QString local_118 [16];
  undefined8 local_108;
  undefined8 local_f8;
  wchar16 *pwStack_f0;
  undefined8 local_e8;
  undefined1 local_d8 [16];
  undefined8 local_c8;
  undefined4 local_b8 [8];
  undefined1 local_98 [16];
  undefined8 local_88;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  void *local_58;
  undefined4 *puStack_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  (**(code **)(**(long **)(param_1 + 0x50) + 0x68))();
  QAction::setEnabled(SUB81(*(undefined8 *)(param_1 + 0x58),0));
  bVar8 = (bool)KGameDifficulty::global();
  KGameDifficulty::setGameRunning(bVar8);
  if (param_2 == '\0') {
    lVar10 = FUN_0011f4c0();
    if (*(char *)(lVar10 + 0x19) != '\0') {
      local_148 = 0;
      local_158 = (undefined1  [16])0x0;
      KStandardGuiItem::cancel();
      local_138[0] = (QString)0x0;
      local_138[1] = (QString)0x0;
      local_138[2] = (QString)0x0;
      local_138[3] = (QString)0x0;
      local_138[4] = (QString)0x0;
      local_138[5] = (QString)0x0;
      local_138[6] = (QString)0x0;
      local_138[7] = (QString)0x0;
      local_138[8] = (QString)0x0;
      local_138[9] = (QString)0x0;
      local_138[10] = (QString)0x0;
      local_138[0xb] = (QString)0x0;
      local_138[0xc] = (QString)0x0;
      local_138[0xd] = (QString)0x0;
      local_138[0xe] = (QString)0x0;
      local_138[0xf] = (QString)0x0;
      local_118[0] = (QString)0x0;
      local_118[1] = (QString)0x0;
      local_118[2] = (QString)0x0;
      local_118[3] = (QString)0x0;
      local_118[4] = (QString)0x0;
      local_118[5] = (QString)0x0;
      local_118[6] = (QString)0x0;
      local_118[7] = (QString)0x0;
      local_118[8] = (QString)0x0;
      local_118[9] = (QString)0x0;
      local_118[10] = (QString)0x0;
      local_118[0xb] = (QString)0x0;
      local_118[0xc] = (QString)0x0;
      local_118[0xd] = (QString)0x0;
      local_118[0xe] = (QString)0x0;
      local_118[0xf] = (QString)0x0;
      local_f8 = 0;
      pwStack_f0 = L"view-refresh";
      local_d8 = (undefined1  [16])0x0;
      local_128 = 0;
      local_108 = 0;
      local_c8 = 0;
      local_e8 = 0xc;
      ki18nc((char *)&local_78,"@action;button");
      KLocalizedString::toString();
      KLocalizedString::~KLocalizedString((KLocalizedString *)&local_78);
      KGuiItem::KGuiItem(local_168,(QString *)local_b8,(QString *)&local_f8,local_118,local_138);
      local_98 = (undefined1  [16])0x0;
      local_88 = 0;
      ki18n((char *)local_160);
      KLocalizedString::toString();
      KLocalizedString::~KLocalizedString(local_160);
      iVar9 = KMessageBox::questionTwoActions
                        (param_1,&local_78,local_98,local_168,local_170,local_158,1);
      FUN_00116630(&local_78);
      FUN_00116630(local_98);
      KGuiItem::~KGuiItem(local_168);
      FUN_00116630(local_b8);
      FUN_00116630((QString *)&local_f8);
      FUN_00116630(local_d8);
      FUN_00116630(local_118);
      FUN_00116630(local_138);
      KGuiItem::~KGuiItem(local_170);
      FUN_00116630(local_158);
      if (iVar9 == 3) {
        lVar10 = *(long *)(param_1 + 0x40);
        pQVar5 = *(QObject **)(lVar10 + 0x30);
        pQVar5[0x89] = (QObject)0x0;
        plVar15 = *(long **)(pQVar5 + 0x28);
        *(int *)(pQVar5 + 0x8c) = *(int *)(pQVar5 + 0x54) * *(int *)(pQVar5 + 0x58);
        plVar1 = plVar15 + *(long *)(pQVar5 + 0x30);
        for (; plVar15 != plVar1; plVar15 = plVar15 + 1) {
          lVar14 = *plVar15;
          *(undefined4 *)(lVar14 + 0x28) = 0;
          FUN_00117930(lVar14);
          *(undefined4 *)(lVar14 + 0x28) = 0;
          FUN_00117930(lVar14);
          *(undefined1 *)(lVar14 + 0x2d) = 0;
        }
        *(undefined4 *)(pQVar5 + 0x60) = 0;
        local_174 = 0;
        local_58 = (void *)0x0;
        puStack_50 = &local_174;
        QMetaObject::activate(pQVar5,(QMetaObject *)&PTR_staticMetaObject_00127a00,0,&local_58);
        KGamePopupItem::forceHide(*(undefined8 *)(lVar10 + 0x38),1);
        (**(code **)(**(long **)(param_1 + 0x50) + 0x60))();
        QAction::setEnabled(SUB81(*(undefined8 *)(param_1 + 0x58),0));
        lVar10 = *(long *)(param_1 + 0x40);
        lVar14 = FUN_0011f4c0();
        *(byte *)(lVar10 + 0x10) = *(byte *)(lVar14 + 0x1a) ^ 1;
      }
    }
  }
  else if (*(char *)(*(long *)(param_1 + 0x40) + 0x10) != '\0') {
    this = operator_new(0x30);
    KGameHighScoreDialog::KGameHighScoreDialog(this,9,param_1);
    piVar11 = (int *)QtSharedPointer::ExternalRefCountData::getAndRef((QObject *)this);
    if (piVar11 == (int *)0x0) {
      bVar8 = (bool)KGameDifficulty::global();
      KGameHighScoreDialog::initFromDifficulty((KGameDifficulty *)0x0,bVar8);
LAB_001184d4:
      iVar9 = 0;
    }
    else {
      if (piVar11[1] == 0) {
        bVar8 = (bool)KGameDifficulty::global();
        KGameHighScoreDialog::initFromDifficulty((KGameDifficulty *)0x0,bVar8);
      }
      else {
        bVar8 = (bool)KGameDifficulty::global();
        KGameHighScoreDialog::initFromDifficulty((KGameDifficulty *)this,bVar8);
      }
      iVar9 = (int)this;
      if (piVar11[1] == 0) goto LAB_001184d4;
    }
    KGameHighScoreDialog::hideField(iVar9);
    auVar7._8_8_ = 0;
    auVar7._0_8_ = local_98._8_8_;
    local_98 = auVar7 << 0x40;
    local_78 = CONCAT44(local_78._4_4_,0x10);
    uVar12 = FUN_00116900(local_98,&local_78);
    iVar9 = KGameClock::seconds();
    QString::setNum(uVar12,iVar9);
    KGameClock::timeString();
    local_b8[0] = 8;
    puVar13 = (undefined8 *)FUN_00116900(local_98,local_b8);
    uVar2 = *puVar13;
    *puVar13 = local_78;
    uVar3 = puVar13[1];
    puVar13[1] = local_70;
    uVar4 = puVar13[2];
    puVar13[2] = local_68;
    local_78 = uVar2;
    local_70 = uVar3;
    local_68 = uVar4;
    FUN_00116630(&local_78);
    if (piVar11 == (int *)0x0) {
      iVar9 = KGameHighScoreDialog::addScore(0,local_98,2);
      if (iVar9 != 0) {
LAB_00118520:
                    /* WARNING: Does not return */
        pcVar6 = (code *)invalidInstructionException();
        (*pcVar6)();
      }
      uVar2 = local_98._0_8_;
      if ((int *)local_98._0_8_ != (int *)0x0) {
        LOCK();
        *(int *)local_98._0_8_ = *(int *)local_98._0_8_ + -1;
        UNLOCK();
        if ((*(int *)local_98._0_8_ == 0) && ((int *)local_98._0_8_ != (int *)0x0)) {
          FUN_00110c90(*(undefined8 *)(local_98._0_8_ + 0x18));
          operator_delete((void *)uVar2,0x38);
        }
      }
    }
    else {
      pKVar16 = (KGameHighScoreDialog *)0x0;
      if (piVar11[1] != 0) {
        pKVar16 = this;
      }
      iVar9 = KGameHighScoreDialog::addScore(pKVar16,local_98,2);
      if (iVar9 != 0) {
        if (piVar11[1] == 0) goto LAB_00118520;
        (**(code **)(*(long *)this + 0x1a8))();
      }
      if (piVar11[1] != 0) {
        (**(code **)(*(long *)this + 0x20))();
      }
      uVar2 = local_98._0_8_;
      if ((int *)local_98._0_8_ != (int *)0x0) {
        LOCK();
        *(int *)local_98._0_8_ = *(int *)local_98._0_8_ + -1;
        UNLOCK();
        if ((*(int *)local_98._0_8_ == 0) && ((int *)local_98._0_8_ != (int *)0x0)) {
          FUN_00110c90(*(undefined8 *)(local_98._0_8_ + 0x18));
          operator_delete((void *)uVar2,0x38);
        }
      }
      LOCK();
      *piVar11 = *piVar11 + -1;
      UNLOCK();
      if (*piVar11 == 0) {
        if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
          operator_delete(piVar11);
          return;
        }
        goto LAB_0011852a;
      }
    }
  }
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
LAB_0011852a:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

