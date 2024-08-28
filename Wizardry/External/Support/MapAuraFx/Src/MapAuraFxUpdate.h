#ifndef MAP_AURA_FX_INCLUDED
#define MAP_AURA_FX_INCLUDED

#include "gbafe.h"

void StartMapAuraFx(void);
void EndMapAuraFx(void);
int  IsMapAuraFxActive(void);
void SetMapAuraFxSpeed(int speed);
void SetMapAuraFxBlend(unsigned blend);
void SetMapAuraFxPalette(const u16 palette[]);
void AddMapAuraFxUnit(struct Unit* unit);

typedef struct LCDIOBuffer LCDIOBuffer;
typedef struct KeyStatusBuffer KeyStatusBuffer;

struct DispControl
{
    unsigned bgMode : 3;
    unsigned : 1; // cgbMode
    unsigned bmpFrameNum : 1;
    unsigned hblankIntervalFree : 1;
    unsigned obj1dMap : 1;
    unsigned forcedBlank : 1;
    unsigned enableBg0 : 1;
    unsigned enableBg1 : 1;
    unsigned enableBg2 : 1;
    unsigned enableBg3 : 1;
    unsigned enableObj : 1;
    unsigned enableWin0 : 1;
    unsigned enableWin1 : 1;
    unsigned enableObjWin : 1;
};

struct WinControl
{
    unsigned win0_enableBg0 : 1;
    unsigned win0_enableBg1 : 1;
    unsigned win0_enableBg2 : 1;
    unsigned win0_enableBg3 : 1;
    unsigned win0_enableObj : 1;
    unsigned win0_enableBlend : 1;
    unsigned : 2;

    unsigned win1_enableBg0 : 1;
    unsigned win1_enableBg1 : 1;
    unsigned win1_enableBg2 : 1;
    unsigned win1_enableBg3 : 1;
    unsigned win1_enableObj : 1;
    unsigned win1_enableBlend : 1;
    unsigned : 2;

    unsigned wout_enableBg0 : 1;
    unsigned wout_enableBg1 : 1;
    unsigned wout_enableBg2 : 1;
    unsigned wout_enableBg3 : 1;
    unsigned wout_enableObj : 1;
    unsigned wout_enableBlend : 1;
    unsigned : 2;

    unsigned wobj_enableBg0 : 1;
    unsigned wobj_enableBg1 : 1;
    unsigned wobj_enableBg2 : 1;
    unsigned wobj_enableBg3 : 1;
    unsigned wobj_enableObj : 1;
    unsigned wobj_enableBlend : 1;
    unsigned : 2;
};

struct BlendControl
{
    unsigned target1_enableBg0 : 1;
    unsigned target1_enableBg1 : 1;
    unsigned target1_enableBg2 : 1;
    unsigned target1_enableBg3 : 1;
    unsigned target1_enableObj : 1;
    unsigned target1_backdrop : 1;

    unsigned effect : 2;

    unsigned target2_enableBg0 : 1;
    unsigned target2_enableBg1 : 1;
    unsigned target2_enableBg2 : 1;
    unsigned target2_enableBg3 : 1;
    unsigned target2_enableObj : 1;
    unsigned target2_backdrop : 1;

    unsigned : 2;
};


struct LCDIOBuffer {
	/* 00 */ struct DispControl dispControl;
	/* 04 */ struct DispStat dispStat;

	/* 08 */ u32 _u08;

	/* 0C */ struct BgControl bgControl[4];
	/* 1C */ struct Vec2u bgOffset[4];

	/* 2C */ u16 win0h;
	/* 2E */ u16 win1h;
	/* 30 */ u16 win0v;
	/* 32 */ u16 win1v;

	/* 34 */ struct WinControl winControl;
	
	/* 38 */ u16 mosaic;
	
	/* 3C */ struct BlendControl blendControl;
	
	/* 40 */ u32 _u40;
	/* 44 */ u8 blendCoeffA;
	/* 45 */ u8 blendCoeffB;
	/* 46 */ u8 blendY;
	
	/* 48 */ u16 bg2pa;
	/* 4A */ u16 bg2pb;
	/* 4C */ u16 bg2pc;
	/* 4E */ u16 bg2pd;
	/* 50 */ u32 bg2x;
	/* 54 */ u32 bg2y;

	/* 58 */ u16 bg3pa;
	/* 5A */ u16 bg3pb;
	/* 5C */ u16 bg3pc;
	/* 5E */ u16 bg3pd;
	/* 60 */ u32 bg3x;
	/* 64 */ u32 bg3y;

	/* 68 */ s8 colorAddition;
};

struct BgControl
{
    unsigned priority : 2;
    unsigned tileBaseBlock : 2;
    unsigned : 2;
    unsigned mosaic : 1;
    unsigned colorMode : 1;
    unsigned mapBaseBlock : 5;
    unsigned areaOverflowMode : 1;
    unsigned screenSize : 2;
};

struct BattleMapState {
	/* 00 */ u8 boolMainLoopEnded; // Used by vblank handler to detect "lag"
	/* 01 */ u8 proc2LockCount;
	/* 02 */ u8 gfxLockCount;
	/* 03 */ u8 _unk03;
	/* 04 */ u8 statebits; // TODO: enumerate bits
	/* 06 */ u16 savedVCount;
	/* 08 */ u32 _unk08;
	/* 0C */ struct Vec2u cameraRealPos;
	/* 10 */ struct Vec2u _unk10;
	/* 14 */ struct Vec2u cursorMapPos;
	/* 18 */ struct Vec2u cursorMapPosPrev;
	/* 1C */ struct Vec2u _unk1C;
	/* 20 */ struct Vec2u cursorDisplayRealPos;
	/* 24 */ struct Vec2 _unk24;
	/* 28 */ u8 _pad28[0x3C - 0x28];
	/* 3C */ u8 _unk3C;
	/* 3D */ u8 partialActionTaken; // bits
};

extern u16 gBg2MapBuffer[0x400]; //! FE8U = 0x2023CA8
extern struct LCDIOBuffer gLCDIOBuffer; //! FE8U = 0x3003080
extern struct BattleMapState gGameState;

typedef struct Proc Proc;
#define ROOT_PROC_3 (Proc*)(3)

#endif // MAP_AURA_FX_INCLUDED
