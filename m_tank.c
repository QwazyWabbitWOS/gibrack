/*
==============================================================================

TANK

==============================================================================
*/

#include "g_local.h"
#include "m_tank.h"


void tank_refire_rocket(edict_t* self);
void tank_doattack_rocket(edict_t* self);
void tank_reattack_blaster(edict_t* self);

static int	sound_thud;
static int	sound_pain;
static int	sound_idle;
static int	sound_die;
static int	sound_step;
static int	sound_sight;
static int	sound_windup;
static int	sound_strike;

//
// misc
//

void tank_sight(edict_t* self, edict_t* other)
{
	gi.sound(self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}


void tank_footstep(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_step, 1, ATTN_NORM, 0);
}

void tank_thud(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_thud, 1, ATTN_NORM, 0);
}

void tank_windup(edict_t* self)
{
	gi.sound(self, CHAN_WEAPON, sound_windup, 1, ATTN_NORM, 0);
}

void tank_idle(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}


//
// stand
//

mframe_t tank_frames_stand[] =
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
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL
};
mmove_t	tank_move_stand = { FRAME_stand01, FRAME_stand30, tank_frames_stand, NULL };

void tank_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &tank_move_stand;
}


//
// walk
//

void tank_walk(edict_t* self);

mframe_t tank_frames_start_walk[] =
{
	MANIM_WALK,  -5, NULL,
	MANIM_WALK,  1, NULL,
	MANIM_WALK,  1, NULL,
	MANIM_WALK, 6, tank_footstep
};
mmove_t	tank_move_start_walk = { FRAME_walk01, FRAME_walk04, tank_frames_start_walk, tank_walk };

mframe_t tank_frames_walk[] =
{
	MANIM_WALK, -1,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, -2,	NULL,
	MANIM_WALK, -3,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, -1,	NULL,
	MANIM_WALK, -1,	tank_footstep,
	MANIM_WALK, -2,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, -1,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 2,	NULL,
	MANIM_WALK, 2,	NULL,
	MANIM_WALK, 1,	NULL,
	MANIM_WALK, 1,	tank_footstep
};
mmove_t	tank_move_walk = { FRAME_walk05, FRAME_walk20, tank_frames_walk, NULL };

mframe_t tank_frames_stop_walk[] =
{
	MANIM_WALK,  -2, NULL,
	MANIM_WALK,  -2, NULL,
	MANIM_WALK,  -3, NULL,
	MANIM_WALK,  -3, NULL,
	MANIM_WALK,  -1, tank_footstep
};
mmove_t	tank_move_stop_walk = { FRAME_walk21, FRAME_walk25, tank_frames_stop_walk, tank_stand };

void tank_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &tank_move_walk;
}


//
// run
//

void tank_run(edict_t* self);

mframe_t tank_frames_start_run[] =
{
	MANIM_RUN,  -5, NULL,
	MANIM_RUN,  1, NULL,
	MANIM_RUN,  1, NULL,
	MANIM_RUN, 6, tank_footstep
};
mmove_t	tank_move_start_run = { FRAME_walk01, FRAME_walk04, tank_frames_start_run, tank_run };

mframe_t tank_frames_run[] =
{
	MANIM_RUN, -1,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, -2,	NULL,
	MANIM_RUN, -3,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, -1,	NULL,
	MANIM_RUN, -1,	tank_footstep,
	MANIM_RUN, -2,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, -1,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 2,	NULL,
	MANIM_RUN, 2,	NULL,
	MANIM_RUN, 1,	NULL,
	MANIM_RUN, 1,	tank_footstep
};
mmove_t	tank_move_run = { FRAME_walk05, FRAME_walk20, tank_frames_run, NULL };

mframe_t tank_frames_stop_run[] =
{
	MANIM_RUN,  -2, NULL,
	MANIM_RUN,  -2, NULL,
	MANIM_RUN,  -3, NULL,
	MANIM_RUN,  -3, NULL,
	MANIM_RUN,  -1, tank_footstep
};
mmove_t	tank_move_stop_run = { FRAME_walk21, FRAME_walk25, tank_frames_stop_run, tank_walk };

void tank_run(edict_t* self)
{
	if (self->enemy && self->enemy->client)
		self->monsterinfo.aiflags |= AI_BRUTAL;
	else
		self->monsterinfo.aiflags &= ~AI_BRUTAL;

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		self->monsterinfo.currentmove = &tank_move_stand;
		return;
	}

	if (self->monsterinfo.currentmove == &tank_move_walk ||
		self->monsterinfo.currentmove == &tank_move_start_run)
	{
		self->monsterinfo.currentmove = &tank_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &tank_move_start_run;
	}
}

//
// pain
//

mframe_t tank_frames_pain1[] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t tank_move_pain1 = { FRAME_pain101, FRAME_pain104, tank_frames_pain1, tank_run };

mframe_t tank_frames_pain2[] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t tank_move_pain2 = { FRAME_pain201, FRAME_pain205, tank_frames_pain2, tank_run };

mframe_t tank_frames_pain3[] =
{
	MANIM_MISC, -7, NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 2,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 3,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 2,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  tank_footstep
};
mmove_t	tank_move_pain3 = { FRAME_pain301, FRAME_pain316, tank_frames_pain3, tank_run };


void tank_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum |= 1;

	if (damage <= 10)
		return;

	if (level.time < self->pain_debounce_time)
		return;

	if (damage <= 30)
		if (random() > 0.2)
			return;

	// If hard or nightmare, don't go into pain while attacking
	if (skill->value >= 2)
	{
		if ((self->s.frame >= FRAME_attak301) && (self->s.frame <= FRAME_attak330))
			return;
		if ((self->s.frame >= FRAME_attak101) && (self->s.frame <= FRAME_attak116))
			return;
	}

	self->pain_debounce_time = level.time + PAINTIME;
	gi.sound(self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 30)
		self->monsterinfo.currentmove = &tank_move_pain1;
	else if (damage <= 60)
		self->monsterinfo.currentmove = &tank_move_pain2;
	else
		self->monsterinfo.currentmove = &tank_move_pain3;
};


//
// attacks
//

void TankBlaster(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;
	int		flash_number;

	if (self->s.frame == FRAME_attak110)
		flash_number = MZ2_TANK_BLASTER_1;
	else if (self->s.frame == FRAME_attak113)
		flash_number = MZ2_TANK_BLASTER_2;
	else // (self->s.frame == FRAME_attak116)
		flash_number = MZ2_TANK_BLASTER_3;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);

	monster_fire_blaster(self, start, dir, 30, 800, flash_number, EF_BLASTER, false);
}

void TankStrike(edict_t* self)
{
	gi.sound(self, CHAN_WEAPON, sound_strike, 1, ATTN_NORM, 0);
}

void TankRocket(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;
	int		flash_number;

	if (self->s.frame == FRAME_attak324)
		flash_number = MZ2_TANK_ROCKET_1;
	else if (self->s.frame == FRAME_attak327)
		flash_number = MZ2_TANK_ROCKET_2;
	else // (self->s.frame == FRAME_attak330)
		flash_number = MZ2_TANK_ROCKET_3;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);

	monster_fire_rocket(self, start, dir, 100, 650, flash_number);
}

void TankMachineGun(edict_t* self)
{
	vec3_t	dir;
	vec3_t	start;
	vec3_t	forward, right;
	int		flash_number;

	flash_number = MZ2_TANK_MACHINEGUN_1 + (self->s.frame - FRAME_attak406);

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	VectorCopy(self->aimangles, dir);
	if (self->s.frame <= FRAME_attak415)
		dir[1] = self->s.angles[1] - 8 * (self->s.frame - FRAME_attak411);
	else
		dir[1] = self->s.angles[1] + 8 * (self->s.frame - FRAME_attak419);

	AngleVectors(dir, forward, NULL, NULL);

	monster_fire_bullet(self, start, forward, 20, 40, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_number, false);
}


mframe_t tank_frames_attack_blast[] =
{
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, -1,	NULL,
	MANIM_MISSILE, -2,	NULL,
	MANIM_MISSILE, -1,	NULL,
	MANIM_MISSILE, -1,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	TankBlaster,		// 10
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	TankBlaster,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	TankBlaster			// 16
};
mmove_t tank_move_attack_blast = { FRAME_attak101, FRAME_attak116, tank_frames_attack_blast, tank_reattack_blaster };

mframe_t tank_frames_reattack_blast[] =
{
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	TankBlaster,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	TankBlaster			// 16
};
mmove_t tank_move_reattack_blast = { FRAME_attak111, FRAME_attak116, tank_frames_reattack_blast, tank_reattack_blaster };

mframe_t tank_frames_attack_post_blast[] =
{
	MANIM_MISC, 0,		NULL,				// 17
	MANIM_MISC, 0,		NULL,
	MANIM_MISC, 2,		NULL,
	MANIM_MISC, 3,		NULL,
	MANIM_MISC, 2,		NULL,
	MANIM_MISC, -2,	tank_footstep		// 22
};
mmove_t tank_move_attack_post_blast = { FRAME_attak117, FRAME_attak122, tank_frames_attack_post_blast, tank_run };

void tank_reattack_blaster(edict_t* self)
{
	if (skill->value >= 2)
		if (visible(self, self->enemy))
			if (self->enemy->health > 0)
				if (random() <= 0.6)
				{
					self->pain_debounce_time = level.time + 0.7;
					self->monsterinfo.currentmove = &tank_move_reattack_blast;
					return;
				}
	self->pain_debounce_time = level.time + 0.7;
	self->monsterinfo.currentmove = &tank_move_attack_post_blast;
}


void tank_poststrike(edict_t* self)
{
	self->enemy = NULL;
	tank_run(self);
}

mframe_t tank_frames_attack_strike[] =
{
	MANIM_MISC, 3,   NULL,
	MANIM_MISC, 2,   NULL,
	MANIM_MISC, 2,   NULL,
	MANIM_MISC, 1,   NULL,
	MANIM_MISC, 6,   NULL,
	MANIM_MISC, 7,   NULL,
	MANIM_MISC, 9,   tank_footstep,
	MANIM_MISC, 2,   NULL,
	MANIM_MISC, 1,   NULL,
	MANIM_MISC, 2,   NULL,
	MANIM_MISC, 2,   tank_footstep,
	MANIM_MISC, 2,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, -2,  NULL,
	MANIM_MISC, -2,  NULL,
	MANIM_MISC, 0,   tank_windup,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, 0,   TankStrike,
	MANIM_MISC, 0,   NULL,
	MANIM_MISC, -1,  NULL,
	MANIM_MISC, -1,  NULL,
	MANIM_MISC, -1,  NULL,
	MANIM_MISC, -1,  NULL,
	MANIM_MISC, -1,  NULL,
	MANIM_MISC, -3,  NULL,
	MANIM_MISC, -10, NULL,
	MANIM_MISC, -10, NULL,
	MANIM_MISC, -2,  NULL,
	MANIM_MISC, -3,  NULL,
	MANIM_MISC, -2,  tank_footstep
};
mmove_t tank_move_attack_strike = { FRAME_attak201, FRAME_attak238, tank_frames_attack_strike, tank_poststrike };

mframe_t tank_frames_attack_pre_rocket[] =
{
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,			// 10

	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 1,  NULL,
	MANIM_MISSILE, 2,  NULL,
	MANIM_MISSILE, 7,  NULL,
	MANIM_MISSILE, 7,  NULL,
	MANIM_MISSILE, 7,  tank_footstep,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,			// 20

	MANIM_MISSILE, -3, NULL
};
mmove_t tank_move_attack_pre_rocket = { FRAME_attak301, FRAME_attak321, tank_frames_attack_pre_rocket, tank_doattack_rocket };

mframe_t tank_frames_attack_fire_rocket[] =
{
	MANIM_MISSILE, -3, NULL,			// Loop Start	22 
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  TankRocket,		// 24
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  TankRocket,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, -1, TankRocket		// 30	Loop End
};
mmove_t tank_move_attack_fire_rocket = { FRAME_attak322, FRAME_attak330, tank_frames_attack_fire_rocket, tank_refire_rocket };

mframe_t tank_frames_attack_post_rocket[] =
{
	MANIM_MISSILE, 0,  NULL,			// 31
	MANIM_MISSILE, -1, NULL,
	MANIM_MISSILE, -1, NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 2,  NULL,
	MANIM_MISSILE, 3,  NULL,
	MANIM_MISSILE, 4,  NULL,
	MANIM_MISSILE, 2,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,			// 40

	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, -9, NULL,
	MANIM_MISSILE, -8, NULL,
	MANIM_MISSILE, -7, NULL,
	MANIM_MISSILE, -1, NULL,
	MANIM_MISSILE, -1, tank_footstep,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,			// 50

	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL,
	MANIM_MISSILE, 0,  NULL
};
mmove_t tank_move_attack_post_rocket = { FRAME_attak331, FRAME_attak353, tank_frames_attack_post_rocket, tank_run };

mframe_t tank_frames_attack_chain[] =
{
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE,      0, TankMachineGun,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL
};
mmove_t tank_move_attack_chain = { FRAME_attak401, FRAME_attak429, tank_frames_attack_chain, tank_run };

void tank_refire_rocket(edict_t* self)
{
	// Only on hard or nightmare
	if (skill->value >= 2)
		if (self->enemy->health > 0)
			if (visible(self, self->enemy))
				if (random() <= 0.4)
				{
					self->pain_debounce_time = level.time + 1;
					self->monsterinfo.currentmove = &tank_move_attack_fire_rocket;
					return;
				}
	self->pain_debounce_time = level.time + 2.4;
	self->monsterinfo.currentmove = &tank_move_attack_post_rocket;
}

void tank_doattack_rocket(edict_t* self)
{
	self->pain_debounce_time = level.time + 1;
	self->monsterinfo.currentmove = &tank_move_attack_fire_rocket;
}

void tank_attack(edict_t* self)
{
	vec3_t	vec;
	float	range;
	float	r;

	if (self->enemy->health < 0)
	{
		self->pain_debounce_time = level.time + 3.9;
		self->monsterinfo.currentmove = &tank_move_attack_strike;
		self->monsterinfo.aiflags &= ~AI_BRUTAL;
		return;
	}

	VectorSubtract(self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength(vec);

	r = random();

	if (range <= 125)
	{
		if (r < 0.4)
		{
			self->pain_debounce_time = level.time + 3;
			self->monsterinfo.currentmove = &tank_move_attack_chain;
		}
		else
		{
			self->pain_debounce_time = level.time + 1.7;
			self->monsterinfo.currentmove = &tank_move_attack_blast;
		}
	}
	else if (range <= 250)
	{
		if (r < 0.5)
		{
			self->pain_debounce_time = level.time + 3;
			self->monsterinfo.currentmove = &tank_move_attack_chain;
		}
		else
		{
			self->pain_debounce_time = level.time + 1.7;
			self->monsterinfo.currentmove = &tank_move_attack_blast;
		}
	}
	else
	{
		if (r < 0.33)
		{
			self->pain_debounce_time = level.time + 1;
			self->monsterinfo.currentmove = &tank_move_attack_chain;
		}
		else if (r < 0.66)
		{
			self->pain_debounce_time = level.time + 2.2;
			self->monsterinfo.currentmove = &tank_move_attack_pre_rocket;
		}
		else
		{
			self->pain_debounce_time = level.time + 1.7;
			self->monsterinfo.currentmove = &tank_move_attack_blast;
		}
	}
}


//
// death
//

void tank_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	tank_thud(self);

	VectorSet(self->mins, -16, -16, -16);
	VectorSet(self->maxs, 16, 16, -0);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity(self);
}

mframe_t tank_frames_death1[] =
{
	MANIM_DEATH, -7,  NULL,
	MANIM_DEATH, -2,  NULL,
	MANIM_DEATH, -2,  NULL,
	MANIM_DEATH, 1,   NULL,
	MANIM_DEATH, 3,   NULL,
	MANIM_DEATH, 6,   NULL,
	MANIM_DEATH, 1,   NULL,
	MANIM_DEATH, 1,   NULL,
	MANIM_DEATH, 2,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, -2,  NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, -3,  NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, -4,  NULL,
	MANIM_DEATH, -6,  NULL,
	MANIM_DEATH, -4,  NULL,
	MANIM_DEATH, -5,  NULL,
	MANIM_DEATH, -7,  NULL,
	MANIM_DEATH, -15, tank_dead,
	MANIM_DEATH, -5,  NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL,
	MANIM_DEATH, 0,   NULL
};
mmove_t	tank_move_death = { FRAME_death101, FRAME_death132, tank_frames_death1, monster_dead_dead };

void tank_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	int		n;

	// check for gib
	if (self->gib_health <= 0)
	{
		gi.sound(self, CHAN_VOICE, gi.soundindex("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n = 0; n < 1 /*4*/; n++)
			ThrowGib(self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		for (n = 0; n < 4; n++)
			ThrowGib(self, "models/objects/gibs/sm_metal/tris.md2", damage, GIB_METALLIC);
		ThrowGib(self, "models/objects/gibs/chest/tris.md2", damage, GIB_ORGANIC);
		ThrowHead(self, "models/objects/gibs/gear/tris.md2", damage, GIB_METALLIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum |= 1;

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	gi.sound(self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	self->monsterinfo.currentmove = &tank_move_death;

}


//
// monster_tank
//

/*QUAKED monster_tank (1 .5 0) (-32 -32 -16) (32 32 72) Ambush Trigger_Spawn Sight
*/
/*QUAKED monster_tank_commander (1 .5 0) (-32 -32 -16) (32 32 72) Ambush Trigger_Spawn Sight
*/
void SP_monster_tank(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	self->s.modelindex = gi.modelindex("models/monsters/tank/tris.md2");
	VectorSet(self->mins, -32, -32, -16);
	VectorSet(self->maxs, 32, 32, 72);
	VectorSet(self->headmins, -32, -32, 52);
	VectorSet(self->headmaxs, 32, 32, 72);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	sound_pain = gi.soundindex("tank/tnkpain2.wav");
	sound_thud = gi.soundindex("tank/tnkdeth2.wav");
	sound_idle = gi.soundindex("tank/tnkidle1.wav");
	sound_die = gi.soundindex("tank/death.wav");
	sound_step = gi.soundindex("tank/step.wav");
	sound_windup = gi.soundindex("tank/tnkatck4.wav");
	sound_strike = gi.soundindex("tank/tnkatck5.wav");
	sound_sight = gi.soundindex("tank/sight1.wav");

	gi.soundindex("tank/tnkatck1.wav");
	gi.soundindex("tank/tnkatk2a.wav");
	gi.soundindex("tank/tnkatk2b.wav");
	gi.soundindex("tank/tnkatk2c.wav");
	gi.soundindex("tank/tnkatk2d.wav");
	gi.soundindex("tank/tnkatk2e.wav");
	gi.soundindex("tank/tnkatck3.wav");

	if (strcmp(self->classname, "monster_tank_commander") == 0)
	{
		self->health = 1000;
		self->gib_health = 1225;
	}
	else
	{
		self->health = 750;
		self->gib_health = 950;
	}

	self->mass = 500;

	self->pain = tank_pain;
	self->die = tank_die;
	self->monsterinfo.stand = tank_stand;
	self->monsterinfo.walk = tank_walk;
	self->monsterinfo.run = tank_run;
	self->monsterinfo.attack = tank_attack;
	self->monsterinfo.sight = tank_sight;
	self->monsterinfo.idle = tank_idle;

	self->monsterinfo.speed = 5;
	self->monsterinfo.walkspeed = 5;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &tank_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);

	if (strcmp(self->classname, "monster_tank_commander") == 0)
		self->s.skinnum = 2;
}
