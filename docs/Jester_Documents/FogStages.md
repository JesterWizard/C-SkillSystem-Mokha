# Make Fog Great Again!

![](../Gifs/Fog_Stages.gif)

##  Index 
- ### [Introduction](#Introduction)
- ### [Plan](#Plan)
- ### [Code Locations](#Code-Locations)
- ### [How To Modify](#How-To-Modify)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction
This is my plan to make fog more inclusive and strategic as a gameplay element.

Historically, fog has plagued every Fire Emblem it has appeared in mostly due to the fact
that its standard implementation stands in direct contradiction to how a strategy game
ought to be played. Wherein you make a plan of action based on the available map data to
determine which of your units should move to fight / defend.

Vanilla fog in the GBA games works by obscuring all enemy units outside of the sight
radius of the player units. In addition, you automatically lose your action if you
run into an enemy unit oscured by fog, and enemy units themselves have an unfair
advantage in that they can see the whole map.

## Plan
To combat these limitations, I propose a three stage revamp to fog

```
        3
      3 2 3
    3 2 1 2 3
  3 2 1 0 1 2 3
3 2 1 0 U 0 1 2 3
  3 2 1 0 1 2 3
    3 2 1 2 3
      3 2 3
        3
```

- The player unit sits in the center at ``U``.
- All tiles at ``0`` signify the player unit's 'sight'. All the data about an enemy unit is available at this stage.
- At ``stage 1`` enemy units can still be seen, but their status screen, MMB and forecast stats are hidden
- At ``stage 2`` enemy units are replaced with a ``citizen`` sprite, making it impossible to tell friend from foe
- At ``stage 3`` enemy units are completely hidden as in vanilla fog. Alongside terrain.

## Code Locations

The modifications required to make this work are silo'd via the ``CONFIG_MULTIPLE_FOG_STAGES`` preprocessor definition
within each function that modifications were made.

- Access to the stat screen is controlled within the ``CanShowUnitStatScreen`` and ``FindNextUnit`` functions at [AccessStatScreen.c](../../Data/StatScreen/Source/AccessStatScreen.c)

- Whether or not to hide enemy units in fog is controlled within the ``RefreshUnitsOnBmMap`` function at [MiscFunctions.c](../../Kernel/Wizardry/Misc/MiscFunctions/Source/MiscFunctions.c)

- Whether or not to show the enemy units' battle forecast data is controlled within the ``DrawBattleForecastContentsStandard`` and ``DrawBattleForecastContentsExtended`` functions are [BattleForecast.c](../../Kernel/Wizardry/Core/CombatArt/BKSELfx/Source/BattleForcast.c)

## How To Modify

### TODO

## Limitations and Bugs

As of the time of writing, stages 2 and 3 of the fog still need fleshing out mechanically and graphically.
There are issues that need resolving with having multiple different fogs occupying the same layer.
Ideally they should all be able to programtically share the same space so long as the total palette sits
below 16 colors, but we'll see.

``JESTER_DOC``