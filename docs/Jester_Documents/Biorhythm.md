# Biorhythm

##  Index 
- ### [Introduction](#Introduction)
- ### [Plan](#Plan)
- ### [Code Locations](#Code-Locations)
- ### [How To Modify](#How-To-Modify)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction

This is a controversial FE9 mechanic made to simulate characters having good days
and bad days. The idea being on the certain turns they will hit and dodge better and
on others they will be worse.

## Plan

This implementation offers the following:

- The ability to set the number of states
- The ability to set a per character value for each state
- The ability to set a per character starting position in the range of states

It works by gradually moving the unit through their various biorhythm states on
a turn by turn basis. Looping back around when it reaches the end.

E.g.

```
Turn 1 = -15 HIT/AVO
Turn 2 = -5 HIT/AVO
Turn 3 = 0 HIT/AVO
Turn 4 = 5 HIT/AVO
Turn 5 = 15 HIT/AVO
Turn 6 = -15 HIT/AVO
```

## Code Locations

- The functions are declared in [custom-arrays.h](../../include/jester_headers/custom-arrays.h)
    - The size of the biorhythm array is determined by ``MAX_BIORHYTHM_STATES``. Increase this if you want more of them
    - ``BiorhythmPInfoConfig`` is the struct that holds the different variables for Biorhythm. You can add new ones here like ``BiorhythmSpeed``
        for determining how fast a unit cycles though their states.

- And they are defined in [Biorhythm.c](../../Kernel/Wizardry/Misc/Biorhythm/Biorhythm.c)
    - There is enough room for unique values for 255 units
    - You can add more states per ``MAX_BIORHYTHM_STATES`` or change the values of each unit's hit/dodge biorhythms

## How To Modify

### Add a new unit

In [Biorhythm.c](../../Kernel/Wizardry/Misc/Biorhythm/Biorhythm.c) you can just add a new entry at the bottom of ``gBiorhythmPInfoConfigList``

e.g. 
```[CHARACTER_WHOEVER]   = { .biorhythm = { -15, -5, 0, 5, 15 }, .startOffset = 0 }```

### Change the Biorhythm values

In the same file above, where you see ``-15, -5, -, 5, 15``, change these to whatever values you want.

### Change starting position of Biorhythm

In the same file above, where you see ``startOffset`` change that to any value between 0 and ``MAX_BIORHYTHM_STATES - 1``

## Limitations/Bugs

None that I'm aware of.

Report in the ``issues`` tab of the repo.
