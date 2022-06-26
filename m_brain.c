/*
==============================================================================

brain

==============================================================================
*/

#include "g_local.h"
#include "m_brain.h"


static int	sound_chest_open;
static int	sound_tentacles_extend;
static int	sound_tentacles_retract;
static int	sound_death;
static int	sound_idle1;
static int	sound_idle2;
static int	sound_idle3;
static int	sound_pain1;
static int	sound_pain2;
static int	sound_sight;
static int	sound_search;
static int	sound_melee1;
static int	sound_melee2;
static int	sound_melee3;


void brain_sight(edict_t* self, edict_t* other)
{
	gi.sound(self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void brain_search(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}


void brain_run(edict_t* self);
void brain_dead(edict_t* self);


//
// STAND
//

mframe_t brain_frames_stand[] =
{
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,

	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,

	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL
};
mmove_t brain_move_stand = { FRAME_stand01, FRAME_stand30, brain_frames_stand, NULL };

void brain_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &brain_move_stand;
}


//
// IDLE
//

mframe_t brain_frames_idle[] =
{
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,

	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,

	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL
};
mmove_t brain_move_idle = { FRAME_stand31, FRAME_stand60, brain_frames_idle, brain_stand };

void brain_idle(edict_t* self)
{
	gi.sound(self, CHAN_AUTO, sound_idle3, 1, ATTN_IDLE, 0);
	self->monsterinfo.currentmove = &brain_move_idle;
}


//
// WALK
//
mframe_t brain_frames_walk1[] =
{
	MANIM_WALK,	2,	NULL,
	MANIM_WALK,	-3,	NULL,
	MANIM_WALK,	-2,	NULL,
	MANIM_WALK,	-2,	NULL,
	MANIM_WALK,	-4,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	4,	NULL,
	MANIM_WALK,	-9,	NULL,
	MANIM_WALK,	-6,	NULL,
	MANIM_WALK,	-3,	NULL
};
mmove_t brain_move_walk1 = { FRAME_walk101, FRAME_walk111, brain_frames_walk1, NULL };

// walk2 is FUBAR, do not use
#if 0
void brain_walk2_cycle(edict_t* self)
{
	if (random() > 0.1)
		self->monsterinfo.nextframe = FRAME_walk220;
}

mframe_t brain_frames_walk2[] =
{
	MANIM_WALK,	-2,	NULL,
	MANIM_WALK,	-7,	NULL,
	MANIM_WALK,	-9,	NULL,
	MANIM_WALK,	-8,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-4,	NULL,
	MANIM_WALK,	7,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-8,	NULL,
	MANIM_WALK,	-5,	NULL,

	MANIM_WALK,	-7,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-4,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	5,	NULL,		// Cycle Start

	MANIM_WALK,	-6,	NULL,
	MANIM_WALK,	2,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-2,	NULL,
	MANIM_WALK,	-8,	NULL,
	MANIM_WALK,	-3,	NULL,
	MANIM_WALK,	-1,	NULL,
	MANIM_WALK,	-8,	NULL,
	MANIM_WALK,	-3,	NULL,
	MANIM_WALK,	-5,	NULL,

	MANIM_WALK,	-1,	brain_walk2_cycle,
	MANIM_WALK,	-6,	NULL,
	MANIM_WALK,	-6,	NULL,
	MANIM_WALK,	-13,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-4,	NULL,
	MANIM_WALK,	1,	NULL,
	MANIM_WALK,	-3,	NULL,
	MANIM_WALK,	-6,	NULL,
	MANIM_WALK,	-10,	NULL
};
mmove_t brain_move_walk2 = { FRAME_walk201, FRAME_walk240, brain_frames_walk2, NULL };
#endif

void brain_walk(edict_t* self)
{
	//	if (random() <= 0.5)
	self->monsterinfo.currentmove = &brain_move_walk1;
	//	else
	//		self->monsterinfo.currentmove = &brain_move_walk2;
}



mframe_t brain_frames_defense[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t brain_move_defense = { FRAME_defens01, FRAME_defens08, brain_frames_defense, NULL };

mframe_t brain_frames_pain3[] =
{
	MANIM_MISC,	-2,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	-4,	NULL
};
mmove_t brain_move_pain3 = { FRAME_pain301, FRAME_pain306, brain_frames_pain3, brain_run };

mframe_t brain_frames_pain2[] =
{
	MANIM_MISC,	-2,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	-2,	NULL
};
mmove_t brain_move_pain2 = { FRAME_pain201, FRAME_pain208, brain_frames_pain2, brain_run };

mframe_t brain_frames_pain1[] =
{
	MANIM_MISC,	-6,	NULL,
	MANIM_MISC,	-2,	NULL,
	MANIM_MISC,	-6,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	7,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	-1,	NULL
};
mmove_t brain_move_pain1 = { FRAME_pain101, FRAME_pain121, brain_frames_pain1, brain_run };


//
// DUCK
//

mframe_t brain_frames_duck[] =
{
	MANIM_DUCK,	0,	monster_duck_down,
	MANIM_DUCK,	-2,	NULL,
	MANIM_DUCK,	17,	monster_duck_hold,
	MANIM_DUCK,	-3,	NULL,
	MANIM_DUCK,	-1,	monster_duck_up,
	MANIM_DUCK,	-5,	NULL,
	MANIM_DUCK,	-6,	NULL,
	MANIM_DUCK,	-6,	NULL
};
mmove_t brain_move_duck = { FRAME_duck01, FRAME_duck08, brain_frames_duck, brain_run };

void brain_duck(edict_t* self, edict_t* attacker, float eta)
{
	self->monsterinfo.pausetime = level.time + eta + 0.5;
	self->monsterinfo.currentmove = &brain_move_duck;
}


mframe_t brain_frames_death2[] =
{
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	brain_dead,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	9,	NULL,
	MANIM_DEATH,	0,	NULL
};
mmove_t brain_move_death2 = { FRAME_death201, FRAME_death205, brain_frames_death2, monster_dead_dead };

mframe_t brain_frames_death1[] =
{
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	brain_dead,
	MANIM_DEATH,	-2,	NULL,
	MANIM_DEATH,	9,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL
};
mmove_t brain_move_death1 = { FRAME_death101, FRAME_death118, brain_frames_death1, monster_dead_dead };


//
// MELEE
//

void brain_swing_right(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_melee1, 1, ATTN_NORM, 0);
}

void brain_hit_right(edict_t* self)
{
	vec3_t	aim;

	VectorSet(aim, MELEE_DISTANCE, self->maxs[0], 8);
	if (fire_hit(self, aim, (15 + (rand() % 5)), 8))
		gi.sound(self, CHAN_WEAPON, sound_melee3, 1, ATTN_NORM, 0);
}

void brain_swing_left(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_melee2, 1, ATTN_NORM, 0);
}

void brain_hit_left(edict_t* self)
{
	vec3_t	aim;

	VectorSet(aim, MELEE_DISTANCE, self->mins[0], 8);
	if (fire_hit(self, aim, (15 + (rand() % 5)), 8))
		gi.sound(self, CHAN_WEAPON, sound_melee3, 1, ATTN_NORM, 0);
}

mframe_t brain_frames_attack1[] =
{
	MANIM_MELEE,	8,	NULL,
	MANIM_MELEE,	3,	NULL,
	MANIM_MELEE,	5,	NULL,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	-3,	brain_swing_right,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	-5,	NULL,
	MANIM_MELEE,	-7,	brain_hit_right,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	6,	brain_swing_left,
	MANIM_MELEE,	1,	NULL,
	MANIM_MELEE,	2,	brain_hit_left,
	MANIM_MELEE,	-3,	NULL,
	MANIM_MELEE,	6,	NULL,
	MANIM_MELEE,	-1,	NULL,
	MANIM_MELEE,	-3,	NULL,
	MANIM_MELEE,	2,	NULL,
	MANIM_MELEE,	-11,NULL
};
mmove_t brain_move_attack1 = { FRAME_attak101, FRAME_attak118, brain_frames_attack1, brain_run };

void brain_chest_open(edict_t* self)
{
	self->spawnflags &= ~65536;
	self->monsterinfo.power_armor_type = POWER_ARMOR_NONE;
	gi.sound(self, CHAN_BODY, sound_chest_open, 1, ATTN_NORM, 0);
}

void brain_tentacle_attack(edict_t* self)
{
	vec3_t	aim;

	VectorSet(aim, MELEE_DISTANCE, 0, 8);
	if (fire_hit(self, aim, (10 + (rand() % 5)), -120) && skill->value > 0)
		self->spawnflags |= 65536;
	gi.sound(self, CHAN_WEAPON, sound_tentacles_retract, 1, ATTN_NORM, 0);
}

void brain_chest_closed(edict_t* self)
{
	self->monsterinfo.power_armor_type = POWER_ARMOR_SCREEN;
	if (self->spawnflags & 65536)
	{
		self->spawnflags &= ~65536;
		self->monsterinfo.currentmove = &brain_move_attack1;
	}
}

mframe_t brain_frames_attack2[] =
{
	MANIM_MELEE,	5,	NULL,
	MANIM_MELEE,	-4,	NULL,
	MANIM_MELEE,	-4,	NULL,
	MANIM_MELEE,	-3,	NULL,
	MANIM_MELEE,	0,	brain_chest_open,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	13,	brain_tentacle_attack,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	2,	NULL,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	-9,	brain_chest_closed,
	MANIM_MELEE,	0,	NULL,
	MANIM_MELEE,	4,	NULL,
	MANIM_MELEE,	3,	NULL,
	MANIM_MELEE,	2,	NULL,
	MANIM_MELEE,	-3,	NULL,
	MANIM_MELEE,	-6,	NULL
};
mmove_t brain_move_attack2 = { FRAME_attak201, FRAME_attak217, brain_frames_attack2, brain_run };

void brain_melee(edict_t* self)
{
	if (random() <= 0.5)
	{
		self->pain_debounce_time = level.time + 1.9;
		self->monsterinfo.currentmove = &brain_move_attack1;
	}
	else
	{
		self->pain_debounce_time = level.time + 1.8;
		self->monsterinfo.currentmove = &brain_move_attack2;
	}
}


//
// RUN
//

mframe_t brain_frames_run[] =
{
	MANIM_RUN,	4,	NULL,
	MANIM_RUN,	-3,	NULL,
	MANIM_RUN,	-2,	NULL,
	MANIM_RUN,	-2,	NULL,
	MANIM_RUN,	-4,	NULL,
	MANIM_RUN,	-5,	NULL,
	MANIM_RUN,	-5,	NULL,
	MANIM_RUN,	5,	NULL,
	MANIM_RUN,	-9,	NULL,
	MANIM_RUN,	-6,	NULL,
	MANIM_RUN,	-3,	NULL
};
mmove_t brain_move_run = { FRAME_walk101, FRAME_walk111, brain_frames_run, NULL };

void brain_run(edict_t* self)
{
	self->monsterinfo.power_armor_type = POWER_ARMOR_SCREEN;
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &brain_move_stand;
	else
		self->monsterinfo.currentmove = &brain_move_run;
}


void brain_pain(edict_t* self, edict_t* other, float kick, int damage)
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
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &brain_move_pain1;
	}
	else if (r < 0.66)
	{
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &brain_move_pain2;
	}
	else
	{
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &brain_move_pain3;
	}
}

void brain_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity(self);
}



void brain_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	int		n;

	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	self->s.effects = 0;
	self->monsterinfo.power_armor_type = POWER_ARMOR_NONE;

	// check for gib
	if (self->gib_health <= 0)
	{
		gi.sound(self, CHAN_VOICE, gi.soundindex("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n = 0; n < 2; n++)
			ThrowGib(self, "models/objects/gibs/bone/tris.md2", damage, GIB_ORGANIC);
		for (n = 0; n < 4; n++)
			ThrowGib(self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowHead(self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	gi.sound(self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	if (random() <= 0.5)
		self->monsterinfo.currentmove = &brain_move_death1;
	else
		self->monsterinfo.currentmove = &brain_move_death2;
}

/*QUAKED monster_brain (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_brain(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_chest_open = gi.soundindex("brain/brnatck1.wav");
	sound_tentacles_extend = gi.soundindex("brain/brnatck2.wav");
	sound_tentacles_retract = gi.soundindex("brain/brnatck3.wav");
	sound_death = gi.soundindex("brain/brndeth1.wav");
	sound_idle1 = gi.soundindex("brain/brnidle1.wav");
	sound_idle2 = gi.soundindex("brain/brnidle2.wav");
	sound_idle3 = gi.soundindex("brain/brnlens1.wav");
	sound_pain1 = gi.soundindex("brain/brnpain1.wav");
	sound_pain2 = gi.soundindex("brain/brnpain2.wav");
	sound_sight = gi.soundindex("brain/brnsght1.wav");
	sound_search = gi.soundindex("brain/brnsrch1.wav");
	sound_melee1 = gi.soundindex("brain/melee1.wav");
	sound_melee2 = gi.soundindex("brain/melee2.wav");
	sound_melee3 = gi.soundindex("brain/melee3.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/brain/tris.md2");
	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, 32);
	VectorSet(self->headmins, -16, -16, 14);
	VectorSet(self->headmaxs, 16, 16, 26);

	self->health = 300;
	self->gib_health = 450;
	self->mass = 400;

	self->pain = brain_pain;
	self->die = brain_die;

	self->monsterinfo.stand = brain_stand;
	self->monsterinfo.walk = brain_walk;
	self->monsterinfo.run = brain_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.duck = brain_duck;
	self->monsterinfo.unduck = monster_duck_up;
	//	self->monsterinfo.attack = brain_attack;
	self->monsterinfo.melee = brain_melee;
	self->monsterinfo.sight = brain_sight;
	self->monsterinfo.search = brain_search;
	self->monsterinfo.idle = brain_idle;

	self->monsterinfo.power_armor_type = POWER_ARMOR_SCREEN;
	self->monsterinfo.power_armor_power = 100;

	self->monsterinfo.speed = 5;
	self->monsterinfo.walkspeed = 5;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &brain_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
