#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "mapanim.h"

// LYN_REPLACE_CHECK(BeginBattleAnimations);
// void BeginBattleAnimations(void) {
//     BG_Fill(gBG2TilemapBuffer, 0);
//     BG_EnableSyncByMask(1 << 2);

//     gPaletteBuffer[PAL_BACKDROP_OFFSET] = 0;
//     EnablePaletteSync();

//     RenderBmMap();

//     if (sub_8055BC4()) {
//         SetBanimLinkArenaFlag(0);
//         BeginAnimsOnBattleAnimations();
//     } else {
//         EndAllMus();
//         RenderBmMap();
//         BeginBattleMapAnims();

//         gBattleStats.config |= BATTLE_CONFIG_MAPANIMS;
//     }
// }

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

LYN_REPLACE_CHECK(ExecMine);
void ExecMine(ProcPtr proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    AddTrap(gActionData.xOther, gActionData.yOther, TRAP_MINE, 0);

    StartMineAnim(proc, gActionData.xOther, gActionData.yOther);

    BattleApplyItemEffect(proc);
    // BeginBattleAnimations(); // I need this for the EXP bar but it's causing a softlock on the prologue, still grants exp and level ups though

    gBattleTarget.statusOut = -1;

    return;
}