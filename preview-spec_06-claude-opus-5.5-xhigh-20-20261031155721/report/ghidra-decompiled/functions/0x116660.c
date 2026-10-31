// rea decompile_all
// name: FUN_00116660
// address: 0x116660
// body: available
// range: 0x116660-0x116723 inclusive
// range: 0x116728-0x1167ce inclusive
// range: 0x1167d0-0x1167ea inclusive
// range: 0x1167f0-0x11680c inclusive
// range: 0x116810-0x11685b inclusive
// range: 0x116860-0x1168d6 inclusive
// range: 0x1168e0-0x1168f0 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1168ec to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00116660(long param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  QTextStream *this;
  char cVar4;
  bool bVar5;
  int iVar6;
  long lVar7;
  QString *pQVar8;
  long in_FS_OFFSET;
  QTextStream *local_90;
  KLocalizedString local_88 [32];
  QArrayData *local_68;
  undefined1 local_60 [16];
  undefined8 local_50;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  lVar7 = FUN_0011f430();
  if (*(char *)(lVar7 + 0x10) != '\0') {
    local_50 = *(undefined8 *)(lVar7 + 8);
    local_60 = (undefined1  [16])0x0;
    local_68 = (QArrayData *)0x2;
    QMessageLogger::debug();
    this = local_90;
    QString::fromUtf8(local_88,0xb,"Inside game");
    QTextStream::operator<<(this,(QString *)local_88);
    FUN_00116630(local_88);
    if (local_90[0x30] != (QTextStream)0x0) {
      QTextStream::operator<<(local_90,' ');
    }
    QDebug::~QDebug((QDebug *)&local_90);
  }
  (**(code **)(**(long **)(param_1 + 0x50) + 0x60))();
  (**(code **)(**(long **)(param_1 + 0x50) + 0x68))();
  cVar4 = QAction::isChecked();
  if (cVar4 != '\0') {
    FUN_0011f320(*(undefined8 *)(param_1 + 0x40),0);
    QAction::setChecked(SUB81(*(undefined8 *)(param_1 + 0x58),0));
  }
  QAction::setEnabled(SUB81(*(undefined8 *)(param_1 + 0x58),0));
  bVar5 = (bool)KGameDifficulty::global();
  KGameDifficulty::setGameRunning(bVar5);
  iVar6 = KGameDifficulty::globalLevel();
  if (iVar6 == 0x28) {
    FUN_001206d0(*(undefined8 *)(param_1 + 0x40),0x10,0x10,0x28);
  }
  else if (iVar6 < 0x29) {
    if (iVar6 == -1) {
      uVar3 = *(undefined8 *)(param_1 + 0x40);
      lVar7 = FUN_0011f4c0();
      uVar1 = *(undefined4 *)(lVar7 + 0x28);
      lVar7 = FUN_0011f4c0();
      uVar2 = *(undefined4 *)(lVar7 + 0x20);
      lVar7 = FUN_0011f4c0();
      FUN_001206d0(uVar3,*(undefined4 *)(lVar7 + 0x24),uVar2,uVar1);
    }
    else if (iVar6 == 0x1e) {
      FUN_001206d0(*(undefined8 *)(param_1 + 0x40),9,9,10);
      lVar7 = *(long *)(param_1 + 0x70);
      goto joined_r0x0011674c;
    }
  }
  else if (iVar6 == 0x32) {
    FUN_001206d0(*(undefined8 *)(param_1 + 0x40),0x10,0x1e,99);
  }
  lVar7 = *(long *)(param_1 + 0x70);
joined_r0x0011674c:
  if ((lVar7 == 0) || (*(int *)(lVar7 + 4) == 0)) {
    pQVar8 = (QString *)0x0;
  }
  else {
    pQVar8 = *(QString **)(param_1 + 0x78);
  }
  ki18n((char *)local_88);
  KLocalizedString::toString();
  KLocalizedString::~KLocalizedString(local_88);
  QLabel::setText(pQVar8);
  if (local_68 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_68 = *(int *)local_68 + -1;
    UNLOCK();
    if (*(int *)local_68 == 0) {
      QArrayData::deallocate(local_68,2,0x10);
    }
  }
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

