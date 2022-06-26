/*
==============================================================================

jorg

==============================================================================
*/

#include "g_local.h"
#include "m_boss31.h"

extern void SP_monster_makron(edict_t* self);
qboolean visible(edict_t* self, edict_t* other);

static int	sound_pain1;
static int	sound_pain2;
static int	sound_pain3;
static int	sound_idle;
static int	sound_death;
static int	sound_search1;
static int	sound_search2;
static int	sound_search3;
static int	sound_attack1;
static int	sound_attack2;
static int	sound_firegun;
static int	sound_step_left;
static int	sound_step_right;
static int	sound_death_hit;

void BossExplode(edict_t* self);
void MakronToss(edict_t* self);


void jorg_search(edict_t* self)
{
	float r;

	r = random();

	if (r <= 0.3)
		gi.sound(self, CHAN_VOICE, sound_search1, 1, ATTN_NORM, 0);
	else if (r <= 0.6)
		gi.sound(self, CHAN_VOICE, sound_search2, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_search3, 1, ATTN_NORM, 0);
}


void jorg_dead(edict_t* self);
void jorgBFG(edict_t* self);
void jorgMachineGun(edict_t* self);
void jorg_firebullet(edict_t* self);
void jorg_reattack1(edict_t* self);
void jorg_attack1(edict_t* self);
void jorg_idle(edict_t* self);
void jorg_step_left(edict_t* self);
void jorg_step_right(edict_t* self);
void jorg_death_hit(edict_t* self);

//
// stand
//

mframe_t jorg_frames_stand[] =
{
	MANIM_STAND, 0, jorg_idle,
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
	MANIM_STAND, 19, NULL,
	MANIM_STAND, 11, jorg_step_left,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 6, NULL,
	MANIM_STAND, 9, jorg_step_right,
	MANIM_STAND, 0, NULL,		// 40
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, -2, NULL,
	MANIM_STAND, -17, jorg_step_left,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, -12, NULL,		// 50
	MANIM_STAND, -14, jorg_step_right	// 51
};
mmove_t	jorg_move_stand = { FRAME_stand01, FRAME_stand51, jorg_frames_stand, NULL };

void jorg_idle(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_idle, 1, ATTN_NORM, 0);
}

void jorg_death_hit(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_death_hit, 1, ATTN_NORM, 0);
}


void jorg_step_left(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_step_left, 1, ATTN_NORM, 0);
}

void jorg_step_right(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_step_right, 1, ATTN_NORM, 0);
}


void jorg_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &jorg_move_stand;
}

mframe_t jorg_frames_run[] =
{
	MANIM_RUN, 5,	jorg_step_left,
	MANIM_RUN, -12,	NULL,
	MANIM_RUN, -12,	NULL,
	MANIM_RUN, -12,	NULL,
	MANIM_RUN, 0,	NULL,
	MANIM_RUN, -4,	NULL,
	MANIM_RUN, -2,	NULL,
	MANIM_RUN, 21,	jorg_step_right,
	MANIM_RUN, -12,	NULL,
	MANIM_RUN, -12,	NULL,
	MANIM_RUN, -12,	NULL,
	MANIM_RUN, -3,	NULL,
	MANIM_RUN, -3,	NULL,
	MANIM_RUN, -3,	NULL
};
mmove_t	jorg_move_run = { FRAME_walk06, FRAME_walk19, jorg_frames_run, NULL };

//
// walk
//

mframe_t jorg_frames_start_walk[] =
{
	MANIM_WALK,	-7,	NULL,
	MANIM_WALK,	-6,	NULL,
	MANIM_WALK,	-5,	NULL,
	MANIM_WALK,	-3,	NULL,
	MANIM_WALK,	3,	NULL
};
mmove_t jorg_move_start_walk = { FRAME_walk01, FRAME_walk05, jorg_frames_start_walk, NULL };

mframe_t jorg_frames_walk[] =
{
	MANIM_WALK, 5,	NULL,
	MANIM_WALK, -12,	NULL,
	MANIM_WALK, -12,	NULL,
	MANIM_WALK, -12,	NULL,
	MANIM_WALK, 0,	NULL,
	MANIM_WALK, -4,	NULL,
	MANIM_WALK, -2,	NULL,
	MANIM_WALK, 21,	NULL,
	MANIM_WALK, -12,	NULL,
	MANIM_WALK, -12,	NULL,
	MANIM_WALK, -12,	NULL,
	MANIM_WALK, -3,	NULL,
	MANIM_WALK, -3,	NULL,
	MANIM_WALK, -3,	NULL
};
mmove_t	jorg_move_walk = { FRAME_walk06, FRAME_walk19, jorg_frames_walk, NULL };

mframe_t jorg_frames_end_walk[] =
{
	MANIM_WALK,	-1,	NULL,
	MANIM_WALK,	-12,	NULL,
	MANIM_WALK,	-12,	NULL,
	MANIM_WALK,	-12,	NULL,
	MANIM_WALK,	-4,	NULL,
	MANIM_WALK,	-20,	NULL
};
mmove_t jorg_move_end_walk = { FRAME_walk20, FRAME_walk25, jorg_frames_end_walk, NULL };

void jorg_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &jorg_move_walk;
}

void jorg_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &jorg_move_stand;
	else
		self->monsterinfo.currentmove = &jorg_move_run;
}

mframe_t jorg_frames_pain3[] =
{
	MANIM_MISC,	-28,	NULL,
	MANIM_MISC,	-6,	NULL,
	MANIM_MISC,	-3,	jorg_step_left,
	MANIM_MISC,	-9,	NULL,
	MANIM_MISC,	0,	jorg_step_right,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	-7,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	-11,	NULL,
	MANIM_MISC,	-4,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	10,	NULL,
	MANIM_MISC,	11,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	10,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	10,	NULL,
	MANIM_MISC,	7,	jorg_step_left,
	MANIM_MISC,	17,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	jorg_step_right
};
mmove_t jorg_move_pain3 = { FRAME_pain301, FRAME_pain325, jorg_frames_pain3, jorg_run };

mframe_t jorg_frames_pain2[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t jorg_move_pain2 = { FRAME_pain201, FRAME_pain203, jorg_frames_pain2, jorg_run };

mframe_t jorg_frames_pain1[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL
};
mmove_t jorg_move_pain1 = { FRAME_pain101, FRAME_pain103, jorg_frames_pain1, jorg_run };

mframe_t jorg_frames_death1[] =
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
	MANIM_DEATH,	0,	NULL,		// 10
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,		// 20
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,		// 30
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,		// 40
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	MakronToss,
	MANIM_DEATH,	0,	BossExplode		// 50
};
mmove_t jorg_move_death = { FRAME_death01, FRAME_death50, jorg_frames_death1, jorg_dead };

mframe_t jorg_frames_attack2[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	jorgBFG,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t jorg_move_attack2 = { FRAME_attak201, FRAME_attak213, jorg_frames_attack2, jorg_run };

mframe_t jorg_frames_start_attack1[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t jorg_move_start_attack1 = { FRAME_attak101, FRAME_attak108, jorg_frames_start_attack1, jorg_attack1 };

mframe_t jorg_frames_attack1[] =
{
	MANIM_MISSILE,	0,	jorg_firebullet,
	MANIM_MISSILE,	0,	jorg_firebullet,
	MANIM_MISSILE,	0,	jorg_firebullet,
	MANIM_MISSILE,	0,	jorg_firebullet,
	MANIM_MISSILE,	0,	jorg_firebullet,
	MANIM_MISSILE,	0,	jorg_firebullet
};
mmove_t jorg_move_attack1 = { FRAME_attak109, FRAME_attak114, jorg_frames_attack1, jorg_reattack1 };

mframe_t jorg_frames_end_attack1[] =
{
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL,
	MANIM_MISSILE,	0,	NULL
};
mmove_t jorg_move_end_attack1 = { FRAME_attak115, FRAME_attak118, jorg_frames_end_attack1, jorg_run };

void jorg_reattack1(edict_t* self)
{
	if (visible(self, self->enemy))
		if (random() < 0.9)
		{
			self->pain_debounce_time = level.time + 0.7;
			self->monsterinfo.currentmove = &jorg_move_attack1;
		}
		else
		{
			self->s.sound = 0;
			self->pain_debounce_time = level.time + 0.5;
			self->monsterinfo.currentmove = &jorg_move_end_attack1;
		}
	else
	{
		self->s.sound = 0;
		self->pain_debounce_time = level.time + 0.5;
		self->monsterinfo.currentmove = &jorg_move_end_attack1;
	}
}

void jorg_attack1(edict_t* self)
{
	self->pain_debounce_time = level.time + 0.7;
	self->monsterinfo.currentmove = &jorg_move_attack1;
}

void jorg_pain(edict_t* self, edict_t* other, float kick, int damage)
{

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	self->s.sound = 0;

	if (level.time < self->pain_debounce_time)
		return;

	// Lessen the chance of him going into his pain frames if he takes little damage
	if (damage <= 40)
		if (random() <= 0.6)
			return;

	/*
	If he's entering his attack1 or using attack1, lessen the chance of him
	going into pain
	*/

	if ((self->s.frame >= FRAME_attak101) && (self->s.frame <= FRAME_attak108))
		if (random() <= 0.005)
			return;

	if ((self->s.frame >= FRAME_attak109) && (self->s.frame <= FRAME_attak114))
		if (random() <= 0.00005)
			return;


	if ((self->s.frame >= FRAME_attak201) && (self->s.frame <= FRAME_attak208))
		if (random() <= 0.005)
			return;


	self->pain_debounce_time = level.time + PAINTIME;
	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 50)
	{
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &jorg_move_pain1;
	}
	else if (damage <= 100)
	{
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &jorg_move_pain2;
	}
	else
	{
		if (random() <= 0.3)
		{
			gi.sound(self, CHAN_VOICE, sound_pain3, 1, ATTN_NORM, 0);
			self->monsterinfo.currentmove = &jorg_move_pain3;
		}
	}
};

void jorgBFG(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_JORG_BFG_1], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);
	gi.sound(self, CHAN_VOICE, sound_attack2, 1, ATTN_NORM, 0);
	monster_fire_bfg(self, start, dir, 500, 400, 100, 1000, MZ2_JORG_BFG_1);
}

void jorg_firebullet_right(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_JORG_MACHINEGUN_R1], forward, right, start);

	AngleVectors(self->aimangles, forward, NULL, NULL);

	monster_fire_bullet(self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_JORG_MACHINEGUN_R1, true);
}

void jorg_firebullet_left(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_JORG_MACHINEGUN_L1], forward, right, start);

	AngleVectors(self->aimangles, forward, NULL, NULL);

	monster_fire_bullet(self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_JORG_MACHINEGUN_L1, true);
}

void jorg_firebullet(edict_t* self)
{
	jorg_firebullet_left(self);
	jorg_firebullet_right(self);
};

void jorg_attack(edict_t* self)
{
	vec3_t	vec;
	float	range;

	VectorSubtract(self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength(vec);

	if (random() <= 0.75)
	{
		gi.sound(self, CHAN_VOICE, sound_attack1, 1, ATTN_NORM, 0);
		self->s.sound = gi.soundindex("boss3/w_loop.wav");
		self->pain_debounce_time = level.time + 0.9;
		self->monsterinfo.currentmove = &jorg_move_start_attack1;
	}
	else
	{
		gi.sound(self, CHAN_VOICE, sound_attack2, 1, ATTN_NORM, 0);
		self->pain_debounce_time = level.time + 1.4;
		self->monsterinfo.currentmove = &jorg_move_attack2;
	}
}

void jorg_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;
}


void jorg_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	gi.sound(self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_NO;
	self->s.sound = 0;
	self->count = 0;
	self->monsterinfo.currentmove = &jorg_move_death;
}

qboolean Jorg_CheckAttack(edict_t* self)
{
	vec3_t	spot1, spot2;
	vec3_t	temp;
	float	chance;
	trace_t	tr;
	qboolean	enemy_infront;
	int			enemy_range;
	float		enemy_pitch;
	float		enemy_yaw;

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

		// TITANIUM--is the enemy on sight?
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

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		chance = 0.4;
	}
	else if (enemy_range == RANGE_MELEE)
	{
		chance = 0.8;
	}
	else if (enemy_range == RANGE_NEAR)
	{
		chance = 0.4;
	}
	else if (enemy_range == RANGE_MID)
	{
		chance = 0.2;
	}
	else
	{
		return false;
	}

	if (random() < chance)
	{
		self->monsterinfo.attack_state = AS_MISSILE;
		self->monsterinfo.attack_finished = level.time + 2 * random();
		return true;
	}

	if (self->flags & FL_FLY)
	{
		if (random() < 0.3)
			self->monsterinfo.aiflags |= AI_SLIDING;
		else
			self->monsterinfo.aiflags &= ~AI_SLIDING;
	}

	return false;
}


void MakronPrecache(void);

/*QUAKED monster_jorg (1 .5 0) (-80 -80 0) (90 90 140) Ambush Trigger_Spawn Sight
*/
void SP_monster_jorg(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain1 = gi.soundindex("boss3/bs3pain1.wav");
	sound_pain2 = gi.soundindex("boss3/bs3pain2.wav");
	sound_pain3 = gi.soundindex("boss3/bs3pain3.wav");
	sound_death = gi.soundindex("boss3/bs3deth1.wav");
	sound_attack1 = gi.soundindex("boss3/bs3atck1.wav");
	sound_attack2 = gi.soundindex("boss3/bs3atck2.wav");
	sound_search1 = gi.soundindex("boss3/bs3srch1.wav");
	sound_search2 = gi.soundindex("boss3/bs3srch2.wav");
	sound_search3 = gi.soundindex("boss3/bs3srch3.wav");
	sound_idle = gi.soundindex("boss3/bs3idle1.wav");
	sound_step_left = gi.soundindex("boss3/step1.wav");
	sound_step_right = gi.soundindex("boss3/step2.wav");
	sound_firegun = gi.soundindex("boss3/xfire.wav");
	sound_death_hit = gi.soundindex("boss3/d_hit.wav");

	MakronPrecache();

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/boss3/rider/tris.md2");
	self->s.modelindex2 = gi.modelindex("models/monsters/boss3/jorg/tris.md2");
	VectorSet(self->mins, -80, -80, 0);
	VectorSet(self->maxs, 80, 80, 140);
	VectorSet(self->headmins, 10, -80, 116);
	VectorSet(self->headmaxs, 80, 80, 140);

	self->health = 3000;
	self->gib_health = 5000;
	self->mass = 1000;

	self->pain = jorg_pain;
	self->die = jorg_die;
	self->monsterinfo.stand = jorg_stand;
	self->monsterinfo.walk = jorg_walk;
	self->monsterinfo.run = jorg_run;
	self->monsterinfo.attack = jorg_attack;
	self->monsterinfo.search = jorg_search;
	self->monsterinfo.checkattack = Jorg_CheckAttack;

	self->monsterinfo.speed = 12;
	self->monsterinfo.walkspeed = 12;
	gi.linkentity(self);

	self->monsterinfo.currentmove = &jorg_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
