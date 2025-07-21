#include "common-chax.h"
#include "debuff.h"
#include "map-task.h"
#include "kernel-lib.h"
#include "stat-screen.h"

extern UnitIconWait unit_icon_wait_table[];

#define GetInfo(id) (unit_icon_wait_table[(id) & ((1<<7)-1)])

struct SMSHandle EWRAM_DATA gSMSHandleArray[100] = {};

LYN_REPLACE_CHECK(PutUnitSpriteIconsOam);
void PutUnitSpriteIconsOam(void)
{
    int i;
    bool icon_blinking;

    if (CheckFlag(0x84) != 0)
        return;

    icon_blinking = !((GetGameClock() & 0x3F) >= 0x28);

    PutChapterMarkedTileIconOam();

    for (i = 1; i <= 0xC0; i++)
    {
        int ix, iy, status;
        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit) || unit->state & US_HIDDEN)
            continue;

        // GetUnitSpriteHideFlag
        if (!unit->pMapSpriteHandle || (unit->pMapSpriteHandle->config & 0x80))
            continue;

        ix = unit->xPos * 16 - gBmSt.camera.x;
        iy = unit->yPos * 16 - gBmSt.camera.y;

        if (ix < -16 || ix > DISPLAY_WIDTH)
            continue;;

        if (iy < -16 || iy > DISPLAY_HEIGHT)
            continue;

        MapTaskVec.x = ix;
        MapTaskVec.y = iy;

        /**
         * 1. HpBar
         */
        if (unit->curHP != unit->maxHP) {
			int _cur = unit->curHP;
			int _max = unit->maxHP;

			if (_cur > _max)
				_cur = _max;

			MapTaskPutOamHi(
				MTSKCONF_HPBAR,
				OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(gpHpBarChr[k_udiv(_cur * 10, _max)]));
		}

#ifdef CONFIG_TALK_MAP_ICON
        if (GetTalkee(unit) != 0)
            MapTaskPutOamHi(MTSKCONF_TALK, OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0xE00 / 0x20));
#endif

        /**
         * 2. Check on blinking icon
         */
        if (icon_blinking)
        {
            /**
             * 3. Debuff icon
             */
            status = GetUnitStatusIndex(unit);
            if (status != UNIT_STATUS_NONE)
            {
                const struct DebuffInfo * info = &gpDebuffInfos[status];
                if (info->on_draw)
                    info->on_draw(unit, ix, iy);
                else
                {
                    switch (info->positive_type) {
                    case STATUS_DEBUFF_NEGATIVE:
                        MapTaskPutOamHi(MTSKCONF_COMM_DEBUFF, OAM2_LAYER(2));
                        break;

                    case STATUS_DEBUFF_POSITIVE:
                        MapTaskPutOamHi(MTSKCONF_COMM_BUFF, OAM2_LAYER(2));
                        break;
                    }
                }
            }
            else
            {
                switch (SimulateStatDebuffPositiveType(unit)) {
                case STATUS_DEBUFF_NEGATIVE:
                    MapTaskPutOamHi(MTSKCONF_COMM_DEBUFF, OAM2_LAYER(2));
                    break;

                case STATUS_DEBUFF_POSITIVE:
                    MapTaskPutOamHi(MTSKCONF_COMM_BUFF, OAM2_LAYER(2));
                    break;
                }
            }

            /**
             * 4. Misc
             */
            if (unit->state & US_RESCUING)
            {
                static const u16 pal_idx[] = { 0xC, 0xE, 0xD };
                int faction = unit->rescue >> 6;

                MapTaskPutOamHi(
                    MTSKCONF_RESCUING,
                    OAM2_PAL(pal_idx[faction]) + OAM2_LAYER(2) + OAM2_CHR(0x60 / 0x20));
            }
            else if ((UNIT_FACTION(unit) != FACTION_BLUE) && (UNIT_CATTRIBUTES(unit) & CA_BOSS))
            {
                MapTaskPutOamHi(
                    MTSKCONF_BOSS,
                    OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x200 / 0x20));
            }
            else if (UNIT_CHAR_ID(unit) == GetROMChapterStruct(gPlaySt.chapterIndex)->protectCharacterIndex)
            {
                MapTaskPutOamHi(
                    MTSKCONF_PROTECTION,
                    OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x220 / 0x20));
            }
        }
    }
}

LYN_REPLACE_CHECK(PutUnitSpritesOam);
void PutUnitSpritesOam(void)
{
    struct SMSHandle * it = gSMSHandleArray->pNext;

    PutUnitSpriteIconsOam();

    if (it == NULL)
        return;

    for (; it != NULL; it = it->pNext)
    {
        int r3 = 0;

        int x = it->xDisplay - gBmSt.camera.x;
        int y = it->yDisplay - gBmSt.camera.y;

        if (x < -16 || x > DISPLAY_WIDTH)
            continue;

        if (y < -32 || y > DISPLAY_HEIGHT)
            continue;

        if (it->config & 0x80)
            continue;

        if (it->config & 0x40)
            r3 = GetGameClock() & 2;

        switch ((it->config & 0xf)) {
        case 0:
            CallARM_PushToSecondaryOAM(OAM1_X(x+r3+0x200), OAM0_Y(0x100+y), gObject_16x16, it->oam2Base + OAM2_LAYER(2));
            break;

        case 1:
            CallARM_PushToSecondaryOAM(OAM1_X(x+r3+0x200), OAM0_Y(0x100+y - 16), gObject_16x32, it->oam2Base + OAM2_LAYER(2));
            break;

        case 2:
            CallARM_PushToSecondaryOAM(OAM1_X((x-8)+r3+0x200), OAM0_Y(0x100+y - 16), gObject_32x32, it->oam2Base + OAM2_LAYER(2));
            break;

        case 3:
            CallARM_PushToSecondaryOAM(OAM1_X(x+r3+0x200), OAM0_Y(0x100+y), gObject_16x16, it->oam2Base + OAM2_LAYER(3));
            break;

        case 4:
            CallARM_PushToSecondaryOAM(OAM1_X(x+r3+0x200), OAM0_Y(0x100+y - 16), gObject_16x32, it->oam2Base + OAM2_LAYER(3));
            break;

        case 5:
            CallARM_PushToSecondaryOAM(OAM1_X((x-8)+r3+0x200), OAM0_Y(0x100+y - 16), gObject_32x32, it->oam2Base + OAM2_LAYER(3));
            break;
        }
    }
}

LYN_REPLACE_CHECK(PutUnitSprite);
void PutUnitSprite(int layer, int x, int y, struct Unit * unit)
{
    u32 id = GetUnitSMSId(unit);
    int chr = UseUnitSprite(id);

    if (x < -16 || x > DISPLAY_WIDTH)
        return;

    if (y < -32 || y > DISPLAY_HEIGHT)
        return;

    switch (GetInfo(id).size) {
    case UNIT_ICON_SIZE_16x16:
        PutSprite(layer, x, y, gObject_16x16, (GetUnitDisplayedSpritePalette(unit) & 0xf) * 0x1000 + 0x880 + chr);
        break;

    case UNIT_ICON_SIZE_16x32:
        PutSprite(layer, x, y - 16, gObject_16x32, (GetUnitDisplayedSpritePalette(unit) & 0xf) * 0x1000 + 0x880 + chr);
        break;

    case UNIT_ICON_SIZE_32x32:
        PutSprite(layer, x - 8, y - 16, gObject_32x32, (GetUnitDisplayedSpritePalette(unit) & 0xf) * 0x1000 + 0x880 + chr);
        break;
    }
}

LYN_REPLACE_CHECK(GetUnitSpritePalette);
int GetUnitSpritePalette(const struct Unit * unit)
{
    switch (UNIT_FACTION(unit)) {
    case FACTION_BLUE:
        return 0xC;

    case FACTION_RED:
        return 0xD;

    case FACTION_GREEN:
        return 0xE;

    case FACTION_PURPLE:
        return 0xB;
    }

    return 0; // BUGFIX for vanilla
}

LYN_REPLACE_CHECK(GetUnitDisplayedSpritePalette);
int GetUnitDisplayedSpritePalette(const struct Unit * unit)
{
    if (unit->state & US_BIT27)
        return 0xB;

    if (unit->state & US_UNSELECTABLE)
        return 0xF;

    return GetUnitSpritePalette(unit);
}