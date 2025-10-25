#include "common-chax.h"
#include "stat-screen.h"
#include "lvup.h"
#include "strmag.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _growth_disp(int x, int y, int growth)
{
#ifdef CONFIG_GROWTHS_AS_LETTERS
    FORCE_DECLARE int character1 = 0;
    FORCE_DECLARE int character2 = 0;

    if (growth >= 100)
    {
        character1 = TEXT_SPECIAL_S;
    }
    else if (growth >= 90)
    {
        character1 = TEXT_SPECIAL_A;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 80)
    {
        character1 = TEXT_SPECIAL_A;
    }
    else if (growth >= 70)
    {
        character1 = TEXT_SPECIAL_B;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 60)
    {
        character1 = TEXT_SPECIAL_B;
    }
    else if (growth >= 50)
    {
        character1 = TEXT_SPECIAL_C;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 40)
    {
        character1 = TEXT_SPECIAL_C;
    }
    else if (growth >= 30)
    {
        character1 = TEXT_SPECIAL_D;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 20)
    {
        character1 = TEXT_SPECIAL_D;
    }
    else if (growth >= 10)
    {
        character1 = TEXT_SPECIAL_E;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth > 0)
    {
        character1 = TEXT_SPECIAL_E;
    }
    else 
    {
        character1 = TEXT_SPECIAL_BIGNUM_0;
    }

    
    PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(x-1, y),
    TEXT_COLOR_SYSTEM_BLUE,
    character1);

    if (character2 > 0)
    {
        PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(x, y),
        TEXT_COLOR_SYSTEM_BLUE,
        character2);
    }
#else

#ifndef CONFIG_GREEN_BONUS_GROWTHS
    int bank, color = GetTextColorFromGrowth(growth);

    ModifyTextPal(bank, color);
    gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

    PutNumberOrBlank(
        gBG0TilemapBuffer + TILEMAP_INDEX(x, y),
        color,
        growth);

#else
    PutNumberOrBlank(
        gBG0TilemapBuffer + TILEMAP_INDEX(x, y),
        TEXT_COLOR_SYSTEM_BLUE,
        growth);
#endif

#endif
}

STATIC_DECLAR void ToggleUnitPageGrowth(void)
{
    struct Unit *unit = gStatScreen.unit;

#ifdef CONFIG_GREEN_BONUS_GROWTHS
    _growth_disp(17, 3, unit->pCharacterData->growthPow);
    _growth_disp(17, 5, GetUnitBasicMagGrowth(unit));
    _growth_disp(17, 7, unit->pCharacterData->growthSkl);
    _growth_disp(17, 9, unit->pCharacterData->growthSpd);
    _growth_disp(17, 11, unit->pCharacterData->growthLck);
    _growth_disp(17, 13, unit->pCharacterData->growthDef);
    _growth_disp(17, 15, unit->pCharacterData->growthRes);

    PutNumberBonus((GetUnitPowGrowth(unit) - unit->pCharacterData->growthPow), (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 3)));
    PutNumberBonus((GetUnitMagGrowth(unit) - GetUnitBasicMagGrowth(unit)),     (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 5)));
    PutNumberBonus((GetUnitSklGrowth(unit) - unit->pCharacterData->growthSkl), (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 7)));
    PutNumberBonus((GetUnitSpdGrowth(unit) - unit->pCharacterData->growthSpd), (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 9)));
    PutNumberBonus((GetUnitLckGrowth(unit) - unit->pCharacterData->growthLck), (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 11)));
    PutNumberBonus((GetUnitDefGrowth(unit) - unit->pCharacterData->growthDef), (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 13)));
    PutNumberBonus((GetUnitResGrowth(unit) - unit->pCharacterData->growthRes), (gBG0TilemapBuffer + TILEMAP_INDEX(17 + 1, 15)));

#else 
    _growth_disp(17, 3, GetUnitPowGrowth(unit));
    _growth_disp(17, 5, GetUnitMagGrowth(unit));
    _growth_disp(17, 7, GetUnitSklGrowth(unit));
    _growth_disp(17, 9, GetUnitSpdGrowth(unit));
    _growth_disp(17, 11, GetUnitLckGrowth(unit));
    _growth_disp(17, 13, GetUnitDefGrowth(unit));
    _growth_disp(17, 15, GetUnitResGrowth(unit));
#endif

    ResetActiveFontPal();
}

STATIC_DECLAR void ToggleUnitPageBm(void)
{
	 struct Unit *unit = gStatScreen.unit;

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
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          unit->pow,
                          GetUnitPower(unit),
                          UNIT_POW_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(1, 0x5, 0x3,
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          UNIT_MAG(unit),
                          GetUnitMagic(unit),
                          GetUnitMaxMagic(unit) + limitBreaker);

    DrawStatWithBarRework(2, 0x5, 0x5,
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          unit->skl,
                          GetUnitSkill(unit),
                          UNIT_SKL_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(3, 0x5, 0x7,
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          unit->spd,
                          GetUnitSpeed(unit),
                          UNIT_SPD_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(4, 0x5, 0x9,
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          unit->lck,
                          GetUnitLuck(unit),
                          UNIT_LCK_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(5, 0x5, 0xB,
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          unit->def,
                          GetUnitDefense(unit),
                          UNIT_DEF_MAX(unit) + limitBreaker);

    DrawStatWithBarRework(6, 0x5, 0xD,
                          gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
                          gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
                          unit->res,
                          GetUnitResistance(unit),
                          UNIT_RES_MAX(unit) + limitBreaker);
}



STATIC_DECLAR void ToggleUnitPage(bool toggle)
{
	TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 16, 3), 4, 13, 0);
	if (toggle == false)
		ToggleUnitPageBm();
	else {
		TileMap_FillRect(TILEMAP_LOCATED(gBG2TilemapBuffer, 15, 3), 6, 13, 0);
		ToggleUnitPageGrowth();
	}

	ToggleUnitLeftPage(toggle);
	BG_EnableSyncByMask(BG0_SYNC_BIT | BG2_SYNC_BIT);
}

LYN_REPLACE_CHECK(PageNumCtrl_DisplayBlinkIcons);
void PageNumCtrl_DisplayBlinkIcons(struct StatScreenPageNameProc *proc)
{
	bool blinking;
	bool saviour_pairup = false;
	static const u16 palidLut[3] = { 0xC, 0xE, 0xD }; // TODO: palid constants

	/* No idle in transition */
	if (gStatScreen.inTransition)
		return;

	blinking = (GetGameClock() % 32) < 20;

	if (gStatScreen.page == STATSCREEN_PAGE_0)
	{
        if (gStatScreen.unit->state & US_RESCUING)
        {
#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
            if (SkillTester(gStatScreen.unit, SID_PairUp))
                saviour_pairup = true;
#endif
            if (!saviour_pairup)
            {
                UpdateStatArrowSprites(120, 56, 1);
                UpdateStatArrowSprites(120, 72, 1);
            }

            if (blinking)
            {
                if (gStatScreenStExpa.talkee == 0)
                {
                    PutSprite(4,
                              184, 94, gObject_8x8,
                              TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue >> 6]) + OAM2_LAYER(2));
                }
                else
                {
                    PutSprite(4,
                              28, 86, gObject_8x8,
                              TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue >> 6]) + OAM2_LAYER(2));
                }
            }
        }
    }

	if ((gStatScreen.unit->state & US_RESCUED) && blinking)
		PutSprite(4,
			28, 86, gObject_8x8,
			TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue>>6]) + OAM2_LAYER(2));

	/**
	 * We direcly put page toggle here since here has been a hook during statscreen IDLE.
	 * It's true that better to put such process into StatScreen_OnIdle(), but both are okay.
	 */
	switch (gpKernelDesigerConfig->unit_page_style) {
	case CONFIG_PAGE1_WITH_BWL:
	case CONFIG_PAGE1_WITH_LEADERSHIP:
		if ((gStatScreen.page == STATSCREEN_PAGE_0) && (UNIT_FACTION(gStatScreen.unit) == FACTION_BLUE)) {
			if (gStatScreenStExpa.toggle_timer == 0) {
				if ((gKeyStatusPtr->newKeys & SELECT_BUTTON))
					gStatScreenStExpa.toggle_timer = STATSCREEN_TOGGLE_DURATION;
			} else {
				gStatScreenStExpa.toggle_timer--;
				if (gStatScreenStExpa.toggle_timer == (STATSCREEN_TOGGLE_DURATION / 2)) {
					PlaySoundEffect(0x6A);

					gStatScreenStExpa.toggle = ~gStatScreenStExpa.toggle;
					ToggleUnitPage(gStatScreenStExpa.toggle);
				}
			}
		}

		/* Refrain left panel */
		if (gStatScreenStExpa.toggle && gStatScreen.page != STATSCREEN_PAGE_0) {
			gStatScreenStExpa.toggle_timer = 0;
			gStatScreenStExpa.toggle = false;
			ToggleUnitLeftPage(false);
			BG_EnableSyncByMask(BG0_SYNC_BIT);
		}
		break;

	default:
		break;
	}
}

LYN_REPLACE_CHECK(GlowBlendCtrl_OnLoop);
void GlowBlendCtrl_OnLoop(struct StatScreenEffectProc *proc)
{
	/**
	 * Since BG1 may also consume chr resource on texts,
	 * so I think it is better to directly remove them.
	 */
	return;
}
