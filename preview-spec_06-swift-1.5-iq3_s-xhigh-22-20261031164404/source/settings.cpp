// Settings — KConfig skeleton
// Reconstructed from FUN_00113390..FUN_00116210. All keys, groups, defaults: proven A
// (see ../evidence/03-config.md).

#include "settings.h"

Settings::Settings()
    : KConfigSkeleton(QStringLiteral("kmines"))     // -> ~/.config/kminesrc
{
    KConfigGroup generalOpts(this, QStringLiteral("GeneralOptsConfig"));   // 0x2202d
    KConfigGroup customGame(this, QStringLiteral("CustomGameConfig"));     // 0x22116

    // --- GeneralOptsConfig (FUN_00115120, ItemBool, PLT 0xfb50)
    ItemBool(&m_useQuestionMarks,  QStringLiteral("UseQuestionMarks"),
             generalOpts, /*default=*/false);                    // proven default 0

    ItemBool(&m_allowKminesReset,  QStringLiteral("AllowKminesReset"),
             generalOpts, /*default=*/true);                     // proven default 1

    ItemBool(&m_disableScoreOnReset, QStringLiteral("DisableScoreOnReset"),
             generalOpts, /*default=*/true);                     // proven default 1

    ItemBool(&m_exploreOnNumbers,  QStringLiteral("ExploreWithLeftClickOnNumberCells"),
             generalOpts, /*default=*/true);                     // proven default 1

    // --- GeneralOptsConfig (FUN_00114c40, ItemEnum, PLT 0xfee0)
    ItemEnum<PlaceFlagOn>(&m_placeFlagOn, QStringLiteral("PlaceFlagOn"),
             generalOpts, PlaceFlag_Released,
             { { PlaceFlag_Released, i18n("Released") },         // enum value 0
               { PlaceFlag_Pressed,  i18n("Pressed") } });       // enum value 1

    // --- CustomGameConfig (FUN_00114e90/FUN_001153c0/FUN_00115650, ItemInt, PLT 0xfb90)
    ItemInt(&m_customWidth,  QStringLiteral("CustomWidth"),  customGame, 10)
        ->setMinValue(5)->setMaxValue(50);                       // PLT 0xf0d0 / 0xf430
    ItemInt(&m_customHeight, QStringLiteral("CustomHeight"), customGame, 10)
        ->setMinValue(5)->setMaxValue(50);
    ItemInt(&m_customMines,  QStringLiteral("CustomMines"),  customGame, 20)
        ->setMinValue(1);                                        // cmp 1 at validation
}
