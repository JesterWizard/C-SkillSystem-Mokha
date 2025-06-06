#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

void StartStatusHealEffect(struct Unit * unit, ProcPtr proc);

LYN_REPLACE_CHECK(StatusDecayDisplay_Display);
void StatusDecayDisplay_Display(struct UnknownBMUSAilmentProc * proc)
{
    struct SelectTarget * target = GetTarget(proc->unk_4C);
    struct Unit * unit = GetUnit(gActionData.subjectIndex);
    int status = GetUnitStatusIndex(unit);
    proc->unk_58 = status;

    SetUnitStatus(unit, 0);

    if (gpDebuffInfos[status].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
        StartStatusHealEffect(GetUnit(target->uid), proc);
}

LYN_REPLACE_CHECK(TickActiveFactionTurn);
void TickActiveFactionTurn(void)
{
    int i, displayMapChange = FALSE;

    InitTargets(0, 0);

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit * unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_UNAVAILABLE | US_RESCUED))
            continue;

        if (unit->barrierDuration != 0)
            unit->barrierDuration--;

        if (unit->torchDuration != 0)
        {
            unit->torchDuration--;
            displayMapChange = TRUE;
        }
    }

    if (displayMapChange)
    {
        RenderBmMapOnBg2();
        RefreshEntityBmMaps();
        RenderBmMap();
        NewBMXFADE(TRUE);
        RefreshUnitSprites();
    }

    #define DEC_STATUS(unit)                                            \
        do {                                                            \
            int _status = GetUnitStatusIndex(unit);                     \
            int _duration = GetUnitStatusDuration(unit);                \
            if (0 != _status && _duration != 0)                         \
            {                                                           \
                if (_status != UNIT_STATUS_RECOVER)                     \
                {                                                       \
                    TryTickUnitStatusDuration(unit);                    \
                }                                                       \
                if (GetUnitStatusDuration(unit) == 0)                   \
                    AddTarget(unit->xPos, unit->yPos, unit->index, 0);  \
            }                                                           \
        } while (0);

    if (FACTION_BLUE == gPlaySt.faction)
    {
        /* Blue buff */
        for (i = FACTION_BLUE + 1; i <= (FACTION_BLUE + CONFIG_UNIT_AMT_ALLY); i++)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            bool doomStatus = false;

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ALLY)
            {
                if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_DOOM)
                    doomStatus = true;
                
                DEC_STATUS(unit);
#if (defined(SID_ShedSkin) && COMMON_SKILL_VALID(SID_ShedSkin))
                if (SkillTester(unit, SID_ShedSkin))
                    DEC_STATUS(unit);
#endif
            }

            if (GetUnitStatusDuration(unit) == 0 && doomStatus == true)
                UnitKill(unit);

            TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ALLY);
        }

        /* Red debuff */
        for (i = FACTION_RED + 1; i <= (FACTION_RED + CONFIG_UNIT_AMT_ENEMY); i++)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            bool doomStatus = false;

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
            {
                if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_DOOM)
                    doomStatus = true;

                DEC_STATUS(unit);
#if (defined(SID_ShedSkin) && COMMON_SKILL_VALID(SID_ShedSkin))
                if (SkillTester(unit, SID_ShedSkin))
                    DEC_STATUS(unit);
#endif
            }

            if (GetUnitStatusDuration(unit) == 0 && doomStatus == true)
                UnitKill(unit);

            TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ENEMY);
        }
    }
    else if (FACTION_RED == gPlaySt.faction)
    {
        /* Red buff */
        for (i = FACTION_RED + 1; i <= (FACTION_RED + CONFIG_UNIT_AMT_ENEMY); i++)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            bool doomStatus = false;

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ALLY)
            {

                if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_DOOM)
                    doomStatus = true;

                DEC_STATUS(unit);
#if (defined(SID_ShedSkin) && COMMON_SKILL_VALID(SID_ShedSkin))
                if (SkillTester(unit, SID_ShedSkin))
                    DEC_STATUS(unit);
#endif
            }

            if (GetUnitStatusDuration(unit) == 0 && doomStatus == true)
                UnitKill(unit);
                
            TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ALLY);
        }

        /* Blue debuff */
        for (i = FACTION_BLUE + 1; i <= (FACTION_BLUE + CONFIG_UNIT_AMT_ALLY); i++)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            bool doomStatus = false;

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
            {

                if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_DOOM)
                    doomStatus = true;

                DEC_STATUS(unit);
#if (defined(SID_ShedSkin) && COMMON_SKILL_VALID(SID_ShedSkin))
                if (SkillTester(unit, SID_ShedSkin))
                    DEC_STATUS(unit);
#endif
            }

            if (GetUnitStatusDuration(unit) == 0 && doomStatus == true)
                UnitKill(unit);

            TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ENEMY);
        }

        /* Green debuff */
        for (i = FACTION_GREEN + 1; i < (FACTION_GREEN + CONFIG_UNIT_AMT_NPC); i++)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            bool doomStatus = false;

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
            {
                if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_DOOM)
                    doomStatus = true;

                DEC_STATUS(unit);
#if (defined(SID_ShedSkin) && COMMON_SKILL_VALID(SID_ShedSkin))
                if (SkillTester(unit, SID_ShedSkin))
                    DEC_STATUS(unit);
#endif
            }

            if (GetUnitStatusDuration(unit) == 0 && doomStatus == true)
                UnitKill(unit);

            TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ENEMY);
        }
    }
    else if (FACTION_GREEN == gPlaySt.faction)
    {
        /* Green buff */
        for (i = FACTION_GREEN + 1; i < (FACTION_GREEN + CONFIG_UNIT_AMT_NPC); i++)
        {
            struct Unit * unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit))
                continue;

            bool doomStatus = false;

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ALLY)
            {
                if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_DOOM)
                    doomStatus = true;

                DEC_STATUS(unit);
#if (defined(SID_ShedSkin) && COMMON_SKILL_VALID(SID_ShedSkin))
                if (SkillTester(unit, SID_ShedSkin))
                    DEC_STATUS(unit);
#endif
            }

            if (GetUnitStatusDuration(unit) == 0 && doomStatus == true)
                UnitKill(unit);
                
            TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ALLY);
        }
    }
}
