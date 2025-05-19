#include "common-chax.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "skill-system.h"

#define LOCAL_TRACE 0

// ---------------------------------------------------------------
// Skill management

static void SortRamSkillList(struct Unit * unit)
{
    int i, cnt = 0;
    u16 buf[UNIT_RAM_SKILLS_LEN] = {0};

    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++) {
        u16 sid = GET_SKILL(unit, i);
        if (EQUIP_SKILL_VALID(sid))
            buf[cnt++] = sid;
    }

    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
        SET_SKILL(unit, i, i < cnt ? buf[i] : 0);
}

inline int GetSkillSlot(struct Unit * unit, int sid)
{
    for (int i = 0; i < RAM_SKILL_LEN_EXT; i++)
        if (GET_SKILL(unit, i) == sid)
            return i;

    return -1;
}

inline int GetFreeSkillSlot(struct Unit * unit)
{
    for (int i = 0; i < RAM_SKILL_LEN_EXT; i++)
        if (!EQUIP_SKILL_VALID(GET_SKILL(unit, i)))
            return i;

    return -1;
}

bool CanRemoveSkill(struct Unit * unit, const u16 sid)
{
    if (!EQUIP_SKILL_VALID(sid))
        return false;

    for (int i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
        if (GET_SKILL(unit, i) == sid)
            return true;

    return false;
}

int RemoveSkill(struct Unit * unit, const u16 sid)
{
    if (!EQUIP_SKILL_VALID(sid))
        return -1;

    for (int i = 0; i < UNIT_RAM_SKILLS_LEN; i++) {
        if (GET_SKILL(unit, i) == sid) {
            SET_SKILL(unit, i, 0);
            SortRamSkillList(unit);
            ResetSkillLists();
            return 0;
        }
    }
    return -1;
}

int AddSkill(struct Unit * unit, const u16 sid)
{
    if (sid >= MAX_EQUIP_SKILL_NUM)
        return -1;

    LearnSkill(unit, sid);

    if (GetSkillSlot(unit, sid) != -1)
        return 0;

    int slot = GetFreeSkillSlot(unit);
    if (slot == -1)
        return -1;

    SET_SKILL(unit, slot, sid);
    ResetSkillLists();
    return 0;
}

static inline void load_skill_ext(struct Unit * unit, u16 sid)
{
    if (EQUIP_SKILL_VALID(sid))
    {
        if (UNIT_FACTION(unit) == FACTION_BLUE)
            LearnSkill(unit, sid);

        AddSkill(unit, sid);
    }
}

void UnitAutoLoadSkills(struct Unit * unit)
{
    int i;
    int level_p, level_j;
    const struct SkillPreloadPConf * pConf = &gpSkillPreloadPData[UNIT_CHAR_ID(unit)];
    const struct SkillPreloadJConf * jConf = &gpSkillPreloadJData[UNIT_CLASS_ID(unit)];

    if (!UNIT_IS_VALID(unit))
        return;

    level_p = simple_div(unit->level, 5) * 5;
    level_j = simple_div(unit->level + GetUnitHiddenLevel(unit), 5) * 5;

    LIMIT_AREA(level_p, 0, UNIT_LEVEL_MAX_RE);
    LIMIT_AREA(level_j, 0, UNIT_RECORDED_LEVEL_MAX);

    while (level_p >= 0)
    {
        for (i = 0; i < 5; i++)
            load_skill_ext(unit, pConf->skills[level_p + i]);

        level_p = level_p - 5;
    }

    while (level_j >= 0)
    {
        for (i = 0; i < 5; i++)
            load_skill_ext(unit, jConf->skills[level_j + i]);

        level_j = level_j - 5;
    }

    /* For debug, we enable unit learn all of skills */
    if (gpKernelDesigerConfig->debug_autoload_skills && UNIT_FACTION(unit) == FACTION_BLUE)
    {
        for (i = 1; i < 254; i++)
            LearnSkill(unit, i);
    }
}

STATIC_DECLAR void TryAddSkillLvupPConf(struct Unit * unit, int level)
{
    int i;
    u16 sid;

    const struct SkillPreloadPConf * pConf = &gpSkillPreloadPData[UNIT_CHAR_ID(unit)];
    int _level = simple_div(level, 5) * 5;

    for (i = 0; i < 5; i++)
    {
        sid = pConf->skills[_level + i];

        if (EQUIP_SKILL_VALID(sid))
            AddSkill(unit, sid);
    }
}

STATIC_DECLAR void TryAddSkillLvupJConf(struct Unit * unit, int level)
{
    int i;
    u16 sid;

    const struct SkillPreloadJConf * jConf = &gpSkillPreloadJData[UNIT_CLASS_ID(unit)];
    int _level = simple_div(level, 5) * 5;

    for (i = 0; i < 5; i++)
    {
        sid = jConf->skills[_level + i];

        if (EQUIP_SKILL_VALID(sid))
            AddSkill(unit, sid);
    }
}

void TryAddSkillLvup(struct Unit * unit, int level)
{
    int _level;

    if (!UNIT_IS_VALID(unit))
        return;

    _level = level;
    if (simple_mod(_level, 5))
        TryAddSkillLvupJConf(unit, _level);

    _level = level + GetUnitHiddenLevel(unit);
    if (simple_mod(_level, 5))
        TryAddSkillLvupPConf(unit, _level);
}

void TryAddSkillPromotion(struct Unit * unit, int jid)
{
    int i;
    u16 sid;

    const struct SkillPreloadJConf * jConf = &gpSkillPreloadJData[jid];

    if (!UNIT_IS_VALID(unit))
        return;

    for (i = 0; i < 5; i++)
    {
        sid = jConf->skills[0 + i];

        if (EQUIP_SKILL_VALID(sid))
            AddSkill(unit, sid);
    }
}
