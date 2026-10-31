// rea decompile_all
// name: <EXTERNAL>::KCoreConfigSkeleton::ItemEnum::ItemEnum
// address: 0x10eee0
// body: available
// range: 0x10eee0-0x10eee9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x10eee4 to 0x12a0e0 <EXTERNAL>::KCoreConfigSkeleton::ItemEnum::ItemEnum (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __thiscall
KCoreConfigSkeleton::ItemEnum::ItemEnum
          (ItemEnum *this,QString *param_1,QString *param_2,int *param_3,QList *param_4,int param_5)

{
  ItemEnum(this,param_1,param_2,param_3,param_4,param_5);
  return;
}

