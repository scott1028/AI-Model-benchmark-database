// rea decompile_all
// name: FUN_001142d0
// address: 0x1142d0
// body: available
// range: 0x1142d0-0x114381 inclusive
// range: 0x114388-0x11438c inclusive
// range: 0x114390-0x1143b9 inclusive
// range: 0x1143c0-0x1143ff inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11437d to 0x10ef40 <EXTERNAL>::operator.delete (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001142d0(QWidget *param_1)

{
  int iVar1;
  code *pcVar2;
  bool bVar3;
  KGameHighScoreDialog *this;
  int *piVar4;
  
  this = operator_new(0x30);
  KGameHighScoreDialog::KGameHighScoreDialog(this,9,param_1);
  piVar4 = (int *)QtSharedPointer::ExternalRefCountData::getAndRef((QObject *)this);
  if (piVar4 == (int *)0x0) {
    bVar3 = (bool)KGameDifficulty::global();
    KGameHighScoreDialog::initFromDifficulty((KGameDifficulty *)0x0,bVar3);
    KGameHighScoreDialog::hideField(0);
                    /* WARNING: Does not return */
    pcVar2 = (code *)invalidInstructionException();
    (*pcVar2)();
  }
  if (piVar4[1] == 0) {
    bVar3 = (bool)KGameDifficulty::global();
    KGameHighScoreDialog::initFromDifficulty((KGameDifficulty *)0x0,bVar3);
    iVar1 = piVar4[1];
  }
  else {
    bVar3 = (bool)KGameDifficulty::global();
    KGameHighScoreDialog::initFromDifficulty((KGameDifficulty *)this,bVar3);
    iVar1 = piVar4[1];
  }
  if (iVar1 == 0) {
    KGameHighScoreDialog::hideField(0);
    iVar1 = piVar4[1];
  }
  else {
    KGameHighScoreDialog::hideField((int)this);
    iVar1 = piVar4[1];
  }
  if (iVar1 != 0) {
    (**(code **)(*(long *)this + 0x1a8))(this);
    if (piVar4[1] != 0) {
      (**(code **)(*(long *)this + 0x20))(this);
    }
    LOCK();
    *piVar4 = *piVar4 + -1;
    UNLOCK();
    if (*piVar4 != 0) {
      return;
    }
    operator_delete(piVar4);
    return;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)invalidInstructionException();
  (*pcVar2)();
}

