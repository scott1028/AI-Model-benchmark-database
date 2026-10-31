// rea decompile_all
// name: FUN_00118810
// address: 0x118810
// body: available
// range: 0x118810-0x1193e7 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x1193e3 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00118810(void)

{
  undefined8 uVar1;
  ulong uVar2;
  uint uVar3;
  long lVar4;
  long in_FS_OFFSET;
  undefined4 local_7c;
  undefined8 local_78;
  wchar16 *pwStack_70;
  undefined8 local_68;
  undefined1 local_58 [16];
  undefined8 local_48;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  local_78._0_4_ = 1;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(local_58._8_4_ & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 9;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicOne";
  FUN_00116630(local_58);
  local_78._0_4_ = 2;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 9;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicTwo";
  FUN_00116630(local_58);
  local_78._0_4_ = 3;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 0xb;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicThree";
  FUN_00116630(local_58);
  local_78._0_4_ = 4;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 10;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicFour";
  FUN_00116630(local_58);
  local_78._0_4_ = 5;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 10;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicFive";
  FUN_00116630(local_58);
  local_78._0_4_ = 6;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 9;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicSix";
  FUN_00116630(local_58);
  local_78._0_4_ = 7;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 0xb;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicSeven";
  FUN_00116630(local_58);
  local_78._0_4_ = 8;
  FUN_00112a20(local_58,&local_78);
  lVar4 = ((ulong)local_58._8_8_ >> 7) * 0x90 + *(long *)(local_58._0_8_ + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)((uint)local_58._8_8_ & 0x7f)) * 0x20 +
          *(long *)(lVar4 + 0x80);
  local_48 = *(undefined8 *)(lVar4 + 0x18);
  local_58._0_8_ = *(long *)(lVar4 + 8);
  local_58._8_8_ = *(ulong *)(lVar4 + 0x10);
  *(undefined8 *)(lVar4 + 0x18) = 0xb;
  *(undefined8 *)(lVar4 + 8) = 0;
  *(wchar16 **)(lVar4 + 0x10) = L"arabicEight";
  FUN_00116630(local_58);
  local_78 = (ulong)local_78._4_4_ << 0x20;
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 7;
  uVar3 = (uint)local_58._8_8_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x12232a) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,1);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 9;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x122346) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,2);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 9;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x122346) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,3);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 7;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x12232a) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,3);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 8;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x122364) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,4);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 7;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x12232a) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,4);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 4;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x12235a) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,5);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 9;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x122346) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,5);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 4;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x1222e8) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_78 = CONCAT44(local_78._4_4_,5);
  FUN_00111d60(local_58,&local_78);
  uVar1 = local_58._0_8_;
  local_48 = 5;
  uVar3 = local_58._8_4_;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = ZEXT816(0x12233a) << 0x40;
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),local_58);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(local_58);
  local_7c = 6;
  FUN_00111d60(local_58,&local_7c);
  uVar1 = local_58._0_8_;
  local_48 = 0;
  uVar3 = local_58._8_4_;
  local_68 = 7;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = (undefined1  [16])0x0;
  local_78 = 0;
  pwStack_70 = L"cell_up";
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),&local_78);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(&local_78);
  FUN_00116630(local_58);
  local_7c = 6;
  FUN_00111d60(local_58,&local_7c);
  uVar1 = local_58._0_8_;
  local_48 = 0;
  uVar3 = local_58._8_4_;
  local_68 = 4;
  uVar2 = (ulong)local_58._8_8_ >> 7;
  local_58 = (undefined1  [16])0x0;
  local_78 = 0;
  pwStack_70 = L"hint";
  lVar4 = uVar2 * 0x90 + *(long *)(uVar1 + 0x20);
  lVar4 = (ulong)*(byte *)(lVar4 + (ulong)(uVar3 & 0x7f)) * 0x20 + *(long *)(lVar4 + 0x80);
  FUN_00113b60(lVar4 + 8,*(undefined8 *)(lVar4 + 0x18),&local_78);
  if ((*(int **)(lVar4 + 8) == (int *)0x0) || (1 < **(int **)(lVar4 + 8))) {
    FUN_00113710(lVar4 + 8,0,0);
  }
  FUN_00116630(&local_78);
  FUN_00116630(local_58);
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

