/*
==============================================================================

mutant

==============================================================================
*/

#include "g_local.h"
#include "m_mutant.h"


static int	sound_swing;
static int	sound_hit;
static int	sound_hit2;
static int	sound_death;
static int	sound_idle;
static int	sound_pain1;
static int	sound_pain2;
static int	sound_sight;
static int	sound_search;
static int	sound_step1;
static int	sound_step2;
static int	sound_step3;
static int	sound_thud;

//
// SOUNDS
//

void mutant_step (edict_t *self)
{
	int		n;
	n = (rand() + 1) % 3;
	if (n == 0)
		gi.sound (self, CHAN_VOICE, sound_step1, 1, ATTN_NORM, 0);		
	else if (n == 1)
		gi.sound (self, CHAN_VOICE, sound_step2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_step3, 1, ATTN_NORM, 0);
}

void mutant_sight (edict_t *self, edict_t *other)
{
	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void mutant_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}

void mutant_swing (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_swing, 1, ATTN_NORM, 0);
}


//
// STAND
//

mframe_t mutant_frames_stand [] =
{
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,		// 10

	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,		// 20

	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,		// 30

	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,		// 40

	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,		// 50

	MANIM_STAND, 0, NULL
};
mmove_t mutant_move_stand = {FRAME_stand101, FRAME_stand151, mutant_frames_stand, NULL};

void mutant_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_stand;
}


//
// IDLE
//

void mutant_idle_loop (edict_t *self)
{
	if (random() < 0.75)
		self->monsterinfo.nextframe = FRAME_stand155;
}

mframe_t mutant_frames_idle [] =
{
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,					// scratch loop start
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, mutant_idle_loop,		// scratch loop end
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL
};
mmove_t mutant_move_idle = {FRAME_stand152, FRAME_stand164, mutant_frames_idle, mutant_stand};

void mutant_idle (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_idle;
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}


//
// WALK
//

void mutant_walk (edict_t *self);

mframe_t mutant_frames_walk [] =
{
	MANIM_WALK,	-5,		NULL,
	MANIM_WALK,	-7,		NULL,
	MANIM_WALK,	-3,		NULL,
	MANIM_WALK,	2,		NULL,
	MANIM_WALK,	5,		NULL,
	MANIM_WALK,	2,		NULL,
	MANIM_WALK,	-8,		NULL,
	MANIM_WALK,	-3,		NULL,
	MANIM_WALK,	-2,		NULL,
	MANIM_WALK,	8,		NULL,
	MANIM_WALK,	7,		NULL,
	MANIM_WALK,	-2,		NULL
};
mmove_t mutant_move_walk = {FRAME_walk05, FRAME_walk16, mutant_frames_walk, NULL};

void mutant_walk_loop (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_walk;
}

mframe_t mutant_frames_start_walk [] =
{
	MANIM_WALK,	-3,		NULL,
	MANIM_WALK,	-3,		NULL,
	MANIM_WALK,	-10,		NULL,
	MANIM_WALK,	-7,		NULL
};
mmove_t mutant_move_start_walk = {FRAME_walk01, FRAME_walk04, mutant_frames_start_walk, mutant_walk_loop};

void mutant_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_start_walk;
}


//
// RUN
//

mframe_t mutant_frames_run [] =
{
	MANIM_RUN,	15,		NULL,
	MANIM_RUN,	15,		mutant_step,
	MANIM_RUN,	-1,		NULL,
	MANIM_RUN,	-20,		mutant_step,
	MANIM_RUN,	-8,		NULL,
	MANIM_RUN,	-15,		NULL
};
mmove_t mutant_move_run = {FRAME_run03, FRAME_run08, mutant_frames_run, NULL};

void mutant_run (edict_t *self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &mutant_move_stand;
	else
		self->monsterinfo.currentmove = &mutant_move_run;
}


//
// MELEE
//

void mutant_hit_left (edict_t *self)
{
	vec3_t	aim;

	VectorSet (aim, MELEE_DISTANCE, self->mins[0], 8);
	if (fire_hit (self, aim, (10 + (rand() %5)), 20))
		gi.sound (self, CHAN_WEAPON, sound_hit, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_WEAPON, sound_swing, 1, ATTN_NORM, 0);
}

void mutant_hit_right (edict_t *self)
{
	vec3_t	aim;

	VectorSet (aim, MELEE_DISTANCE, self->maxs[0], 8);
	if (fire_hit (self, aim, (10 + (rand() %5)), 20))
		gi.sound (self, CHAN_WEAPON, sound_hit2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_WEAPON, sound_swing, 1, ATTN_NORM, 0);
}

void mutant_check_refire (edict_t *self)
{
	if (!self->enemy || !self->enemy->inuse || self->enemy->health <= 0)
		return;

	if ( ((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE) )
		self->monsterinfo.nextframe = FRAME_attack09;
}

mframe_t mutant_frames_attack [] =
{
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	0,	mutant_hit_left,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	0,	mutant_hit_right,
	MANIM_MELEE,	0,	mutant_check_refire
};
mmove_t mutant_move_attack = {FRAME_attack09, FRAME_attack15, mutant_frames_attack, mutant_run};

void mutant_melee (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_attack;
}


//
// ATTACK
//

void mutant_jump_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (self->health <= 0)
	{
		self->touch = NULL;
		return;
	}

	if (other->takedamage)
	{
		if (VectorLength(self->velocity) > 400)
		{
			vec3_t	point;
			vec3_t	normal;
			int		damage;

			VectorCopy (self->velocity, normal);
			VectorNormalize(normal);
			VectorMA (self->s.origin, self->maxs[0], normal, point);
			damage = 40 + 10 * random();
			T_Damage (other, self, self, self->velocity, point, normal, damage, 0, damage, 0, MOD_UNKNOWN);
		}
	}

	if (!M_CheckBottom (self))
	{
		if (self->groundentity)
		{
			self->monsterinfo.nextframe = FRAME_attack02;
			self->touch = NULL;
		}
		return;
	}

	self->touch = NULL;
}

void mutant_jump_takeoff (edict_t *self)
{
	vec3_t	forward;

	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
	AngleVectors (self->s.angles, forward, NULL, NULL);
	self->s.origin[2] += 1;
	VectorScale (forward, 600, self->velocity);
	self->velocity[2] = 250;
	self->groundentity = NULL;
	self->monsterinfo.aiflags |= AI_DUCKED;
	self->monsterinfo.attack_finished = level.time + 3;
	self->touch = mutant_jump_touch;
}

void mutant_check_landing (edict_t *self)
{
	if (self->groundentity)
	{
		gi.sound (self, CHAN_WEAPON, sound_thud, 1, ATTN_NORM, 0);
		self->monsterinfo.attack_finished = 0;
		self->monsterinfo.aiflags &= ~AI_DUCKED;
		return;
	}

	if (level.time > self->monsterinfo.attack_finished)
		self->monsterinfo.nextframe = FRAME_attack02;
	else
		self->monsterinfo.nextframe = FRAME_attack05;
}

mframe_t mutant_frames_jump [] =
{
	MANIM_MELEE,	 0,	NULL,
	MANIM_MELEE,	17,	NULL,
	MANIM_MELEE,	15,	mutant_jump_takeoff,
	MANIM_MELEE,	15,	NULL,
	MANIM_MELEE,	15,	mutant_check_landing,
	MANIM_MELEE,	 0,	NULL,
	MANIM_MELEE,	 3,	NULL,
	MANIM_MELEE,	 0,	NULL
};
mmove_t mutant_move_jump = {FRAME_attack01, FRAME_attack08, mutant_frames_jump, mutant_run};

void mutant_jump (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_jump;
}


//
// CHECKATTACK
//

qboolean mutant_check_melee (edict_t *self)
{
	if (range (self, self->enemy) == RANGE_MELEE)
		return true;
	return false;
}

qboolean mutant_check_jump (edict_t *self)
{
	vec3_t	v;
	float	distance;

	if (self->absmin[2] > (self->enemy->absmin[2] + 0.75 * self->enemy->size[2]))
		return false;

	if (self->absmax[2] < (self->enemy->absmin[2] + 0.25 * self->enemy->size[2]))
		return false;

	v[0] = self->s.origin[0] - self->enemy->s.origin[0];
	v[1] = self->s.origin[1] - self->enemy->s.origin[1];
	v[2] = 0;
	distance = VectorLength(v);

	if (distance < 100)
		return false;
	if (distance > 100)
	{
		if (random() < 0.9)
			return false;
	}

	return true;
}

qboolean mutant_checkattack (edict_t *self)
{
	float		chance;
	qboolean	is_sliding;

	if (self->monsterinfo.aiflags & AI_DODGING)
		self->monsterinfo.aiflags |= AI_SLIDING;
	else
		self->monsterinfo.aiflags &= ~AI_SLIDING;

	if (!self->enemy || self->enemy->health <= 0)
		return false;

	if (mutant_check_melee(self))
	{
		self->monsterinfo.attack_state = AS_MELEE;
		return true;
	}

	// TITANIUM--can strafe depending on skill
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		chance = 0;
	else if (skill->value == 0)
		chance = 0.3;
	else if (skill->value >= 2)
		chance = 0.7;
	else
		chance = 0.5;

	is_sliding = (random() < chance) || (self->monsterinfo.aiflags & AI_DODGING);

	if (mutant_check_jump(self))
	{
		// TITANIUM--jump
		if (is_sliding)
			self->monsterinfo.aiflags |= AI_SLIDING;
		else
			self->monsterinfo.aiflags &= ~AI_SLIDING;
		self->monsterinfo.attack_state = AS_MISSILE;
		// FIXME play a jump sound here
		return true;
	}

	return false;
}


//
// PAIN
//

mframe_t mutant_frames_pain1 [] =
{
	MANIM_MISC,	4,	NULL,
	MANIM_MISC,	-3,	NULL,
	MANIM_MISC,	-8,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	5,	NULL
};
mmove_t mutant_move_pain1 = {FRAME_pain101, FRAME_pain105, mutant_frames_pain1, mutant_run};

mframe_t mutant_frames_pain2 [] =
{
	MANIM_MISC,	-24,NULL,
	MANIM_MISC,	11,	NULL,
	MANIM_MISC,	5,	NULL,
	MANIM_MISC,	-2,	NULL,
	MANIM_MISC,	6,	NULL,
	MANIM_MISC,	4,	NULL
};
mmove_t mutant_move_pain2 = {FRAME_pain201, FRAME_pain206, mutant_frames_pain2, mutant_run};

mframe_t mutant_frames_pain3 [] =
{
	MANIM_MISC,	-22,NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	6,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	1,	NULL
};
mmove_t mutant_move_pain3 = {FRAME_pain301, FRAME_pain311, mutant_frames_pain3, mutant_run};

void mutant_pain (edict_t *self, edict_t *other, float kick, int damage)
{
	float	r;

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	r = random();
	if (r < 0.33)
	{
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &mutant_move_pain1;
	}
	else if (r < 0.66)
	{
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &mutant_move_pain2;
	}
	else
	{
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &mutant_move_pain3;
	}
}


//
// DEATH
//

void mutant_dead (edict_t *self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity (self);
}

mframe_t mutant_frames_death1 [] =
{
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	mutant_dead,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL
};
mmove_t mutant_move_death1 = {FRAME_death101, FRAME_death109, mutant_frames_death1, M_FlyCheck};

mframe_t mutant_frames_death2 [] =
{
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	mutant_dead,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL
};
mmove_t mutant_move_death2 = {FRAME_death201, FRAME_death210, mutant_frames_death2, M_FlyCheck};

void mutant_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int		n;

	if (self->gib_health <= 0)
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 2; n++)
			ThrowGib (self, "models/objects/gibs/bone/tris.md2", damage, GIB_ORGANIC);
		for (n= 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (self->deadflag == DEAD_DEAD)
		return;

	gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	if (random() < 0.5)
		self->monsterinfo.currentmove = &mutant_move_death1;
	else
		self->monsterinfo.currentmove = &mutant_move_death2;
}


//
// SPAWN
//

/*QUAKED monster_mutant (1 .5 0) (-32 -32 -24) (32 32 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_mutant (edict_t *self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict (self);
		return;
	}

	sound_swing = gi.soundindex ("mutant/mutatck1.wav");
	sound_hit = gi.soundindex ("mutant/mutatck2.wav");
	sound_hit2 = gi.soundindex ("mutant/mutatck3.wav");
	sound_death = gi.soundindex ("mutant/mutdeth1.wav");
	sound_idle = gi.soundindex ("mutant/mutidle1.wav");
	sound_pain1 = gi.soundindex ("mutant/mutpain1.wav");
	sound_pain2 = gi.soundindex ("mutant/mutpain2.wav");
	sound_sight = gi.soundindex ("mutant/mutsght1.wav");
	sound_search = gi.soundindex ("mutant/mutsrch1.wav");
	sound_step1 = gi.soundindex ("mutant/step1.wav");
	sound_step2 = gi.soundindex ("mutant/step2.wav");
	sound_step3 = gi.soundindex ("mutant/step3.wav");
	sound_thud = gi.soundindex ("mutant/thud1.wav");
	
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/mutant/tris.md2");
	VectorSet (self->mins, -32, -32, -24);
	VectorSet (self->maxs, 32, 32, 48);
	VectorSet (self->headmins, 10, -32, 16);
	VectorSet (self->headmaxs, 32, 32, 48);

	self->health = 300;
	self->gib_health = 420;
	self->mass = 300;

	self->pain = mutant_pain;
	self->die = mutant_die;

	self->monsterinfo.stand = mutant_stand;
	self->monsterinfo.walk = mutant_walk;
	self->monsterinfo.run = mutant_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.attack = mutant_jump;
	self->monsterinfo.melee = mutant_melee;
	self->monsterinfo.sight = mutant_sight;
	self->monsterinfo.search = mutant_search;
	self->monsterinfo.idle = mutant_idle;
	self->monsterinfo.checkattack = mutant_checkattack;

	self->monsterinfo.speed = 25;
	self->monsterinfo.walkspeed = 8;

	gi.linkentity (self);
	
	self->monsterinfo.currentmove = &mutant_move_stand;

	self->monsterinfo.scale = MODEL_SCALE;
	walkmonster_start (self);
}
