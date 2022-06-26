/*
==============================================================================

SUPERTANK

==============================================================================
*/

#include "g_local.h"
#include "m_supertank.h"

qboolean visible(edict_t* self, edict_t* other);

static int	sound_pain1;
static int	sound_pain2;
static int	sound_pain3;
static int	sound_death;
static int	sound_search1;
static int	sound_search2;

static	int	tread_sound;

void BossExplode(edict_t* self);

void TreadSound(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, tread_sound, 1, ATTN_NORM, 0);
}

void supertank_search(edict_t* self)
{
	if (random() < 0.5)
		gi.sound(self, CHAN_VOICE, sound_search1, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_search2, 1, ATTN_NORM, 0);
}


void supertank_dead(edict_t* self);
void supertankRocket(edict_t* self);
void supertankMachineGun(edict_t* self);
void supertank_reattack1(edict_t* self);


//
// stand
//

mframe_t supertank_frames_stand[] =
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
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL
};
mmove_t	supertank_move_stand = { FRAME_stand_1, FRAME_stand_60, supertank_frames_stand, NULL };

void supertank_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &supertank_move_stand;
}


mframe_t supertank_frames_run[] =
{
	MANIM_RUN, 0,	TreadSound,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, 0,	NULL
};
mmove_t	supertank_move_run = { FRAME_forwrd_1, FRAME_forwrd_18, supertank_frames_run, NULL };

//
// walk
//


mframe_t supertank_frames_forward[] =
{
	MANIM_WALK, 0,	TreadSound,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL
};
mmove_t	supertank_move_forward = { FRAME_forwrd_1, FRAME_forwrd_18, supertank_frames_forward, NULL };

void supertank_forward(edict_t* self)
{
	self->monsterinfo.currentmove = &supertank_move_forward;
}

void supertank_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &supertank_move_forward;
}

void supertank_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &supertank_move_stand;
	else
		self->monsterinfo.currentmove = &supertank_move_run;
}

mframe_t supertank_frames_turn_right[] =
{
	MANIM_MISC,	0,	TreadSound,
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
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t supertank_move_turn_right = { FRAME_right_1, FRAME_right_18, supertank_frames_turn_right, supertank_run };

mframe_t supertank_frames_turn_left[] =
{
	MANIM_MISC,	0,	TreadSound,
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
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t supertank_move_turn_left = { FRAME_left_1, FRAME_left_18, supertank_frames_turn_left, supertank_run };


mframe_t supertank_frames_pain3[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t supertank_move_pain3 = { FRAME_pain3_9, FRAME_pain3_12, supertank_frames_pain3, supertank_run };

mframe_t supertank_frames_pain2[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t supertank_move_pain2 = { FRAME_pain2_5, FRAME_pain2_8, supertank_frames_pain2, supertank_run };

mframe_t supertank_frames_pain1[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t supertank_move_pain1 = { FRAME_pain1_1, FRAME_pain1_4, supertank_frames_pain1, supertank_run };

mframe_t supertank_frames_death1[] =
{
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
	MANIM_DEATH,	0,	BossExplode
};
mmove_t supertank_move_death = { FRAME_death_1, FRAME_death_24, supertank_frames_death1, supertank_dead };

mframe_t supertank_frames_backward[] =
{
	MANIM_WALK, 0,	TreadSound,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, 0,	NULL
};
mmove_t	supertank_move_backward = { FRAME_backwd_1, FRAME_backwd_18, supertank_frames_backward, NULL };

mframe_t supertank_frames_attack4[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t supertank_move_attack4 = { FRAME_attak4_1, FRAME_attak4_6, supertank_frames_attack4, supertank_run };

mframe_t supertank_frames_attack3[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t supertank_move_attack3 = { FRAME_attak3_1, FRAME_attak3_27, supertank_frames_attack3, supertank_run };

mframe_t supertank_frames_attack2[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	supertankRocket,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	supertankRocket,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	supertankRocket,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t supertank_move_attack2 = { FRAME_attak2_1, FRAME_attak2_27, supertank_frames_attack2, supertank_run };

mframe_t supertank_frames_attack1[] =
{
	MANIM_MISSILE,	0,	supertankMachineGun,
	MANIM_MISSILE,	0,	supertankMachineGun,
	MANIM_MISSILE,	0,	supertankMachineGun,
	MANIM_MISSILE,	0,	supertankMachineGun,
	MANIM_MISSILE,	0,	supertankMachineGun,
	MANIM_MISSILE,	0,	supertankMachineGun,

};
mmove_t supertank_move_attack1 = { FRAME_attak1_1, FRAME_attak1_6, supertank_frames_attack1, supertank_reattack1 };

mframe_t supertank_frames_end_attack1[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t supertank_move_end_attack1 = { FRAME_attak1_7, FRAME_attak1_20, supertank_frames_end_attack1, supertank_run };


void supertank_reattack1(edict_t* self)
{
	if (visible(self, self->enemy))
		if (random() < 0.9)
		{
			self->pain_debounce_time = level.time + 0.7;
			self->monsterinfo.currentmove = &supertank_move_attack1;
		}
		else
		{
			self->pain_debounce_time = level.time + 1.5;
			self->monsterinfo.currentmove = &supertank_move_end_attack1;
		}
	else
	{
		self->pain_debounce_time = level.time + 1.5;
		self->monsterinfo.currentmove = &supertank_move_end_attack1;
	}
}

void supertank_pain(edict_t* self, edict_t* other, float kick, int damage)
{

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	// Lessen the chance of him going into his pain frames
	if (damage <= 25)
		if (random() < 0.2)
			return;

	// Don't go into pain if he's firing his rockets
	if (skill->value >= 2)
		if ((self->s.frame >= FRAME_attak2_1) && (self->s.frame <= FRAME_attak2_14))
			return;

	self->pain_debounce_time = level.time + PAINTIME;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 10)
	{
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &supertank_move_pain1;
	}
	else if (damage <= 25)
	{
		gi.sound(self, CHAN_VOICE, sound_pain3, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &supertank_move_pain2;
	}
	else
	{
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &supertank_move_pain3;
	}
};


void supertankRocket(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;
	int		flash_number;

	if (self->s.frame == FRAME_attak2_8)
		flash_number = MZ2_SUPERTANK_ROCKET_1;
	else if (self->s.frame == FRAME_attak2_11)
		flash_number = MZ2_SUPERTANK_ROCKET_2;
	else // (self->s.frame == FRAME_attak2_14)
		flash_number = MZ2_SUPERTANK_ROCKET_3;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);

	monster_fire_rocket(self, start, dir, 100, 650, flash_number);
}

void supertankMachineGun(edict_t* self)
{
	vec3_t	start;
	vec3_t	forward, right;
	int		flash_number;

	flash_number = MZ2_SUPERTANK_MACHINEGUN_1 + (self->s.frame - FRAME_attak1_1);

	//FIXME!!! SLUGFILLER--FIXEDYA!

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	AngleVectors(self->aimangles, forward, NULL, NULL);

	monster_fire_bullet(self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_number, false);
}


void supertank_attack(edict_t* self)
{
	vec3_t	vec;
	float	range;
	//float	r;

	VectorSubtract(self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength(vec);

	//r = random();

	// Attack 1 == Chaingun
	// Attack 2 == Rocket Launcher

	if (range <= 160)
	{
		self->pain_debounce_time = level.time + 2.8;
		self->monsterinfo.currentmove = &supertank_move_attack1;
	}
	else
	{	// fire rockets more often at distance
		if (random() < 0.3)
		{
			self->pain_debounce_time = level.time + 2.8;
			self->monsterinfo.currentmove = &supertank_move_attack1;
		}
		else
		{
			self->pain_debounce_time = level.time + 2.8;
			self->monsterinfo.currentmove = &supertank_move_attack2;
		}
	}
}


//
// death
//

void supertank_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -60, -60, 0);
	VectorSet(self->maxs, 60, 60, 72);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity(self);
}


void BossExplode(edict_t* self)
{
	vec3_t	org;

	self->think = BossExplode;
	VectorCopy(self->s.origin, org);
	org[2] += 24 + (rand() & 15);
	switch (self->count++)
	{
	case 0:
		org[0] -= 24;
		org[1] -= 24;
		break;
	case 1:
		org[0] += 24;
		org[1] += 24;
		break;
	case 2:
		org[0] += 24;
		org[1] -= 24;
		break;
	case 3:
		org[0] -= 24;
		org[1] += 24;
		break;
	case 4:
		org[0] -= 48;
		org[1] -= 48;
		break;
	case 5:
		org[0] += 48;
		org[1] += 48;
		break;
	case 6:
		org[0] -= 48;
		org[1] += 48;
		break;
	case 7:
		org[0] += 48;
		org[1] -= 48;
		break;
	case 8:
		self->s.sound = 0;
		// SLUGFILLER--no reason for the boss to get smashed to bits
		self->solid = SOLID_NOT;
		self->deadflag = DEAD_DEAD;
		if (self->s.modelindex2)
		{
			// SLUGFILLER--makron has left the jorg
			self->s.modelindex = self->s.modelindex2;
			self->s.modelindex2 = 0;
		}
		return;
	}

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_EXPLOSION1);
	gi.WritePosition(org);
	gi.multicast(self->s.origin, MULTICAST_PVS);

	self->nextthink = level.time + 0.1;
}


void supertank_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	gi.sound(self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_NO;
	self->count = 0;
	self->monsterinfo.currentmove = &supertank_move_death;
}

//
// monster_supertank
//

/*QUAKED monster_supertank (1 .5 0) (-64 -64 0) (64 64 72) Ambush Trigger_Spawn Sight
*/
void SP_monster_supertank(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain1 = gi.soundindex("bosstank/btkpain1.wav");
	sound_pain2 = gi.soundindex("bosstank/btkpain2.wav");
	sound_pain3 = gi.soundindex("bosstank/btkpain3.wav");
	sound_death = gi.soundindex("bosstank/btkdeth1.wav");
	sound_search1 = gi.soundindex("bosstank/btkunqv1.wav");
	sound_search2 = gi.soundindex("bosstank/btkunqv2.wav");

	//	self->s.sound = gi.soundindex ("bosstank/btkengn1.wav");
	tread_sound = gi.soundindex("bosstank/btkengn1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/boss1/tris.md2");
	VectorSet(self->mins, -64, -64, 0);
	VectorSet(self->maxs, 64, 64, 112);
	VectorSet(self->headmins, -64, -64, 90);
	VectorSet(self->headmaxs, 64, 64, 112);

	self->health = 1500;
	self->gib_health = 2000;
	self->mass = 800;

	self->pain = supertank_pain;
	self->die = supertank_die;
	self->monsterinfo.stand = supertank_stand;
	self->monsterinfo.walk = supertank_walk;
	self->monsterinfo.run = supertank_run;
	self->monsterinfo.attack = supertank_attack;
	self->monsterinfo.search = supertank_search;

	self->monsterinfo.speed = 12;
	self->monsterinfo.walkspeed = 4;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &supertank_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
