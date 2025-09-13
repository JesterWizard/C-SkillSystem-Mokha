#include "common-chax.h"
#include "weapon-range.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "debuff.h"
#include "class-types.h"
#include "jester_headers/custom-functions.h"

#if defined(SID_SpellBlade) && (COMMON_SKILL_VALID(SID_SpellBlade))
u8 SpellBlade_Usability(const struct MenuItemDef* def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!HasSelectTarget(gActiveUnit, MakeTargetListForAdjacentUnits))
		return MENU_NOTSHOWN;

	return MENU_ENABLED;
}

int SpellBlade_Hover(struct MenuProc* menu, struct MenuItemProc* item)
{
	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);
	GenerateUnitStandingReachRange(gActiveUnit, 0b11);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
	return 0;
}

int SpellBlade_Unhover(struct MenuProc* menu, struct MenuItemProc* menuItem)
{
	HideMoveRangeGraphics();
	return 0;
}

static u8 SpellBlade_OnSelectTarget(ProcPtr proc, struct SelectTarget* target)
{
	gActionData.xOther = target->x;
	gActionData.yOther = target->y;

	HideMoveRangeGraphics();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

	gActionData.unk08 = SID_SpellBlade;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	struct Unit* unit = GetUnit(target->uid);

	int targetClassId = UNIT_CLASS_ID(unit);

	if (CheckClassCavalry(targetClassId))
		SetUnitStatusExt(gActiveUnit, NEW_UNIT_STATUS_CAVALRY_EFFECTIVE, 1);
	else if (CheckClassArmor(targetClassId))
		SetUnitStatusExt(gActiveUnit, NEW_UNIT_STATUS_ARMOR_EFFECTIVE, 1);
	else if (CheckClassFlier(targetClassId))
		SetUnitStatusExt(gActiveUnit, NEW_UNIT_STATUS_FLIER_EFFECTIVE, 1);
	else if (CheckClassInfantry(targetClassId))
		SetUnitStatusExt(gActiveUnit, NEW_UNIT_STATUS_INFANTRY_EFFECTIVE, 1);

	return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 SpellBlade_OnSelected(struct MenuProc* menu, struct MenuItemProc* item)
{

	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_SKILL_CommonFail);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();

	MakeTargetListForAdjacentUnits(gActiveUnit);
	BmMapFill(gBmMapMovement, -1);

	StartSubtitleHelp(
		NewTargetSelection_Specialized(&gSelectInfo_PutTrap, SpellBlade_OnSelectTarget),
		GetStringFromIndex(MSG_SELECT_UNIT));

	PlaySoundEffect(0x6A);
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
	// Nothing to do here, the result is calculated in SpellBlade_OnSelectTarget
}

bool Action_SpellBlade(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif
