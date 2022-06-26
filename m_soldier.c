/*
==============================================================================

SOLDIER

==============================================================================
*/

#include "g_local.h"
#include "m_soldier.h"


static int	sound_idle;
static int	sound_sight1;
static int	sound_sight2;
static int	sound_pain_light;
static int	sound_pain;
static int	sound_pain_ss;
static int	sound_death_light;
static int	sound_death;
static int	sound_death_ss;
static int	sound_cock;


void soldier_idle(edict_t* self)
{
	if (random() > 0.8)
		gi.sound(self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

void soldier_cock(edict_t* self)
{
	if (self->s.frame == FRAME_stand322)
		gi.sound(self, CHAN_WEAPON, sound_cock, 1, ATTN_IDLE, 0);
	else
		gi.sound(self, CHAN_WEAPON, sound_cock, 1, ATTN_NORM, 0);
}


// STAND

void soldier_stand(edict_t* self);

mframe_t soldier_frames_stand1[] =
{
	{MANIM_STAND, 0, soldier_idle},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},

	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},

	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL}
};
mmove_t soldier_move_stand1 = { FRAME_stand101, FRAME_stand130, soldier_frames_stand1, soldier_stand };

mframe_t soldier_frames_stand3[] =
{
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},

	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},

	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, soldier_cock},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},

	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL}
};
mmove_t soldier_move_stand3 = { FRAME_stand301, FRAME_stand339, soldier_frames_stand3, soldier_stand };

#if 0
mframe_t soldier_frames_stand4[] =
{
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},

	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},

	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},

	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},

	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 0, NULL},
	ai_stand, 4, NULL},
	ai_stand, 1, NULL},
	ai_stand, -1, NULL},
	ai_stand, -2, NULL},

	ai_stand, 0, NULL},
	ai_stand, 0, NULL}
};
mmove_t soldier_move_stand4 = { FRAME_stand401, FRAME_stand452, soldier_frames_stand4, NULL };
#endif

void soldier_stand(edict_t* self)
{
	if ((self->monsterinfo.currentmove == &soldier_move_stand3) || (random() < 0.8))
		self->monsterinfo.currentmove = &soldier_move_stand1;
	else
		self->monsterinfo.currentmove = &soldier_move_stand3;
}


//
// WALK
//

void soldier_walk1_random(edict_t* self)
{
	if (random() > 0.1)
		self->monsterinfo.nextframe = FRAME_walk101;
}

mframe_t soldier_frames_walk1[] =
{
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0, soldier_walk1_random},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL}
};
mmove_t soldier_move_walk1 = { FRAME_walk101, FRAME_walk133, soldier_frames_walk1, NULL };

mframe_t soldier_frames_walk2[] =
{
	{MANIM_WALK, -1,  NULL},
	{MANIM_WALK, -1,  NULL},
	{MANIM_WALK, 4,  NULL},
	{MANIM_WALK, 3,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, -4,  NULL},
	{MANIM_WALK, -2,  NULL},
	{MANIM_WALK, 2,  NULL},
	{MANIM_WALK, 1,  NULL},
	{MANIM_WALK, 2,  NULL}
};
mmove_t soldier_move_walk2 = { FRAME_walk209, FRAME_walk218, soldier_frames_walk2, NULL };

void soldier_walk(edict_t* self)
{
	if (random() < 0.5)
		self->monsterinfo.currentmove = &soldier_move_walk1;
	else
		self->monsterinfo.currentmove = &soldier_move_walk2;
}


//
// RUN
//

void soldier_run(edict_t* self);

mframe_t soldier_frames_start_run[] =
{
	{MANIM_RUN, 0,  NULL},
	{MANIM_RUN, 0,  NULL}
};
mmove_t soldier_move_start_run = { FRAME_run01, FRAME_run02, soldier_frames_start_run, soldier_run };

mframe_t soldier_frames_run[] =
{
	{MANIM_RUN, -2, NULL},
	{MANIM_RUN, -1, NULL},
	{MANIM_RUN, -1, NULL},
	{MANIM_RUN, 4, NULL},
	{MANIM_RUN, -2, NULL},
	{MANIM_RUN, 3, NULL}
};
mmove_t soldier_move_run = { FRAME_run03, FRAME_run08, soldier_frames_run, NULL };

void soldier_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		self->monsterinfo.currentmove = &soldier_move_stand1;
		return;
	}

	if (self->monsterinfo.currentmove == &soldier_move_walk1 ||
		self->monsterinfo.currentmove == &soldier_move_walk2 ||
		self->monsterinfo.currentmove == &soldier_move_start_run)
	{
		self->monsterinfo.currentmove = &soldier_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &soldier_move_start_run;
	}
}


//
// PAIN
//

mframe_t soldier_frames_pain1[] =
{
	{MANIM_MISC, -3, NULL},
	{MANIM_MISC, 4,  NULL},
	{MANIM_MISC, 1,  NULL},
	{MANIM_MISC, 1,  NULL},
	{MANIM_MISC, 0,  NULL}
};
mmove_t soldier_move_pain1 = { FRAME_pain101, FRAME_pain105, soldier_frames_pain1, soldier_run };

mframe_t soldier_frames_pain2[] =
{
	{MANIM_MISC, -13, NULL},
	{MANIM_MISC, -1,  NULL},
	{MANIM_MISC, 2,   NULL},
	{MANIM_MISC, 4,   NULL},
	{MANIM_MISC, 2,   NULL},
	{MANIM_MISC, 3,   NULL},
	{MANIM_MISC, 2,   NULL}
};
mmove_t soldier_move_pain2 = { FRAME_pain201, FRAME_pain207, soldier_frames_pain2, soldier_run };

mframe_t soldier_frames_pain3[] =
{
	{MANIM_MISC, -8, NULL},
	{MANIM_MISC, 10, NULL},
	{MANIM_MISC, -4, NULL},
	{MANIM_MISC, -1, NULL},
	{MANIM_MISC, -3, NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 3,  NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 1,  NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 1,  NULL},
	{MANIM_MISC, 2,  NULL},
	{MANIM_MISC, 4,  NULL},
	{MANIM_MISC, 3,  NULL},
	{MANIM_MISC, 2,  NULL}
};
mmove_t soldier_move_pain3 = { FRAME_pain301, FRAME_pain318, soldier_frames_pain3, soldier_run };

mframe_t soldier_frames_pain4[] =
{
	{MANIM_MISC, 0,   NULL},
	{MANIM_MISC, 0,   NULL},
	{MANIM_MISC, 0,   NULL},
	{MANIM_MISC, -10, NULL},
	{MANIM_MISC, -6,  NULL},
	{MANIM_MISC, 8,   NULL},
	{MANIM_MISC, 4,   NULL},
	{MANIM_MISC, 1,   NULL},
	{MANIM_MISC, 0,   NULL},
	{MANIM_MISC, 2,   NULL},
	{MANIM_MISC, 5,   NULL},
	{MANIM_MISC, 2,   NULL},
	{MANIM_MISC, -1,  NULL},
	{MANIM_MISC, -1,  NULL},
	{MANIM_MISC, 3,   NULL},
	{MANIM_MISC, 2,   NULL},
	{MANIM_MISC, 0,   NULL}
};
mmove_t soldier_move_pain4 = { FRAME_pain401, FRAME_pain417, soldier_frames_pain4, soldier_run };


void soldier_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	float	r;
	int		n;

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum |= 1;

	if (level.time < self->pain_debounce_time)
	{
		if ((self->velocity[2] > 100) && ((self->monsterinfo.currentmove == &soldier_move_pain1) || (self->monsterinfo.currentmove == &soldier_move_pain2) || (self->monsterinfo.currentmove == &soldier_move_pain3)))
			self->monsterinfo.currentmove = &soldier_move_pain4;
		return;
	}

	self->pain_debounce_time = level.time + PAINTIME;

	n = self->s.skinnum | 1;
	if (n == 1)
		gi.sound(self, CHAN_VOICE, sound_pain_light, 1, ATTN_NORM, 0);
	else if (n == 3)
		gi.sound(self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_pain_ss, 1, ATTN_NORM, 0);

	if (self->velocity[2] > 100)
	{
		monster_duck_up(self);
		self->monsterinfo.currentmove = &soldier_move_pain4;
		return;
	}

	if (self->monsterinfo.currentmove->frame[self->s.frame - self->monsterinfo.currentmove->firstframe].animtype > MANIM_MISC)
		return;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	monster_duck_up(self);

	r = random();

	if (r < 0.33)
		self->monsterinfo.currentmove = &soldier_move_pain1;
	else if (r < 0.66)
		self->monsterinfo.currentmove = &soldier_move_pain2;
	else
		self->monsterinfo.currentmove = &soldier_move_pain3;
}


//
// ATTACK
//

static int blaster_flash[] = { MZ2_SOLDIER_BLASTER_1, MZ2_SOLDIER_BLASTER_2, MZ2_SOLDIER_BLASTER_3, MZ2_SOLDIER_BLASTER_4, MZ2_SOLDIER_BLASTER_5, MZ2_SOLDIER_BLASTER_6, MZ2_SOLDIER_BLASTER_7, MZ2_SOLDIER_BLASTER_8 };
static int shotgun_flash[] = { MZ2_SOLDIER_SHOTGUN_1, MZ2_SOLDIER_SHOTGUN_2, MZ2_SOLDIER_SHOTGUN_3, MZ2_SOLDIER_SHOTGUN_4, MZ2_SOLDIER_SHOTGUN_5, MZ2_SOLDIER_SHOTGUN_6, MZ2_SOLDIER_SHOTGUN_7, MZ2_SOLDIER_SHOTGUN_8 };
static int machinegun_flash[] = { MZ2_SOLDIER_MACHINEGUN_1, MZ2_SOLDIER_MACHINEGUN_2, MZ2_SOLDIER_MACHINEGUN_3, MZ2_SOLDIER_MACHINEGUN_4, MZ2_SOLDIER_MACHINEGUN_5, MZ2_SOLDIER_MACHINEGUN_6, MZ2_SOLDIER_MACHINEGUN_7, MZ2_SOLDIER_MACHINEGUN_8 };

void soldier_fire(edict_t* self, int flash_number)
{
	vec3_t	start;
	vec3_t	forward, right, up;
	vec3_t	aim;
	vec3_t	dir;
	vec3_t	end;
	float	r, u;
	int		flash_index;

	if (self->s.skinnum < 2)
		flash_index = blaster_flash[flash_number];
	else if (self->s.skinnum < 4)
		flash_index = shotgun_flash[flash_number];
	else
		flash_index = machinegun_flash[flash_number];

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_index], forward, right, start);

	if (flash_number == 5 || flash_number == 6)
	{
		VectorCopy(forward, aim);
	}
	else
	{
		AngleVectors(self->aimangles, aim, NULL, NULL);
		vectoangles(aim, dir);
		AngleVectors(dir, forward, right, up);

		r = crandom() * 500;
		u = crandom() * 500;
		VectorMA(start, 8192, forward, end);
		VectorMA(end, r, right, end);
		VectorMA(end, u, up, end);

		VectorSubtract(end, start, aim);
		VectorNormalize(aim);
	}

	if (self->s.skinnum <= 1)
	{
		monster_fire_blaster(self, start, aim, 30, 600, flash_index, EF_BLASTER, false);
	}
	else if (self->s.skinnum <= 3)
	{
		monster_fire_shotgun(self, start, aim, 4, 8, DEFAULT_SHOTGUN_HSPREAD, DEFAULT_SHOTGUN_VSPREAD, DEFAULT_SHOTGUN_COUNT, flash_index);
	}
	else
	{
		if (!(self->monsterinfo.aiflags & AI_HOLD_FRAME))
			self->monsterinfo.pausetime = level.time + (3.0f + rand() % 8) * FRAMETIME;

		monster_fire_bullet(self, start, aim, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_index, false);

		if (level.time >= self->monsterinfo.pausetime)
			self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
		else
			self->monsterinfo.aiflags |= AI_HOLD_FRAME;
	}
}

// ATTACK1 (blaster/shotgun)

void soldier_fire1(edict_t* self)
{
	soldier_fire(self, 0);
}

void soldier_attack1_refire1(edict_t* self)
{
	if (self->s.skinnum > 1)
		return;

	if (!self->enemy || self->enemy->health <= 0)
		return;

	if (((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE))
	{
		self->pain_debounce_time = level.time + 1.2;
		self->monsterinfo.nextframe = FRAME_attak102;
	}
	else
	{
		self->pain_debounce_time = level.time + 0.4;
		self->monsterinfo.nextframe = FRAME_attak110;
	}
}

void soldier_attack1_refire2(edict_t* self)
{
	if (self->s.skinnum < 2)
		return;

	if (!self->enemy || self->enemy->health <= 0)
		return;

	if (((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE))
	{
		self->pain_debounce_time = level.time + 1.2;
		self->monsterinfo.nextframe = FRAME_attak102;
	}
}

mframe_t soldier_frames_attack1[] =
{
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  soldier_fire1},
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  soldier_attack1_refire1},
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  soldier_cock},
	{MANIM_MISSILE, 0,  soldier_attack1_refire2},
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  NULL},
	{MANIM_MISSILE, 0,  NULL}
};
mmove_t soldier_move_attack1 = { FRAME_attak101, FRAME_attak112, soldier_frames_attack1, soldier_run };

// ATTACK2 (blaster/shotgun)

void soldier_fire2(edict_t* self)
{
	soldier_fire(self, 1);
}

void soldier_attack2_refire1(edict_t* self)
{
	if (self->s.skinnum > 1)
		return;

	if (!self->enemy || self->enemy->health <= 0)
		return;

	if (((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE))
	{
		self->pain_debounce_time = level.time + 1.6;
		self->monsterinfo.nextframe = FRAME_attak204;
	}
	else
	{
		self->pain_debounce_time = level.time + 0.4;
		self->monsterinfo.nextframe = FRAME_attak216;
	}
}

void soldier_attack2_refire2(edict_t* self)
{
	if (self->s.skinnum < 2)
		return;

	if (!self->enemy || self->enemy->health <= 0)
		return;

	if (((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE))
	{
		self->pain_debounce_time = level.time + 1.6;
		self->monsterinfo.nextframe = FRAME_attak204;
	}
}

mframe_t soldier_frames_attack2[] =
{
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_fire2},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_attack2_refire1},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_cock},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_attack2_refire2},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL}
};
mmove_t soldier_move_attack2 = { FRAME_attak201, FRAME_attak218, soldier_frames_attack2, soldier_run };

// ATTACK3 (duck and shoot)

void soldier_fire3(edict_t* self)
{
	soldier_fire(self, 2);
}

mmove_t soldier_move_duck;

void soldier_attack3_refire(edict_t* self)
{
	if (level.time < self->monsterinfo.pausetime)
	{
		self->monsterinfo.nextframe = FRAME_attak303;
		// SLUGFILLER--if enemy not targeted stop firing
		if (!ai_checkenemy(self) || random() < 0.5)
		{
			self->monsterinfo.currentmove = &soldier_move_duck;
			self->monsterinfo.nextframe = FRAME_duck02;
		}
	}
}

mframe_t soldier_frames_attack3[] =
{
	{MANIM_MISSILE, 0, monster_duck_down},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_fire3},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_attack3_refire},
	{MANIM_MISSILE, 0, monster_duck_up},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL}
};
mmove_t soldier_move_attack3 = { FRAME_attak301, FRAME_attak309, soldier_frames_attack3, soldier_run };

// ATTACK4 (machinegun)

void soldier_fire4(edict_t* self)
{
	soldier_fire(self, 3);
	//
	//	if (!self->enemy || self->enemy->health <= 0)
	//		return;
	//
	//	if ( ((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE) )
	//		self->monsterinfo.nextframe = FRAME_attak402;
}

mframe_t soldier_frames_attack4[] =
{
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_fire4},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL}
};
mmove_t soldier_move_attack4 = { FRAME_attak401, FRAME_attak406, soldier_frames_attack4, soldier_run };

#if 0
// ATTACK5 (prone)

void soldier_fire5(edict_t* self)
{
	soldier_fire(self, 4);
}

void soldier_attack5_refire(edict_t* self)
{
	if (!self->enemy || self->enemy->health <= 0)
		return;

	if (((skill->value == 3) && (random() < 0.5)) || (range(self, self->enemy) == RANGE_MELEE))
		self->monsterinfo.nextframe = FRAME_attak505;
}

mframe_t soldier_frames_attack5[] =
{
	{MANIM_MISSILE, 8, NULL},
	{MANIM_MISSILE, 8, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_fire5,
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, NULL},
	{MANIM_MISSILE, 0, soldier_attack5_refire
};
mmove_t soldier_move_attack5 = { FRAME_attak501, FRAME_attak508, soldier_frames_attack5, soldier_run };
#endif

// ATTACK6 (run & shoot)

void soldier_fire8(edict_t* self)
{
	soldier_fire(self, 7);
}

void soldier_attack6_refire(edict_t* self)
{
	if (!self->enemy || self->enemy->health <= 0)
		return;

	if (range(self, self->enemy) < RANGE_MID)
		return;

	if (skill->value == 3)
	{
		self->monsterinfo.nextframe = FRAME_runs03;
	}
}

mframe_t soldier_frames_attack6[] =
{
	{MANIM_MISSILE, 10, NULL},
	{MANIM_MISSILE,  4, NULL},
	{MANIM_MISSILE, 12, soldier_fire8},
	{MANIM_MISSILE, 11, NULL},
	{MANIM_MISSILE, 13, NULL},
	{MANIM_MISSILE, 18, NULL},
	{MANIM_MISSILE, 15, NULL},
	{MANIM_MISSILE, 14, NULL},
	{MANIM_MISSILE, 11, NULL},
	{MANIM_MISSILE,  8, NULL},
	{MANIM_MISSILE, 11, NULL},
	{MANIM_MISSILE, 12, NULL},
	{MANIM_MISSILE, 12, NULL},
	{MANIM_MISSILE, 17, soldier_attack6_refire}
};
mmove_t soldier_move_attack6 = { FRAME_runs01, FRAME_runs14, soldier_frames_attack6, soldier_run };

void soldier_attack(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_SLIDING)
	{
		self->monsterinfo.currentmove = &soldier_move_attack6;
		return;
	}

	if (self->s.skinnum < 4)
	{
		if (random() < 0.5)
		{
			self->monsterinfo.currentmove = &soldier_move_attack1;
		}
		else
		{
			self->monsterinfo.currentmove = &soldier_move_attack2;
		}
	}
	else
	{
		self->monsterinfo.currentmove = &soldier_move_attack4;
	}
}


//
// SIGHT
//

void soldier_sight(edict_t* self, edict_t* other)
{
	if (random() < 0.5)
		gi.sound(self, CHAN_VOICE, sound_sight1, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_sight2, 1, ATTN_NORM, 0);

	if ((skill->value > 0) && (range(self, self->enemy) >= RANGE_MID))
	{
		if (random() > 0.5)
		{
			self->monsterinfo.currentmove = &soldier_move_attack6;
		}
	}
}

//
// DUCK
//

void soldier_duck_hold(edict_t* self)
{
	if (level.time >= self->monsterinfo.pausetime)
		self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	else
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		// SLUGFILLER--shoot at him?
		if (ai_checkenemy(self) && infront(self, self->enemy) &&
			random() < 0.4)
		{
			self->monsterinfo.currentmove = &soldier_move_attack3;
			self->monsterinfo.nextframe = FRAME_attak303;
			self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
		}
	}
}

mframe_t soldier_frames_duck[] =
{
	{MANIM_DUCK, 5,  monster_duck_down},
	{MANIM_DUCK, -1, soldier_duck_hold},
	{MANIM_DUCK, 1,  NULL},
	{MANIM_DUCK, 0,  monster_duck_up},
	{MANIM_DUCK, 5,  NULL}
};
mmove_t soldier_move_duck = { FRAME_duck01, FRAME_duck05, soldier_frames_duck, soldier_run };

void soldier_duck(edict_t* self, edict_t* attacker, float eta)
{
	float	r;

	self->monsterinfo.pausetime = level.time + eta + 0.3;

	// SLUGFILLER--if no target, just duck
	if (skill->value == 0 || !ai_checkenemy(self) || !infront(self, self->enemy))
	{
		self->monsterinfo.currentmove = &soldier_move_duck;
		return;
	}

	r = random();

	if (skill->value == 1)
	{
		if (r > 0.33)
			self->monsterinfo.currentmove = &soldier_move_duck;
		else
		{
			self->pain_debounce_time = level.time + 1;
			self->monsterinfo.currentmove = &soldier_move_attack3;
		}
		return;
	}

	if (skill->value >= 2)
	{
		if (r > 0.66)
			self->monsterinfo.currentmove = &soldier_move_duck;
		else
		{
			self->pain_debounce_time = level.time + 1;
			self->monsterinfo.currentmove = &soldier_move_attack3;
		}
		return;
	}

	self->pain_debounce_time = level.time + 1;
	self->monsterinfo.currentmove = &soldier_move_attack3;
}


//
// DEATH
//

void soldier_fire6(edict_t* self)
{
	soldier_fire(self, 5);
}

void soldier_fire7(edict_t* self)
{
	soldier_fire(self, 6);
}

void soldier_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity(self);
}

mframe_t soldier_frames_death1[] =
{
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, -10, NULL},
	{MANIM_DEATH, -10, NULL},
	{MANIM_DEATH, -10, soldier_dead},
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   soldier_fire6},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   soldier_fire7},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t soldier_move_death1 = { FRAME_death101, FRAME_death136, soldier_frames_death1, monster_dead_dead };

mframe_t soldier_frames_death2[] =
{
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, -5,  soldier_dead},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t soldier_move_death2 = { FRAME_death201, FRAME_death235, soldier_frames_death2, monster_dead_dead };

mframe_t soldier_frames_death3[] =
{
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, -5,  soldier_dead},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t soldier_move_death3 = { FRAME_death301, FRAME_death345, soldier_frames_death3, monster_dead_dead };

mframe_t soldier_frames_death4[] =
{
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   soldier_dead},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t soldier_move_death4 = { FRAME_death401, FRAME_death453, soldier_frames_death4, monster_dead_dead };

mframe_t soldier_frames_death5[] =
{
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, -5,  NULL},
	{MANIM_DEATH, -5,  soldier_dead},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},

	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t soldier_move_death5 = { FRAME_death501, FRAME_death524, soldier_frames_death5, monster_dead_dead };

mframe_t soldier_frames_death6[] =
{
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   soldier_dead},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t soldier_move_death6 = { FRAME_death601, FRAME_death610, soldier_frames_death6, monster_dead_dead };

void soldier_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	int		n;

	// check for gib
	if (self->gib_health <= 0)
	{
		gi.sound(self, CHAN_VOICE, gi.soundindex("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n = 0; n < 3; n++)
			ThrowGib(self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowGib(self, "models/objects/gibs/chest/tris.md2", damage, GIB_ORGANIC);
		ThrowHead(self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum |= 1;

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	if (self->s.skinnum == 1)
		gi.sound(self, CHAN_VOICE, sound_death_light, 1, ATTN_NORM, 0);
	else if (self->s.skinnum == 3)
		gi.sound(self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	else // (self->s.skinnum == 5)
		gi.sound(self, CHAN_VOICE, sound_death_ss, 1, ATTN_NORM, 0);

	// SLUGFILLER--checking for a head shot is much simpler with my new head shot system
	if (meansOfDeath & MOD_HEAD_SHOT)
	{
		// head shot
		self->monsterinfo.currentmove = &soldier_move_death3;
		return;
	}

	n = rand() % 5;
	if (n == 0)
		self->monsterinfo.currentmove = &soldier_move_death1;
	else if (n == 1)
		self->monsterinfo.currentmove = &soldier_move_death2;
	else if (n == 2)
		self->monsterinfo.currentmove = &soldier_move_death4;
	else if (n == 3)
		self->monsterinfo.currentmove = &soldier_move_death5;
	else
		self->monsterinfo.currentmove = &soldier_move_death6;
}


//
// SPAWN
//

void SP_monster_soldier_x(edict_t* self)
{

	self->s.modelindex = gi.modelindex("models/monsters/soldier/tris.md2");
	self->monsterinfo.scale = MODEL_SCALE;
	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, 32);
	VectorSet(self->headmins, -16, -16, 20);
	VectorSet(self->headmaxs, 16, 16, 32);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	sound_idle = gi.soundindex("soldier/solidle1.wav");
	sound_sight1 = gi.soundindex("soldier/solsght1.wav");
	sound_sight2 = gi.soundindex("soldier/solsrch1.wav");
	sound_cock = gi.soundindex("infantry/infatck3.wav");

	self->mass = 100;

	self->pain = soldier_pain;
	self->die = soldier_die;

	self->monsterinfo.stand = soldier_stand;
	self->monsterinfo.walk = soldier_walk;
	self->monsterinfo.run = soldier_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.duck = soldier_duck;
	self->monsterinfo.unduck = monster_duck_up;
	self->monsterinfo.attack = soldier_attack;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = soldier_sight;

	self->monsterinfo.speed = 12;
	self->monsterinfo.walkspeed = 5;

	gi.linkentity(self);

	self->monsterinfo.stand(self);

	walkmonster_start(self);
}


/*QUAKED monster_soldier_light (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_soldier_light(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain_light = gi.soundindex("soldier/solpain2.wav");
	sound_death_light = gi.soundindex("soldier/soldeth2.wav");
	gi.modelindex("models/objects/laser/tris.md2");
	gi.soundindex("misc/lasfly.wav");
	gi.soundindex("soldier/solatck2.wav");

	self->health = 20;
	self->gib_health = 50;

	SP_monster_soldier_x(self);

	self->s.skinnum = 0;
}

/*QUAKED monster_soldier (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_soldier(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain = gi.soundindex("soldier/solpain1.wav");
	sound_death = gi.soundindex("soldier/soldeth1.wav");
	gi.soundindex("soldier/solatck1.wav");

	self->health = 30;
	self->gib_health = 60;

	SP_monster_soldier_x(self);

	self->s.skinnum = 2;
}

/*QUAKED monster_soldier_ss (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_soldier_ss(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain_ss = gi.soundindex("soldier/solpain3.wav");
	sound_death_ss = gi.soundindex("soldier/soldeth3.wav");
	gi.soundindex("soldier/solatck3.wav");

	self->health = 40;
	self->gib_health = 70;

	SP_monster_soldier_x(self);

	self->s.skinnum = 4;
}
