#ifndef MODULAR_SAVE_H
#define MODULAR_SAVE_H

// #include "gbafe.h"
#include "common-chax.h"
#include "bmsave.h"
//#include "../../../../Gambit/include/StanH/chapterdata.h"

enum {
	SAVE_TYPE_GAME      = 0,
	SAVE_TYPE_SUSPEND   = 1,
	SAVE_TYPE_LINKARENA = 2,
	SAVE_TYPE_BLOCK6    = 3,
};

enum {
	SAVE_BLOCK_SAVE1      = 0,
	SAVE_BLOCK_SAVE2      = 1,
	SAVE_BLOCK_SAVE3      = 2,
	SAVE_BLOCK_SUSPEND    = 3,
	SAVE_BLOCK_SUSPENDALT = 4,
	SAVE_BLOCK_LINKARENA  = 5,
	SAVE_BLOCK_UNK6       = 6,
};

enum {
	SBM_MAGIC1_GAME      = 0x40624,
	SBM_MAGIC1_LINKARENA = 0x20112,
	SBM_MAGIC1_SLOT6     = 0x20223,

	SBM_MAGIC2           = 0x200A,
};

struct ChapterState {
	/* 00 */ u32 _u00;
	/* 04 */ u32 _u04;
	/* 08 */ u32 goldAmount;
	/* 0C */ u8 saveSlotIndex;
	/* 0D */ u8 visionRange;
	/* 0E */ u8 chapterIndex;
	/* 0F */ u8 currentPhase;
	/* 10 */ u16 turnNumber;
	/* 12 */ u8 xCursorSaved;
	/* 13 */ u8 yCursorSaved;
	/* 14 */ u8 chapterStateBits;
	/* 15 */ u8 weather;
	/* 16 */ u16 supportGainTotal;
	/* 18 */ u8 playthroughId;
	/* 19 */ u8 _u19;
	/* 1A */ u8 lastUnitListSortType;
	/* 1B */ u8 mode;
	/* 1C */ u8 unk1C[4]; // Weapon type lookup by weapon specifying which character ids may bypass the unusable bit????? (see FE8U:80174AC)

	/* 20 */ char playerName[0x2B - 0x20]; // unused outside of link arena (was tactician name in FE7); Size unknown

	/* 2B */
	u32 unk2B_1:1;
	u32:0;

	/* 2C */
	u32 unk2C_1:1;
	u32 unk2C_2:3;
	u32 unk2C_5:9;
	u32 unk2D_6:10;
	u32 unk2E_8:5;

	/* 30 */ u32 fundsTotalDifference;
	/* 34 */ u32 unk34;

	/* 38 */ u8 pad38[0x40 - 0x38];

	// option byte 1 (of 3)
	u32 unk40_1:1;
	u32 terrainWindowOption:1;
	u32 unitWindowOption:2;
	u32 autocursorOption:1;
	u32 textSpeedOption:2;
	u32 gameSpeedOption:1;

	u32 unk41_1:1;
	u32 muteSfxOption:1;
	u32 unk41_3:5;
	u32 subtitleHelpOption:1;

	u32 unk42_1:1;
	u32 unk42_2:1;
	u32 unk42_3:1;
	u32 unk42_4:1;
	u32 unk42_5:1;
	u32 unk42_6:1;
	u32 unk42_7:1;
	u32 unk42_8:1;

	u32 unk43_1:8;

	u8 unk44[0x48 - 0x44];

	u16 unk48;

	u16 unk4A_1 : 1;
	u16 unk4A_2 : 3;
};

extern struct ChapterState gChapterData; //! FE8U = (0x202BCF0)

struct SaveBlockDecl {
	/* 00 */ u16 offset;
	/* 02 */ u16 type;
};

struct SaveChunkDecl {
	/* 00 */ u16 offset;
	/* 02 */ u16 size;

	/* 04 */ void (*save)(void* target, unsigned size);
	/* 08 */ void (*load)(void* source, unsigned size);

	/* 0C */ u16 identifier;
};

extern const u8 gMS_ChapterStateChunkId;
extern const u8 gMS_BWLChunkId;
extern const u8 gMS_PermanentEidsChunkId;
extern const u8 gMS_ClaimFlagsChunkId;
extern const u8 gMS_WMDataChunkId;

extern const struct SaveBlockDecl gSaveBlockDecl[];
extern const u16 gSaveBlockTypeSizeLookup[];

extern const struct SaveChunkDecl gGameSaveChunks[];
extern const struct SaveChunkDecl gSuspendSaveChunks[];

void* MS_GetSaveAddressBySlot(unsigned slot);
const struct SaveChunkDecl* MS_FindGameSaveChunk(unsigned chunkId);
const struct SaveChunkDecl* MS_FindSuspendSaveChunk(unsigned chunkId);
void MS_LoadChapterStateFromGameSave(unsigned slot, struct ChapterState* target);
u32 MS_GetClaimFlagsFromGameSave(unsigned slot);

// TODO: add to libgbafe

void SaveUnit(struct Unit* unit, void* target) __attribute__((long_call));
//void LoadSavedUnit(void* source, struct Unit* unit) __attribute__((long_call));

void SaveWMStuff(void*, void*) __attribute__((long_call));
void LoadWMStuff(void*, void*) __attribute__((long_call));

extern u8 gSomeWMEventRelatedStruct;

void StoreRNStateToActionStruct(void) __attribute__((long_call));
void LoadRNStateFromActionStruct(void) __attribute__((long_call));

// Those aren't consistent:
// the packing function works with a buffer
// But the unpacking functions loads from SRAM directly
void PackUnitStructForSuspend(struct Unit* unit, void* target) __attribute__((long_call));
void UnpackUnitStructFromSuspend(void* source, struct Unit* unit) __attribute__((long_call));

void Set0203EDB4(u32 value) __attribute__((long_call));

#endif // MODULAR_SAVE_H
