// rea decompile_all
// name: FUN_00111450
// address: 0x111450
// body: available
// range: 0x111450-0x11155c inclusive
// range: 0x111560-0x1116a0 inclusive
// range: 0x1116a8-0x11173b inclusive
// range: 0x111740-0x1117ac inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00111450(undefined4 *param_1,long param_2)

{
  char cVar1;
  undefined8 *puVar2;
  byte bVar3;
  undefined8 uVar4;
  int *piVar5;
  undefined8 uVar6;
  ulong uVar7;
  ulong *puVar8;
  ulong *puVar9;
  long lVar10;
  long lVar11;
  void *pvVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  undefined4 *puVar16;
  ulong *puVar17;
  ulong uVar18;
  ulong uVar19;
  ulong uVar20;
  long lVar21;
  long lVar22;
  
  uVar4 = *(undefined8 *)(param_2 + 8);
  uVar14 = *(ulong *)(param_2 + 0x10);
  *param_1 = 1;
  *(undefined8 *)(param_1 + 2) = uVar4;
  *(ulong *)(param_1 + 4) = uVar14;
  uVar14 = uVar14 >> 7;
  *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_2 + 0x18);
  uVar7 = uVar14 * 0x90;
  uVar19 = uVar7 + 8;
  puVar8 = operator_new__(uVar19);
  *puVar8 = uVar14;
  puVar9 = puVar8 + 1;
  if (uVar14 != 0) {
    puVar17 = puVar9;
    uVar18 = uVar7;
    do {
      uVar20 = uVar18;
      if (uVar18 <= uVar19) {
        uVar20 = uVar19;
      }
      *(undefined2 *)(puVar17 + 0x11) = 0;
      puVar17[0x10] = 0;
      uVar15 = uVar20 - uVar18;
      uVar13 = uVar15;
      if (uVar15 <= uVar20) {
        uVar13 = uVar20;
      }
      lVar10 = __memset_chk(puVar17,0xff,0x80,uVar13 + (uVar18 - uVar20));
      uVar13 = uVar15 + 0x90;
      if (uVar15 + 0x90 < uVar20) {
        uVar13 = uVar20;
      }
      puVar17 = (ulong *)(lVar10 + 0x90);
      uVar18 = (uVar18 - uVar20) + -0x90 + uVar13;
    } while (puVar8 + uVar14 * 0x12 + 1 != puVar17);
    uVar19 = 0;
    *(ulong **)(param_1 + 8) = puVar9;
    do {
      lVar21 = 0;
      lVar10 = *(long *)(param_2 + 0x20) + uVar19;
      do {
        bVar3 = *(byte *)(lVar10 + lVar21);
        if (bVar3 != 0xff) {
          puVar16 = (undefined4 *)((ulong)bVar3 * 0x20 + *(long *)(lVar10 + 0x80));
          lVar22 = *(long *)(param_1 + 8) + uVar19;
          bVar3 = *(byte *)(lVar22 + 0x89);
          if (bVar3 == *(byte *)(lVar22 + 0x88)) {
            if (bVar3 == 0) {
              pvVar12 = operator_new__(0x600);
              cVar1 = '0';
              uVar18 = 0x30;
              uVar14 = 0;
            }
            else {
              uVar14 = (ulong)bVar3;
              if (bVar3 == 0x30) {
                pvVar12 = operator_new__(0xa00);
                cVar1 = 'P';
                uVar20 = 0xa00;
                uVar18 = 0x50;
              }
              else {
                uVar18 = uVar14 + 0x10;
                uVar20 = uVar18 * 0x20;
                pvVar12 = operator_new__(uVar20);
                cVar1 = bVar3 + 0x10;
              }
              __memcpy_chk(pvVar12,*(undefined8 *)(lVar22 + 0x80),uVar14 << 5,uVar20);
            }
            do {
              uVar20 = uVar14 + 1;
              *(char *)((long)pvVar12 + uVar14 * 0x20) = (char)uVar14 + '\x01';
              uVar14 = uVar20;
            } while (uVar20 < uVar18);
            if (*(void **)(lVar22 + 0x80) != (void *)0x0) {
              operator_delete__(*(void **)(lVar22 + 0x80));
            }
            *(void **)(lVar22 + 0x80) = pvVar12;
            bVar3 = *(byte *)(lVar22 + 0x89);
            *(char *)(lVar22 + 0x88) = cVar1;
          }
          else {
            pvVar12 = *(void **)(lVar22 + 0x80);
          }
          lVar11 = (ulong)bVar3 * 0x20;
          *(undefined1 *)(lVar22 + 0x89) = *(undefined1 *)((long)pvVar12 + lVar11);
          *(byte *)(lVar22 + lVar21) = bVar3;
          uVar4 = *(undefined8 *)(puVar16 + 6);
          *(undefined4 *)((long)pvVar12 + lVar11) = *puVar16;
          piVar5 = *(int **)(puVar16 + 2);
          uVar6 = *(undefined8 *)(puVar16 + 4);
          *(undefined8 *)((long)pvVar12 + lVar11 + 0x18) = uVar4;
          puVar2 = (undefined8 *)((long)pvVar12 + lVar11 + 8);
          *puVar2 = piVar5;
          puVar2[1] = uVar6;
          if (piVar5 != (int *)0x0) {
            LOCK();
            *piVar5 = *piVar5 + 1;
            UNLOCK();
          }
        }
        lVar21 = lVar21 + 1;
      } while (lVar21 != 0x80);
      uVar19 = uVar19 + 0x90;
    } while (uVar7 != uVar19);
    return;
  }
  *(ulong **)(param_1 + 8) = puVar9;
  return;
}

