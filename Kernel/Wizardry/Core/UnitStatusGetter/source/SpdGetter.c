#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"
#include "bwl.h"
#include "unit-expa.h"
#include "debuff.h"
#include "jester_headers/custom-functions.h"

int _GetUnitSpeed(struct Unit *unit)
{
	const StatusGetterFunc_t *it;
	int status = unit->spd;

   if (unit->state & US_RESCUING) {
        bool hasPairUp = false;
        bool hasSavior = false;

#if defined(SID_PairUp) && COMMON_SKILL_VALID(SID_PairUp)
        hasPairUp = SkillTester(unit, SID_PairUp);
#endif

#if defined(SID_Savior) && COMMON_SKILL_VALID(SID_Savior)
        hasSavior = SkillTester(unit, SID_Savior);
#endif

        if (!hasPairUp && !hasSavior) {
            status = unit->skl / 2;
        }
    }

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

	for (it = gpSpdGetters; *it; it++)
		status = (*it)(status, unit);

	if (gpExternalSpdGetters)
		status = gpExternalSpdGetters(status, unit);

#ifdef SETH_INJURED
    if (unit->pCharacterData->number == CHARACTER_SETH)
        status -= gPlaySt.chapterIndex < INJURED_TURN_COUNT ? INJURED_TURN_COUNT - gPlaySt.chapterIndex : 0;
#endif

	return status;
}

/* Hooks */
int SpdGetterWeaponBonus(int status, struct Unit *unit)
{
	u16 weapon = GetUnitEquippedWeapon(unit);

	status += GetItemSpdBonus(weapon);
	return status;
}

int SpdGetterSkills(int status, struct Unit *unit)
{
	int cur_hp = GetUnitCurrentHp(unit);
	int max_hp = GetUnitMaxHp(unit);

    FORCE_DECLARE struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
	if (SkillTester(unit, SID_LifeAndDeath))
		status += SKILL_EFF0(SID_LifeAndDeath);
#endif

#if defined(SID_SpdBonus) && (COMMON_SKILL_VALID(SID_SpdBonus))
	if (SkillTester(unit, SID_SpdBonus))
		status += SKILL_EFF0(SID_SpdBonus);
#endif

#if defined(SID_DefiantSpd) && (COMMON_SKILL_VALID(SID_DefiantSpd))
	if (SkillTester(unit, SID_DefiantSpd))
		if ((cur_hp * 4) < max_hp)
			status += SKILL_EFF0(SID_DefiantSpd);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
	if (SkillTester(unit, SID_Fury))
		status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
	if (SkillTesterPlus(unit, SID_FuryPlus))
		status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
	if (SkillTester(unit, SID_RuinedBlade))
		status += SKILL_EFF1(SID_RuinedBlade);
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
	if (SkillTesterPlus(unit, SID_RuinedBladePlus))
		status += SKILL_EFF0(SID_RuinedBladePlus);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
	if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SPD)
		status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_SpeedBoost) && (COMMON_SKILL_VALID(SID_SpeedBoost))
	if (SkillTester(unit, SID_SpeedBoost)) {
		int __buf = SKILL_EFF0(SID_SpeedBoost);

		if (gPlaySt.chapterTurnNumber >= __buf)
			status += __buf;
		else
			status += gPlaySt.chapterTurnNumber;
	}
#endif

#if defined(SID_SpectrumBoost) && (COMMON_SKILL_VALID(SID_SpectrumBoost))
	if (SkillTester(unit, SID_SpectrumBoost)) {
		int __buf = SKILL_EFF0(SID_SpectrumBoost);

		if (gPlaySt.chapterTurnNumber >= __buf)
			status += __buf;
		else
			status += gPlaySt.chapterTurnNumber;
	}
#endif

#if (defined(SID_TakerSpeed) && (COMMON_SKILL_VALID(SID_TakerSpeed)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
    if (SkillTester(unit, SID_TakerSpeed))
    {
        int takerBoost = bwl->winAmt * SKILL_EFF0(SID_TakerSpeed);
        
        if (takerBoost > 10)
            status += 10;
        else   
            status += takerBoost;
    }
#endif

#if (defined(SID_TakerSpectrum) && (COMMON_SKILL_VALID(SID_TakerSpectrum)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
    if (SkillTester(unit, SID_TakerSpectrum))
    {
        int takerBoost = bwl->winAmt * SKILL_EFF0(SID_TakerSpectrum);
        
        if (takerBoost > 10)
            status += 10;
        else   
            status += takerBoost;
    }
#endif

#if defined(SID_Freelancer) && (COMMON_SKILL_VALID(SID_Freelancer))
    if (SkillTester(unit, SID_Freelancer) && unit->ranks[ITYPE_SWORD] >= 1)
        status += SKILL_EFF0(SID_Freelancer);
#endif

	if (cur_hp == max_hp) {
#if defined(SID_PushSpeed) && (COMMON_SKILL_VALID(SID_PushSpeed))
		if (SkillTester(unit, SID_PushSpeed))
			status += SKILL_EFF0(SID_PushSpeed);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
		if (SkillTester(unit, SID_PushSpectrum))
			status += SKILL_EFF0(SID_PushSpectrum);
#endif
	}

#if defined(SID_LightWeight) && (COMMON_SKILL_VALID(SID_LightWeight))
	if (SkillTester(unit, SID_LightWeight))
		if (unit->items[3] == 0)
			status += SKILL_EFF0(SID_LightWeight);
#endif

#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve)))
	if (SkillTester(unit, SID_Resolve)) {
		if ((cur_hp * 2) < max_hp)
			status += status / 2;
	}
#endif

#if (defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage))) 
    if (SkillTester(unit, SID_Rampage))
            status += unit->spd / 2;
#endif

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitSpeed(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

#if defined(SID_Sellsword) && (COMMON_SKILL_VALID(SID_Sellsword))
    if (SkillTester(unit, SID_Sellsword) && CheckBitUES(unit, UES_BIT_SELLSWORD_SKILL_USED))
        status += SKILL_EFF0(SID_Sellsword);
#endif

#if defined(SID_GoldenGlory) && (COMMON_SKILL_VALID(SID_GoldenGlory))
    if (SkillTester(unit, SID_GoldenGlory))
    {
        const int statBoost = gPlaySt.partyGoldAmount / 20000;
        status += statBoost > 5 ? 5 : statBoost;
    }
#endif

#if defined(SID_SupremeOverlord) && (COMMON_SKILL_VALID(SID_SupremeOverlord))
        if (SkillTester(unit, SID_SupremeOverlord))
        {
            int deadAllies = 0;

            for (int i = UNIT_FACTION(unit) + 1; i < (UNIT_FACTION(unit) + GetFactionUnitAmount(UNIT_FACTION(unit))); i++)
            {
                if (!(GetUnit(i)->pCharacterData))
                    break;
                if (!UNIT_ALIVE(GetUnit(i)))
                    deadAllies += 1;
            }

            if (UNIT_FACTION(unit) != FACTION_RED)
                status += deadAllies * 3;
            else
                status += deadAllies / 3;
        }
#endif

#if defined(SID_SlowStart) && (COMMON_SKILL_VALID(SID_SlowStart))
    if (SkillTester(unit, SID_SlowStart) && gPlaySt.chapterTurnNumber < 6)
        status -= Div(status * SKILL_EFF0(SID_SlowStart), 100);
#endif

	return status;
}

int SpdPsychUpCheck(int status, struct Unit *unit)
{
    FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = SpdGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) + 
                        SpdGetterSkills(0, GetUnit(gBattleTarget.unit.index)) +
                        GetStatDebuffMsgBuf(GetUnit(gBattleTarget.unit.index))->spd;

        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = SpdGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) + 
                        SpdGetterSkills(0, GetUnit(gBattleTarget.unit.index)) +
                        GetStatDebuffMsgBuf(GetUnit(gBattleTarget.unit.index))->spd;
                        
        return status + stolen_status;
    }
#endif

    return status;
}


int SpdGetterStaffBoost(int status, struct Unit *unit)
{
    if (unit->boostType == 3)
	    return status + unit->barrierDuration;

    return status;
}