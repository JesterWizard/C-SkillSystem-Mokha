void SetGameOptions() 
{
    SetGameOption(GAME_OPTION_ANIMATION, 2); // Set battle animations off
    SetGameOption(GAME_OPTION_TEXT_SPEED, 1); // Set game speed to max
    SetGameOption(GAME_OPTION_GAME_SPEED, 3); // Set text speed to max

    SetPartyGoldAmount(10000);

    struct Unit * unit = GetUnitFromCharId(CHARACTER_EIRIKA);
    unit->maxHP = 26;
    unit->curHP = 26;
}