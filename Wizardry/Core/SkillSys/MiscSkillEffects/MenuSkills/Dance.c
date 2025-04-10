#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "debuff.h"
#include "constants/skills.h"


LYN_REPLACE_CHECK(DanceCommandUsability);
u8 DanceCommandUsability(const struct MenuItemDef * def, int number)
{
#if !CHAX
    if (!(UNIT_CATTRIBUTES(gActiveUnit) & CA_DANCE))
        return MENU_NOTSHOWN;
#endif

    gBmSt.um_tmp_item = ITEM_DANCE;
    return sub_80230F0(def);
}

LYN_REPLACE_CHECK(PlayCommandEffect);
u8 PlayCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
    s8 hasTargets;
    int i;

    s8 itemUsable = 0;

    MakeTargetListForRefresh(gActiveUnit);

    hasTargets = 0;
    if (GetSelectTargetCount() != 0) {
        hasTargets = 1;
    }

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        int item = gActiveUnit->items[i];

        if (item == 0) {
            break;
        }

        if (GetItemType(item) != ITYPE_12) {
            continue;
        }

        if (!CanUnitUseItem(gActiveUnit, item)) {
            continue;
        }

        itemUsable = 1;
    }

    gActionData.unk08 = SID_Dance;

    gBattleActorGlobalFlag.skill_activated_dance = true;

    if (hasTargets && !itemUsable) {
        return ItemMenu_Select1stCommand(menu, menuItem);
    } else {
        ProcPtr proc = StartOrphanMenu(&gItemMenuDef);

        StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
        SetFaceBlinkControlById(0, 5);
        ForceMenuItemPanel(proc, gActiveUnit, 0xF, 0xB);

        ResetIconGraphics();
        LoadIconPalettes(4);

        return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
    }

}


LYN_REPLACE_CHECK(RefreshMapSelect_Select);
u8 RefreshMapSelect_Select(ProcPtr proc, struct SelectTarget* target) {

    gActionData.unitActionType = UNIT_ACTION_DANCE;
    gActionData.targetIndex = target->uid;

#if defined(SID_VigorDance) && (COMMON_SKILL_VALID(SID_VigorDance))
    if (SkillTester(gActiveUnit, SID_VigorDance) & !gBattleActorGlobalFlag.skill_activated_vigor_dance)
    {
        gBattleActorGlobalFlag.skill_activated_vigor_dance = true;
        SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_BUFF_VIGORDANCE);
    }
#endif

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

LYN_REPLACE_CHECK(BeginMapAnimForDance);
void BeginMapAnimForDance(void)
{
    gBattleActor.weaponBefore = ITEM_STAFF_FORTIFY;

    gManimSt.hp_changing = 0;
    gManimSt.mapAnimKind = MANIM_KIND_REFRESH;
    gManimSt.actorCount = 1;

    gManimSt.subjectActorId = 0;
    gManimSt.targetActorId = 0;

    /* If we're not using Tethys, turn off the map animations as the game will freeze otherwise */
    if (gActiveUnit->pClassData->number != CLASS_DANCER)
        return;

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Start(ProcScr_MapAnimDance, PROC_TREE_3);
}