#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "constants/texts.h"
#include "jester_headers/menu_commands.h"
#include "bmmenu.h"

#define MenuItemsEnd {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/* Placement in this struct determins priority order */

const struct MenuItemDef gUnitActionMenuItemsRework[] = {
   // {"　脱出", TILE_COMMAND_TEXT_ESCAPE, R_TEXT_COMMAND_ESCAPE, TEXT_COLOR_SYSTEM_WHITE, 0x6D, EscapeCommandUsability, 0, EscapeCommandEffect, 0, 0, 0}, // Escape > 
    {"　制圧", 0x67A, 0x6CC, 0, 0x4E, UnitActionMenu_CanSeize, 0, UnitActionMenu_Seize, 0, 0, 0}, // Seize
    {"　攻撃", 0x67B, 0x6C0, 0, 0x4F, AttackCommandUsability, 0, UnitActionMenu_Attack, 0, DisplayUnitStandingAttackRange, HideMoveRangeGraphicsWrapper}, // Attack >
    {"　攻撃", 0x67B, 0x6C0, 0, 0x50, AttackBallistaCommandUsability, 0, UnitActionMenu_Attack, 0, DisplayUnitStandingAttackRange, HideMoveRangeGraphicsWrapper}, // Attack w/Ballista >

    /* Combat Art */
    {
        "　戰技",
        MSG_COMBATART_UM_NAME,
        MSG_COMBATART_UM_DESC,
        TEXT_COLOR_SYSTEM_WHITE,
        0x6C,
        CombatArtActionCommandUsability,
        CombatArtActionCommandOnDarw,
        CombatArtActionCommandEffect,
        NULL,
        CombatArtActionCommandHover,
        CombatArtActionCommandUnhover
    },

    {"　杖", 0x67C, 0x6C1, 0, 0x51, StaffCommandUsability, 0, StaffCommandEffect, 0, StaffCommandRange, HideMoveRangeGraphicsWrapper2}, // Staff
    {"　乗る", 0x691, 0x6D6, 4, 0x52, RideCommandUsability, 0, RideCommandEffect, 0, 0, 0}, // Ride (Ballista) >
    {"　降りる", 0x692, 0x6D7, 4, 0x53, ExitCommandUsability, 0, ExitCommandEffect, 0, 0, 0}, // Exit (Ballista) >

#if CHAX
    {
        "　特技",
        0x4EC,
        0,
        TEXT_COLOR_SYSTEM_WHITE,
        0x54,
        MenuSkills_Usability,
        MenuSkills_OnDraw,
        MenuSkills_OnSelected,
        MenuSkills_Idle,
        MenuSkills_Hover,
        MenuSkills_Unhover
    },
    {
        "　特技",
        0x4EC,
        1,
        TEXT_COLOR_SYSTEM_WHITE,
        0x55,
        MenuSkills_Usability,
        MenuSkills_OnDraw,
        MenuSkills_OnSelected,
        MenuSkills_Idle,
        MenuSkills_Hover,
        MenuSkills_Unhover
    },
    {
        "　特技",
        0x4EC,
        2,
        TEXT_COLOR_SYSTEM_WHITE,
        0x56,
        MenuSkills_Usability,
        MenuSkills_OnDraw,
        MenuSkills_OnSelected,
        MenuSkills_Idle,
        MenuSkills_Hover,
        MenuSkills_Unhover
    },
    {
        "　特技",
        0x4EC,
        3,
        TEXT_COLOR_SYSTEM_WHITE,
        0x57,
        MenuSkills_Usability,
        MenuSkills_OnDraw,
        MenuSkills_OnSelected,
        MenuSkills_Idle,
        MenuSkills_Hover,
        MenuSkills_Unhover
    },
#else
    {"　奏でる", 0x67D, 0x6C3, TEXT_COLOR_SYSTEM_WHITE, 0x54, PlayCommandUsability, 0, PlayCommandEffect, 0, 0, 0}, // Play >
    {"　踊る", 0x67E, 0x6C2, TEXT_COLOR_SYSTEM_WHITE, 0x55, DanceCommandUsability, 0, PlayCommandEffect, 0, 0, 0}, // Dance
    {"　盗む", 0x67F, 0x6C4, TEXT_COLOR_SYSTEM_WHITE, 0x56, StealCommandUsability, 0, StealCommandEffect, 0, 0, 0}, // Steal
    {"　召喚", 0x693, 0x6DD, TEXT_COLOR_SYSTEM_WHITE, 0x57, SummonCommandUsability, 0, SummonCommandEffect, 0, 0, 0}, // Summon (Shoukanshi) >
    {"　呼魔", 0x693, 0x6DD, TEXT_COLOR_SYSTEM_WHITE, 0x58, YobimaCommandUsability, 0, YobimaCommandEffect, 0, 0, 0}, // Summon (Deamon King) >
    {"　かぎ開", 0x694, 0x6DE, TEXT_COLOR_SYSTEM_WHITE, 0x59, PickCommandUsability, 0, PickCommandEffect, 0, 0, 0}, // Pick >
#endif

    {"　話す", 0x680, 0x6C9, TEXT_COLOR_SYSTEM_WHITE, 0x5A, TalkCommandUsability, 0, TalkCommandEffect, 0, 0, 0}, // Talk >
    {"　支援", 0x681, 0x6CA, TEXT_COLOR_SYSTEM_WHITE, 0x5B, SupportCommandUsability, 0, SupportCommandEffect, 0, 0, 0}, // Support
    {"　訪問", 0x682, 0x6CB, TEXT_COLOR_SYSTEM_WHITE, 0x5C, VisitCommandUsability, 0, VisitCommandEffect, 0, 0, 0}, // Visit
    {"　宝箱", 0x683, 0x6CE, TEXT_COLOR_SYSTEM_WHITE, 0x5D, ChestCommandUsability, 0, ChestCommandEffect, 0, 0, 0}, // Chest
    {"　扉", 0x684, 0x6CD, TEXT_COLOR_SYSTEM_WHITE, 0x5E, DoorCommandUsability, 0, DoorCommandEffect, 0, 0, 0}, // Door >
    {"　武器屋", 0x685, 0x6CF, TEXT_COLOR_SYSTEM_WHITE, 0x5F, ArmoryCommandUsability, 0, ArmoryCommandEffect, 0, 0, 0}, //Armory > 
    {"　道具屋", 0x686, 0x6D0, TEXT_COLOR_SYSTEM_WHITE, 0x60, VendorCommandUsability, 0, VendorCommandEffect, 0, 0, 0}, //Vendor > 
    {"　秘密店", 0x687, 0x6D1, TEXT_COLOR_SYSTEM_WHITE, 0x61, SecretShopCommandUsability, 0, SecretShopCommandEffect, 0, 0, 0}, //SecretShop > 
    {"　闘技場", 0x688, 0x6D2, TEXT_COLOR_SYSTEM_WHITE, 0x62, ArenaCommandUsability, 0, ArenaCommandEffect, 0, 0, 0}, // Arena
    {"　救出", 0x689, 0x6C5, TEXT_COLOR_SYSTEM_WHITE, 0x63, RescueUsability, 0, RescueEffect, 0, 0, 0}, // Rescue > 
#ifdef CONFIG_REFUGE_FEATURE
    {"　", MSG_MenuCommand_Refuge_NAME, MSG_MenuCommand_Refuge_DESC, TEXT_COLOR_SYSTEM_WHITE, 0x6C, Refuge_Usability, 0, Refuge_OnSelected, 0, 0, 0}, // Refuge > 
#endif
#ifdef CONFIG_LAGUZ_BARS
    {"　", MSG_MenuCommand_Transform_Laguz_NAME, MSG_MenuCommand_Transform_Laguz_DESC, TEXT_COLOR_SYSTEM_GOLD, 0x6C, Transform_Laguz_Usability, 0, Transform_Laguz_Effect, 0, 0, 0}, // Laguz Transform > 
#endif
    {"　降ろす", 0x68A, 0x6C6, TEXT_COLOR_SYSTEM_WHITE, 0x64, DropUsability, 0, DropEffect, 0, 0, 0}, // Drop > 
    {"　引受け", 0x68B, 0x6C8, TEXT_COLOR_SYSTEM_GREEN, 0x65, TakeUsability, 0, TakeEffect, 0, 0, 0}, // Take > 
    {"　引渡し", 0x68C, 0x6C7, TEXT_COLOR_SYSTEM_GREEN, 0x66, GiveUsability, 0, GiveEffect, 0, 0, 0}, // Give > 
    {"　持ち物", 0x68D, 0x6D3, TEXT_COLOR_SYSTEM_WHITE, 0x67, ItemCommandUsability, 0, ItemCommandEffect, 0, 0, 0}, // Item > 
    {"　交換", 0x68E, 0x6D4, TEXT_COLOR_SYSTEM_GREEN, 0x68, ItemSubMenu_IsTradeAvailable, 0, TradeCommandEffect, 0, 0, 0}, // Trade > 
    {"　輸送隊", 0x68F, 0x6D5, TEXT_COLOR_SYSTEM_GREEN, 0x69, SupplyUsability, 0, SupplyCommandEffect, 0, 0, 0}, // Supply > 
    {"　待機", 0x695, 0x6BF, TEXT_COLOR_SYSTEM_WHITE, 0x6B, MenuAlwaysEnabled, 0, EffectWait, 0, 0, 0}, // Wait > 

    MenuItemsEnd
};
