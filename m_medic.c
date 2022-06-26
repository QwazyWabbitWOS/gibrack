/*
==============================================================================

MEDIC

==============================================================================
*/

#include "g_local.h"
#include "m_medic.h"

qboolean visible(edict_t* self, edict_t* other);


static int	sound_idle1;
static int	sound_pain1;
static int	sound_pain2;
static int	sound_die;
static int	sound_sight;
static int	sound_search;
static int	sound_hook_launch;
static int	sound_hook_hit;
static int	sound_hook_heal;
static int	sound_hook_retract;


edict_t* medic_FindDeadMonster(edict_t* self)
{
	edict_t* ent = NULL;
	edict_t* best = NULL;

	while ((ent = findradius(ent, self->s.origin, 1024)) != NULL)
	{
		if (ent == self)
			continue;
		if (!(ent->svflags & SVF_MONSTER))
			continue;
		if (ent->monsterinfo.aiflags & AI_GOOD_GUY)
			continue;
		if (ent->owner)
			continue;
		if (ent->health > 0)
			continue;
		if (ent->nextthink)
			continue;
		if (!visible(self, ent))
			continue;
		if (!best)
		{
			best = ent;
			continue;
		}
		if (ent->max_health <= best->max_health)
			continue;
		best = ent;
	}

	return best;
}

void medic_idle(edict_t* self)
{
	edict_t* ent;

	gi.sound(self, CHAN_VOICE, sound_idle1, 1, ATTN_IDLE, 0);

	ent = medic_FindDeadMonster(self);
	if (ent)
	{
		self->enemy = ent;
		self->enemy->owner = self;
		self->monsterinfo.aiflags |= AI_MEDIC;
		// SLUGFILLER--were healing not killing
		self->chain = NULL;
		VectorCopy(self->enemy->s.origin, self->monsterinfo.last_sighting);
		FoundTarget(self);
	}
}

void medic_search(edict_t* self)
{
	edict_t* ent;

	gi.sound(self, CHAN_VOICE, sound_search, 1, ATTN_IDLE, 0);

	if (!self->oldenemy)
	{
		ent = medic_FindDeadMonster(self);
		if (ent)
		{
			self->oldenemy = self->enemy;
			self->enemy = ent;
			self->enemy->owner = self;
			self->monsterinfo.aiflags |= AI_MEDIC;
			// SLUGFILLER--were healing not killing
			self->chain = NULL;
			VectorCopy(self->enemy->s.origin, self->monsterinfo.last_sighting);
			FoundTarget(self);
		}
	}
}

void medic_sight(edict_t* self, edict_t* other)
{
	gi.sound(self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}


mframe_t medic_frames_stand[] =
{
	MANIM_STAND, 0, medic_idle,
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

};
mmove_t medic_move_stand = { FRAME_wait1, FRAME_wait90, medic_frames_stand, NULL };

void medic_stand(edict_t* self)
{
	self->monsterinfo.currentmove = &medic_move_stand;
}


mframe_t medic_frames_walk[] =
{
	MANIM_WALK, -2.8,	NULL,
	MANIM_WALK, 9.1,	NULL,
	MANIM_WALK, -8,		NULL,
	MANIM_WALK, 0,		NULL,
	MANIM_WALK, 1,		NULL,
	MANIM_WALK, 0,		NULL,
	MANIM_WALK, 2,		NULL,
	MANIM_WALK, 2.6,	NULL,
	MANIM_WALK, -7,		NULL,
	MANIM_WALK, 0.9,	NULL,
	MANIM_WALK, 5,		NULL,
	MANIM_WALK, 0.3,	NULL
};
mmove_t medic_move_walk = { FRAME_walk1, FRAME_walk12, medic_frames_walk, NULL };

void medic_walk(edict_t* self)
{
	self->monsterinfo.currentmove = &medic_move_walk;
}


mframe_t medic_frames_run[] =
{
	MANIM_RUN, -4,		NULL,
	MANIM_RUN, 0.5,		NULL,
	MANIM_RUN, 3.4,		NULL,
	MANIM_RUN, 1.4,		NULL,
	MANIM_RUN, 2,		NULL,
	MANIM_RUN, 13.6,	NULL

};
mmove_t medic_move_run = { FRAME_run1, FRAME_run6, medic_frames_run, NULL };

void medic_run(edict_t* self)
{
	if (!(self->monsterinfo.aiflags & AI_MEDIC))
	{
		edict_t* ent;

		ent = medic_FindDeadMonster(self);
		if (ent)
		{
			self->oldenemy = self->enemy;
			self->enemy = ent;
			self->enemy->owner = self;
			self->monsterinfo.aiflags |= AI_MEDIC;
			// SLUGFILLER--were healing not killing
			self->chain = NULL;
			VectorCopy(self->enemy->s.origin, self->monsterinfo.last_sighting);
			FoundTarget(self);
			return;
		}
	}

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &medic_move_stand;
	else
		self->monsterinfo.currentmove = &medic_move_run;
}


mframe_t medic_frames_pain1[] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t medic_move_pain1 = { FRAME_paina1, FRAME_paina8, medic_frames_pain1, medic_run };

mframe_t medic_frames_pain2[] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t medic_move_pain2 = { FRAME_painb1, FRAME_painb15, medic_frames_pain2, medic_run };

void medic_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (random() < 0.5)
	{
		self->monsterinfo.currentmove = &medic_move_pain1;
		gi.sound(self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	}
	else
	{
		self->monsterinfo.currentmove = &medic_move_pain2;
		gi.sound(self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
	}
}

void medic_fire_blaster(edict_t* self)
{
	vec3_t	start;
	vec3_t	forward, right;
	vec3_t	dir;
	int		effect;

	if ((self->s.frame == FRAME_attack9) || (self->s.frame == FRAME_attack12))
		effect = EF_BLASTER;
	else if ((self->s.frame == FRAME_attack19) || (self->s.frame == FRAME_attack22) || (self->s.frame == FRAME_attack25) || (self->s.frame == FRAME_attack28))
		effect = EF_HYPERBLASTER;
	else
		effect = 0;

	AngleVectors(self->s.angles, forward, right, NULL);
	G_ProjectSource(self->s.origin, monster_flash_offset[MZ2_MEDIC_BLASTER_1], forward, right, start);

	AngleVectors(self->aimangles, dir, NULL, NULL);

	if (effect == EF_HYPERBLASTER)
		monster_fire_blaster(self, start, dir, 5, 1000, MZ2_MEDIC_BLASTER_1, effect, true);
	else
		monster_fire_blaster(self, start, dir, 30, 600, MZ2_MEDIC_BLASTER_1, effect, false);
}


void medic_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->clipmask = MASK_SOLID;
	gi.linkentity(self);
}

mframe_t medic_frames_death[] =
{
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, medic_dead,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL
};
mmove_t medic_move_death = { FRAME_death1, FRAME_death30, medic_frames_death, monster_dead_dead };

void medic_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	int		n;

	// SLUGFILLER--in case his health dropped from over half full to none in one shot
	if (self->gib_health < (self->max_gib_health / 2))
		self->s.skinnum = 1;

	// if we had a pending patient, free him up for another medic
	if ((self->enemy) && (self->enemy->owner == self))
		self->enemy->owner = NULL;

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
	gi.sound(self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	self->monsterinfo.currentmove = &medic_move_death;
}


mframe_t medic_frames_duck[] =
{
	MANIM_DUCK, -1,	monster_duck_down,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	monster_duck_hold,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	monster_duck_up,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL,
	MANIM_DUCK, -1,	NULL
};
mmove_t medic_move_duck = { FRAME_duck1, FRAME_duck16, medic_frames_duck, medic_run };

void medic_duck(edict_t* self, edict_t* attacker, float eta)
{
	self->monsterinfo.currentmove = &medic_move_duck;
	self->monsterinfo.pausetime = level.time + 1;
}

void medic_unduck(edict_t* self)
{
	// TITANIUM--stop the ducking sequence
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->monsterinfo.pausetime = level.time;
	if (self->s.frame < FRAME_duck3)
		self->monsterinfo.nextframe = FRAME_duck16;
	else if (self->s.frame < FRAME_duck6)
		self->monsterinfo.nextframe = FRAME_duck6;
}

mframe_t medic_frames_attackHyperBlaster[] =
{
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	medic_fire_blaster
};
mmove_t medic_move_attackHyperBlaster = { FRAME_attack15, FRAME_attack30, medic_frames_attackHyperBlaster, medic_run };


void medic_continue(edict_t* self)
{
	if (visible(self, self->enemy))
		if (random() <= 0.95)
		{
			self->pain_debounce_time = level.time + 1.7;
			self->monsterinfo.currentmove = &medic_move_attackHyperBlaster;
		}
}


mframe_t medic_frames_attackBlaster[] =
{
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 5,	NULL,
	MANIM_MISSILE, 5,	NULL,
	MANIM_MISSILE, 3,	NULL,
	MANIM_MISSILE, 2,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	medic_fire_blaster,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,	medic_continue	// Change to medic_continue... Else, go to frame 32
};
mmove_t medic_move_attackBlaster = { FRAME_attack1, FRAME_attack14, medic_frames_attackBlaster, medic_run };


void medic_hook_launch(edict_t* self)
{
	gi.sound(self, CHAN_WEAPON, sound_hook_launch, 1, ATTN_NORM, 0);
}

void ED_CallSpawn(edict_t* ent);

static vec3_t	medic_cable_offsets[] =
{
	45.0,  -9.2, 15.5,
	48.4,  -9.7, 15.2,
	47.8,  -9.8, 15.8,
	47.3,  -9.3, 14.3,
	45.4, -10.1, 13.1,
	41.9, -12.7, 12.0,
	37.8, -15.8, 11.2,
	34.3, -18.4, 10.7,
	32.7, -19.7, 10.4,
	32.7, -19.7, 10.4
};

void medic_cable_attack(edict_t* self)
{
	vec3_t	offset, start, end, f, r;
	trace_t	tr;
	vec3_t	dir, angles;
	float	distance;

	if (!self->enemy->inuse)
	{
		// TITANIUM--no enemy, reel in
		self->monsterinfo.nextframe = FRAME_attack52;
		return;
	}

	AngleVectors(self->s.angles, f, r, NULL);
	VectorCopy(medic_cable_offsets[self->s.frame - FRAME_attack42], offset);
	G_ProjectSource(self->s.origin, offset, f, r, start);

	// check for max distance
	VectorSubtract(start, self->enemy->s.origin, dir);
	distance = VectorLength(dir);
	if (distance > 256)
	{
		// TITANIUM--enemy too far, reel in
		self->monsterinfo.nextframe = FRAME_attack52;
		return;
	}

	// check for min/max pitch
	vectoangles(dir, angles);
	if (angles[0] < -180)
		angles[0] += 360;
	if (fabs(angles[0]) > 45)
	{
		// TITANIUM--no angle on enemy, reel in
		self->monsterinfo.nextframe = FRAME_attack52;
		return;
	}

	tr = gi.trace(start, NULL, NULL, self->enemy->s.origin, self, MASK_SHOT);
	if (tr.fraction != 1.0 && tr.ent != self->enemy)
	{
		// TITANIUM--something is in the way, reel in
		self->monsterinfo.nextframe = FRAME_attack52;
		return;
	}

	if (self->s.frame == FRAME_attack43)
	{
		gi.sound(self->enemy, CHAN_AUTO, sound_hook_hit, 1, ATTN_NORM, 0);
		self->enemy->monsterinfo.aiflags |= AI_RESURRECTING;
	}
	else if (self->s.frame == FRAME_attack50)
	{
		self->enemy->spawnflags = 0;
		self->enemy->monsterinfo.aiflags = 0;
		self->enemy->target = NULL;
		self->enemy->targetname = NULL;
		self->enemy->combattarget = NULL;
		self->enemy->deathtarget = NULL;
		self->enemy->owner = self;
		ED_CallSpawn(self->enemy);
		self->enemy->owner = NULL;
		if (self->enemy->nextthink > 0)
		{
			self->enemy->nextthink = 0;
			self->enemy->think(self->enemy);
		}
		self->enemy->monsterinfo.aiflags |= AI_RESURRECTING;
		if (self->oldenemy && self->oldenemy->client && visible(self->enemy, self->oldenemy))
		{
			self->enemy->enemy = self->oldenemy;
			// TITANIUM--he's getting angry at a player
			self->enemy->chain = NULL;
			VectorCopy(self->enemy->s.origin, self->monsterinfo.last_sighting);
			FoundTarget(self->enemy);
		}
	}
	else
	{
		if (self->s.frame == FRAME_attack44)
			gi.sound(self, CHAN_WEAPON, sound_hook_heal, 1, ATTN_NORM, 0);
	}

	// adjust start for beam origin being in middle of a segment
	VectorMA(start, 8, f, start);

	// adjust end z for end spot since the monster is currently dead
	VectorCopy(self->enemy->s.origin, end);
	end[2] = self->enemy->absmin[2] + self->enemy->size[2] / 2;

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_MEDIC_CABLE_ATTACK);
	gi.WriteShort(self - g_edicts);
	gi.WritePosition(start);
	gi.WritePosition(end);
	gi.multicast(self->s.origin, MULTICAST_PVS);
}

void medic_hook_retract(edict_t* self)
{
	gi.sound(self, CHAN_WEAPON, sound_hook_retract, 1, ATTN_NORM, 0);
	self->enemy->monsterinfo.aiflags &= ~AI_RESURRECTING;
}

mframe_t medic_frames_attackCable[] =
{
	MANIM_MISSILE, 2,		NULL,
	MANIM_MISSILE, 3,		NULL,
	MANIM_MISSILE, 5,		NULL,
	MANIM_MISSILE, 4.4,	NULL,
	MANIM_MISSILE, 4.7,	NULL,
	MANIM_MISSILE, 5,	NULL,
	MANIM_MISSILE, 6,	NULL,
	MANIM_MISSILE, 4,	NULL,
	MANIM_MISSILE, 0,	NULL,
	MANIM_MISSILE, 0,		medic_hook_launch,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, 0,		medic_cable_attack,
	MANIM_MISSILE, -15,	medic_hook_retract,
	MANIM_MISSILE, -1.5,	NULL,
	MANIM_MISSILE, -1.2,	NULL,
	MANIM_MISSILE, -3,	NULL,
	MANIM_MISSILE, -2,	NULL,
	MANIM_MISSILE, 0.3,	NULL,
	MANIM_MISSILE, 0.7,	NULL,
	MANIM_MISSILE, 1.2,	NULL,
	MANIM_MISSILE, 1.3,	NULL
};
mmove_t medic_move_attackCable = { FRAME_attack33, FRAME_attack60, medic_frames_attackCable, medic_run };


void medic_attack(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_MEDIC)
	{
		self->pain_debounce_time = level.time + 2.9;
		self->monsterinfo.currentmove = &medic_move_attackCable;
	}
	else
	{
		self->pain_debounce_time = level.time + 1.5;
		self->monsterinfo.currentmove = &medic_move_attackBlaster;
	}
}

qboolean medic_checkattack(edict_t* self)
{
	if (self->monsterinfo.aiflags & AI_MEDIC)
	{
		// TITANIUM--we don't want to shoot for nothing
		// so check if we can even shoot
		vec3_t	offset, start, f, r;
		trace_t	tr;
		vec3_t	dir, angles;
		float	distance;

		if (!self->enemy->inuse)
			return false;

		AngleVectors(self->s.angles, f, r, NULL);
		VectorCopy(medic_cable_offsets[self->s.frame - FRAME_attack42], offset);
		G_ProjectSource(self->s.origin, offset, f, r, start);

		// check for max distance
		VectorSubtract(start, self->enemy->s.origin, dir);
		distance = VectorLength(dir);
		if (distance > 256)
			return false;

		// check for min/max pitch
		vectoangles(dir, angles);
		if (angles[0] < -180)
			angles[0] += 360;
		if (fabs(angles[0]) > 45)
			return false;

		tr = gi.trace(start, NULL, NULL, self->enemy->s.origin, self, MASK_SHOT);
		if (tr.fraction != 1.0 && tr.ent != self->enemy)
			return false;

		medic_attack(self);
		return true;
	}

	return M_CheckAttack(self);
}


/*QUAKED monster_medic (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_medic(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_idle1 = gi.soundindex("medic/idle.wav");
	sound_pain1 = gi.soundindex("medic/medpain1.wav");
	sound_pain2 = gi.soundindex("medic/medpain2.wav");
	sound_die = gi.soundindex("medic/meddeth1.wav");
	sound_sight = gi.soundindex("medic/medsght1.wav");
	sound_search = gi.soundindex("medic/medsrch1.wav");
	sound_hook_launch = gi.soundindex("medic/medatck2.wav");
	sound_hook_hit = gi.soundindex("medic/medatck3.wav");
	sound_hook_heal = gi.soundindex("medic/medatck4.wav");
	sound_hook_retract = gi.soundindex("medic/medatck5.wav");

	gi.soundindex("medic/medatck1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/medic/tris.md2");
	VectorSet(self->mins, -24, -24, -24);
	VectorSet(self->maxs, 24, 24, 32);
	VectorSet(self->headmins, 16, -24, 20);
	VectorSet(self->headmaxs, 24, 24, 32);

	self->health = 300;
	self->gib_health = 430;
	self->mass = 400;

	self->pain = medic_pain;
	self->die = medic_die;

	self->monsterinfo.stand = medic_stand;
	self->monsterinfo.walk = medic_walk;
	self->monsterinfo.run = medic_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.duck = medic_duck;
	self->monsterinfo.unduck = monster_duck_up;
	self->monsterinfo.attack = medic_attack;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = medic_sight;
	self->monsterinfo.idle = medic_idle;
	self->monsterinfo.search = medic_search;
	self->monsterinfo.checkattack = medic_checkattack;

	self->monsterinfo.speed = 22;
	self->monsterinfo.walkspeed = 9;

	gi.linkentity(self);

	self->monsterinfo.currentmove = &medic_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
