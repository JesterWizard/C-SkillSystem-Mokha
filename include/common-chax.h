#pragma once

#include "gbafe.h"
#include <stdint.h>

#include "Configs/configs.h"
#include "debug-kit.h"
#include "vanilla.h"

/* Add an identifier so that we can figure out where we hacked inside C */
#define CHAX 1
#define BUGFIX 1

/* Set static declaration as a macro for debug purpose */
#ifdef CONFIG_USE_DEBUG
#define STATIC_DECLAR
#else
#define STATIC_DECLAR static
#endif

#ifndef hang
#define hang() do { while (1); } while (0)
#endif

#define FORCE_DECLARE __attribute__((unused))
#define NOINLINE __attribute__((noinline))

#define LYN_REPLACE_CHECK(name) \
	static const void *const __attribute__((unused)) lyn_exists_check_ ## name = &name

/* Pointer valid judge */
#define IS_IWRAM_PTR(ptr)    ((((u32)(ptr)) & 0xFF000000) == 0x03000000)
#define IS_EWRAM_PTR(ptr)    ((((u32)(ptr)) & 0xFF000000) == 0x02000000)
#define IS_ROM_PTR(ptr)      ((((u32)(ptr)) & 0xF8000000) == 0x08000000)
#define IS_ROM_THUMB(ptr)    ((((u32)(ptr)) & 0xF8000001) == 0x08000001)
#define IS_ROM_DATA(ptr)     ((((u32)(ptr)) & 0xF8000001) == 0x08000000)
#define IS_ANTI_HUFFMAN(ptr) ((((u32)(ptr)) & 0xF8000000) == 0x88000000)

#define IS_UNIT_PTR(unit)    (UNIT_IS_VALID((unit)))
#define UNIT_ALIVE(unit)     (UNIT_IS_VALID((unit)) && !((unit)->state & US_UNAVAILABLE))

#define UNIT_LEVEL_MAX_RE 25
#define UNIT_RECORDED_LEVEL_MAX 80

<<<<<<< HEAD
extern void SetGameOption(u8 index, u8 newValue);
extern void TryAddUnitToTradeTargetList(struct Unit* unit);
extern void MakeTradeTargetList(struct Unit* unit);
extern void TryAddUnitToRescueTargetList(struct Unit* unit);
extern s8 Vanilla_CanUnitCrossTerrain(struct Unit* unit, int terrain);
extern bool Generic_CanUnitBeOnPos(struct Unit *unit, s8 x, s8 y, int x2, int y2);


typedef bool (* HookProcFunc_t)(ProcPtr);
=======
typedef bool (*HookProcFunc_t)(ProcPtr proc);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
