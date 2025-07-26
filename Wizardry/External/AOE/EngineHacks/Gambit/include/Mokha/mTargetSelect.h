#ifndef HEARDERMOKHA_TARGETSELECT
#define HEARDERMOKHA_TARGETSELECT

// 0x80251B5
void MakeTargetListForWeapon(struct Unit * unit, int item);

// 0x802517D
void AddUnitToTargetListIfNotAllied(struct Unit * unit);
#endif // HEARDERMOKHA_TARGETSELECT