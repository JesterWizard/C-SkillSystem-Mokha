#include "common-chax.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#ifndef CONFIG_INSTALL_CONVOYEXPA_AMT
    #define CONFIG_INSTALL_CONVOYEXPA_AMT 200
#endif

extern int sExpaConvoyItemCount; // gConvoyItemCount is just a u8 that can be no more than 255, not enough
extern u16 sExpaConvoyItemArray[CONFIG_INSTALL_CONVOYEXPA_AMT];

#ifdef CONFIG_TURN_ON_ALL_SKILLS
struct LearnedSkillList {
    u32 data[32]; // 32 * 32 = 1024 bits needed for 10-bit skill IDs
};
#else
struct LearnedSkillList {
    u32 data[8];
};
#endif

extern struct LearnedSkillList sLearnedSkillPLists[NEW_BWL_ARRAY_NUM];

#ifdef CONFIG_USE_DEBUG
void * const Identifier_sLearnedSkillPLists = sLearnedSkillPLists;
#endif

/* GameInitHook */
void ResetUnitLearnedSkillLists(void)
{
    CpuFill16(0, sLearnedSkillPLists, sizeof(sLearnedSkillPLists));
}

/* SaveData */
void SaveUnitLearnedSkillLists(u8 * dst, const u32 size)
{
    Assert(size == sizeof(sLearnedSkillPLists));

    WriteAndVerifySramFast(sLearnedSkillPLists, dst, size);
}

/* LoadData */
void LoadUnitLearnedSkillLists(u8 * src, const u32 size)
{
    Assert(size == sizeof(sLearnedSkillPLists));

    ReadSramFast(src, sLearnedSkillPLists, size);
}

bool IsSkillLearned(struct Unit * unit, const u16 sid)
{
    u8 pid = UNIT_CHAR_ID(unit);
    if (EQUIP_SKILL_VALID(sid) && pid < NEW_BWL_ARRAY_NUM && _BIT_CHK(sLearnedSkillPLists[pid].data, sid))
        return true;

    struct SkillList *list;
    list = GetUnitSkillList(unit);

    for (int i = 0; i < list->amt; i++)
    {
        if (list->sid[i] == sid)
            return true;
    }

    return false;
}

void LearnSkill(struct Unit * unit, const u16 sid)
{
    u8 pid = UNIT_CHAR_ID(unit);

    /* Make sure that the enemy is not effective on allies */
    if (UNIT_FACTION(unit) != FACTION_BLUE)
        return;

    if (EQUIP_SKILL_VALID(sid) && pid < NEW_BWL_ARRAY_NUM)
        _BIT_SET(sLearnedSkillPLists[pid].data, sid);
}

void ForgetSkill(struct Unit * unit, const u16 sid)
{
    u8 pid = UNIT_CHAR_ID(unit);
    if (EQUIP_SKILL_VALID(sid) && pid < NEW_BWL_ARRAY_NUM)
        _BIT_CLR(sLearnedSkillPLists[pid].data, sid);
}

struct PopupInstruction const PopupScr_LearnSkill[] = {
    POPUP_SOUND(0x5A),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_Learned),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_GOLD),
    POPUP_ITEM_STR,
    POPUP_SPACE(1),
    POPUP_ITEM_ICON,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_SPACE(1),
    POPUP_END
};

struct PopupInstruction const PopupScr_ObtainedSkill[] = {
    POPUP_SOUND(0x5A),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_Obtained),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_GOLD),
    POPUP_ITEM_STR,
    POPUP_SPACE(1),
    POPUP_ITEM_ICON,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_SPACE(1),
    POPUP_END
};