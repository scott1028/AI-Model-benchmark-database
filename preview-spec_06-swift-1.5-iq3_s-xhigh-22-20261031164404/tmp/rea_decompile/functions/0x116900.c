// rea decompile_all
// name: FUN_00116900
// address: 0x116900
// body: available
// range: 0x116900-0x116974 inclusive
// range: 0x116980-0x1169c5 inclusive
// range: 0x1169e0-0x116a26 inclusive
// range: 0x116a30-0x116afd inclusive
// range: 0x116b00-0x116bf3 inclusive
// range: 0x116bf8-0x116c1a inclusive
// range: 0x116c20-0x116c94 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x116c66 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


_Rb_tree_node_base * FUN_00116900(long *param_1,int *param_2)

{
  _Rb_tree_node_base *p_Var1;
  _Rb_tree_node_base *p_Var2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  undefined8 uVar7;
  _Rb_tree_node_base *p_Var8;
  _Rb_tree_node_base *p_Var9;
  int *piVar10;
  long lVar11;
  long lVar12;
  int *piVar13;
  _Rb_tree_node_base *p_Var14;
  int *piVar15;
  _Rb_tree_node_base *p_Var16;
  bool bVar17;
  long in_FS_OFFSET;
  undefined1 local_50 [16];
  undefined8 local_40;
  long local_30;
  
  piVar13 = (int *)*param_1;
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  if (piVar13 == (int *)0x0) {
    piVar15 = (int *)0x0;
LAB_00116c22:
    piVar13 = operator_new(0x38);
    *piVar13 = 0;
    piVar13[4] = 0;
    piVar13[6] = 0;
    piVar13[7] = 0;
    *(int **)(piVar13 + 8) = piVar13 + 4;
    *(int **)(piVar13 + 10) = piVar13 + 4;
    piVar13[0xc] = 0;
    piVar13[0xd] = 0;
    *param_1 = (long)piVar13;
    LOCK();
    *piVar13 = *piVar13 + 1;
    UNLOCK();
LAB_00116c5d:
    piVar10 = (int *)*param_1;
LAB_00116958:
    p_Var16 = (_Rb_tree_node_base *)(piVar10 + 4);
  }
  else {
    piVar15 = (int *)0x0;
    piVar10 = piVar13;
    if (*piVar13 != 1) {
      LOCK();
      *piVar13 = *piVar13 + 1;
      UNLOCK();
      piVar10 = (int *)*param_1;
      piVar15 = piVar13;
    }
    if (piVar10 == (int *)0x0) goto LAB_00116c22;
    if (*piVar10 == 1) goto LAB_00116958;
    piVar10 = operator_new(0x38);
    lVar5 = *param_1;
    piVar10[6] = 0;
    piVar10[7] = 0;
    p_Var16 = (_Rb_tree_node_base *)(piVar10 + 4);
    lVar6 = *(long *)(lVar5 + 0x18);
    *piVar10 = 0;
    piVar10[4] = 0;
    *(_Rb_tree_node_base **)(piVar10 + 8) = p_Var16;
    *(_Rb_tree_node_base **)(piVar10 + 10) = p_Var16;
    piVar10[0xc] = 0;
    piVar10[0xd] = 0;
    if (lVar6 != 0) {
      lVar11 = FUN_00110fe0(lVar6,p_Var16);
      lVar6 = lVar11;
      do {
        lVar12 = lVar6;
        lVar6 = *(long *)(lVar12 + 0x10);
      } while (lVar6 != 0);
      *(long *)(piVar10 + 8) = lVar12;
      lVar6 = lVar11;
      do {
        lVar12 = lVar6;
        lVar6 = *(long *)(lVar12 + 0x18);
      } while (lVar6 != 0);
      uVar7 = *(undefined8 *)(lVar5 + 0x30);
      *(long *)(piVar10 + 10) = lVar12;
      *(long *)(piVar10 + 6) = lVar11;
      *(undefined8 *)(piVar10 + 0xc) = uVar7;
    }
    LOCK();
    *piVar10 = *piVar10 + 1;
    UNLOCK();
    piVar13 = (int *)*param_1;
    *param_1 = (long)piVar10;
    if (piVar13 != (int *)0x0) {
      LOCK();
      *piVar13 = *piVar13 + -1;
      UNLOCK();
      if (*piVar13 == 0) {
        FUN_00110c90(*(undefined8 *)(piVar13 + 6));
        operator_delete(piVar13,0x38);
        piVar10 = (int *)*param_1;
        p_Var16 = (_Rb_tree_node_base *)(piVar10 + 4);
        goto LAB_0011695c;
      }
      goto LAB_00116c5d;
    }
  }
LAB_0011695c:
  iVar3 = *param_2;
  p_Var8 = *(_Rb_tree_node_base **)(piVar10 + 6);
  p_Var9 = p_Var16;
  p_Var14 = p_Var8;
  if (p_Var8 == (_Rb_tree_node_base *)0x0) {
    local_40 = 0;
    local_50 = (undefined1  [16])0x0;
    if ((p_Var16 == *(_Rb_tree_node_base **)(piVar10 + 8)) ||
       (p_Var9 = (_Rb_tree_node_base *)std::_Rb_tree_decrement(p_Var16),
       *(int *)(p_Var9 + 0x20) < iVar3)) {
      bVar17 = true;
      p_Var14 = p_Var16;
      goto LAB_00116a71;
    }
  }
  else {
    do {
      while( true ) {
        p_Var1 = p_Var14 + 0x10;
        p_Var2 = p_Var14 + 0x18;
        if (*(int *)(p_Var14 + 0x20) < iVar3) break;
        p_Var9 = p_Var14;
        p_Var14 = *(_Rb_tree_node_base **)p_Var1;
        if (*(_Rb_tree_node_base **)p_Var1 == (_Rb_tree_node_base *)0x0) goto LAB_001169a1;
      }
      p_Var14 = *(_Rb_tree_node_base **)p_Var2;
    } while (*(_Rb_tree_node_base **)p_Var2 != (_Rb_tree_node_base *)0x0);
LAB_001169a1:
    if ((p_Var16 != p_Var9) && (*(int *)(p_Var9 + 0x20) <= iVar3)) goto LAB_00116acb;
    local_40 = 0;
    local_50 = (undefined1  [16])0x0;
    do {
      p_Var14 = p_Var8;
      iVar4 = *(int *)(p_Var14 + 0x20);
      p_Var8 = *(_Rb_tree_node_base **)(p_Var14 + 0x18);
      if (iVar3 < iVar4) {
        p_Var8 = *(_Rb_tree_node_base **)(p_Var14 + 0x10);
      }
    } while (p_Var8 != (_Rb_tree_node_base *)0x0);
    p_Var9 = p_Var14;
    if (iVar3 < iVar4) {
      if (*(_Rb_tree_node_base **)(piVar10 + 8) != p_Var14) {
        p_Var9 = (_Rb_tree_node_base *)std::_Rb_tree_decrement(p_Var14);
        iVar4 = *(int *)(p_Var9 + 0x20);
        goto LAB_00116a06;
      }
    }
    else {
LAB_00116a06:
      if (iVar3 <= iVar4) goto LAB_00116aba;
    }
    bVar17 = true;
    if (p_Var16 != p_Var14) {
      bVar17 = iVar3 < *(int *)(p_Var14 + 0x20);
    }
LAB_00116a71:
    p_Var9 = operator_new(0x40);
    *(int *)(p_Var9 + 0x20) = iVar3;
    *(undefined8 *)(p_Var9 + 0x38) = 0;
    *(undefined1 (*) [16])(p_Var9 + 0x28) = (undefined1  [16])0x0;
    std::_Rb_tree_insert_and_rebalance(bVar17,p_Var9,p_Var14,p_Var16);
    *(long *)(piVar10 + 0xc) = *(long *)(piVar10 + 0xc) + 1;
  }
LAB_00116aba:
  FUN_00116630(local_50);
LAB_00116acb:
  if (piVar15 != (int *)0x0) {
    LOCK();
    *piVar15 = *piVar15 + -1;
    UNLOCK();
    if (*piVar15 == 0) {
      FUN_00110c90(*(undefined8 *)(piVar15 + 6));
      operator_delete(piVar15,0x38);
    }
  }
  if (local_30 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return p_Var9 + 0x28;
}

