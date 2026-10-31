// CustomGameConfig + GeneralOptsConfig — settings pages
// Reconstructed from FUN_001128c0..FUN_00113210 (custom page), FUN_00112820
// (validators), FUN_001126a0 / FUN_00112740 (apply), FUN_00111a60 / FUN_00111580 /
// FUN_00111290 (general page). Confidence: A for widgets/strings, B/C structure.
//
// Proven:
//   C1 fields: i18n("Width:"), i18n("Height:"), i18n("Mines:") each QSpinBox
//      with QIntValidator binding (0x112c5c-0x112e74)
//   C2 apply: Settings::setCustomWidth/Height/Mines (0x112b64-0x112bae)
//   C3 defaults 10/10/20, min 5/5/1 (see config-schema.md)
//   G1 checkboxes: i18n("Inside game") -> AllowKminesReset;
//      i18n("Use \'?\' marks") -> UseQuestionMarks (0x1116a3/0x1116f3)
//   G2 KComboBox bound to PlaceFlagOn enum ("Released"/"Pressed")
//   G3 explore checkbox -> ExploreWithLeftClickOnNumberCells;
//      disable-score checkbox -> DisableScoreOnReset

// --- CustomGameConfig (KConfigDialogPage with 3 spin boxes)
CustomGameConfig::CustomGameConfig(QWidget *parent) : KConfigDialogPage(parent)
{
    auto *widthSpin  = addRow(i18n("Width:"));     // bound CustomWidth,  min 5,  def 10
    auto *heightSpin = addRow(i18n("Height:"));    // bound CustomHeight, min 5,  def 10
    auto *minesSpin  = addRow(i18n("Mines:"));     // bound CustomMines,  min 1,  def 20
    // FUN_00112820: KRestrictedNumericValidator per field (width*height bound check
    // enforced by updateWidgets(); max mines < width*height)
}

void CustomGameConfig::apply()
{
    // FUN_001126a0 / FUN_00112740
    Settings::setCustomWidth(widthSpin->value());
    Settings::setCustomHeight(heightSpin->value());
    Settings::setCustomMines(minesSpin->value());
    // then: if current difficulty == KGCustomLevel -> scene->setCustomField(...)
}

// --- GeneralOptsConfig (KConfigDialogPage)
GeneralOptsConfig::GeneralOptsConfig(QWidget *parent) : KConfigDialogPage(parent)
{
    addCheckBox(i18n("Inside game"), Settings::allowKminesReset());      // G1
    addCheckBox(i18n("Use '?' marks"), Settings::useQuestionMarks());    // G1
    addComboBox(i18n("Place flag on:"), Settings::placeFlagOn());        // G2
    addCheckBox(i18n("Explore with left click on number cells"),
                Settings::exploreWithLeftClickOnNumberCells());          // G3
    addCheckBox(i18n("Disable score on reset"),
                Settings::disableScoreOnReset());                        // G3
}
