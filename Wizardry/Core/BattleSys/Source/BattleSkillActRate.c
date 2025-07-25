#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool CheckBattleSkillActivate(struct BattleUnit * actor, struct BattleUnit * target, int sid, int rate)
{
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
        return false;

    if (!BattleSkillTester(actor, sid))
        return false;

#if (defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight)))
    if (BattleSkillTester(target, SID_Foresight))
        return false;
#endif

#if (defined(SID_RightfulArch) && (COMMON_SKILL_VALID(SID_RightfulArch)))
    if (BattleSkillTester(actor, SID_RightfulArch))
        return true;
#endif

#if (defined(SID_RightfulKing) && (COMMON_SKILL_VALID(SID_RightfulKing)))
    if (BattleSkillTester(actor, SID_RightfulKing))
        rate += SKILL_EFF0(SID_RightfulKing);
#endif

#if (defined(SID_RightfulGod) && (COMMON_SKILL_VALID(SID_RightfulGod)))
    if (BattleSkillTester(actor, SID_RightfulGod))
        rate += SKILL_EFF0(SID_RightfulGod);
#endif

#if (defined(SID_Hero) && (COMMON_SKILL_VALID(SID_Hero)))
    if (BattleSkillTester(actor, SID_Hero))
        if ((actor->hpInitial * 2) < actor->unit.maxHP)
            rate += SKILL_EFF0(SID_Hero);
#endif

#if (defined(SID_SereneGrace) && (COMMON_SKILL_VALID(SID_SereneGrace)))
    if (BattleSkillTester(actor, SID_SereneGrace))
        rate *= 2;
#endif

    LIMIT_AREA(rate, 0, 100);

    if (Roll2RN(rate))
        return true;

    return false;
}
