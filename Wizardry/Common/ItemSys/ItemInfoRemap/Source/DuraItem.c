#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"

bool IsDuraItem(int item)
{
    switch (ITEM_INDEX(item)) {
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_1
    case CONFIG_ITEM_INDEX_SKILL_SCROLL_1:
        return true;
#endif
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_2
    case CONFIG_ITEM_INDEX_SKILL_SCROLL_2:
        return true;
#endif
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_3
    case CONFIG_ITEM_INDEX_SKILL_SCROLL_3:
        return true;
#endif
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_4
    case CONFIG_ITEM_INDEX_SKILL_SCROLL_4:
        return true;
#endif
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    case CONFIG_ITEM_INDEX_ARMS_SCROLL:
        return true;
#endif
    default:
        return false;
    }
}

char * GetDuraItemName(int item)
{
    return GetSkillScrollItemName(item);
}

int GetDuraItemDescId(int item)
{
    return GetSkillScrollItemDescId(item);
}

int GetDuraItemUseDescId(int item)
{
    return GetSkillScrollItemUseDescId(item);
}

int GetDuraItemIconId(int item)
{
    return GetSkillScrollItemIconId(item);
}
