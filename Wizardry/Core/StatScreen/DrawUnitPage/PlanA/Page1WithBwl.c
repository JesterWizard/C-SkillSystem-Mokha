#include "common-chax.h"
#include "stat-screen.h"
#include "strmag.h"
#include "bwl.h"
#include "lvup.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "kernel-glyph.h"
#include "constants/texts.h"
#include "skill-system.h"
#include "constants/skills.h"

static void DrawPage1TextCommon(void)
{
    struct Unit * unit = gStatScreen.unit;

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_POWLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x1),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitPowGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FE)); // Str

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_ITEM0],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x3),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitMagGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FF)); // Mag

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_SKLLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x5),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitSklGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EC)); // Skl

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_SPDLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x7),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitSpdGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4ED)); // Spd

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_LCKLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x9),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitLckGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EE)); // Lck

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_DEFLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0xB),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitDefGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EF)); // Def

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_RESLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0xD),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitResGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F0)); // Res

    /* All growth related value done */
    ResetActiveFontPal();

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_MOVLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x1),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F6)); // Mov

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_CONLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F7)); // Con

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_AIDLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F8)); // Aid

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_SUPPORT4],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F1)); // Affin

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_STATUS],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0xB),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FA)); // Cond
}

static void DrawPage1ValueReal(void)
{
    struct Unit * unit = gStatScreen.unit;

    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

    DrawStatWithBarRework(0, 0x5, 0x1,
                    gUiTmScratchA, gUiTmScratchC,
                    unit->pow,
                    GetUnitPower(unit),
                    UNIT_POW_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(1, 0x5, 0x3,
                    gUiTmScratchA, gUiTmScratchC,
                    UNIT_MAG(unit),
                    GetUnitMagic(unit),
                    GetUnitMaxMagic(unit) + limitBreaker);

    DrawStatWithBarRework(2, 0x5, 0x5,
                    gUiTmScratchA, gUiTmScratchC,
                    unit->skl,
                    GetUnitSkill(unit),
                    UNIT_SKL_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(3, 0x5, 0x7,
                    gUiTmScratchA, gUiTmScratchC,
                    unit->spd,
                    GetUnitSpeed(unit),
                    UNIT_SPD_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(4, 0x5, 0x9,
                    gUiTmScratchA, gUiTmScratchC,
                    unit->lck,
                    GetUnitLuck(unit),
                    UNIT_LCK_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(5, 0x5, 0xB,
                    gUiTmScratchA, gUiTmScratchC,
                    unit->def,
                    GetUnitDefense(unit),
                    UNIT_DEF_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(6, 0x5, 0xD,
                    gUiTmScratchA, gUiTmScratchC,
                    unit->res,
                    GetUnitResistance(unit),
                    UNIT_RES_MAX(unit) + limitBreaker);
}

static void DrawPage1ValueCommon(void)
{
    struct Unit * unit = gStatScreen.unit;

    DrawStatWithBarRework(7, 0xD, 0x1,
                    gUiTmScratchA, gUiTmScratchC,
                    UNIT_MOV(unit),
                    MovGetter(unit),
                    UNIT_MOV_MAX(unit));

    DrawStatWithBarRework(8, 0xD, 0x3,
                    gUiTmScratchA, gUiTmScratchC,
                    UNIT_CON_BASE(unit),
                    ConGetter(unit),
                    UNIT_CON_MAX(unit));

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0xD, 0x5),
        TEXT_COLOR_SYSTEM_BLUE,
        GetUnitAid(unit));

    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0xE, 0x5),
             GetUnitAidIconId(UNIT_CATTRIBUTES(unit)),
             TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

    if (GetUnitStatusIndex(unit) == UNIT_STATUS_NONE)
    {
        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_STATUS],
            24, TEXT_COLOR_SYSTEM_BLUE,
            GetUnitStatusName(unit));
    }
    else
    {
        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_STATUS],
            22, TEXT_COLOR_SYSTEM_BLUE,
            GetUnitStatusName(unit));
    }

   if (GetUnitStatusIndex(gStatScreen.unit) != UNIT_STATUS_NONE && GetUnitStatusIndex(gStatScreen.unit) != NEW_UNIT_STATUS_DISMOUNT)
    {
        PutNumberSmall(
            gUiTmScratchA + TILEMAP_INDEX(0x10, 0xB),
            0,
            GetUnitStatusDuration(unit));
    }
}

static void DrawPage1BattleAmt(void)
{
    int amt = GetUnitBattleAmt(gStatScreen.unit);

#ifdef CONFIG_TELLIUS_CAPACITY_SYSTEM
    int max = CONFIG_TELLIUS_CAPACITY_BASE;

    if (UNIT_CATTRIBUTES(gStatScreen.unit) & CA_PROMOTED)
        max += CONFIG_TELLIUS_CAPACITY_PROMOTED;
#else
    int max = 50 * 7;
#endif

    if (amt > max)
        amt = max;

#ifdef CONFIG_TELLIUS_CAPACITY_SYSTEM
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_ITEM3],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0xD),
        TEXT_COLOR_SYSTEM_GOLD, 0, 0,
        GetStringFromIndex(MSG_MSS_SkillCapacity));
#else
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_ITEM3],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0xD),
        TEXT_COLOR_SYSTEM_GOLD, 0, 0,
        GetStringFromIndex(MSG_MSS_BattleAmt));
#endif

if (amt == max)
{
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(0xC + CountDigits(amt), 0xD),
        TEXT_COLOR_SYSTEM_GREEN, amt);
}
else
{
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(0xC + CountDigits(amt), 0xD),
        TEXT_COLOR_SYSTEM_BLUE, amt);
}

    DrawStatWithBarReworkExt(
        0x9, 0xD, 0xD,
        gUiTmScratchC,
        amt, amt, max, max);
}

static u8 GetUnitLaguzBarValue(struct Unit * unit)
{
    struct NewBwl * bwl;
    u8 pid = UNIT_CHAR_ID(unit);
    bwl = GetNewBwl(pid);

    if (pid > 0x40)
        return 0;
    else
        return bwl->_pad_[1];
}

FORCE_DECLARE static void DrawPage1LaguzBar(void)
{
    int amt = GetUnitLaguzBarValue(gStatScreen.unit);
    int max = 30;
    int textColor = (amt == max) ? TEXT_COLOR_SYSTEM_GREEN : TEXT_COLOR_SYSTEM_BLUE;
    
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_ITEM4],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0xF),
        TEXT_COLOR_SYSTEM_GOLD, 0, 0,
        GetStringFromIndex(MSG_MSS_LaguzBar));

    PutNumber(gUiTmScratchA + TILEMAP_INDEX(0xC + CountDigits(amt), 0xF),
        textColor, amt);

    DrawStatWithBarReworkExt(
        0xA, 0xD, 0xF,
        gUiTmScratchC,
        amt, amt, max, max);
}

/* BWL */
void DrawPage1BWL(void)
{
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(gStatScreen.unit));
    if (!bwl)
        return;

    if (UNIT_FACTION(gStatScreen.unit) != FACTION_BLUE)
        return;

    ClearText(&gStatScreen.text[STATSCREEN_TEXT_BWL]);

#if 0
    if (gpKernelDesigerConfig->use_chinese_character == true)
    {
        // Draw B label
        Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
            0, TEXT_COLOR_SYSTEM_GOLD,
            GetStringFromIndex(MSG_MSS_BWL_BATTLE_JP));

        // Draw W label
        Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
            32, TEXT_COLOR_SYSTEM_GOLD,
            GetStringFromIndex(MSG_MSS_BWL_WIN_JP));

        // Draw L label
        Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
            64, TEXT_COLOR_SYSTEM_GOLD,
            GetStringFromIndex(MSG_MSS_BWL_LOSE_JP));
    }
    else
#else
    /* Well, not enough length */
    if (1)
#endif
    {
        // Draw B label
        Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
            0, TEXT_COLOR_SYSTEM_GOLD,
            GetStringFromIndex(MSG_MSS_BWL_BATTLE));

        // Draw W label
        Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
            32, TEXT_COLOR_SYSTEM_GOLD,
            GetStringFromIndex(MSG_MSS_BWL_WIN));

        // Draw L label
        Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
            64, TEXT_COLOR_SYSTEM_GOLD,
            GetStringFromIndex(MSG_MSS_BWL_LOSE));
    }

    // Display labels
    PutText(&gStatScreen.text[STATSCREEN_TEXT_BWL],
        gUiTmScratchA + TILEMAP_INDEX(3, 0xF));

    // Display Battle Amt
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(3 + CountDigits(bwl->battleAmt), 0xF),
        TEXT_COLOR_SYSTEM_BLUE, bwl->battleAmt);

    // Display Win Amt
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(7 + CountDigits(bwl->winAmt), 0xF),
        TEXT_COLOR_SYSTEM_BLUE, bwl->winAmt);

    // Display Loss Amt
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(11 + CountDigits(bwl->lossAmt), 0xF),
        TEXT_COLOR_SYSTEM_BLUE, bwl->lossAmt);
}

static void DrawPage1Affin(void)
{
    struct Unit * unit = gStatScreen.unit;
    int affin = unit->pCharacterData->affinity;

    const char * cn_affin[] = {
        [UNIT_AFFIN_FIRE]    = "炎",
        [UNIT_AFFIN_THUNDER] = "雷",
        [UNIT_AFFIN_WIND]    = "風",
        [UNIT_AFFIN_ICE]     = "冰",
        [UNIT_AFFIN_DARK]    = "闇",
        [UNIT_AFFIN_LIGHT]   = "光",
        [UNIT_AFFIN_ANIMA]   = "理",
    };

    if (affin)
    {
        DrawIcon(
            gUiTmScratchA + TILEMAP_INDEX(0xC, 0x7),
            GetUnitAffinityIcon(unit),
            TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

        if (gpKernelDesigerConfig->use_chinese_character == true)
        {
            PutDrawText(
                &gStatScreen.text[STATSCREEN_TEXT_ITEM1],
                gUiTmScratchA + TILEMAP_INDEX(0xE, 0x7),
                TEXT_COLOR_SYSTEM_GOLD,
                0, 0,
                cn_affin[affin]);
        }
    }
    else
    {
        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_SUPPORT4],
            24, TEXT_COLOR_SYSTEM_BLUE,
            GetStringFromIndex(0x536));
    }
}

static void DrawPage1TalkTrv(void)
{
    if (gStatScreenStExpa.talkee != 0)
    {
        /* Talk */
        PutDrawText(
            &gStatScreen.text[STATSCREEN_TEXT_RESCUENAME],
            gUiTmScratchA + TILEMAP_INDEX(0x9, 0x9),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            GetStringFromIndex(MSG_MSS_TALK));

        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_RESCUENAME],
            24, TEXT_COLOR_SYSTEM_BLUE,
            GetStringFromIndex(UNIT_NAME_ID(GetUnitFromCharId(gStatScreenStExpa.talkee))));
    }
    else
    {
        /* Rescue */
        PutDrawText(
            &gStatScreen.text[STATSCREEN_TEXT_RESCUENAME],
            gUiTmScratchA + TILEMAP_INDEX(0x9, 0x9),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            GetStringFromIndex(0x4F9));

        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_RESCUENAME],
            24, TEXT_COLOR_SYSTEM_BLUE,
            GetUnitRescueName(gStatScreen.unit));
    }
}

/* External hook */
void DisplayPage_WithBWL(void)
{
    int i;

    for (i = STATSCREEN_TEXT_POWLABEL; i < STATSCREEN_TEXT_BSATKLABEL; i++)
        ClearText(&gStatScreen.text[i]);

    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT3]);
    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT4]);

    DrawPage1TextCommon();
    DrawPage1ValueReal();
    DrawPage1ValueCommon();
    DrawPage1BattleAmt();
#ifdef CONFIG_LAGUZ_BARS
    DrawPage1LaguzBar();
#else
    DrawPage1BWL();
#endif
    DrawPage1Affin();
    DrawPage1TalkTrv();
}
