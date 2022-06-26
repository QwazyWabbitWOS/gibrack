/*
==============================================================================

hover

==============================================================================
*/

#include "g_local.h"
#include "m_hover.h"

qboolean visible(edict_t* self, edict_t* other);


static int	sound_pain1;
static int	sound_pain2;
static int	sound_death1;
static int	sound_death2;
static int	sound_sight;
static int	sound_search1;
static int	sound_search2;


void hover_sight(edict_t* self, edict_t* other)
{
	gi.sound(self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void hover_search(edict_t* self)
{
	if (random() < 0.5)
		gi.sound(self, CHAN_VOICE, sound_search1, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_search2, 1, ATTN_NORM, 0);
}


void hover_run(edict_t* self);
void hover_stand(edict_t* self);
void hover_dead(edict_t* self);
void hover_attack(edict_t* self);
void hover_reattack(edict_t* self);
void hover_fire_blaster(edict_t* self);
void hover_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point);

mframe_t hover_frames_stand[] =
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
mmove_t	hover_move_stand = { FRAME_stand01, FRAME_stand30, hover_frames_stand, NULL };

mframe_t hover_frames_stop1[] =
{
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
mmove_t hover_move_stop1 = { FRAME_stop101, FRAME_stop109, hover_frames_stop1, NULL };

mframe_t hover_frames_stop2[] =
{
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL
};
mmove_t hover_move_stop2 = { FRAME_stop201, FRAME_stop208, hover_frames_stop2, NULL };

mframe_t hover_frames_takeoff[] =
{
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	-2,	NULL,
	MANIM_STAND,	5,	NULL,
	MANIM_STAND,	-1,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	-1,	NULL,
	MANIM_STAND,	-1,	NULL,
	MANIM_STAND,	-1,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	-6,	NULL,
	MANIM_STAND,	-9,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	1,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	3,	NULL,
	MANIM_STAND,	2,	NULL,
	MANIM_STAND,	0,	NULL
};
mmove_t hover_move_takeoff = { FRAME_takeof01, FRAME_takeof30, hover_frames_takeoff, NULL };

mframe_t hover_frames_pain3[] =
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
mmove_t hover_move_pain3 = { FRAME_pain301, FRAME_pain309, hover_frames_pain3, hover_run };

mframe_t hover_frames_pain2[] =
{
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
mmove_t hover_move_pain2 = { FRAME_pain201, FRAME_pain212, hover_frames_pain2, hover_run };

mframe_t hover_frames_pain1[] =
{
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	-8,	NULL,
	MANIM_MISC,	-4,	NULL,
	MANIM_MISC,	-6,	NULL,
	MANIM_MISC,	-4,	NULL,
	MANIM_MISC,	-3,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	7,	NULL,
	MANIM_MISC,	1,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	2,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	0,	NULL,
	MANIM_MISC,	5,	NULL,
	MANIM_MISC,	3,	NULL,
	MANIM_MISC,	4,	NULL
};
mmove_t hover_move_pain1 = { FRAME_pain101, FRAME_pain128, hover_frames_pain1, hover_run };

mframe_t hover_frames_land[] =
{
	MANIM_STAND,	0,	NULL
};
mmove_t hover_move_land = { FRAME_land01, FRAME_land01, hover_frames_land, NULL };

mframe_t hover_frames_forward[] =
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
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL
};
mmove_t hover_move_forward = { FRAME_forwrd01, FRAME_forwrd35, hover_frames_forward, NULL };

mframe_t hover_frames_walk[] =
{
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL,
	MANIM_WALK,	0,	NULL
};
mmove_t hover_move_walk = { FRAME_forwrd01, FRAME_forwrd35, hover_frames_walk, NULL };

mframe_t hover_frames_run[] =
{
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL,
	MANIM_RUN,	0,	NULL
};
mmove_t hover_move_run = { FRAME_forwrd01, FRAME_forwrd35, hover_frames_run, NULL };

mframe_t hover_frames_death1[] =
{
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	0,	NULL,
	MANIM_DEATH,	-10,NULL,
	MANIM_DEATH,	3,	NULL,
	MANIM_DEATH,	5,	NULL,
	MANIM_DEATH,	4,	NULL,
	MANIM_DEATH,	7,	NULL
};
mmove_t hover_move_death1 = { FRAME_death101, FRAME_death111, hover_frames_death1, hover_dead };

mframe_t hover_frames_backward[] =
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
	MANIM_STAND,	0,	NULL
};
mmove_t hover_move_backward = { FRAME_backwd01, FRAME_backwd24, hover_frames_backward, NULL };

mframe_t hover_frames_start_attack[] =
{
	MANIM_MISSILE,	1,	NULL,
	MANIM_MISSILE,	1,	NULL,
	MANIM_MISSILE,	1,	NULL
};
mmove_t hover_move_start_attack = { FRAME_attak101, FRAME_attak103, hover_frames_start_attack, hover_attack };

mframe_t hover_frames_attack1[] =
{
	MANIM_MISSILE,	-10,	hover_fire_blaster,
	MANIM_MISSILE,	-10,	hover_fire_blaster,
	MANIM_MISSILE,	0,		hover_reattack,
};
mmove_t hover_move_attack1 = { FRAME_attak104, FRAME_attak106, hover_frames_attack1, NULL };


mframe_t hover_frames_end_attack[] =
{
	MANIM_MISSILE,	1,	NULL,
	MANIM_MISSILE,	1,	NULL
};
mmove_t hover_move_end_attack = { FRAME_attak107, FRAME_attak108, hover_frames_end_attack, hover_run };

void hover_reattack(edict_t* self)
{
	if (self->enemy->health > 0)
		if (visible(self, self->enemy))
			if (random() <= 0.6)
			{
				self->pain_debounce_time = level.time + 0.4;
				self->monsterinfo.currentmove = &hover_move_attack1;
				return;
			}
	self->pain_debounce_time = level.time + 0.3;
	self->monsterinfo.currentmove = &hover_move_end_attack;
}


void hover_fire_blaster(edict_t* self)
{
	vec3_t	start;
	vec3_t	forward, right;
	vec3_t	dir;
	int		effect;

	if (self->s.frame == FRAME_attak104)
		effect = EF_HYPERBLASTER;
	else
		effect = 0;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_HOVER_BLASTER_1], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);

	monster_fire_blaster(self, start, dir, 5, 1000, MZ2_HOVER_BLASTER_1, effect, true);
}


void hover_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &hover_move_stand;
}

void hover_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &hover_move_stand;
	else
		self->monsterinfo.currentmove = &hover_move_run;
}

void hover_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &hover_move_walk;
}

void hover_start_attack(edict_t* self)
{
	self->pain_debounce_time = level.time + 0.4;
	self->monsterinfo.currentmove = &hover_move_start_attack;
}

void hover_attack(edict_t* self)
{
	self->pain_debounce_time = level.time + 0.4;
	self->monsterinfo.currentmove = &hover_move_attack1;
}


void hover_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 25)
	{
		if (random() < 0.5)
		{
			gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
			self->monsterinfo.currentmove = &hover_move_pain3;
		}
		else
		{
			gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
			self->monsterinfo.currentmove = &hover_move_pain2;
		}
	}
	else
	{
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &hover_move_pain1;
	}
}

void hover_deadthink(edict_t* self)
{
	if (!self->groundentity && level.time < self->timestamp)
	{
		self->nextthink = level.time + FRAMETIME;
		return;
	}
	BecomeExplosion1(self);
}

void hover_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->think = hover_deadthink;
	self->nextthink = level.time + FRAMETIME;
	self->timestamp = level.time + 15;
	gi.linkentity(self);
}

void hover_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
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
		for (n = 0; n < 2; n++)
			ThrowGib(self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowHead(self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	if (random() < 0.5)
		gi.sound(self, CHAN_VOICE, sound_death1, 1, ATTN_NORM, 0);
	else
		gi.sound(self, CHAN_VOICE, sound_death2, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	self->monsterinfo.currentmove = &hover_move_death1;
}

/*QUAKED monster_hover (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_hover(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_pain1 = gi.soundindex("hover/hovpain1.wav");
	sound_pain2 = gi.soundindex("hover/hovpain2.wav");
	sound_death1 = gi.soundindex("hover/hovdeth1.wav");
	sound_death2 = gi.soundindex("hover/hovdeth2.wav");
	sound_sight = gi.soundindex("hover/hovsght1.wav");
	sound_search1 = gi.soundindex("hover/hovsrch1.wav");
	sound_search2 = gi.soundindex("hover/hovsrch2.wav");

	gi.soundindex("hover/hovatck1.wav");

	self->s.sound = gi.soundindex("hover/hovidle1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/hover/tris.md2");
	VectorSet(self->mins, -24, -24, -24);
	VectorSet(self->maxs, 24, 24, 32);
	VectorSet(self->headmins, 5, -5, 16);
	VectorSet(self->headmaxs, 24, 5, 32);

	self->health = 240;
	self->gib_health = 340;
	self->mass = 150;

	self->pain = hover_pain;
	self->die = hover_die;

	self->monsterinfo.stand = hover_stand;
	self->monsterinfo.walk = hover_walk;
	self->monsterinfo.run = hover_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.attack = hover_start_attack;
	self->monsterinfo.sight = hover_sight;
	self->monsterinfo.search = hover_search;

	self->monsterinfo.speed = 10;
	self->monsterinfo.walkspeed = 4;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &hover_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	flymonster_start(self);
}
