// g_ai.c

#include "g_local.h"

extern cvar_t	*maxclients;

qboolean	enemy_vis;
qboolean	enemy_infront;
int			enemy_range;
float		enemy_pitch;
float		enemy_yaw;

//============================================================================

// SLUGFILLER--enable both ducking and sliding to avoid projectiles
void M_MonsterDodge (edict_t *self, edict_t *inflictor, edict_t *attacker, float eta, trace_t *tr, qboolean must)
{
	float		r = random();
	float		height;
	qboolean	ducker = false;
	qboolean	dodger = false;
	int			anim;
	float		yaw;

	anim = self->monsterinfo.currentmove->frame[self->s.frame - self->monsterinfo.currentmove->firstframe].animtype;

	// just in case
	if (self->health <= 0)
		return;

	if (self->monsterinfo.duck)
		ducker = true;
	if (!(self->monsterinfo.aiflags & AI_STAND_GROUND))
		dodger = true;

	// got ourselves a new rival
	if (!self->enemy && (attacker->client || attacker->enemy == self))
	{
		self->enemy = attacker;
		if (attacker->svflags & SVF_MONSTER)
		{
			// SLUGFILLER--why is he trying to kill us
			// that's what caused us to try to kill him
			self->chain = attacker->chain;
		}
		VectorCopy (inflictor->s.origin, self->monsterinfo.last_sighting);
		FoundTarget (self);
	}

	// too soon or too late
	if ((eta < 0.1) || (eta > 5))
		return;

	if (ducker && !(self->monsterinfo.aiflags & AI_DUCKED) && anim < MANIM_MELEE)
		height = self->absmax[2]-32;
	else
		height = self->absmax[2];

	// dont duck if its aimed for the legs
	if ((!dodger) && (tr->endpos[2] <= height))
		return;

	if (dodger)
	{
		// ducking won't do us any good or we need to adjust our dodge
		if ((self->monsterinfo.aiflags & AI_DODGING) || (tr->endpos[2] <= height))
		{
			vec3_t right, diff;

			AngleVectors (self->s.angles, NULL, right, NULL);
			VectorSubtract (tr->endpos, self->s.origin, diff);

			if (!(self->monsterinfo.aiflags & AI_DODGE_MUST))
			{
				if (DotProduct (right, diff) < 0)
					self->monsterinfo.lefty = 0;
				else
					self->monsterinfo.lefty = 1;
			}
			yaw = ((self->monsterinfo.lefty)?90:-90);
			if (!M_CheckMove (self, self->s.angles[YAW] + yaw, 32))
			{
				if (!M_CheckMove (self, self->s.angles[YAW] - yaw, 32))
				{
					if (!M_CheckMove (self, self->s.angles[YAW] + yaw, 8))
						self->monsterinfo.lefty = !self->monsterinfo.lefty;
				}
				else
					self->monsterinfo.lefty = !self->monsterinfo.lefty;
			}

			self->monsterinfo.aiflags |= AI_SLIDING;

			// SLUGFILLER--let the monster dodge for a frame
			// then if he isn't in the line of fire he stops,
			// if he's still on the line of fire it will come back here
			// and give it more time for dodging
			self->monsterinfo.dodge_time = level.time + FRAMETIME;

			// start running
			if (eta < 2)
			{
				if ((ducker) && (self->monsterinfo.aiflags & AI_DUCKED))
					self->monsterinfo.unduck(self);

				if (anim <= MANIM_DUCK && anim != MANIM_RUN)
					self->monsterinfo.run (self);
			}
			else if (anim < MANIM_DUCK && anim != MANIM_RUN)
				self->monsterinfo.run (self);

			self->monsterinfo.aiflags |= AI_DODGING;
			if (must)
				self->monsterinfo.aiflags |= AI_DODGE_MUST;
			if (eta < 2)
				return; // don't duck
		}
	}

	if (ducker)
	{
		// get down
		if (anim != MANIM_DUCK && anim < MANIM_MELEE)
			self->monsterinfo.duck (self, attacker, eta);
	}
}

void monster_duck_down (edict_t *self)
{
	if (self->monsterinfo.aiflags & AI_DUCKED)
		return;
	self->monsterinfo.aiflags |= AI_DUCKED;
	self->maxs[2] -= 32;
	self->headmins[2] -= 32;
	self->headmaxs[2] -= 32;
	self->viewheight -= 32;
	self->takedamage = DAMAGE_YES;
	gi.linkentity (self);
}

void monster_duck_hold (edict_t *self)
{
	if (level.time >= self->monsterinfo.pausetime)
		self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	else
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
}

void monster_duck_up (edict_t *self)
{
	if (!(self->monsterinfo.aiflags & AI_DUCKED))
		return;
	self->monsterinfo.aiflags &= ~AI_DUCKED;
	self->maxs[2] += 32;
	self->headmins[2] += 32;
	self->headmaxs[2] += 32;
	self->viewheight += 32;
	self->takedamage = DAMAGE_AIM;
	gi.linkentity (self);
}

//============================================================================


/*

.enemy
Will be world if not currently angry at anyone.

.movetarget
The next path spot to walk toward.  If .enemy, ignore .movetarget.
When an enemy is killed, the monster will try to return to it's path.

.hunt_time
Set to time + something when the player is in sight, but movement straight for
him is blocked.  This causes the monster to use wall following code for
movement direction instead of sighting on the player.

.ideal_pitch
A pitch angle for the aiming direction

.ideal_yaw
A yaw angle of the intended direction, which will be turned towards at up
to 45 deg / state.  If the enemy is in view and hunt_time is not active,
this will be the exact line towards the enemy.

.pausetime
A monster will leave it's stand state and head towards it's .movetarget when
time > .pausetime.

walkmove(angle, speed) primitive is all or nothing
*/

/*
=============
range

returns the range catagorization of an entity reletive to self
0	melee range, will become hostile even if back is turned
1	visibility and infront, or visibility and show hostile
2	infront and show hostile
3	only triggered by damage
=============
*/
int range (edict_t *self, edict_t *other)
{
	vec3_t	v;
	float	len;

	VectorSubtract (self->s.origin, other->s.origin, v);
	len = VectorLength (v);
	if (len < MELEE_DISTANCE)
		return RANGE_MELEE;
	if (len < 500)
		return RANGE_NEAR;
	if (len < 1000)
		return RANGE_MID;
	return RANGE_FAR;
}

/*
=============
visible

returns 1 if the entity is visible to self, even if not infront ()
=============
*/
qboolean visible (edict_t *self, edict_t *other)
{
	vec3_t	spot1;
	vec3_t	spot2;
	trace_t	trace;

	VectorCopy (self->s.origin, spot1);
	spot1[2] += self->viewheight;
	VectorCopy (other->s.origin, spot2);
	spot2[2] += other->viewheight;
	trace = gi.trace (spot1, vec3_origin, vec3_origin, spot2, self, MASK_OPAQUE);
	
	if (trace.fraction == 1.0)
		return true;
	return false;
}


/*
=============
infront

returns 1 if the entity is in front (in sight) of self
=============
*/
qboolean infront (edict_t *self, edict_t *other)
{
	vec3_t	vec;
	float	dot;
	vec3_t	forward;

	AngleVectors (self->aimangles, forward, NULL, NULL);
	VectorSubtract (other->s.origin, self->s.origin, vec);
	VectorNormalize (vec);
	dot = DotProduct (vec, forward);

	if (dot > 0.3)
		return true;
	return false;
}


//============================================================================

void HuntTarget (edict_t *self)
{
	self->goalentity = self->enemy;
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.stand (self);
	else
		self->monsterinfo.run (self);
	// SLUGFILLER--don't wait a while before first attack
	// shoot first, ask questions later
}

void FoundTarget (edict_t *self)
{
	self->show_hostile = level.time + 1;		// wake up other monsters

	// SLUGFILLER--reinforce them before they reinforce us
	if (!(self->monsterinfo.aiflags & AI_SOUND_TARGET))
		Reinforce (self);

	if (!self->combattarget)
	{
		HuntTarget (self);
		return;
	}

	self->goalentity = self->movetarget = G_PickTarget(self->combattarget);
	if (!self->movetarget)
	{
		self->goalentity = self->movetarget = self->enemy;
		HuntTarget (self);
		gi.dprintf("%s at %s, combattarget %s not found\n", self->classname, vtos(self->s.origin), self->combattarget);
		return;
	}

	// clear out our combattarget, these are a one shot deal
	self->combattarget = NULL;
	self->monsterinfo.aiflags |= AI_COMBAT_POINT;

	// clear the targetname, that point is ours!
	self->movetarget->targetname = NULL;
	self->monsterinfo.pausetime = 0;

	// run for it
	self->monsterinfo.run (self);
}

// SLUGFILLER--if another monster hurts us, were calling friends for help
void Reinforce (edict_t *self)
{
	edict_t	*ent;
	int		r;

	// SLUGFILLER--were already telling other monsters about clients so
	if (self->enemy->client)
		return;

	for (ent = g_edicts ; ent < &g_edicts[globals.num_edicts] ; ent++)
	{
		if (ent == self)
			continue;

		if (!ent->inuse)
			continue;

		if (!(ent->svflags & SVF_MONSTER))
			continue;

		if (ent->deadflag == DEAD_DEAD)
			continue;

		if (ent->enemy == self->enemy)
			return;

		// don't bother a monster which is already fighting a monster
		if (ent->enemy && (ent->enemy->svflags & SVF_MONSTER))
			return;

		if (ent->spawnflags & 1)
			continue;

		if (strcmp(self->classname, ent->classname) != 0)
			continue;

		r = range (ent, self);

		if (r == RANGE_FAR)
			continue;

		if (!visible (ent, self) && !visible (ent, self->enemy))
			continue;

		ent->enemy = self->enemy;
		ent->monsterinfo.aiflags &= ~AI_SOUND_TARGET;

		// SLUGFILLER--we are helping our buddy to kill another monster(maybe)
		// the same guy who caused our buddy to attack that monster
		// caused us to attack that monster
		ent->chain = self->chain;

		if (visible (ent, self->enemy))
			VectorCopy (self->enemy->s.origin, ent->monsterinfo.last_sighting);
		else
			VectorCopy (self->s.origin, ent->monsterinfo.last_sighting);
		FoundTarget (ent);

		if (visible (ent, self->enemy) && ent->monsterinfo.sight)
			ent->monsterinfo.sight (ent, ent->enemy);
	}
}


/*
===========
FindTarget

Self is currently not attacking anything, so try to find a target

Returns TRUE if an enemy was sighted

When a player fires a missile, the point of impact becomes a fakeplayer so
that monsters that see the impact will respond as if they had seen the
player.
============
*/
qboolean FindTarget (edict_t *self)
{
	edict_t		*client;
	int			r, i;
	vec3_t	temp;

	if (self->monsterinfo.aiflags & AI_GOOD_GUY)
	{
		if (self->goalentity && self->goalentity->inuse && self->goalentity->classname)
		{
			if (strcmp(self->goalentity->classname, "target_actor") == 0)
				return false;
		}

		//FIXME look for monsters?
		return false;
	}

	// if we're going to a combat point, just proceed
	if (self->monsterinfo.aiflags & AI_COMBAT_POINT)
		return false;

	// SLUGFILLER--totaly revised code, removing hacks, old behavior remains

	// SLUGFILLER--search for a client
	for (i=0 ; i<maxclients->value ; i++)
	{
		client = g_edicts + 1 + i;

		if (!client->inuse)
			continue;

		if (client->client->resp.exitframe)
			continue;

		if (client->health <= 0)
			continue;

		if (client->flags & FL_NOTARGET)
			continue;

		r = range (self, client);

		if (r == RANGE_FAR)
			continue;

		if (client->light_level <= 5)
			continue;

		if (!visible (self, client))
			continue;

		if (r == RANGE_NEAR || r == RANGE_MID)
			if (!infront (self, client))
				continue;

		self->monsterinfo.aiflags &= ~AI_SOUND_TARGET;

		if (client == self->enemy)
			return true;

		self->enemy = client;

		// TITANIUM--we looked for that target ourselves
		self->chain = NULL;
		VectorCopy (self->enemy->s.origin, self->monsterinfo.last_sighting);
		FoundTarget (self);

		if (self->monsterinfo.sight)
			self->monsterinfo.sight (self, self->enemy);

		return true;
	}

	// SLUGFILLER--search for an angry monster
	for (client = g_edicts ; client < &g_edicts[globals.num_edicts] ; client++)
	{
		if (client == self)
			continue;

		if (!client->inuse)
			continue;

		if (!(client->svflags & SVF_MONSTER))
			continue;

		if (client->health <= 0)
			continue;

		if (!client->enemy)
			continue;

		if (!client->enemy->client)
			continue;

		if (client->enemy->health <= 0)
			continue;

		if (client->enemy->flags & FL_NOTARGET)
			continue;

		r = range (self, client);

		if (r == RANGE_FAR)
			continue;

		if ((!visible (self, client) || (self->spawnflags & 1)) && !visible (self, client->enemy))
			continue;

		if (r == RANGE_NEAR)
		{
			if (client->show_hostile < level.time && !infront (self, client))
				continue;
		}
		else if (r == RANGE_MID)
		{
			if (!infront (self, client))
				continue;
		}

		// SLUGFILLER--we looked for that target ourselves
		self->chain = NULL;
		if (visible (self, client->enemy))
		{
			self->monsterinfo.aiflags &= ~AI_SOUND_TARGET;

			if (client->enemy == self->enemy)
				return true;

			self->enemy = client->enemy;

			VectorCopy (self->enemy->s.origin, self->monsterinfo.last_sighting);
			FoundTarget (self);

			if (self->monsterinfo.sight)
				self->monsterinfo.sight (self, self->enemy);
		}
		else
		{
			self->enemy = client;
			self->monsterinfo.aiflags |= AI_SOUND_TARGET;

			VectorCopy (self->enemy->s.origin, self->monsterinfo.last_sighting);
			FoundTarget (self);
		}
		return true;
	}

	// SLUGFILLER--heared something
	for (client = g_edicts ; client < &g_edicts[globals.num_edicts] ; client++)
	{
		if (!client->inuse)
			return false;

		if (strcmp (client->classname, "player_noise1") != 0 && (self->enemy || (self->spawnflags & 1) || strcmp (client->classname, "player_noise2") != 0))
			continue;

		if (client->owner->flags & FL_NOTARGET)
			return false;

		if (self->spawnflags & 1)
		{
			if (!visible (self, client))
				return false;
		}
		else
		{
			if (!gi.inPHS(self->s.origin, client->s.origin))
				return false;
		}

		VectorSubtract (client->s.origin, self->s.origin, temp);

		if (VectorLength(temp) > 1000)	// too far to hear
		{
			return false;
		}

		// check area portals - if they are different and not connected then we can't hear it
		if (client->areanum != self->areanum)
			if (!gi.AreasConnected(self->areanum, client->areanum))
				return false;

		self->ideal_pitch = 0;
		self->ideal_yaw = vectoyaw(temp);
		M_ChangeAngles (self);

		// hunt the sound for a bit; hopefully find the real player
		self->monsterinfo.aiflags |= AI_SOUND_TARGET;
		self->enemy = client;

		// SLUGFILLER--we looked for that target ourselves
		self->chain = NULL;
		VectorCopy (self->enemy->s.origin, self->monsterinfo.last_sighting);
		FoundTarget (self);

		return true;
	}

	return false;
}


//=============================================================================

/*
============
FacingIdeal

============
*/
qboolean FacingIdeal(edict_t *self)
{
	float	delta;

	delta = anglemod(self->s.angles[YAW] - self->ideal_yaw);
	if (delta > 45 && delta < 315)
		return false;
	return true;
}


//=============================================================================

qboolean M_CheckAttack (edict_t *self)
{
	vec3_t		spot1, spot2;
	float		chance;
	trace_t		tr;
	qboolean	is_sliding;

	if (self->enemy->health > 0)
	{
	// see if any entities are in the way of the shot
		VectorCopy (self->s.origin, spot1);
		spot1[2] += self->viewheight;
		VectorCopy (self->enemy->s.origin, spot2);
		spot2[2] += self->enemy->viewheight;

		tr = gi.trace (spot1, NULL, NULL, spot2, self, CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_SLIME|CONTENTS_LAVA|CONTENTS_WINDOW);

		// do we have a clear shot?
		if (tr.ent != self->enemy)
			return false;

		// SLUGFILLER--is the enemy on sight?
		if (!infront (self, self->enemy))
			return false;
	}
	
	// melee attack
	if (enemy_range == RANGE_MELEE)
	{
		// SLUGFILLER--always melee, even in easy mode
		// so monsters won't try to run into their enemy
		if (self->monsterinfo.melee)
			self->monsterinfo.attack_state = AS_MELEE;
		else
			self->monsterinfo.attack_state = AS_MISSILE;
		return true;
	}
	
	// SLUGFILLER--melee monsters charge the enemy rather than shooting him
		
	if (level.time < self->monsterinfo.attack_finished)
		return false;
		
	if (enemy_range == RANGE_FAR)
		return false;

	// SLUGFILLER--can strafe depending on skill
	// dodge slide is handled somewhere else
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		chance = 0;
	else if (skill->value == 0)
		chance = 0.3;
	else if (skill->value >= 2)
		chance = 0.7;
	else
		chance = 0.5;

	is_sliding = (random() < chance);

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		chance = 0.7; // SLUGFILLER--enemy in sight - fire
	}
	else if (enemy_range == RANGE_MID)
	{
		chance = 0.2; // SLUGFILLER--enemy in close range - charge
	}
	else if (enemy_range == RANGE_NEAR)
	{
		chance = 0.4; // SLUGFILLER--enemy in optimal range - fire at will
	}

	if (skill->value == 0)
		chance *= 0.5;
	else if (skill->value >= 2)
		chance *= 2;

	if (random () < chance)
	{
		// SLUGFILLER--fire
		if (is_sliding)
			self->monsterinfo.aiflags |= AI_SLIDING;
		if (self->monsterinfo.attack)
			self->monsterinfo.attack_state = AS_MISSILE;
		else
			self->monsterinfo.attack_state = AS_MELEE;
		self->monsterinfo.attack_finished = level.time + 2*random();
		return true;
	}

	// SLUGFILLER--wait a bit more
	if (is_sliding)
		self->monsterinfo.aiflags |= AI_SLIDING;

	return false;
}


/*
=============
ai_run_slide

Strafe sideways, but stay at aproximately the same range
=============
*/
void ai_run_slide(edict_t *self, float distance)
{
	float	ofs;
	
	if (self->monsterinfo.lefty)
		ofs = 90;
	else
		ofs = -90;
	
	if (M_walkmove (self, self->s.angles[YAW] + ofs, distance))
		return;

	if (self->monsterinfo.aiflags & AI_DODGE_MUST)
	{
		self->monsterinfo.lefty = 1 - self->monsterinfo.lefty;
		if (M_walkmove (self, self->s.angles[YAW] - ofs, distance))
			return;
	}

	// SLUGFILLER--we can't strafe in here lets keep going
	self->monsterinfo.aiflags &= ~AI_SLIDING;

	// SLUGFILLER--we're stuck between two walls, can't dodge anyway
	self->monsterinfo.aiflags &= ~AI_DODGING;
	self->monsterinfo.aiflags &= ~AI_DODGE_MUST;
}


/*
=============
ai_checkenemy

Makes sure that there's a valid, living enemy
=============
*/
qboolean ai_checkenemy (edict_t *self)
{
	vec3_t		temp;
	qboolean	hesDeadJim;

	enemy_vis = false;

	// SLUGFILLER--combat points are valid enemies
	if (self->goalentity)
	{
		if (self->monsterinfo.aiflags & AI_COMBAT_POINT)
			return false;
	}

// see if the enemy is dead
	hesDeadJim = false;
	if ((!self->enemy) || (!self->enemy->inuse) || ((self->enemy->client) && (self->enemy->client->resp.exitframe)))
	{
		hesDeadJim = true;
	}
	else if (self->monsterinfo.aiflags & AI_MEDIC)
	{
		if (self->enemy->health > 0)
		{
			hesDeadJim = true;
			self->monsterinfo.aiflags &= ~AI_MEDIC;
		}
	}
	else if (!(self->monsterinfo.aiflags & AI_SOUND_TARGET))
	{
		if (self->monsterinfo.aiflags & AI_BRUTAL)
		{
			if (self->enemy->gib_health <= 0)
				hesDeadJim = true;
		}
		else
		{
			if (self->enemy->health <= 0)
				hesDeadJim = true;
		}
	}

	if (hesDeadJim)
	{
		// SLUGFILLER--enemy is dead, therefore not valid
		if (self->enemy && self->enemy->inuse && !((self->enemy->client) && (self->enemy->client->resp.exitframe)) && !visible(self, self->enemy))
			return false;
		// SLUGFILLER--if sees dead foe, replacement is in order
		self->enemy = NULL;
		// SLUGFILLER--if dead guy was a monster, chase last fought client
		if (self->oldenemy && self->oldenemy->inuse && (!self->oldenemy->client || !self->oldenemy->client->resp.exitframe) && self->oldenemy->health > 0 && visible (self, self->oldenemy))
		{
			self->enemy = self->oldenemy;
			self->oldenemy = NULL;
			HuntTarget (self);
		}
		return false;
	}

	// SLUGFILLER--it's a sound target, not an enemy
	if (self->monsterinfo.aiflags & AI_SOUND_TARGET)
		return false;

	// check knowledge of enemy
	enemy_vis = visible(self, self->enemy);
	if (enemy_vis)
	{
		self->monsterinfo.search_time = level.time + 5;
		VectorCopy (self->enemy->s.origin, self->monsterinfo.last_sighting);
	}

	enemy_infront = infront(self, self->enemy);
	enemy_range = range(self, self->enemy);
	VectorSubtract (self->enemy->s.origin, self->s.origin, temp);
	temp[2] += self->enemy->viewheight - self->viewheight;
	enemy_pitch = vectopitch(temp);
	enemy_yaw = vectoyaw(temp);

	// SLUGFILLER--enemy must be visible to be valid
	return enemy_vis;
}


/*
=============
ai_checkattack

Decides if we're going to attack or do something else
used by ai_run and ai_stand
=============
*/
qboolean ai_checkattack (edict_t *self)
{
	qboolean	retval;

	// check enemy availability
	if (!ai_checkenemy (self))
		return false;

	retval = self->monsterinfo.checkattack (self);
	
	return retval;
}


void M_RunThink (edict_t *self)
{
	vec3_t		v;
	edict_t		*tempgoal;
	edict_t		*save;
	qboolean	new;
	float		d1, d2;
	trace_t		tr;
	vec3_t		v_forward, v_right;
	float		left, center, right;
	vec3_t		left_target, right_target;
	float		dist;
	int			animtype;

	dist = self->monsterinfo.speed;
	animtype = self->monsterinfo.currentmove->frame[self->s.frame - self->monsterinfo.currentmove->firstframe].animtype;

	// SLUGFILLER--it's dead, therefore it doesn't think
	if (animtype == MANIM_DEATH)
		return;

	// if we're going to a combat point, just proceed
	if (self->monsterinfo.aiflags & AI_COMBAT_POINT)
	{
		M_MoveToGoal (self, dist);
		return;
	}

	// SLUGFILLER--make search noises, and set animations
	if (!self->enemy)
	{
		if (self->movetarget)
		{
			if ((self->monsterinfo.search) && (level.time > self->monsterinfo.idle_time))
			{
				if (self->monsterinfo.idle_time)
				{
					self->monsterinfo.search (self);
					self->monsterinfo.idle_time = level.time + 15 + random() * 15;
				}
				else
				{
					self->monsterinfo.idle_time = level.time + random() * 15;
				}
			}
			if (animtype == MANIM_RUN || animtype == MANIM_STAND)
			{
				self->goalentity = self->movetarget;
				self->monsterinfo.walk (self);
			}
		}
		else
		{
			if (!(self->spawnflags & 1) && (self->monsterinfo.idle) && (level.time > self->monsterinfo.idle_time))
			{
				if (self->monsterinfo.idle_time)
				{
					self->monsterinfo.idle (self);
					self->monsterinfo.idle_time = level.time + 15 + random() * 15;
				}
				else
				{
					self->monsterinfo.idle_time = level.time + random() * 15;
				}
			}
			if (animtype == MANIM_RUN || animtype == MANIM_WALK)
			{
				self->monsterinfo.pausetime = 0;
				self->monsterinfo.stand (self);
			}
		}
	}

	// SLUGFILLER--search for other enemies
	if (!ai_checkenemy (self))
		FindTarget (self);
	else
	{
		self->ideal_pitch = enemy_pitch;
		self->ideal_yaw = enemy_yaw;
	}

	if (level.time > self->monsterinfo.dodge_time)
	{
		if (!(self->monsterinfo.aiflags & AI_DODGE_MUST))
			self->monsterinfo.aiflags &= ~AI_DODGING;
		else
			self->monsterinfo.dodge_time = level.time + 0.1;
		self->monsterinfo.aiflags &= ~AI_DODGE_MUST;
	}

	if (!self->monsterinfo.attack_state && animtype != MANIM_MELEE && animtype != MANIM_MISSILE && animtype != MANIM_DUCK)
	{
		self->monsterinfo.aiflags &= ~AI_SLIDING;
		ai_checkattack (self);
	}

	// SLUGFILLER--always slide if dodging
	if (self->monsterinfo.aiflags & AI_DODGING)
		self->monsterinfo.aiflags |= AI_SLIDING;

	// SLUGFILLER--fire?
	if (self->monsterinfo.attack_state && self->enemy)
	{
		if (self->monsterinfo.attack_state == AS_MISSILE)
		{
			if (FacingIdeal(self))
			{
				self->monsterinfo.attack (self);
				self->monsterinfo.attack_state = AS_NONE;
			}
		}
		else if (self->monsterinfo.attack_state == AS_MELEE)
		{
			if (FacingIdeal(self) && range(self, self->enemy) == RANGE_MELEE)
			{
				self->monsterinfo.melee (self);
				self->monsterinfo.attack_state = AS_NONE;
			}
		}
	}

	if (self->monsterinfo.aiflags & AI_SLIDING)
	{
		ai_run_slide (self, dist);
	}

	if (enemy_vis)
	{
		if (self->monsterinfo.attack_state == AS_MELEE || animtype == MANIM_MELEE || range(self, self->enemy) > RANGE_NEAR)
		{
			// SLUGFILLER--either charging the enemy, or trying to get near it
			if (animtype == MANIM_STAND)
				self->monsterinfo.run (self);
			M_MoveToGoal (self, dist);
		}
		else if (range(self, self->enemy) == RANGE_NEAR)
		{
			// SLUGFILLER--optimal distance, either slide for dodging, or stand still
			if (self->monsterinfo.aiflags & AI_SLIDING)
			{
				if (animtype == MANIM_STAND)
					self->monsterinfo.run (self);
			}
			else
			{
				if (animtype == MANIM_RUN)
					self->monsterinfo.stand (self);
			}
			M_ChangeAngles (self);
		}
		else
		{
			// SLUGFILLER--too close, and we're not attacking, better step back
			M_MoveToGoal (self, -dist);
		}
		self->monsterinfo.aiflags &= ~AI_LOST_SIGHT;
		VectorCopy (self->enemy->s.origin, self->monsterinfo.last_sighting);
		if (!(self->monsterinfo.aiflags & AI_SOUND_TARGET))
			Reinforce (self);	// SLUGFILLER--pick up some help on the way
		return;
	}

	// SLUGFILLER--we are sliding, probably dodging, we are too busy to search for the player
	if (self->monsterinfo.aiflags & AI_SLIDING)
		return;

	// SLUGFILLER--no enemy, just walk
	if (!self->enemy)
	{
		// SLUGFILLER--when walking use walkspeed instead of speed
		if (self->goalentity)
			M_MoveToGoal (self, self->monsterinfo.walkspeed);
		return;
	}

	// SLUGFILLER--go to last sighting

	if ((self->monsterinfo.search_time) && (level.time > self->monsterinfo.search_time))
	{
		self->monsterinfo.search_time = 0;
		// SLUGFILLER--give up on enemy, for us he is dead
		self->enemy = NULL;
		return;
	}

	save = self->goalentity;
	tempgoal = G_Spawn();
	self->goalentity = tempgoal;

	new = false;

	if (!(self->monsterinfo.aiflags & AI_LOST_SIGHT))
	{
		// just lost sight of the player, decide where to go first
//		dprint("lost sight of player, last seen at "); dprint(vtos(self.last_sighting)); dprint("\n");
		self->monsterinfo.aiflags |= (AI_LOST_SIGHT | AI_PURSUIT_LAST_SEEN);
		self->monsterinfo.aiflags &= ~(AI_PURSUE_NEXT | AI_PURSUE_TEMP);
		new = true;
	}

	if (self->monsterinfo.aiflags & AI_PURSUE_NEXT)
	{
		self->monsterinfo.aiflags &= ~AI_PURSUE_NEXT;
//		dprint("reached current goal: "); dprint(vtos(self.origin)); dprint(" "); dprint(vtos(self.last_sighting)); dprint(" "); dprint(ftos(vlen(self.origin - self.last_sighting))); dprint("\n");

		// give ourself more time since we got this far
		self->monsterinfo.search_time = level.time + 5;

		if (self->monsterinfo.aiflags & AI_PURSUE_TEMP)
		{
//			dprint("was temp goal; retrying original\n");
			self->monsterinfo.aiflags &= ~AI_PURSUE_TEMP;
			VectorCopy (self->monsterinfo.saved_goal, self->monsterinfo.last_sighting);
			new = true;
		}
		else if (self->monsterinfo.aiflags & AI_PURSUIT_LAST_SEEN)
		{
			self->monsterinfo.aiflags &= ~AI_PURSUIT_LAST_SEEN;
			// TITANIUM--wait a bit then give up
			self->monsterinfo.search_time = level.time + 0.5;
		}
	}

	VectorSubtract (self->s.origin, self->monsterinfo.last_sighting, v);
	d1 = VectorLength(v);
	if (d1 <= dist)
	{
		self->monsterinfo.aiflags |= AI_PURSUE_NEXT;
		dist = d1;
	}

	VectorCopy (self->monsterinfo.last_sighting, self->goalentity->s.origin);

	if (new)
	{
//		gi.dprintf("checking for course correction\n");

		tr = gi.trace(self->s.origin, self->mins, self->maxs, self->monsterinfo.last_sighting, self, MASK_PLAYERSOLID);
		if (tr.fraction < 1)
		{
			VectorSubtract (self->goalentity->s.origin, self->s.origin, v);
			d1 = VectorLength(v);
			center = tr.fraction;
			d2 = d1 * ((center+1)/2);
			self->s.angles[PITCH] = self->ideal_pitch = 0;
			self->s.angles[YAW] = self->ideal_yaw = vectoyaw(v);
			AngleVectors(self->s.angles, v_forward, v_right, NULL);

			VectorSet(v, d2, -16, 0);
			G_ProjectSource (self->s.origin, v, v_forward, v_right, left_target);
			tr = gi.trace(self->s.origin, self->mins, self->maxs, left_target, self, MASK_PLAYERSOLID);
			left = tr.fraction;

			VectorSet(v, d2, 16, 0);
			G_ProjectSource (self->s.origin, v, v_forward, v_right, right_target);
			tr = gi.trace(self->s.origin, self->mins, self->maxs, right_target, self, MASK_PLAYERSOLID);
			right = tr.fraction;

			center = (d1*center)/d2;
			if (left >= center && left > right)
			{
				if (left < 1)
				{
					VectorSet(v, d2 * left * 0.5, -16, 0);
					G_ProjectSource (self->s.origin, v, v_forward, v_right, left_target);
//					gi.dprintf("incomplete path, go part way and adjust again\n");
				}
				VectorCopy (self->monsterinfo.last_sighting, self->monsterinfo.saved_goal);
				self->monsterinfo.aiflags |= AI_PURSUE_TEMP;
				VectorCopy (left_target, self->goalentity->s.origin);
				VectorCopy (left_target, self->monsterinfo.last_sighting);
				VectorSubtract (self->goalentity->s.origin, self->s.origin, v);
				self->s.angles[PITCH] = self->ideal_pitch = 0;
				self->s.angles[YAW] = self->ideal_yaw = vectoyaw(v);
//				gi.dprintf("adjusted left\n");
//				debug_drawline(self.origin, self.last_sighting, 152);
			}
			else if (right >= center && right > left)
			{
				if (right < 1)
				{
					VectorSet(v, d2 * right * 0.5, 16, 0);
					G_ProjectSource (self->s.origin, v, v_forward, v_right, right_target);
//					gi.dprintf("incomplete path, go part way and adjust again\n");
				}
				VectorCopy (self->monsterinfo.last_sighting, self->monsterinfo.saved_goal);
				self->monsterinfo.aiflags |= AI_PURSUE_TEMP;
				VectorCopy (right_target, self->goalentity->s.origin);
				VectorCopy (right_target, self->monsterinfo.last_sighting);
				VectorSubtract (self->goalentity->s.origin, self->s.origin, v);
				self->s.angles[PITCH] = self->ideal_pitch = 0;
				self->s.angles[YAW] = self->ideal_yaw = vectoyaw(v);
//				gi.dprintf("adjusted right\n");
//				debug_drawline(self.origin, self.last_sighting, 152);
			}
		}
//		else gi.dprintf("course was fine\n");
	}

	M_MoveToGoal (self, dist);

	G_FreeEdict(tempgoal);

	if (self)
		self->goalentity = save;
}
