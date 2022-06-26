/*
==============================================================================

boss2

==============================================================================
*/

#include "g_local.h"
#include "m_boss2.h"

void BossExplode(edict_t* self);

qboolean infront(edict_t* self, edict_t* other);

static int	sound_pain1;
static int	sound_pain2;
static int	sound_pain3;
static int	sound_death;
static int	sound_search1;

void boss2_search(edict_t* self)
{
	if (random() < 0.5)
		gi.sound(self, CHAN_VOICE, sound_search1, 1, ATTN_NONE, 0);
}

void boss2_run(edict_t* self);
void boss2_stand(edict_t* self);
void boss2_attack(edict_t* self);
void boss2_attack_mg(edict_t* self);
void boss2_reattack_mg(edict_t* self);
void boss2_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point);

void Boss2Rocket(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;

	AngleVectors(self->s.angles, forward, right, NULL);
	AngleVectors(self->aimangles, dir, NULL, NULL);

	//1
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_BOSS2_ROCKET_1], forward, right, start);
	monster_fire_rocket(self, start, dir, 100, 650, MZ2_BOSS2_ROCKET_1);

	//2
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_BOSS2_ROCKET_2], forward, right, start);
	monster_fire_rocket(self, start, dir, 100, 650, MZ2_BOSS2_ROCKET_2);

	//3
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_BOSS2_ROCKET_3], forward, right, start);
	monster_fire_rocket(self, start, dir, 100, 650, MZ2_BOSS2_ROCKET_3);

	//4
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_BOSS2_ROCKET_4], forward, right, start);
	monster_fire_rocket(self, start, dir, 100, 650, MZ2_BOSS2_ROCKET_4);
}

void boss2_firebullet_right(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_BOSS2_MACHINEGUN_R1], forward, right, start);

	AngleVectors(self->aimangles, forward, NULL, NULL);

	monster_fire_bullet(self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_BOSS2_MACHINEGUN_R1, true);
}

void boss2_firebullet_left(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_BOSS2_MACHINEGUN_L1], forward, right, start);

	AngleVectors(self->aimangles, forward, NULL, NULL);

	monster_fire_bullet(self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_BOSS2_MACHINEGUN_L1, true);
}

void Boss2MachineGun(edict_t* self)
{
	boss2_firebullet_left(self);
	boss2_firebullet_right(self);
}


mframe_t boss2_frames_stand[] =
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
	{MANIM_STAND, 0, NULL}
};
mmove_t	boss2_move_stand = { FRAME_stand30, FRAME_stand50, boss2_frames_stand, NULL };

mframe_t boss2_frames_fidget[] =
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
	{MANIM_STAND, 0, NULL}
};
mmove_t boss2_move_fidget = { FRAME_stand1, FRAME_stand30, boss2_frames_fidget, NULL };

mframe_t boss2_frames_walk[] =
{
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL},
	{MANIM_WALK,	0,	NULL}
};
mmove_t boss2_move_walk = { FRAME_walk1, FRAME_walk20, boss2_frames_walk, NULL };


mframe_t boss2_frames_run[] =
{
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL},
	{MANIM_RUN,	0,	NULL}
};
mmove_t boss2_move_run = { FRAME_walk1, FRAME_walk20, boss2_frames_run, NULL };

mframe_t boss2_frames_attack_pre_mg[] =
{
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	boss2_attack_mg}
};
mmove_t boss2_move_attack_pre_mg = { FRAME_attack1, FRAME_attack9, boss2_frames_attack_pre_mg, NULL };


// Loop this
mframe_t boss2_frames_attack_mg[] =
{
	{MANIM_MISSILE,	1,	Boss2MachineGun},
	{MANIM_MISSILE,	1,	Boss2MachineGun},
	{MANIM_MISSILE,	1,	Boss2MachineGun},
	{MANIM_MISSILE,	1,	Boss2MachineGun},
	{MANIM_MISSILE,	1,	Boss2MachineGun},
	{MANIM_MISSILE,	1,	boss2_reattack_mg}
};
mmove_t boss2_move_attack_mg = { FRAME_attack10, FRAME_attack15, boss2_frames_attack_mg, NULL };

mframe_t boss2_frames_attack_post_mg[] =
{
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL}
};
mmove_t boss2_move_attack_post_mg = { FRAME_attack16, FRAME_attack19, boss2_frames_attack_post_mg, boss2_run };

mframe_t boss2_frames_attack_rocket[] =
{
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	-20,	Boss2Rocket},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL},
	{MANIM_MISSILE,	1,	NULL}
};
mmove_t boss2_move_attack_rocket = { FRAME_attack20, FRAME_attack40, boss2_frames_attack_rocket, boss2_run };

mframe_t boss2_frames_pain_heavy[] =
{
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL}
};
mmove_t boss2_move_pain_heavy = { FRAME_pain2, FRAME_pain19, boss2_frames_pain_heavy, boss2_run };

mframe_t boss2_frames_pain_light[] =
{
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL}
};
mmove_t boss2_move_pain_light = { FRAME_pain20, FRAME_pain23, boss2_frames_pain_light, boss2_run };

mframe_t boss2_frames_death[] =
{
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	BossExplode}
};
mmove_t boss2_move_death = { FRAME_death2, FRAME_death50, boss2_frames_death, NULL };

void boss2_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &boss2_move_stand;
}

void boss2_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &boss2_move_stand;
	else
		self->monsterinfo.currentmove = &boss2_move_run;
}

void boss2_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &boss2_move_walk;
}

void boss2_attack(edict_t* self)
{
	vec3_t	vec;
	float	range;

	VectorSubtract(self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength(vec);

	if (range <= 125)
	{
		self->pain_debounce_time = level.time + 1;
		self->monsterinfo.currentmove = &boss2_move_attack_pre_mg;
	}
	else
	{
		if (random() <= 0.6)
		{
			self->pain_debounce_time = level.time + 1;
			self->monsterinfo.currentmove = &boss2_move_attack_pre_mg;
		}
		else
		{
			self->pain_debounce_time = level.time + 2.2;
			self->monsterinfo.currentmove = &boss2_move_attack_rocket;
		}
	}
}

void boss2_attack_mg(edict_t* self)
{
	self->pain_debounce_time = level.time + 0.7;
	self->monsterinfo.currentmove = &boss2_move_attack_mg;
}

void boss2_reattack_mg(edict_t* self)
{
	if (infront(self, self->enemy))
		if (random() <= 0.7)
		{
			self->pain_debounce_time = level.time + 0.7;
			self->monsterinfo.currentmove = &boss2_move_attack_mg;
		}
		else
		{
			self->pain_debounce_time = level.time + 0.5;
			self->monsterinfo.currentmove = &boss2_move_attack_post_mg;
		}
	else
	{
		self->pain_debounce_time = level.time + 0.5;
		self->monsterinfo.currentmove = &boss2_move_attack_post_mg;
	}
}


void boss2_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;
	// American wanted these at no attenuation
	if (damage < 10)
	{
		gi.sound(self, CHAN_VOICE, sound_pain3, 1, ATTN_NONE, 0);
		self->monsterinfo.currentmove = &boss2_move_pain_light;
	}
	else if (damage < 30)
	{
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NONE, 0);
		self->monsterinfo.currentmove = &boss2_move_pain_light;
	}
	else
	{
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NONE, 0);
		self->monsterinfo.currentmove = &boss2_move_pain_heavy;
	}
}

void boss2_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	gi.sound(self, CHAN_VOICE, sound_death, 1, ATTN_NONE, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_NO;
	self->count = 0;
	self->monsterinfo.currentmove = &boss2_move_death;
#if 0
	int		n;

	self->s.sound = 0;
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

	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	self->monsterinfo.currentmove = &boss2_move_death;
#endif
}

qboolean Boss2_CheckAttack(edict_t* self)
{
	vec3_t	spot1, spot2;
	vec3_t	temp;
	float	chance;
	trace_t	tr;
	qboolean	is_sliding;
	qboolean	enemy_infront;
	int			enemy_range;
	float		enemy_pitch;
	float		enemy_yaw;

	if (self->monsterinfo.aiflags & AI_DODGING)
		self->monsterinfo.aiflags |= AI_SLIDING;
	else
		self->monsterinfo.aiflags &= ~AI_SLIDING;

	if (self->enemy->health > 0)
	{
		// see if any entities are in the way of the shot
		VectorCopy(self->s.origin, spot1);
		spot1[2] += self->viewheight;
		VectorCopy(self->enemy->s.origin, spot2);
		spot2[2] += self->enemy->viewheight;

		tr = gi.trace(spot1, NULL, NULL, spot2, self, CONTENTS_SOLID | CONTENTS_MONSTER | CONTENTS_SLIME | CONTENTS_LAVA);

		// do we have a clear shot?
		if (tr.ent != self->enemy)
			return false;

		// SLUGFILLER--is the enemy on sight?
		if (!infront(self, self->enemy))
			return false;
	}

	enemy_infront = infront(self, self->enemy);
	enemy_range = range(self, self->enemy);
	VectorSubtract(self->enemy->s.origin, self->s.origin, temp);
	temp[2] += self->enemy->viewheight - self->viewheight;
	enemy_pitch = vectopitch(temp);
	enemy_yaw = vectoyaw(temp);

	self->ideal_pitch = enemy_pitch;
	self->ideal_yaw = enemy_yaw;


	// melee attack
	if (enemy_range == RANGE_MELEE)
	{
		if (self->monsterinfo.melee)
			self->monsterinfo.attack_state = AS_MELEE;
		else
			self->monsterinfo.attack_state = AS_MISSILE;
		return true;
	}

	// missile attack
	if (!self->monsterinfo.attack)
		return false;

	if (level.time < self->monsterinfo.attack_finished)
		return false;

	if (enemy_range == RANGE_FAR)
		return false;

	is_sliding = (random() < 0.5) || (self->monsterinfo.aiflags & AI_DODGING);

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		chance = 0.4;
		is_sliding = false;
	}
	else if (enemy_range == RANGE_MELEE)
	{
		chance = 0.8;
	}
	else if (enemy_range == RANGE_NEAR)
	{
		chance = 0.8;
	}
	else if (enemy_range == RANGE_MID)
	{
		chance = 0.8;
	}
	else
	{
		return false;
	}

	if (random() < chance)
	{
		// TITANIUM--fire
		if (is_sliding)
			self->monsterinfo.aiflags |= AI_SLIDING;
		else
			self->monsterinfo.aiflags &= ~AI_SLIDING;
		self->monsterinfo.attack_state = AS_MISSILE;
		self->monsterinfo.attack_finished = level.time + 2 * random();
		return true;
	}

	// TITANIUM--try to get closer
	if (is_sliding)
		self->monsterinfo.aiflags |= AI_SLIDING;
	else
		self->monsterinfo.aiflags &= ~AI_SLIDING;

	return false;
}



/*QUAKED monster_boss2 (1 .5 0) (-56 -56 0) (56 56 80) Ambush Trigger_Spawn Sight
*/
void SP_monster_boss2(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain1 = gi.soundindex("bosshovr/bhvpain1.wav");
	sound_pain2 = gi.soundindex("bosshovr/bhvpain2.wav");
	sound_pain3 = gi.soundindex("bosshovr/bhvpain3.wav");
	sound_death = gi.soundindex("bosshovr/bhvdeth1.wav");
	sound_search1 = gi.soundindex("bosshovr/bhvunqv1.wav");

	self->s.sound = gi.soundindex("bosshovr/bhvengn1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/boss2/tris.md2");
	VectorSet(self->mins, -56, -56, 0);
	VectorSet(self->maxs, 56, 56, 80);
	VectorSet(self->headmins, 16, -8, 68);
	VectorSet(self->headmaxs, 56, 4, 80);

	self->health = 2000;
	self->gib_health = 2200;
	self->mass = 1000;

	self->flags |= FL_IMMUNE_LASER;

	self->pain = boss2_pain;
	self->die = boss2_die;

	self->monsterinfo.stand = boss2_stand;
	self->monsterinfo.walk = boss2_walk;
	self->monsterinfo.run = boss2_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.attack = boss2_attack;
	self->monsterinfo.search = boss2_search;
	self->monsterinfo.checkattack = Boss2_CheckAttack;

	self->monsterinfo.speed = 8;
	self->monsterinfo.walkspeed = 8;
	gi.linkentity(self);

	self->monsterinfo.currentmove = &boss2_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	flymonster_start(self);
}
