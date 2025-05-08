#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"
#include "constants/skills.h"

extern struct ProcCmd CONST_DATA ProcScr_SetTargetStatus[];
extern struct ProcCmd CONST_DATA ProcScr_ExecNightmareStaff[];

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

LYN_REPLACE_CHECK(ActionStaffDoorChestUseItem);
void ActionStaffDoorChestUseItem(ProcPtr proc) {
    int itemId = GetItemIndex(GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex]);

    gBattleActor.hasItemEffectTarget = 0;

    switch (itemId) {
        case ITEM_STAFF_HEAL:
        case ITEM_STAFF_MEND:
        case ITEM_STAFF_RECOVER:
        case ITEM_STAFF_PHYSIC:
            ExecStandardHeal(proc);
            break;
        case ITEM_STAFF_SILENCE:
        case ITEM_STAFF_SLEEP:
        case ITEM_STAFF_BERSERK:
            ExecStatusStaff(proc);
            break;
        case ITEM_NIGHTMARE:
            ExecNightmare(proc);
            break;
        case ITEM_STAFF_FORTIFY:
            ExecFortify(proc);
            break;
        case ITEM_STAFF_RESTORE:
            ExecRestore(proc);
            break;
        case ITEM_STAFF_RESCUE:
            ExecRescueStaff(proc);
            break;
        case CONFIG_ITEM_INDEX_FORCE_STAFF:
        case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
        case CONFIG_ITEM_INDEX_ACUITY_STAFF:
        case CONFIG_ITEM_INDEX_SPRINT_STAFF:
        case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
        case CONFIG_ITEM_INDEX_IRON_STAFF:
        case ITEM_STAFF_BARRIER:
            ExecBarrierStaff(proc);
            break;
        case ITEM_STAFF_WARP:
            ExecWarpStaff(proc);
            break;
        case ITEM_STAFF_UNLOCK:
            ExecUnlockStaff(proc);
            break;
        case ITEM_STAFF_REPAIR:
            ExecHammerne(proc);
            break;
        case ITEM_TORCH:
            ExecTorchItem(proc);
            break;
        case ITEM_VULNERARY:
        case ITEM_VULNERARY_2:
            ExecVulneraryItem(proc, 10);
            break;
        case ITEM_ELIXIR:
            ExecElixirItem(proc);
            break;
        case ITEM_PUREWATER:
            ExecPureWaterItem(proc);
            break;
        case ITEM_ANTITOXIN:
            ExecAntitoxinItem(proc);
            break;
        case ITEM_CHESTKEY:
        case ITEM_DOORKEY:
        case ITEM_LOCKPICK:
        case ITEM_CHESTKEY_BUNDLE:
            ExecKeyItem();
            break;
        case ITEM_HEROCREST:
        case ITEM_KNIGHTCREST:
        case ITEM_ORIONSBOLT:
        case ITEM_ELYSIANWHIP:
        case ITEM_GUIDINGRING:
        case ITEM_MASTERSEAL:
        case ITEM_HEAVENSEAL:
        case ITEM_OCEANSEAL:
        case ITEM_LUNARBRACE:
        case ITEM_SOLARBRACE:
        // case ITEM_UNK_C1:
            gBattleActor.weaponBefore = gBattleTarget.weaponBefore = GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex];

            gBattleActor.weapon = gBattleTarget.weapon = GetUnitEquippedWeapon(GetUnit(gActionData.subjectIndex));

            gBattleTarget.statusOut = -1;

            StartBmPromotion(proc);
            break;
        case ITEM_BOOSTER_HP:
        case ITEM_BOOSTER_POW:
        case ITEM_BOOSTER_SKL:
        case ITEM_BOOSTER_SPD:
        case ITEM_BOOSTER_LCK:
        case ITEM_BOOSTER_DEF:
        case ITEM_BOOSTER_RES:
        case ITEM_BOOSTER_MOV:
        case ITEM_BOOSTER_CON:
        case ITEM_METISSTOME:
            ExecStatBoostItem(proc);
            break;
        case ITEM_STAFF_LATONA:
            ExecLatona(proc);
            break;
        case CONFIG_ITEM_INDEX_MINE_STAFF:
        case ITEM_MINE:
            ExecMine(proc);
            break;
        case CONFIG_ITEM_INDEX_RUNE_STAFF:
        case ITEM_LIGHTRUNE:
            ExecLightRune(proc);
            break;
        case ITEM_STAFF_TORCH:
            ExecTorchStaff(proc);
            break;
        case ITEM_FILLAS_MIGHT:
        case ITEM_NINISS_GRACE:
        case ITEM_THORS_IRE:
        case ITEM_SETS_LITANY:
            ExecDanceRing(proc);
            break;
        case ITEM_JUNAFRUIT:
            ExecJunaFruitItem(proc);
            break;
        case CONFIG_ITEM_INDEX_SLOW_STAFF:
            ExecCustomStaves(proc);
            break;

#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL
        case CONFIG_ITEM_INDEX_SKILL_SCROLL:
            ItemUseAction_SkillScroll(proc);
            break;
#endif
    }

    if (itemId == ITEM_NIGHTMARE) {
        Proc_StartBlocking(ProcScr_ExecNightmareStaff, proc);
        return;
    }

    if (gBattleTarget.statusOut >= 0) {
        Proc_StartBlocking(ProcScr_SetTargetStatus, proc);
    }

    return;
}