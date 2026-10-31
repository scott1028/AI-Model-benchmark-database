// rea decompile_all
// name: FUN_0011e030
// address: 0x11e030
// body: available
// range: 0x11e030-0x11ea3a inclusive
// range: 0x11ea40-0x11ea5a inclusive
// range: 0x11ea60-0x11ea7a inclusive
// range: 0x11ea80-0x11ea9a inclusive
// range: 0x11eaa0-0x11eaba inclusive
// range: 0x11eac0-0x11eada inclusive
// range: 0x11eae0-0x11eb35 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x11eb31 to 0x10ee50 <EXTERNAL>::__stack_chk_fail (external=false, thunk=true, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0011e030(KConfigSkeleton *param_1)

{
  int *piVar1;
  code *pcVar2;
  int iVar3;
  ItemBool *pIVar4;
  ItemEnum *this;
  ItemInt *pIVar5;
  long lVar6;
  undefined8 *puVar7;
  QString *pQVar8;
  undefined8 *puVar9;
  long in_FS_OFFSET;
  byte bVar10;
  undefined1 local_f8 [16];
  long local_e8;
  QArrayData *local_d8;
  wchar16 *pwStack_d0;
  undefined8 local_c8;
  QArrayData *local_b8;
  wchar16 *pwStack_b0;
  undefined8 local_a8;
  long local_40;
  
  bVar10 = 0;
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"kminesrc";
  local_a8 = 8;
  KConfigSkeleton::KConfigSkeleton(param_1,(QString *)&local_b8,(QObject *)0x0);
  FUN_00116630((QString *)&local_b8);
  *(undefined ***)param_1 = &PTR_metaObject_001284e8;
  if (DAT_001292d8 < -1) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)invalidInstructionException();
    _DAT_00000000 = param_1;
    (*pcVar2)();
  }
  if ((DAT_001292d0 == '\0') && (iVar3 = __cxa_guard_acquire(&DAT_001292d0), iVar3 != 0)) {
    DAT_001292c8 = (KConfigSkeleton *)0x0;
    DAT_001292d8 = -1;
    __cxa_atexit(FUN_0011c700,&DAT_001292c8,&PTR_LOOP_00129008);
    __cxa_guard_release(&DAT_001292d0);
  }
  local_a8 = 7;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"General";
  DAT_001292c8 = param_1;
  KCoreConfigSkeleton::setCurrentGroup((QString *)param_1);
  FUN_00116630((QString *)&local_b8);
  pIVar4 = operator_new(0x68);
  local_c8 = 0xd;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"question mark";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemBool::ItemBool
            (pIVar4,(QString *)&local_b8,(QString *)&local_d8,(bool *)(param_1 + 0x18),true);
  FUN_00116630((QString *)&local_b8);
  FUN_00116630(&local_d8);
  local_a8 = 0x10;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"UseQuestionMarks";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar4);
  FUN_00116630((QString *)&local_b8);
  pIVar4 = operator_new(0x68);
  local_c8 = 0xc;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"kmines_reset";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemBool::ItemBool
            (pIVar4,(QString *)&local_b8,(QString *)&local_d8,(bool *)(param_1 + 0x19),true);
  FUN_00116630((QString *)&local_b8);
  FUN_00116630(&local_d8);
  local_a8 = 0x10;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"AllowKminesReset";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar4);
  FUN_00116630((QString *)&local_b8);
  pIVar4 = operator_new(0x68);
  local_c8 = 0x16;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"disable_score_on_reset";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemBool::ItemBool
            (pIVar4,(QString *)&local_b8,(QString *)&local_d8,(bool *)(param_1 + 0x1a),false);
  FUN_00116630((QString *)&local_b8);
  FUN_00116630(&local_d8);
  local_a8 = 0x13;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"DisableScoreOnReset";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar4);
  FUN_00116630((QString *)&local_b8);
  pIVar4 = operator_new(0x68);
  local_c8 = 0x27;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"explore_with_left_click_on_number_cells";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemBool::ItemBool
            (pIVar4,(QString *)&local_b8,(QString *)&local_d8,(bool *)(param_1 + 0x1b),false);
  FUN_00116630((QString *)&local_b8);
  FUN_00116630(&local_d8);
  local_a8 = 0x21;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"ExploreWithLeftClickOnNumberCells";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar4);
  FUN_00116630((QString *)&local_b8);
  pQVar8 = (QString *)&local_b8;
  for (lVar6 = 0xf; lVar6 != 0; lVar6 = lVar6 + -1) {
    *(undefined8 *)pQVar8 = 0;
    pQVar8 = pQVar8 + (ulong)bVar10 * -0x10 + 8;
  }
  pwStack_b0 = L"MouseRelease";
  local_e8 = 0;
  local_a8 = 0xc;
  local_f8 = (undefined1  [16])0x0;
  FUN_0011cda0((QList *)local_f8,0,(QString *)&local_b8);
  if (((int *)local_f8._0_8_ == (int *)0x0) || (1 < *(int *)local_f8._0_8_)) {
    FUN_0011c770((QList *)local_f8,0,0);
  }
  pQVar8 = (QString *)&local_b8;
  for (lVar6 = 0xf; lVar6 != 0; lVar6 = lVar6 + -1) {
    *(undefined8 *)pQVar8 = 0;
    pQVar8 = pQVar8 + (ulong)bVar10 * -0x10 + 8;
  }
  pwStack_b0 = L"MousePress";
  local_a8 = 10;
  FUN_0011cda0((QList *)local_f8,local_e8,(QString *)&local_b8);
  if (((int *)local_f8._0_8_ == (int *)0x0) || (1 < *(int *)local_f8._0_8_)) {
    FUN_0011c770((QList *)local_f8,0,0);
  }
  this = operator_new(0x90);
  local_c8 = 0xd;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"place_flag_on";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemEnum::ItemEnum
            (this,(QString *)&local_b8,(QString *)&local_d8,(int *)(param_1 + 0x1c),
             (QList *)local_f8,0);
  FUN_00116630((QString *)&local_b8);
  FUN_00116630(&local_d8);
  local_a8 = 0xb;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"PlaceFlagOn";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)this);
  FUN_00116630((QString *)&local_b8);
  local_a8 = 7;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"Options";
  KCoreConfigSkeleton::setCurrentGroup((QString *)param_1);
  FUN_00116630((QString *)&local_b8);
  pIVar5 = operator_new(0x78);
  local_c8 = 0xc;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"custom width";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemInt::ItemInt
            (pIVar5,(QString *)&local_b8,(QString *)&local_d8,(int *)(param_1 + 0x20),10);
  FUN_00116630((QString *)&local_b8);
  FUN_00116630(&local_d8);
  KCoreConfigSkeleton::ItemInt::setMinValue((int)pIVar5);
  KCoreConfigSkeleton::ItemInt::setMaxValue((int)pIVar5);
  local_a8 = 0xb;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"CustomWidth";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar5);
  FUN_00116630((QString *)&local_b8);
  pIVar5 = operator_new(0x78);
  local_c8 = 0xd;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"custom height";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemInt::ItemInt
            (pIVar5,(QString *)&local_b8,(QString *)&local_d8,(int *)(param_1 + 0x24),10);
  if (local_b8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_b8 = *(int *)local_b8 + -1;
    UNLOCK();
    if (*(int *)local_b8 == 0) {
      QArrayData::deallocate(local_b8,2,0x10);
    }
  }
  if (local_d8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_d8 = *(int *)local_d8 + -1;
    UNLOCK();
    if (*(int *)local_d8 == 0) {
      QArrayData::deallocate(local_d8,2,0x10);
    }
  }
  KCoreConfigSkeleton::ItemInt::setMinValue((int)pIVar5);
  KCoreConfigSkeleton::ItemInt::setMaxValue((int)pIVar5);
  local_a8 = 0xc;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"CustomHeight";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar5);
  if (local_b8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_b8 = *(int *)local_b8 + -1;
    UNLOCK();
    if (*(int *)local_b8 == 0) {
      QArrayData::deallocate(local_b8,2,0x10);
    }
  }
  pIVar5 = operator_new(0x78);
  local_c8 = 0xc;
  local_d8 = (QArrayData *)0x0;
  pwStack_d0 = L"custom mines";
  KCoreConfigSkeleton::currentGroup();
  KCoreConfigSkeleton::ItemInt::ItemInt
            (pIVar5,(QString *)&local_b8,(QString *)&local_d8,(int *)(param_1 + 0x28),0x14);
  if (local_b8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_b8 = *(int *)local_b8 + -1;
    UNLOCK();
    if (*(int *)local_b8 == 0) {
      QArrayData::deallocate(local_b8,2,0x10);
    }
  }
  if (local_d8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_d8 = *(int *)local_d8 + -1;
    UNLOCK();
    if (*(int *)local_d8 == 0) {
      QArrayData::deallocate(local_d8,2,0x10);
    }
  }
  KCoreConfigSkeleton::ItemInt::setMinValue((int)pIVar5);
  local_a8 = 0xb;
  local_b8 = (QArrayData *)0x0;
  pwStack_b0 = L"CustomMines";
  KCoreConfigSkeleton::addItem((KConfigSkeletonItem *)param_1,(QString *)pIVar5);
  if (local_b8 != (QArrayData *)0x0) {
    LOCK();
    *(int *)local_b8 = *(int *)local_b8 + -1;
    UNLOCK();
    if (*(int *)local_b8 == 0) {
      QArrayData::deallocate(local_b8,2,0x10);
    }
  }
  if ((int *)local_f8._0_8_ != (int *)0x0) {
    LOCK();
    *(int *)local_f8._0_8_ = *(int *)local_f8._0_8_ + -1;
    UNLOCK();
    if (*(int *)local_f8._0_8_ == 0) {
      puVar9 = (undefined8 *)(local_f8._8_8_ + local_e8 * 0xf * 8);
      for (puVar7 = (undefined8 *)local_f8._8_8_; puVar7 != puVar9; puVar7 = puVar7 + 0xf) {
        piVar1 = (int *)puVar7[0xc];
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (*piVar1 == 0) {
            QArrayData::deallocate((QArrayData *)puVar7[0xc],2,0x10);
          }
        }
        piVar1 = (int *)puVar7[9];
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (*piVar1 == 0) {
            QArrayData::deallocate((QArrayData *)puVar7[9],2,0x10);
          }
        }
        piVar1 = (int *)puVar7[6];
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (*piVar1 == 0) {
            QArrayData::deallocate((QArrayData *)puVar7[6],2,0x10);
          }
        }
        piVar1 = (int *)puVar7[3];
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (*piVar1 == 0) {
            QArrayData::deallocate((QArrayData *)puVar7[3],2,0x10);
          }
        }
        piVar1 = (int *)*puVar7;
        if (piVar1 != (int *)0x0) {
          LOCK();
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (*piVar1 == 0) {
            QArrayData::deallocate((QArrayData *)*puVar7,2,0x10);
          }
        }
      }
      QArrayData::deallocate((QArrayData *)local_f8._0_8_,0x78,0x10);
    }
  }
  if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

