#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "unit-expa.h"
#include "eventinfo.h"
#include "bmmenu.h"
#include "eventscript.h"
#include "EAstdlib.h"
#include "worldmap.h"
#include "constants/texts.h"
#include "jester_headers/event-call.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"
#include "jester_headers/miscellaenous.h"
#include "debuff.h"
#include "jester_headers/custom-structs.h"
#include "jester_headers/custom-functions.h"
#include "action-expa.h"
#include "bmusailment.h"
#include "vanilla.h"
#include "jester_headers/class-arrays.h"
#include "unitlistscreen.h"
#include "prepscreen.h"
#include "savemenu.h"
#include "rn.h"
#include "icon-rework.h"
#include "status-getter.h"
#include "mapanim.h"
#include "playst-expa.h"
#include "ending_details.h"
#include "uichapterstatus.h"
#include "jester_headers/Forging.h"

#ifndef CONFIG_INSTALL_CONVOYEXPA_AMT
    #define CONFIG_INSTALL_CONVOYEXPA_AMT 200
#endif

#if defined(SID_CatchEmAll) && (COMMON_SKILL_VALID(SID_CatchEmAll))
    const unsigned int gCatchEmAllId = SID_CatchEmAll;
#endif

#ifdef CONFIG_BEXP
    extern u16 sBEXP[CONFIG_BEXP];
#endif

// Misc Functions

extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

extern void GenerateFireTileTrapTargets(int x, int y, int damage);
extern void GenerateArrowTrapTargets(int x, int y, int damage);
extern void GenerateGasTrapTargets(int x, int y, int damage, int facing);

extern void PoisonDamageDisplay_Display(struct UnknownBMUSAilmentProc* proc);
extern void PoisonDamageDisplay_Next(struct UnknownBMUSAilmentProc* proc);

extern const u16 * Events_WM_Beginning[];
extern const u16 * Events_WM_ChapterIntro[];

typedef struct {
    /* 00 */ struct Font font;
    /* 18 */ struct Text th[18];
} PrepItemSuppyText;

struct ProcCmd CONST_DATA ProcScr_PrepItemScreen[] = {
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_Init),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_CALL(sub_8098CC0),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_SLEEP(0),

    PROC_GOTO(1),

PROC_LABEL(0),
    PROC_CALL(sub_8098CC0),

    // fallthrough

PROC_LABEL(1),
    PROC_REPEAT(sub_8099120),
    PROC_CALL(DisableAllGfx),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_StartStatScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_ResumeFromStatScreen),
    PROC_CALL(sub_8098CC0),
    PROC_SLEEP(0),

    PROC_CALL(EnableAllGfx),

    PROC_GOTO(1),

PROC_LABEL(2),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_SLEEP(0),

    PROC_CALL(sub_8099654),

    // fallthrough

PROC_LABEL(3),
    PROC_REPEAT(sub_80996E8),

    // fallthrough

PROC_LABEL(4),
    PROC_CALL(PrepItemScreen_HideFunds),
    PROC_CALL(sub_8099AF8),
    PROC_SLEEP(0),

    PROC_CALL(sub_8099C60),

    // fallthrough

PROC_LABEL(5),
    PROC_REPEAT(PrepItemScreen_Loop_MainKeyHandler),
    PROC_CALL(DisableAllGfx),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_StartStatScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_ResumeFromStatScreen),
    PROC_CALL(sub_8099AF8),
    PROC_CALL(sub_8099C60),
    PROC_SLEEP(0),

    PROC_CALL(EnableAllGfx),

    PROC_GOTO(5),

PROC_LABEL(6),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(StartPrepItemTradeScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),

    PROC_CALL(sub_8099AF8),
    PROC_CALL(sub_8099C60),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(5),

PROC_LABEL(8),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepItemListScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),

    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(9),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepItemUse),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),

    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(10),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepItemSupply),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(11),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepSell),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(12),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepArmory),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(13),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),

    PROC_CALL(sub_8099E68),

    PROC_END,

#ifdef CONFIG_BEXP
/* BEXP proc */
PROC_LABEL(14),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepSell),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),
#endif
};

// static struct PlayerInterfaceConfigEntry const sPlayerInterfaceConfigLut[4] = {
//     {
//         +1, +1,
//         -1, +1,
//         +1, -1,
//     },
//     {
//         -1, +1,
//         -1, -1,
//         +1, +1,
//     },
//     {
//         +1, +1,
//         -1, -1,
//         +1, -1,
//     },
//     {
//         -1, +1,
//         -1, -1,
//         +1, -1,
//     }
// };

extern void DrawHpBar(u16* buffer, struct Unit* unit, int tileBase);
extern void sub_808C360(struct PlayerInterfaceProc* proc, u16* buffer, struct Unit* unit);
extern void GetMinimugFactionPalette(int faction, int palId);
extern void DrawUnitDisplayHpOrStatus(struct PlayerInterfaceProc*, struct Unit*);

//! FE8U: 0x080B1F64
LYN_REPLACE_CHECK(SetGameOption);
void SetGameOption(u8 index, u8 newValue)
{

    switch (index)
    {
        case GAME_OPTION_ANIMATION:
            switch (newValue)
            {
                case 0:
                    gPlaySt.config.animationType = 0;
                    return;

                case 1:
                    gPlaySt.config.animationType = 3;
                    return;

                case 2:
                    gPlaySt.config.animationType = 1;
                    return;

                case 3:
                    gPlaySt.config.animationType = 2;
                    return;
            }

            // fallthrough

        case GAME_OPTION_TERRAIN:
            gPlaySt.config.disableTerrainDisplay = newValue;

            break;

        case GAME_OPTION_UNIT:
            gPlaySt.config.unitDisplayType = newValue;

            break;

        case GAME_OPTION_AUTOCURSOR:
            gPlaySt.config.autoCursor = newValue;

            break;

        case GAME_OPTION_TEXT_SPEED:
            gPlaySt.config.textSpeed = newValue;

            break;

        case GAME_OPTION_GAME_SPEED:
            gPlaySt.config.gameSpeed = newValue;

            break;

        case GAME_OPTION_MUSIC:
            gPlaySt.config.disableBgm = newValue;

            break;

        case GAME_OPTION_SOUND_EFFECTS:
            gPlaySt.config.disableSoundEffects = newValue;

            break;

        case GAME_OPTION_WINDOW_COLOR:
            gPlaySt.config.windowColor = newValue;

            break;

        case GAME_OPTION_COMBAT:
            gPlaySt.config.battleForecastType = newValue;

            break;

        case GAME_OPTION_SUBTITLE_HELP:
            gPlaySt.config.noSubtitleHelp = newValue;

            break;

        case GAME_OPTION_AUTOEND_TURNS:
            gPlaySt.config.disableAutoEndTurns = newValue;

            break;

        case GAME_OPTION_UNIT_COLOR:
            gPlaySt.config.unitColor = newValue;

            break;

        case GAME_OPTION_OBJECTIVE:
            gPlaySt.config.disableGoalDisplay = newValue;

            break;

        case GAME_OPTION_CONTROLLER:
            gPlaySt.config.controller = newValue;

            break;

        case GAME_OPTION_RANK_DISPLAY:
            gPlaySt.config.rankDisplay = newValue;

            break;
    }

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToTradeTargetList);
void TryAddUnitToTradeTargetList(struct Unit * unit)
{
    /**
     *  With capture, a unit should be able to trade with rescued enemies
     */
#if (defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture)))
    bool capture_active = false;

    if (SkillTester(gSubjectUnit, SID_Capture) && gSubjectUnit->rescue)
        if (!IsSameAllegiance(gSubjectUnit->index, gSubjectUnit->rescue))
            capture_active = true;
    
    if (!IsSameAllegiance(gSubjectUnit->index, unit->index) && !capture_active)
        return;

#else 
    if (!IsSameAllegiance(gSubjectUnit->index, unit->index))
        return;
#endif


    if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM)
        return;

    if (unit->statusIndex != UNIT_STATUS_BERSERK)
    {

        if (gSubjectUnit->items[0] != 0 || unit->items[0] != 0)
        {

            if (!(UNIT_CATTRIBUTES(unit) & CA_SUPPLY))
            {
                AddTarget(unit->xPos, unit->yPos, unit->index, 0);
            }
        }
    }

    if (unit->state & US_RESCUING)
    {
        struct Unit * rescue = GetUnit(unit->rescue);

        if (UNIT_FACTION(rescue) != FACTION_BLUE)
        {
            return;
        }

        if (gSubjectUnit->items[0] == 0 && rescue->items[0] == 0)
        {
            return;
        }

        AddTarget(unit->xPos, unit->yPos, rescue->index, 0);
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(MakeTradeTargetList);
void MakeTradeTargetList(struct Unit * unit)
{
    int x = unit->xPos;
    int y = unit->yPos;
    bool shotputSkill = false;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

#if defined(SID_ShotputPlus) && (COMMON_SKILL_VALID(SID_ShotputPlus))
    if (SkillTester(unit, SID_ShotputPlus))
    {
        shotputSkill = true;
        MapAddInRange(x, y, 5, 1);
        ForEachUnitInRange(TryAddUnitToTradeTargetList);
    }
#endif

#if defined(SID_Shotput) && (COMMON_SKILL_VALID(SID_Shotput))
    if (SkillTester(unit, SID_Shotput) && !shotputSkill)
    {
        shotputSkill = true;
        MapAddInRange(x, y, 3, 1);
        ForEachUnitInRange(TryAddUnitToTradeTargetList);
    }
#endif

    if (!shotputSkill)
    {
        ForEachAdjacentUnit(x, y, TryAddUnitToTradeTargetList);
    }

    if (gSubjectUnit->state & US_RESCUING)
    {
        int count = GetSelectTargetCount();
        TryAddUnitToTradeTargetList(GetUnit(gSubjectUnit->rescue));

        if (count != GetSelectTargetCount())
        {
            GetTarget(count)->x = gSubjectUnit->xPos;
            GetTarget(count)->y = gSubjectUnit->yPos;
        }
    }

    return;
}

//! FE8U = 0x08032728
LYN_REPLACE_CHECK(KillUnitOnCombatDeath);
void KillUnitOnCombatDeath(struct Unit * unitA, struct Unit * unitB)
{
    if (GetUnitCurrentHp(unitA) != 0)
    {
        return;
    }

#if (defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture)))
    if (SkillTester(unitB, SID_Capture) && CheckBitUES(unitB, UES_BIT_CAPTURE_SKILL_USED))
    {
        UnitRescue(unitB, unitA);
        HideUnitSprite(unitA);
        ClearBitUES(unitB, UES_BIT_CAPTURE_SKILL_USED);
        return;
    }
#endif

/* Can still be viewed in the stat screen, but eh it's fine. */
#if defined(SID_Casual) && (COMMON_SKILL_VALID(SID_Casual))
    if (SkillTester(unitA, SID_Casual))
    {
        unitA->state |= US_HIDDEN;
        return;
    }
#endif

#if defined(SID_DestinyBond) && (COMMON_SKILL_VALID(SID_DestinyBond))
    if (SkillTester(unitA, SID_DestinyBond))
        UnitKill(unitB);
#endif

    PidStatsRecordDefeatInfo(unitA->pCharacterData->number, unitB->pCharacterData->number, DEFEAT_CAUSE_COMBAT);

    UnitKill(unitA);

    return;
}

// LYN_REPLACE_CHECK(TryAddUnitToRescueTargetList);
// void TryAddUnitToRescueTargetList(struct Unit * unit)
// {

//     if (!AreUnitsAllied(gSubjectUnit->index, unit->index))
//     {
//         return;
//     }

//     if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM)
//     {
//         return;
//     }

//     if (unit->statusIndex == UNIT_STATUS_BERSERK)
//     {
//         return;
//     }

//     if (unit->state & (US_RESCUING | US_RESCUED))
//     {
//         return;
//     }

//     if (!CanUnitRescue(gSubjectUnit, unit))
//     {
//         return;
//     }

//     AddTarget(unit->xPos, unit->yPos, unit->index, 0);

//     return;
// }

LYN_REPLACE_CHECK(UnitDrop);
void UnitDrop(struct Unit * actor, int xTarget, int yTarget)
{
    struct Unit * target = GetUnit(actor->rescue);

    actor->state = actor->state & ~(US_RESCUING | US_RESCUED);
    target->state = target->state & ~(US_RESCUING | US_RESCUED | US_HIDDEN);

/* Let rescued units move after the rescuer dies */
#ifdef CONFIG_DEATH_DANCE
    if (UNIT_FACTION(target) == gPlaySt.faction && actor->curHP != 0)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED    
#else
    if (UNIT_FACTION(target) == gPlaySt.faction)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED
#endif

    actor->rescue = 0;
    target->rescue = 0;

    target->xPos = xTarget;
    target->yPos = yTarget;

    /* If we've captured an enemy, dropping them will kill them immediately */
    if (target->curHP == 0)
    {
        PidStatsRecordDefeatInfo(target->pCharacterData->number, actor->pCharacterData->number, DEFEAT_CAUSE_COMBAT);
        UnitKill(target);
    }
}

LYN_REPLACE_CHECK(UnitRescue);
void UnitRescue(struct Unit* actor, struct Unit* target) {
    actor->state  |= US_RESCUING;
    target->state |= US_RESCUED | US_HIDDEN;

    actor->rescue = target->index;
    target->rescue = actor->index;

    target->xPos = actor->xPos;
    target->yPos = actor->yPos;
}


// use vanilla version so we don't lag by using hooked versions that accounts for pass etc
s8 Vanilla_CanUnitCrossTerrain(struct Unit * unit, int terrain)
{
    const s8 * lookup = (s8 *)GetUnitMovementCost(unit);
    return (lookup[terrain] > 0) ? TRUE : FALSE;
}

bool Generic_CanUnitBeOnPos(struct Unit * unit, s8 x, s8 y, int x2, int y2)
{
    if (x < 0 || y < 0)
        return 0; // position out of bounds
    if (x >= gBmMapSize.x || y >= gBmMapSize.y)
        return 0; // position out of bounds
    if (gBmMapUnit[y][x])
        return 0;
    if (gBmMapHidden[y][x] & 1)
        return 0; // a hidden unit is occupying this position
    if ((x2 == x) && (y2 == y))
        return 0;                                                  // exception / a battle unit is on this tile
    return Vanilla_CanUnitCrossTerrain(unit, gBmMapTerrain[y][x]); // CanUnitCrossTerrain(unit, gMapTerrain[y][x]);
}

//! FE8U = 0x0801538C
LYN_REPLACE_CHECK(SwitchPhases);
void SwitchPhases(void)
{
    switch (gPlaySt.faction)
    {
        case FACTION_BLUE:

            /**
             * There's probably a more efficient way to do this,
             * but this is all I've found to work right now.
             * I change back the unit faction for a 'turncoat' unit
             * if they haven't moved after switching factions initially.
             */
            for (int uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
            {
                struct Unit * unit = GetUnit(uid);

                if (CheckBitUES(unit, UES_BIT_CHANGED_FACTIONS))
                    UnitChangeFaction(unit, FACTION_RED);
            }
            gPlaySt.faction = FACTION_RED;

            break;

        case FACTION_RED:
            gPlaySt.faction = FACTION_GREEN;

            for (int uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
            {
                struct Unit * unit = GetUnit(uid);

                if (CheckBitUES(unit, UES_BIT_CHANGED_FACTIONS))
                    UnitChangeFaction(unit, FACTION_BLUE);
            }
            break;

        case FACTION_GREEN:
            gPlaySt.faction = FACTION_BLUE;

            for (int uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
            {
                struct Unit * unit = GetUnit(uid);

                if (CheckBitUES(unit, UES_BIT_CHANGED_FACTIONS))
                    UnitChangeFaction(unit, FACTION_RED);
            }

            if (gPlaySt.chapterTurnNumber < 999)
                gPlaySt.chapterTurnNumber++;

            if (gPlaySt.chapterTurnNumber % 2 == 0)
                PlayStExpa_SetBit(PLAYSTEXPA_BIT_AbsorbAlternation_InForce);
            else
                PlayStExpa_ClearBit(PLAYSTEXPA_BIT_AbsorbAlternation_InForce);

            ProcessTurnSupportExp();
    }
}

struct EvCheck0A
{
    u32 unk0;
    u32 script;
    u8 x;
    u8 y;
    u16 tileCommand;
};

//! FE8U = 0x08083A58
LYN_REPLACE_CHECK(EvCheck0A_SHOP);
int EvCheck0A_SHOP(struct EventInfo * info)
{
    struct EvCheck0A * listScript = (void *)info->listScript;

    int x = listScript->x;
    int y = listScript->y;

    int tileCommand = listScript->tileCommand;

#if defined(SID_Secret) && (COMMON_SKILL_VALID(SID_Secret))
    if (SkillTester(gActiveUnit, SID_Secret))
    {
        info->script = listScript->script;
        info->flag = listScript->unk0 >> 16;
        info->commandId = tileCommand;
        return 1;
    }
#endif

    if ((x == info->xPos) && (y == info->yPos))
    {
        if ((tileCommand != TILE_COMMAND_SECRET || (GetUnitItemSlot(gActiveUnit, ITEM_MEMBERCARD) != -1)))
        {
            info->script = listScript->script;
            info->flag = listScript->unk0 >> 16;
            info->commandId = tileCommand;
            return 1;
        }
    }

    return 0;
}

LYN_REPLACE_CHECK(BeginUnitPoisonDamageAnim);
void BeginUnitPoisonDamageAnim(struct Unit * unit, int damage)
{

#if (defined(SID_PoisonHeal) && COMMON_SKILL_VALID(SID_PoisonHeal))
    if (SkillTester(gActiveUnit, SID_PoisonHeal))
    {
        BeginUnitHealAnim(gActiveUnit, damage);
        AddUnitHp(gActiveUnit, damage * 2); // A quick fix for poison damage applying, double the damage and heal it
        return;
    }
#endif

    BattleInitItemEffect(unit, -1);

    AddUnitHp(&gBattleActor.unit, -damage);

    // if (gBattleActor.unit.curHP < 0)
    // {
    //     gBattleActor.unit.curHP = 0;
    // }

    gBattleHitIterator->hpChange = gBattleActor.hpInitial - gBattleActor.unit.curHP;

    if (gBattleActor.unit.curHP == 0)
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
    }

    BattleHitTerminate();

    BeginMapAnimForPoisonDmg();

    RenderMapForFogFadeIfUnitDied(unit);

    return;
}

LYN_REPLACE_CHECK(HasBattleUnitGainedWeaponLevel);
s8 HasBattleUnitGainedWeaponLevel(struct BattleUnit * bu)
{

#if (defined(SID_ShadowGiftPlus) && (COMMON_SKILL_VALID(SID_ShadowGiftPlus)))
    if (BattleSkillTester(bu, SID_ShadowGiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_DARK)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_DARK] == 0)
                return false;
#endif

#if (defined(SID_ShadowGift) && (COMMON_SKILL_VALID(SID_ShadowGift)))
    if (BattleSkillTester(bu, SID_ShadowGift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_DARK)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_DARK] == 0)
                return false;
#endif

#if (defined(SID_LightGiftPlus) && (COMMON_SKILL_VALID(SID_LightGiftPlus)))
    if (BattleSkillTester(bu, SID_LightGiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LIGHT)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LIGHT] == 0)
                return false;
#endif

#if (defined(SID_StormgiftPlus) && (COMMON_SKILL_VALID(SID_StormgiftPlus)))
    if (BattleSkillTester(bu, SID_StormgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_ANIMA)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_ANIMA] == 0)
                return false;
#endif

#if (defined(SID_Stormgift) && (COMMON_SKILL_VALID(SID_Stormgift)))
    if (BattleSkillTester(bu, SID_Stormgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_ANIMA)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_ANIMA] == 0)
                return false;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (BattleSkillTester(bu, SID_Gracegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_GracegiftPlus) && (COMMON_SKILL_VALID(SID_GracegiftPlus)))
    if (BattleSkillTester(bu, SID_GracegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (BattleSkillTester(bu, SID_Gracegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_BladegiftPlus) && (COMMON_SKILL_VALID(SID_BladegiftPlus)))
    if (BattleSkillTester(bu, SID_BladegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_SWORD)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_SWORD] == 0)
                return false;
#endif

#if (defined(SID_Bladegift) && (COMMON_SKILL_VALID(SID_Bladegift)))
    if (BattleSkillTester(bu, SID_Bladegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_SWORD)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_SWORD] == 0)
                return false;
#endif

#if (defined(SID_PiercegiftPlus) && (COMMON_SKILL_VALID(SID_PiercegiftPlus)))
    if (BattleSkillTester(bu, SID_PiercegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LANCE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LANCE] == 0)
                return false;
#endif

#if (defined(SID_Piercegift) && (COMMON_SKILL_VALID(SID_Piercegift)))
    if (BattleSkillTester(bu, SID_Piercegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LANCE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LANCE] == 0)
                return false;
#endif

#if (defined(SID_HackgiftPlus) && (COMMON_SKILL_VALID(SID_HackgiftPlus)))
    if (BattleSkillTester(bu, SID_HackgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_AXE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_AXE] == 0)
                return false;
#endif

#if (defined(SID_Hackgift) && (COMMON_SKILL_VALID(SID_Hackgift)))
    if (BattleSkillTester(bu, SID_Hackgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_AXE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_AXE] == 0)
                return false;
#endif

#if (defined(SID_ArcgiftPlus) && (COMMON_SKILL_VALID(SID_ArcgiftPlus)))
    if (BattleSkillTester(bu, SID_ArcgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_BOW)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_BOW] == 0)
                return false;
#endif

#if (defined(SID_Arcgift) && (COMMON_SKILL_VALID(SID_Arcgift)))
    if (BattleSkillTester(bu, SID_Arcgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_BOW)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_BOW] == 0)
                return false;
#endif

    int oldWexp = bu->unit.ranks[bu->weaponType];
    int newWexp = GetBattleUnitUpdatedWeaponExp(bu);

    if (newWexp < 0)
        return FALSE;

    return GetWeaponLevelFromExp(oldWexp) != GetWeaponLevelFromExp(newWexp);
}

LYN_REPLACE_CHECK(GenerateFireTileTrapTargets);
void GenerateFireTileTrapTargets(int x, int y, int damage)
{
    FORCE_DECLARE bool dontAddTarget = false;

#if (defined(SID_Absolve) && (COMMON_SKILL_VALID(SID_Absolve)))
    if (SkillTester(GetUnit(gBmMapUnit[y][x]), SID_Absolve))
        dontAddTarget = true;
#endif

    if (!dontAddTarget)
        AddTarget(x, y, gBmMapUnit[y][x], damage);
}

LYN_REPLACE_CHECK(GenerateArrowTrapTargets);
void GenerateArrowTrapTargets(int x, int y, int damage)
{
    FORCE_DECLARE bool dontAddTarget = false;
    int iy;

    for (iy = 0; iy < gBmMapSize.y; ++iy)
    {
        if (gBmMapUnit[iy][x])
        {
#if (defined(SID_Absolve) && (COMMON_SKILL_VALID(SID_Absolve)))
            if (SkillTester(GetUnit(gBmMapUnit[iy][x]), SID_Absolve))
                dontAddTarget = true;
#endif
            if (!dontAddTarget)
                AddTarget(x, iy, gBmMapUnit[iy][x], damage);
        }
    }
}

LYN_REPLACE_CHECK(GenerateGasTrapTargets);
void GenerateGasTrapTargets(int x, int y, int damage, int facing)
{
    FORCE_DECLARE bool dontAddTarget = false;

    int i;

    int xInc = 0;
    int yInc = 0;

    switch (facing)
    {

        case FACING_UP:
            xInc = 0;
            yInc = -1;

            break;

        case FACING_DOWN:
            xInc = 0;
            yInc = +1;

            break;

        case FACING_LEFT:
            xInc = -1;
            yInc = 0;

            break;

        case FACING_RIGHT:
            xInc = +1;
            yInc = 0;

            break;

    } // switch (facing)

    for (i = 2; i >= 0; --i)
    {
        x += xInc;
        y += yInc;

        if (gBmMapUnit[y][x])
        {
#if (defined(SID_Absolve) && (COMMON_SKILL_VALID(SID_Absolve)))
            if (SkillTester(GetUnit(gBmMapUnit[y][x]), SID_Absolve))
                dontAddTarget = true;
#endif
            if (!dontAddTarget)
                AddTarget(x, y, gBmMapUnit[y][x], damage);
        }
    }
}

bool isWeaponTriangleAdvantage(int attackerWeapon, int defenderWeapon)
{
    if (attackerWeapon == ITYPE_SWORD && defenderWeapon == ITYPE_AXE)
        return true;
    if (attackerWeapon == ITYPE_LANCE && defenderWeapon == ITYPE_SWORD)
        return true;
    if (attackerWeapon == ITYPE_AXE && defenderWeapon == ITYPE_LANCE)
        return true;
    if (attackerWeapon == ITYPE_ANIMA && defenderWeapon == ITYPE_LIGHT)
        return true;
    if (attackerWeapon == ITYPE_LIGHT && defenderWeapon == ITYPE_DARK)
        return true;
    if (attackerWeapon == ITYPE_DARK && defenderWeapon == ITYPE_ANIMA)
        return true;

    return false;
}

bool weaponHasSpecialEffect(int weaponAttributes)
{
    if (weaponAttributes & IA_NEGATE_DEFENSE)
        return true;
    if (weaponAttributes & IA_NEGATE_CRIT)
        return true;
    if (weaponAttributes & IA_NEGATE_FLYING)
        return true;
    if (weaponAttributes & IA_REVERTTRIANGLE)
        return true;
    if (weaponAttributes & IA_UNCOUNTERABLE)
        return true;
    if (weaponAttributes & IA_BRAVE)
        return true;
    if (weaponAttributes & IA_UNBREAKABLE)
        return true;
    
    return false;
}

int findMax(u8 *array, int size) {
    u8 max = 0;
    FORCE_DECLARE u8 array_position = 0;

    for (int i = 0; i < size - 1; i++) {
        if (array[i] > max) {
            max = array[i];
            array_position = i;
        }
    }
    return array_position;
}

u8 EscapeCommandUsability(const struct MenuItemDef* def, int number) {

    if (gActiveUnit->pClassData->number == CLASS_PHANTOM) {
        return MENU_NOTSHOWN;
    }

    if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }

    return GetAvailableTileEventCommand(gActiveUnit->xPos, gActiveUnit->yPos) == TILE_COMMAND_MAP_ESCAPE
    ? MENU_ENABLED : MENU_NOTSHOWN;
}

u8 EscapeCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {

    StartAvailableTileEvent(gActiveUnit->xPos, gActiveUnit->yPos);
    
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

LYN_REPLACE_CHECK(Event19_Checks);
u8 Event19_Checks(struct EventEngineProc * proc)
{
    u8 subcode = EVT_SUB_CMD(proc->pEventCurrent);
    switch (subcode) {
    case EVSUBCMD_CHECK_MODE: // Check Mode
        gEventSlots[0xC] = gPlaySt.chapterModeIndex;
        break;

    case EVSUBCMD_CHECK_CHAPTER_NUMBER: // Check (Next?) Chapter Index
        gEventSlots[0xC] = proc->chapterIndex;
        break;

    case EVSUBCMD_CHECK_HARD: // Check Difficult Mode
        if (!(gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
            gEventSlots[0xC] = FALSE;
        else
            gEventSlots[0xC] = TRUE;

        break;

    case EVSUBCMD_CHECK_TURNS: // Check Turn Number
        gEventSlots[0xC] = gPlaySt.chapterTurnNumber;
        break;

    case EVSUBCMD_CHECK_ENEMIES: // Check Red Unit Count
        gEventSlots[0xC] = CountRedUnits();
        break;

    case EVSUBCMD_CHECK_OTHERS: // Check Green Unit Count
        gEventSlots[0xC] = CountGreenUnits();
        break;

    case EVSUBCMD_CHECK_PLAYERS: // Check Blue Unit Count
        gEventSlots[0xC] = CountAvailableBlueUnits();
        break;

    case EVSUBCMD_CHECK_SKIRMISH: // Check Chapter Type?
        gEventSlots[0xC] = GetBattleMapKind();
        break;

    case EVSUBCMD_CHECK_TUTORIAL: // Check Some option or difficult mode
        if (gPlaySt.config.controller || (gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
            gEventSlots[0xC] = FALSE;
        else
            gEventSlots[0xC] = TRUE;

        break;

    case EVSUBCMD_CHECK_MONEY: // Check gold
        gEventSlots[0xC] = GetPartyGoldAmount();
        break;

    case EVSUBCMD_19CHECK_EVENTID: // Check Event Trigger ID
        gEventSlots[0xC] = GetEventTriggerId(proc->pEventStart);
        break;

    case EVSUBCMD_CHECK_POSTGAME: // Check Game Complete
        if (gPlaySt.chapterStateBits & PLAY_FLAG_COMPLETE)
            gEventSlots[0xC] = TRUE;
        else
            gEventSlots[0xC] = FALSE;

        break;

    default:
        break;
    } // switch (subcode)

    return EVC_ADVANCE_CONTINUE;
}

#ifdef CONFIG_FE8_REWRITE
LYN_REPLACE_CHECK(WorldMap_CallBeginningEvent);
//! FE8U = 0x080BA334
void WorldMap_CallBeginningEvent(struct WorldMapMainProc * proc)
{
    int chIndex;
    int node_next;

    Sound_FadeOutBGM(4);

    if ((gGMData.state.bits.monster_merged) || (gPlaySt.chapterStateBits & PLAY_FLAG_POSTGAME))
    {
        sub_80BA008(proc->timer);
    }
    else
    {
        int loc = gGMData.units[0].location;

        gGMData.current_node = loc;
        node_next = WMLoc_GetNextLocId(loc);

        if (node_next > -1)
        {
            chIndex = WMLoc_GetChapterId(node_next);

            gPlaySt.chapterIndex = chIndex;
            

            if (Events_WM_Beginning[GetROMChapterStruct(chIndex)->gmapEventId] == NULL)
                return;

            ResetGmStoryNode();
            proc->gm_icon->merge_next_node = false;

            /**
             * JESTER - I've resorted to hooking into the WM call function to directly load the
             * WM events I want based on the supplied eventSCR. It's an unfortunate bit of hardcoding
             * I'm looking to remove, but it frees me from having to rely on the list in ASM in vanilla.
             */
            int eventID = GetROMChapterStruct(chIndex)->gmapEventId;

            // NoCashGBAPrintf("SET event id is: %d", eventID);
            
            switch (eventID) {
            case 55:
                CallEvent((const u16 *)EventScrWM_Ch1_ENDING, 0);
                break;
            case 1:
                CallEvent((const u16 *)EventScrWM_Prologue_SET_NODE, 0);
                break;
            case 2:
                break;
            case 3:
                CallEvent((const u16 *)EventScrWM_Ch2_SET_NODE, 0);
                break;
            case 4:
                CallEvent((const u16 *)EventScrWM_Ch3_SET_NODE, 0);
                break;
            case 5:
                CallEvent((const u16 *)EventScrWM_Ch4_SET_NODE, 0);
                break;
            case 6:
                break;
            case 7:
                CallEvent((const u16 *)EventScrWM_Ch5_SET_NODE, 0);
                break;
            case 8:
                CallEvent((const u16 *)EventScrWM_Ch6_SET_NODE, 0);
                break;
            case 9:
                CallEvent((const u16 *)EventScrWM_Ch7_SET_NODE, 0);
                break;
            case 10:
                CallEvent((const u16 *)EventScrWM_Ch8_SET_NODE, 0);
                break;
            case 11:
                CallEvent((const u16 *)EventScrWM_Ch9_SET_NODE, 0);
                break;
            case 12:
                CallEvent((const u16 *)EventScrWM_Ch10_SET_NODE, 0);
                break;
            default: 
                CallEvent(Events_WM_Beginning[eventID], 0);
                break;
            }
        }
    }

    StartWMFaceCtrl(proc);
    StartGmapMuEntry(NULL);
}

LYN_REPLACE_CHECK(CallChapterWMIntroEvents);
//! FE8U = 0x080BA3D4
void CallChapterWMIntroEvents(ProcPtr proc)
{
    if (Events_WM_ChapterIntro[GetROMChapterStruct(gPlaySt.chapterIndex)->gmapEventId] != NULL)
    {
         /**
        * JESTER - I've resorted to hooking into the WM call function to directly load the
        * WM events I want based on the supplied eventSCR. It's an unfortunate bit of hardcoding
        * I'm looking to remove, but it frees me from having to rely on the list in ASM in vanilla.
        */
        int eventID = GetROMChapterStruct(gPlaySt.chapterIndex)->gmapEventId;

        // NoCashGBAPrintf("TRAVEL event id is: %d", eventID);
            
        switch (eventID) {
        case 55:
            break;
        case 1:
            CallEvent((const u16 *)EventScrWM_Prologue_TRAVEL_TO_NODE, 0);
            break;
        case 2:
            break;
        case 3:
            CallEvent((const u16 *)EventScrWM_Ch2_TRAVEL_TO_NODE, 0);
            break;
        case 4:
            CallEvent((const u16 *)EventScrWM_Ch3_TRAVEL_TO_NODE, 0);
            break;
        case 5:
            CallEvent((const u16 *)EventScrWM_Ch4_TRAVEL_TO_NODE, 0);
            break;
        case 6:
            break;
        case 7:
            CallEvent((const u16 *)EventScrWM_Ch5_TRAVEL_TO_NODE, 0);
            break;
        case 8:
            CallEvent((const u16 *)EventScrWM_Ch6_TRAVEL_TO_NODE, 0);
            break;
        case 9:
            CallEvent((const u16 *)EventScrWM_Ch7_TRAVEL_TO_NODE, 0);
            break;
        case 10:
            CallEvent((const u16 *)EventScrWM_Ch8_TRAVEL_TO_NODE, 0);
            break;
        case 11:
            CallEvent((const u16 *)EventScrWM_Ch9_TRAVEL_TO_NODE, 0);
            break;
        case 12:
            CallEvent((const u16 *)EventScrWM_Ch10_TRAVEL_TO_NODE, 0);
            break;
        default: 
            CallEvent(Events_WM_ChapterIntro[eventID], 0);
            break;
        }
        StartWMFaceCtrl(proc);
        StartGmapMuEntry(NULL);
    }
}

LYN_REPLACE_CHECK(Event97_WmInitNextStoryNode);
//! FE8U = 0x0800C2DC
u8 Event97_WmInitNextStoryNode(struct EventEngineProc * proc)
{
    // struct WorldMapMainProc * worldMapProc;

    int nodeId = WMLoc_GetNextLocId(gGMData.current_node);

    // NoCashGBAPrintf("Next node ID is: %d", nodeId);

    if (nodeId < 0)
    {
        return EVC_ADVANCE_CONTINUE;
    }

    if (EVENT_IS_SKIPPING(proc))
    {
        ResetGmStoryNode();
        gGMData.nodes[nodeId].state |= 1;
        gGMData.nodes[nodeId].state |= 2;

        GM_ICON->nodeId = nodeId;
        GM_ICON->merge_next_node = true;
    }
    else
    {
        if (!(gGMData.nodes[nodeId].state & 1))
        {
            StartGmBaseEntry(nodeId, 0, NULL);
            ResetGmStoryNode();
            gGMData.nodes[nodeId].state |= 2;
        }
    }

    return EVC_ADVANCE_CONTINUE;
};

LYN_REPLACE_CHECK(Event3E_PrepScreenCall);
//! FE8U = 0x08010968
u8 Event3E_PrepScreenCall(struct EventEngineProc * proc)
{
    HideAllUnits();
    ClearFlag(0x84);
    Proc_StartBlocking(gProcScr_SALLYCURSOR, proc);

    return EVC_ADVANCE_YIELD;
}
#endif

void GiveScroll(void)
{
    FORCE_DECLARE u16 skillId = gEventSlots[EVT_SLOT_3];
    u16 charId = gEventSlots[EVT_SLOT_4];

    FORCE_DECLARE struct Unit * unit;
    unit = GetUnitFromCharId(charId);

#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_1
    for (int i = 0; i < 5; i++) {
        if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_1))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_1;
            break;
        }
        else if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_2))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_2;
            break;
        }
        else if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_3))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_3;
            break;
        }
        else if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_4))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_4;
            break;
        }
    }

    // unsigned short *items;
    // items = GetConvoyItemArray();

    for (int i = 0; i < CONFIG_INSTALL_CONVOYEXPA_AMT; i++) {
        if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_1))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_1;
            break;
        }
        else if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_2))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_2;
            break;
        }
        else if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_3))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_3;
            break;
        }
        else if(unit->items[i] == ((skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_4))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL_4;
            break;
        }
    }
#endif
}

LYN_REPLACE_CHECK(ItemGot_GotLeItem);
void ItemGot_GotLeItem(struct GotItemPopupProc * proc)
{
    /* Stop gap measure to enable scrolls to have the right IDs
    ** They will return a short (maybe a word?) with the first half being the skill ID
    ** and the last part being the skill scroll ID, which is currently 0xBD.
    ** Check if the item we're giving is above the byte limit to determine that
    ** we're giving a scroll.
    
    ** The limitation of this approach is that it locks us out of having other dynamically
    ** assigned effects and icons for other items in the future. It might be best to create
    ** another version of this function in that case
    */
    if (proc->item > 255)
        HandleNewItemGetFromDrop(proc->unit, proc->item, proc);
    else
        HandleNewItemGetFromDrop(proc->unit, MakeNewItem(proc->item), proc);
}

void IsTraineeLevelCappedOrPromoted(void)
{
    u16 charId = gEventSlots[EVT_SLOT_3];
    u16 level = gEventSlots[EVT_SLOT_4];
    u16 class = gEventSlots[EVT_SLOT_5];

    struct Unit * unit;
    unit = GetUnitFromCharId(charId);

    if (unit->level >= level || unit->pClassData->number != class)
        gEventSlots[EVT_SLOT_C] = true;
    else
        gEventSlots[EVT_SLOT_C] = false;
}

//! FE8U = 0x08084800
LYN_REPLACE_CHECK(IsCharacterForceDeployed_);
bool IsCharacterForceDeployed_(u16 pid)
{
#ifdef CONFIG_FE8_REWRITE
    const struct ForceDeploymentEnt gForceDeploymentList[] = {
        {CHARACTER_EIRIKA,  CHAPTER_MODE_COMMON,  -1  },
        {CHARACTER_EIRIKA,  CHAPTER_MODE_EIRIKA,  -1  },
        {CHARACTER_EPHRAIM, -1,                   -1  },
        {CHARACTER_ARTUR,   -1,                    4  },
        {CHARACTER_NATASHA, -1,                    6  },
        {CHARACTER_JOSHUA,  -1,                    6  },
        {CHARACTER_EIRIKA,  -1,                    10 },
        {CHARACTER_SALEH,   -1,                    12 },
        {CHARACTER_EPHRAIM, CHAPTER_MODE_EIRIKA,   21 },
        {CHARACTER_EIRIKA,  CHAPTER_MODE_EPHRAIM,  34 },
        {-1, 0, 0},
    };
#endif

    const struct ForceDeploymentEnt * it;

    for (it = gForceDeploymentList; it->pid != (u16)-1; it++)
    {
        if (it->route != 0xFF && it->route != gPlaySt.chapterModeIndex)
            continue;

        if (it->chapter != 0xFF && it->chapter != gPlaySt.chapterIndex)
            continue;

        if (pid != it->pid)
            continue;

        return true;
    }
    return false;
}

LYN_REPLACE_CHECK(GetUnitEquippedWeaponSlot);
int GetUnitEquippedWeaponSlot(struct Unit* unit) {

    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_BREAK)
        return -1;

    int i;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i)
        if (CanUnitUseWeaponNow(unit, unit->items[i]) == TRUE)
            return i;

    return -1;
}

LYN_REPLACE_CHECK(GetUnitEquippedWeapon);
u16 GetUnitEquippedWeapon(struct Unit* unit) {
    
    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_BREAK)
        return 0;

    int i;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i)
        if (CanUnitUseWeapon(unit, unit->items[i]) == TRUE)
            return unit->items[i];

    return 0;
}

LYN_REPLACE_CHECK(AddArrowTrap);
void AddArrowTrap(int x, int turnCountdown, int turnInterval)
{
    if(gEventSlots[EVT_SLOT_9] == 99)
        AddDamagingTrap(gEventSlots[EVT_SLOT_3], 0, TRAP_LIGHTARROW, 0, gEventSlots[EVT_SLOT_4], gEventSlots[EVT_SLOT_5], 10);
    else
        AddDamagingTrap(x, 0, TRAP_LIGHTARROW, 0, turnCountdown, turnInterval, 10);
}

//! FE8U = 0x08010D28
LYN_REPLACE_CHECK(Event44_BreakingSacredStone);
u8 Event44_BreakingSacredStone(struct EventEngineProc * proc)
{
    struct Unit * unit;

    if (EVENT_IS_SKIPPING(proc))
    {
        return EVC_ADVANCE_CONTINUE;
    }

    if (gEventSlots[EVT_SLOT_9] == 99) 
        unit = GetUnit(gBattleTarget.unit.index);
    else
        unit = GetUnitStructFromEventParameter(EVT_CMD_ARGV(proc->pEventCurrent)[0]);

    if (!unit)
    {
        return EVC_ERROR;
    }

    StartStoneShatterAnim(unit, proc);

    return EVC_ADVANCE_YIELD;
}

// LYN_REPLACE_CHECK(InitSubtitleHelpText);
// void InitSubtitleHelpText(struct SubtitleHelpProc * proc)
// {
//     const char * iter;
//     int line;
//     /* Originally an int in the decomp, but won't compile unless it's u32 */
//     u32 width;

//     iter = proc->string;

// #ifdef CONFIG_EXTENDED_HELPBOXES
//     InitSpriteTextFont(&proc->font, OBJ_VRAM0 + 0x5800, 0x14);
// #else
//     InitSpriteTextFont(&proc->font, OBJ_VRAM0 + 0x4800, 0x14);
// #endif
//     SetTextFontGlyphs(1);

//     ApplyPalette(gUnknown_0859EF20, 0x14);

//     for (line = 0; line < 2; line++) {
//         InitSpriteText(proc->text + line);

//         SpriteText_DrawBackgroundExt(proc->text + line, 0);
//         Text_SetColor(proc->text + line, 0);
//     }

//     line = 0;

//     if (iter != 0) {
//         while (*iter > 1) {

//             iter = Text_DrawCharacter(proc->text + line, iter);

//             if (Text_GetCursor(proc->text + line) > 0xE0) {

//                 iter -= 2;
//                 line++;

//                 GetCharTextLen(iter, &width);

//                 Text_SetCursor(proc->text + line, (Text_GetCursor(proc->text) - width) - 0xC0);
//             }
//         }

//         proc->textCount = ((GetStringTextLen(proc->string) + 16) >> 5) + 1;
//         proc->textNum = proc->textCount - 1;
//     }

//     SetTextFont(0);

//     return;
// }

LYN_REPLACE_CHECK(PrepItemScreen_SetupGfx);
//! FE8U = 0x08098620
void PrepItemScreen_SetupGfx(struct PrepItemScreenProc * proc)
{
    int i;

    struct FaceVramEntry faceConfig[4] = {
        { 0x5800, 6, },
        { 0x6800, 7, },
        { 0x0000, 0, },
        { 0x0000, 0, },
    };

    SetupBackgrounds(gBgConfig_ItemUseScreen);

    gLCDControlBuffer.dispcnt.mode = 0;

    SetupFaceGfxData(faceConfig);

    SetDispEnable(0, 0, 0, 0, 0);

    SetupDebugFontForOBJ(0x06017800, 0);

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetText();
    ResetIconGraphics_();
    LoadIconPalettes(4);
    LoadUiFrameGraphics();
    ApplyPalette(gUiFramePaletteD, 2);
    LoadObjUIGfx();

    MakePrepUnitList();
    proc->hoverUnitIdx = UnitGetIndexInPrepList(PrepGetLatestCharId());

    ResetSysHandCursor(proc);
    StartParallelWorker(sub_809A274, proc);
    StartUiCursorHand(proc);

    SetPrimaryHBlankHandler(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gPaletteBuffer[PAL_BACKDROP_OFFSET] = 0;
    EnablePaletteSync();

    for (i = 0; i < 15; i++) {
        InitText(gPrepItemTexts + i, 5);
    }

    for (i = 0; i < 5; i++) {
        InitText(&gPrepItemTexts[15 + i], 7);
        InitText(&gPrepItemTexts[20 + i], 7);
    }

    InitTextDb(&gPrepItemTexts[25], 8);
    InitTextDb(&gPrepItemTexts[26], 8);
    InitText(&gPrepItemTexts[27], 8);
    InitText(&gPrepItemTexts[30], 7);
    InitText(&gPrepItemTexts[31], 5);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void *)0x06012000, -1);    
#else
    LoadHelpBoxGfx((void *)0x06014000, -1);
#endif


    SetBlendBackdropA(0);
    SetBlendBackdropB(0);

    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 0, 0);

    gLCDControlBuffer.wincnt.wout_enableBlend = 1;

    SetWinEnable(1, 0, 0);
    SetWin0Box(0, 4, 240, 68);
    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(1, 1, 0, 1, 1);

    BG_SetPosition(0, 4, -4);
    BG_SetPosition(1, 0, 4);
    BG_SetPosition(2, -40, (proc->unk_34 - 4) & 0xff);
    BG_EnableSyncByMask(7);

    ApplyUnitSpritePalettes();
    CpuFastFill(0, PAL_OBJ(0x0B), 0x20);

    ForceSyncUnitSpriteSheet();

    Decompress(Img_PrepTextShadow, (void *)0x06013E00);
    UiCursorHand_80ACA4C(0, 0, 0, 0xd0, 0x3c);
    DisplaySysHandCursorTextShadow(0x600, 1);

    RestartMuralBackground();

    if (proc->selectedUnitIdx != 0xff) {
        SetUiCursorHandConfig(
            0,
            ((proc->selectedUnitIdx % 3) * 64) + 24,
            ((proc->selectedUnitIdx / 3) * 16) + 4 - proc->unk_34,
            2
        );
        UpdatePrepItemScreenFace(0, GetUnitFromPrepList(proc->selectedUnitIdx), 60, 76, 0x0503);
    }

    StartMenuScrollBarExt(proc, 224, 11, 0x200, 4);

    UpdateMenuScrollBarConfig(6, proc->unk_34, ((PrepGetUnitAmount() - 1) / 3) + 1, 4);
    sub_8097668();
    PrepUpdateSMS();
    SetBlendConfig(1, 8, 8, 0);
    SetBlendTargetA(0, 1, 0, 0, 0);
    SetBlendTargetB(0, 1, 0, 0, 0);

    SetPrimaryHBlankHandler(PrepItemScreen_OnHBlank);

    StartDrawPrepFundsSprite(168, 133, 11, proc);

    StartDrawPrepUnitInfoBgSprites(proc);

    return;
}

LYN_REPLACE_CHECK(PrepItemTrade_Init);
//! FE8U = 0x0809B86C
void PrepItemTrade_Init(struct PrepMenuTradeProc * proc)
{
    const char* str;
    int i;

    struct FaceVramEntry faceConfig[4] = {
        { 0x5800, 6 },
        { 0x6800, 7 },
        { 0x0000, 0 },
        { 0x0000, 0 }
    };

    // TODO: Seems to be necessary to match - there is a "ldrh" here...
    SetupBackgrounds((void*)(u32)*gBgConfig_ItemUseScreen);

    SetupFaceGfxData(faceConfig);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06012000, -1);
#else
    LoadHelpBoxGfx((void*)0x06014000, -1);
#endif

    LoadIconPalettes(4);

    RestartMuralBackground();

    for (i = 0; i < 5; i++) {
        InitTextDb(gPrepItemTexts + 15 + i, 7);
        InitTextDb(gPrepItemTexts + 20 + i, 7);
    }

    proc->selectedItemSlot = 0xff;

    if (proc->unk_40 != -1) {
        proc->cursorItemSlot = proc->unk_40 + 8;
    } else {
        if (GetUnitItemCount(proc->units[0]) == 0) {
            proc->cursorItemSlot = 8;
        } else {
            proc->cursorItemSlot = 0;
        }
    }

    proc->helpBoxItemSlot = 0xff;

    StartFace2(0, GetUnitPortraitId(proc->units[0]), 64, -4, (FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_3)));
    StartFace2(1, GetUnitPortraitId(proc->units[1]), 174, -4, (FACE_DISP_KIND(FACE_96x80) | FACE_DISP_HLAYER(FACE_HLAYER_3)));

    DrawUiFrame2(1, 8, 14, 12, 0);
    DrawUiFrame2(15, 8, 14, 12, 0);

    BG_EnableSyncByMask(7);

    str = GetStringFromIndex(proc->units[0]->pCharacterData->nameTextId);
    PutDrawText(0, gBG0TilemapBuffer, 0, ((48 - GetStringTextLen(str)) / 2), 6, str);

    str = GetStringFromIndex(proc->units[1]->pCharacterData->nameTextId);
    PutDrawText(0, gBG0TilemapBuffer + 0x18, 0, ((48 - GetStringTextLen(str)) / 2), 6, str);

    DrawPrepScreenItems(gBG0TilemapBuffer + 0x122, gPrepItemTexts + 15, proc->units[0], 0);
    DrawPrepScreenItems(gBG0TilemapBuffer + 0x130, gPrepItemTexts + 20, proc->units[1], 0);

    StartUiCursorHand(proc);

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);
    ShowSysHandCursor(
        (proc->cursorItemSlot >> 3) * 0x70 + 0x10,
        (proc->cursorItemSlot & 7) * 0x10 + 0x48,
        0xb,
        0x800
    );

    StartHelpPromptSprite(200, 0x93, 2, proc);

    StartSysBrownBox(0xd, 0xe00, 0xf, 0xc00, 0x400, proc);

    EnableSysBrownBox(0, -0x28, -1, 1);
    EnableSysBrownBox(1, 0xb8, -1, 0);

    SetBlendConfig(1, 0xe, 4, 0);

    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    return;
}

LYN_REPLACE_CHECK(sub_809D914);
//! FE8U = 0x0809D914
void sub_809D914(struct PrepItemSupplyProc * proc)
{

    gLCDControlBuffer.dispcnt.mode = 0;
    SetupBackgrounds(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetFaces();
    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, proc->yOffsetPerPage[proc->currentPage] - 0x28);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06012000, -1);
#else
    LoadHelpBoxGfx((void*)0x06013000, -1);
#endif

    LoadIconPalettes(4);
    RestartMuralBackground();
    BG_EnableSyncByMask(8);

    gLCDControlBuffer.dispcnt.bg0_on = 0;
    gLCDControlBuffer.dispcnt.bg1_on = 0;
    gLCDControlBuffer.dispcnt.bg2_on = 0;
    gLCDControlBuffer.dispcnt.bg3_on = 0;
    gLCDControlBuffer.dispcnt.obj_on = 0;

    return;
}

LYN_REPLACE_CHECK(PrepItemList_InitGfx);
//! FE8U = 0x0809ED8C
void PrepItemList_InitGfx(struct PrepItemListProc * proc)
{
    int i;
    const char* str;

    PrepItemSuppyText PrepItemSuppyTexts;

    gLCDControlBuffer.dispcnt.mode = 0;

    SetupBackgrounds(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetFaces();
    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, proc->yOffsetPerPage[proc->currentPage] - 40);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06013000, -1);
#else
    LoadHelpBoxGfx((void*)0x06012000, -1);
#endif

    LoadIconPalettes(4);

    RestartMuralBackground();

    PutImg_PrepItemUseUnk(0x5000, 5);

    Decompress(gUnknown_08A1B9EC, gGenericBuffer);
    CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);

    BG_EnableSyncByMask(7);

    StartUiCursorHand(proc);

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);

    gLCDControlBuffer.dispcnt.win0_on = 1;
    gLCDControlBuffer.dispcnt.win1_on = 0;
    gLCDControlBuffer.dispcnt.objWin_on = 0;

    gLCDControlBuffer.win0_left = 128;
    gLCDControlBuffer.win0_top = 40;
    gLCDControlBuffer.win0_right = 224;
    gLCDControlBuffer.win0_bottom = 152;

    gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg2 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
    gLCDControlBuffer.wincnt.win0_enableObj = 1;

    gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg2 = 0;
    gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
    gLCDControlBuffer.wincnt.wout_enableObj = 1;

    StartGreenText(proc);

    StartHelpPromptSprite(195, 147, 9, proc);

    InitText(PrepItemSuppyTexts.th + 0, 6);
    InitText(PrepItemSuppyTexts.th + 1, 5);

    InitText(PrepItemSuppyTexts.th + 15, 4);

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        InitText(PrepItemSuppyTexts.th + 2 + i, 7);
    }

    for (i = 0; i < 8; i++) {
        InitTextDb(PrepItemSuppyTexts.th + 7 + i, 7);
    }

    StoreConvoyWeaponIconGraphics(0x4000, 6);

    sub_809D8D4(gBG0TilemapBuffer + 0x6F, 0x4000, 6);

    Decompress(gUnknown_08A19CCC, (void*)0x06015000);
    ApplyPalette(Pal_SpinningArrow, 0x14);

    StartMenuScrollBarExt(proc, 225, 47, 0x5800, 9);
    sub_8097668();
    SomethingPrepListRelated(proc->unit, proc->currentPage, 3);
    sub_809F5F4(proc);

    sub_809D300(
        PrepItemSuppyTexts.th + 7,
        gBG2TilemapBuffer + 0xF,
        (proc->yOffsetPerPage[proc->currentPage]) >> 4,
        proc->unit
    );

    BG_EnableSyncByMask(4);

    DrawPrepScreenItems(gBG0TilemapBuffer + 0x6F + 0xb3, PrepItemSuppyTexts.th + 2, proc->unit, 0);
    sub_809EBF0();

    StartUiSpinningArrows(proc);
    LoadUiSpinningArrowGfx(0, 0x280, 2);
    SetUiSpinningArrowPositions(0x78, 0x18, 0xe9, 0x18);
    SetUiSpinningArrowConfig(3);

    StartParallelWorker(List_PutHighlightedCategorySprites, proc);

    StartFace2(0, GetUnitPortraitId(proc->unit), 64, -4, FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_3));

    str = GetStringFromIndex(proc->unit->pCharacterData->nameTextId);

    StartSysBrownBox(0xd, 0xe00, 0xf, 0xc00, 0x400, proc);

    EnableSysBrownBox(0, -40, -1, 1);
    EnableSysBrownBox(1, 0x98, 6, 2);

    SetBlendConfig(1, 0xe, 4, 0);
    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    PutDrawText(PrepItemSuppyTexts.th, gBG0TilemapBuffer, 0, GetStringTextCenteredPos(48, str), 0, str);

    PrepItemList_DrawCurrentOwnerText(proc);

    return;
}

// LYN_REPLACE_CHECK(PutSubtitleHelpText);
// void PutSubtitleHelpText(struct SubtitleHelpProc * proc, int y)
// {
// #ifdef CONFIG_EXTENDED_HELPBOXES
//     static u16 lut[] = {
//         0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C,
//         0x40, 0x44, 0x48, 0x4C, 0x50, 0x54, 0x58, 0x5C,
//     };

// #else
//     static u16 lut[] = {
//         0x00,
//         0x04, 0x08, 0x0C, 0x10, 0x14, 0x18,
//         0x44, 0x48, 0x4C, 0x50, 0x54, 0x58,
//     };
// #endif

//     int i;

//     for (i = 0; i < 9; i++) {
//         int x = (i * 32) - 32 + proc->textOffset;
//         int index = (proc->textNum + i) % proc->textCount;

// #ifdef CONFIG_EXTENDED_HELPBOXES
//     /* Delete this when this area is fixed, placeholder so the variables in the if statement don't go unused */
//     if (index + x + lut[index] == 0)
//         return;
//     else
//         return;
//         /* Turning it off for now as the suggested new area 42C0 is already used/corrupted */
//         // PutSprite(2, x, y, gObject_32x16, 0x42C0 + lut[index]);
// #else
//         PutSprite(2, x, y, gObject_32x16, 0x4240 + lut[index]);
// #endif
//     }

//     return;
// }

LYN_REPLACE_CHECK(TradeMenu_InitUnitNameDisplay);
void TradeMenu_InitUnitNameDisplay(struct TradeMenuProc * proc)
{
    char* str;
    int xStart;

    // TODO: constants
#ifdef CONFIG_EXTENDED_HELPBOXES
    StartSysBrownBox(6, 0x5800, 0x08, 0x800, 0x400, (struct Proc *) (proc));
#else
    StartSysBrownBox(6, 0x4800, 0x08, 0x800, 0x400, (struct Proc *) (proc));
#endif

    EnableSysBrownBox(0, -40, -1, 1);
    EnableSysBrownBox(1, 184, -1, 0);

    // TODO: special effect constants
    SetBlendConfig(1, 12, 6, 0);

    // TODO: name functions
    SetBlendTargetA(FALSE, FALSE, FALSE, FALSE, FALSE);
    SetBlendTargetB(TRUE,  TRUE,  TRUE,  TRUE,  TRUE);

    // TODO: text color constants

    str = GetStringFromIndex(UNIT_NAME_ID(proc->units[0]));
    xStart = ((8 * UNIT_PANEL_WIDTH) - GetStringTextLen(str)) / 2;

    PutDrawText(NULL, gBG0TilemapBuffer + TILEMAP_INDEX(0, 0), 0, xStart, UNIT_PANEL_WIDTH, str);

    str = GetStringFromIndex(UNIT_NAME_ID(proc->units[1]));
    xStart = ((8 * UNIT_PANEL_WIDTH) - GetStringTextLen(str)) / 2;

    PutDrawText(NULL, gBG0TilemapBuffer + TILEMAP_INDEX(24, 0), 0, xStart, UNIT_PANEL_WIDTH, str);

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

//! FE8U = 0x0809D300 (DisplayConvoyItemStrings?)
LYN_REPLACE_CHECK(sub_809D300);
void sub_809D300(struct Text * textBase, u16 * tm, int yLines, struct Unit * unit)
{
    int i;

    TileMap_FillRect(tm, 12, 31, 0);

    if (gUnknown_02012F56 == 0) {
        ClearText(textBase);
        Text_InsertDrawString(textBase, 0, 1, GetStringFromIndex(0x5a8)); // TODO: msgid "Nothing[.]"
        PutText(textBase, tm + 3);
        return;
    }

    for (i = yLines; (i < yLines + 7) && (i < gUnknown_02012F56); i++) {
        struct Text* th = textBase + (i & 7);
        int item = gPrepScreenItemList[i].item;
        int unusable = !IsItemDisplayUsable(unit, item);

        ClearText(th);

        Text_InsertDrawString(
            th,
            0,
            unusable,
            GetItemName(item)
        );

        DrawIcon(tm + TILEMAP_INDEX(1, i*2 & 0x1f), GetItemIconId(item), 0x4000);

        PutText(th, tm + TILEMAP_INDEX(3, i*2 & 0x1f));

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + TILEMAP_INDEX(12, i*2 & 0x1f), !unusable ? 2 : 1, GetItemUses(item));
#endif

#ifdef CONFIG_FORGING
		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        if (CanItemBeForged(item)) {
            PutSpecialChar(tm + TILEMAP_INDEX(8, i * 2 & 0x1f),
                            !unusable ? TEXT_COLOR_SYSTEM_GOLD
                                    : TEXT_COLOR_SYSTEM_GRAY,
                            TEXT_SPECIAL_PLUS);
            PutNumberOrBlank(tm + TILEMAP_INDEX(9, i * 2 & 0x1f),
                            !unusable ? TEXT_COLOR_SYSTEM_GOLD
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetItemForgeCount(item));
            PutNumberOrBlank(tm + TILEMAP_INDEX(12, i * 2 & 0x1f), !unusable ? 2 : 1,
                            GetForgedItemDurability(item));
        } else if (limits.maxCount == 0) {
            PutNumberOrBlank(tm + TILEMAP_INDEX(12, i * 2 & 0x1f), !unusable ? 2 : 1,
                            GetItemUses(item));
        }
#endif
    }

    return;
}


//! FE8U = 0x0809D47C
LYN_REPLACE_CHECK(sub_809D47C);
void sub_809D47C(struct Text * textBase, u16 * tm, int yLines, struct Unit * unit)
{
    if (gUnknown_02012F56 > yLines) {
        int y = (yLines * 2) & 0x1f;
        struct Text* th = textBase + (yLines & 7);
        int item = gPrepScreenItemList[yLines].item;
        int unusable = !IsItemDisplayUsable(unit, item);

        int offset = TILEMAP_INDEX(0, y);
        TileMap_FillRect(tm + offset, 12, 1, 0);

        ClearText(th);
        Text_InsertDrawString(th, 0, unusable, GetItemName(item));
        DrawIcon(tm + offset + 1, GetItemIconId(item), 0x4000);
        PutText(th, tm + offset + 3);

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + offset + 12, !unusable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY,  GetItemUses(item));
#endif

#ifdef CONFIG_FORGING
		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        if (CanItemBeForged(item)) {
            PutSpecialChar(tm + offset + 8,
                            !unusable ? TEXT_COLOR_SYSTEM_GOLD
                                    : TEXT_COLOR_SYSTEM_GRAY,
                            TEXT_SPECIAL_PLUS);
            PutNumberOrBlank(tm + offset + 9,
                            !unusable ? TEXT_COLOR_SYSTEM_GOLD
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetItemForgeCount(item));
            PutNumberOrBlank(tm + offset + 12, !unusable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY,
                            GetForgedItemDurability(item));
        } else if (limits.maxCount == 0) {
            PutNumberOrBlank(tm + offset + 12, !unusable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
        }
#endif
    }
}

//! FE8U = 0x08099F7C
LYN_REPLACE_CHECK(sub_8099F7C);
void sub_8099F7C(struct Text* th, u16 * tm, struct Unit* unit, u16 flags) {
    int itemCount;
    int i;

    TileMap_FillRect(tm, 12, 20, 0);

    if ((flags & 2) != 0) {
        ResetIconGraphics();
    }

    if (!unit) {
        return;
    }

    itemCount = GetUnitItemCount(unit);

    for (i = 0; i < itemCount; th++, i++) {
        u16 item = unit->items[i];

        int isUnusable = ((flags & 4) != 0)
            ? !CanUnitUseItemPrepScreen(unit, item)
            : !IsItemDisplayUsable(unit, item);

        if ((flags & 1) == 0) {
            ClearText(th);
            Text_SetColor(th, isUnusable);
            Text_SetCursor(th, 0);
            Text_DrawString(th, GetItemName(item));
        }

        DrawIcon(tm + i * 0x40, GetItemIconId(item), 0x4000);

        PutText(th, tm + 2 + i * 0x40);

#ifdef CONFIG_FORGING
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        if (CanItemBeForged(item)) {
            PutSpecialChar(tm + 8 + i * 0x40,
                            !isUnusable ? TEXT_COLOR_SYSTEM_GOLD
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            TEXT_SPECIAL_PLUS);
            PutNumberOrBlank(tm + 9 + i * 0x40,
                            !isUnusable ? TEXT_COLOR_SYSTEM_GOLD
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetItemForgeCount(item));
            PutNumberOrBlank(tm + 11 + i * 0x40,
                            !isUnusable ? TEXT_COLOR_SYSTEM_BLUE
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetForgedItemDurability(item));
        } else if (limits.maxCount == 0) {
            PutNumberOrBlank(tm + 11 + i * 0x40,
                            !isUnusable ? TEXT_COLOR_SYSTEM_BLUE
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetItemUses(item));
        }
#endif

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + 11 + i * 0x40, !isUnusable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
#endif
    }

    return;
}

//! FE8U = 0x0809B74C
LYN_REPLACE_CHECK(DrawPrepScreenItems);
void DrawPrepScreenItems(u16 * tm, struct Text* th, struct Unit* unit, u8 checkPrepUsability) {
    s8 isUsable;
    int i;
    int itemCount;

    TileMap_FillRect(tm, 11, 9, 0);

    itemCount = GetUnitItemCount(unit);

    for (i = 0; i < itemCount; i++) {
        int item = unit->items[i];

        if (checkPrepUsability != 0) {
            isUsable = CanUnitUseItemPrepScreen(unit, item);
        } else {
            isUsable = IsItemDisplayUsable(unit, item);
        }

        ClearText(th);
        PutDrawText(
            th,
            tm + i * 0x40 + 2,
            !isUsable ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE,
            0,
            0,
            GetItemName(item)
        );

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + i * 0x40 + 0xB, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
#endif

#ifdef CONFIG_FORGING
		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        if (CanItemBeForged(item)) {
            PutSpecialChar(tm + i * 0x40 + 8,
                            isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY,
                            TEXT_SPECIAL_PLUS);
            PutNumberOrBlank(tm + i * 0x40 + 9,
                            isUsable ? TEXT_COLOR_SYSTEM_GOLD
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetItemForgeCount(item));
            PutNumberOrBlank(tm + i * 0x40 + 0xB,
                            isUsable ? TEXT_COLOR_SYSTEM_BLUE
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetForgedItemDurability(item));
            } else if (limits.maxCount == 0) {
            PutNumberOrBlank(tm + i * 0x40 + 0xB,
                            isUsable ? TEXT_COLOR_SYSTEM_BLUE
                                        : TEXT_COLOR_SYSTEM_GRAY,
                            GetItemUses(item));
        }
#endif

        DrawIcon(tm + i * 0x40, GetItemIconId(item), 0x4000);

        th++;
    }

    return;
}

//! FE8U = 0x08099328 - PrepItemScreen_DisplayMenuOptions
LYN_REPLACE_CHECK(sub_8099328);
void sub_8099328(struct PrepItemScreenProc* proc, u16 * tm, struct Unit* unit) {

    /* It doesn't seem to save on shutdown (just suspend), so this is a test to give me something to work with for now */
#ifdef CONFIG_BEXP
    sBEXP[0] = 50;
#endif

    /* Funds textbox */
    TileMap_FillRect(tm, 10, 6, 0);

    ClearText(&gPrepItemTexts[25]);
    Text_InsertDrawString(&gPrepItemTexts[25], 0, PrepGetUnitAmount() < 2 ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x594)); // TODO: msgid "Trade"

    Text_InsertDrawString(&gPrepItemTexts[25], 32, PrepGetUnitAmount() < 2 ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x595)); // TODO: msgid "List"

    PutText(&gPrepItemTexts[25], tm + TILEMAP_INDEX(0, 1));

    ClearText(&gPrepItemTexts[26]);
    Text_InsertDrawString(&gPrepItemTexts[26], 0, !CanUnitPrepScreenUse(unit) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x596)); // TODO: msgid "Use"

#ifdef CONFIG_BEXP
    Text_InsertDrawString(&gPrepItemTexts[26], 32, (sBEXP[0] <= 0 ) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_GOLD, "BEXP"); // TODO: msgid "BEXP"
#else
    Text_InsertDrawString(&gPrepItemTexts[26], 32, !proc->hasConvoyAccess ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x59A)); // TODO: msgid "Give all"
#endif

    PutText(&gPrepItemTexts[26], tm + TILEMAP_INDEX(0, 3));

    ClearText(&gPrepItemTexts[27]);
    Text_InsertDrawString(&gPrepItemTexts[27], 0, !proc->hasConvoyAccess ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x598)); // TODO: msgid "Supply"

    if (gGMData.state.bits.state_0) {
        struct Text* th = &gPrepItemTexts[27];
        int color = TEXT_COLOR_SYSTEM_WHITE;
        if ((!proc->hasConvoyAccess) || (GetUnitItemCount(unit) < 1) || CheckInLinkArena()) {
            color = TEXT_COLOR_SYSTEM_GRAY;
        }
        Text_InsertDrawString(th, 32, color, GetStringFromIndex(0x597)); // TODO: msgid "Sell"
    } else {
        if (CheckInLinkArena()) {
            Text_InsertDrawString(&gPrepItemTexts[27], 32, TEXT_COLOR_SYSTEM_GRAY, GetStringFromIndex(0x599)); // TODO: msgid "Armory"
        } else {
            Text_InsertDrawString(&gPrepItemTexts[27], 32, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x599)); // TODO: msgid "Armory"
        }
    }

    PutText(&gPrepItemTexts[27], tm + TILEMAP_INDEX(0, 5));
    return;
}

//! FE8U = 0x080996E8 - PrepItemScreen_MenuOptionProcs
LYN_REPLACE_CHECK(sub_80996E8);
void sub_80996E8(struct PrepItemScreenProc* proc) {
    int previous = proc->popupPromptIdx;

    if (proc->unk_2d == 0xff) {
        if (gKeyStatusPtr->newKeys & R_BUTTON) {
            proc->unk_2d = proc->popupPromptIdx;

            if ((gGMData.state.bits.state_0)) {
                StartHelpBox(
                    (proc->popupPromptIdx & 1) * 32 + 136,
                    (proc->popupPromptIdx >> 1) * 16 + 84,
                    gHelpTextIds_GMapItemScreen[proc->popupPromptIdx]
                );
                return;
            } else {
                StartHelpBox(
                    (proc->popupPromptIdx & 1) * 32 + 136,
                    (proc->popupPromptIdx >> 1) * 16 + 84,
                    gHelpTextIds_PrepItemScreen[proc->popupPromptIdx]
                );
                return;
            }
        }

        if (gKeyStatusPtr->newKeys & A_BUTTON) {
            switch (proc->popupPromptIdx) {
                case 0:
                    if (PrepGetUnitAmount() < 2) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 4);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 1:
                    if (PrepGetUnitAmount() < 2) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 8);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 2:
                    if (!CanUnitPrepScreenUse(GetUnitFromPrepList(proc->selectedUnitIdx))) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 9);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 3:
                    if (CheckInLinkArena() && (!(GetUnitFromPrepList(proc->selectedUnitIdx)->state & US_NOT_DEPLOYED))) {
                        StartPrepErrorHelpbox(-1, -1, 0x088B, proc);
                        return;
                    }

#ifdef CONFIG_BEXP
                    if (sBEXP[0] > 0) {
                            Proc_Goto(proc, 14);
                            PlaySoundEffect(0x6a);
                            return;
                        }
#else

                    if (proc->hasConvoyAccess) {
                        if (PrepItemScreen_GiveAll(GetUnitFromPrepList(proc->selectedUnitIdx)) != 0) {
                            sub_8099328(proc, TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 9), GetUnitFromPrepList(proc->selectedUnitIdx));
                            sub_8099F7C(&gPrepItemTexts[15], TILEMAP_LOCATED(gBG0TilemapBuffer, 2, 9), GetUnitFromPrepList(proc->selectedUnitIdx), 0);

                            BG_EnableSyncByMask(1);

                            PlaySoundEffect(0x6a);
                            return;
                        }
                    }
#endif

                    PlaySoundEffect(0x6c);
                    break;

                case 4:
                    if (!proc->hasConvoyAccess) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 10);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 5:
                    if ((gGMData.state.bits.state_0) != 0) {
                        if (proc->hasConvoyAccess) {
                            int itemCount = GetUnitItemCount(GetUnitFromPrepList(proc->selectedUnitIdx));
                            if ((itemCount > 0) && (!CheckInLinkArena())) {
                                Proc_Goto(proc, 11);
                                PlaySoundEffect(0x6a);
                                return;
                            }
                        }
                    } else {
                        if (!CheckInLinkArena()) {
                            Proc_Goto(proc, 12);
                            PlaySoundEffect(0x6a);
                            return;
                        }
                    }

                    PlaySoundEffect(0x6c);
                    break;

                default:
                    PlaySoundEffect(0x6c);
                    break;
            }

            return;
        }

        if (gKeyStatusPtr->newKeys & B_BUTTON) {
            proc->hoverUnitIdx = proc->selectedUnitIdx;
            proc->selectedUnitIdx = 0xff;
            sub_80ACA84(0);
            PlaySoundEffect(0x6b);
            Proc_Goto(proc, 0);
            return;
        }
    } else if (gKeyStatusPtr->newKeys & (B_BUTTON | R_BUTTON)) {
        CloseHelpBox();
        proc->unk_2d = 0xff;
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT) {
        if ((proc->popupPromptIdx & 1) != 0) {
            proc->popupPromptIdx--;
        } else if (gKeyStatusPtr->newKeys & DPAD_LEFT) {
            proc->popupPromptIdx++;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT) {
        if ((proc->popupPromptIdx & 1) == 0) {
            proc->popupPromptIdx++;
        } else if (gKeyStatusPtr->newKeys & DPAD_RIGHT) {
            proc->popupPromptIdx--;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_UP) {
        if (proc->popupPromptIdx >= 2) {
            proc->popupPromptIdx -= 2;
        } else if (gKeyStatusPtr->newKeys & DPAD_UP) {
            proc->popupPromptIdx += 4;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN) {
        if (proc->popupPromptIdx < 4) {
            proc->popupPromptIdx += 2;
        } else if (gKeyStatusPtr->newKeys & DPAD_DOWN) {
            proc->popupPromptIdx -= 4;
        }
    }

    if (previous == proc->popupPromptIdx) {
        return;
    }

    PlaySoundEffect(0x65);
    ShowSysHandCursor(
        (proc->popupPromptIdx & 1) * 32 + 144,
        (proc->popupPromptIdx >> 1) * 16 + 84,
        3,
        0x400
    );

    if (proc->unk_2d == 0xff) {
        return;
    }

    if (gGMData.state.bits.state_0) {
        StartHelpBox(
            (proc->popupPromptIdx & 1) * 38 + 144,
            (proc->popupPromptIdx >> 1) * 16 + 84,
            gHelpTextIds_GMapItemScreen[proc->popupPromptIdx]
        );
    } else {
        StartHelpBox(
            (proc->popupPromptIdx & 1) * 38 + 144,
            (proc->popupPromptIdx >> 1) * 16 + 84,
            gHelpTextIds_PrepItemScreen[proc->popupPromptIdx]
        );
    }

    return;
}

//! FE8U = 0x080995D4
LYN_REPLACE_CHECK(sub_80995D4);
void sub_80995D4(struct PrepItemScreenProc* proc) {
    s8 unk;

    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 15, 9), 12, 20, 0);

    PutImg_PrepItemUseUnk(0x6000, 5);
    PutImg_PrepPopupWindow(0x800, 10);
    sub_80994C4(proc);

    unk = sub_809A21C(
        ((proc->selectedUnitIdx % 3) * 64) + 20,
        ((proc->selectedUnitIdx / 3) * 16) + 4 - proc->unk_34
    );

    if (unk != 0) {
        BlockUiCursorHand();
    } else {
        UnblockUiCursorHand();
    }

    sub_8098BE8();

    BG_EnableSyncByMask(1);

    return;
}

//! FE8U = 0x08099654
LYN_REPLACE_CHECK(sub_8099654);
void sub_8099654(struct PrepItemScreenProc* proc) {
    /* Display prep menu light brown box options */
    /* Shift the x direction of the text for the options for BEXP using the second parameter of TILEMAP_LOCATED */
#ifdef CONFIG_BEXP
    sub_8099328(proc, TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 9), GetUnitFromPrepList(proc->selectedUnitIdx));
#else
    sub_8099328(proc, TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 9), GetUnitFromPrepList(proc->selectedUnitIdx));
#endif
    /* Display lightbox brown box after selecting unit */
    StartParallelWorker(sub_8098BC8, proc);
    StartHelpPromptSprite(120, 140, 9, proc);

    ShowSysHandCursor(
        (proc->popupPromptIdx & 1) * 32 + 144,
        (proc->popupPromptIdx >> 1) * 16 + 84,
        3,
        0x400
    );

    BG_EnableSyncByMask(1);

    return;
}

// void WmBEXP_Setup(struct WmSellProc* proc) {
//     int i;

//     gLCDControlBuffer.dispcnt.mode = 0;

//     SetupBackgrounds(0);

//     BG_Fill(BG_GetMapBuffer(0), 0);
//     BG_Fill(BG_GetMapBuffer(1), 0);
//     BG_Fill(BG_GetMapBuffer(2), 0);

//     gLCDControlBuffer.bg0cnt.priority = 0;
//     gLCDControlBuffer.bg1cnt.priority = 2;
//     gLCDControlBuffer.bg2cnt.priority = 0;
//     gLCDControlBuffer.bg3cnt.priority = 3;

//     ResetFaces();

//     ResetText();
//     ResetIconGraphics_();
//     LoadUiFrameGraphics();
//     LoadObjUIGfx();

//     BG_SetPosition(0, 0, 0);
//     BG_SetPosition(1, 0, 0);
//     BG_SetPosition(2, 0, 0);

//     LoadHelpBoxGfx((void*)0x06014000, -1);
//     LoadIconPalettes(4);

//     RestartMuralBackground();

//     PutImg_PrepItemUseUnk(0x5000, 5);
//     PutImg_PrepPopupWindow(0x800, 8);

//     Decompress(gUnknown_08A1BBD0, gGenericBuffer);
//     CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);

//     BG_EnableSyncByMask(7);

//     StartFace2(
//         0,
//         GetUnitPortraitId(proc->unit),
//         68,
//         72,
//         FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_2) | FACE_DISP_BLEND
//     );
//     StartUiCursorHand(proc);
//     ResetSysHandCursor(proc);
//     DisplaySysHandCursorTextShadow(0x600, 1);

//     gLCDControlBuffer.dispcnt.win0_on = 1;
//     gLCDControlBuffer.dispcnt.win1_on = 0;
//     gLCDControlBuffer.dispcnt.objWin_on = 0;

//     gLCDControlBuffer.win0_left = 128;
//     gLCDControlBuffer.win0_top = 40;
//     gLCDControlBuffer.win0_right = 224;
//     gLCDControlBuffer.win0_bottom = 152;

//     gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
//     gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
//     gLCDControlBuffer.wincnt.win0_enableBg2 = 1;
//     gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
//     gLCDControlBuffer.wincnt.win0_enableObj = 1;

//     gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
//     gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
//     gLCDControlBuffer.wincnt.wout_enableBg2 = 0;
//     gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
//     gLCDControlBuffer.wincnt.wout_enableObj = 1;

//     gLCDControlBuffer.wincnt.win0_enableBlend = 1;
//     gLCDControlBuffer.wincnt.wout_enableBlend = 1;

//     SetBlendConfig(0, 8, 8, 8);

//     StartGreenText((struct Proc*)proc);

//     StartHelpPromptSprite(120, 140, 2, (struct Proc*)proc);
//     StartDrawPrepFundsSprite(165, 128, 10, proc);
//     ShowPrepFundsSpriteAt(165, 128);

//     InitText(&_PrepItemSuppyTexts->textA, 4);
//     InitText(&_PrepItemSuppyTexts->textB, 2);

//     sub_809FE68();

//     for (i = 0; i < 5; i++) {
//         InitText(&_PrepItemSuppyTexts->textArray[i], 7);
//     }

//     SetPrimaryHBlankHandler(0);
//     SetPrimaryHBlankHandler(sub_809FE1C);

//     BG_EnableSyncByMask(4);

//     DrawPrepScreenItems(gBG0TilemapBuffer + 0x122, &_PrepItemSuppyTexts->textArray[0], proc->unit, 0);
//     WmSell_PutSupplyFaceAndText();

//     StartParallelWorker(WmSell_DrawValueSpriteText, proc);

//     WmSell_DrawItemGoldValue(proc->unit->items[proc->unk_30]);
//     WmSell_DrawPartyFunds();

//     return;
// }

LYN_REPLACE_CHECK(ApplyHazardHealing);
void ApplyHazardHealing(ProcPtr proc, struct Unit* unit, int hp, int status) {
    AddUnitHp(unit, hp);

    if (status >= 0) {
        SetUnitStatus(unit, status);
    }

    if (GetUnitCurrentHp(unit) <= 0) {
        UnitKill(unit);
    }

    DropRescueOnDeath(proc, unit);

    return;
}

LYN_REPLACE_CHECK(MakePoisonDamageTargetList);
void MakePoisonDamageTargetList(int faction) {

    int i;

    InitTargets(0, 0);

    for (i = faction + 1; i < faction + 0x40; i++) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit)) {
            continue;
        }

        if (unit->state & (US_DEAD | US_NOT_DEPLOYED | US_RESCUED | US_BIT16)) {
            continue;
        }

        if (unit->statusIndex != UNIT_STATUS_POISON && GetUnitStatusIndex(unit) != NEW_UNIT_STATUS_TOXIC_POISON) {
            continue;
        }

        AddTarget(unit->xPos, unit->yPos, unit->index, NextRN_N(3) + 1);
    }

    return;
}

LYN_REPLACE_CHECK(PoisonDamageDisplay_Display);
void PoisonDamageDisplay_Display(struct UnknownBMUSAilmentProc* proc) {
    struct SelectTarget* target = GetTarget(proc->unk_4C);
    struct Unit* unit = GetUnit(target->uid);

    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_TOXIC_POISON)
       target->extra = ((unit->curHP / 10) * (gDebuffInfos[NEW_UNIT_STATUS_TOXIC_POISON].duration - (GetUnitStatusDuration(unit) - 1)));

    HideUnitSprite(unit);

    BeginUnitPoisonDamageAnim(unit, target->extra);

    return;
}

LYN_REPLACE_CHECK(PoisonDamageDisplay_Next);
void PoisonDamageDisplay_Next(struct UnknownBMUSAilmentProc* proc) {
    struct SelectTarget* target = GetTarget(proc->unk_4C);
    struct Unit* unit = GetUnit(target->uid);

    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_TOXIC_POISON)
       target->extra = ((unit->curHP / 10) * (gDebuffInfos[NEW_UNIT_STATUS_TOXIC_POISON].duration - (GetUnitStatusDuration(unit) - 1)));

    ApplyHazardHealing(proc, unit, -(target->extra), -1);

    proc->unk_4C++;

    if (GetUnitCurrentHp(GetUnit(gActionData.subjectIndex)) == 0) {
        if (CheckForWaitEvents() != 0) {
            RunWaitEvents();
        }
    }

    if (GetUnitCurrentHp(GetUnit(gActionData.subjectIndex)) < 1) {
        RefreshUnitSprites();
    }

    return;
}

//! FE8U = 0x080B9D14
#ifdef CONFIG_FE8_REWRITE
LYN_REPLACE_CHECK(WorldMap_GenerateRandomMonsters);
void WorldMap_GenerateRandomMonsters(ProcPtr proc)
{
    int i;
    int monster_amt;

    s8 flag = 0;

    /* JESTER - Flag was originally set to 1. Turned off to prevent 
    ** game from crashing.
    ** I suspect the way I'm loading the nodes is fucking with
    ** monster generation, which is causing the game to crash after chapter 8.
    */

    if (!(gGMData.state.bits.monster_merged))
    {
        flag = 0;
    }
    else
    {
        if (gPlaySt.chapterStateBits & PLAY_FLAG_POSTGAME)
        {
            for (i = WM_MONS_UID_ENTRY; i < WM_MONS_UID_END; i++)
                if (gGMData.units[i].id != 0)
                    break;

            if (i == 7)
                flag = 1;
        }
        else
        {
            if (gGMData.units[0].location[gWMNodeData].placementFlag == GMAP_NODE_PLACEMENT_DUNGEON)
            {
                for (i = WM_MONS_UID_ENTRY; i < WM_MONS_UID_END; i++)
                    if (gGMData.units[i].id != 0)
                        break;

                if (i == WM_MONS_UID_END)
                    flag = 1;
            }
        }
    }

    if (flag)
    {
        NewGmapTimeMons(NULL, &monster_amt);
        if (monster_amt > 0)
            Proc_Goto(proc, 2);
    }
    WmShowMonsters();
}
#endif

LYN_REPLACE_CHECK(AttackCommandUsability);
u8 AttackCommandUsability(const struct MenuItemDef* def, int number) {
    int i;

#if defined(SID_GridMasterAtk) && (COMMON_SKILL_VALID(SID_GridMasterAtk))
    if (SkillTester(gActiveUnit, SID_GridMasterAtk) && gActiveUnit->state & US_CANTOING)
        return MENU_ENABLED;
#endif

/* These skills have the same effect here, but GridmasterAtk also prevents the unit from moving */
#if defined(SID_Warpath) && (COMMON_SKILL_VALID(SID_Warpath))
    if (SkillTester(gActiveUnit, SID_Warpath) && gActiveUnit->state & US_CANTOING)
        return MENU_ENABLED;
#endif   

    if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }

    if (gActiveUnit->state & US_IN_BALLISTA) {
        return MENU_NOTSHOWN;
    }

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        int item = gActiveUnit->items[i];

        if (item == 0) {
            break;
        }

        if (!(GetItemAttributes(item) & IA_WEAPON)) {
            continue;
        }

        if (!CanUnitUseWeaponNow(gActiveUnit, item)) {
            continue;
        }

        MakeTargetListForWeapon(gActiveUnit, item);
        if (GetSelectTargetCount() == 0) {
            continue;
        }

        return MENU_ENABLED;
    }

#if (defined(SID_UnarmedCombat) && (COMMON_SKILL_VALID(SID_UnarmedCombat)))
    if (SkillTester(gActiveUnit, SID_UnarmedCombat))
    {
        MakeTargetListForWeapon(gActiveUnit, ITEM_SWORD_IRON);
        if (GetSelectTargetCount() > 0) 
        {
            return MENU_ENABLED;
        }
    }
#endif

    return MENU_NOTSHOWN;

}

LYN_REPLACE_CHECK(DoorCommandUsability);
u8 DoorCommandUsability(const struct MenuItemDef* def, int number) {
    if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }

    if (GetUnitKeyItemSlotForTerrain(gActiveUnit, TERRAIN_DOOR) < 0) {
        return MENU_NOTSHOWN;
    }

    MakeTargetListForDoorAndBridges(gActiveUnit, TERRAIN_DOOR);

    return GetSelectTargetCount() != 0
        ? MENU_ENABLED : MENU_NOTSHOWN;
}

LYN_REPLACE_CHECK(DoorCommandEffect);
u8 DoorCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {

    gActionData.unitActionType = UNIT_ACTION_DOOR;

    gActionData.subjectIndex = gActiveUnit->index;

    gActionData.itemSlotIndex = GetUnitKeyItemSlotForTerrain(gActiveUnit, TERRAIN_DOOR);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

//! FE8U = 0x0801D084
LYN_REPLACE_CHECK(PlayerPhase_PrepareAction);
s8 PlayerPhase_PrepareAction(ProcPtr proc)
{
    s8 cameraReturn;
    int item;

    cameraReturn = EnsureCameraOntoPosition(
        proc, GetUnit(gActionData.subjectIndex)->xPos, GetUnit(gActionData.subjectIndex)->yPos);
    cameraReturn ^= 1;

    switch (gActionData.unitActionType)
    {
        case 0:
        // case CONFIG_UNIT_ACTION_EXPA_ExecSkill: // For backing out the attack forecast for menu skills
            /**
             * If the unit has used action, such as trading,
             * then the unit may take another menu action
             */
            if (gBmSt.taken_action != 0)
            {
                gActionData.unitActionType = UNIT_ACTION_FORCE_WAIT;
                break;
            }

            PlayerPhase_BackToMove(proc);

            return 1;

        case UNIT_ACTION_TRADED:
            gBmSt.taken_action |= BM_TAKEN_ACTION_TRADE;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_TRADED_SUPPLY:
            gBmSt.taken_action |= BM_TAKEN_ACTION_SUPPLY;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_TAKE:
        case UNIT_ACTION_GIVE:
            gBmSt.taken_action |= BM_TAKEN_ACTION_TAKE;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_RIDE_BALLISTA:
        case UNIT_ACTION_EXIT_BALLISTA:
            gBmSt.taken_action |= BM_TAKEN_ACTION_BALLISTA;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_TRADED_1D:
            PlayerPhase_CancelAction(proc);

            return 1;
    }

    item = GetItemIndex(GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex]);

    gBattleActor.hasItemEffectTarget = 0;

    switch (item)
    {
        case ITEM_HEAVENSEAL:
        case ITEM_HEROCREST:
        case ITEM_KNIGHTCREST:
        case ITEM_ORIONSBOLT:
        case ITEM_ELYSIANWHIP:
        case ITEM_GUIDINGRING:
        case ITEM_MASTERSEAL:
        case ITEM_OCEANSEAL:
        case ITEM_LUNARBRACE:
        case ITEM_SOLARBRACE:
        case ITEM_UNK_C1:
            return cameraReturn;
    }

    if ((gActionData.unitActionType != UNIT_ACTION_WAIT) && !gBmSt.just_resumed)
    {
        gActionData.suspendPointType = SUSPEND_POINT_DURINGACTION;
        WriteSuspendSave(SAVE_ID_SUSPEND);
    }

    return cameraReturn;
}

LYN_REPLACE_CHECK(CallChestOpeningEvent);
void CallChestOpeningEvent(u16 tileChangeIndex, u16 idr) {

    CallEvent((u16 *)EventScr_OpenChest, EV_EXEC_GAMEPLAY);

    gEventSlots[0xD] = 1; // qp
    gEventSlotQueue[0]   = tileChangeIndex;
    gEventSlots[0x3] = idr;
}

//! FE8U = 0x080840C4
LYN_REPLACE_CHECK(StartAvailableTileEvent);
void StartAvailableTileEvent(s8 x, s8 y) {
    struct EventInfo info;

    info.listScript = GetChapterEventDataPointer(gPlaySt.chapterIndex)->locationBasedEvents;
    info.xPos = x;
    info.yPos = y;

    if (SearchAvailableEvent(&info) == NULL) {
        return;
    }

    switch (info.commandId) {
        case TILE_COMMAND_VISIT:
            gBmMapUnit[y][x] = gActiveUnit->pCharacterData->number;

            // fallthrough

        case TILE_COMMAND_SEIZE:
            if (GetBattleMapKind() == 2) {
                return;
            }

            ClearActiveEventRegistry();
            StartEventFromInfo(&info, EV_EXEC_CUTSCENE);

            if (info.givenMoney == 3) {
                CallTileChangeEvent(GetMapChangeIdAt(info.xPos, info.yPos));
            }

            return;

        case TILE_COMMAND_20:
            if (GetBattleMapKind() == 2) {
                return;
            }

            CallTileChangeEvent(GetMapChangeIdAt(info.xPos,info.yPos));

            return;

        case TILE_COMMAND_DOOR:
        case TILE_COMMAND_BRIDGE:
            if (GetBattleMapKind() == 2) {
                return;
            }

            if (info.script == 1) {
                CallTileChangeEvent(GetMapChangeIdAt(info.xPos, info.yPos));
                SetFlag(info.flag);
            } else {
                ClearActiveEventRegistry();
                StartEventFromInfo(&info, EV_EXEC_CUTSCENE);
            }

            return;

        case TILE_COMMAND_CHEST:
            if (GetBattleMapKind() == 2) {
                return;
            }

            if (info.givenItem == 0) {
                u8 b;
                u8 rand = NextRN_100();
                u8* ptr = (u8*)(info.script);
                u8 r0;

                r0 = ptr[0];
                if (r0 != 0) {

                    b = ptr[1];
                    if (rand >= b) {
                        for (; rand >= b; b += ptr[1]) {
                            ptr+=2;
                            if (ptr[0] == 0) {
                                goto _08084274;
                            }
                        }
                        r0 = ptr[0];
                    }

                    info.givenItem = r0;
                }
            /* Commenting out these lines lets me use a dummy item in chests so I can AFEV a skill scroll into them */
            _08084274:
                // info.givenItem = sub_8083FFC(info.givenItem);
                // CallChestOpeningEvent(GetMapChangeIdAt(info.xPos, info.yPos), info.givenItem);
            } else {
                if (info.givenItem != 0x77) {
                    info.givenItem = sub_8083FFC(info.givenItem);
                    CallChestOpeningEvent(GetMapChangeIdAt(info.xPos, info.yPos), info.givenItem);
                } else {
                    CallChestOpeningEvent(GetMapChangeIdAt(info.xPos, info.yPos), info.givenMoney);
                }
            }

            SetFlag(info.flag);

            return;

        case TILE_COMMAND_ARMORY:
            if (GetBattleMapKind() == 2) {
                return;
            }

            StartArmoryScreenOrphaned(gActiveUnit, (u16*)info.script);

            return;

        case TILE_COMMAND_VENDOR:
            if (GetBattleMapKind() == 2) {
                return;
            }
            StartVendorScreenOrphaned(gActiveUnit, (u16*)info.script);

            return;

        case TILE_COMMAND_SECRET:
            if (GetBattleMapKind() == 2) {
                return;
            }

            StartSecretShopScreenOrphaned(gActiveUnit, (u16*)info.script);

            return;

#if !NONMATCHING
        case TILE_COMMAND_SHOP_UNK:
            asm("nop");
            return;

        case TILE_COMMAND_NONE:
            asm("nop");
            return;
#endif
    }

    return;
}

LYN_REPLACE_CHECK(GetClassData);
const struct ClassData* GetClassData(int classId) {
    if (classId < 1)
        return NULL;

    return gNewClassData + (classId - 1);
};

LYN_REPLACE_CHECK(StoreNumberStringOrDashesToSmallBuffer);
void StoreNumberStringOrDashesToSmallBuffer(int n)
{
    ClearSmallStringBuffer();
#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    StoreNumberStringToSmallBuffer(n);
#else
    if (n == 255 || n == -1)
    {
        gUnknown_02028E44[7] = ':';
        gUnknown_02028E44[6] = ':';
    }
    else
    {
        StoreNumberStringToSmallBuffer(n);
    }
#endif
}

/* Controls the drawing of the HP and / symbols in the minimum box */
LYN_REPLACE_CHECK(sub_808C360);
void sub_808C360(struct PlayerInterfaceProc* proc, u16* buffer, struct Unit* unit) {

    buffer[0] = 0x2120;
    buffer[1] = 0x2121;
    buffer[2] = 0;
    buffer[3] = 0;
#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    if (GetUnitCurrentHp(unit) < 100)
        buffer[4] = 0x2121 + 0x1D;
#else
    buffer[4] = 0x2121 + 0x1D;
#endif
    buffer[5] = 0;
    buffer[6] = 0;

    return;
}

LYN_REPLACE_CHECK(UnitMapUiUpdate);
void UnitMapUiUpdate(struct PlayerInterfaceProc* proc, struct Unit* unit) {
    s16 frameCount = proc->unitClock;

    if (unit->statusIndex == UNIT_STATUS_RECOVER) {
        frameCount = 0;
    }

    if ((frameCount & 63) == 0) {
        if ((frameCount & 64) != 0) {
            MMB_DrawStatusText(proc->statusTm, unit);

            BG_EnableSyncByMask(BG0_SYNC_BIT);
        } else {
#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
            StoreNumberStringOrDashesToSmallBuffer(GetUnitCurrentHp(unit));
#else
            if (GetUnitCurrentHp(unit) >= 100) {
                StoreNumberStringOrDashesToSmallBuffer(0xFF);
            } else {
                StoreNumberStringOrDashesToSmallBuffer(GetUnitCurrentHp(unit));
            }
#endif

            proc->hpCurHi = gUnknown_02028E44[6] - 0x30;
            proc->hpCurLo = gUnknown_02028E44[7] - 0x30;

#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
            StoreNumberStringOrDashesToSmallBuffer(GetUnitMaxHp(unit));
#else
            if (GetUnitMaxHp(unit) >= 100) {
                StoreNumberStringOrDashesToSmallBuffer(0xFF);
            } else {
                StoreNumberStringOrDashesToSmallBuffer(GetUnitMaxHp(unit));
            }
#endif

            proc->hpMaxHi = gUnknown_02028E44[6] - 0x30;
            proc->hpMaxLo = gUnknown_02028E44[7] - 0x30;

            sub_808C360(proc, proc->statusTm, unit);

            BG_EnableSyncByMask(BG0_SYNC_BIT);
        }
    }

    if ((proc->hideContents == false) && ((frameCount & 64) == 0 || (unit->statusIndex == UNIT_STATUS_NONE))) {
        int x;
        int y;

        int x2;

        x = proc->xHp * 8;
        x2 = x + 17;

        y = proc->yHp * 8;


#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
        if (GetUnitMaxHp(unit) < 100)
        {
            if (proc->hpCurHi != (u8)(' ' - '0'))
            {
                /* Tens character current HP */
                CallARM_PushToSecondaryOAM(x2, y, gObject_8x8, proc->hpCurHi + 0x82E0);
            }
            /* Single character current HP */
            CallARM_PushToSecondaryOAM(x + 24, y, gObject_8x8, proc->hpCurLo + 0x82E0);

            if (proc->hpMaxHi != (u8)(' ' - '0'))
            {
                /* Tens character max HP */
                CallARM_PushToSecondaryOAM(x + 41, y, gObject_8x8, proc->hpMaxHi + 0x82E0);
            }
            /* Single character max HP */
            CallARM_PushToSecondaryOAM(x + 48, y, gObject_8x8, proc->hpMaxLo + 0x82E0);
        }
        else {
            /* Hundreds character current HP */
            u8 hundreds = GetUnitCurrentHp(unit) / 100;

            if (hundreds > 0) {
                CallARM_PushToSecondaryOAM(x2, y, gObject_8x8, hundreds + 0x82E0);
                /* Tens character current HP */
                CallARM_PushToSecondaryOAM(x + 24, y, gObject_8x8, proc->hpCurHi + 0x82E0);
                /* Single character current HP */
                CallARM_PushToSecondaryOAM(x + 32, y, gObject_8x8, proc->hpCurLo + 0x82E0);
            }
            else
            {
                if (proc->hpCurHi != (u8)(' ' - '0'))
                {
                    /* Tens character current HP */
                    CallARM_PushToSecondaryOAM(x2, y, gObject_8x8, proc->hpCurHi + 0x82E0);
                }
                /* Single character currenty HP */
                CallARM_PushToSecondaryOAM(x + 24, y, gObject_8x8, proc->hpCurLo + 0x82E0);
            }
        }
#else
        if (proc->hpCurHi != (u8)(' ' - '0'))
        {
            /* Tens character current HP */
            CallARM_PushToSecondaryOAM(x2, y, gObject_8x8, proc->hpCurHi + 0x82E0);
        }
        /* Single character current HP */
        CallARM_PushToSecondaryOAM(x + 24, y, gObject_8x8, proc->hpCurLo + 0x82E0);

        if (proc->hpMaxHi != (u8)(' ' - '0'))
        {
            /* Tens character max HP */
            CallARM_PushToSecondaryOAM(x + 41, y, gObject_8x8, proc->hpMaxHi + 0x82E0);
        }
        /* Single character max HP */
        CallARM_PushToSecondaryOAM(x + 48, y, gObject_8x8, proc->hpMaxLo + 0x82E0);
#endif
    }

    return;
}

LYN_REPLACE_CHECK(BattleUnitTargetCheckCanCounter);
void BattleUnitTargetCheckCanCounter(struct BattleUnit* bu) {

    if (!bu->canCounter) {
        bu->battleAttack = 0xFF;
        bu->battleHitRate = 0xFF;
        bu->battleEffectiveHitRate = 0xFF;
        bu->battleCritRate = 0xFF;
        bu->battleEffectiveCritRate = 0xFF;
    }
}

LYN_REPLACE_CHECK(BattleInitTargetCanCounter);
void BattleInitTargetCanCounter(void) {

    // Target cannot counter if it is a gorgon egg
    if (UNIT_IS_GORGON_EGG(&gBattleTarget.unit)) {
        gBattleTarget.weapon = 0;
        gBattleTarget.canCounter = FALSE;
    }

    // Target cannot counter if either units are using "uncounterable" weapons
#if defined(SID_Counterattack) && (COMMON_SKILL_VALID(SID_Counterattack))
    if (!SkillTester(&gBattleTarget.unit, SID_Counterattack))
        {
            if ((gBattleActor.weaponAttributes | gBattleTarget.weaponAttributes) & IA_UNCOUNTERABLE) 
            {
                gBattleTarget.weapon = 0;
                gBattleTarget.canCounter = FALSE;
            }
        }
#else
    if ((gBattleActor.weaponAttributes | gBattleTarget.weaponAttributes) & IA_UNCOUNTERABLE) {
        gBattleTarget.weapon = 0;
        gBattleTarget.canCounter = FALSE;
    }
    else
    {
#if defined(SID_DualWieldPlus) && (COMMON_SKILL_VALID(SID_DualWieldPlus))
        if (SkillTester(GetUnit(gBattleActor.unit.index), SID_DualWieldPlus))
        {
            for (int i = 1; i < UNIT_MAX_INVENTORY; i++)
            {
                if (GetItemMight(gBattleActor.unit.items[i]) > 0 && CanUnitUseWeapon(GetUnit(gBattleActor.unit.index), gBattleActor.unit.items[i]))
                {
                    if (GetItemAttributes(gBattleActor.unit.items[i]) & IA_UNCOUNTERABLE)
                    {
                        gBattleTarget.weapon = 0;
                        gBattleTarget.canCounter = FALSE;
                        break;
                    }
                }
            }
        }
#endif
    }
#endif

    // Target cannot counter if a berserked player unit is attacking another player unit
    if (gBattleActor.unit.statusIndex == UNIT_STATUS_BERSERK) {
        if ((UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE) && (UNIT_FACTION(&gBattleTarget.unit) == FACTION_BLUE)) {
            gBattleTarget.weapon = 0;
            gBattleTarget.canCounter = FALSE;
        }
    }
}

LYN_REPLACE_CHECK(ProcMAExpBar_OnIncrement);
void ProcMAExpBar_OnIncrement(struct MAExpBarProc* proc)
{

    // Check if we've reached target BEFORE incrementing
    if (proc->expFrom == proc->expTo) {
        Proc_Break(proc);
        m4aSongNumStop(0x74);
        return;
    }

    proc->expFrom++;

    if (proc->expFrom >= 100)
    {
        proc->expFrom = 0;
        proc->expTo -= 100;
    }

    DrawMAExpBar(6, 8, proc->expFrom);
}

LYN_REPLACE_CHECK(MapAnim_DisplayExpBar);
void MapAnim_DisplayExpBar(struct Proc* proc)
{
    int actorNum = -1;
    switch (gManimSt.actorCount) {
    case 2:
        if (gManimSt.actor[1].bu->expGain != 0)
            actorNum = 1;
        // fallthrough

    case 1:
        if (gManimSt.actor[0].bu->expGain != 0)
            actorNum = 0;
        break;
    }

    if (actorNum >= 0) {
        struct MAExpBarProc* expProc = Proc_StartBlocking(ProcScr_MapAnimExpBar, proc);

        expProc->expFrom = gManimSt.actor[actorNum].bu->expPrevious;
        expProc->expTo   = gManimSt.actor[actorNum].bu->expPrevious + gManimSt.actor[actorNum].bu->expGain;
        expProc->actorId = actorNum;
    }
}

LYN_REPLACE_CHECK(ProcMAExpBar_LevelUpIfPossible);
void ProcMAExpBar_LevelUpIfPossible(struct MAExpBarProc* proc)
{
    // if (proc->expTo >= 100)
    //     StartManimLevelUp(proc->actorId, (struct Proc*) proc);

    int actorNum = -1;
    switch (gManimSt.actorCount) {
    case 2:
        if (gManimSt.actor[1].bu->expGain != 0)
            actorNum = 1;
        // fallthrough

    case 1:
        if (gManimSt.actor[0].bu->expGain != 0)
            actorNum = 0;
        break;
    }

    if ((gManimSt.actor[actorNum].bu->expPrevious + gManimSt.actor[actorNum].bu->expGain) >= 100)
        StartManimLevelUp(proc->actorId, (struct Proc*) proc);
}

LYN_REPLACE_CHECK(AddTrap);
struct Trap * AddTrap(int x, int y, int trapType, int meta)
{
    struct Trap* trap;

    // Find first free trap
    for (trap = GetTrap(0); trap->type != TRAP_NONE; ++trap) {}

    trap->xPos = x;
    trap->yPos = y;
    trap->type = trapType;
    trap->extra = meta;

    return trap;
}

void TryAddUnitToAdjacentSameFactionTargetList(struct Unit* unit) {

    if (UNIT_FACTION(gSubjectUnit) != UNIT_FACTION(unit))
    {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForAdjacentSameFaction(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddUnitToAdjacentSameFactionTargetList);

    return;
}

void TryAddUnitToAdjacentEnemyTargetList(struct Unit* unit) {

    if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForAdjacentEnemies(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddUnitToAdjacentEnemyTargetList);

    return;
}

void TryAddUnitToAdjacentEnemyNonBossTargetList(struct Unit* unit) {

    if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (UNIT_CATTRIBUTES(unit) & CA_BOSS) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForAdjacentNonBossEnemies(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddUnitToAdjacentEnemyNonBossTargetList);

    return;
}

void TryAddUnitToAdjacentUnitsTargetList(struct Unit* unit) {
    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForAdjacentUnits(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddUnitToAdjacentUnitsTargetList);

    return;
}

void TryAddUnitToRangedStatusStavesTargetList(struct Unit* unit) {

    if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForRangedStatusStaves(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    InitTargets(x, y);

    BmMapFill(gBmMapRange, 0);

    MapAddInRange(x, y, GetUnitMagBy2Range(gSubjectUnit), 1);

    ForEachUnitInRange(TryAddUnitToRangedStatusStavesTargetList);

    return;
}

LYN_REPLACE_CHECK(UnitUpdateUsedItem);
void UnitUpdateUsedItem(struct Unit* unit, int itemSlot) {

    /**
     * If the item is a scroll, we need to check for the scroll savant skill.
     * If the unit has the skill, then we skip the check to reduce item uses.
     */
    if (GetItemIndex(unit->items[itemSlot]) == 0xBD)
    {
#if defined(SID_ScrollSavant) && (COMMON_SKILL_VALID(SID_ScrollSavant))
        if (SkillTester(unit, SID_ScrollSavant))
            return;
#endif
    }

    if (unit->items[itemSlot]) {
        unit->items[itemSlot] = GetItemAfterUse(unit->items[itemSlot]);
        UnitRemoveInvalidItems(unit);
    }
}

LYN_REPLACE_CHECK(ItemMenu_ButtonBPressed);
u8 ItemMenu_ButtonBPressed(struct MenuProc* menu, struct MenuItemProc* menuItem) {
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    ResetTextFont();

    StartSemiCenteredOrphanMenu(&gUnitActionMenuDef, gBmSt.cursorTarget.x - gBmSt.camera.x, 1, 22);

    HideMoveRangeGraphics();

    /**
     * We need to reset the action data unk08 here when we're cancelling out
     * of menu skills that hijack the attack menu
     */

    gActionData.unk08 = 0xFFFF;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B | MENU_ACT_CLEAR | MENU_ACT_ENDFACE;
}

static inline s16 GetBanimAllyPosition(int faction1, int faction2)
{
    int pos = EKR_POS_L;
    if (GetBanimLinkArenaFlag() != true)
    {
        if (FACTION_ID_BLUE == (s16)faction1)
            pos = EKR_POS_R;
        else if (FACTION_ID_RED == (s16)faction1)
            pos = EKR_POS_R;
        else if (FACTION_ID_GREEN == (s16)faction1 && FACTION_ID_GREEN == faction2)
            pos = EKR_POS_R;
    }
    return pos;
}

LYN_REPLACE_CHECK(PrepareBattleGraphicsMaybe);
bool PrepareBattleGraphicsMaybe(void)
{
    u16 i;
    u16 pid, jid;
    int zero;
    struct Unit * unit_bu1;
    struct Unit * unit_bu2;
    struct BattleUnit * bu1;
    struct BattleUnit * bu2;
    const struct CharacterData * pinfo1;
    const struct CharacterData * pinfo2;
    int usrdefined_enable;
    const void * animdef1;
    const void * animdef2;
    s16 valid_l;
    s16 valid_r;
    u32 animid1, animid2;

    int char_cnt = 1;

    ResetEkrDragonStatus();

    if (!(gBattleStats.config & BATTLE_CONFIG_ARENA))
        SetBanimArenaFlag(false);
    else
        SetBanimArenaFlag(true);

    if (!(gBmSt.gameStateBits & BM_FLAG_LINKARENA))
        SetBanimLinkArenaFlag(false);
    else
        SetBanimLinkArenaFlag(true);

    if (gBattleStats.config & BATTLE_CONFIG_PROMOTION)
        gEkrDistanceType = EKR_DISTANCE_PROMOTION;
    else
        gEkrDistanceType = EKR_DISTANCE_CLOSE;

    if (gEkrDistanceType == EKR_DISTANCE_PROMOTION)
    {
        bu1 = gpEkrBattleUnitLeft = &gBattleActor;
        bu2 = gpEkrBattleUnitRight = &gBattleTarget;

        gBanimPositionIsEnemy[POS_L] = gBanimPositionIsEnemy[POS_R] = 0;
        gBanimValid[EKR_POS_R] = gBanimValid[EKR_POS_L] = true;
    }
    else
    {
        // int pos;
        u8 i1 = -0x40 & gBattleActor.unit.index;
        u16 faction1 = GetBanimFactionPalette(i1);
        u8 i2 = -0x40 & gBattleTarget.unit.index;
        u16 faction2 = GetBanimFactionPalette(i2);

        if (gBattleStats.config & BATTLE_CONFIG_REFRESH)
            char_cnt = 2;
        else if (gBattleActor.weaponBefore == ITEM_NONE)
            char_cnt = 2;
        else
            char_cnt = GetSpellAssocCharCount(GetItemIndex(gBattleActor.weaponBefore));

        gBanimValid[EKR_POS_L] = gBanimValid[EKR_POS_R] = true;

        if (EKR_POS_R == GetBanimAllyPosition(faction1, faction2))
        {
            bu1 = gpEkrBattleUnitLeft = &gBattleTarget;
            bu2 = gpEkrBattleUnitRight = &gBattleActor;

            gBanimPositionIsEnemy[POS_L] = true;
            gBanimPositionIsEnemy[POS_R] = false;

            if (char_cnt == 1)
                gBanimValid[EKR_POS_L] = false;
        }
        else
        {
            bu1 = gpEkrBattleUnitLeft = &gBattleActor;
            bu2 = gpEkrBattleUnitRight = &gBattleTarget;

            gBanimPositionIsEnemy[POS_L] = false;
            gBanimPositionIsEnemy[POS_R] = true;

            if (char_cnt == 1)
                gBanimValid[EKR_POS_R] = false;
        }
    }

    unit_bu1 = &bu1->unit;
    unit_bu2 = &bu2->unit;

    pinfo1 = unit_bu1->pCharacterData;
    pinfo2 = unit_bu2->pCharacterData;

    animdef1 = animdef2 = 0;

    valid_l = gBanimValid[POS_L];
    valid_r = gBanimValid[POS_R];

    if (valid_l)
        animdef1 = unit_bu1->pClassData->pBattleAnimDef;

    if (valid_r)
        animdef2 = unit_bu2->pClassData->pBattleAnimDef;

    if (valid_l)
    {
        gEkrBmLocation[POS_L] = (16 * unit_bu1->xPos - gBmSt.camera.x) >> 4;
        gEkrBmLocation[POS_R] = (16 * unit_bu1->yPos - gBmSt.camera.y) >> 4;
    }

    if (valid_r)
    {
        gEkrBmLocation[2] = (16 * unit_bu2->xPos - gBmSt.camera.x) >> 4;
        gEkrBmLocation[3] = (16 * unit_bu2->yPos - gBmSt.camera.y) >> 4;
    }

    if (gEkrDistanceType != EKR_DISTANCE_PROMOTION)
    {
        if (GetItemAttributes(gBattleActor.weaponBefore) & IA_UNCOUNTERABLE)
            gEkrDistanceType = EKR_DISTANCE_FARFAR;
        else
        {
            gEkrDistanceType = EKR_DISTANCE_MONOCOMBAT;

            if (valid_l + valid_r == 2)
            {
                s16 x_distance, y_distance;
                x_distance = ABS(gEkrBmLocation[POS_L] - gEkrBmLocation[2]);
                y_distance = ABS(gEkrBmLocation[1] - gEkrBmLocation[3]);

                if (x_distance + y_distance <= 1)
                {
                    gEkrDistanceType = EKR_DISTANCE_CLOSE;
                }
                else if (x_distance + y_distance <= 3)
                {
                    gEkrDistanceType = EKR_DISTANCE_FAR;
                }
                else
                {
                    gEkrDistanceType = EKR_DISTANCE_FARFAR;
                }
            }
        }
    }

    if (gEkrDistanceType == EKR_DISTANCE_PROMOTION)
    {
        gBanimIdx[POS_L] = gBanimIdx_bak[POS_L] = GetBattleAnimationId(unit_bu1, animdef1, bu1->weapon, &animid1);
        gBanimIdx[POS_R] = gBanimIdx_bak[POS_R] = GetBattleAnimationId(unit_bu2, animdef2, bu2->weapon, &animid2);
    }
    else
    {
        if (valid_l)
        {
            gBanimIdx[POS_L] = gBanimIdx_bak[POS_L] = GetBattleAnimationId(unit_bu1, animdef1, bu1->weaponBefore, &animid1);
        }

        if (valid_r)
        {
            gBanimIdx[POS_R] = gBanimIdx_bak[POS_R] = GetBattleAnimationId(unit_bu2, animdef2, bu2->weaponBefore, &animid2);
        }
    }

    pid = unit_bu1->pCharacterData->number - 1;
    jid = unit_bu1->pClassData->number;

    if (valid_l)
        gBanimUniquePal[POS_L] = -1;

    for (i = 0; i < 7; i++)
    {
        if (gAnimCharaPalConfig[pid][i] == jid && valid_l)
        {
            gBanimUniquePal[POS_L] = gAnimCharaPalIt[pid][i] - 1;
            break;
        }
    }

    pid = unit_bu2->pCharacterData->number - 1;
    jid = unit_bu2->pClassData->number;

    if (valid_r)
        gBanimUniquePal[POS_R] = -1;

    for (i = 0; i < 7; i++)
    {
        if (gAnimCharaPalConfig[pid][i] == jid && valid_r)
        {
            gBanimUniquePal[POS_R] = gAnimCharaPalIt[pid][i] - 1;
            break;
        }
    }

    if (valid_l)
        gBanimTriAtkPalettes[POS_L] = (void *)FilterBattleAnimCharacterPalette(gBanimIdx[POS_L], bu1->weaponBefore);

    if (valid_r)
        gBanimTriAtkPalettes[POS_R] = (void *)FilterBattleAnimCharacterPalette(gBanimIdx[POS_R], bu2->weaponBefore);

    gBanimTerrain[POS_L] = bu1->terrainId;
    gBanimTerrain[POS_R] = bu2->terrainId;

    gBanimFloorfx[POS_L] = gBanimFloorfx[POS_R] = -1;

    if (valid_l)
        gBanimFloorfx[POS_L] =
            GetBanimTerrainGround(bu1->terrainId, GetROMChapterStruct(gPlaySt.chapterIndex)->battleTileSet);

    if (valid_r)
        gBanimFloorfx[POS_R] =
            GetBanimTerrainGround(bu2->terrainId, GetROMChapterStruct(gPlaySt.chapterIndex)->battleTileSet);

    if (gBmSt.gameStateBits & BM_FLAG_LINKARENA)
    {
        gBanimTerrain[POS_R] = gBanimTerrain[POS_L] = TERRAIN_ARENA_30;

        if (valid_l)
            gBanimFloorfx[POS_L] =
                GetBanimTerrainGround(gBanimTerrain[POS_L], GetROMChapterStruct(gPlaySt.chapterIndex)->battleTileSet);

        if (valid_r)
            gBanimFloorfx[POS_R] =
                GetBanimTerrainGround(gBanimTerrain[POS_R], GetROMChapterStruct(gPlaySt.chapterIndex)->battleTileSet);
    }

    if (CheckBanimHensei() == true)
    {
        gBanimFloorfx[POS_L] = gBanimFloorfx[POS_R] = 20;
        gBanimTerrain[POS_L] = gBanimTerrain[POS_R] = TERRAIN_ARENA_30;
    }

    switch (gEkrDistanceType)
    {
        case EKR_DISTANCE_CLOSE:
        case EKR_DISTANCE_FAR:
        case EKR_DISTANCE_FARFAR:
        case EKR_DISTANCE_MONOCOMBAT:
            break;

        case EKR_DISTANCE_PROMOTION:
            gBanimFloorfx[POS_L] = gBanimFloorfx[POS_R];
            break;
    }

    switch (gPlaySt.chapterWeatherId)
    {
        case WEATHER_SNOW:
        case WEATHER_SNOWSTORM:
            gEkrSnowWeather = 1;
            break;

        default:
            gEkrSnowWeather = 0;
            break;
    }

    if (valid_l)
        gBanimCon[POS_L] = unit_bu1->pClassData->baseCon;

    if (valid_r)
        gBanimCon[POS_R] = unit_bu2->pClassData->baseCon;

    if (valid_l)
    {
        gEkrGaugeHp[POS_L] = bu1->hpInitial;
        gBanimMaxHP[POS_L] = unit_bu1->maxHP;
    }

    if (valid_r)
    {
        gEkrGaugeHp[POS_R] = bu2->hpInitial;
        gBanimMaxHP[POS_R] = unit_bu2->maxHP;
    }

    ParseBattleHitToBanimCmd();

    if (gEkrDistanceType == EKR_DISTANCE_PROMOTION)
    {
        gEkrSpellAnimIndex[POS_R] = 1;
        gEkrSpellAnimIndex[POS_L] = 1;
    }
    else
    {
        if (valid_l)
            gEkrSpellAnimIndex[POS_L] = GetSpellAnimId(unit_bu1->pClassData->number, bu1->weaponBefore);

        if (valid_r)
            gEkrSpellAnimIndex[POS_R] = GetSpellAnimId(unit_bu2->pClassData->number, bu2->weaponBefore);

        if (gBattleStats.config & BATTLE_CONFIG_REFRESH)
            if (!IsItemDisplayedInBattle(bu2->weaponBefore))
                if (unit_bu2->pClassData->number == CLASS_DANCER)
                    gEkrSpellAnimIndex[POS_R] = 0xF;
    }

    if (valid_l)
        UnsetMapStaffAnim(&gEkrSpellAnimIndex[POS_L], 0, bu1->weaponBefore);

    if (valid_r)
        UnsetMapStaffAnim(&gEkrSpellAnimIndex[POS_R], 1, bu2->weaponBefore);

    switch (gEkrDistanceType)
    {
        case EKR_DISTANCE_CLOSE:
        case EKR_DISTANCE_FAR:
        case EKR_DISTANCE_FARFAR:
            switch (unit_bu1->pClassData->number)
            {
                case CLASS_DRACO_ZOMBIE:
                    SetEkrDragonStatusType(gAnims[POS_L], EKRDRGON_TYPE_DRACO_ZOMBIE);
                    break;

                case CLASS_DEMON_KING:
                    SetEkrDragonStatusType(gAnims[POS_L], EKRDRGON_TYPE_DEMON_KING);
                    break;
            }

            break;

        case EKR_DISTANCE_MONOCOMBAT:
        case EKR_DISTANCE_PROMOTION:
            break;

        default:
            break;
    }

    if (valid_l)
    {
        u8 i1 = -0x40 & unit_bu1->index;
        gBanimFactionPal[POS_L] = GetBanimFactionPalette(i1);
    }

    if (valid_r)
    {
        u8 i2 = -0x40 & unit_bu2->index;
        gBanimFactionPal[POS_R] = GetBanimFactionPalette(i2);
    }

    gEkrPids[POS_R] = 0;
    gEkrPids[POS_L] = 0;

    if (valid_l)
        gEkrPids[POS_L] = pinfo1->number;

    if (valid_r)
        gEkrPids[POS_R] = pinfo2->number;

    if (valid_l)
        gEkrGaugeHit[POS_L] = bu1->battleEffectiveHitRate;

    if (valid_r)
        gEkrGaugeHit[POS_R] = bu2->battleEffectiveHitRate;

    if (gEkrGaugeHit[POS_L] == 0xFF)
        gEkrGaugeHit[POS_L] = -1;

    if (gEkrGaugeHit[POS_R] == 0xFF)
        gEkrGaugeHit[POS_R] = -1;

    if (valid_l)
    {
        gEkrGaugeDmg[POS_L] = bu1->battleAttack - bu2->battleDefense;
        if (gEkrGaugeDmg[POS_L] < 0)
            gEkrGaugeDmg[POS_L] = 0;

        if (bu1->battleAttack == 0xFF)
            gEkrGaugeDmg[POS_L] = -1;

        if (GetItemIndex(bu1->weapon) == ITEM_MONSTER_STONE)
            gEkrGaugeDmg[POS_L] = -1;
    }

    if (valid_r)
    {
        gEkrGaugeDmg[POS_R] = bu2->battleAttack - bu1->battleDefense;
        if (gEkrGaugeDmg[POS_R] < 0)
            gEkrGaugeDmg[POS_R] = 0;

        if (bu2->battleAttack == 0xFF)
            gEkrGaugeDmg[POS_R] = -1;

        if (GetItemIndex(bu2->weapon) == ITEM_MONSTER_STONE)
            gEkrGaugeDmg[POS_R] = -1;
    }

    if (valid_l)
        gEkrGaugeCrt[POS_L] = bu1->battleEffectiveCritRate;

    if (valid_r)
        gEkrGaugeCrt[POS_R] = bu2->battleEffectiveCritRate;

    if (gEkrGaugeCrt[POS_L] == 0xFF)
        gEkrGaugeCrt[POS_L] = -1;

    if (gEkrGaugeCrt[POS_R] == 0xFF)
        gEkrGaugeCrt[POS_R] = -1;

    if (GetItemIndex(bu1->weapon) == ITEM_MONSTER_STONE)
        gEkrGaugeCrt[POS_L] = -1;

    if (GetItemIndex(bu2->weapon) == ITEM_MONSTER_STONE)
        gEkrGaugeCrt[POS_R] = -1;

    if (gEkrDistanceType == EKR_DISTANCE_PROMOTION)
    {
        gEkrGaugeHit[POS_R] = -1;
        gEkrGaugeDmg[POS_R] = -1;
        gEkrGaugeCrt[POS_R] = -1;
    }

    if (valid_l)
        gBanimExpPrevious[POS_L] = (s8)bu1->expPrevious; // needed explicit casts

    if (valid_r)
        gBanimExpPrevious[POS_R] = (s8)bu2->expPrevious; // needed explicit casts

    if (valid_l)
        gBanimExpGain[POS_L] = bu1->expGain;

    if (valid_r)
        gBanimExpGain[POS_R] = bu2->expGain;

    gBanimWtaBonus[POS_R] = 0;
    gBanimWtaBonus[POS_L] = 0;

    if (gEkrDistanceType != EKR_DISTANCE_PROMOTION)
    {
        if (valid_l)
            gBanimWtaBonus[POS_L] = bu1->wTriangleHitBonus;

        if (valid_r)
            gBanimWtaBonus[POS_R] = bu2->wTriangleHitBonus;

        if (valid_l)
            gBanimEffectiveness[POS_L] = IsUnitEffectiveAgainst(unit_bu1, unit_bu2);

        if (valid_r)
            gBanimEffectiveness[POS_R] = IsUnitEffectiveAgainst(unit_bu2, unit_bu1);

        if (!gBanimEffectiveness[POS_L] && valid_l)
            gBanimEffectiveness[POS_L] = IsItemEffectiveAgainst(bu1->weapon, unit_bu2);

        if (!gBanimEffectiveness[POS_R] && valid_r)
            gBanimEffectiveness[POS_R] = IsItemEffectiveAgainst(bu2->weapon, unit_bu1);
    }

    // gBanimForceUnitChgDebug[POS_L] = gBanimForceUnitChgDebug[POS_R] = (void *)zero = 0;

    zero = 0;
    gBanimForceUnitChgDebug[POS_R] = (void *)zero;
    gBanimForceUnitChgDebug[POS_L] = gBanimForceUnitChgDebug[POS_R];


    if (valid_l)
        (void)GetItemIndex(bu1->weaponBefore);

    if (valid_r)
        (void)GetItemIndex(bu2->weaponBefore);

    if (GetBanimLinkArenaFlag() == true || gPlaySt.config.unitColor)
    {
        gBanimUniquePaletteDisabled[POS_L] = gBanimUniquePaletteDisabled[POS_R] = 1;
    }
    else
    {
        gBanimUniquePaletteDisabled[POS_L] = gBanimUniquePaletteDisabled[POS_R] = zero;
    }

    ++zero; --zero; // :/

    gBanimBG = 0;

    if (GetBattleAnimPreconfType() == PLAY_ANIMCONF_ON_UNIQUE_BG)
    {
        if (gBanimValid[POS_L] != false)
            gBanimBG =
                GetBanimBackgroundIndex(gBanimTerrain[POS_L], GetROMChapterStruct(gPlaySt.chapterIndex)->battleTileSet);
        else
            gBanimBG =
                GetBanimBackgroundIndex(gBanimTerrain[POS_R], GetROMChapterStruct(gPlaySt.chapterIndex)->battleTileSet);
    }

    if (CheckBanimHensei() == 1)
        gBanimBG = 0x3C;

    usrdefined_enable = false;
    if (GetBattleAnimPreconfType() == PLAY_ANIMCONF_ON)
        usrdefined_enable = true;
    if (GetBattleAnimPreconfType() == PLAY_ANIMCONF_ON_UNIQUE_BG)
        usrdefined_enable = true;
    if (GetBattleAnimPreconfType() == PLAY_ANIMCONF_OFF)
    {
        /**
         * Banim can also display regardless used-configuration
         * in the following case:
         *
         * 1. promotion
         * 2. arena
         * 3. scripted battle
         */
        if (gEkrDistanceType == EKR_DISTANCE_PROMOTION)
            usrdefined_enable = true;
        if (GetBattleAnimArenaFlag() == true)
            usrdefined_enable = true;
        if ((CheckBattleScripted() == true))
            usrdefined_enable = true;
    }

    SetBattleUnscripted();

    if (gEkrDistanceType != EKR_DISTANCE_PROMOTION)
    {
        if (unit_bu1->state & US_IN_BALLISTA)
            return false;

        if (unit_bu2->state & US_IN_BALLISTA)
            return false;
    }

    if (unit_bu1->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu2->weaponBefore) == ITEM_STAFF_SLEEP)
        return false;

    if (unit_bu2->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu1->weaponBefore) == ITEM_STAFF_SLEEP)
        return false;

    if (unit_bu1->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu2->weaponBefore) == ITEM_STAFF_BERSERK)
        return false;

    if (unit_bu2->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu1->weaponBefore) == ITEM_STAFF_BERSERK)
        return false;

    if (unit_bu1->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu2->weaponBefore) == ITEM_STAFF_SILENCE)
        return false;

    if (unit_bu2->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu1->weaponBefore) == ITEM_STAFF_SILENCE)
        return false;

    if (unit_bu1->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu2->weaponBefore) == ITEM_MONSTER_STONE)
        return false;

    if (unit_bu2->pClassData->number == CLASS_MANAKETE_MYRRH && GetItemIndex(bu1->weaponBefore) == ITEM_MONSTER_STONE)
        return false;

    /* JESTER - Added here as a bugfix for using the dance skill on characters without those animations */
#if defined(SID_Dance) && defined(SID_Dance)
    if(unit_bu1->pClassData->number != CLASS_DANCER && gActionData.unk08 == SID_Dance)
        return false;
#endif

    if (char_cnt != 1 && unit_bu1->pClassData->number == CLASS_DEMON_KING && GetItemIndex(bu1->weaponBefore) != ITEM_NIGHTMARE &&
        unit_bu2->pClassData->number != CLASS_PHANTOM && unit_bu2->pClassData->number != CLASS_DRACO_ZOMBIE)
        return true;

    if (usrdefined_enable == false)
        return false;

    if (gBanimValid[POS_L] == true)
    {
        if (unit_bu1->statusIndex == UNIT_STATUS_BERSERK)
            return false;

        if (gBanimIdx[POS_L] == -1)
            return false;

        if (gEkrSpellAnimIndex[POS_L] == -2)
            return false;

        if (gBanimFloorfx[POS_L] == -1)
            return false;

        if (gBanimTerrain[POS_L] == TERRAIN_WALL_1B)
            return false;

        if (gBanimTerrain[POS_L] == TERRAIN_SNAG)
            return false;
    }

    if (gBanimValid[POS_R] == true)
    {
        if (unit_bu2->statusIndex == UNIT_STATUS_BERSERK)
            return false;

        if (gBanimIdx[POS_R] == -1)
            return false;

        if (gEkrSpellAnimIndex[POS_R] == -2)
            return false;

        if (gBanimFloorfx[POS_R] == -1)
            return false;

        if (gBanimTerrain[POS_R] == TERRAIN_WALL_1B)
            return false;

        if (gBanimTerrain[POS_R] == TERRAIN_SNAG)
            return false;
    }

    return true;
}

LYN_REPLACE_CHECK(StartUnitWeaponSelect);
u8 StartUnitWeaponSelect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
    
    bool noPortraitUnit = false;

    ProcPtr proc = StartOrphanMenu(&gUnknownMenuDef);

    if (gActiveUnit->pCharacterData->portraitId == 0)
        noPortraitUnit = true;

    if (!noPortraitUnit)
    {
        StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
        SetFaceBlinkControlById(0, 5);
    }

    ForceMenuItemPanel(proc, gActiveUnit, 0xF, 0xB);

    sub_80832C4();

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

LYN_REPLACE_CHECK(ProcessMenuDpadInput);
void ProcessMenuDpadInput(struct MenuProc* proc)
{
    proc->itemPrevious = proc->itemCurrent;

    // Handle Up keyin

    if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
    {
        if (proc->itemCurrent == 0)
        {
            if (gKeyStatusPtr->repeatedKeys != gKeyStatusPtr->newKeys)
                return;

            proc->itemCurrent = proc->itemCount;
        }

        proc->itemCurrent--;

// Reset the last item forge count if we move to another item
#ifdef CONFIG_FORGING
        if (gActionData.unk08 == 10000) // Arbitrary value we set to indicate the forge menu is active
        {
            int item = gActiveUnit->items[proc->itemPrevious];
            gEventSlots[EVT_SLOT_7] = GetItemForgeCost(gActiveUnit->items[proc->itemCurrent]);
            SetItemForgeCount(item, gEventSlots[EVT_SLOT_8]);
            // Set the initial count of the latest item
            gEventSlots[EVT_SLOT_8] = GetItemForgeCount(gActiveUnit->items[proc->itemCurrent]); 
        }
#endif

/* A little something to change the monster image displayed as the user scrolls the summon select screen */
#if defined(SID_SummonPlus) && (COMMON_SKILL_VALID(SID_SummonPlus))
        if (gActionData.unk08 == SID_SummonPlus)
        {
            if (UNIT_CATTRIBUTES(gActiveUnit) & CA_PROMOTED)
                PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetClassData(classPromotedIndexes[proc->itemCurrent])->defaultPortraitId, 0x200, 5);
            else
                PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetClassData(classIndexes[proc->itemCurrent])->defaultPortraitId, 0x200, 5);
        }
#endif
    }

    // Handle down keyin

    if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN)
    {
        if (proc->itemCurrent == (proc->itemCount - 1))
        {
            if (gKeyStatusPtr->repeatedKeys != gKeyStatusPtr->newKeys)
                return;

            proc->itemCurrent = -1;
        }

        proc->itemCurrent++;

// Reset the last item forge count if we move to another item
#ifdef CONFIG_FORGING
        if (gActionData.unk08 == 10000) // Arbitrary value we set to indicate the forge menu is active
        {
            int item = gActiveUnit->items[proc->itemPrevious];
            gEventSlots[EVT_SLOT_7] = GetItemForgeCost(gActiveUnit->items[proc->itemCurrent]);
            SetItemForgeCount(item, gEventSlots[EVT_SLOT_8]);
            // Set the initial count of the latest item
            gEventSlots[EVT_SLOT_8] = GetItemForgeCount(gActiveUnit->items[proc->itemCurrent]); 
        }
#endif

/* A little something to change the monster image displayed as the user scrolls the summon select screen */
#if defined(SID_SummonPlus) && (COMMON_SKILL_VALID(SID_SummonPlus))
        if (gActionData.unk08 == SID_SummonPlus)
        {
            if (UNIT_CATTRIBUTES(gActiveUnit) & CA_PROMOTED)
                PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetClassData(classPromotedIndexes[proc->itemCurrent])->defaultPortraitId, 0x200, 5);
            else
                PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetClassData(classIndexes[proc->itemCurrent])->defaultPortraitId, 0x200, 5);
        }
#endif
    }

#ifdef CONFIG_FORGING
    if (gActionData.unk08 == 10000) // Arbitrary value we set to indicate the forge menu is active
    {
        // Handle left/right input for forge menu
        int item = gActiveUnit->items[proc->itemCurrent];
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        int count = GetItemForgeCount(item);

        if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT)
        {
            // Ensure the forge count can't go below what it started at (which is stored in EVT_SLOT_8)
            if (count > 0 && (u32)(count - 1) >= gEventSlots[EVT_SLOT_8])
            {
                int forgeSlot = ITEM_USES(item);
                if (forgeSlot >= 0)
                {
                    // Calculate cumulative cost increase  
                    u32 costAmount = (count + 1) * limits.baseCost;

                    item = GetItemIndex(item) | (forgeSlot << 8);
                    // Calculate cumulative cost reduction
                    u32 refundAmount = 0;
                    for (int i = count; i > count - 1; i--) // This loop refunds 'count * limits.baseCost'
                        refundAmount += i * limits.baseCost;

                    gActiveUnit->items[proc->itemCurrent] = DecrementForgeCount(item, 1);
                    gPlaySt.partyGoldAmount += refundAmount;
                    gEventSlots[EVT_SLOT_7] -= costAmount;

                    struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];

                    if (GetItemForgeCount(gActiveUnit->items[proc->itemCurrent]) < limits.maxCount - 1)
                        MakeForgedItemUnbreakable(item, false);

                    // Refresh the menu display
                    if (proc->menuItems[proc->itemCurrent]->def->onSwitchIn)
                        proc->menuItems[proc->itemCurrent]->def->onSwitchIn(proc, proc->menuItems[proc->itemCurrent]);
                }
            }
        }

        if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT)
        {
            if (count < limits.maxCount - 1 && IsItemForgeable(item))
            {
                int forgeSlot = ITEM_USES(item);
                if (!forgeSlot)
                    forgeSlot = InitFreeForgedItemSlot(item);
                if (forgeSlot >= 0)
                {
                    // Calculate cumulative cost increase  
                    u32 costAmount = (count + 2) * limits.baseCost;

                    if (costAmount <= gPlaySt.partyGoldAmount)
                    {
                        item = GetItemIndex(item) | (forgeSlot << 8);
                        gPlaySt.partyGoldAmount -= costAmount;
                        gActiveUnit->items[proc->itemCurrent] = IncrementForgeCount(item, 1);
                        gEventSlots[EVT_SLOT_7] += costAmount;

                        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];

                        if (GetItemForgeCount(gActiveUnit->items[proc->itemCurrent]) == limits.maxCount - 1)
                            MakeForgedItemUnbreakable(item, true);
                        
                        // Refresh the menu display
                        if (proc->menuItems[proc->itemCurrent]->def->onSwitchIn)
                            proc->menuItems[proc->itemCurrent]->def->onSwitchIn(proc, proc->menuItems[proc->itemCurrent]);
                    }
                }
            }
        }
    }
#endif

    if (proc->itemPrevious != proc->itemCurrent)
    {
        DrawMenuItemHover(proc, proc->itemPrevious, FALSE);
        DrawMenuItemHover(proc, proc->itemCurrent, TRUE);

        PlaySoundEffect(BGM_SYS_CURSOR_UD1);
    }

    // Call def's switch in/out funcs

    if (HasMenuChangedItem(proc))
    {
        if (proc->menuItems[proc->itemPrevious]->def->onSwitchOut)
            proc->menuItems[proc->itemPrevious]->def->onSwitchOut(proc, proc->menuItems[proc->itemPrevious]);

        if (proc->menuItems[proc->itemCurrent]->def->onSwitchIn)
            proc->menuItems[proc->itemCurrent]->def->onSwitchIn(proc, proc->menuItems[proc->itemCurrent]);
    }
}

LYN_REPLACE_CHECK(GenericSelection_BackToUM);
u8 GenericSelection_BackToUM(ProcPtr proc, struct SelectTarget * target) {
    EndTargetSelection(proc);

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    ResetTextFont();

    HideMoveRangeGraphics();

    EnsureCameraOntoPosition(
        StartSemiCenteredOrphanMenu(&gUnitActionMenuDef, gBmSt.cursorTarget.x - gBmSt.camera.x, 1, 22),
        gActiveUnit->xPos,
        gActiveUnit->yPos
    );

    gActionData.unk08 = 0;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_SND6B | MENU_ACT_CLEAR;
}

//! FE8U = 0x08090D80
LYN_REPLACE_CHECK(sub_8090D80);
void sub_8090D80(struct UnitListScreenProc * proc)
{
    int i;
    u8 val;

    SetDispEnable(1, 1, 1, 1, 1);

    SetInterrupt_LCDVCountMatch(NULL);
    SetupBackgrounds(NULL);
    ResetText();
    ResetTextFont();
    ResetIconGraphics();
    ApplyUnitSpritePalettes();

    CpuFastFill(0, gPaletteBuffer + 0x1B0, PLTT_SIZE_4BPP);

    LoadObjUIGfx();

    StartGreenText(proc);

    proc->deployedCount = 0;
    proc->unk_2e = 6;

    sub_8090D00(proc);

    if ((proc->mode != UNITLIST_MODE_PREPMENU) || (proc->unk_2a == 1))
    {
        val = gPlaySt.lastUnitSortType;

        if (val != 0)
        {
            proc->unk_33 = (val >> 7) & 1;
            proc->unk_34 = proc->unk_33;
            proc->unk_32 = val & 0x7f;
        }

        if ((proc->unk_29 != 4) && (proc->page != 0))
        {
            val = gPlaySt.unk19 / 16;

            if (val != 0)
            {
                if (val > 6)
                {
                    proc->page = 6;
                }
                else
                {
                    proc->page = val;
                }

                proc->pageTarget = proc->page;
            }
        }

        SortUnitList(proc->unk_32, proc->unk_34);
    }

    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);
    BG_Fill(gBG2TilemapBuffer, 0);

    ResetIconGraphics_();
    LoadIconPalettes(4);
    LoadUiFrameGraphics();

    ApplyPalettes(Pal_SysBrownBox, 0x19, 2);

    sub_8097FDC();

    CallARM_FillTileRect(gBG1TilemapBuffer, gUnknown_08A1C8B4, 0x1000);

    for (i = 0; i < 7; i++)
    {
        InitText(&gUnknown_0200E060[i], 5);
        InitTextDb(&gUnknown_0200E098[i][0], 8);
        InitText(&gUnknown_0200E098[i][1], 7);
        InitText(&gUnknown_0200E098[i][2], 5);
    }

    InitText(&gUnknown_0200E140, 4);
    InitText(&gUnknown_0200E148, 20);
    InitText(&gUnknown_0200E150, 8);

    sub_8090238(proc->unk_32);

    if (proc->unk_29 == 4)
    {
        sub_8090418(proc, 0);
        proc->unk_29 = 0;
    }
    else if (proc->mode == UNITLIST_MODE_PREPMENU)
    {
        sub_8090418(proc, 1);
    }

    proc->unk_3c = 0;
    proc->helpActive = 0;

    ClearText(&gUnknown_0200E140);
    Text_SetCursor(&gUnknown_0200E140, 4);
    Text_SetColor(&gUnknown_0200E140, 0);
    Text_DrawString(&gUnknown_0200E140, GetStringFromIndex(0x4E5));
    PutText(&gUnknown_0200E140, TILEMAP_LOCATED(gBG2TilemapBuffer, 3, 5));

    for (i = 0; i < 8; i++)
    {
        gUnknown_0200F15C[i] = UINT8_MAX;
    }

    for (i = proc->unk_3e / 16; i < (proc->unk_3e / 16) + 6 && i < gUnknown_0200F158; i++)
    {
        UnitList_PutRow(proc, i, gBG0TilemapBuffer, proc->page, 1);
    }

    sub_8092298(proc->unk_2e, proc->page, 1);

    SetWinEnable(1, 0, 0);
    SetWin0Box(16, 58, 224, 152);
    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(0, 1, 1, 1, 1);

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT | BG3_SYNC_BIT);

    BG_SetPosition(BG_3, 0, 0);
    BG_SetPosition(BG_2, 0, 0);
    BG_SetPosition(BG_1, 0, 0);
    BG_SetPosition(BG_0, 0, (proc->unk_3e - 56) & 0xff);

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    Decompress(gImg_UiSpinningArrow_Horizontal, gBG1TilemapBuffer + 0x280);
    ApplyPalette(Pal_SpinningArrow, 0xf);

    proc->pSpriteProc = Proc_Start(ProcScr_bmview, proc);
    proc->pMuralProc = StartMuralBackground(0, 0, 10);
    LoadHelpBoxGfx(0, -1);

    return;
}

//! FE8U = 0x0804B71C
LYN_REPLACE_CHECK(sub_804B71C);
void sub_804B71C(struct SioBattleMapProc * proc)
{
    int i;

    LoadHelpBoxGfx((void *)0x06015000, 6);
    StartHelpBoxExt_Unk(64, 56, 0x756); // TODO: msgid "Each unit receives 30 extra pts."

    for (i = 0; i < 4; i++)
    {
        if (!sub_8042194(i))
        {
            continue;
        }

        if (gUnk_Sio_0203DD90.unk_0A[i] == 0)
        {
            continue;
        }

        proc->unk_58 = i;
    }

    proc->unk_5c = 0;

    return;
}

extern struct HelpBoxInfo sHelpInfo_ChapterStatus_AllyUnits;
//! FE8U = 0x0808DCAC
LYN_REPLACE_CHECK(StartChapterStatusHelpBox);
void StartChapterStatusHelpBox(ProcPtr proc)
{
    LoadHelpBoxGfx(OBJ_CHR_ADDR(0x280), 6);

    StartMovingHelpBox(&sHelpInfo_ChapterStatus_AllyUnits, proc);

    return;
}

LYN_REPLACE_CHECK(LoadUnit);
struct Unit* LoadUnit(const struct UnitDefinition* uDef) {
    struct UnitDefinition buf;

    struct Unit* unit = NULL;

    if (uDef->genMonster) {
        u32 packedItems;
        u16 item1, item2;

        u16 monsterClass = GenerateMonsterClass(uDef->classIndex);

        buf = *uDef;

        buf.autolevel = TRUE;
        buf.classIndex = monsterClass;
        buf.level = GenerateMonsterLevel(uDef->level);

        packedItems = GenerateMonsterItems(monsterClass);

        // ew
        item1 = packedItems >> 16;
        item2 = packedItems & 0xFFFF;

        buf.items[0] = item1;
        buf.items[1] = item2;
        buf.items[2] = 0;
        buf.items[3] = 0;

        if ((GetItemWeaponEffect(item1) == 1) || !item2)
            buf.itemDrop = FALSE;
        else
            buf.itemDrop = TRUE;

        if (item1 == ITEM_MONSTER_SHADOWSHOT || item1 == ITEM_MONSTER_STONE) {
            // Add another weapon item if weapon is either Shadowshot or Stone

            buf.items[2] = buf.items[1];

            switch (monsterClass) {

            case CLASS_MOGALL:
                buf.items[1] = ITEM_MONSTER_EVILEYE;
                break;

            case CLASS_ARCH_MOGALL:
                buf.items[1] = ITEM_MONSTER_CRIMSON_EYE;
                break;

            case CLASS_GORGON:
                buf.items[1] = ITEM_MONSTER_DEMONSURGE;

            } // switch (monsterClass)
        }

        if (CanClassWieldWeaponType(monsterClass, ITYPE_BOW) == TRUE) {
            // TODO: AI BIT DEFINITIONS
            buf.ai[2] = buf.ai[2] & (1 | 2 | 4);
            buf.ai[2] = buf.ai[2] | (8 | 32);
        }

        uDef = &buf;
    } // (uDef->genMonster)

    switch (uDef->allegiance) {

        // TODO: unit definition faction constants

    case 0:
        unit = GetFreeBlueUnit(uDef);
        break;

    case 1:
        unit = GetFreeUnit(FACTION_GREEN);
        break;

    case 2:
        unit = GetFreeUnit(FACTION_RED);
        break;

#ifdef CONFIG_FOURTH_ALLEGIANCE
    case 3:
        unit = GetFreeUnit(FACTION_PURPLE);
        break;
#endif

    } // switch (uDef->allegiance)

    if (!unit)
        return NULL;

    ClearUnit(unit);

    UnitInitFromDefinition(unit, uDef);
    UnitLoadStatsFromCharacter(unit, unit->pCharacterData);
    UnitHideIfUnderRoof(unit);

    if (UNIT_IS_GORGON_EGG(unit))
        SetUnitStatus(unit, UNIT_STATUS_RECOVER);

    if (uDef->autolevel) {
        if (UNIT_FACTION(unit) == FACTION_BLUE) {
            UnitAutolevelRealistic(unit);
            UnitAutolevelWExp(unit, uDef);
        } else {
            if ((UNIT_CATTRIBUTES(unit) & CA_BOSS) || (unit->pCharacterData->number < 0x40)) {
                struct Unit* unit2 = GetFreeUnit(0);

                CopyUnit(unit, unit2);

                unit2->exp = 0;
                UnitAutolevelRealistic(unit2);

                ClearUnit(unit);
                CopyUnit(unit2, unit);

                ClearUnit(unit2);

                unit->exp   = UNIT_EXP_DISABLED;
                unit->level = uDef->level;
            } else
                UnitAutolevel(unit);

            UnitAutolevelWExp(unit, uDef);
            SetUnitLeaderCharId(unit, uDef->leaderCharIndex);
        }

        if (UNIT_IS_GORGON_EGG(unit))
            unit->maxHP = (unit->level + 1) * 5;
    } // if (uDef->autolevel)

    FixROMUnitStructPtr(unit);
    UnitLoadSupports(unit);

    if (uDef->itemDrop)
        unit->state |= US_DROP_ITEM;

    UnitCheckStatCaps(unit);

    unit->curHP = GetUnitMaxHp(unit);

    if (UNIT_IS_GORGON_EGG(unit))
        SetUnitHp(unit, 5);

    return unit;
}

//! FE8U = 0x080A8A9C
LYN_REPLACE_CHECK(SaveMenuPutChapterTitle);
void SaveMenuPutChapterTitle(struct SaveMenuProc * proc)
{
    int i;
    PutChapterTitleBG(OBJ_PRIORITY(2) + OBJ_CHAR(OBJCHR_SAVEMENU_TITLEBG));
    for (i = 0; i < 3; i++)
    {
        if (proc->chapter_idx[i] != (u8)-1)
        {
            PutChapterTitleGfx((((OBJ_PRIORITY(2) + OBJCHR_SAVEMENU_TITLEGFX) * TILE_SIZE_4BPP + (0x800 * (u32)i)) & 0x1FFFF) / TILE_SIZE_4BPP, proc->chapter_idx[i]);
        }
        else
            PutChapterTitleGfx((((OBJ_PRIORITY(2) + OBJCHR_SAVEMENU_TITLEGFX) * TILE_SIZE_4BPP + (0x800 * (u32)i)) & 0x1FFFF) / TILE_SIZE_4BPP, -1);
    }
}

LYN_REPLACE_CHECK(TradeMenu_InitItemDisplay);
void TradeMenu_InitItemDisplay(struct TradeMenuProc * proc)
{
    DrawUiFrame2(1,  8, 14, 12, 0);
    DrawUiFrame2(15, 8, 14, 12, 0);

    ResetTextFont();

    ResetIconGraphics();
    LoadIconPalettes(4); // TODO: palette id constant

    TradeMenu_InitItemText(proc);
    TradeMenu_RefreshItemText(proc);

#ifdef CONFIG_ESSENTIALS_AI_TRADE_FIX
    bool noPortraitUnit_1 = false;
    bool noPortraitUnit_2 = false;

    if (proc->units[0]->pCharacterData->portraitId == 0)
        noPortraitUnit_1 = true;

    if (proc->units[1]->pCharacterData->portraitId == 0)
        noPortraitUnit_2 = true;

    if (!noPortraitUnit_1)
        StartFace(0, GetUnitPortraitId(proc->units[0]), 64,  -4, 3);
    if (!noPortraitUnit_2)
        StartFace(1, GetUnitPortraitId(proc->units[1]), 176, -4, 2);

#else
    // TODO: face display type (arg 5) constants
    StartFace(0, GetUnitPortraitId(proc->units[0]), 64,  -4, 3);
    StartFace(1, GetUnitPortraitId(proc->units[1]), 176, -4, 2);
#endif

    SetFaceBlinkControlById(0, 5);
    SetFaceBlinkControlById(1, 5);

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
}

//! FE8U = 0x0808C388
LYN_REPLACE_CHECK(PutUnitMapUiStatus);
void PutUnitMapUiStatus(u16 * buffer, struct Unit * unit)
{
    int offset = 0;

    int tileIdx = TILEREF(0x16F, 0);

    if (unit == NULL)
    {
        return;
    }

    switch (unit->statusIndex)
    {
    case UNIT_STATUS_POISON:
        offset = 0;
        break;

    case UNIT_STATUS_SLEEP:
        offset = 0xA0;
        break;

    case UNIT_STATUS_SILENCED:
        offset = 0x140;
        break;

    case UNIT_STATUS_BERSERK:
        offset = 0x1E0;
        break;

    case UNIT_STATUS_ATTACK:
        offset = 0x280;
        break;

    case UNIT_STATUS_DEFENSE:
        offset = 0x320;
        break;

    case UNIT_STATUS_CRIT:
        offset = 0x3C0;
        break;

    case UNIT_STATUS_AVOID:
        offset = 0x460;
        break;

    case UNIT_STATUS_PETRIFY:
    case UNIT_STATUS_13:
        offset = 0x500;
        break;

    case UNIT_STATUS_NONE:
    case UNIT_STATUS_SICK:
    case UNIT_STATUS_RECOVER:
        return;

    case UNIT_STATUS_12:
        break;
    }

    CpuFastCopy(gGfx_StatusText + offset, BG_CHR_ADDR(0x16F), 5 * CHR_SIZE);

    buffer[0] = tileIdx++;
    buffer[1] = tileIdx++;
    buffer[2] = tileIdx++;
    buffer[3] = tileIdx++;
    buffer[4] = tileIdx++;
    buffer[5] = 0;
    buffer[6] = TILEREF(0x128 + unit->statusDuration, 1);

    return;
}

//! FE8U = 0x0808C360
LYN_REPLACE_CHECK(ClearUnitMapUiStatus);
void ClearUnitMapUiStatus(struct PlayerInterfaceProc * proc, u16 * buffer, struct Unit * unit)
{
    buffer[0] = TILEREF(0x120, 2);
    buffer[1] = TILEREF(0x121, 2);
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = TILEREF(0x13E, 2);
    buffer[5] = 0;
    buffer[6] = 0;

    return;
}

//! FE8U = 0x0808C2CC
LYN_REPLACE_CHECK(ApplyUnitMapUiFramePal);
void ApplyUnitMapUiFramePal(int faction, int palId)
{
    u16 * pal = NULL;

    switch (faction)
    {
    case FACTION_BLUE:
        pal = gPal_PlayerInterface_Blue;
        break;

    case FACTION_RED:
        pal = gPal_PlayerInterface_Red;
        break;

    case FACTION_GREEN:
        pal = gPal_PlayerInterface_Green;
        break;

    default:
        nullsub_8();
        break;
    }

    ApplyPalette(pal, palId);

    return;
}

static char * fe8_characters[62] = {
    // Main Story Characters
    "Eirika", "Seth", "Franz", "Gilliam", "Moulder", "Vanessa", "Ross", "Garcia",
    "Neimi", "Colm", "Lute", "Artur", "Natasha", "Joshua", "Ephraim", "Forde",
    "Kyle", "Tana", "Amelia", "Duessel", "Cormag", "L'Arachel", "Dozla", "Ewan",
    "Marisa", "Tethys", "Gerik", "Rennac", "Saleh", "Knoll", "Innes", "Myrrh", "Syrene",

    // Bosses
    "O'Neill", "Berguet", "Bones", "Bazba", "Saar", "Novala", "Murray", "Tirado",
    "Binks", "Pablo", "Aias", "Carlyle", "Gheb", "Beran", "Zonta", "Vigarde",
    
    //Extras
    "Mansel", "Klimt", "Dara",

    // Post-Game Unlockable Characters
    "Caellach", "Orson", "Riev", "Ismaire", "Selena", "Hayden", "Glen", "Valter",
    "Fado", "Lyon"
};

// ! FE8U = 0x0808C5D0
LYN_REPLACE_CHECK(DrawUnitMapUi);
void DrawUnitMapUi(struct PlayerInterfaceProc * proc, struct Unit * unit)
{
    char * str;
    int pos;
    int faceId;

    CpuFastFill(0, gUiTmScratchA, 6 * CHR_SIZE * sizeof(u16));

    str = GetStringFromIndex(unit->pCharacterData->nameTextId);

#if (defined(SID_IdentityProblems) && (COMMON_SKILL_VALID(SID_IdentityProblems)))
    if (SkillTester(unit, SID_IdentityProblems))
        str = fe8_characters[NextRN_N(sizeof(fe8_characters) / sizeof((fe8_characters)[0]))];
#endif

    pos = GetStringTextCenteredPos(56, str);

    ClearText(proc->texts);
    Text_SetParams(proc->texts, pos, TEXT_COLOR_SYSTEM_BLACK);
    Text_DrawString(proc->texts, str);
    PutText(proc->texts, gUiTmScratchA + TILEMAP_INDEX(5, 1));

    faceId = GetUnitMiniPortraitId(unit);

    if (unit->state & US_BIT23)
    {
        faceId = faceId + 1;
    }

    PutFaceChibi(faceId, gUiTmScratchA + TILEMAP_INDEX(1, 1), 0xF0, 4, 0);

    proc->statusTm = gUiTmScratchA + TILEMAP_INDEX(5, 3);
    proc->unitClock = 0;

    if (sPlayerInterfaceConfigLut[proc->cursorQuadrant].xMinimug < 0)
    {
        proc->xHp = 5;
    }
    else
    {
        proc->xHp = 23;
    }

    if (sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug < 0)
    {
        proc->yHp = 3;
    }
    else
    {
        proc->yHp = 17;
    }

    UnitMapUiUpdate(proc, unit);
    DrawHpBar(gUiTmScratchA + TILEMAP_INDEX(5, 4), unit, TILEREF(0x140, 1));

    CallARM_FillTileRect(gUiTmScratchB, gTSA_MinimugBox, TILEREF(0x0, 3));
    ApplyUnitMapUiFramePal(UNIT_FACTION(unit), 3);

    return;
}

LYN_REPLACE_CHECK(IsItemStealable);
s8 IsItemStealable(int item) {

    bool stealable = false;

#if defined(SID_StealPlus) && (COMMON_SKILL_VALID(SID_StealPlus))
    if (SkillTester(gActiveUnit, SID_StealPlus))
        stealable = (GetItemType(item) == ITYPE_ITEM  || 
        GetItemType(item) == ITYPE_ANIMA ||
        GetItemType(item) == ITYPE_LIGHT ||
        GetItemType(item) == ITYPE_DARK  ||
        GetItemType(item) == ITYPE_STAFF ||
        GetItemType(item) == ITYPE_AXE   ||
        GetItemType(item) == ITYPE_BOW   ||
        GetItemType(item) == ITYPE_SWORD ||
        GetItemType(item) == ITYPE_LANCE);
    else
        stealable = GetItemType(item) == ITYPE_ITEM;
#else
    stealable = GetItemType(item) == ITYPE_ITEM;
#endif

    return stealable;

};

extern int _GetUnitCon(struct Unit * unit); 

LYN_REPLACE_CHECK(AddAsTarget_IfCanStealFrom);
void AddAsTarget_IfCanStealFrom(struct Unit* unit) {
    int i;

    if (UNIT_FACTION(unit) != FACTION_RED) {
        return;
    }

    if (gActiveUnit->spd < unit->spd) {
        return;
    }

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        u16 item = unit->items[i];

        if (item == 0) {
            return;
        }

        if (!IsItemStealable(item)) {
            continue;
        }

#if defined(SID_StealPlus) && (COMMON_SKILL_VALID(SID_StealPlus))
        if (SkillTester(gActiveUnit, SID_StealPlus)) 
        {
            if (GetUnitEquippedWeaponSlot(unit) == i) {
                continue;
            }

            if (_GetUnitCon(gActiveUnit) <= GetItemWeight(unit->items[i])) {
                continue;
            }
        }
#endif

#if defined(SID_StickyHold) && (COMMON_SKILL_VALID(SID_StickyHold))
        if (SkillTester(unit, SID_StickyHold)) 
            continue;
#endif

        AddTarget(unit->xPos, unit->yPos, unit->index, 0);
        return;
    }

    return;
}

extern struct ProcCmd CONST_DATA gProcScr_SupportScreen[];

//! FE8U = 0x080A1984
LYN_REPLACE_CHECK(StartSupportScreen);
void StartSupportScreen(ProcPtr parent) {
    struct SupportScreenProc* proc = Proc_StartBlocking(gProcScr_SupportScreen, parent);
    proc->fromPrepScreen = FALSE;
    return;
}

//! FE8U = 0x080A21D0
LYN_REPLACE_CHECK(SupportSubScreen_Init);
void SupportSubScreen_Init(struct SubScreenProc* proc) {
    proc->x = 0;
    proc->y = 0;
    proc->unk_39 &= 0xfc;
    proc->unk_39 &= 0xe3;
    proc->partnerCount = GetSupportScreenPartnerCount(GetSupportScreenCharIdAt(proc->unitIdx));

    InitSupportSubScreenPartners(proc);
    InitSupportSubScreenPartnerLevels(proc);
    InitSupportSubScreenRemainingSupports(proc);
    SupportSubScreen_MoveCursorToNextValidUnit(proc, 0, +1);

    return;
}

//! FE8U = 0x080A2274
LYN_REPLACE_CHECK(SupportSubScreen_SetupGraphics);
void SupportSubScreen_SetupGraphics(struct SubScreenProc* proc) {
    gLCDControlBuffer.dispcnt.mode = 0;

    SetupBackgrounds(0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 3;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetText();
    ResetIconGraphics_();

    LoadUiFrameGraphics();
    LoadObjUIGfx();

    ApplyUnitSpritePalettes();
    sub_80A221C();
    LoadIconPalettes(0xd);

    StartGreenText((void*)proc);

    if (!proc->fromPrepScreen) {
        gPlaySt.config.textSpeed = 1; // TODO: Text speed constants

        ResetSysHandCursor(proc);
        DisplaySysHandCursorTextShadow(0x600, 1);
        ConfigSysHandCursorShadowEnabled(1);

        proc->unk_3a = -1;

        if (proc->unk_3b != 0) {
            ShowSysHandCursor(
                (proc->unk_39 & 3) * 8 + 0xc4,
                ((proc->unk_39 >> 2) & 7) * 16 + 0x18,
                1,
                0x800
            );
        }
    }

    BG_SetPosition(0, 4, 0);
    BG_SetPosition(1, 4, 0);
    BG_SetPosition(2, 0, 0);

    SetBlendConfig(1, 0xd, 3, 0);
    SetBlendTargetA(0, 1, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    SetBlendBackdropA(0);
    SetBlendBackdropB(0);

    RestartMuralBackground();

    PutImg_PrepItemUseUnk(0x4000, 5);

    Decompress(gTsa_SupportSubScreen, gGenericBuffer);
    CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);

    PutFace80x72(
        (struct Proc*)proc,
        TILEMAP_LOCATED(gBG0TilemapBuffer, 2, 1),
        gCharacterData[GetSupportScreenCharIdAt(proc->unitIdx) - 1].portraitId,
        0x200,
        2
    );

    DrawSupportSubScreenUnitPartnerDetails(proc);
    DrawSupportSubScreenRemainingText(proc);

    Decompress(gGfx_SupportMenu, (void*)0x06017800);
    ApplyPalette(gPal_SupportMenu, 0x1A);
    ApplyPalette(Pal_MapBattleInfoNum, 0x12);

    StartParallelWorker(DrawSupportSubScreenSprites, proc);

    return;
}

//! FE8U = 0x080A2448
LYN_REPLACE_CHECK(SupportSubScreen_Loop_KeyHandler);
void SupportSubScreen_Loop_KeyHandler(struct SubScreenProc* proc) {
    if (gKeyStatusPtr->newKeys & B_BUTTON) {
        PlaySoundEffect(SONG_SE_SYS_WINDOW_CANSEL1);
        Proc_Goto(proc, 3);
        return;
    }

    if (gKeyStatusPtr->repeatedKeys & R_BUTTON) {
        Proc_Goto(proc, 4);
        return;
    }

    if (gKeyStatusPtr->repeatedKeys & L_BUTTON) {
        Proc_Goto(proc, 5);
        return;
    }

    if (proc->fromPrepScreen) {
        return;
    }

    if (proc->unk_3b != 0) {
        u32 previous = proc->unk_39;

        if (gKeyStatusPtr->newKeys & A_BUTTON) {
            PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
            Proc_Goto(proc, 2);
            return;
        }

        if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT) {
            if ((proc->unk_39 & 3) != 0) {
                int unk = (proc->unk_39 & 0xfc) + 0xFF;
                proc->unk_39 = unk + (proc->unk_39 & 3);
            }
        }

        if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT) {
            if ((proc->unk_39 & 3) < GetSupportScreenPartnerSupportLevel(proc->unitIdx, (proc->unk_39 >> 2) & 7) - 1) {
                int unk = (proc->unk_39 & 0xfc) + 1;
                proc->unk_39 = unk + (proc->unk_39 & 3);
            }
        }

        if (gKeyStatusPtr->repeatedKeys & DPAD_UP) {
            SupportSubScreen_MoveCursorToNextValidUnit(proc, ((proc->unk_39 >> 2) & 7) - 1, -1);
        }

        if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN) {
            SupportSubScreen_MoveCursorToNextValidUnit(proc, ((proc->unk_39 >> 2) & 7) + 1, +1);
        }

        if (previous != proc->unk_39) {
            ShowSysHandCursor(
                (proc->unk_39 & 3) * 8 + 0xc4,
                ((proc->unk_39 >> 2) & 7) * 16  + 0x18,
                1,
                0x800
            );
            PlaySoundEffect(SONG_65);
        }

    } else {
        if (gKeyStatusPtr->newKeys & A_BUTTON) {
            PlaySoundEffect(SONG_6C);
        }

        return;
    }

    return;
}

//! FE8U = 0x08032270
LYN_REPLACE_CHECK(ActionVisitAndSeize);
s8 ActionVisitAndSeize(ProcPtr proc) {
    int x = GetUnit(gActionData.subjectIndex)->xPos;
    int y = GetUnit(gActionData.subjectIndex)->yPos;

    StartAvailableTileEvent(x, y);

    return 0;
}

LYN_REPLACE_CHECK(MapAnim_DisplayDeathFade);
void MapAnim_DisplayDeathFade(void)
{
    int actorNum = -1;

    switch (gManimSt.actorCount)
    {
    case 2:
        if (gManimSt.actor[1].hp_cur == 0)
            actorNum = 1;

        // fallthrough

    case 1:
        if (gManimSt.actor[0].hp_cur == 0)
            actorNum = 0;

        break;
    } // switch (gManimSt.actorCount)

    if (actorNum != -1)
        MU_StartDeathFade(gManimSt.actor[actorNum].mu);
}

//! FE8U = 0x0801D300
LYN_REPLACE_CHECK(RunPotentialWaitEvents);
bool RunPotentialWaitEvents(void)
{
#if defined(SID_GoodListener) && (COMMON_SKILL_VALID(SID_GoodListener))
    if (SkillTester(gActiveUnit, SID_GoodListener) && gActionData.unitActionType == UNIT_ACTION_VISIT)
    {
        AddExp_Event(10);
    }
#endif  

    if (CheckForWaitEvents())
    {
        RunWaitEvents();
        return false;
    }

    return true;
}

LYN_REPLACE_CHECK(DisplayUnitStandingAttackRange);
int DisplayUnitStandingAttackRange(struct MenuProc* menu, struct MenuItemProc* menuItem) {
    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);

    if (gActiveUnit->state & US_IN_BALLISTA) {
        MapAddInBoundedRange(gActiveUnit->xPos, gActiveUnit->yPos, 1, 10);
    } else {
        int reach = GetUnitWeaponReachBits(gActiveUnit, -1);

#if defined(SID_UnarmedCombat) && (COMMON_SKILL_VALID(SID_UnarmedCombat))
        if (SkillTester(gActiveUnit, SID_UnarmedCombat) && reach < 3)
            reach = 3;
#endif  

        GenerateUnitStandingReachRange(gActiveUnit, reach);
    }

    DisplayMoveRangeGraphics(3);

    return 0;
}

LYN_REPLACE_CHECK(UnitActionMenu_Attack);
u8 UnitActionMenu_Attack(struct MenuProc* menu, struct MenuItemProc* menuItem) {

    if (menuItem->availability == MENU_DISABLED) {
        MenuFrozenHelpBox(menu, 0x858); // TODO: msgid "There's no more ammo for[NL]the ballista.[.]"
        return MENU_ACT_SND6B;
    }

    ResetIconGraphics();

    LoadIconPalettes(4);

    if (gActiveUnit->state & US_IN_BALLISTA) {
        return StartUnitBallistaSelect(menu, menuItem);
    }

#if defined(SID_UnarmedCombat) && (COMMON_SKILL_VALID(SID_UnarmedCombat))
    if (SkillTester(gActiveUnit, SID_UnarmedCombat))
    {
        ClearBg0Bg1();
        MakeTargetListForWeapon(gActiveUnit, ITEM_SWORD_IRON);
        NewTargetSelection(&gSelectInfo_Attack);
        sub_80832C8();
        return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
    }
#endif  

    return StartUnitWeaponSelect(menu, menuItem);
}

LYN_REPLACE_CHECK(AttackMapSelect_SwitchIn);
u8 AttackMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget* target) {

    struct Unit* unit = GetUnit(target->uid);

    ChangeActiveUnitFacing(target->x, target->y);

    if (target->uid == 0) {
        gActionData.xOther = target->x;
        gActionData.yOther = target->y;
        gActionData.trapType = target->extra;

        InitObstacleBattleUnit();
    }

    if (gActionData.itemSlotIndex == BU_ISLOT_BALLISTA) {
        BattleGenerateBallistaSimulation(gActiveUnit, unit, gActiveUnit->xPos, gActiveUnit->yPos);
    } else {
        BattleGenerateSimulation(gActiveUnit, unit, -1, -1, gActionData.itemSlotIndex);
    }

    UpdateBattleForecastContents();

    return 0;
}

LYN_REPLACE_CHECK(DrawBattleForecastContentsStandard);
void DrawBattleForecastContentsStandard(struct BattleForecastProc * proc)
{
    int damage;
    int critRate;

    CallARM_FillTileRect(gUiTmScratchB, gTSA_BattleForecastStandard, 0x1000);

    TileMap_FillRect(gUiTmScratchA, 10, 15, 0);

    PutBattleForecastUnitName(gUiTmScratchA + 0x23, &proc->unitNameTextA, &gBattleActor.unit);

    PutBattleForecastUnitName(gUiTmScratchA + 0x161, &proc->unitNameTextA, &gBattleTarget.unit);

#if defined(SID_UnarmedCombat) && (COMMON_SKILL_VALID(SID_UnarmedCombat))
    if (SkillTester(GetUnit(gBattleActor.unit.index), SID_UnarmedCombat))
    {
        if (gBattleActor.weapon == 0)
        {
            int actorAccuracy = gBattleActor.unit.skl * 2;
            int targetAvoid = gBattleTarget.battleAvoidRate;
            int calculatedHit = SKILL_EFF0(SID_UnarmedCombat) + actorAccuracy - targetAvoid;
            gBattleActor.battleEffectiveHitRate = calculatedHit > 100 ? 100 : calculatedHit;
        }
    }
    if (SkillTester(GetUnit(gBattleTarget.unit.index), SID_UnarmedCombat))
    {
        if (gBattleTarget.weapon == 0)
        {
            int targetAccuracy = gBattleTarget.unit.skl * 2;
            int actorAvoid = gBattleActor.battleAvoidRate;
            int calculatedHit = SKILL_EFF0(SID_UnarmedCombat) + targetAccuracy - actorAvoid;
            gBattleTarget.battleEffectiveHitRate = calculatedHit > 100 ? 100 : calculatedHit;
        }
    }
#endif

    PutBattleForecastItemName(gUiTmScratchA + 0x1A1, &proc->itemNameText, gBattleTarget.weaponBefore);

    if ((gBattleTarget.weapon == 0) && (gBattleTarget.weaponBroke == 0)) {
        damage = -1;

        gBattleTarget.battleEffectiveHitRate = 0xFF;
        gBattleTarget.battleEffectiveCritRate = 0xFF;
    } else {
        damage = gBattleTarget.battleAttack - gBattleActor.battleDefense;

        if (damage < 0) {
            damage = 0;
        }
    }

    if (gBattleTarget.hpInitial > 99) {
        PutNumberTwoChr(gUiTmScratchA + 0x62, 2, 0xFF);
    } else {
        PutNumberTwoChr(gUiTmScratchA + 0x62, 2, gBattleTarget.hpInitial);
    }

    PutNumberTwoChr(gUiTmScratchA + 0xA2, 2, damage);
    PutNumberTwoChr(gUiTmScratchA + 0xA2 + 0x40, 2, gBattleTarget.battleEffectiveHitRate);
    PutNumberTwoChr(gUiTmScratchA + 0xA2 + 0x80, 2, gBattleTarget.battleEffectiveCritRate);

    damage = gBattleActor.battleAttack - gBattleTarget.battleDefense;

    if (GetItemIndex(gBattleActor.weapon) == ITEM_MONSTER_STONE) {
        damage = 0xFF;
    }

    if (damage < 0) {
        damage = 0;
    }

    critRate = gBattleActor.battleEffectiveCritRate;

    if (GetItemIndex(gBattleActor.weapon) == ITEM_MONSTER_STONE) {
        critRate = 0xFF;
    }

    if (critRate < 0) {
        critRate = 0;
    }

    if (gBattleActor.hpInitial > 99) {
        PutNumberTwoChr(gUiTmScratchA + 0xA8 - 0x40, 2, 0xFF);
    } else {
        PutNumberTwoChr(gUiTmScratchA + 0xA8 - 0x40, 2, gBattleActor.hpInitial);
    }

    PutNumberTwoChr(gUiTmScratchA + 0xA8, 2, damage);
    PutNumberTwoChr(gUiTmScratchA + 0xA8 + 0x40, 2, gBattleActor.battleEffectiveHitRate);
    PutNumberTwoChr(gUiTmScratchA + 0xA8 + 0x80, 2, critRate);

    PutTwoSpecialChar(gUiTmScratchA + 0xA8 - 0x44, TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_HP_A, TEXT_SPECIAL_HP_B);

    PutText(gaBattleForecastTextStructs, gUiTmScratchA  + 0xA8 - 5);
    PutText(gaBattleForecastTextStructs + 1, gUiTmScratchA  + 0xA8 + 0x3B);
    PutText(gaBattleForecastTextStructs + 2, gUiTmScratchA  + 0xA8 + 0x7B);

    DrawIcon(gUiTmScratchA + 0xA8 + 0xBF, GetItemIconId(gBattleTarget.weaponBefore), 0x4000);

    DrawIcon(gUiTmScratchA + 0xA8 - 0x87, GetItemIconId(gBattleActor.weaponBefore), 0x3000);

}

extern struct ProcCmd CONST_DATA gProcScr_0859B630[];

LYN_REPLACE_CHECK(AttackMapSelect_Cancel);
u8 AttackMapSelect_Cancel(ProcPtr proc, struct SelectTarget * target) {
    if (EventEngineExists() == 1) {
        return 0;
    }

#if defined(SID_UnarmedCombat) && (COMMON_SKILL_VALID(SID_UnarmedCombat))
    if (SkillTester(gActiveUnit, SID_UnarmedCombat))
    {
      return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B;      
    }
#endif

    Proc_Start(gProcScr_0859B630, PROC_TREE_3);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B;
}

//! FE8U = 0x080A0AD4
LYN_REPLACE_CHECK(GetSupportScreenPartnerSupportLevel);
int GetSupportScreenPartnerSupportLevel(int idx, int partner) 
{

#ifdef CONFIG_UNLOCK_ALL_SUPPORTS
    return 3;
#else
    return sSupportScreenUnits[idx].supportLevel[partner];
#endif

}

//! FE8U = 0x080B723C
LYN_REPLACE_CHECK(PairedEndingBattleDisp_InitBlend);
void PairedEndingBattleDisp_InitBlend(struct EndingBattleDisplayProc * proc)
{
    proc->timer = 0;

    SetBlendAlpha(0x10, 0);
    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 1, 0, 0);

    return;
}

//! FE8U = 0x080B7274
LYN_REPLACE_CHECK(PairedEndingBattleDisp_Loop_Blend);
void PairedEndingBattleDisp_Loop_Blend(struct EndingBattleDisplayProc * proc)
{
    int bldAmt = proc->timer >> 2;

    proc->timer++;

    SetBlendAlpha(0x10 - bldAmt, bldAmt);

    if (bldAmt == 8)
    {
        Proc_Break(proc);
    }

    return;
}

#ifdef CONFIG_QUALITY_OF_LIFE_EPILOGUE_FADE

    static const struct ProcCmd gProcScr_EndingBattleDisplay_Solo_NEW[] =
    {
        PROC_YIELD,

        PROC_CALL(SoloEndingBattleDisp_Init),
        PROC_REPEAT(SoloEndingBattleDisp_Loop),
        
        PROC_SLEEP(16),

        PROC_CALL(PairedEndingBattleDisp_InitBlend),
        PROC_REPEAT(PairedEndingBattleDisp_Loop_Blend),

        PROC_END,
    };

    LYN_REPLACE_CHECK(StartSoloEndingBattleDisplay);
    void StartSoloEndingBattleDisplay(struct CharacterEndingEnt * endingEnt, struct Unit * unit, struct CharacterEndingProc * parent)
    {
        struct EndingBattleDisplayProc * proc = Proc_StartBlocking(gProcScr_EndingBattleDisplay_Solo_NEW, parent);

        proc->units[0] = unit;
        proc->units[1] = NULL;

        proc->pCharacterEnding = endingEnt;

        return;
    }

#endif

#ifdef CONFIG_QUALITY_OF_LIFE_UNIT_NAME_DROP

    struct PopupInstruction const NewItemDropPopup[] = {
        POPUP_SOUND(0x5A),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOT_ITEM),             /* got */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_ITEM_STR,
        POPUP_SPACE(1),
        POPUP_ITEM_ICON,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_SPACE(1),
        POPUP_MSG(0x022),                   /* .[.] */
        POPUP_END
    };

    struct PopupInstruction const NewItemPilferedPopup[] = {
        POPUP_SOUND(0x5A),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_PILFERED_ITEM),       /* pilfered */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_ITEM_STR,
        POPUP_SPACE(1),
        POPUP_ITEM_ICON,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_SPACE(1),
        POPUP_MSG(0x022),                   /* .[.] */
        POPUP_END
    };

    struct PopupInstruction const NewItemStolePopup[] = {
        POPUP_SOUND(0x5C),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_STOLE_ITEM),          /* Stole */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_ITEM_STR,
        POPUP_SPACE(1),
        POPUP_ITEM_ICON,
        POPUP_SPACE(1),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(0x022),                   /* .[.] */
        POPUP_END
    };

    struct PopupInstruction const NewGoldGotPopup[] = {
        POPUP_SOUND(0x5A),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOT_ITEM),               /* got */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_NUM,
        POPUP_SPACE(3),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOLD),                   /* gold.[.] */
        POPUP_END
    };

    struct PopupInstruction const NewGoldStolenPopup[] = {
        POPUP_SOUND(0x5C),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_STOLE_ITEM),              /* Stole */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_NUM,
        POPUP_SPACE(3),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOLD),                   /* gold. */
        POPUP_END
    };

    LYN_REPLACE_CHECK(NewPopup_ItemGot_unused);
    void NewPopup_ItemGot_unused(struct Unit* unit, u16 item, ProcPtr parent)
    {
        SetPopupItem(item);
        SetPopupUnit(unit);

        if (FACTION_BLUE == UNIT_FACTION(unit))
            NewPopup_Simple(NewItemDropPopup, 0x60, 0x0, parent);
        else
            NewPopup_Simple(NewItemPilferedPopup, 0x60, 0x0, parent);
    }

    LYN_REPLACE_CHECK(NewPopup_GoldGot);
    void NewPopup_GoldGot(ProcPtr parent, struct Unit *unit, int value)
    {
        SetPopupNumber(value);
        SetPopupUnit(unit);

        if (FACTION_BLUE == UNIT_FACTION(unit)) 
        {
            value += GetPartyGoldAmount();
            SetPartyGoldAmount(value);
            NewPopup_Simple(NewGoldGotPopup, 0x60, 0x0, parent);
        } 
        else
            NewPopup_Simple(NewGoldStolenPopup, 0x60, 0x0, parent);
    }

    LYN_REPLACE_CHECK(NewPopup_ItemStealing);
    void NewPopup_ItemStealing(u16 item, ProcPtr parent)
    {
        SetPopupItem(item);
        SetPopupUnit(gActiveUnit);

        NewPopup_Simple(NewItemStolePopup, 0x60, 0x0, parent);
    }

#endif

LYN_REPLACE_CHECK(MenuCancelSelect);
u8 MenuCancelSelect(struct MenuProc* menu, struct MenuItemProc* item)
{
    
    /*
    ** So we reset this value here after exiting the forge menu.
    ** This way, the left and right dPad buttons do not take effect in other menus
    */
    
#ifdef CONFIG_FORGING
    if (gActionData.unk08 == 10000)
    {
        gActionData.unk08 = 0;
    }
#endif

    return MENU_ACT_SKIPCURSOR | MENU_ACT_CLEAR | MENU_ACT_END | MENU_ACT_SND6B;
}

struct ProcCmd const ProcScr_ChapterStatusScreen_FromPrep_NEW[] =
{
    PROC_YIELD,

    PROC_CALL(ChapterStatus_Init),
    PROC_CALL(ChapterStatus_DrawText),
    PROC_YIELD,

    PROC_CALL(ChapterStatus_ShowAllLayers),
    PROC_CALL(FadeInBlackSpeed40),
    PROC_YIELD,

PROC_LABEL(0),
    PROC_REPEAT(ChapterStatus_LoopKeyHandler),

PROC_LABEL(1),
    PROC_CALL(sub_8013F58),
    PROC_YIELD,

    PROC_CALL(EndMuralBackground),
    PROC_CALL(ChapterStatus_OnEnd),

    PROC_END,
};

//! FE8U = 0x0808E79C
LYN_REPLACE_CHECK(StartChapterStatusScreen_FromPrep);
void StartChapterStatusScreen_FromPrep(ProcPtr parent)
{
    struct ChapterStatusProc * proc = Proc_StartBlocking(ProcScr_ChapterStatusScreen_FromPrep_NEW, parent);
    proc->unk_3f = 1;

    return;
}