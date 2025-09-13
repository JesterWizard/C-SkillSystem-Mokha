#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"

bool IsDuraItem(int item)
{
    switch (ITEM_INDEX(item)) {
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_1
    case CONFIG_ITEM_INDEX_SKILL_SCROLL_1:
	case CONFIG_ITEM_INDEX_SKILL_SCROLL_FEB:
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

char *GetDuraItemName(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemName(item);

	Errorf("Invalid dura-item: %x", item);
	return NULL;
}

int GetDuraItemDescId(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemDescId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}

int GetDuraItemUseDescId(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemUseDescId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}

int GetDuraItemIconId(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemIconId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}
