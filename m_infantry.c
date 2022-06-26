/*
==============================================================================

INFANTRY

==============================================================================
*/

#include "g_local.h"
#include "m_infantry.h"

void InfantryMachineGun(edict_t* self);


static int	sound_pain1;
static int	sound_pain2;
static int	sound_die1;
static int	sound_die2;

static int	sound_gunshot;
static int	sound_weapon_cock;
static int	sound_punch_swing;
static int	sound_punch_hit;
static int	sound_sight;
static int	sound_search;
static int	sound_idle;


mframe_t infantry_frames_stand[] =
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
	{MANIM_STAND, 0, NULL}
};
mmove_t infantry_move_stand = { FRAME_stand50, FRAME_stand71, infantry_frames_stand, NULL };

void infantry_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &infantry_move_stand;
}


void infantry_fidget_down(edict_t* self)
{
	self->maxs[2] -= 14;
	self->headmins[2] -= 14;
	self->headmaxs[2] -= 14;
}

void infantry_fidget_up(edict_t* self)
{
	self->maxs[2] += 14;
	self->headmins[2] += 14;
	self->headmaxs[2] += 14;
}

mframe_t infantry_frames_fidget[] =
{
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 3,  NULL},
	{MANIM_STAND, 6,  NULL},
	{MANIM_STAND, 3,  infantry_fidget_down},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, -1, NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, -2, NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, -1, NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, -1, NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, -1, NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 1,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, 0,  NULL},
	{MANIM_STAND, -1, NULL},
	{MANIM_STAND, -1, NULL},
	{MANIM_STAND, 0,  infantry_fidget_up},
	{MANIM_STAND, -3, NULL},
	{MANIM_STAND, -2, NULL},
	{MANIM_STAND, -3, NULL},
	{MANIM_STAND, -3, NULL},
	{MANIM_STAND, -2, NULL}
};
mmove_t infantry_move_fidget = { FRAME_stand01, FRAME_stand49, infantry_frames_fidget, infantry_stand };

void infantry_fidget(edict_t* self)
{
	self->monsterinfo.currentmove = &infantry_move_fidget;
	gi.sound(self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

mframe_t infantry_frames_walk[] =
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
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL},
	{MANIM_WALK, 0,  NULL}
};
mmove_t infantry_move_walk = { FRAME_walk03, FRAME_walk14, infantry_frames_walk, NULL };

void infantry_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &infantry_move_walk;
}

mframe_t infantry_frames_run[] =
{
	{MANIM_RUN, 0, NULL},
	{MANIM_RUN, 0, NULL},
	{MANIM_RUN, 0,  NULL},
	{MANIM_RUN, 0,  NULL},
	{MANIM_RUN, 0, NULL},
	{MANIM_RUN, 0, NULL},
	{MANIM_RUN, 0,  NULL},
	{MANIM_RUN, 0,  NULL}
};
mmove_t infantry_move_run = { FRAME_run01, FRAME_run08, infantry_frames_run, NULL };

void infantry_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &infantry_move_stand;
	else
		self->monsterinfo.currentmove = &infantry_move_run;
}


mframe_t infantry_frames_pain1[] =
{
	{MANIM_MISC, -3, NULL},
	{MANIM_MISC, -2, NULL},
	{MANIM_MISC, -1, NULL},
	{MANIM_MISC, -2, NULL},
	{MANIM_MISC, -1, NULL},
	{MANIM_MISC, 1,  NULL},
	{MANIM_MISC, -1, NULL},
	{MANIM_MISC, 1,  NULL},
	{MANIM_MISC, 6,  NULL},
	{MANIM_MISC, 2,  NULL}
};
mmove_t infantry_move_pain1 = { FRAME_pain101, FRAME_pain110, infantry_frames_pain1, infantry_run };

mframe_t infantry_frames_pain2[] =
{
	{MANIM_MISC, -3, NULL},
	{MANIM_MISC, -3, NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, -1, NULL},
	{MANIM_MISC, -2, NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 0,  NULL},
	{MANIM_MISC, 2,  NULL},
	{MANIM_MISC, 5,  NULL},
	{MANIM_MISC, 2,  NULL}
};
mmove_t infantry_move_pain2 = { FRAME_pain201, FRAME_pain210, infantry_frames_pain2, infantry_run };

void infantry_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	int		n;

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	monster_duck_up(self);

	n = rand() % 2;
	if (n == 0)
	{
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	}
	else
	{
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
	}
	if (self->monsterinfo.currentmove->frame[self->s.frame - self->monsterinfo.currentmove->firstframe].animtype > MANIM_MISC)
		return;
	if (n == 0)
	{
		self->monsterinfo.currentmove = &infantry_move_pain1;
	}
	else
	{
		self->monsterinfo.currentmove = &infantry_move_pain2;
	}
}


vec3_t	aimangles[] =
{
	{0.0, 5.0, 0.0 },
	{10.0, 15.0, 0.0},
	{20.0, 25.0, 0.0},
	{25.0, 35.0, 0.0},
	{30.0, 40.0, 0.0},
	{30.0, 45.0, 0.0},
	{25.0, 50.0, 0.0},
	{20.0, 40.0, 0.0},
	{15.0, 35.0, 0.0},
	{40.0, 35.0, 0.0},
	{70.0, 35.0, 0.0},
	{90.0, 35.0, 0.0}
};

void InfantryMachineGun(edict_t* self)
{
	vec3_t	start;
	vec3_t	forward, right;
	vec3_t	vec;
	int		flash_number;

	if (self->s.frame == FRAME_attak111)
	{
		flash_number = MZ2_INFANTRY_MACHINEGUN_1;
		AngleVectors(self->s.angles, forward, right, NULL);
		G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

		AngleVectors(self->aimangles, forward, NULL, NULL);
	}
	else
	{
		flash_number = MZ2_INFANTRY_MACHINEGUN_2 + (self->s.frame - FRAME_death211);

		AngleVectors(self->s.angles, forward, right, NULL);
		G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

		VectorSubtract(self->s.angles, aimangles[flash_number - MZ2_INFANTRY_MACHINEGUN_2], vec);
		AngleVectors(vec, forward, NULL, NULL);
	}

	monster_fire_bullet(self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_number, true);
}

void infantry_sight(edict_t* self, edict_t* other)
{
	gi.sound(self, CHAN_BODY, sound_sight, 1, ATTN_NORM, 0);
}

void infantry_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity(self);
}

void SpawnDamage(int type, vec3_t origin, vec3_t normal, int damage);

// SLUGFILLER--got blood spilling from where his head was
void InfantryBlood(edict_t* self)
{
	vec3_t	org, vec;

	VectorSet(vec, crandom() * 20, crandom() * 20, 300);
	VectorCopy(self->s.origin, org);
	org[2] += self->headmins[2];

	ThrowBlood(org, vec);
	SpawnDamage(TE_BLOOD, org, vec, 20);
}

// SLUGFILLER--a combination of InfantryBlood and InfantryMachineGun
void InfantryMachineGunBlood(edict_t* self)
{
	InfantryBlood(self);
	InfantryMachineGun(self);
}

mframe_t infantry_frames_death1[] =
{
	{MANIM_DEATH, -4, InfantryBlood},
	{MANIM_DEATH, 0,  InfantryBlood},
	{MANIM_DEATH, 0,  InfantryBlood},
	{MANIM_DEATH, -1, InfantryBlood},
	{MANIM_DEATH, -4, InfantryBlood},
	{MANIM_DEATH, 0,  InfantryBlood},
	{MANIM_DEATH, 0,  InfantryBlood},
	{MANIM_DEATH, 0,  InfantryBlood},
	{MANIM_DEATH, -1, InfantryBlood},
	{MANIM_DEATH, 3,  InfantryBlood},
	{MANIM_DEATH, 1,  InfantryBlood},
	{MANIM_DEATH, 1,  InfantryBlood},
	{MANIM_DEATH, -2, NULL},
	{MANIM_DEATH, 2,  NULL},
	{MANIM_DEATH, 2,  NULL},
	{MANIM_DEATH, 9,  NULL},
	{MANIM_DEATH, 9,  NULL},
	{MANIM_DEATH, 5,  infantry_dead},
	{MANIM_DEATH, -3, NULL},
	{MANIM_DEATH, -3, NULL}
};
mmove_t infantry_move_death1 = { FRAME_death101, FRAME_death120, infantry_frames_death1, M_FlyCheck };

// Off with his head
mframe_t infantry_frames_death2[] =
{
	{MANIM_DEATH, 0,   InfantryBlood},
	{MANIM_DEATH, 1,   InfantryBlood},
	{MANIM_DEATH, 5,   InfantryBlood},
	{MANIM_DEATH, -1,  InfantryBlood},
	{MANIM_DEATH, 0,   InfantryBlood},
	{MANIM_DEATH, 1,   InfantryBlood},
	{MANIM_DEATH, 1,   InfantryBlood},
	{MANIM_DEATH, 4,   InfantryBlood},
	{MANIM_DEATH, 3,   InfantryBlood},
	{MANIM_DEATH, 0,   InfantryBlood},
	{MANIM_DEATH, -2,  InfantryMachineGunBlood},
	{MANIM_DEATH, -2,  InfantryMachineGunBlood},
	{MANIM_DEATH, -3,  InfantryMachineGunBlood},
	{MANIM_DEATH, -1,  InfantryMachineGunBlood},
	{MANIM_DEATH, -2,  InfantryMachineGunBlood},
	{MANIM_DEATH, 0,   InfantryMachineGunBlood},
	{MANIM_DEATH, 2,   InfantryMachineGunBlood},
	{MANIM_DEATH, 2,   InfantryMachineGunBlood},
	{MANIM_DEATH, 3,   InfantryMachineGunBlood},
	{MANIM_DEATH, -10, InfantryMachineGun},
	{MANIM_DEATH, -7,  InfantryMachineGun},
	{MANIM_DEATH, -8,  InfantryMachineGun},
	{MANIM_DEATH, -6,  infantry_dead},
	{MANIM_DEATH, 4,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t infantry_move_death2 = { FRAME_death201, FRAME_death225, infantry_frames_death2, M_FlyCheck };

mframe_t infantry_frames_death3[] =
{
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   infantry_dead},
	{MANIM_DEATH, -6,  NULL},
	{MANIM_DEATH, -11, NULL},
	{MANIM_DEATH, -3,  NULL},
	{MANIM_DEATH, -11, NULL},
	{MANIM_DEATH, 0,   NULL},
	{MANIM_DEATH, 0,   NULL}
};
mmove_t infantry_move_death3 = { FRAME_death301, FRAME_death309, infantry_frames_death3, M_FlyCheck };


void infantry_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
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
		if (self->monsterinfo.currentmove != &infantry_move_death1 && self->monsterinfo.currentmove != &infantry_move_death2)
			ThrowHead(self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		else
		{
			G_FreeEdict(self);
			return;
		}
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	n = rand() % 2;
	if (meansOfDeath & MOD_HEAD_SHOT)
	{
		edict_t* head;
		head = G_Spawn();
		VectorCopy(self->s.origin, head->s.origin);
		head->s.origin[2] += self->viewheight;
		VectorCopy(self->s.angles, head->s.angles);
		VectorCopy(self->velocity, head->velocity);
		ThrowHead(head, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		if (n == 0)
		{
			self->monsterinfo.currentmove = &infantry_move_death1;
			gi.sound(self, CHAN_VOICE, sound_die2, 1, ATTN_NORM, 0);
		}
		else
		{
			self->monsterinfo.currentmove = &infantry_move_death2;
			gi.sound(self, CHAN_VOICE, sound_die1, 1, ATTN_NORM, 0);
		}
		self->s.skinnum = 1;
	}
	else
	{
		self->monsterinfo.currentmove = &infantry_move_death3;
		gi.sound(self, CHAN_VOICE, sound_die2, 1, ATTN_NORM, 0);
	}
}


mframe_t infantry_frames_duck[] =
{
	{MANIM_DUCK, -2, monster_duck_down},
	{MANIM_DUCK, -5, monster_duck_hold},
	{MANIM_DUCK, 3,  NULL},
	{MANIM_DUCK, 4,  monster_duck_up},
	{MANIM_DUCK, 0,  NULL}
};
mmove_t infantry_move_duck = { FRAME_duck01, FRAME_duck05, infantry_frames_duck, infantry_run };

void infantry_duck(edict_t* self, edict_t* attacker, float eta)
{
	self->monsterinfo.currentmove = &infantry_move_duck;
	self->monsterinfo.pausetime = level.time + 1;
}


void infantry_cock_gun(edict_t* self)
{
	int		n;

	gi.sound(self, CHAN_WEAPON, sound_weapon_cock, 1, ATTN_NORM, 0);
	n = (rand() & 15) + 3 + 7;
	self->monsterinfo.pausetime = level.time + n * FRAMETIME;
	self->pain_debounce_time = self->monsterinfo.pausetime + 1.2;
}

void infantry_fire(edict_t* self)
{
	InfantryMachineGun(self);

	if (level.time >= self->monsterinfo.pausetime)
		self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	else
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
}

mframe_t infantry_frames_attack1[] =
{
	{MANIM_MISSILE, 4,  NULL},
	{MANIM_MISSILE, -1, NULL},
	{MANIM_MISSILE, -1, NULL},
	{MANIM_MISSILE, 0,  infantry_cock_gun},
	{MANIM_MISSILE, -1, NULL},
	{MANIM_MISSILE, 1,  NULL},
	{MANIM_MISSILE, 1,  NULL},
	{MANIM_MISSILE, 2,  NULL},
	{MANIM_MISSILE, -2, NULL},
	{MANIM_MISSILE, -3, NULL},
	{MANIM_MISSILE, 1,  infantry_fire},
	{MANIM_MISSILE, 5,  NULL},
	{MANIM_MISSILE, -1, NULL},
	{MANIM_MISSILE, -2, NULL},
	{MANIM_MISSILE, -3, NULL}
};
mmove_t infantry_move_attack1 = { FRAME_attak101, FRAME_attak115, infantry_frames_attack1, infantry_run };


void infantry_swing(edict_t* self)
{
	gi.sound(self, CHAN_WEAPON, sound_punch_swing, 1, ATTN_NORM, 0);
}

void infantry_smack(edict_t* self)
{
	vec3_t	aim;

	VectorSet(aim, MELEE_DISTANCE, 0, 0);
	if (fire_hit(self, aim, (5 + (rand() % 5)), 10))
		gi.sound(self, CHAN_WEAPON, sound_punch_hit, 1, ATTN_NORM, 0);
}

mframe_t infantry_frames_attack2[] =
{
	{MANIM_MELEE, 0, NULL},
	{MANIM_MELEE, 0, NULL},
	{MANIM_MELEE, 0, infantry_swing},
	{MANIM_MELEE, 0, NULL},
	{MANIM_MELEE, 0, NULL},
	{MANIM_MELEE, 0, infantry_smack},
	{MANIM_MELEE, 0, NULL},
	{MANIM_MELEE, 0, NULL}
};
mmove_t infantry_move_attack2 = { FRAME_attak201, FRAME_attak208, infantry_frames_attack2, infantry_run };

void infantry_attack(edict_t* self)
{
	if (range(self, self->enemy) == RANGE_MELEE)
	{
		self->pain_debounce_time = level.time + 0.9;
		self->monsterinfo.currentmove = &infantry_move_attack2;
	}
	else
	{
		self->pain_debounce_time = level.time + 1.6;
		self->monsterinfo.currentmove = &infantry_move_attack1;
	}
}


/*QUAKED monster_infantry (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_infantry(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain1 = gi.soundindex("infantry/infpain1.wav");
	sound_pain2 = gi.soundindex("infantry/infpain2.wav");
	sound_die1 = gi.soundindex("infantry/infdeth1.wav");
	sound_die2 = gi.soundindex("infantry/infdeth2.wav");

	sound_gunshot = gi.soundindex("infantry/infatck1.wav");
	sound_weapon_cock = gi.soundindex("infantry/infatck3.wav");
	sound_punch_swing = gi.soundindex("infantry/infatck2.wav");
	sound_punch_hit = gi.soundindex("infantry/melee2.wav");

	sound_sight = gi.soundindex("infantry/infsght1.wav");
	sound_search = gi.soundindex("infantry/infsrch1.wav");
	sound_idle = gi.soundindex("infantry/infidle1.wav");


	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/infantry/tris.md2");
	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, 32);
	VectorSet(self->headmins, -16, -16, 22);
	VectorSet(self->headmaxs, 16, 16, 32);

	self->health = 100;
	self->gib_health = 140;
	self->mass = 200;

	self->pain = infantry_pain;
	self->die = infantry_die;

	self->monsterinfo.stand = infantry_stand;
	self->monsterinfo.walk = infantry_walk;
	self->monsterinfo.run = infantry_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.duck = infantry_duck;
	self->monsterinfo.unduck = monster_duck_up;
	self->monsterinfo.attack = infantry_attack;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = infantry_sight;
	self->monsterinfo.idle = infantry_fidget;

	self->monsterinfo.speed = 20;
	self->monsterinfo.walkspeed = 5;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &infantry_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
