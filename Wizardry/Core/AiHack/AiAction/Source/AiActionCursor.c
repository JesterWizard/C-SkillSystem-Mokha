#include "common-chax.h"
#include "strmag.h"

#ifndef CONFIG_AI_ACTION_EXPA_Teleportation
    #define CONFIG_AI_ACTION_EXPA_Teleportation 20
#endif

LYN_REPLACE_CHECK(CpPerform_MoveCameraOntoTarget);
void CpPerform_MoveCameraOntoTarget(struct CpPerformProc * proc)
{
    struct Unit * unit;

    int x = 0;
    int y = 0;
    int magic_seal_set = false;

    if (gActionData.unitActionType == UNIT_ACTION_TRAPPED)
        return;

    /**
     * Magic seal AI fix to prevent softlocks when it's used on the enemy
     */
    if (IsUnitMagicSealed(gActiveUnit) && IsMagicAttack(&gBattleActor))
    {
        gAiDecision.actionId = AI_CMD_ACTION_IN_PLACE;
        magic_seal_set = true;
    }

    switch (gAiDecision.actionId) {
        case AI_ACTION_NONE:
        case AI_ACTION_ESCAPE:
        case AI_ACTION_PILLAGE:
        case AI_ACTION_USEITEM:
        case AI_ACTION_RIDEBALLISTA:
        case AI_ACTION_EXITBALLISTA:
        case AI_ACTION_DKNIGHTMARE:
        case AI_ACTION_DKSUMMON:
        case AI_ACTION_PICK:

#if CHAX
        case CONFIG_AI_ACTION_EXPA_Teleportation:
        default:
#endif
            return;

        case AI_ACTION_COMBAT:
            if (gAiDecision.targetId == 0)
            {
                x = gAiDecision.xTarget;
                y = gAiDecision.yTarget;
            }
            else
            {
                unit = GetUnit(gAiDecision.targetId);
                x = unit->xPos;
                y = unit->yPos;
            }

            if (((s8)gAiDecision.itemSlot == -1) && !(gActiveUnit->state & US_IN_BALLISTA))
            {
                EndAllMus();

                gActiveUnit->xPos = gAiDecision.xMove;
                gActiveUnit->yPos = gAiDecision.yMove;

                RideBallista(gActiveUnit);

                StartMu(gActiveUnit);
                SetAutoMuDefaultFacing();
            }

            break;

        case AI_ACTION_STEAL:
            unit = GetUnit(gAiDecision.targetId);

            x = unit->xPos;
            y = unit->yPos;

            break;

        case AI_ACTION_REFRESH:
            unit = GetUnit(gAiDecision.targetId);

            x = unit->xPos;
            y = unit->yPos;

            break;

        case AI_ACTION_TALK:
            unit = GetUnit(gAiDecision.yTarget);

            x = unit->xPos;
            y = unit->yPos;

            break;

        case AI_ACTION_STAFF:
            if (gAiDecision.targetId == 0)
                return;

            unit = GetUnit(gAiDecision.targetId);

            x = unit->xPos;
            y = unit->yPos;

            break;
    }

    /**
     * If this is set, we don't want to bother setting the target red cursor
     */
    if(magic_seal_set)
        return;

    EnsureCameraOntoPosition(proc, x, y);
    StartAiTargetCursor(x * 16, y * 16, 2, proc);
}
