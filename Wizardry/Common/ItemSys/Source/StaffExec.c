#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(ExecLightRune);
void ExecLightRune(ProcPtr proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    AddLightRune(gActionData.xOther, gActionData.yOther);

    BattleApplyItemEffect(proc);

#ifdef CONFIG_ITEM_INDEX_RUNE_STAFF
    int x = gActionData.xOther * 0x10 - gBmSt.camera.x - 0x18;
    int y = gActionData.yOther * 0x10 - gBmSt.camera.y - 0x28;

    BG_SetPosition(0, -x, -y);
    BeginBattleAnimations(); // This way we can gain EXP for using a light rune staff
#else
    StartLightRuneAnim(proc, gActionData.xOther, gActionData.yOther);
#endif

    gBattleTarget.statusOut = -1;

    return;
}