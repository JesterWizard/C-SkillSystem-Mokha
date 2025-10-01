#include "common-chax.h"
#include "hooks.h"
#include "stat-screen.h"
#include "skill-system.h"
#include "bwl.h"
#include "utf8.h"

typedef struct
{
    const int key;
    const char * quotes[2];   // up to 2 lines of quotes
    const char * age;         // character age (approximate where unknown)
    const char * height_cm;   // height in centimeters
    const char * likes[2];    // up to 2 likes
    const char * dislikes[2]; // up to 2 dislikes
} CharacterTraits;

static const CharacterTraits character_traits[] = 
{ 
    {0},

    { CHARACTER_EIRIKA,    
      {"I will fight to protect my", "friends, no matter the cost."},
      "17", "160cm",
      {"Peace", "Family"}, {"War", "Betrayal"} },

    { CHARACTER_SETH,      
      {"My blade exists to guard", "you, and you alone, my liege."},
      "27", "185cm",
      {"Duty", "Order"}, {"Injustice", "Cowards"} },

    { CHARACTER_GILLIAM,   
      {"Even if steel should break,", "I will not yield my post."},
      "28", "190cm",
      {"Discipline", "Loyalty"}, {"Neglect", "Disorder"} },

    { CHARACTER_FRANZ,     
      {"I’ll train until the day I", "become the knight I dream of."},
      "15", "165cm",
      {"Training", "Heroes"}, {"Fear", "Failure"} },

    { CHARACTER_MOULDER,   
      {"Though my body may falter,", "my faith will carry me on."},
      "45", "178cm",
      {"Prayer", "Guidance"}, {"Doubt", "Weakness"} },

    { CHARACTER_VANESSA,   
      {"With wings that pierce the sky,", "I’ll strike before they react."},
      "18", "165cm",
      {"Flying", "Freedom"}, {"Restraint", "Storms"} },

    { CHARACTER_ROSS,      
      {"One day I’ll be as strong as", "Dad, just wait and see me grow."},
      "13", "150cm",
      {"Adventure", "Dad"}, {"Losing", "Waiting"} },

    { CHARACTER_NEIMI,     
      {"Even if my hands tremble,", "I’ll steady my aim for you."},
      "14", "155cm",
      {"Archery", "Friends"}, {"Loneliness", "Fear"} },

    { CHARACTER_COLM,      
      {"No lock or chain can stop me,", "I’ll slip through every wall."},
      "15", "160cm",
      {"Freedom", "Gold"}, {"Rules", "Guards"} },

    { CHARACTER_GARCIA,    
      {"I’ll fight on so my son can", "see the man I used to be."},
      "40", "185cm",
      {"Family", "Training"}, {"Regret", "Weakness"} },

    { CHARACTER_INNES,     
      {"My arrows never falter,", "for I am prince of the bow."},
      "20", "182cm",
      {"Archery", "Honor"}, {"Failure", "Disrespect"} },

    { CHARACTER_LUTE,      
      {"The world will soon acknowledge", "my brilliance as its light."},
      "18", "162cm",
      {"Study", "Magic"}, {"Ignorance", "Mediocrity"} },

    { CHARACTER_NATASHA,   
      {"If the gods guide my hands, then", "no darkness can overcome."},
      "19", "165cm",
      {"Faith", "Kindness"}, {"Cruelty", "Despair"} },

    { CHARACTER_CORMAG,    
      {"I’ll honor my brother’s name", "with each lance I thrust ahead."},
      "24", "183cm",
      {"Honor", "Family"}, {"Betrayal", "Tyranny"} },

    { CHARACTER_EPHRAIM,   
      {"My spear will carve a path for", "all who follow me to peace."},
      "18", "175cm",
      {"Battle", "Leadership"}, {"Cowardice", "Oppression"} },

    { CHARACTER_FORDE,     
      {"Though I jest, when duty calls,", "I’ll fight as hard as any."},
      "23", "178cm",
      {"Painting", "Rest"}, {"Discipline", "War"} },

    { CHARACTER_KYLE,      
      {"Discipline will guide my", "every step until the end."},
      "22", "182cm",
      {"Duty", "Training"}, {"Laziness", "Folly"} },

    { CHARACTER_AMELIA,    
      {"Someday I’ll stand among the", "knights I always admired."},
      "13", "150cm",
      {"Knights", "Justice"}, {"Weakness", "Doubt"} },

    { CHARACTER_ARTUR,     
      {"With light shining before me,", "I’ll banish every shadow."},
      "19", "172cm",
      {"Light", "Books"}, {"Darkness", "Corruption"} },

    { CHARACTER_GERIK,     
      {"I’m just a mercenary, but", "I’ll protect my crew with steel."},
      "29", "185cm",
      {"Loyalty", "Freedom"}, {"Greed", "Dishonor"} },

    { CHARACTER_TETHYS,    
      {"If my dance can lift your heart,", "then my steps are not in vain."},
      "22", "165cm",
      {"Dance", "Beauty"}, {"Cruelty", "Silence"} },

    { CHARACTER_MARISA,    
      {"My sword will speak for me, in", "a silence sharper than words."},
      "19", "168cm",
      {"Swordplay", "Quiet"}, {"Noise", "Distraction"} },

    { CHARACTER_SALEH,     
      {"The wisdom of ages past", "will light the road before me."},
      "30", "180cm",
      {"Study", "Teaching"}, {"Arrogance", "Chaos"} },

    { CHARACTER_EWAN,      
      {"I’ll prove that even I can", "stand tall as a true mage."},
      "14", "155cm",
      {"Magic", "Adventure"}, {"Doubt", "Failure"} },

    { CHARACTER_LARACHEL,  
      {"Born perfect, I shall show you", "what true divinity is."},
      "18", "162cm",
      {"Perfection", "Attention"}, {"Neglect", "Dirt"} },

    { CHARACTER_DOZLA,     
      {"Ha! Nothing beats the feeling", "of battle and laughter both!"},
      "45", "190cm",
      {"Laughter", "Battle"}, {"Sadness", "Cowards"} },

    { CHARACTER_ENEMY_1B,  
      {"...", "..."},
      "???", "???",
      {"", ""}, {"", ""} },

    { CHARACTER_RENNAC,    
      {"If the pay’s high enough, then", "I suppose I’ll stick around."},
      "24", "178cm",
      {"Money", "Freedom"}, {"Work", "Risk"} },

    { CHARACTER_DUESSEL,   
      {"The strength of years may fade,", "but my honor will not break."},
      "50", "188cm",
      {"Honor", "Mentorship"}, {"Treachery", "Decay"} },

    { CHARACTER_MYRRH,     
      {"Though my power frightens me,", "I’ll use it to guard my friends."},
      "1200+", "140cm",
      {"Friends", "Nature"}, {"Loneliness", "War"} },

    { CHARACTER_KNOLL,     
      {"The dark reveals its secrets,", "but it demands a harsh price."},
      "28", "178cm",
      {"Research", "Truth"}, {"Naivety", "Faith"} },

    { CHARACTER_JOSHUA,    
      {"If luck remains on my side,", "then fortune will guide my hand."},
      "20", "180cm",
      {"Gambling", "Freedom"}, {"Fate", "Obligation"} },

    { CHARACTER_SYRENE,    
      {"With sky and lance together,", "I’ll shield those I must protect."},
      "27", "170cm",
      {"Duty", "Flight"}, {"Fear", "Cowardice"} },

    { CHARACTER_TANA,      
      {"Even if I stumble now,", "I’ll rise and soar again soon."},
      "16", "162cm",
      {"Freedom", "Family"}, {"Losing", "Restraint"} },

    { CHARACTER_LYON,      
      {"This power twists inside me,", "yet I must bear it alone."},
      "19", "175cm",
      {"Research", "Peace"}, {"Corruption", "Loneliness"} },

    { CHARACTER_ORSON,     
      {"Though my heart is lost to her,", "I’ll fight as if she still lives."},
      "28", "182cm",
      {"Love", "Memories"}, {"Reality", "Loss"} },

    { CHARACTER_ORSON_CH5X,
      {"Her memory drives me on,", "though the world has taken her."},
      "28", "182cm",
      {"Love", "Memories"}, {"Reality", "Loss"} },

    { CHARACTER_GLEN,      
      {"I’ll carry out my duty,", "though the weight is heavy still."},
      "26", "185cm",
      {"Honor", "Duty"}, {"Corruption", "Lies"} },

    { CHARACTER_SELENA,    
      {"The storm answers my command,", "and I’ll wield it to the end."},
      "25", "172cm",
      {"Loyalty", "Justice"}, {"Treachery", "Cruelty"} },

    { CHARACTER_VALTER,    
      {"Each battle feeds my hunger,", "each kill my twisted delight."},
      "29", "184cm",
      {"Battle", "Chaos"}, {"Peace", "Calm"} },

    { CHARACTER_RIEV,      
      {"The void whispers to me, and", "I obey its every call."},
      "40", "178cm",
      {"Darkness", "Power"}, {"Faith", "Light"} },

    { CHARACTER_CAELLACH,  
      {"I’ll climb and grasp the power", "that crowns the mightiest king."},
      "25", "183cm",
      {"Power", "Ambition"}, {"Weakness", "Fate"} },

    { CHARACTER_FADO,      
      {"A king’s burden is to stand", "when his people need him most."},
      "35", "185cm",
      {"Duty", "Peace"}, {"Cowardice", "Betrayal"} },

    { CHARACTER_ISMAIRE,   
      {"My sword must strike with honor,", "my grace its truest edge."},
      "32", "170cm",
      {"Grace", "Swordplay"}, {"Dishonor", "Violence"} },

    { CHARACTER_HAYDEN,    
      {"Though age may weigh my shoulders,", "my crown is my strength to bear."},
      "50", "182cm",
      {"Stability", "Wisdom"}, {"Chaos", "Loss"} },
};

	const int character_traits_size = sizeof(character_traits) / sizeof(character_traits[0]);

// ---------------------------------------------------------------------------
// Core helper: find traits by charId
// ---------------------------------------------------------------------------
static const CharacterTraits *GetCharacterTraits(int charId)
{
    for (int i = 0; i < character_traits_size; i++)
    {
        if (character_traits[i].key == charId)
            return &character_traits[i];
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// Unified getters
// ---------------------------------------------------------------------------
static inline const char *SafeStr(const char *s)
{
    return (s && s[0] != '\0') ? s : "";
}

const char *GetCharacterQuote(int charId, int index)
{
    const CharacterTraits *t = GetCharacterTraits(charId);
    if (!t || index < 0 || index >= 2) return "";
    return SafeStr(t->quotes[index]);
}

const char *GetCharacterAge(int charId)
{
    const CharacterTraits *t = GetCharacterTraits(charId);
    return t ? SafeStr(t->age) : "";
}

const char *GetCharacterHeight(int charId)
{
    const CharacterTraits *t = GetCharacterTraits(charId);
    return t ? SafeStr(t->height_cm) : "";
}

const char *GetCharacterLikes(int charId, int index)
{
    const CharacterTraits *t = GetCharacterTraits(charId);
    if (!t || index < 0 || index >= 2) return "";
    return SafeStr(t->likes[index]);
}

const char *GetCharacterDislikes(int charId, int index)
{
    const CharacterTraits *t = GetCharacterTraits(charId);
    if (!t || index < 0 || index >= 2) return "";
    return SafeStr(t->dislikes[index]);
}

// ---------------------------------------------------------------------------
// Display helpers
// ---------------------------------------------------------------------------
static void DisplayLabeledText(int textIndex, int x, int y, int color, const char *label)
{
    PutDrawText(&gStatScreen.text[textIndex], gUiTmScratchA + TILEMAP_INDEX(x, y), color, 0, 0, label);
}

static void DisplayValueText(int textIndex, int x, int y, int color, const char *value)
{
    PutDrawText(&gStatScreen.text[textIndex], gUiTmScratchA + TILEMAP_INDEX(x, y), color, 0, 0, value);
}

void DisplayQuote(struct Unit *unit, int charId)
{
    DisplayLabeledText(15, 0x1, 0xB, TEXT_COLOR_SYSTEM_GREEN, "Quote:");
    DisplayValueText(  2, 0x1, 0xD, TEXT_COLOR_SYSTEM_WHITE, GetCharacterQuote(charId, 0));
    DisplayValueText( 34, 0x1, 0xF, TEXT_COLOR_SYSTEM_WHITE, GetCharacterQuote(charId, 1));
}

void DisplayAge(struct Unit *unit, int charId)
{
    DisplayLabeledText(16, 0x1, 0x9, TEXT_COLOR_SYSTEM_GOLD, "Age:");
    DisplayValueText( 17, 0x4, 0x9, TEXT_COLOR_SYSTEM_WHITE, GetCharacterAge(charId));
}

void DisplayHeight(struct Unit *unit, int charId)
{
    DisplayLabeledText(18, 0x9, 0x9, TEXT_COLOR_SYSTEM_GOLD, "Height:");
    DisplayValueText( 19, 0xD, 0x9, TEXT_COLOR_SYSTEM_WHITE, GetCharacterHeight(charId));
}

void DisplayLikes(struct Unit *unit, int charId)
{
    DisplayLabeledText(31, 0x1, 0x1, TEXT_COLOR_SYSTEM_BLUE, "Likes:");
    DisplayValueText( 32, 0x9, 0x1, TEXT_COLOR_SYSTEM_WHITE, GetCharacterLikes(charId, 0));
    DisplayValueText( 33, 0x9, 0x3, TEXT_COLOR_SYSTEM_WHITE, GetCharacterLikes(charId, 1));
}

void DisplayDislikes(struct Unit *unit, int charId)
{
    DisplayLabeledText(14, 0x1, 0x5, TEXT_COLOR_SYSTEM_BLUE, "Dislikes:");
    DisplayValueText( 29, 0x9, 0x5, TEXT_COLOR_SYSTEM_WHITE, GetCharacterDislikes(charId, 0));
    DisplayValueText( 30, 0x9, 0x7, TEXT_COLOR_SYSTEM_WHITE, GetCharacterDislikes(charId, 1));
}

// ---------------------------------------------------------------------------
// Page draw
// ---------------------------------------------------------------------------
void DrawPage6Rework(void)
{
    struct Unit *unit = gStatScreen.unit;
    int charId = UNIT_CHAR_ID(unit);

    for (int i = 14; i < 21; i++)
        ClearText(&gStatScreen.text[i]);
    for (int i = 29; i < 35; i++)
        ClearText(&gStatScreen.text[i]);

    TileMap_FillRect(gUiTmScratchA + 0x34, 12, 1, 0);

    DisplayQuote(unit, charId);
    DisplayAge(unit, charId);
    DisplayHeight(unit, charId);
    DisplayLikes(unit, charId);
    DisplayDislikes(unit, charId);
}