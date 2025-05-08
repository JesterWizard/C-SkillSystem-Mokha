#include "common-chax.h"
#include "unit-expa.h"
#include "bwl.h"

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
                if (bwl->_pad_[1] + 4 > 30)
                    bwl->_pad_[1] = 30;
                else    
                    bwl->_pad_[1] += 4;
            }
        }
    }
#endif
}
