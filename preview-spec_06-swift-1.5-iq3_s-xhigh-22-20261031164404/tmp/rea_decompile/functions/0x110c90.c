// rea decompile_all
// name: FUN_00110c90
// address: 0x110c90
// body: available
// range: 0x110c90-0x110e16 inclusive
// range: 0x110e20-0x110ea6 inclusive
// range: 0x110eb0-0x110fd4 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00110c90(void *param_1)

{
  void *pvVar1;
  int *piVar2;
  void *pvVar3;
  void *pvVar4;
  void *pvVar5;
  void *pvVar6;
  void *pvVar7;
  void *pvVar8;
  void *pvVar9;
  void *pvVar10;
  
  while (param_1 != (void *)0x0) {
    pvVar10 = *(void **)((long)param_1 + 0x18);
    while (pvVar10 != (void *)0x0) {
      pvVar9 = *(void **)((long)pvVar10 + 0x18);
      while (pvVar9 != (void *)0x0) {
        pvVar8 = *(void **)((long)pvVar9 + 0x18);
        while (pvVar8 != (void *)0x0) {
          pvVar6 = *(void **)((long)pvVar8 + 0x18);
          while (pvVar6 != (void *)0x0) {
            pvVar4 = *(void **)((long)pvVar6 + 0x18);
            while (pvVar4 != (void *)0x0) {
              pvVar5 = *(void **)((long)pvVar4 + 0x18);
              while (pvVar5 != (void *)0x0) {
                pvVar7 = *(void **)((long)pvVar5 + 0x18);
                while (pvVar7 != (void *)0x0) {
                  pvVar3 = *(void **)((long)pvVar7 + 0x18);
                  while (pvVar3 != (void *)0x0) {
                    FUN_00110c90(*(undefined8 *)((long)pvVar3 + 0x18));
                    pvVar1 = *(void **)((long)pvVar3 + 0x10);
                    piVar2 = *(int **)((long)pvVar3 + 0x28);
                    if (piVar2 != (int *)0x0) {
                      LOCK();
                      *piVar2 = *piVar2 + -1;
                      UNLOCK();
                      if (*piVar2 == 0) {
                        QArrayData::deallocate(*(QArrayData **)((long)pvVar3 + 0x28),2,0x10);
                      }
                    }
                    operator_delete(pvVar3,0x40);
                    pvVar3 = pvVar1;
                  }
                  piVar2 = *(int **)((long)pvVar7 + 0x28);
                  pvVar3 = *(void **)((long)pvVar7 + 0x10);
                  if (piVar2 != (int *)0x0) {
                    LOCK();
                    *piVar2 = *piVar2 + -1;
                    UNLOCK();
                    if (*piVar2 == 0) {
                      QArrayData::deallocate(*(QArrayData **)((long)pvVar7 + 0x28),2,0x10);
                    }
                  }
                  operator_delete(pvVar7,0x40);
                  pvVar7 = pvVar3;
                }
                piVar2 = *(int **)((long)pvVar5 + 0x28);
                pvVar7 = *(void **)((long)pvVar5 + 0x10);
                if (piVar2 != (int *)0x0) {
                  LOCK();
                  *piVar2 = *piVar2 + -1;
                  UNLOCK();
                  if (*piVar2 == 0) {
                    QArrayData::deallocate(*(QArrayData **)((long)pvVar5 + 0x28),2,0x10);
                  }
                }
                operator_delete(pvVar5,0x40);
                pvVar5 = pvVar7;
              }
              piVar2 = *(int **)((long)pvVar4 + 0x28);
              pvVar5 = *(void **)((long)pvVar4 + 0x10);
              if (piVar2 != (int *)0x0) {
                LOCK();
                *piVar2 = *piVar2 + -1;
                UNLOCK();
                if (*piVar2 == 0) {
                  QArrayData::deallocate(*(QArrayData **)((long)pvVar4 + 0x28),2,0x10);
                }
              }
              operator_delete(pvVar4,0x40);
              pvVar4 = pvVar5;
            }
            piVar2 = *(int **)((long)pvVar6 + 0x28);
            pvVar4 = *(void **)((long)pvVar6 + 0x10);
            if (piVar2 != (int *)0x0) {
              LOCK();
              *piVar2 = *piVar2 + -1;
              UNLOCK();
              if (*piVar2 == 0) {
                QArrayData::deallocate(*(QArrayData **)((long)pvVar6 + 0x28),2,0x10);
              }
            }
            operator_delete(pvVar6,0x40);
            pvVar6 = pvVar4;
          }
          piVar2 = *(int **)((long)pvVar8 + 0x28);
          pvVar6 = *(void **)((long)pvVar8 + 0x10);
          if (piVar2 != (int *)0x0) {
            LOCK();
            *piVar2 = *piVar2 + -1;
            UNLOCK();
            if (*piVar2 == 0) {
              QArrayData::deallocate(*(QArrayData **)((long)pvVar8 + 0x28),2,0x10);
            }
          }
          operator_delete(pvVar8,0x40);
          pvVar8 = pvVar6;
        }
        piVar2 = *(int **)((long)pvVar9 + 0x28);
        pvVar8 = *(void **)((long)pvVar9 + 0x10);
        if (piVar2 != (int *)0x0) {
          LOCK();
          *piVar2 = *piVar2 + -1;
          UNLOCK();
          if (*piVar2 == 0) {
            QArrayData::deallocate(*(QArrayData **)((long)pvVar9 + 0x28),2,0x10);
          }
        }
        operator_delete(pvVar9,0x40);
        pvVar9 = pvVar8;
      }
      piVar2 = *(int **)((long)pvVar10 + 0x28);
      pvVar9 = *(void **)((long)pvVar10 + 0x10);
      if (piVar2 != (int *)0x0) {
        LOCK();
        *piVar2 = *piVar2 + -1;
        UNLOCK();
        if (*piVar2 == 0) {
          QArrayData::deallocate(*(QArrayData **)((long)pvVar10 + 0x28),2,0x10);
        }
      }
      operator_delete(pvVar10,0x40);
      pvVar10 = pvVar9;
    }
    piVar2 = *(int **)((long)param_1 + 0x28);
    pvVar10 = *(void **)((long)param_1 + 0x10);
    if (piVar2 != (int *)0x0) {
      LOCK();
      *piVar2 = *piVar2 + -1;
      UNLOCK();
      if (*piVar2 == 0) {
        QArrayData::deallocate(*(QArrayData **)((long)param_1 + 0x28),2,0x10);
      }
    }
    operator_delete(param_1,0x40);
    param_1 = pvVar10;
  }
  return;
}

