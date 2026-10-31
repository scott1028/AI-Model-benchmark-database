// rea decompile_all
// name: FUN_00119b00
// address: 0x119b00
// body: available
// range: 0x119b00-0x119b33 inclusive
// range: 0x119b40-0x119b7e inclusive
// range: 0x119b80-0x119c31 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x119bb1 to 0x1197f0 FUN_001197f0 (external=false, thunk=false, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Ghidra Listing reports a terminal call at 0x119c2d to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 FUN_00119b00(long param_1,long param_2)

{
  long *plVar1;
  QTextStream *this;
  long *plVar2;
  long *plVar3;
  long lVar4;
  undefined8 uVar5;
  int iVar6;
  ulong uVar7;
  long in_FS_OFFSET;
  QTextStream *local_80;
  QString local_78 [32];
  undefined8 local_58;
  undefined1 local_50 [16];
  undefined8 local_40;
  long local_30;
  
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if (0 < *(long *)(param_1 + 0x30)) {
    plVar1 = *(long **)(param_1 + 0x28);
    plVar2 = plVar1;
    do {
      plVar3 = plVar2;
      if (plVar1 + *(long *)(param_1 + 0x30) == plVar3) goto LAB_00119b52;
      plVar2 = plVar3 + 1;
    } while (*plVar3 != param_2);
    uVar7 = (long)plVar3 - (long)plVar1 >> 3;
    iVar6 = (int)uVar7;
    if (iVar6 != -1) {
      uVar7 = (ulong)(uint)(iVar6 >> 0x1f) << 0x20 | uVar7 & 0xffffffff;
      if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
        uVar5 = FUN_001197f0(param_1,(long)uVar7 / (long)*(int *)(param_1 + 0x58) & 0xffffffff,
                             (long)uVar7 % (long)*(int *)(param_1 + 0x58) & 0xffffffff);
        return uVar5;
      }
      goto LAB_00119c2d;
    }
  }
LAB_00119b52:
  lVar4 = FUN_0011f430();
  if (*(char *)(lVar4 + 0x10) != '\0') {
    local_40 = *(undefined8 *)(lVar4 + 8);
    local_50 = (undefined1  [16])0x0;
    local_58 = 2;
    QMessageLogger::debug();
    this = local_80;
    QString::fromUtf8(local_78,0x1f,"really strange - item not found");
    QTextStream::operator<<(this,local_78);
    FUN_00116630(local_78);
    if (local_80[0x30] != (QTextStream)0x0) {
      QTextStream::operator<<(local_80,' ');
    }
    QDebug::~QDebug((QDebug *)&local_80);
  }
  if (local_30 == *(long *)(in_FS_OFFSET + 0x28)) {
    return 0;
  }
LAB_00119c2d:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

