// rea decompile_all
// name: FUN_001197f0
// address: 0x1197f0
// body: available
// range: 0x1197f0-0x1198b6 inclusive
// range: 0x1198c0-0x119912 inclusive
// range: 0x119918-0x119946 inclusive
// range: 0x119950-0x11998c inclusive
// range: 0x119990-0x1199a6 inclusive
// range: 0x1199b0-0x1199c5 inclusive
// range: 0x1199d0-0x119a42 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x119a3e to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined8 FUN_001197f0(QObject *param_1,undefined8 param_2,int param_3)

{
  long *plVar1;
  long lVar2;
  undefined4 uVar3;
  int iVar4;
  long *plVar5;
  undefined8 uVar6;
  uint uVar7;
  long *plVar8;
  long in_FS_OFFSET;
  undefined1 local_4d;
  uint local_4c;
  void *local_48;
  uint *puStack_40;
  long local_30;
  
  plVar8 = *(long **)(param_1 + 0x28);
  local_30 = *(long *)(in_FS_OFFSET + 0x28);
  uVar7 = *(int *)(param_1 + 0x8c) - 1;
  *(uint *)(param_1 + 0x8c) = uVar7;
  if (*(char *)(plVar8[*(int *)(param_1 + 0x58) * (int)param_2 + param_3] + 0x2c) == '\0') {
    if (*(int *)(plVar8[*(int *)(param_1 + 0x58) * (int)param_2 + param_3] + 0x30) == 0) {
      FUN_00118690(param_1,param_2,param_3);
      goto LAB_0011989b;
    }
LAB_001198a0:
    plVar1 = plVar8 + *(long *)(param_1 + 0x30);
    plVar5 = plVar8;
    if (plVar8 == plVar1) {
      uVar7 = *(uint *)(param_1 + 0x8c);
      goto LAB_00119a31;
    }
    do {
      if (*(char *)(*plVar5 + 0x2d) != '\0') {
        puStack_40 = &local_4c;
        local_4c = local_4c & 0xffffff00;
        param_1[0x89] = (QObject)0x1;
        goto LAB_001198f0;
      }
      plVar5 = plVar5 + 1;
    } while (plVar1 != plVar5);
    if (*(int *)(param_1 + 0x8c) == *(int *)(param_1 + 0x5c)) {
      do {
        while( true ) {
          lVar2 = *plVar8;
          iVar4 = *(int *)(lVar2 + 0x28);
          if (iVar4 == 3) {
            FUN_00117e70(lVar2);
            iVar4 = *(int *)(lVar2 + 0x28);
          }
          if (1 < iVar4 - 4U) break;
LAB_00119960:
          plVar8 = plVar8 + 1;
          if (plVar1 == plVar8) goto LAB_001199d0;
        }
        if (iVar4 != 2) {
          FUN_00117e70(lVar2);
          goto LAB_00119960;
        }
        plVar8 = plVar8 + 1;
      } while (plVar1 != plVar8);
LAB_001199d0:
      uVar7 = *(uint *)(param_1 + 0x5c);
      goto LAB_001199d5;
    }
  }
  else {
    plVar1 = plVar8 + *(long *)(param_1 + 0x30);
    if (plVar8 != plVar1) {
      do {
        lVar2 = *plVar8;
        iVar4 = *(int *)(lVar2 + 0x28);
        if ((bool)*(char *)(lVar2 + 0x2c) != (iVar4 == 4)) {
          if ((iVar4 != 2) && (iVar4 != 5)) {
            if ((iVar4 != 4) || (uVar3 = 5, *(char *)(lVar2 + 0x2c) != '\0')) {
              uVar3 = 2;
            }
            *(undefined4 *)(lVar2 + 0x28) = uVar3;
            FUN_00117930();
          }
          *(int *)(param_1 + 0x8c) = *(int *)(param_1 + 0x8c) + -1;
        }
        plVar8 = plVar8 + 1;
      } while (plVar1 != plVar8);
LAB_0011989b:
      plVar8 = *(long **)(param_1 + 0x28);
      goto LAB_001198a0;
    }
LAB_00119a31:
    if (*(uint *)(param_1 + 0x5c) == uVar7) {
LAB_001199d5:
      puStack_40 = &local_4c;
      param_1[0x89] = (QObject)0x1;
      local_48 = (void *)0x0;
      local_4c = uVar7;
      QMetaObject::activate(param_1,(QMetaObject *)&PTR_staticMetaObject_00127a00,0,&local_48);
      puStack_40 = (uint *)&local_4d;
      local_4d = 1;
LAB_001198f0:
      local_48 = (void *)0x0;
      QMetaObject::activate(param_1,(QMetaObject *)&PTR_staticMetaObject_00127a00,2,&local_48);
      uVar6 = 1;
      goto LAB_00119929;
    }
  }
  uVar6 = 0;
LAB_00119929:
  if (local_30 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar6;
}

