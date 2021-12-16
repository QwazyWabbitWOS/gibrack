// g_turret.c

#include "g_local.h"


void AnglesNormalize(vec3_t vec)
{
	while(vec[0] > 360)
		vec[0] -= 360;
	while(vec[0] < 0)
		vec[0] += 360;
	while(vec[1] > 360)
		vec[1] -= 360;
	while(vec[1] < 0)
		vec[1] += 360;
}

float SnapToEights(float x)
{
	x *= 8.0;
	if (x > 0.0)
		x += 0.5;
	else
		x -= 0.5;
	return 0.125 * (int)x;
}


void turret_blocked(edict_t *self, edict_t *other)
{
	edict_t	*attacker;

	if (other->takedamage)
	{
		if (self->teammaster->owner)
			attacker = self->teammaster->owner;
		else
			attacker = self->teammaster;
		T_Damage (other, self, attacker, vec3_origin, other->s.origin, vec3_origin, self->teammaster->dmg, self->teammaster->dmg, 10, DAMAGE_NO_BLOOD, MOD_CRUSH);
	}
}

/*QUAKED turret_breach (0 0 0) ?
This portion of the turret can change both pitch and yaw.
The model  should be made with a flat pitch.
It (and the associated base) need to be oriented towards 0.
Use "angle" to set the starting angle.

"wait"		time between shots : default 1.0
"speed"		default 50
"dmg"		default 10
"angle"		point this forward
"target"	point this at an info_notnull at the muzzle tip
"minpitch"	min acceptable pitch angle : default -30
"maxpitch"	max acceptable pitch angle : default 30
"minyaw"	min acceptable yaw angle   : default 0
"maxyaw"	max acceptable yaw angle   : default 360
*/

void turret_breach_fire (edict_t *self)
{
	vec3_t	f, r, u;
	vec3_t	start;
	int		damage;
	int		speed;

	AngleVectors (self->s.angles, f, r, u);
	VectorMA (self->s.origin, self->move_origin[0], f, start);
	VectorMA (start, self->move_origin[1], r, start);
	VectorMA (start, self->move_origin[2], u, start);

	damage = 100 + random() * 50;
	speed = 550 + 50 * skill->value;
	fire_rocket (self->teammaster->owner, start, f, damage, speed, 150, damage);
	gi.positioned_sound (start, self, CHAN_WEAPON, gi.soundindex("weapons/rocklf1a.wav"), 1, ATTN_NORM, 0);
}

void turret_breach_think (edict_t *self)
{
	edict_t	*ent;
	vec3_t	current_angles;
	vec3_t	delta;

	VectorCopy (self->s.angles, current_angles);
	AnglesNormalize(current_angles);

	AnglesNormalize(self->move_angles);
	if (self->move_angles[PITCH] > 180)
		self->move_angles[PITCH] -= 360;

	// clamp angles to mins & maxs
	if (self->move_angles[PITCH] > self->pos1[PITCH])
		self->move_angles[PITCH] = self->pos1[PITCH];
	else if (self->move_angles[PITCH] < self->pos2[PITCH])
		self->move_angles[PITCH] = self->pos2[PITCH];

	if ((self->move_angles[YAW] < self->pos1[YAW]) || (self->move_angles[YAW] > self->pos2[YAW]))
	{
		float	dmin, dmax;

		dmin = fabs(self->pos1[YAW] - self->move_angles[YAW]);
		if (dmin < -180)
			dmin += 360;
		else if (dmin > 180)
			dmin -= 360;
		dmax = fabs(self->pos2[YAW] - self->move_angles[YAW]);
		if (dmax < -180)
			dmax += 360;
		else if (dmax > 180)
			dmax -= 360;
		if (fabs(dmin) < fabs(dmax))
			self->move_angles[YAW] = self->pos1[YAW];
		else
			self->move_angles[YAW] = self->pos2[YAW];
	}

	VectorSubtract (self->move_angles, current_angles, delta);
	if (delta[0] < -180)
		delta[0] += 360;
	else if (delta[0] > 180)
		delta[0] -= 360;
	if (delta[1] < -180)
		delta[1] += 360;
	else if (delta[1] > 180)
		delta[1] -= 360;
	delta[2] = 0;

	if (delta[0] > self->speed * FRAMETIME)
		delta[0] = self->speed * FRAMETIME;
	if (delta[0] < -1 * self->speed * FRAMETIME)
		delta[0] = -1 * self->speed * FRAMETIME;
	if (delta[1] > self->speed * FRAMETIME)
		delta[1] = self->speed * FRAMETIME;
	if (delta[1] < -1 * self->speed * FRAMETIME)
		delta[1] = -1 * self->speed * FRAMETIME;

	VectorScale (delta, 1.0/FRAMETIME, self->avelocity);

	// SLUGFILLER--rotate all linked turret bases
	for (ent = self->teammaster; ent; ent = ent->teamchain)
		if (Q_stricmp(ent->classname, "turret_base") == 0)
			ent->avelocity[YAW] = self->avelocity[YAW];

	// if we have a driver, adjust his velocities
	if (self->owner)
	{
		float	angle;
		float	target_z;
		float	diff;
		vec3_t	target;
		vec3_t	dir;

		// angular is easy, just copy ours
		self->owner->avelocity[0] = self->avelocity[0];
		self->owner->avelocity[1] = self->avelocity[1];

		// x & y
		angle = self->s.angles[1] + self->owner->move_origin[1];
		angle *= (M_PI*2 / 360);
		target[0] = SnapToEights(self->s.origin[0] + cos(angle) * self->owner->move_origin[0]);
		target[1] = SnapToEights(self->s.origin[1] + sin(angle) * self->owner->move_origin[0]);
		target[2] = self->owner->s.origin[2];

		VectorSubtract (target, self->owner->s.origin, dir);
		self->owner->velocity[0] = dir[0] * 1.0 / FRAMETIME;
		self->owner->velocity[1] = dir[1] * 1.0 / FRAMETIME;

		// z
		angle = self->s.angles[PITCH] * (M_PI*2 / 360);
		target_z = SnapToEights(self->s.origin[2] + self->owner->move_origin[0] * tan(angle) + self->owner->move_origin[2]);

		diff = target_z - self->owner->s.origin[2];
		self->owner->velocity[2] = diff * 1.0 / FRAMETIME;

		if (self->spawnflags & 65536)
		{
			turret_breach_fire (self);
			self->spawnflags &= ~65536;
		}
	}
}

void turret_breach_finish_init (edict_t *self)
{
	// get and save info for muzzle location
	if (!self->target)
	{
		gi.dprintf("%s at %s needs a target\n", self->classname, vtos(self->s.origin));
	}
	else
	{
		self->target_ent = G_PickTarget (self->target);
		VectorSubtract (self->target_ent->s.origin, self->s.origin, self->move_origin);
		G_FreeEdict(self->target_ent);
	}

	self->teammaster->dmg = self->dmg;
	// SLUGFILLER--when blocked bmodels don't think, it will be much safer
	// as a prethink
	self->prethink = turret_breach_think;
}

void SP_turret_breach (edict_t *self)
{
	self->solid = SOLID_BSP;
	self->movetype = MOVETYPE_PUSH;
	gi.setmodel (self, self->model);

	if (!self->wait)
		self->wait = 1.0;
	if (!self->speed)
		self->speed = 50;
	if (!self->dmg)
		self->dmg = 10;

	if (!st.minpitch)
		st.minpitch = -30;
	if (!st.maxpitch)
		st.maxpitch = 30;
	if (!st.maxyaw)
		st.maxyaw = 360;

	self->pos1[PITCH] = -1 * st.minpitch;
	self->pos1[YAW]   = st.minyaw;
	self->pos2[PITCH] = -1 * st.maxpitch;
	self->pos2[YAW]   = st.maxyaw;

	self->ideal_yaw = self->s.angles[YAW];
	self->move_angles[YAW] = self->ideal_yaw;

	self->blocked = turret_blocked;

	self->think = turret_breach_finish_init;
	self->nextthink = level.time + FRAMETIME;
	gi.linkentity (self);
}


/*QUAKED turret_base (0 0 0) ?
This portion of the turret changes yaw only.
MUST be teamed with a turret_breach.
*/

void SP_turret_base (edict_t *self)
{
	self->solid = SOLID_BSP;
	self->movetype = MOVETYPE_PUSH;
	gi.setmodel (self, self->model);
	self->blocked = turret_blocked;
	gi.linkentity (self);
}


/*QUAKED turret_driver (1 .5 0) (-16 -16 -24) (16 16 32)
Must NOT be on the team with the rest of the turret parts.
Instead it must target the turret_breach.
*/

void infantry_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);
void infantry_stand (edict_t *self);
void monster_use (edict_t *self, edict_t *other, edict_t *activator);

void ED_CallSpawn (edict_t *ent);

void turret_driver_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	edict_t	*ent;
	float	angle;

	// SLUGFILLER--stop the ai
	self->prethink = NULL;
	
	// SLUGFILLER--stop the gun
	VectorCopy (self->target_ent->s.angles, self->target_ent->move_angles);
	for (ent = self->target_ent->teammaster; ent; ent = ent->teamchain)
		VectorClear (ent->avelocity);

	// remove the driver from the end of them team chain
	for (ent = self->target_ent->teammaster; ent->teamchain != self; ent = ent->teamchain)
		;
	ent->teamchain = NULL;
	self->teammaster = NULL;
	self->flags &= ~FL_TEAMSLAVE;

	self->target_ent->owner = NULL;
	self->target_ent->teammaster->owner = NULL;

	// SLUGFILLER--add a seat so a player can take over
	ent = G_Spawn ();
	ent->classname = "turret_seat";
	ent->target = self->target;
	angle = self->move_origin[1];
	angle *= (M_PI*2 / 360);
	ent->s.origin[0] = self->target_ent->s.origin[0] + cos(angle) * self->move_origin[0];
	ent->s.origin[1] = self->target_ent->s.origin[1] + sin(angle) * self->move_origin[0];
	ent->s.origin[2] = self->target_ent->s.origin[2] + self->move_origin[2];
	ED_CallSpawn (ent);
	if (ent->nextthink > 0)
	{
		ent->nextthink = 0;
		ent->think (ent);
	}

	infantry_die (self, inflictor, attacker, damage, point);
}

qboolean FindTarget (edict_t *self);

void turret_driver_think (edict_t *self)
{
	vec3_t	target;
	vec3_t	dir;
	float	reaction_time;

	// SLUGFILLER--fire rate is determind by skill-in hard the turret driver
	// actually shoots faster than the player
	reaction_time = (3 - skill->value) * 0.5 * self->target_ent->wait;

	if (self->enemy && (!self->enemy->inuse || self->enemy->health <= 0))
		self->enemy = NULL;

	if (!self->enemy)
	{
		if (!FindTarget (self))
			return;
		self->monsterinfo.aiflags &= ~AI_LOST_SIGHT;
	}
	else
	{
		if (visible (self, self->enemy))
		{
			if (self->monsterinfo.aiflags & AI_LOST_SIGHT)
				self->monsterinfo.aiflags &= ~AI_LOST_SIGHT;
		}
		else
		{
			self->monsterinfo.aiflags |= AI_LOST_SIGHT;
			return;
		}
	}

	// let the turret know where we want it to aim
	VectorCopy (self->enemy->s.origin, target);
	target[2] += self->enemy->viewheight;
	VectorSubtract (target, self->target_ent->s.origin, dir);
	vectoangles (dir, self->target_ent->move_angles);

	// SLUGFILLER--decide IF we should shoot
	if (self->monsterinfo.aiflags & AI_SOUND_TARGET)
		return;

	VectorCopy (self->s.angles, self->aimangles);
	if (infront (self, self->enemy) > 45)
		return;

	// decide when we should shoot
	if (level.time < self->monsterinfo.attack_finished)
		return;

	self->monsterinfo.attack_finished = level.time + reaction_time;
	//FIXME how do we really want to pass this along?
	self->target_ent->spawnflags |= 65536;
}

void turret_driver_link (edict_t *self)
{
	vec3_t	vec;
	edict_t	*ent;

	// SLUGFILLER--this is a better place to handle the ai
	self->prethink = turret_driver_think;

	self->target_ent = G_PickTarget (self->target);
	self->target_ent->owner = self;
	self->target_ent->teammaster->owner = self;
	VectorCopy (self->target_ent->s.angles, self->s.angles);

	vec[0] = self->target_ent->s.origin[0] - self->s.origin[0];
	vec[1] = self->target_ent->s.origin[1] - self->s.origin[1];
	vec[2] = 0;
	self->move_origin[0] = VectorLength(vec);

	VectorSubtract (self->s.origin, self->target_ent->s.origin, vec);
	vectoangles (vec, vec);
	AnglesNormalize(vec);
	self->move_origin[1] = vec[1];

	self->move_origin[2] = self->s.origin[2] - self->target_ent->s.origin[2];

	// add the driver to the end of them team chain
	for (ent = self->target_ent->teammaster; ent->teamchain; ent = ent->teamchain)
		;
	ent->teamchain = self;
	self->teammaster = self->target_ent->teammaster;
	self->flags |= FL_TEAMSLAVE;
}

void SP_turret_driver (edict_t *self)
{
	if (deathmatch->value && deathmatch->value != 3 && !(((int)(gibflags->value) & GF_WITH_MONSTERS) && G_PickTarget (self->target)))
	{
		self->classname = "turret_seat";
		ED_CallSpawn (self);
		return;
	}

	self->spawnflags |= 1;

	self->movetype = MOVETYPE_PUSH;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/infantry/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 22);
	VectorSet (self->headmins, -16, -16, 12);
	VectorSet (self->headmaxs, 16, 16, 22);

	self->health = 100;
	self->gib_health = 0;
	self->mass = 200;
	self->viewheight = 17;

	self->die = turret_driver_die;
	self->monsterinfo.stand = infantry_stand;

	self->flags |= FL_NO_KNOCKBACK;

	level.total_monsters++;

	self->svflags |= SVF_MONSTER;
	self->s.renderfx |= RF_FRAMELERP | RF_IR_VISIBLE;
	self->takedamage = DAMAGE_AIM;
	self->use = monster_use;
	self->clipmask = MASK_MONSTERSOLID;
	VectorCopy (self->s.origin, self->s.old_origin);
	self->monsterinfo.aiflags |= AI_STAND_GROUND|AI_DUCKED;

	if (st.item)
	{
		self->item = FindItemByClassname (st.item);
		if (!self->item)
			gi.dprintf("%s at %s has bad item: %s\n", self->classname, vtos(self->s.origin), st.item);
	}

	self->think = turret_driver_link;
	self->nextthink = level.time + FRAMETIME;

	gi.linkentity (self);
}

/*SLUGFILLER--QUAKED turret_seat (-16 -16 -24) (16 16 -22)
target this to a turret_breach and it will enable players to use the turret
*/
void turret_seat_think (edict_t *self)
{
	int			i, num;
	edict_t		*touch[MAX_EDICTS], *ent;

	if (!self->owner)
		return;
	
	// SLUGFILLER--is the owner dead?
	if (!self->owner->inuse || self->owner->health <= 0)
	{
		// SLUGFILLER--stop the gun
		VectorCopy (self->target_ent->s.angles, self->target_ent->move_angles);
		for (ent = self->target_ent->teammaster; ent; ent = ent->teamchain)
			VectorClear (ent->avelocity);

		self->owner = NULL;
		return;
	}

	// SLUGFILLER--is the owner still on the seat?
	num = gi.BoxEdicts (self->absmin, self->absmax, touch, MAX_EDICTS, AREA_SOLID);

	for (i=0 ; i<num ; i++)
		if (touch[i] == self->owner)
			break;
	if (i == num)
	{
		// SLUGFILLER--stop the gun
		VectorCopy (self->target_ent->s.angles, self->target_ent->move_angles);
		for (ent = self->target_ent->teammaster; ent; ent = ent->teamchain)
			VectorClear (ent->avelocity);

		self->owner = NULL;
		return;
	}

	// SLUGFILLER--adjust turret angles and shoot if used
	VectorCopy (self->owner->client->v_angle, self->target_ent->move_angles);
	if (level.time < self->monsterinfo.attack_finished)
		return;

	if (self->owner->client->buttons & BUTTON_USE)
	{
		self->target_ent->spawnflags |= 65536;
		self->monsterinfo.attack_finished = level.time + self->target_ent->wait;
	}
}

void turret_seat_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (self->owner)
		return;

	if (!other->client)
		return;

	self->owner = other;
}

void turret_seat_link (edict_t *self)
{
	vec3_t vec;
	edict_t *ent;

	self->target_ent = G_PickTarget (self->target);
	self->target_ent->owner = self;
	self->target_ent->teammaster->owner = self;
	VectorCopy (self->target_ent->s.angles, self->s.angles);

	vec[0] = self->target_ent->s.origin[0] - self->s.origin[0];
	vec[1] = self->target_ent->s.origin[1] - self->s.origin[1];
	vec[2] = 0;
	self->move_origin[0] = VectorLength(vec);

	VectorSubtract (self->s.origin, self->target_ent->s.origin, vec);
	vectoangles (vec, vec);
	AnglesNormalize(vec);
	self->move_origin[1] = vec[1];

	self->move_origin[2] = self->s.origin[2] - self->target_ent->s.origin[2];

	// add the seat as the master of the team chain
	self->teamchain = self->target_ent->teammaster;
	self->teammaster = self;
	for (ent = self->target_ent->teammaster; ent; ent = ent->teamchain)
	{
		ent->teammaster = self;
		ent->flags |= FL_TEAMSLAVE;
	}

	self->touch = turret_seat_touch;
	self->prethink = turret_seat_think;
}

void SP_turret_seat (edict_t *self)
{
	self->movetype = MOVETYPE_PUSH;
	self->solid = SOLID_TRIGGER;
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -22);
	self->svflags = SVF_NOCLIENT;

	self->think = turret_seat_link;
	self->nextthink = level.time + FRAMETIME;

	gi.linkentity (self);
}