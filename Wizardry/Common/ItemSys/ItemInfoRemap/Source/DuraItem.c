#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"

bool IsDuraItem(int item)
{
	switch (ITEM_INDEX(item)) {
	case CONFIG_ITEM_INDEX_SKILL_SCROLL:
		return true;

	default:
		return false;
	}
}

char *GetDuraItemName(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemName(item);

<<<<<<< HEAD
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemName(item);

    Errorf("Invalid dura-item: %x", item);
    return NULL;
=======
	Errorf("Invalid dura-item: %x", item);
	return NULL;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

int GetDuraItemDescId(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemDescId(item);

<<<<<<< HEAD
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemDescId(item);

    Errorf("Invalid dura-item: %x", item);
    return 0;
=======
	Errorf("Invalid dura-item: %x", item);
	return 0;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

int GetDuraItemUseDescId(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemUseDescId(item);

<<<<<<< HEAD
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemUseDescId(item);

    Errorf("Invalid dura-item: %x", item);
    return 0;
=======
	Errorf("Invalid dura-item: %x", item);
	return 0;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

int GetDuraItemIconId(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemIconId(item);

<<<<<<< HEAD
    if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_ARMS_SCROLL)
        return GetSkillScrollItemIconId(item);

    Errorf("Invalid dura-item: %x", item);
    return 0;
=======
	Errorf("Invalid dura-item: %x", item);
	return 0;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}
