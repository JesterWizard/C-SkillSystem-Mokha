#include "common-chax.h"
#include "skill-system.h"
#include "prep-skill.h"
#include "icon-rework.h"
#include "constants/texts.h"

void PrepSkill1_DrawLeftSkillIcon(struct ProcPrepSkill1 * proc)
{
    int x, y;
    struct Unit * unit = GetUnitFromPrepList(proc->list_num_cur);
    struct SkillList * list = GetUnitSkillList(unit);
    ResetIconGraphics_();
    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 1, 6), 10, 6, 0);

    if (list->amt == 0)
    {
        struct Text * text = &gPrepUnitTexts[0x16];
        ClearText(text);
        PutDrawText(text, TILEMAP_LOCATED(gBG0TilemapBuffer, 2, 6), TEXT_COLOR_SYSTEM_GRAY, 0, 0, GetStringFromIndex(MSG_MSS_NOSKILLS));
    }

    for (y = 0; y < PREP_SLLIST_HEIGHT; y++)
    {
        for (x = 0; x < PREP_SLLIST_LENGTH-1; x++)
        {
            int count = x + y * (PREP_SLLIST_LENGTH - 1);
            if (count >= list->amt)
                break;

            DrawIcon(
                TILEMAP_LOCATED(gBG0TilemapBuffer, 2 + x * 2, 6 + y * 2),
                SKILL_ICON(list->sid[count]), 
                TILEREF(0, STATSCREEN_BGPAL_ITEMICONS));
        }
    }
    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

void PrepSkill1_InitTexts(void)
{
    int i;

    ResetText();

    /* 0x00 ~ 0x0D (size = 14): unit name */
    for (i = 0; i < 14; i++)
        InitText(&gPrepUnitTexts[i], 5);

    /* Left unit name */
    InitText(&gPrepUnitTexts[0x13], 7);
    InitText(&gPrepUnitTexts[0x14], 10);

    /* Right top bar */
    InitText(&gPrepUnitTexts[0x12], 3);
    InitText(&gPrepUnitTexts[0x15], 10);

    /* Left no-skills */
    InitText(&gPrepUnitTexts[0x16], 7);
}

void PrepSkill1_DrawRightTopBar(struct ProcPrepSkill1 * proc)
{
    struct Unit * unit = GetUnitFromPrepList(proc->list_num_cur);
    int color = AddSkill(unit, 0) == 0
              ? TEXT_COLOR_SYSTEM_WHITE
              : TEXT_COLOR_SYSTEM_GREEN;
    struct SkillList * llist = GetUnitSkillList(unit);

    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 16, 1), 16, 0x1, 0);

    PutDrawText(&gPrepUnitTexts[0x15], TILEMAP_LOCATED(gBG0TilemapBuffer, 14, 1), TEXT_COLOR_SYSTEM_WHITE, 0, 0, GetStringFromIndex(MSG_PREPSKILL_LeftTopBar));
    PutNumber(TILEMAP_LOCATED(gBG0TilemapBuffer, 24, 1), color, llist->amt);
    PutDrawText(&gPrepUnitTexts[0x12], TILEMAP_LOCATED(gBG0TilemapBuffer, 25, 1), TEXT_COLOR_SYSTEM_WHITE, 0, 0, "/");
    PutNumber(TILEMAP_LOCATED(gBG0TilemapBuffer, 26, 1), color, UNIT_RAM_SKILLS_LEN);
}
