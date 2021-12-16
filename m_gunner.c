/*
==============================================================================

GUNNER

==============================================================================
*/

#include "g_local.h"
#include "m_gunner.h"


static int	sound_pain;
static int	sound_pain2;
static int	sound_death;
static int	sound_idle;
static int	sound_open;
static int	sound_search;
static int	sound_sight;


void gunner_idlesound (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

void gunner_sight (edict_t *self, edict_t *other)
{
	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void gunner_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}


qboolean visible (edict_t *self, edict_t *other);
void GunnerGrenade (edict_t *self);
void GunnerFire (edict_t *self);
void gunner_fire_chain(edict_t *self);
void gunner_refire_chain(edict_t *self);


void gunner_stand (edict_t *self);

mframe_t gunner_frames_fidget [] =
{
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, gunner_idlesound,
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
mmove_t	gunner_move_fidget = {FRAME_stand31, FRAME_stand70, gunner_frames_fidget, gunner_stand};

void gunner_fidget (edict_t *self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		return;
	if (random() <= 0.05)
		self->monsterinfo.currentmove = &gunner_move_fidget;
}

mframe_t gunner_frames_stand [] =
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
	MANIM_STAND, 0, gunner_fidget,

	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, gunner_fidget,

	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, gunner_fidget
};
mmove_t	gunner_move_stand = {FRAME_stand01, FRAME_stand30, gunner_frames_stand, NULL};

void gunner_stand (edict_t *self)
{
		self->monsterinfo.currentmove = &gunner_move_stand;
}


mframe_t gunner_frames_walk [] =
{
	MANIM_WALK, -5, NULL,
	MANIM_WALK, -2, NULL,
	MANIM_WALK, -1, NULL,
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 2, NULL,
	MANIM_WALK, -3, NULL,
	MANIM_WALK, 1, NULL,
	MANIM_WALK, -1, NULL,
	MANIM_WALK, -3, NULL,
	MANIM_WALK, 2, NULL,
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 2, NULL,
	MANIM_WALK, -1, NULL
};
mmove_t gunner_move_walk = {FRAME_walk07, FRAME_walk19, gunner_frames_walk, NULL};

void gunner_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &gunner_move_walk;
}

mframe_t gunner_frames_run [] =
{
	MANIM_RUN, 11, NULL,
	MANIM_RUN, -6, NULL,
	MANIM_RUN, -6, NULL,
	MANIM_RUN, -6, NULL,
	MANIM_RUN, 0,  NULL,
	MANIM_RUN, -5, NULL,
	MANIM_RUN, -2, NULL,
	MANIM_RUN, -9, NULL
};

mmove_t gunner_move_run = {FRAME_run01, FRAME_run08, gunner_frames_run, NULL};

void gunner_run (edict_t *self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &gunner_move_stand;
	else
		self->monsterinfo.currentmove = &gunner_move_run;
}

mframe_t gunner_frames_runandshoot [] =
{
	MANIM_MISSILE, 32, NULL,
	MANIM_MISSILE, 15, NULL,
	MANIM_MISSILE, 10, NULL,
	MANIM_MISSILE, 18, NULL,
	MANIM_MISSILE, 8,  NULL,
	MANIM_MISSILE, 20, NULL
};

mmove_t gunner_move_runandshoot = {FRAME_runs01, FRAME_runs06, gunner_frames_runandshoot, NULL};

void gunner_runandshoot (edict_t *self)
{
	self->monsterinfo.currentmove = &gunner_move_runandshoot;
}

mframe_t gunner_frames_pain3 [] =
{
	MANIM_MISC, -3, NULL,
	MANIM_MISC, 1,	 NULL,
	MANIM_MISC, 1,	 NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, 1,	 NULL
};
mmove_t gunner_move_pain3 = {FRAME_pain301, FRAME_pain305, gunner_frames_pain3, gunner_run};

mframe_t gunner_frames_pain2 [] =
{
	MANIM_MISC, -2, NULL,
	MANIM_MISC, 11, NULL,
	MANIM_MISC, 6,	 NULL,
	MANIM_MISC, 2,	 NULL,
	MANIM_MISC, -1, NULL,
	MANIM_MISC, -7, NULL,
	MANIM_MISC, -2, NULL,
	MANIM_MISC, -7, NULL
};
mmove_t gunner_move_pain2 = {FRAME_pain201, FRAME_pain208, gunner_frames_pain2, gunner_run};

mframe_t gunner_frames_pain1 [] =
{
	MANIM_MISC, 2,	 NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, -5, NULL,
	MANIM_MISC, 3,	 NULL,
	MANIM_MISC, -1, NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, 1,	 NULL,
	MANIM_MISC, 1,	 NULL,
	MANIM_MISC, 2,	 NULL,
	MANIM_MISC, 1,	 NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, -2, NULL,
	MANIM_MISC, -2, NULL,
	MANIM_MISC, 0,	 NULL,
	MANIM_MISC, 0,	 NULL
};
mmove_t gunner_move_pain1 = {FRAME_pain101, FRAME_pain118, gunner_frames_pain1, gunner_run};

void gunner_pain (edict_t *self, edict_t *other, float kick, int damage)
{
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	if (rand()&1)
		gi.sound (self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 10)
		self->monsterinfo.currentmove = &gunner_move_pain3;
	else if (damage <= 25)
		self->monsterinfo.currentmove = &gunner_move_pain2;
	else
		self->monsterinfo.currentmove = &gunner_move_pain1;
}

void gunner_dead (edict_t *self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity (self);
}

mframe_t gunner_frames_death [] =
{
	MANIM_DEATH, 0,	 NULL,
	MANIM_DEATH, 0,	 NULL,
	MANIM_DEATH, 0,	 gunner_dead,
	MANIM_DEATH, -7, NULL,
	MANIM_DEATH, -3, NULL,
	MANIM_DEATH, -5, NULL,
	MANIM_DEATH, 8,	 NULL,
	MANIM_DEATH, 6,	 NULL,
	MANIM_DEATH, 0,	 NULL,
	MANIM_DEATH, 0,	 NULL,
	MANIM_DEATH, 0,	 NULL
};
mmove_t gunner_move_death = {FRAME_death01, FRAME_death11, gunner_frames_death, monster_dead_dead};

void gunner_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int		n;

	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

// check for gib
	if (self->gib_health <= 0)
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 2; n++)
			ThrowGib (self, "models/objects/gibs/bone/tris.md2", damage, GIB_ORGANIC);
		for (n= 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

// regular death
	gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	self->monsterinfo.currentmove = &gunner_move_death;
}


mframe_t gunner_frames_duck [] =
{
	MANIM_DUCK, 1,  monster_duck_down,
	MANIM_DUCK, 1,  NULL,
	MANIM_DUCK, 1,  monster_duck_hold,
	MANIM_DUCK, 0,  NULL,
	MANIM_DUCK, -1, NULL,
	MANIM_DUCK, -1, NULL,
	MANIM_DUCK, 0,  monster_duck_up,
	MANIM_DUCK, -1, NULL
};
mmove_t	gunner_move_duck = {FRAME_duck01, FRAME_duck08, gunner_frames_duck, gunner_run};

void gunner_duck (edict_t *self, edict_t *attacker, float eta)
{
	self->monsterinfo.currentmove = &gunner_move_duck;
	self->monsterinfo.pausetime = level.time + 1;
}


void gunner_opengun (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_open, 1, ATTN_IDLE, 0);
}

void GunnerFire (edict_t *self)
{
	vec3_t	start;
	vec3_t	forward, right;
	vec3_t	aim;
	int		flash_number;

	flash_number = MZ2_GUNNER_MACHINEGUN_1 + (self->s.frame - FRAME_attak216);

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	// SLUGFILLER--instead of projecting enemy back a bit and targeting there
	// just use the aim angles
	AngleVectors (self->aimangles, aim, NULL, NULL);

	monster_fire_bullet (self, start, aim, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_number, true);
}

void GunnerGrenade (edict_t *self)
{
	vec3_t	start;
	vec3_t	forward, right;
	vec3_t	aim;
	int		flash_number;

	if (self->s.frame == FRAME_attak105)
		flash_number = MZ2_GUNNER_GRENADE_1;
	else if (self->s.frame == FRAME_attak108)
		flash_number = MZ2_GUNNER_GRENADE_2;
	else if (self->s.frame == FRAME_attak111)
		flash_number = MZ2_GUNNER_GRENADE_3;
	else // (self->s.frame == FRAME_attak114)
		flash_number = MZ2_GUNNER_GRENADE_4;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	//FIXME : do a spread -225 -75 75 225 degrees around forward
	AngleVectors (self->aimangles, aim, NULL, NULL);

	monster_fire_grenade (self, start, aim, 60, 300, flash_number);
}

mframe_t gunner_frames_attack_chain [] =
{
	/*
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	*/
	MANIM_MELEE, 0, gunner_opengun,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL
};
mmove_t gunner_move_attack_chain = {FRAME_attak209, FRAME_attak215, gunner_frames_attack_chain, gunner_fire_chain};

mframe_t gunner_frames_fire_chain [] =
{
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire,
	MANIM_MELEE,   0, GunnerFire
};
mmove_t gunner_move_fire_chain = {FRAME_attak216, FRAME_attak223, gunner_frames_fire_chain, gunner_refire_chain};

mframe_t gunner_frames_endfire_chain [] =
{
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL
};
mmove_t gunner_move_endfire_chain = {FRAME_attak224, FRAME_attak230, gunner_frames_endfire_chain, gunner_run};

mframe_t gunner_frames_attack_grenade [] =
{
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, GunnerGrenade,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, GunnerGrenade,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, GunnerGrenade,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, GunnerGrenade,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL,
	MANIM_MELEE, 0, NULL
};
mmove_t gunner_move_attack_grenade = {FRAME_attak101, FRAME_attak121, gunner_frames_attack_grenade, gunner_run};

void gunner_attack(edict_t *self)
{
	if (range (self, self->enemy) == RANGE_MELEE)
	{
		self->pain_debounce_time = level.time + 0.8;
		self->monsterinfo.currentmove = &gunner_move_attack_chain;
	}
	else
	{
		if (random() <= 0.5)
		{
			self->pain_debounce_time = level.time + 2.2;
			self->monsterinfo.currentmove = &gunner_move_attack_grenade;
		}
		else
		{
			self->pain_debounce_time = level.time + 0.8;
			self->monsterinfo.currentmove = &gunner_move_attack_chain;
		}
	}
}

void gunner_fire_chain(edict_t *self)
{
	self->pain_debounce_time = level.time + 0.9;
	self->monsterinfo.currentmove = &gunner_move_fire_chain;
}

void gunner_refire_chain(edict_t *self)
{
	if (self->enemy && self->enemy->health > 0)
		if ( visible (self, self->enemy) )
			if (random() <= 0.5)
			{
				self->pain_debounce_time = level.time + 0.9;
				self->monsterinfo.currentmove = &gunner_move_fire_chain;
				return;
			}
	self->pain_debounce_time = level.time + 0.8;
	self->monsterinfo.currentmove = &gunner_move_endfire_chain;
}

/*QUAKED monster_gunner (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_gunner (edict_t *self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict (self);
		return;
	}

	sound_death = gi.soundindex ("gunner/death1.wav");	
	sound_pain = gi.soundindex ("gunner/gunpain2.wav");	
	sound_pain2 = gi.soundindex ("gunner/gunpain1.wav");	
	sound_idle = gi.soundindex ("gunner/gunidle1.wav");	
	sound_open = gi.soundindex ("gunner/gunatck1.wav");	
	sound_search = gi.soundindex ("gunner/gunsrch1.wav");	
	sound_sight = gi.soundindex ("gunner/sight1.wav");	

	gi.soundindex ("gunner/gunatck2.wav");
	gi.soundindex ("gunner/gunatck3.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/gunner/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 40);
	VectorSet (self->headmins, -16, -16, 32);
	VectorSet (self->headmaxs, 16, 16, 40);

	self->health = 175;
	self->gib_health = 245;
	self->mass = 200;

	self->pain = gunner_pain;
	self->die = gunner_die;

	self->monsterinfo.stand = gunner_stand;
	self->monsterinfo.walk = gunner_walk;
	self->monsterinfo.run = gunner_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.duck = gunner_duck;
	self->monsterinfo.unduck = monster_duck_up;
	self->monsterinfo.attack = gunner_attack;
	self->monsterinfo.sight = gunner_sight;
	self->monsterinfo.search = gunner_search;

	self->monsterinfo.speed = 15;
	self->monsterinfo.walkspeed = 5;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &gunner_move_stand;	
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start (self);
}
