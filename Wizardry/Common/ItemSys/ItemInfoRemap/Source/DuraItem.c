#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"

bool IsDuraItem(int item)
{
    switch (ITEM_INDEX(item)) {
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL
    case CONFIG_ITEM_INDEX_SKILL_SCROLL:
        return true;
#endif
    default:
        return false;
    }
}

char * GetDuraItemName(int item)
{
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
        return GetSkillScrollItemName(item);
#endif
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemName(item);
#endif
    Errorf("Invalid dura-item: %x", item);
    return NULL;
}

int GetDuraItemDescId(int item)
{
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
        return GetSkillScrollItemDescId(item);
#endif
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemDescId(item);
#endif
    Errorf("Invalid dura-item: %x", item);
    return 0;
}

int GetDuraItemUseDescId(int item)
{
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
        return GetSkillScrollItemUseDescId(item);
#endif
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemUseDescId(item);
#endif
    Errorf("Invalid dura-item: %x", item);
    return 0;
}

int GetDuraItemIconId(int item)
{
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
        return GetSkillScrollItemIconId(item);
#endif
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemIconId(item);
#endif
    Errorf("Invalid dura-item: %x", item);
    return 0;
}
