#include "common-chax.h"
#include "debuff.h"
#include "action-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionTakerSkills(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit))
        return false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
#if defined(SID_TakerStrength) && (COMMON_SKILL_VALID(SID_TakerStrength))
        if (SkillTester(unit, SID_TakerStrength) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_POW);
#endif

#if defined(SID_TakerMagic) && (COMMON_SKILL_VALID(SID_TakerMagic))
        if (SkillTester(unit, SID_TakerMagic) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_MAG);
#endif

#if defined(SID_TakerSkill) && (COMMON_SKILL_VALID(SID_TakerSkill))
        if (SkillTester(unit, SID_TakerSkill) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_SKL);
#endif

#if defined(SID_TakerSpeed) && (COMMON_SKILL_VALID(SID_TakerSpeed))
        if (SkillTester(unit, SID_TakerSpeed) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_SPD);
#endif

#if defined(SID_TakerLuck) && (COMMON_SKILL_VALID(SID_TakerLuck))
        if (SkillTester(unit, SID_TakerLuck) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_LCK);
#endif

#if defined(SID_TakerDefense) && (COMMON_SKILL_VALID(SID_TakerDefense))
        if (SkillTester(unit, SID_TakerDefense) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_DEF);
#endif

#if defined(SID_TakerResistance) && (COMMON_SKILL_VALID(SID_TakerResistance))
        if (SkillTester(unit, SID_TakerResistance) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_RES);
#endif
    }

    return true;
}
