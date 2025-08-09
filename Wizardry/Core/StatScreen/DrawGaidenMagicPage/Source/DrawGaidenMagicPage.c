#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "combat-art.h"
#include "help-box.h"
#include "jester_headers/gaiden-magic.h"

void DrawPage5Rework(void)
{
    DrawGaidenMagicPage();
}

void StartGaidenMagicScreenHelp(int pageid, struct Proc * proc) 
{
    gStatScreen.help = RTextGaidenMagic;
};

/* HelpBox API */
void HbPopuplate_GaidenWhiteMagicPageCommon(struct HelpBoxProc * proc)
{
    // struct SkillList * list = GetUnitSkillList(gStatScreen.unit);

    int whiteMagic = sWhiteMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[proc->info->mid];

    proc->item = whiteMagic;
    proc->mid  = GetItemDescId(whiteMagic);
    
};

void HbRedirect_GaidenWhiteMagicPageCommon(struct HelpBoxProc * proc)
{
    if (proc->info->mid < sWhiteMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].amt)
        return;

    switch (proc->moveKey) {
    case DPAD_DOWN:
        TryRelocateHbDown(proc);
        break;

    case DPAD_UP:
        TryRelocateHbUp(proc);
        break;

    case DPAD_LEFT:
        TryRelocateHbLeft(proc);
        break;

    case DPAD_RIGHT:
    default:
        TryRelocateHbRight(proc);
        break;
    }
}

void HbPopulate_GaidenBlackMagicPageCommon(struct HelpBoxProc * proc)
{
    int blackMagic = sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[proc->info->mid];
    NoCashGBAPrintf("This black magic ID is: %d", blackMagic);

    proc->item = blackMagic;
    proc->mid  = GetItemDescId(blackMagic);
};

void HbRedirect_GaidenBlackMagicPageCommon(struct HelpBoxProc * proc)
{
    sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].amt = 1;
    if (proc->info->mid < sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].amt)
        return;

    // NoCashGBAPrintf("proc->info->mid is: %d", proc->info->mid);
    // NoCashGBAPrintf("Black magic skill list amt is: %d", sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].amt);

    switch (proc->moveKey) {
    case DPAD_DOWN:
        TryRelocateHbDown(proc);
        break;

    case DPAD_UP:
        TryRelocateHbUp(proc);
        break;

    case DPAD_LEFT:
        TryRelocateHbLeft(proc);
        break;

    case DPAD_RIGHT:
    default:
        TryRelocateHbRight(proc);
        break;
    }
}
