# Free Movement

##  Index
- ### [Introduction](#Introduction)
- ### [How To Use](#How-To-Use)
- ### [Limitations and Bugs](#Limitations-and-Bugs)

## Introduction

Free movement that enables typical JRPG movements on your units, so that they can freely move
around and change directions without needing to be restricted to their initial movement range.

This fork comes installed with the ASM Sme version of free movement.

## How To Use

This can be enabled in any ``.c``/``.h`` file that includes ``jester_headers/custom-functions.h``.

In your starting event, you just need to include ``ASMC(EnableFreeMovementASMC)``

## Limitations and Bugs

Please note, it seems that the free movement ASMC turns off if you save and suspend

It is only employed on the first unit in your deployment list, s don;t try to load
more than one and once.

You cannot interact with enemies to start fights or use typical commands like trade.