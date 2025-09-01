# Modular Fog Vision Range

Add GIF

##  Index 
- ### [Introduction](#Introduction)
- ### [Plan](#Plan)
- ### [Code Locations](#Code-Locations)
- ### [How To Modify](#How-To-Modify)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction

By default in FE8, fog vision is very static with almost all units having a 1 tile range.
Thieves are an exception with a bonus +5. Other than this, your only other options have historically
been torch items or torch staves.

## Plan

I propose to make this more modular, with fog vision ranges based on a class by class basis.

This way you can have situatons where you can have far sighted archers (who realistically should have better vision than thieves). As well as flying units who can see further than your infantry etc.

## Code Locations

- The actual vision ranges are controlled in the ``GetUnitFogViewRange`` function in [FogVision.c](../../Kernel/Wizardry/Common/FogVision/FogVision.c)

- The drawing of the vision text in the stat screen is controlled in the ``DrawPage1TextCommon`` function in [Page1WithBWL.c](../../Kernel/Wizardry/Core/StatScreen/DrawUnitPage/PlanA/Page1WithBwl.c) or [Page1WithLeadershi.c](../../Kernel/Wizardry/Core/StatScreen/DrawUnitPage/PlanB/Page1WithLeadership.c)

- The drawing of the vision bar in the stat screen is controlled in the ``DisplayPage_WithBWL`` function in [Page1WithBwl.c](../../Kernel/Wizardry/Core/StatScreen/DrawUnitPage/PlanA/Page1WithBwl.c) or the ``DisplayPage_WithLeadership`` function in [Page1WithLeadership.c](../../Kernel/Wizardry/Core/StatScreen/DrawUnitPage/PlanB/Page1WithLeadership.c)

- The linking of each stat screen option as you cycle through them is controlled by the various ``RText`` arrays in [Page1WithBwlHelpBox.c](../../Kernel/Wizardry/Core/StatScreen/DrawUnitPage/PlanA/Page1WithBwlHelpBox.c) or [Page1WithLeadershipHelpBox.c](../../Kernel//Wizardry/Core/StatScreen/DrawUnitPage/PlanB/Page1WithLeadershipHelpBox.c)

## How To Modify

TODO

## Limitations/Bugs

None that I'm aware of.

Report in the ``issues`` tab of the repo.
