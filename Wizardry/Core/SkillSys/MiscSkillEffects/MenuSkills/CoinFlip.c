#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"
#include "debuff.h"
#include "jester_headers/miscellaenous.h"
#include "rng.h"

#if defined(SID_CoinFlip) && (COMMON_SKILL_VALID(SID_CoinFlip))

u8 CoinFlip_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING || gActiveUnit->state & US_RESCUING)
        return MENU_NOTSHOWN;

    if (CheckBitUES(gActiveUnit, UES_BIT_COIN_FLIP_SKILL_USED))
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

u8 CoinFlip_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, 0x858); // TODO: msgid "There's no more ammo for[NL]the ballista.[.]"
        return MENU_ACT_SND6B;
    }

    ResetIconGraphics();

    LoadIconPalettes(4);

    if (gActiveUnit->state & US_IN_BALLISTA)
    {
        return StartUnitBallistaSelect(menu, item);
    }

    gActionData.unk08 = SID_CoinFlip;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
    PlaySoundEffect(0x269);
    Proc_StartBlocking(ProcScr_DanceringAnim, proc);

    BG_SetPosition(
        BG_0,
        -SCREEN_TILE_IX(gActiveUnit->xPos - 1),
        -SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(gActiveUnit, proc);
    SetBitUES(gActiveUnit, UES_BIT_COIN_FLIP_SKILL_USED);
}

bool Action_CoinFlip(ProcPtr parent)
{
    if (Roll1RN(50))
    {
        NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
        return true;
    }

    return false;
}

#endif

/* Doesn't work, can't reset the unit movement here as it happens too soon. */