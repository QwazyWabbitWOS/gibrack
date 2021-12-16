// g_actor.c

#include "g_local.h"
//#include "m_actor.h"
#include "m_player.h"

#define	MAX_ACTOR_NAMES		8
char *actor_names[MAX_ACTOR_NAMES] =
{
	"Hellrot",
	"Tokay",
	"Killme",
	"Disruptor",
	"Adrianator",
	"Rambear",
	"Titus",
	"Bitterman"
};


mframe_t actor_frames_stand [] =
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
	MANIM_STAND, 0, NULL
};
mmove_t actor_move_stand = {FRAME_stand01, FRAME_stand20, actor_frames_stand, NULL};

void actor_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &actor_move_stand;

	// randomize on startup
	if (level.time < 1.0)
		self->s.frame = self->monsterinfo.currentmove->firstframe + (rand() % (self->monsterinfo.currentmove->lastframe - self->monsterinfo.currentmove->firstframe + 1));
}


mframe_t actor_frames_walk [] =
{
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 0, NULL,
	MANIM_WALK, 0, NULL
};
mmove_t actor_move_walk = {FRAME_run1, FRAME_run6, actor_frames_walk, NULL};

void actor_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &actor_move_walk;
}


mframe_t actor_frames_run [] =
{
	MANIM_RUN, 0, NULL,
	MANIM_RUN, 0, NULL,
	MANIM_RUN, 0, NULL,
	MANIM_RUN, 0, NULL,
	MANIM_RUN, 0, NULL,
	MANIM_RUN, 0, NULL,
};
mmove_t actor_move_run = {FRAME_run1, FRAME_run6, actor_frames_run, NULL};

void actor_run (edict_t *self)
{
	if ((level.time < self->pain_debounce_time) && (!self->enemy))
	{
		if (self->movetarget)
			actor_walk(self);
		else
			actor_stand(self);
		return;
	}

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		actor_stand(self);
		return;
	}

	self->monsterinfo.currentmove = &actor_move_run;
}


mframe_t actor_frames_pain1 [] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t actor_move_pain1 = {FRAME_pain101, FRAME_pain104, actor_frames_pain1, actor_run};

mframe_t actor_frames_pain2 [] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t actor_move_pain2 = {FRAME_pain201, FRAME_pain204, actor_frames_pain2, actor_run};

mframe_t actor_frames_pain3 [] =
{
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL,
	MANIM_MISC, 0, NULL
};
mmove_t actor_move_pain3 = {FRAME_pain301, FRAME_pain304, actor_frames_pain3, actor_run};

mframe_t actor_frames_flipoff [] =
{
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL
};
mmove_t actor_move_flipoff = {FRAME_flip01, FRAME_flip12, actor_frames_flipoff, actor_run};

mframe_t actor_frames_taunt [] =
{
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL,
	MANIM_MISC, 0,  NULL
};
mmove_t actor_move_taunt = {FRAME_taunt01, FRAME_taunt17, actor_frames_taunt, actor_run};

char *messages[] =
{
	"Watch it",
	"#$@*&",
	"Idiot",
	"Check your targets"
};

void actor_pain (edict_t *self, edict_t *other, float kick, int damage)
{
	int		n;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;
//	gi.sound (self, CHAN_VOICE, actor.sound_pain, 1, ATTN_NORM, 0);

	if ((other->client) && (random() < 0.4))
	{
		vec3_t	v;

		VectorSubtract (other->s.origin, self->s.origin, v);
		v[2] += self->enemy->viewheight - self->viewheight;
		self->ideal_pitch = vectopitch (v);
		self->ideal_yaw = vectoyaw (v);
		if (random() < 0.5)
			self->monsterinfo.currentmove = &actor_move_flipoff;
		else
			self->monsterinfo.currentmove = &actor_move_taunt;
		gi.cprintf (other, PRINT_CHAT, "%s: %s!\n", self->message, messages[rand()%3]);
		return;
	}

	n = rand() % 3;
	if (n == 0)
		self->monsterinfo.currentmove = &actor_move_pain1;
	else if (n == 1)
		self->monsterinfo.currentmove = &actor_move_pain2;
	else
		self->monsterinfo.currentmove = &actor_move_pain3;
}


void actorMachineGun (edict_t *self)
{
	vec3_t	start;
	vec3_t	forward, right;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[MZ2_ACTOR_MACHINEGUN_1], forward, right, start);
	AngleVectors (self->aimangles, forward, NULL, NULL);
	monster_fire_bullet (self, start, forward, 8, 16, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_ACTOR_MACHINEGUN_1, false);
}


void actor_dead (edict_t *self)
{
	self->flags |= FL_NO_KNOCKBACK;

	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t actor_frames_death [] =
{
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL
};
mmove_t actor_move_death1 = {FRAME_death101, FRAME_death106, actor_frames_death, actor_dead};
mmove_t actor_move_death2 = {FRAME_death201, FRAME_death206, actor_frames_death, actor_dead};

mframe_t actor_frames_death3 [] =
{
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL,
	MANIM_DEATH, 0, NULL
};
mmove_t actor_move_death3 = {FRAME_death301, FRAME_death308, actor_frames_death3, actor_dead};

void actor_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int		n;

	self->s.modelindex2 = 0;

// check for gib
	if (self->gib_health <= 0)
	{
//		gi.sound (self, CHAN_VOICE, actor.sound_gib, 1, ATTN_NORM, 0);
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
//	gi.sound (self, CHAN_VOICE, actor.sound_die, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	n = rand() % 3;
	if (n == 0)
		self->monsterinfo.currentmove = &actor_move_death1;
	else if (n == 1)
		self->monsterinfo.currentmove = &actor_move_death2;
	else
		self->monsterinfo.currentmove = &actor_move_death3;
}


void actor_fire (edict_t *self)
{
	actorMachineGun (self);

	if (random()+0.25<1)
	{
		if (level.time >= self->monsterinfo.pausetime)
			self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
		else
			self->monsterinfo.aiflags |= AI_HOLD_FRAME;
	}
	else
	{
		self->monsterinfo.nextframe = self->s.frame - 1;
		self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	}
}

mframe_t actor_frames_attack [] =
{
	MANIM_MISSILE, 0, actorMachineGun,
	MANIM_MISSILE, 0, actor_fire,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL,
	MANIM_MISSILE, 0, NULL
};
mmove_t actor_move_attack = {FRAME_attack1, FRAME_attack8, actor_frames_attack, actor_run};

void actor_attack(edict_t *self)
{
	int		n;

	self->monsterinfo.currentmove = &actor_move_attack;
	n = (rand() & 15) + 3 + 7;
	self->monsterinfo.pausetime = level.time + n * FRAMETIME;
	self->pain_debounce_time = self->monsterinfo.pausetime + 0.8;
}


void actor_use (edict_t *self, edict_t *other, edict_t *activator)
{
	vec3_t		v;

	self->goalentity = self->movetarget = G_PickTarget(self->target);
	if ((!self->movetarget) || (strcmp(self->movetarget->classname, "target_actor") != 0))
	{
		gi.dprintf ("%s has bad target %s at %s\n", self->classname, self->target, vtos(self->s.origin));
		self->target = NULL;
		self->monsterinfo.pausetime = 0;
		self->monsterinfo.stand (self);
		return;
	}

	VectorSubtract (self->goalentity->s.origin, self->s.origin, v);
	self->ideal_pitch = self->s.angles[PITCH] = 0;
	self->ideal_yaw = self->s.angles[YAW] = vectoyaw(v);
	self->monsterinfo.walk (self);
	self->target = NULL;
}


/*QUAKED misc_actor (1 .5 0) (-16 -16 -24) (16 16 32)
*/

void SP_misc_actor (edict_t *self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict (self);
		return;
	}

	if (!self->targetname)
	{
		gi.dprintf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));
		G_FreeEdict (self);
		return;
	}

	if (!self->target)
	{
		gi.dprintf("%s with no target at %s\n", self->classname, vtos(self->s.origin));
		G_FreeEdict (self);
		return;
	}

	if (!self->message)
		self->message = actor_names[(self - g_edicts)%MAX_ACTOR_NAMES];

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = 255;
	self->s.modelindex2 = 255;
	self->s.skinnum = ((self - g_edicts)%MAX_ACTOR_NAMES) | (WEAP_MACHINEGUN<<8);
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 32);
	VectorSet (self->headmins, -16, -16, 12);
	VectorSet (self->headmaxs, 16, 16, 32);

	if (!self->health)
		self->health = 100;
	self->gib_health = self->health + 80;
	self->mass = 200;

	self->pain = actor_pain;
	self->die = actor_die;

	self->monsterinfo.stand = actor_stand;
	self->monsterinfo.walk = actor_walk;
	self->monsterinfo.run = actor_run;
	self->monsterinfo.dodge = M_MonsterDodge;
	self->monsterinfo.attack = actor_attack;

	self->monsterinfo.speed = 20;
	self->monsterinfo.walkspeed = 20;

	self->monsterinfo.aiflags |= AI_GOOD_GUY;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &actor_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start (self);

	// actors always start in a dormant state, they *must* be used to get going
	self->use = actor_use;
}


/*QUAKED target_actor (.5 .3 0) (-8 -8 -8) (8 8 8) JUMP SHOOT ATTACK x HOLD BRUTAL
JUMP			jump in set direction upon reaching this target
SHOOT			take a single shot at the pathtarget
ATTACK			attack pathtarget until it or actor is dead 

"target"		next target_actor
"pathtarget"	target of any action to be taken at this point
"wait"			amount of time actor should pause at this point
"message"		actor will "say" this to the player

for JUMP only:
"speed"			speed thrown forward (default 200)
"height"		speed thrown upwards (default 200)
*/

void target_actor_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	vec3_t	v;

	if (other->movetarget != self)
		return;
	
	if (other->enemy)
		return;

	other->goalentity = other->movetarget = NULL;

	if (self->message)
	{
		int		n;
		edict_t	*ent;

		for (n = 1; n <= game.maxclients; n++)
		{
			ent = &g_edicts[n];
			if (!ent->inuse)
				continue;
			gi.cprintf (ent, PRINT_CHAT, "%s: %s\n", actor_names[(other - g_edicts)%MAX_ACTOR_NAMES], self->message);
		}
	}

	if (self->spawnflags & 1)		//jump
	{
		other->s.angles[YAW] = vectoyaw(self->movedir);
		other->velocity[0] = self->movedir[0] * self->speed;
		other->velocity[1] = self->movedir[1] * self->speed;
		
		if (other->groundentity)
		{
			other->groundentity = NULL;
			other->velocity[2] = self->movedir[2];
			gi.sound(other, CHAN_VOICE, gi.soundindex("player/male/jump1.wav"), 1, ATTN_NORM, 0);
		}
	}

	if (self->spawnflags & 2)	//shoot
	{
		trace_t	tr;
		vec3_t	start, end;

		VectorCopy (other->s.origin, start);
		start[2] += 22;

		VectorMA (start, 8192, self->movedir, end);

		tr = gi.trace (start, NULL, NULL, end, other, MASK_SHOT);

		other->enemy = G_Spawn();
		VectorCopy (tr.endpos, other->enemy->s.origin);
		VectorSet (other->enemy->mins, -8, -8, -8);
		VectorSet (other->enemy->maxs, 8, 8, 8);
		other->enemy->svflags = SVF_NOCLIENT;
		gi.linkentity (other->enemy);

		other->enemy->nextthink = level.time + 1;
		other->enemy->think = G_FreeEdict;

		other->monsterinfo.run (other);
	}
	else if (self->spawnflags & 4)	//attack
	{
		other->enemy = G_PickTarget(self->pathtarget);
		if (other->enemy)
		{
			other->goalentity = other->enemy;
			if (self->spawnflags & 32)
				other->monsterinfo.aiflags |= AI_BRUTAL;
			if (self->spawnflags & 16)
			{
				other->monsterinfo.aiflags |= AI_STAND_GROUND;
				other->monsterinfo.stand (other);
			}
			else
			{
				other->monsterinfo.run (other);
			}
		}
	}

	if (!(self->spawnflags & 6) && (self->pathtarget))
	{
		char *savetarget;

		savetarget = self->target;
		self->target = self->pathtarget;
		G_UseTargets (self, other);
		self->target = savetarget;
	}

	other->movetarget = G_PickTarget(self->target);

	if (!other->goalentity)
		other->goalentity = other->movetarget;

	if (!other->movetarget && !other->enemy)
	{
		other->monsterinfo.pausetime = 0;
		other->monsterinfo.stand (other);
	}
	else if (other->movetarget == other->goalentity)
	{
		VectorSubtract (other->movetarget->s.origin, other->s.origin, v);
		other->ideal_pitch = 0;
		other->ideal_yaw = vectoyaw (v);
	}
}

void SP_target_actor (edict_t *self)
{
	if (!self->targetname)
		gi.dprintf ("%s with no targetname at %s\n", self->classname, vtos(self->s.origin));

	self->solid = SOLID_TRIGGER;
	self->touch = target_actor_touch;
	VectorSet (self->mins, -8, -8, -8);
	VectorSet (self->maxs, 8, 8, 8);
	self->svflags = SVF_NOCLIENT;

	if (self->spawnflags & 3)
	{
		if (!self->speed)
			self->speed = 200;
		if (!st.height)
			st.height = 200;
		if (self->s.angles[YAW] == 0)
			self->s.angles[YAW] = 360;
		G_SetMovedir (self->s.angles, self->movedir);
		self->movedir[2] = st.height;
	}

	gi.linkentity (self);
}
