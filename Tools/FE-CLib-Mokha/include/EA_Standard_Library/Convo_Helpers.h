#pragma once

#define TEXT(msg) \
    TEXTSTART \
    TEXTSHOW(msg) \
    TEXTEND \
    REMA

#define TEXT_CONSECUTIVE(msg) \
    EvtTextShow2(msg) \
    TEXTEND

#define TEXT_BG(bg, msg) \
    SVAL(EVT_SLOT_2, (bg)) \
    SVAL(EVT_SLOT_3, (msg)) \
    CALL(Event_TextWithBG)

#define SET_BACKGROUND(bg) \
    SVAL(EVT_SLOT_2, bg) \
    CALL(EventScr_SetBackground)
