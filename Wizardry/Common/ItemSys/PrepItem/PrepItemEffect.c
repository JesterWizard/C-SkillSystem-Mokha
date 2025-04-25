#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(PrepItemUse_HandleItemEffect);
void PrepItemUse_HandleItemEffect(struct ProcPrepItemUse * proc)
{
    u16 item = proc->unit->items[proc->slot];
    PrepItemEffectFunc_t it = gPrepItemEffectFunc[ITEM_INDEX(item)];

    if (it)
        it(proc, item);
}

void PrepItemEffect_StatBooster(struct ProcPrepItemUse * proc, u16 item)
{
    Proc_StartBlocking(ProcScr_PrepItemUseBooster, proc);
}

void PrepItemEffect_Promotion(struct ProcPrepItemUse * proc, u16 item)
{
    PlaySoundEffect(0x6A);
    Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_PROMO);
}

void PrepItemEffect_JunaFruit(struct ProcPrepItemUse * proc, u16 item)
{
    Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_JUNA);
}

LYN_REPLACE_CHECK(ExecVulneraryItem);
void ExecVulneraryItem(ProcPtr proc, int amount) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex), gActionData.itemSlotIndex);

#if (defined(SID_ItemLore) && (COMMON_SKILL_VALID(SID_ItemLore)))
    if (SkillTester(gActiveUnit, SID_ItemLore))
        amount *= 2;
#endif

    AddUnitHp(GetUnit(gActionData.subjectIndex), amount);

    gBattleHitIterator->hpChange = gBattleActor.unit.curHP - GetUnitCurrentHp(GetUnit(gActionData.subjectIndex));

    gBattleActor.unit.curHP = GetUnitCurrentHp(GetUnit(gActionData.subjectIndex));

    gBattleActor.weaponBefore = ITEM_VULNERARY;

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();

    return;
}

LYN_REPLACE_CHECK(ExecElixirItem);
void ExecElixirItem(ProcPtr proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex), gActionData.itemSlotIndex);

    int amount = 80;

#if (defined(SID_ItemLore) && (COMMON_SKILL_VALID(SID_ItemLore)))
    if (SkillTester(gActiveUnit, SID_ItemLore))
        amount *= 2;
#endif

    AddUnitHp(GetUnit(gActionData.subjectIndex), amount);

    /* For the purposes of dealing with healing > 127 we employ the abs function to always return the positive value */
    gBattleHitIterator->hpChange = abs(gBattleActor.unit.curHP - GetUnitCurrentHp(GetUnit(gActionData.subjectIndex)));

    gBattleActor.unit.curHP = GetUnitCurrentHp(GetUnit(gActionData.subjectIndex));

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();

    return;
}