#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

/**
 * 0: generic use
 * 1: battle actor
 * 2: battle target
 */
extern struct SkillList sSkillList[3];
#define SkillListGeneric      (&sSkillList[0])
#define SkillListBattleActor  (&sSkillList[1])
#define SkillListBattleTarget (&sSkillList[2])

/**
 * Fast list
 */
extern u32 sSkillFastList[0x40];
#define SkillFastListActor  (&sSkillFastList[0])
#define SkillFastListTarget (&sSkillFastList[0x20])

extern void (*gpExternalSkillListGenerator)(struct Unit * unit, struct SkillList * list, u16 * ref);

typedef struct {
    int base;
    int plus;
} SkillUpgrade;

const SkillUpgrade skill_upgrades[] = {
	#if defined(SID_NoGuard) && defined(SID_NoGuardPlus) && COMMON_SKILL_VALID(SID_NoGuard) && COMMON_SKILL_VALID(SID_NoGuardPlus)
		{ SID_NoGuard, SID_NoGuardPlus },
	#endif
	#if defined(SID_Canto) && defined(SID_CantoPlus) && COMMON_SKILL_VALID(SID_Canto) && COMMON_SKILL_VALID(SID_CantoPlus)
		{ SID_Canto, SID_CantoPlus },
	#endif
	#if defined(SID_Bladegift) && defined(SID_BladegiftPlus) && COMMON_SKILL_VALID(SID_Bladegift) && COMMON_SKILL_VALID(SID_BladegiftPlus)
		{ SID_Bladegift, SID_BladegiftPlus },
	#endif
	#if defined(SID_Piercegift) && defined(SID_PiercegiftPlus) && COMMON_SKILL_VALID(SID_Piercegift) && COMMON_SKILL_VALID(SID_PiercegiftPlus)
		{ SID_Piercegift, SID_PiercegiftPlus },
	#endif
	#if defined(SID_Gracegift) && defined(SID_GracegiftPlus) && COMMON_SKILL_VALID(SID_Gracegift) && COMMON_SKILL_VALID(SID_GracegiftPlus)
		{ SID_Gracegift, SID_GracegiftPlus },
	#endif
	#if defined(SID_Arcgift) && defined(SID_ArcgiftPlus) && COMMON_SKILL_VALID(SID_Arcgift) && COMMON_SKILL_VALID(SID_ArcgiftPlus)
		{ SID_Arcgift, SID_ArcgiftPlus },
	#endif
	#if defined(SID_Hackgift) && defined(SID_HackgiftPlus) && COMMON_SKILL_VALID(SID_Hackgift) && COMMON_SKILL_VALID(SID_HackgiftPlus)
		{ SID_Hackgift, SID_HackgiftPlus },
	#endif
	#if defined(SID_Stormgift) && defined(SID_StormgiftPlus) && COMMON_SKILL_VALID(SID_Stormgift) && COMMON_SKILL_VALID(SID_StormgiftPlus)
		{ SID_Stormgift, SID_StormgiftPlus },
	#endif
	#if defined(SID_Shadowgift) && defined(SID_ShadowgiftPlus) && COMMON_SKILL_VALID(SID_Shadowgift) && COMMON_SKILL_VALID(SID_ShadowgiftPlus)
		{ SID_Shadowgift, SID_ShadowgiftPlus },
	#endif
	#if defined(SID_Lumina) && defined(SID_LuminaPlus) && COMMON_SKILL_VALID(SID_Lumina) && COMMON_SKILL_VALID(SID_LuminaPlus)
		{ SID_Lumina, SID_LuminaPlus },
	#endif
	#if defined(SID_Fury) && defined(SID_FuryPlus) && COMMON_SKILL_VALID(SID_Fury) && COMMON_SKILL_VALID(SID_FuryPlus)
		{ SID_Fury, SID_FuryPlus },
	#endif
	#if defined(SID_Steal) && defined(SID_StealPlus) && COMMON_SKILL_VALID(SID_Steal) && COMMON_SKILL_VALID(SID_StealPlus)
		{ SID_Steal, SID_StealPlus },
	#endif
	#if defined(SID_Charge) && defined(SID_ChargePlus) && COMMON_SKILL_VALID(SID_Charge) && COMMON_SKILL_VALID(SID_ChargePlus)
		{ SID_Charge, SID_ChargePlus },
	#endif
	#if defined(SID_Shade) && defined(SID_ShadePlus) && COMMON_SKILL_VALID(SID_Shade) && COMMON_SKILL_VALID(SID_ShadePlus)
		{ SID_Shade, SID_ShadePlus },
	#endif
	#if defined(SID_Renewal) && defined(SID_RenewalPlus) && COMMON_SKILL_VALID(SID_Renewal) && COMMON_SKILL_VALID(SID_RenewalPlus)
		{ SID_Renewal, SID_RenewalPlus },
	#endif
	#if defined(SID_Barricade) && defined(SID_BarricadePlus) && COMMON_SKILL_VALID(SID_Barricade) && COMMON_SKILL_VALID(SID_BarricadePlus)
		{ SID_Barricade, SID_BarricadePlus },
	#endif
	#if defined(SID_Blacksmith) && defined(SID_BlacksmithPlus) && COMMON_SKILL_VALID(SID_Blacksmith) && COMMON_SKILL_VALID(SID_BlacksmithPlus)
		{ SID_Blacksmith, SID_BlacksmithPlus },
	#endif
	#if defined(SID_Armsthrift) && defined(SID_ArmsthriftPlus) && COMMON_SKILL_VALID(SID_Armsthrift) && COMMON_SKILL_VALID(SID_ArmsthriftPlus)
		{ SID_Armsthrift, SID_ArmsthriftPlus },
	#endif
	#if defined(SID_AlertStance) && defined(SID_AlertStancePlus) && COMMON_SKILL_VALID(SID_AlertStance) && COMMON_SKILL_VALID(SID_AlertStancePlus)
		{ SID_AlertStance, SID_AlertStancePlus },
	#endif
	#if defined(SID_LimitBreaker) && defined(SID_LimitBreakerPlus) && COMMON_SKILL_VALID(SID_LimitBreaker) && COMMON_SKILL_VALID(SID_LimitBreakerPlus)
		{ SID_LimitBreaker, SID_LimitBreakerPlus },
	#endif
	#if defined(SID_WaterWalking) && defined(SID_WaterWalkingPlus) && COMMON_SKILL_VALID(SID_WaterWalking) && COMMON_SKILL_VALID(SID_WaterWalkingPlus)
		{ SID_WaterWalking, SID_WaterWalkingPlus },
	#endif
	#if defined(SID_MountainClimber) && defined(SID_MountainClimberPlus) && COMMON_SKILL_VALID(SID_MountainClimber) && COMMON_SKILL_VALID(SID_MountainClimberPlus)
		{ SID_MountainClimber, SID_MountainClimberPlus },
	#endif
	#if defined(SID_TriangleAdept) && defined(SID_TriangleAdeptPlus) && COMMON_SKILL_VALID(SID_TriangleAdept) && COMMON_SKILL_VALID(SID_TriangleAdeptPlus)
		{ SID_TriangleAdept, SID_TriangleAdeptPlus },
	#endif
	#if defined(SID_Predation) && defined(SID_PredationPlus) && COMMON_SKILL_VALID(SID_Predation) && COMMON_SKILL_VALID(SID_PredationPlus)
		{ SID_Predation, SID_PredationPlus },
	#endif
	#if defined(SID_Alacrity) && defined(SID_AlacrityPlus) && COMMON_SKILL_VALID(SID_Alacrity) && COMMON_SKILL_VALID(SID_AlacrityPlus)
		{ SID_Alacrity, SID_AlacrityPlus },
	#endif
	#if defined(SID_Summon) && defined(SID_SummonPlus) && COMMON_SKILL_VALID(SID_Summon) && COMMON_SKILL_VALID(SID_SummonPlus)
		{ SID_Summon, SID_SummonPlus },
	#endif
	#if defined(SID_HugePower) && defined(SID_HugePowerPlus) && COMMON_SKILL_VALID(SID_HugePower) && COMMON_SKILL_VALID(SID_HugePowerPlus)
		{ SID_HugePower, SID_HugePowerPlus },
	#endif
	#if defined(SID_DualWield) && defined(SID_DualWieldPlus) && COMMON_SKILL_VALID(SID_DualWield) && COMMON_SKILL_VALID(SID_DualWieldPlus)
		{ SID_DualWield, SID_DualWieldPlus },
	#endif
	#if defined(SID_Shuffle) && defined(SID_ShufflePlus) && COMMON_SKILL_VALID(SID_Shuffle) && COMMON_SKILL_VALID(SID_ShufflePlus)
		{ SID_Shuffle, SID_ShufflePlus },
	#endif
	#if defined(SID_Persuade) && defined(SID_PersuadePlus) && COMMON_SKILL_VALID(SID_Persuade) && COMMON_SKILL_VALID(SID_PersuadePlus)
		{ SID_Persuade, SID_PersuadePlus },
	#endif
	#if defined(SID_ScrollScribe) && defined(SID_ScrollScribePlus) && COMMON_SKILL_VALID(SID_ScrollScribe) && COMMON_SKILL_VALID(SID_ScrollScribePlus)
		{ SID_ScrollScribe, SID_ScrollScribePlus },
	#endif
	#if defined(SID_Dance) && defined(SID_DancePlus) && COMMON_SKILL_VALID(SID_Dance) && COMMON_SKILL_VALID(SID_DancePlus)
		{ SID_Dance, SID_DancePlus },
	#endif
	};
	

// Function to find the "plus" version
int get_plus_version(int sid) {
    size_t count = sizeof(skill_upgrades) / sizeof(skill_upgrades[0]);
    if (count == 0)
        return sid;
        
    for (size_t i = 0; i < count; i++) {
        if (skill_upgrades[i].base == sid) {
            return skill_upgrades[i].plus;
        }
    }
    return sid; // Return original if no upgrade
}

#define REG_VCOUNT_CUSTOM (*(volatile unsigned short*)0x04000006)
#define REG_TM0CNT_L_CUSTOM (*(volatile unsigned short*)0x04000100)

int gba_random() {
    return (REG_VCOUNT_CUSTOM * REG_TM0CNT_L_CUSTOM);
}

// Random integer in range [0, max-1]
int getRandom(int max) {
	int random = NextRN_N(max);
	return random;
}

void GenerateSkillListExt(struct Unit * unit, struct SkillList * list)
{
    FORCE_DECLARE int weapon;
    int i, sid;
    int pid = UNIT_CHAR_ID(unit);
    int jid = UNIT_CLASS_ID(unit);
	bool upgrade = false;

#if defined(SID_Upgrade) && (COMMON_SKILL_VALID(SID_Upgrade))
	if (SkillTester(unit, SID_Upgrade))
		upgrade = true;
#endif

    u16 * tmp_list = (u16 *)gGenericBuffer;

    memset(list, 0, sizeof(*list));
#ifdef CONFIG_TURN_ON_ALL_SKILLS
	memset(tmp_list, 0, (MAX_SKILL_NUM + 1) * sizeof(u16));
#else
    memset(tmp_list, 0, MAX_SKILL_NUM + 1);
#endif

    /* person */
	sid = gpConstSkillTable_Person[pid * 2];

	if (upgrade)
		sid = get_plus_version(sid);

    if (COMMON_SKILL_VALID(sid))
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

	sid = gpConstSkillTable_Person[pid * 2 + 1];

	if (upgrade)
		sid = get_plus_version(sid);

    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    /* job */
	sid = gpConstSkillTable_Job[jid * 2];

	if (upgrade)
		sid = get_plus_version(sid);

    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

	sid = gpConstSkillTable_Job[jid * 2 + 1];

	if (upgrade)
		sid = get_plus_version(sid);

    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    /* item */
    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        u8 iid = ITEM_INDEX(unit->items[i]);

		sid = gpConstSkillTable_Item[iid * 2];

		if (upgrade)
			sid = get_plus_version(sid);

        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }

		sid = gpConstSkillTable_Item[iid * 2 + 1];

		if (upgrade)
			sid = get_plus_version(sid);

        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }
    }

    /* equippable */
    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
    {
#ifdef CONFIG_TURN_ON_ALL_SKILLS
		sid = GET_UNIT_SKILL(unit, i);
#else
		sid = UNIT_RAM_SKILLS(unit)[i];
#endif

		if (upgrade)
			sid = get_plus_version(sid);
		
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }
    }

    /* external */
    if (gpExternalSkillListGenerator)
        gpExternalSkillListGenerator(unit, list, tmp_list);

    WriteUnitList(unit, &list->header);
}

void ForceUpdateUnitSkillList(struct Unit *unit)
{
	struct SkillList *list = SkillListGeneric;

	if (unit == &gBattleActor.unit)
		list = SkillListBattleActor;
	else if (unit == &gBattleTarget.unit)
		list = SkillListBattleTarget;

	GenerateSkillListExt(unit, list);
}

struct SkillList *GetUnitSkillList(struct Unit *unit)
{
	struct SkillList *list = SkillListGeneric;

	if (unit == &gBattleActor.unit)
		list = SkillListBattleActor;
	else if (unit == &gBattleTarget.unit)
		list = SkillListBattleTarget;

	if (!JudgeUnitList(unit, &list->header))
		GenerateSkillListExt(unit, list);

	return list;
}

bool _SkillListTester(struct Unit *unit, const u16 sid)
{
	int i;
	struct SkillList *list = GetUnitSkillList(unit);

	for (i = 0; i < list->amt; i++)
		if (list->sid[i] == sid)
			return true;

	return false;
}

void SetupBattleSkillList(void)
{
	GenerateSkillListExt(&gBattleActor.unit,  SkillListBattleActor);
	GenerateSkillListExt(&gBattleTarget.unit, SkillListBattleTarget);
}

void DisableUnitSkilLList(struct Unit *unit)
{
	struct SkillList *list = GetUnitSkillList(unit);

	list->amt = 0;
}

void ResetSkillLists(void)
{
	memset(&sSkillList, 0, sizeof(sSkillList));
}

STATIC_DECLAR void SetupBattleSkillFastList(void)
{
	int i;
	struct SkillList *list;
	u32 *fast_list;

	Assert(MAX_SKILL_NUM <= (sizeof(sSkillFastList) * 8 / 2));

	CpuFastFill(0, sSkillFastList, sizeof(sSkillFastList));

	/**
	 * Actor
	 */
	list = GetUnitSkillList(&gBattleActor.unit);
	fast_list = SkillFastListActor;

	for (i = 0; i < list->amt; i++)
		_BIT_SET(fast_list, list->sid[i]);

	/**
	 * Target
	 */
	list = GetUnitSkillList(&gBattleTarget.unit);
	fast_list = SkillFastListTarget;

	for (i = 0; i < list->amt; i++)
		_BIT_SET(fast_list, list->sid[i]);
}

#if 0
bool _BattleSkillTester(struct BattleUnit *bu, const u16 sid)
{
	u32 *fast_list;

	if (bu == &gBattleActor)
		fast_list = SkillFastListActor;
	else if (bu == &gBattleTarget)
		fast_list = SkillFastListTarget;
	else
		return false;

	return _BIT_CHK(fast_list, sid);
}
#endif

void AppendBattleUnitSkillList(struct BattleUnit *bu, u16 skill)
{
	u32 i;
	struct SkillList *list;
	u32 *fast_list;

	list = GetUnitSkillList(&bu->unit);

	/**
	 * Full filled
	 */
	if (list->amt >= ARRAY_COUNT(list->sid))
		return;

	/**
	 * Already in force
	 */
	for (i = 0; i < list->amt; i++) {
		if (skill == list->sid[i])
			return;
	}

	/**
	 * Add to list
	 */
	list->sid[list->amt++] = skill;


	if (bu == &gBattleActor)
		fast_list = SkillFastListActor;
	else if (bu == &gBattleTarget)
		fast_list = SkillFastListTarget;
	else
		return;

	/**
	 * Add to fast-list
	 */
	_BIT_SET(fast_list, skill);
}

void UnitToBattle_SetupSkillList(struct Unit *unit, struct BattleUnit *bu)
{
	FORCE_DECLARE bool nihil_on_actor, nihil_on_target;

	/**
	 * Here we hold 3 assumption:
	 *
	 * 1. UnitToBattle routine stands at the very beginning of battle-generate
	 * 2. Battle target initialization is behind actor.
	 * 3. No skill activcated before during function: InitBattleUnit()
	 */
	if (bu == &gBattleTarget) {
		SetupBattleSkillList();

#if (defined(SID_Nihil) && COMMON_SKILL_VALID(SID_Nihil))
		nihil_on_actor  = _SkillListTester(&gBattleActor.unit,  SID_Nihil);
		nihil_on_target = _SkillListTester(&gBattleTarget.unit, SID_Nihil);

		if (nihil_on_actor)
			DisableUnitSkilLList(&gBattleActor.unit);

		if (nihil_on_target)
			DisableUnitSkilLList(&gBattleTarget.unit);
#endif

		SetupBattleSkillFastList();
	}
}