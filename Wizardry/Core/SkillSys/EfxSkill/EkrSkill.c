#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "combat-art.h"
#include "banim-hack.h"

#define LOCAL_TRACE 0

struct ProcEkrSkill {
	PROC_HEADER;
	struct Anim *anim;
	u8 cid;
	u16 sid_atk;
	u16 sid_def;
};

<<<<<<< HEAD
STATIC_DECLAR void EkrSkillOnInit(struct ProcEkrSkill *proc) {}

STATIC_DECLAR void EkrSkillOnEnd(struct ProcEkrSkill *proc) {}

STATIC_DECLAR void NewEfxSkillForAttacker(struct ProcEkrSkill *proc)
{
	if (COMBART_VALID(proc->cid))
		NewEfxCombatArt(proc->anim, proc->cid);
	else if (COMMON_SKILL_VALID(proc->sid_atk))
		NewEfxSkill(proc->anim, proc->sid_atk);
}

STATIC_DECLAR void NewEfxSkillForDefener(struct ProcEkrSkill *proc)
{
	/* For now, far-far anim is not supported */
	if (gEkrDistanceType == EKR_DISTANCE_FARFAR)
		return;

=======
STATIC_DECLAR void EfxSkillSetAnimState(struct Anim *anim)
{
	struct Anim *anim1, *anim2;

	anim1 = gAnims[GetAnimPosition(anim) * 2];
	anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];

	anim->state3 |= ANIM_BIT3_BLOCKING;
	anim->state |= ANIM_BIT_FROZEN;

	anim1->state3 |= ANIM_BIT3_BLOCKING;
	anim1->state |= ANIM_BIT_FROZEN;

	anim2->state3 |= ANIM_BIT3_BLOCKING;
	anim2->state |= ANIM_BIT_FROZEN;
}

STATIC_DECLAR void EfxSkillResetAnimState(struct Anim *anim)
{
	struct Anim *anim1, *anim2;

	anim1 = gAnims[GetAnimPosition(anim) * 2];
	anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];

	anim->state3 |= ANIM_BIT3_BLOCKEND;
	anim->state &= ~ANIM_BIT_FROZEN;

	anim1->state3 |= ANIM_BIT3_BLOCKEND;
	anim1->state &= ~ANIM_BIT_FROZEN;

	anim2->state3 |= ANIM_BIT3_BLOCKEND;
	anim2->state &= ~ANIM_BIT_FROZEN;
}

STATIC_DECLAR void EkrSkillOnInit(struct ProcEkrSkill *proc) {}

STATIC_DECLAR void EkrSkillOnEnd(struct ProcEkrSkill *proc)
{
	EfxSkillResetAnimState(proc->anim);
}

STATIC_DECLAR void NewEfxSkillForAttacker(struct ProcEkrSkill *proc)
{
	if (COMBART_VALID(proc->cid))
		NewEfxCombatArt(proc->anim, proc->cid);
	else if (COMMON_SKILL_VALID(proc->sid_atk))
		NewEfxSkill(proc->anim, proc->sid_atk);
}

STATIC_DECLAR void NewEfxSkillForDefener(struct ProcEkrSkill *proc)
{
	/* For now, far-far anim is not supported */
	if (gEkrDistanceType == EKR_DISTANCE_FARFAR)
		return;

>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
	if (COMMON_SKILL_VALID(proc->sid_def)) {
		LTRACEF("sid %d", proc->sid_def);
		NewEfxSkill(GetAnimAnotherSide(proc->anim), proc->sid_def);
	}
}

STATIC_DECLAR const struct ProcCmd ProcScr_EkrSkill[] = {
	PROC_NAME("EkrSkill"),
<<<<<<< HEAD
=======
	PROC_SLEEP(2),
	PROC_WHILE(EfxAnimNumberExists),
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
	PROC_YIELD,
	PROC_CALL(EkrSkillOnInit),
	PROC_YIELD,
	PROC_CALL(NewEfxSkillForAttacker),
	PROC_WHILE(EfxSkillExists),
	PROC_YIELD,
	PROC_CALL(NewEfxSkillForDefener),
	PROC_WHILE(EfxSkillExists),
	PROC_YIELD,
	PROC_CALL(EkrSkillOnEnd),
	PROC_END
};

/* Called from BanimHack */
void NewEkrSkill(struct Anim *anim)
{
	int round;
	struct ProcEkrSkill *proc;

	proc = Proc_Start(ProcScr_EkrSkill, PROC_TREE_3);
	proc->anim = anim;

	round = anim->nextRoundId - 1;

	proc->cid = GetEfxCombatArt(round);
	proc->sid_atk = GetActorEfxSkill(round);
	proc->sid_def = GetTargetEfxSkill(round);

	LTRACEF("cid %#x, sid-atk %#x, sid-def %#x",
		proc->cid, proc->sid_atk, proc->sid_def);

#if defined(CONFIG_USE_DEBUG) && defined(CONFIG_DEBUG_EFXSKILL)
	proc->sid_atk = 1;
	proc->sid_def = 2;
#endif
<<<<<<< HEAD
=======

	EfxSkillSetAnimState(anim);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

bool EkrSkillExists(void)
{
	return Proc_Exists(ProcScr_EkrSkill);
}