# FE8U_bases
A patch for FE8U. Allows the player to enter a previously completed chapter from the world map with a different set of events. These chapters have [free](https://feuniverse.us/t/smes-asm-and-miscellany/4445/108) [movement](https://feuniverse.us/t/fe8-freemovement-rework/13332) enabled.

![Gif broken](https://i.imgur.com/z8ZfXr0.gif)

## Implementation / Features
This patch makes use of [Sme's](https://feuniverse.us/t/smes-asm-and-miscellany/4445/108) and [Mokha's](https://feuniverse.us/t/fe8-freemovement-rework/13332) Free movement patch. It also includes [Sme's Trap Rework](https://feuniverse.us/t/smes-asm-and-miscellany/4445/86) and [7743's PLIST Split](https://feuniverse.us/t/fe-builder-gba-if-you-have-any-questions-attach-report7z/2845/872). Some changes were made to Free Movement (FMU):
- When FMU starts, the initially controlled unit is set to be the one at the start of the [`gGMData.units[]`](https://github.com/StanHash/FE-CLib/blob/973d069e6dabb21af6c6a3a27c0ff65139959075/include/gbafe/wm.h#L12) [array](https://github.com/StanHash/DOC/blob/de69d185c39c3d654f0b07784b20ea0309ca7fbc/WMSave.txt#L9). This was done as I'd otherwise have to find a free/redundant byte in SRAM to save to or adapt [EMS](https://github.com/FireEmblemUniverse/SkillSystem_FE8/blob/master/EngineHacks/Necessary/ExpandedModularSave/ExModularSave.event) to this patch as well, which could introduce more snags for the user when adapting the patch to an existing project.
- Originally an unsaved flag was used to determine if FMU is active. In this adaptation Chapters are marked as either base or non-base chapters in ROM. Now, the game determines if FMU is active by checking if either the original flag is active, or the chapter is a base chapter (or both hold true). Like the previous point, this was done to avoid having to adapt [EMS](https://github.com/FireEmblemUniverse/SkillSystem_FE8/blob/master/EngineHacks/Necessary/ExpandedModularSave/ExModularSave.event).
- FMU originally allowed the player to switch which unit is controlled by pressing select. Now, a global flag needs to be set to allow this functionality. This flag can be configured in [FreeMovement.event](MokhaFreeMU/cFreeMU/FreeMovement.event#L18).
- The controlled unit can face in one of four directions (North, East, South, West). If you wish to trigger a talk event, the controlled unit needs to be facing the unit whose event you wish to trigger. Like vanilla, the talk event still requires two specific units to trigger it. As the direction the unit faces is not preserved when switching the controlled unit, I'd recommend leaving the flag mentioned in the previous point unset. Facing direction could be stored in two bits in a unit's RAM UnitStruct. I'll leave implementing that as an exercise to the reader.

Other additions were made as well:
- To determine whether a worldmap node has an available base chapter, the third bit (0000 0X00) of [the flags](https://github.com/StanHash/DOC/blob/de69d185c39c3d654f0b07784b20ea0309ca7fbc/WMSave.txt#L14) in the [world map nodes struct](https://github.com/StanHash/FE-CLib/blob/973d069e6dabb21af6c6a3a27c0ff65139959075/include/gbafe/wm.h#L14) was used. Redundancy in the [world map save data](https://github.com/StanHash/DOC/blob/de69d185c39c3d654f0b07784b20ea0309ca7fbc/WMSave.txt#L34) was used to save the 29 extra bits (one per world map node, of which vanilla has 29), without relying on or hindering the potential presence of [EMS](https://github.com/FireEmblemUniverse/SkillSystem_FE8/blob/master/EngineHacks/Necessary/ExpandedModularSave/ExModularSave.event). If you've replaced or modified the functions used to save or load world map struct data ([EMS](https://github.com/FireEmblemUniverse/SkillSystem_FE8/blob/master/EngineHacks/Necessary/ExpandedModularSave/ExModularSave.event) does **not** replace these by default), you will probably need to modify the functions in the [save](src/save) directory. Details on what redundancy was exploited can be found in [loadWMSave.asm](src/save/loadWMSave.asm) and [saveWMSave.asm](src/save/saveWMSave.asm) (repeat).
- An [ASMC, or ASM Call,](src/ASMC/ASMC.event) was added to set and unset the flags determining a world map node has a base available or not. There are three macros:
  - WLC_baseChapterAvailable(nodeID): Makes base available for given world map nodeID.
  - WLC_baseChapterUnavailable(nodeID): Makes base unavailable for given world map nodeID.
  - WLC_baseChapterInvertAvailability(nodeID): for a given world map nodeID, makes its base unavailable if it were available before, available if it were unavailable before.
  
## Building
Run [AssembleText.bat](text/AssembleText.bat) to generate text entries. Then, apply [WMLoadChapter.event](WMLoadChapter.event) to a clean FE8U ROM using [Everything Assembler, AKA EA](https://feuniverse.us/t/event-assembler/1749). Borgo Ridge has a few interactable map events (map objects and other people to talk to). You can enable the base using one of the [ASMC macros](src/ASMC/ASMC.event) and get a feel for things.

## Adapting to an existing project
If you're making a bigger ROMHack or project involving multiple patches and you're using [FEBuilderGBA](https://github.com/FEBuilderGBA/FEBuilderGBA/releases/) as your main tool, you might be familiar with [Custom Build](https://dw.ngmansion.xyz/doku.php?id=en:en:guide:febuildergba:skillsystems_custombuild). Whilst it's generally not recommended to mix buildfile patches and FEBuilderGBA, if the patch you're interested in using is not available in FEBuilderGBA, Custom Build is one way to still adapt the patch.

If you're using Custom Build or buildfiles as your tool/editor/w/e, pay attention to the following things when adapting the patch to your project:
- TextProcess is used in this patch. If you're already using TextProcess, re-organize the [text](text) directory.
- ROM Chapterdata has been repointed to allow the extra base chapters to be added. If you've already repointed Chapterdata, re-organize [chapters.event](baseChapters/chapters/chapters.event).
- Vanilla makes use of a switch statement at 0xBD084 to determine if a chapter has skirmish/valni/lagdou settings, or is merely used as story. This patch repoints this switch statement in [chapterType.event](tables/chapterType/chapterType.event). If you've already repointed this, you might want to re-organize.
- Events for the base chapters can be found in [events.event](baseChapters/events/events.event). If you're keeping your chapter events elsewhere, you might want to move these there as well.
- The PLIST split is applied in [PLISTSplit.event](tables/PLIST/PLISTSplit.event). This directory also contains the other PLISTs. Re-organize as you deem fit.
- If you're already applying TrapRework, you'll need to re-organize the [TrapRework](MokhaFreeMU/TrapRework) directory.
- If you're already using a different version of FreeMovement, you should probably remove that patch.
- There's a table, indexed by Standing Map Sprite (SMS) IDs in [gfx.event](MokhaFreeMU/cFreeMU/gfx/gfx.event). Each entry in the table consists of a pointer to compressed graphics of the different direction-facing SMSes. Only one entry, corresponding to Eirika's SMS, has a pointer, the others are NULL. If you wish to add more graphics of SMSes facing left, right or up, format them the same way as [EirikaIdles.png](MokhaFreeMU/cFreeMU/gfx/EirikaIdles.png) and add their pointer to the table.

## Known Issues
- Sometimes, during Free Movement, the controlled unit staggers when moving. Not sure what causes it. Seems to happen when changing directions sometimes. Stopping movement and starting seems to be a way to fix it when it happens.

## Credits
Lemme know if I forgot anyone and I'll add them.
Credits to:
- [Sme](https://feuniverse.us/t/smes-asm-and-miscellany/4445/108) and [Mokha](https://feuniverse.us/t/fe8-freemovement-rework/13332) for the Free Movement patch.
- [CardCaph](https://discord.com/channels/144670830150811649/206588291053649921/1091416807874773162) for the SMS where Eirika faces left, right and up.
- [7743](https://feuniverse.us/t/fe-builder-gba-if-you-have-any-questions-attach-report7z/2845/872) for the PLIST Split.
- [Sme](https://feuniverse.us/t/smes-asm-and-miscellany/4445/86) for the Trap Rework.
- [circleseverywhere](https://feuniverse.us/t/text-processor-for-use-with-ea-v10-1-updated-to-v2-1/1776) for textprocess
- [StanH and HyperGammaSpaces](https://github.com/StanHash/DOC/tree/de69d185c39c3d654f0b07784b20ea0309ca7fbc) for all the documentation they wrote regarding world map stuff and other stuff.
- Huichelaar for this patch.