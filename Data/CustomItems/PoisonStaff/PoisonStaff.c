// #include "common-chax.h"
// #include "item-sys.h"
// #include "battle-system.h"
// #include "constants/texts.h"
// #include "jester_headers/custom-functions.h"
// #include "bmitemuse.h"
// #include "bmtarget.h"
// #include "bm.h"
// #include "proc.h"
// #include "weapon-range.h"

// // Called when the Poison tile selection UI starts (kept for compatibility)
// void PoisonMapSelect_Init(ProcPtr proc) {
//     StartSubtitleHelp(proc, GetStringFromIndex(MSG_ITEM_POISON_STAFF_SUBTITLE));
// }

// // Cursor switch hook (not required when using the warp square-select proc)
// u8 PoisonMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget* target) {
//     return 0;
// }

// const struct SelectInfo gSelectInfo_PoisonTile =
// {
//     .onInit = PoisonMapSelect_Init,
//     .onEnd = SubtitleMapSelect_End,
//     .onSwitchIn = PoisonMapSelect_SwitchIn,
//     .onCancel = GenericSelection_BackToUM_CamWait,
// };

// void TryAddUnitToPoisonStaffTargetList(struct Unit * unit) {

//     if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
//         return;
//     }

//     AddTarget(unit->xPos, unit->yPos, unit->index, 0);
// }

// void MakeTargetListForPoison(struct Unit* unit) {
// 	gSubjectUnit = unit;
// 	InitTargets(unit->xPos, unit->yPos);

// 	BmMapFill(gBmMapRange, 0);
// 	AddMapForItem(unit, CONFIG_ITEM_INDEX_POISON_STAFF);
// 	ForEachUnit(TryAddUnitToPoisonStaffTargetList, gBmMapRange, 0);
// }

// /* IER hooks (ports) */
// bool IER_Usability_Poison(struct Unit * unit, int item)
// {
//     return HasSelectTarget(unit, MakeTargetListForPoison);
// }