# Tellius Skill Capacity

![](../Gifs/Tellius_Skill_Capacity.gif)

##  Index 
- ### [Introduction](#Introduction)
- ### [Plan](#Plan)
- ### [Code Locations](#Code-Locations)
- ### [TODO](#TODO)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction

One thing I've never really seen any GBA hacks do is introduce
a skill capacity system. This rectifies that.

For those unfamiliar, skill capacity is a concept introduced in
the Tellius games (FE9/FE10) and pretty much only those games.
It's aim was to add another layer of strategy to skill placements
on units by giving every unit a capacity meter and each skill a
capacity amount. Even if you have a spare skill slot, if your remaining
skill capacity is below the capacity of the skill you can't equip it.
This forces players to consider tradeoffs between different pairs of skills
rather than just slapping a ton of busted skills on one unit.

## Plan

The proposal is as follows:

- Each skill gets a capacity amount
- Every unit gets a capacity meter (starting at 50)
- Promoted units get an additional 25
- Any learned skills that would tip the unit over this capacity
amount are prevented from being learned

## Code Locations

### Skill capacity location
- The definitions in [Skills_Capacity.h](../../include/kernel/Skills_Capacity.h)
### Skill capacity getter
- ``GetSkillCapacity(u16 sid)`` in [Infos.c](../../Kernel/Wizardry/Core/SkillSys/kernel/Infos.c)
### Scroll usability with Skill capacity
- ``ItemUseAction_SkillScroll`` in [SkillScroll.c](../../Kernel/Wizardry/Core/SkillSys/SkillScroll/Source/SkillScroll.c)

## TODO

## Limitations/Bugs

Report in the ``issues`` tab of the repo.

- There are skills that reduce the capacity required for other skills.
An issue arises if this first skill is removed as now the unit might be over capacity
and yet still able to use skills they wouldn't normally be able to use.
I'm unsure how to remedy this, if at all.

- A similar situation comes to mind for skills that are learned though level up
or through other events, I don't currently have a check to prevent them from
being learned. Maybe that's somethint to look into.
