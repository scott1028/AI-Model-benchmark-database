// rea decompile_all
// name: FUN_001196e0
// address: 0x1196e0
// body: available
// range: 0x1196e0-0x1197ea inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1197e6 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_001196e0(long param_1)

{
  undefined8 *puVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined8 *puVar6;
  int iVar7;
  long in_FS_OFFSET;
  
  lVar2 = *(long *)(in_FS_OFFSET + 0x28);
  if (0 < *(int *)(param_1 + 0x54)) {
    iVar3 = *(int *)(param_1 + 0x58);
    iVar4 = 0;
    do {
      iVar7 = 0;
      iVar5 = iVar4 + 1;
      if (0 < iVar3) {
        do {
          iVar3 = iVar3 * iVar4 + iVar7;
          iVar7 = iVar7 + 1;
          QGraphicsItem::setPos(*(QPointF **)(*(long *)(param_1 + 0x28) + (long)iVar3 * 8));
          iVar3 = *(int *)(param_1 + 0x58);
        } while (iVar7 < iVar3);
      }
      iVar4 = iVar5;
    } while (iVar5 < *(int *)(param_1 + 0x54));
  }
  puVar6 = *(undefined8 **)(param_1 + 0x40);
  puVar1 = puVar6 + *(long *)(param_1 + 0x48);
  for (; puVar1 != puVar6; puVar6 = puVar6 + 1) {
    QGraphicsItem::setPos((QPointF *)*puVar6);
  }
  if (lVar2 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

