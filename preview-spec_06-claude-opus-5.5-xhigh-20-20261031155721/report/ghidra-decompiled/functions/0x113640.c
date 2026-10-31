// rea decompile_all
// name: FUN_00113640
// address: 0x113640
// body: available
// range: 0x113640-0x11365c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: ec02c82469e9dcd907026786823105d7493ccfea415575d85dad47773ce02c45
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x113658 to 0x10f330 <EXTERNAL>::KGameRenderedItem::~KGameRenderedItem (external=false, thunk=true, hasNoReturn=false); fallthrough is excluded by Ghidra's flow model. The reason for the terminal flow is not established by this observation.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00113640(KGameRenderedItem *param_1)

{
  *(undefined ***)param_1 = &PTR_FUN_001281e8;
  *(undefined ***)(param_1 + 0x10) = &PTR_FUN_00128328;
  KGameRenderedItem::~KGameRenderedItem(param_1);
  return;
}

