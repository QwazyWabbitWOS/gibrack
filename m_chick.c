/*
==============================================================================

chick

==============================================================================
*/

#include "g_local.h"
#include "m_chick.h"

qboolean visible(edict_t* self, edict_t* other);

void chick_stand(edict_t* self);
void chick_run(edict_t* self);
void chick_reslash(edict_t* self);
void chick_rerocket(edict_t* self);
void chick_attack1(edict_t* self);

static int	sound_missile_prelaunch;
static int	sound_missile_launch;
static int	sound_melee_swing;
static int	sound_melee_hit;
static int	sound_missile_reload;
static int	sound_death1;
static int	sound_death2;
static int	sound_fall_down;
static int	sound_idle1;
static int	sound_idle2;
static int	sound_pain1;
static int	sound_pain2;
static int	sound_pain3;
static int	sound_sight;
static int	sound_search;


void ChickMoan(edict_t* self)
{
	if (random() < 0.5)
		gi.sound(self, CHAN_VOICE, sound_idle1, 1, ATTN_IDLE, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_idle2, 1, ATTN_IDLE, 0);
}

mframe_t chick_frames_fidget[] =
{
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  ChickMoan},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL}
};
mmove_t chick_move_fidget = { FRAME_stand201, FRAME_stand230, chick_frames_fidget, chick_stand };

void chick_fidget(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		return;
	if (random() <= 0.3)
		self->monsterinfo.currentmove = &chick_move_fidget;
}

mframe_t chick_frames_stand[] =
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
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, chick_fidget}

};
mmove_t chick_move_stand = { FRAME_stand101, FRAME_stand130, chick_frames_stand, NULL };

void chick_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &chick_move_stand;
}

mframe_t chick_frames_start_run[] =
{
	{MANIM_RUN, -8,  NULL},
	{MANIM_RUN, -9,  NULL},
	{MANIM_RUN, -9,	 NULL},
	{MANIM_RUN, -10, NULL},
	{MANIM_RUN, -10, NULL},
	{MANIM_RUN, -9,  NULL},
	{MANIM_RUN, -8,  NULL},
	{MANIM_RUN, -6,  NULL},
	{MANIM_RUN, -3,	 NULL},
	{MANIM_RUN, -6,	 NULL}
};
mmove_t chick_move_start_run = { FRAME_walk01, FRAME_walk10, chick_frames_start_run, chick_run };

mframe_t chick_frames_run[] =
{
	{MANIM_RUN, -3,	NULL},
	{MANIM_RUN, -1,  NULL},
	{MANIM_RUN, 4, NULL},
	{MANIM_RUN, -4,  NULL},
	{MANIM_RUN, -2,  NULL},
	{MANIM_RUN, -5,  NULL},
	{MANIM_RUN, 2, NULL},
	{MANIM_RUN, -4,  NULL},
	{MANIM_RUN, 0,  NULL},
	{MANIM_RUN, -2,  NULL}

};

mmove_t chick_move_run = { FRAME_walk11, FRAME_walk20, chick_frames_run, NULL };

mframe_t chick_frames_walk[] =
{
	{MANIM_WALK, -3,	 NULL},
	{MANIM_WALK, -1,  NULL},
	{MANIM_WALK, 4, NULL},
	{MANIM_WALK, -4,  NULL},
	{MANIM_WALK, -2,  NULL},
	{MANIM_WALK, -5,  NULL},
	{MANIM_WALK, 2, NULL},
	{MANIM_WALK, -4,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, -2,  NULL}
};

mmove_t chick_move_walk = { FRAME_walk11, FRAME_walk20, chick_frames_walk, NULL };

void chick_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &chick_move_walk;
}

void chick_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		self->monsterinfo.currentmove = &chick_move_stand;
		return;
	}

	if (self->monsterinfo.currentmove == &chick_move_walk ||
		self->monsterinfo.currentmove == &chick_move_start_run)
	{
		self->monsterinfo.currentmove = &chick_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &chick_move_start_run;
	}
}

mframe_t chick_frames_pain1[] =
{
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL}
};
mmove_t chick_move_pain1 = { FRAME_pain101, FRAME_pain105, chick_frames_pain1, chick_run };

mframe_t chick_frames_pain2[] =
{
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL},
	{MANIM_MISC, 0, NULL}
};
mmove_t chick_move_pain2 = { FRAME_pain201, FRAME_pain205, chick_frames_pain2, chick_run };

mframe_t chick_frames_pain3[] =
{
	{MANIM_MISC, 0,		NULL},
	{MANIM_MISC, 0,		NULL},
	{MANIM_MISC, -6,	NULL},
	{MANIM_MISC, 3,		NULL},
	{MANIM_MISC, 11,	NULL},
	{MANIM_MISC, 3,		NULL},
	{MANIM_MISC, 0,		NULL},
	{MANIM_MISC, 0,		NULL},
	{MANIM_MISC, 4,		NULL},
	{MANIM_MISC, 1,		NULL},
	{MANIM_MISC, 0,		NULL},
	{MANIM_MISC, -3,	NULL},
	{MANIM_MISC, -4,	NULL},
	{MANIM_MISC, 5,		NULL},
	{MANIM_MISC, 7,		NULL},
	{MANIM_MISC, -2,	NULL},
	{MANIM_MISC, 3,		NULL},
	{MANIM_MISC, -5,	NULL},
	{MANIM_MISC, -2,	NULL},
	{MANIM_MISC, -8,	NULL},
	{MANIM_MISC, 2,		NULL}
};
mmove_t chick_move_pain3 = { FRAME_pain301, FRAME_pain321, chick_frames_pain3, chick_run };

void chick_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	float	r;

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	r = random();
	if (r < 0.33)
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else if (r < 0.66)
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_pain3, 1, ATTN_NORM, 0);

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 10)
		self->monsterinfo.currentmove = &chick_move_pain1;
	else if (damage <= 25)
		self->monsterinfo.currentmove = &chick_move_pain2;
	else
		self->monsterinfo.currentmove = &chick_move_pain3;
}

void chick_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -16, -16, 0);
	VectorSet(self->maxs, 16, 16, 16);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity(self);
}

mframe_t chick_frames_death2[] =
{
	{MANIM_DEATH, -6, NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, -1,  NULL},
	{MANIM_DEATH, -5, NULL},
	{MANIM_DEATH, 0, NULL},
	{MANIM_DEATH, -1,  NULL},
	{MANIM_DEATH, -2,  NULL},
	{MANIM_DEATH, 1,  NULL},
	{MANIM_DEATH, 10, NULL},
	{MANIM_DEATH, 2,  NULL},
	{MANIM_DEATH, 3,  NULL},
	{MANIM_DEATH, 1,  NULL},
	{MANIM_DEATH, 2, NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 3,  NULL},
	{MANIM_DEATH, 3,  NULL},
	{MANIM_DEATH, 1,  NULL},
	{MANIM_DEATH, -3,  NULL},
	{MANIM_DEATH, -5, NULL},
	{MANIM_DEATH, 4, NULL},
	{MANIM_DEATH, 15, NULL},
	{MANIM_DEATH, 14, chick_dead},
	{MANIM_DEATH, 1, NULL}
};
mmove_t chick_move_death2 = { FRAME_death201, FRAME_death223, chick_frames_death2, monster_dead_dead };

mframe_t chick_frames_death1[] =
{
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  chick_dead},
	{MANIM_DEATH, -7, NULL},
	{MANIM_DEATH, 4,  NULL},
	{MANIM_DEATH, 11, NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  NULL},
	{MANIM_DEATH, 0,  NULL}

};
mmove_t chick_move_death1 = { FRAME_death101, FRAME_death112, chick_frames_death1, monster_dead_dead };

void chick_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	int		n;

	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

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
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	n = rand() % 2;
	if (n == 0)
	{
		self->monsterinfo.currentmove = &chick_move_death1;
		gi.sound(self, CHAN_VOICE, sound_death1, 1, ATTN_NORM, 0);
	}
	else
	{
		self->monsterinfo.currentmove = &chick_move_death2;
		gi.sound(self, CHAN_VOICE, sound_death2, 1, ATTN_NORM, 0);
	}
}


mframe_t chick_frames_duck[] =
{
	{MANIM_DUCK, 0, monster_duck_down},
	{MANIM_DUCK, 1, NULL},
	{MANIM_DUCK, 4, monster_duck_hold},
	{MANIM_DUCK, -4,  NULL},
	{MANIM_DUCK, -5,  monster_duck_up},
	{MANIM_DUCK, 3, NULL},
	{MANIM_DUCK, 1,  NULL}
};
mmove_t chick_move_duck = { FRAME_duck01, FRAME_duck07, chick_frames_duck, chick_run };

void chick_duck(edict_t* self, edict_t* attacker, float eta)
{
	self->monsterinfo.currentmove = &chick_move_duck;
	self->monsterinfo.pausetime = level.time + 1;
}

void ChickSlash(edict_t* self)
{
	vec3_t	aim;

	VectorSet(aim, MELEE_DISTANCE, self->mins[0], 10);
	gi.sound(self, CHAN_WEAPON, sound_melee_swing, 1, ATTN_NORM, 0);
	fire_hit(self, aim, (10 + (rand() % 6)), 20);
}


void ChickRocket(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_CHICK_ROCKET_1], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);

	monster_fire_rocket(self, start, dir, 100, 650, MZ2_CHICK_ROCKET_1);
}

void Chick_PreAttack1(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_missile_prelaunch, 1, ATTN_NORM, 0);
}

void ChickReload(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_missile_reload, 1, ATTN_NORM, 0);
}


mframe_t chick_frames_start_attack1[] =
{
	{MANIM_MISSILE, 0,	Chick_PreAttack1},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, 4,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, -3,  NULL},
	{MANIM_MISSILE, 3,	NULL},
	{MANIM_MISSILE, 5,	NULL},
	{MANIM_MISSILE, 7,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, 0,	chick_attack1}
};
mmove_t chick_move_start_attack1 = { FRAME_attak101, FRAME_attak113, chick_frames_start_attack1, NULL };


mframe_t chick_frames_attack1[] =
{
	{MANIM_MISSILE, 19,	ChickRocket},
	{MANIM_MISSILE, -6,	NULL},
	{MANIM_MISSILE, -5,	NULL},
	{MANIM_MISSILE, -2,	NULL},
	{MANIM_MISSILE, -7,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, 1,	NULL},
	{MANIM_MISSILE, 10,	ChickReload},
	{MANIM_MISSILE, 4,	NULL},
	{MANIM_MISSILE, 5,	NULL},
	{MANIM_MISSILE, 6,	NULL},
	{MANIM_MISSILE, 6,	NULL},
	{MANIM_MISSILE, 4,	NULL},
	{MANIM_MISSILE, 3,	chick_rerocket}

};
mmove_t chick_move_attack1 = { FRAME_attak114, FRAME_attak127, chick_frames_attack1, NULL };

mframe_t chick_frames_end_attack1[] =
{
	{MANIM_MISSILE, -3,	NULL},
	{MANIM_MISSILE, 0,	NULL},
	{MANIM_MISSILE, -6,	NULL},
	{MANIM_MISSILE, -4,	NULL},
	{MANIM_MISSILE, -2,  NULL}
};
mmove_t chick_move_end_attack1 = { FRAME_attak128, FRAME_attak132, chick_frames_end_attack1, chick_run };

void chick_rerocket(edict_t* self)
{
	if (self->enemy->health > 0)
	{
		if (range(self, self->enemy) > RANGE_MELEE)
			if (visible(self, self->enemy))
				if (random() <= 0.6)
				{
					self->pain_debounce_time = level.time + 1.5;
					self->monsterinfo.currentmove = &chick_move_attack1;
					return;
				}
	}
	self->pain_debounce_time = level.time + 0.6;
	self->monsterinfo.currentmove = &chick_move_end_attack1;
}

void chick_attack1(edict_t* self)
{
	self->pain_debounce_time = level.time + 1.5;
	self->monsterinfo.currentmove = &chick_move_attack1;
}

mframe_t chick_frames_slash[] =
{
	{MANIM_MELEE, 1,	NULL},
	{MANIM_MELEE, 7,	ChickSlash},
	{MANIM_MELEE, -7,	NULL},
	{MANIM_MELEE, 1,	NULL},
	{MANIM_MELEE, -1,	NULL},
	{MANIM_MELEE, 1,	NULL},
	{MANIM_MELEE, 0,	NULL},
	{MANIM_MELEE, 1,	NULL},
	{MANIM_MELEE, -2,	chick_reslash}
};
mmove_t chick_move_slash = { FRAME_attak204, FRAME_attak212, chick_frames_slash, NULL };

mframe_t chick_frames_end_slash[] =
{
	{MANIM_MELEE, -6,	NULL},
	{MANIM_MELEE, -1,	NULL},
	{MANIM_MELEE, -6,	NULL},
	{MANIM_MELEE, 0,	NULL}
};
mmove_t chick_move_end_slash = { FRAME_attak213, FRAME_attak216, chick_frames_end_slash, chick_run };


void chick_reslash(edict_t* self)
{
	if (self->enemy->health > 0)
	{
		if (range(self, self->enemy) == RANGE_MELEE)
		{
			if (random() <= 0.9)
			{
				self->pain_debounce_time = level.time + 1;
				self->monsterinfo.currentmove = &chick_move_slash;
				return;
			}
			else
			{
				self->pain_debounce_time = level.time + 0.5;
				self->monsterinfo.currentmove = &chick_move_end_slash;
				return;
			}
		}
	}
	self->pain_debounce_time = level.time + 0.5;
	self->monsterinfo.currentmove = &chick_move_end_slash;
}

void chick_slash(edict_t* self)
{
	self->pain_debounce_time = level.time + 1;
	self->monsterinfo.currentmove = &chick_move_slash;
}


mframe_t chick_frames_start_slash[] =
{
	{MANIM_MELEE, 1,	NULL},
	{MANIM_MELEE, 8,	NULL},
	{MANIM_MELEE, 3,	NULL}
};
mmove_t chick_move_start_slash = { FRAME_attak201, FRAME_attak203, chick_frames_start_slash, chick_slash };



void chick_melee(edict_t* self)
{
	self->pain_debounce_time = level.time + 0.4;
	self->monsterinfo.currentmove = &chick_move_start_slash;
}


void chick_attack(edict_t* self)
{
	self->pain_debounce_time = level.time + 1.4;
	self->monsterinfo.currentmove = &chick_move_start_attack1;
}

void chick_sight(edict_t* self, edict_t* other)
{
	gi.sound(self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

/*QUAKED monster_chick (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_chick(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_missile_prelaunch = gi.soundindex("chick/chkatck1.wav");
	sound_missile_launch = gi.soundindex("chick/chkatck2.wav");
	sound_melee_swing = gi.soundindex("chick/chkatck3.wav");
	sound_melee_hit = gi.soundindex("chick/chkatck4.wav");
	sound_missile_reload = gi.soundindex("chick/chkatck5.wav");
	sound_death1 = gi.soundindex("chick/chkdeth1.wav");
	sound_death2 = gi.soundindex("chick/chkdeth2.wav");
	sound_fall_down = gi.soundindex("chick/chkfall1.wav");
	sound_idle1 = gi.soundindex("chick/chkidle1.wav");
	sound_idle2 = gi.soundindex("chick/chkidle2.wav");
	sound_pain1 = gi.soundindex("chick/chkpain1.wav");
	sound_pain2 = gi.soundindex("chick/chkpain2.wav");
	sound_pain3 = gi.soundindex("chick/chkpain3.wav");
	sound_sight = gi.soundindex("chick/chksght1.wav");
	sound_search = gi.soundindex("chick/chksrch1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/bitch/tris.md2");
	VectorSet(self->mins, -16, -16, 0);
	VectorSet(self->maxs, 16, 16, 56);
	VectorSet(self->headmins, -16, -16, 48);
	VectorSet(self->headmaxs, 16, 16, 56);

	self->health = 175;
	self->gib_health = 245;
	self->mass = 200;

	self->pain = chick_pain;
	self->die = chick_die;

	self->monsterinfo.stand = chick_stand;
	self->monsterinfo.walk = chick_walk;
	self->monsterinfo.run = chick_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.duck = chick_duck;
	self->monsterinfo.unduck = monster_duck_up;
	self->monsterinfo.attack = chick_attack;
	self->monsterinfo.melee = chick_melee;
	self->monsterinfo.sight = chick_sight;

	self->monsterinfo.speed = 9;
	self->monsterinfo.walkspeed = 9;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &chick_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
