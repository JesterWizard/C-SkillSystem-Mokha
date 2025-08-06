#include "common-chax.h"
#include "unit-expa.h"
#include "bwl.h"
#include "debuff.h"
#include "jester_headers/class-pairs.h"

void PrePhase_LaguzBars(ProcPtr proc)
{
#ifdef CONFIG_LAGUZ_BARS
    int uid;
    struct Unit *unit;
    FORCE_DECLARE struct NewBwl * bwl;
   
    for (uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
    {
        unit = GetUnit(uid);
        if (UNIT_IS_VALID(unit))
        {
            // for (unsigned i = 0; i < ARRAY_COUNT(laguzBarValues); i++)
            // {
            //     if (UNIT_CHAR_ID(unit) == laguzBarValues[i][0])
            //     {
            //         bwl = GetNewBwl(UNIT_CHAR_ID(unit));
            //         if (gPlaySt.chapterTurnNumber == 1)
            //             bwl->laguzBar = laguzBarValues[i][1];
            //         else
            //         {
            //             if (bwl->laguzBar + laguzBarValues[i][2] > 30)
            //                 bwl->laguzBar = 30;
            //             else    
            //                 bwl->laguzBar += laguzBarValues[i][2];
            //         }
            //     }   
            // }

            bwl = GetNewBwl(UNIT_CHAR_ID(unit));
            if (gPlaySt.chapterTurnNumber == 1)
                bwl->laguzBar = 10;
            else
            {
                if (CheckUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ) || CheckUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ_HALFSHIFT))
                {
                    if (bwl->laguzBar - 4 <= 0)
                    {
                        bwl->laguzBar = 0;
                        for (int i = 0; i < laguzListSize; i++)
                        {
                            if (unit->pClassData->number == laguzPairs[i][1])
                            {
                                unit->pClassData = GetClassData(laguzPairs[i][0]);
                                ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ);
                                ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ_HALFSHIFT);
                                break;
                            }
                        }
                    }
                    else
                        #if defined(SID_FormShift) && (COMMON_SKILL_VALID(SID_FormShift))
                            if (!SkillTester(unit, SID_FormShift))
                                bwl->laguzBar -= 4;
                        #else
                            bwl->laguzBar -= 4;
                        #endif
                }
                else
                {
                    if (bwl->laguzBar + 4 > 30)
                        bwl->laguzBar = 30;
                    else    
                        bwl->laguzBar += 4;
                }
            }
        }
    }
#endif
}
