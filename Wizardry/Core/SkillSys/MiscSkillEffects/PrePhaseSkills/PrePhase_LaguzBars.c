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
    // FORCE_DECLARE const u8 laguzBarValues[3][3] = {
    //     { CHARACTER_EIRIKA, 15, 4},
    //     { CHARACTER_SETH, 10, 4},
    //     { CHARACTER_FRANZ, 10, 4},
    //    {0}
    // };   
   
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
            //             bwl->_pad_[1] = laguzBarValues[i][1];
            //         else
            //         {
            //             if (bwl->_pad_[1] + laguzBarValues[i][2] > 30)
            //                 bwl->_pad_[1] = 30;
            //             else    
            //                 bwl->_pad_[1] += laguzBarValues[i][2];
            //         }
            //     }   
            // }

            bwl = GetNewBwl(UNIT_CHAR_ID(unit));
            if (gPlaySt.chapterTurnNumber == 1)
                bwl->_pad_[1] = 10;
            else
            {
                if (CheckUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ) || CheckUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ_HALFSHIFT))
                {
                    if (bwl->_pad_[1] - 4 <= 0)
                    {
                        bwl->_pad_[1] = 0;
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
                                bwl->_pad_[1] -= 4;
                        #else
                            bwl->_pad_[1] -= 4;
                        #endif
                }
                else
                {
                    if (bwl->_pad_[1] + 4 > 30)
                        bwl->_pad_[1] = 30;
                    else    
                        bwl->_pad_[1] += 4;
                }
            }
        }
    }
#endif
}
