#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, re

start = eval(sys.argv[1])

with open(sys.argv[2], "r") as fi:
    i = 1

    for line in fi.readlines():
        line = line.strip()

        if (len(line) == 0):
            continue

        if line[0] == '/':
            continue


        print(f"#define {line.split()[0]} 0x{(i + start):03X}")

        i = i + 1

        # If CONFIG_TURN_ALL_SKILLS_ON is enabled use this
        if i >= 0x3FF:
            break

        # If CONFIG_TURN_ALL_SKILLS_ON is disabled use this
        # if i >= 0xFF:
        #    break
