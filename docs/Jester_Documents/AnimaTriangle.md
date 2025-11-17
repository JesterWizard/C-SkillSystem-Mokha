# Anima Triangle

![](../Gifs/Anima_Triangle.gif)

##  Index 
- ### [Introduction](#Introduction)
- ### [Plan](#Plan)
- ### [Code Locations](#Code-Locations)
- ### [TODO](#TODO)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction

This is a feature from FE4/FE5 where anima magic has a weapon triangle
in much the same way as standard weapons and magic do.

## Plan

The proposal is as follows:

- Fire -> Wind/Ice
- Wind/Ice -> Thunder
- Thunder -> Fire

The system is set up to support a maximum of three anima magics of each type
in a triangular relationship.

## Code Locations

### Anima storage struct
- ``weaponId_1`` and ``weaponId_2`` in ``WeaponTriangleItemConf`` in [battle-system.h](../../include/kernel/battle-system.h)
### Weapon Triangle relationship struct (and bonus damage/hit)
- ``gWeaponTriangleItemConf`` in [WeaponTriangle.c](../../Data/BattleSys/Source/WeaponTriangle.c)
### WTA logic
- ``#ifdef CONFIG_ANIMA_WEAPON_TRIANGLE`` in ``WTA_HandlerWeapon`` in [WeaponTriangle.c](../../Kernel/Wizardry/Core/BattleSys/Source/WeaponTriangle.c) (different file)

## TODO

## Limitations/Bugs

Report in the ``issues`` tab of the repo.

- There appears to be some slight lag when running this, not enought to be a problem but good to keep an eye on. I may revist this and try to resolve it.
