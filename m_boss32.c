/*
==============================================================================

Makron -- Final Boss

==============================================================================
*/

#include "g_local.h"
#include "m_boss32.h"

qboolean visible(edict_t* self, edict_t* other);

void MakronRailgun(edict_t* self);
void MakronSaveloc(edict_t* self);
void MakronHyperblaster(edict_t* self);
void makron_step_left(edict_t* self);
void makron_step_right(edict_t* self);
void makronBFG(edict_t* self);
void makron_dead(edict_t* self);

static int	sound_pain4;
static int	sound_pain5;
static int	sound_pain6;
static int	sound_death;
static int	sound_step_left;
static int	sound_step_right;
static int	sound_attack_bfg;
static int	sound_brainsplorch;
static int	sound_prerailgun;
static int	sound_popup;
static int	sound_taunt1;
static int	sound_taunt2;
static int	sound_taunt3;
static int	sound_hit;

void makron_taunt(edict_t* self)
{
	float r;

	r = random();
	if (r <= 0.3)
		gi.sound(self, CHAN_AUTO, sound_taunt1, 1, ATTN_NONE, 0);
	else if (r <= 0.6)
		gi.sound(self, CHAN_AUTO, sound_taunt2, 1, ATTN_NONE, 0);
	else
		gi.sound(self, CHAN_AUTO, sound_taunt3, 1, ATTN_NONE, 0);
}

//
// stand
//

mframe_t makron_frames_stand[] =
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
	{MANIM_STAND, 0, NULL},		// 10
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},		// 20
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},		// 30
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},		// 40
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},		// 50
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL},
	{MANIM_STAND, 0, NULL}		// 60
};
mmove_t	makron_move_stand = { FRAME_stand201, FRAME_stand260, makron_frames_stand, NULL };

void makron_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &makron_move_stand;
}

mframe_t makron_frames_run[] =
{
	{MANIM_RUN, -6,	makron_step_left},
	{MANIM_RUN, 3,	NULL},
	{MANIM_RUN, -1,	NULL},
	{MANIM_RUN, -1,	NULL},
	{MANIM_RUN, -1,	makron_step_right},
	{MANIM_RUN, -3,	NULL},
	{MANIM_RUN, 3,	NULL},
	{MANIM_RUN, 0,	NULL},
	{MANIM_RUN, -3,	NULL},
	{MANIM_RUN, 3,	NULL}
};
mmove_t	makron_move_run = { FRAME_walk204, FRAME_walk213, makron_frames_run, NULL };

void makron_hit(edict_t* self)
{
	gi.sound(self, CHAN_AUTO, sound_hit, 1, ATTN_NONE, 0);
}

void makron_popup(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_popup, 1, ATTN_NONE, 0);
}

void makron_step_left(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_step_left, 1, ATTN_NORM, 0);
}

void makron_step_right(edict_t* self)
{
	gi.sound(self, CHAN_BODY, sound_step_right, 1, ATTN_NORM, 0);
}

void makron_brainsplorch(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_brainsplorch, 1, ATTN_NORM, 0);
}

void makron_prerailgun(edict_t* self)
{
	gi.sound(self, CHAN_WEAPON, sound_prerailgun, 1, ATTN_NORM, 0);
}


mframe_t makron_frames_walk[] =
{
	{MANIM_WALK, -6,	makron_step_left},
	{MANIM_WALK, 3,	NULL},
	{MANIM_WALK, -1,	NULL},
	{MANIM_WALK, -1,	NULL},
	{MANIM_WALK, -1,	makron_step_right},
	{MANIM_WALK, -3,	NULL},
	{MANIM_WALK, 3,	NULL},
	{MANIM_WALK, 0,	NULL},
	{MANIM_WALK, -3,	NULL},
	{MANIM_WALK, 3,	NULL}
};
mmove_t	makron_move_walk = { FRAME_walk204, FRAME_walk213, makron_frames_run, NULL };

void makron_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &makron_move_walk;
}

void makron_run(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &makron_move_stand;
	else
		self->monsterinfo.currentmove = &makron_move_run;
}

mframe_t makron_frames_pain6[] =
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
	{MANIM_MISC,	0,	NULL},		// 10
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	makron_popup},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},		// 20
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	makron_taunt},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL}
};
mmove_t makron_move_pain6 = { FRAME_pain601, FRAME_pain627, makron_frames_pain6, makron_run };

mframe_t makron_frames_pain5[] =
{
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL}
};
mmove_t makron_move_pain5 = { FRAME_pain501, FRAME_pain504, makron_frames_pain5, makron_run };

mframe_t makron_frames_pain4[] =
{
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL},
	{MANIM_MISC,	0,	NULL}
};
mmove_t makron_move_pain4 = { FRAME_pain401, FRAME_pain404, makron_frames_pain4, makron_run };

mframe_t makron_frames_death2[] =
{
	{MANIM_DEATH,	-15,	NULL},
	{MANIM_DEATH,	3,	NULL},
	{MANIM_DEATH,	-12,	NULL},
	{MANIM_DEATH,	0,	makron_step_left},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 10
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	11,	NULL},
	{MANIM_DEATH,	12,	NULL},
	{MANIM_DEATH,	11,	makron_step_right},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 20
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 30
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	5,	NULL},
	{MANIM_DEATH,	7,	NULL},
	{MANIM_DEATH,	6,	makron_step_left},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	-1,	NULL},
	{MANIM_DEATH,	2,	NULL},			// 40
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 50
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	-6,	NULL},
	{MANIM_DEATH,	-4,	NULL},
	{MANIM_DEATH,	-6,	makron_step_right},
	{MANIM_DEATH,	-4,	NULL},
	{MANIM_DEATH,	-4,	makron_step_left},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 60
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	-2,	NULL},
	{MANIM_DEATH,	-5,	NULL},
	{MANIM_DEATH,	-3,	makron_step_right},
	{MANIM_DEATH,	-8,	NULL},
	{MANIM_DEATH,	-3,	makron_step_left},
	{MANIM_DEATH,	-7,	NULL},
	{MANIM_DEATH,	-4,	NULL},
	{MANIM_DEATH,	-4,	makron_step_right},			// 70
	{MANIM_DEATH,	-6,	NULL},
	{MANIM_DEATH,	-7,	NULL},
	{MANIM_DEATH,	0,	makron_step_left},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 80
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	-2,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	2,	NULL},
	{MANIM_DEATH,	0,	NULL},			// 90
	{MANIM_DEATH,	27,	makron_hit},
	{MANIM_DEATH,	26,	NULL},
	{MANIM_DEATH,	0,	makron_brainsplorch},
	{MANIM_DEATH,	0,	NULL},
	{MANIM_DEATH,	0,	NULL}			// 95
};
mmove_t makron_move_death2 = { FRAME_death201, FRAME_death295, makron_frames_death2, makron_dead };

mframe_t makron_frames_death3[] =
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
	{MANIM_DEATH,	0,	NULL}
};
mmove_t makron_move_death3 = { FRAME_death301, FRAME_death320, makron_frames_death3, NULL };

mframe_t makron_frames_sight[] =
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
	{MANIM_RUN,	0,	NULL}
};
mmove_t makron_move_sight = { FRAME_active01, FRAME_active13, makron_frames_sight, makron_run };

void makronBFG(edict_t* self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_MAKRON_BFG], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);
	gi.sound(self, CHAN_VOICE, sound_attack_bfg, 1, ATTN_NORM, 0);
	monster_fire_bfg(self, start, dir, 500, 400, 100, 1000, MZ2_MAKRON_BFG);
}


mframe_t makron_frames_attack3[] =
{
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	makronBFG},		// FIXME: BFG Attack here
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL}
};
mmove_t makron_move_attack3 = { FRAME_attak301, FRAME_attak308, makron_frames_attack3, makron_run };

mframe_t makron_frames_attack4[] =
{
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	MakronHyperblaster},		// fire
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL}
};
mmove_t makron_move_attack4 = { FRAME_attak401, FRAME_attak426, makron_frames_attack4, makron_run };

mframe_t makron_frames_attack5[] =
{
	{MANIM_MISSILE,	0,	makron_prerailgun},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	MakronRailgun},		// Fire railgun
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL},
	{MANIM_MISSILE,	0,	NULL}
};
mmove_t makron_move_attack5 = { FRAME_attak501, FRAME_attak516, makron_frames_attack5, makron_run };

// FIXME: He's not firing from the proper Z
void MakronRailgun(edict_t* self)
{
	vec3_t	start;
	vec3_t	dir;
	vec3_t	forward, right;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_MAKRON_RAILGUN_1], forward, right, start);

	// calc direction to where we targted
	// SLUGFILLER--thanks to the aimangles method the standard method shall do
	AngleVectors(self->aimangles, dir, NULL, NULL);

	monster_fire_railgun(self, start, dir, 30, 100, MZ2_MAKRON_RAILGUN_1);
}

// FIXME: This is all wrong. He's not firing at the proper angles.
void MakronHyperblaster(edict_t* self)
{
	vec3_t	dir;
	vec3_t	start;
	vec3_t	forward, right;
	int		flash_number;

	flash_number = MZ2_MAKRON_BLASTER_1 + (self->s.frame - FRAME_attak405);

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	VectorCopy(self->aimangles, dir);
	if (self->s.frame <= FRAME_attak413)
		dir[1] -= 10 * (self->s.frame - FRAME_attak413);
	else
		dir[1] += 10 * (self->s.frame - FRAME_attak421);

	AngleVectors(dir, forward, NULL, NULL);

	monster_fire_blaster(self, start, forward, 15, 1000, MZ2_MAKRON_BLASTER_1, EF_BLASTER, true);
}


void makron_pain(edict_t* self, edict_t* other, float kick, int damage)
{

	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	// Lessen the chance of him going into his pain frames
	if (damage <= 25)
		if (random() < 0.2)
			return;

	self->pain_debounce_time = level.time + PAINTIME;
	if (skill->value == 3)
		return;		// no pain anims in nightmare


	if (damage <= 40)
	{
		gi.sound(self, CHAN_VOICE, sound_pain4, 1, ATTN_NONE, 0);
		self->monsterinfo.currentmove = &makron_move_pain4;
	}
	else if (damage <= 110)
	{
		gi.sound(self, CHAN_VOICE, sound_pain5, 1, ATTN_NONE, 0);
		self->monsterinfo.currentmove = &makron_move_pain5;
	}
	else
	{
		if (damage <= 150) 
		{
			if (random() <= 0.45)
			{
				gi.sound(self, CHAN_VOICE, sound_pain6, 1, ATTN_NONE, 0);
				self->monsterinfo.currentmove = &makron_move_pain6;
			}
			else
				if (random() <= 0.35)
				{
					gi.sound(self, CHAN_VOICE, sound_pain6, 1, ATTN_NONE, 0);
					self->monsterinfo.currentmove = &makron_move_pain6;
				}
		}
	}
}

void makron_sight(edict_t* self, edict_t* other)
{
	self->monsterinfo.currentmove = &makron_move_sight;
}

void makron_attack(edict_t* self)
{
	vec3_t	vec;
	float	range;
	float	r;

	r = random();

	VectorSubtract(self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength(vec);


	if (r <= 0.3)
	{
		self->pain_debounce_time = level.time + 0.9;
		self->monsterinfo.currentmove = &makron_move_attack3;
	}
	else if (r <= 0.6)
	{
		self->pain_debounce_time = level.time + 2.7;
		self->monsterinfo.currentmove = &makron_move_attack4;
	}
	else
	{
		self->pain_debounce_time = level.time + 1.7;
		self->monsterinfo.currentmove = &makron_move_attack5;
	}
}

/*
---
Makron Torso. This needs to be spawned in
---
*/

void makron_torso_think(edict_t* self)
{
	if (++self->s.frame < 365)
		self->nextthink = level.time + FRAMETIME;
	else
	{
		self->s.frame = 346;
		self->nextthink = level.time + FRAMETIME;
	}
}

void makron_torso(edict_t* ent)
{
	ent->movetype = MOVETYPE_NONE;
	ent->solid = SOLID_NOT;
	VectorSet(ent->mins, -8, -8, 0);
	VectorSet(ent->maxs, 8, 8, 8);
	ent->s.frame = 346;
	ent->s.modelindex = gi.modelindex("models/monsters/boss3/rider/tris.md2");
	ent->think = makron_torso_think;
	ent->nextthink = level.time + 2 * FRAMETIME;
	ent->s.sound = gi.soundindex("makron/spine.wav");
	gi.linkentity(ent);
}


//
// death
//

void makron_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -60, -60, 0);
	VectorSet(self->maxs, 60, 60, 72);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity(self);
}


void makron_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	edict_t* tempent;

	int		n;

	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	self->s.sound = 0;
	// check for gib
	if (self->gib_health <= 0)
	{
		gi.sound(self, CHAN_VOICE, gi.soundindex("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n = 0; n < 1 /*4*/; n++)
			ThrowGib(self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		for (n = 0; n < 4; n++)
			ThrowGib(self, "models/objects/gibs/sm_metal/tris.md2", damage, GIB_METALLIC);
		ThrowHead(self, "models/objects/gibs/gear/tris.md2", damage, GIB_METALLIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	gi.sound(self, CHAN_VOICE, sound_death, 1, ATTN_NONE, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	tempent = G_Spawn();
	VectorCopy(self->s.origin, tempent->s.origin);
	VectorCopy(self->s.angles, tempent->s.angles);
	tempent->s.origin[1] -= 84;
	makron_torso(tempent);

	self->monsterinfo.currentmove = &makron_move_death2;

}

qboolean Makron_CheckAttack(edict_t* self)
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


//
// monster_makron
//

void MakronPrecache(void)
{
	sound_pain4 = gi.soundindex("makron/pain3.wav");
	sound_pain5 = gi.soundindex("makron/pain2.wav");
	sound_pain6 = gi.soundindex("makron/pain1.wav");
	sound_death = gi.soundindex("makron/death.wav");
	sound_step_left = gi.soundindex("makron/step1.wav");
	sound_step_right = gi.soundindex("makron/step2.wav");
	sound_attack_bfg = gi.soundindex("makron/bfg_fire.wav");
	sound_brainsplorch = gi.soundindex("makron/brain1.wav");
	sound_prerailgun = gi.soundindex("makron/rail_up.wav");
	sound_popup = gi.soundindex("makron/popup.wav");
	sound_taunt1 = gi.soundindex("makron/voice4.wav");
	sound_taunt2 = gi.soundindex("makron/voice3.wav");
	sound_taunt3 = gi.soundindex("makron/voice.wav");
	sound_hit = gi.soundindex("makron/bhit.wav");

	gi.modelindex("models/monsters/boss3/rider/tris.md2");
}

/*QUAKED monster_makron (1 .5 0) (-30 -30 0) (30 30 90) Ambush Trigger_Spawn Sight
*/
void SP_monster_makron(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	MakronPrecache();

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/boss3/rider/tris.md2");
	VectorSet(self->mins, -30, -30, 0);
	VectorSet(self->maxs, 30, 30, 90);
	VectorSet(self->headmins, -30, -30, 45);
	VectorSet(self->headmaxs, 30, 30, 70);

	self->health = 3000;
	self->gib_health = 5000;
	self->mass = 500;

	self->pain = makron_pain;
	self->die = makron_die;
	self->monsterinfo.stand = makron_stand;
	self->monsterinfo.walk = makron_walk;
	self->monsterinfo.run = makron_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.attack = makron_attack;
	self->monsterinfo.sight = makron_sight;
	self->monsterinfo.checkattack = Makron_CheckAttack;

	self->monsterinfo.speed = 9;
	self->monsterinfo.walkspeed = 9;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &makron_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}


void ED_CallSpawn(edict_t* ent);

/*
=================
MakronSpawn

=================
*/
void walkmonster_start_go(edict_t* self);

void makron_start_go(edict_t* self)
{
	walkmonster_start_go(self);
	self->monsterinfo.currentmove = &makron_move_sight;
}

void MakronSpawn(edict_t* self)
{
	vec3_t		vec;
	edict_t* player;

	// SLUGFILLER--the new makron will start in his jumping frame
	self->classname = "monster_makron";
	ED_CallSpawn(self);
	self->think = makron_start_go;

	// jump at player
	player = self->enemy;
	if (!player)
		return;

	VectorSubtract(player->s.origin, self->s.origin, vec);
	self->aimangles[YAW] = self->s.angles[YAW] = vectoyaw(vec);
	VectorNormalize(vec);
	VectorMA(vec3_origin, 400, vec, self->velocity);
	self->velocity[2] = 200;
	self->groundentity = NULL;
}

/*
=================
MakronToss

Jorg is just about dead, so set up to launch Makron out
=================
*/
void MakronToss(edict_t* self)
{
	edict_t* ent;

	ent = G_Spawn();
	ent->nextthink = level.time + 0.8;
	ent->think = MakronSpawn;
	ent->target = self->target;
	ent->enemy = self->enemy;
	VectorCopy(self->s.origin, ent->s.origin);
	VectorCopy(self->s.angles, ent->s.angles);
}
