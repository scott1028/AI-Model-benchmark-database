// rea decompile_all
// name: FUN_00119c40
// address: 0x119c40
// body: available
// range: 0x119c40-0x119d5d inclusive
// range: 0x119d60-0x119df7 inclusive
// range: 0x119e00-0x119e9b inclusive
// range: 0x119ea0-0x119efe inclusive
// range: 0x119f00-0x119f6e inclusive
// range: 0x119f70-0x119f78 inclusive
// range: 0x119f80-0x119fe8 inclusive
// range: 0x119ff0-0x11a010 inclusive
// range: 0x11a018-0x11a039 inclusive
// range: 0x11a040-0x11a057 inclusive
// range: 0x11a060-0x11a06c inclusive
// range: 0x11a070-0x11a0aa inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11a0a6 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined1 (*) [16] FUN_00119c40(undefined1 (*param_1) [16],long param_2)

{
  undefined8 uVar1;
  long lVar2;
  bool bVar3;
  int *piVar4;
  long lVar5;
  undefined8 *puVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  undefined8 *__src;
  long lVar10;
  long lVar11;
  long lVar12;
  int *piVar13;
  long in_FS_OFFSET;
  QArrayData *local_58;
  int *local_50;
  long local_48;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  FUN_00116240(&local_58);
  *(undefined8 *)param_1[1] = 0;
  *param_1 = (undefined1  [16])0x0;
  if (local_50 + local_48 * 2 != local_50) {
    piVar4 = (int *)0x0;
    lVar12 = 0;
    piVar13 = local_50;
    do {
      uVar1 = *(undefined8 *)
               (*(long *)(param_2 + 0x28) +
               (long)(*piVar13 * *(int *)(param_2 + 0x58) + piVar13[1]) * 8);
      if (piVar4 == (int *)0x0) {
LAB_00119eb0:
        FUN_0011d400(param_1,0,1);
        lVar10 = *(long *)param_1[1];
        __src = (undefined8 *)(*(long *)(*param_1 + 8) + lVar12 * 8);
        if (lVar12 < lVar10) {
LAB_00119ed4:
          memmove(__src + 1,__src,(lVar10 - lVar12) * 8);
          lVar10 = *(long *)param_1[1];
          piVar4 = *(int **)*param_1;
        }
        else {
LAB_00119d29:
          piVar4 = *(int **)*param_1;
        }
LAB_00119d2c:
        *(long *)param_1[1] = lVar10 + 1;
        *__src = uVar1;
        if (piVar4 != (int *)0x0) {
LAB_00119daa:
          if (*piVar4 < 2) goto joined_r0x00119d50;
        }
LAB_00119d3c:
        FUN_0011d400(param_1,0,0);
      }
      else {
        lVar10 = *(long *)param_1[1];
        if (*piVar4 < 2) {
          if (lVar12 != lVar10) {
            if (lVar12 != 0) goto LAB_00119ce0;
            piVar4 = *(int **)*param_1;
            if (piVar4 != (int *)0x0) {
              uVar8 = *(ulong *)(*param_1 + 8);
              if (uVar8 != ((long)piVar4 + 0x1fU & 0xfffffffffffffff0)) goto LAB_00119d93;
LAB_00119f40:
              if (lVar10 == 0) goto LAB_00119cec;
              goto LAB_00119f49;
            }
LAB_00119ea5:
            if (lVar10 != 0) goto LAB_0011a085;
            goto LAB_00119eb0;
          }
          piVar4 = *(int **)*param_1;
          if (piVar4 == (int *)0x0) {
            if (lVar12 != 0) goto LAB_00119eb0;
            goto LAB_00119ea5;
          }
          uVar8 = *(ulong *)(*param_1 + 8);
          uVar7 = (long)piVar4 + 0x1fU & 0xfffffffffffffff0;
          if (*(long *)(piVar4 + 2) - ((long)(uVar8 - uVar7) >> 3) == lVar12) {
            if (lVar12 != 0) goto LAB_00119cec;
            if (uVar8 == uVar7) goto LAB_00119f40;
LAB_00119d93:
            *(undefined8 *)(uVar8 - 8) = uVar1;
            *(ulong *)(*param_1 + 8) = uVar8 - 8;
            lVar12 = lVar10;
          }
          else {
            *(undefined8 *)(uVar8 + lVar12 * 8) = uVar1;
          }
          *(long *)param_1[1] = lVar12 + 1;
          piVar4 = *(int **)*param_1;
          goto LAB_00119daa;
        }
        if ((lVar10 == 0) || (lVar12 != 0)) {
LAB_00119ce0:
          piVar4 = *(int **)*param_1;
          if (piVar4 == (int *)0x0) goto LAB_00119eb0;
LAB_00119cec:
          if ((*piVar4 < 2) && (lVar2 = *(long *)*param_1, lVar2 != 0)) {
            lVar9 = *(long *)(*param_1 + 8);
            lVar10 = *(long *)param_1[1];
            lVar11 = lVar9 - (lVar2 + 0x1fU & 0xfffffffffffffff0);
            lVar5 = lVar11 >> 3;
            if (*(long *)(lVar2 + 8) - lVar5 <= lVar10) {
              if ((lVar11 < 1) || (*(long *)(lVar2 + 8) * 2 <= lVar10 * 3)) goto LAB_00119cf7;
              bVar3 = false;
              lVar10 = 0;
LAB_00119e5a:
              puVar6 = (undefined8 *)(lVar9 + (lVar10 - lVar5) * 8);
              FUN_0011dca0();
              *(undefined8 **)(*param_1 + 8) = puVar6;
              __src = puVar6 + lVar12;
              if (!bVar3) goto LAB_00119d0e;
              goto LAB_00119e84;
            }
            __src = (undefined8 *)(lVar9 + lVar12 * 8);
          }
          else {
LAB_00119cf7:
            FUN_0011d400(param_1,0,1);
            __src = (undefined8 *)(*(long *)(*param_1 + 8) + lVar12 * 8);
LAB_00119d0e:
            lVar10 = *(long *)param_1[1];
          }
          if (lVar12 < lVar10) goto LAB_00119ed4;
          goto LAB_00119d29;
        }
        piVar4 = *(int **)*param_1;
        if (piVar4 == (int *)0x0) {
LAB_0011a085:
          FUN_0011d400(param_1,1,1);
          puVar6 = *(undefined8 **)(*param_1 + 8);
          piVar4 = *(int **)*param_1;
          __src = puVar6;
LAB_00119e87:
          lVar10 = *(long *)param_1[1];
          __src = __src + -1;
          *(undefined8 **)(*param_1 + 8) = puVar6 + -1;
          goto LAB_00119d2c;
        }
LAB_00119f49:
        if ((1 < *piVar4) || (piVar4 = *(int **)*param_1, piVar4 == (int *)0x0)) {
LAB_00119f50:
          FUN_0011d400(param_1,1,1);
          puVar6 = *(undefined8 **)(*param_1 + 8);
          __src = puVar6 + lVar12;
LAB_00119e84:
          piVar4 = *(int **)*param_1;
          goto LAB_00119e87;
        }
        lVar9 = *(long *)(*param_1 + 8);
        lVar5 = lVar9 - ((long)piVar4 + 0x1fU & 0xfffffffffffffff0);
        if (lVar5 < 1) {
          lVar10 = *(long *)(piVar4 + 2);
          lVar5 = lVar5 >> 3;
          lVar2 = *(long *)param_1[1];
          if (lVar10 - lVar5 <= lVar2) goto LAB_00119f50;
          if (lVar2 * 3 < lVar10) {
            lVar10 = ((lVar10 - lVar2) + -1) - ((lVar10 - lVar2) + -1 >> 0x3f) >> 1;
            if (lVar10 < 0) {
              lVar10 = 0;
            }
            bVar3 = true;
            lVar10 = lVar10 + 1;
            goto LAB_00119e5a;
          }
          FUN_0011d400(param_1,1,1);
          puVar6 = *(undefined8 **)(*param_1 + 8);
          piVar4 = *(int **)*param_1;
          __src = puVar6 + lVar12;
          goto LAB_00119e87;
        }
        *(long *)param_1[1] = *(long *)param_1[1] + 1;
        *(long *)(*param_1 + 8) = lVar9 + -8;
        *(undefined8 *)(lVar9 + -8) = uVar1;
        if (1 < *piVar4) goto LAB_00119d3c;
      }
joined_r0x00119d50:
      if (local_50 + local_48 * 2 == piVar13 + 2) break;
      piVar13 = piVar13 + 2;
      lVar12 = *(long *)param_1[1];
      piVar4 = *(int **)*param_1;
    } while( true );
  }
  if (local_58 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_58 = *(int *)local_58 + -1;
    UNLOCK();
    if (*(int *)local_58 == 0) {
      QArrayData::deallocate(local_58,8,0x10);
    }
  }
  if (local_40 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return param_1;
}

