#include "g_local.h"


/*
=================
check_dodge

This is a support routine used when a client is firing
a non-instant attack weapon.  It checks to see if a
monster's dodge function should be called.
=================
*/
static void check_dodge (edict_t *self, edict_t *inflictor, vec3_t start, vec3_t dir, int speed)
{
	edict_t	*ent;
	trace_t tr;
	vec3_t	v, v1;
	float	eta;

	for (ent = g_edicts ; ent < &g_edicts[globals.num_edicts] ; ent++)
	{
		if (!ent->inuse)
			continue;

		if (!(ent->svflags & SVF_MONSTER))
			continue;

		if (!(ent->monsterinfo.dodge))
			continue;

		if (ent->deadflag == DEAD_DEAD)
			continue;

		if (!visible (ent, inflictor))
			continue;

		if (!infront(ent, inflictor))
			continue;

		VectorSubtract (ent->s.origin, start, v);
		VectorScale (dir, DotProduct(dir, v), v1);

		VectorSubtract (v, v1, v);
		VectorNormalize (v);
		VectorScale (v, 32, v);
		VectorAdd (v, v1, v);

		VectorAdd (start, v, v);
		VectorAdd (start, v1, v1);

		tr = gi.trace (v1, NULL, NULL, v, inflictor, MASK_SHOT);

		if (tr.ent != ent)
			continue;

		VectorSubtract (v1, start, v);
		eta = (VectorLength(v) - tr.ent->maxs[0]) / speed;

		tr.ent->monsterinfo.dodge (tr.ent, inflictor, self, eta, &tr, (tr.fraction < 0.1));
	}
}


/*
=================
projectile_think

SLUGFILLER--Gives an early warning for monsters to avoid projectiles
=================
*/
static void projectile_think (edict_t *self)
{
	vec3_t dir;

	VectorCopy (self->velocity, dir);
	VectorNormalize (dir);

	check_dodge (self->owner, self, self->s.origin, dir, VectorLength (self->velocity));
}


/*
=================
fire_hit

Used for all impact (hit/punch/slash) attacks
=================
*/
qboolean fire_hit (edict_t *self, vec3_t aim, int damage, int kick)
{
	trace_t		tr;
	vec3_t		forward, right, up;
	vec3_t		v;
	vec3_t		point;
	float		range;
	vec3_t		dir;

	//see if enemy is in range
	if (!self->enemy || !self->enemy->inuse)
		return false;

	VectorSubtract (self->enemy->s.origin, self->s.origin, dir);
	range = VectorLength(dir);
	if (range > aim[0])
		return false;

	if (aim[1] > self->mins[0] && aim[1] < self->maxs[0])
	{
		// the hit is straight on so back the range up to the edge of their bbox
		range -= self->enemy->maxs[0];
	}
	else
	{
		// this is a side hit so adjust the "right" value out to the edge of their bbox
		if (aim[1] < 0)
			aim[1] = self->enemy->mins[0];
		else
			aim[1] = self->enemy->maxs[0];
	}

	VectorMA (self->s.origin, range, dir, point);

	tr = gi.trace (self->s.origin, NULL, NULL, point, self, MASK_SHOT);
	if (tr.fraction < 1)
	{
		if (!tr.ent->takedamage)
			return false;
		// if it will hit any client/monster then hit the one we wanted to hit
		if ((tr.ent->svflags & SVF_MONSTER) || (tr.ent->client))
			tr.ent = self->enemy;
	}

	AngleVectors(self->s.angles, forward, right, up);
	VectorMA (self->s.origin, range, forward, point);
	VectorMA (point, aim[1], right, point);
	VectorMA (point, aim[2], up, point);
	VectorSubtract (point, self->enemy->s.origin, dir);

	// do the damage
	// SLUGFILLER--messed with the kick so the blood won't fly too far
	T_Damage (tr.ent, self, self, dir, point, vec3_origin, damage, 0, kick, DAMAGE_NO_KNOCKBACK, MOD_HIT);

	if (!(tr.ent->svflags & SVF_MONSTER) && (!tr.ent->client))
		return false;

	// do our special form of knockback here
	VectorMA (self->enemy->absmin, 0.5, self->enemy->size, v);
	VectorSubtract (v, point, v);
	VectorNormalize (v);
	VectorMA (self->enemy->velocity, 1000.0 * (float)abs(kick) / self->enemy->mass, v, self->enemy->velocity);
	if (self->enemy->velocity[2] > 0)
		self->enemy->groundentity = NULL;
	return true;
}


/*
=================
fire_lead

This is an internal support routine used for bullet/pellet based weapons.
=================
*/
static void lead_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}
	
	if (other->takedamage)
		T_Damage (other, self, self->owner, self->velocity, self->s.origin, plane->normal, self->dmg, (int)bullets_gib->value, self->kick, DAMAGE_BULLET, self->spawnflags);
	else
	{
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (self->radius_dmg);
		gi.WritePosition (self->s.origin);
		if (!plane)
			gi.WriteDir (vec3_origin);
		else
			gi.WriteDir (plane->normal);
		gi.multicast (self->s.origin, MULTICAST_PVS);

		if (self->owner->client)
			PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);
	}

	self->nextthink = level.time + FRAMETIME;
	self->think = G_FreeEdict;
}

// SLUGFILLER--instead of tracing it fires projectiles
static void fire_lead (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int te_impact, int hspread, int vspread, int mod)
{
	vec3_t		dir;
	vec3_t		forward, right, up;
	vec3_t		end;
	float		r;
	float		u;
	edict_t		*lead;
	trace_t tr;

	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);

	r = crandom()*hspread;
	u = crandom()*vspread;
	VectorMA (start, 8192, forward, end);
	VectorMA (end, r, right, end);
	VectorMA (end, u, up, end);

	lead = G_Spawn();
	// SLUGFILLER--so you can walk through the shots, so you can't be walled by them
	// although it would be stupid to try
	lead->svflags = SVF_DEADMONSTER;
	VectorCopy (start, lead->s.origin);
	VectorSubtract (end, start, dir);
	VectorNormalize (dir);
	VectorScale (dir, 2048, lead->velocity);
	vectoangles(dir, lead->s.angles);
	lead->s.modelindex = gi.modelindex ("models/objects/bullet/tris.md2");
	lead->movetype = MOVETYPE_FLYMISSILE;
	lead->clipmask = MASK_SHOT;
	lead->solid = SOLID_BBOX;
	if ((int)gibflags->value & GF_BEAM_BULLETS)
	{
		lead->s.renderfx |= RF_BEAM;
		lead->s.skinnum = 0xf0f0f0f0;
		lead->s.frame = 2;
	}
	lead->owner = self;
	VectorClear (lead->mins);
	VectorClear (lead->maxs);
	lead->touch = lead_touch;
	lead->dmg = damage;
	lead->kick = kick;
	lead->spawnflags = mod;
	lead->radius_dmg = te_impact;
	// SLUGFILLER--warning
	lead->prethink = projectile_think;
	VectorCopy (end, lead->move_origin);
	gi.linkentity (lead);

	if (!(self->svflags & SVF_NOCLIENT)) {
		tr = gi.trace (self->s.origin, NULL, NULL, lead->s.origin, lead, MASK_SHOT);
		if (tr.fraction < 1.0)
		{
			VectorMA (lead->s.origin, -10, dir, lead->s.origin);
			lead->touch (lead, tr.ent, NULL, NULL);
		}
	}
}


/*
=================
fire_bullet

Fires a single round.  Used for machinegun and chaingun.  Would be fine for
pistols, rifles, etc....
=================
*/
void fire_bullet (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod)
{
	fire_lead (self, start, aimdir, damage, kick, TE_GUNSHOT, hspread, vspread, mod);
}


/*
=================
fire_shotgun

Shoots shotgun pellets.  Used by shotgun and super shotgun.
=================
*/
void fire_shotgun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int mod)
{
	int		i;

	for (i = 0; i < count; i++)
		fire_lead (self, start, aimdir, damage, kick, TE_SHOTGUN, hspread, vspread, mod);
}


/*
=================
fire_blaster

Fires a single blaster bolt.  Used by the blaster and hyper blaster.
=================
*/
void blaster_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	int		mod;

	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		if (self->spawnflags & 1)
			mod = MOD_HYPERBLASTER;
		else
			mod = MOD_BLASTER;
		T_Damage (other, self, self->owner, self->velocity, self->s.origin, plane->normal, self->dmg, 0, self->dmg*2, DAMAGE_ENERGY | DAMAGE_NO_BLOOD, mod);
	}

	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_BLASTER);
	gi.WritePosition (self->s.origin);
	if (!plane)
		gi.WriteDir (vec3_origin);
	else
		gi.WriteDir (plane->normal);
	gi.multicast (self->s.origin, MULTICAST_PVS);

	self->nextthink = level.time + FRAMETIME;
	self->think = G_FreeEdict;
}

void fire_blaster (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect, qboolean hyper)
{
	edict_t	*bolt;
	trace_t tr;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->svflags = SVF_DEADMONSTER;
	// yes, I know it looks weird that projectiles are deadmonsters
	// what this means is that when prediction is used against the object
	// (blaster/hyperblaster shots), the player won't be solid clipped against
	// the object.  Right now trying to run into a firing hyperblaster
	// is very jerky since you are predicted 'against' the shots.
	VectorCopy (start, bolt->s.origin);
	VectorCopy (start, bolt->s.old_origin);
	vectoangles (dir, bolt->s.angles);
	VectorScale (dir, speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	if ((int)gibflags->value & GF_BEAM_BLASTER)
	{
		bolt->s.effects |= EF_HYPERBLASTER;
		bolt->s.renderfx |= RF_BEAM;
		bolt->s.skinnum = 0x34343434;
		bolt->s.frame = 2;
	}
	else
		bolt->s.effects |= effect;
	VectorClear (bolt->mins);
	VectorClear (bolt->maxs);
	bolt->s.modelindex = gi.modelindex ("models/objects/laser/tris.md2");
	bolt->s.sound = gi.soundindex ("misc/lasfly.wav");
	bolt->owner = self;
	bolt->touch = blaster_touch;
	// SLUGFILLER--removed limited range
	bolt->think = G_FreeEdict;
	bolt->nextthink = level.time+1;
	bolt->dmg = damage;
	bolt->classname = "bolt";
	// SLUGFILLER--warning
	bolt->prethink = projectile_think;
	if (hyper)
		bolt->spawnflags = 1;
	gi.linkentity (bolt);

	if (!(self->svflags & SVF_NOCLIENT)) {
		tr = gi.trace (self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
		if (tr.fraction < 1.0)
		{
			VectorMA (bolt->s.origin, -10, dir, bolt->s.origin);
			bolt->touch (bolt, tr.ent, NULL, NULL);
		}
	}
}	


/*
=================
fire_grenade
=================
*/
static void Grenade_Explode (edict_t *ent)
{
	vec3_t		origin;
	int			mod;

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	//FIXME: if we are onground then raise our Z just a bit since we are a point?
	if (ent->enemy)
	{
		float	points;
		vec3_t	v;
		vec3_t	dir;

		VectorAdd (ent->enemy->mins, ent->enemy->maxs, v);
		VectorMA (ent->enemy->s.origin, 0.5, v, v);
		VectorSubtract (ent->s.origin, v, v);
		points = ent->dmg - 0.5 * VectorLength (v);
		VectorSubtract (ent->enemy->s.origin, ent->s.origin, dir);
		T_Damage (ent->enemy, ent, ent->owner, dir, ent->s.origin, vec3_origin, (int)points, (int)points, (int)points, DAMAGE_RADIUS, MOD_GRENADE);
	}

	if (ent->spawnflags & 1)
		mod = MOD_HG_SPLASH;
	else
		mod = MOD_G_SPLASH;
	T_RadiusDamage(ent, ent->owner, ent->dmg, ent->enemy, ent->dmg_radius, mod);

	VectorMA (ent->s.origin, -0.02, ent->velocity, origin);
	gi.WriteByte (svc_temp_entity);
	if (ent->waterlevel)
	{
		if (ent->groundentity)
			gi.WriteByte (TE_GRENADE_EXPLOSION_WATER);
		else
			gi.WriteByte (TE_ROCKET_EXPLOSION_WATER);
	}
	else
	{
		if (ent->groundentity)
			gi.WriteByte (TE_GRENADE_EXPLOSION);
		else
			gi.WriteByte (TE_ROCKET_EXPLOSION);
	}
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PHS);

	G_FreeEdict (ent);
}

static void Grenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}

	if (!other->takedamage)
	{
		gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
		return;
	}

	ent->enemy = other;
	Grenade_Explode (ent);
}

void Grenade_Die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	self->enemy = NULL;
	self->takedamage = DAMAGE_NO;
	self->nextthink = level.time + FRAMETIME;
	self->think = Grenade_Explode;
}

void fire_grenade (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius)
{
	edict_t	*grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy (start, grenade->s.origin);
	VectorScale (aimdir, speed, grenade->velocity);
	VectorMA (grenade->velocity, 200 + crandom() * 10.0, up, grenade->velocity);
	VectorMA (grenade->velocity, crandom() * 10.0, right, grenade->velocity);
	VectorSet (grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	grenade->s.modelindex = gi.modelindex ("models/objects/grenade/tris.md2");
	grenade->owner = self;
	grenade->touch = Grenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = Grenade_Explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "grenade";
	// SLUGFILLER--so you can walk through the grenade, just in case it doesn't kill you
	grenade->svflags = SVF_DEADMONSTER;

	// SLUGFILLER--killable grenades
	// looks alot like the exploding barrel code
	VectorSet (grenade->mins, -3, -3, -3);
	VectorSet (grenade->maxs, 3, 3, 3);
	grenade->mass = 2;
	grenade->health = 10;
	grenade->die = Grenade_Die;
	grenade->takedamage = DAMAGE_YES;
	// removed AI_NOSTEP:
	// AI_NOSTEP used so barrel dont get pushed upstairs(remove?)

	gi.linkentity (grenade);
}

static void Grenade2_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}

	if (other->solid == SOLID_BSP)
	{
		if (random() > 0.5)
			gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/hgrenb1a.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/hgrenb2a.wav"), 1, ATTN_NORM, 0);
	}
	else
		ent->velocity[0] = ent->velocity[1] = 0;
}

void fire_grenade2 (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float damage_radius)
{
	edict_t	*grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy (start, grenade->s.origin);
	VectorScale (aimdir, speed, grenade->velocity);
	VectorMA (grenade->velocity, 200 + crandom() * 10.0, up, grenade->velocity);
	VectorMA (grenade->velocity, crandom() * 10.0, right, grenade->velocity);
	VectorSet (grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	grenade->s.modelindex = gi.modelindex ("models/objects/grenade2/tris.md2");
	grenade->owner = self;
	grenade->touch = Grenade2_Touch; // SLUGFILLER--real hand grenades dont have impact triggers
	grenade->nextthink = level.time + 1.0;
	grenade->think = Grenade_Explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "hgrenade";
	grenade->s.sound = gi.soundindex("weapons/hgrenc1b.wav");
	grenade->spawnflags |= 1;
	// SLUGFILLER--so you can walk through the grenade, just in case it doesn't kill you
	grenade->svflags = SVF_DEADMONSTER;

	// SLUGFILLER--killable grenades
	// looks alot like the exploding barrel code
	VectorSet (grenade->mins, -3, -3, -3);
	VectorSet (grenade->maxs, 3, 3, 3);
	grenade->mass = 2;
	grenade->health = 10;
	grenade->die = Grenade_Die;
	grenade->takedamage = DAMAGE_YES;
	// removed AI_NOSTEP:
	// AI_NOSTEP used so barrel dont get pushed upstairs(remove?)

	gi.sound (self, CHAN_WEAPON, gi.soundindex ("weapons/hgrent1a.wav"), 1, ATTN_NORM, 0);
	gi.linkentity (grenade);
}

/*
=================
fire_rocket
=================
*/
void rocket_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	vec3_t		origin;
	int			n;

	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA (ent->s.origin, -0.02, ent->velocity, origin);

	if (other->takedamage)
	{
		T_Damage (other, ent, ent->owner, ent->velocity, ent->s.origin, plane->normal, ent->dmg, ent->dmg, 0, 0, MOD_ROCKET);
	}
	else
	{
		// don't throw any debris in net games
		if (!deathmatch->value && !coop->value)
		{
			if ((surf) && !(surf->flags & (SURF_WARP|SURF_TRANS33|SURF_TRANS66|SURF_FLOWING)))
			{
				n = rand() % 5;
				while(n--)
					ThrowDebris (ent, "models/objects/debris2/tris.md2", 2, ent->s.origin);
			}
		}
	}

	T_RadiusDamage(ent, ent->owner, ent->radius_dmg, other, ent->dmg_radius, MOD_R_SPLASH);

	gi.WriteByte (svc_temp_entity);
	if (ent->waterlevel)
		gi.WriteByte (TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte (TE_ROCKET_EXPLOSION);
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PHS);

	G_FreeEdict (ent);
}

void Rocket_Explode (edict_t *ent)
{
	vec3_t		origin;
	

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA (ent->s.origin, -0.02, ent->velocity, origin);

	T_RadiusDamage(ent, ent->owner, ent->radius_dmg, NULL, ent->dmg_radius, MOD_R_SPLASH);

	gi.WriteByte (svc_temp_entity);
	if (ent->waterlevel)
		gi.WriteByte (TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte (TE_ROCKET_EXPLOSION);
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PVS);

	G_FreeEdict (ent);
}

void Rocket_Die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	self->takedamage = DAMAGE_NO;
	self->nextthink = level.time + FRAMETIME;
	self->think = Rocket_Explode;
}

void fire_rocket (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage)
{
	edict_t	*rocket;

	rocket = G_Spawn();
	VectorCopy (start, rocket->s.origin);
	VectorCopy (dir, rocket->movedir);
	vectoangles (dir, rocket->s.angles);
	VectorScale (dir, speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear (rocket->mins);
	VectorClear (rocket->maxs);
	rocket->s.modelindex = gi.modelindex ("models/objects/rocket/tris.md2");
	rocket->owner = self;
	rocket->touch = rocket_touch;
	// SLUGFILLER--removed limited range
	rocket->dmg = damage;
	rocket->radius_dmg = radius_damage;
	rocket->dmg_radius = damage_radius;
	rocket->s.sound = gi.soundindex ("weapons/rockfly.wav");
	rocket->classname = "rocket";
	// SLUGFILLER--warning
	rocket->prethink = projectile_think;

	// SLUGFILLER--killable rockets
	// looks alot like the exploding barrel code
	VectorSet (rocket->mins, -3, -3, -3);
	VectorSet (rocket->maxs, 3, 3, 3);
	rocket->mass = 10;
	rocket->health = 10;
	rocket->die = Rocket_Die;
	rocket->takedamage = DAMAGE_YES;
	// removed AI_NOSTEP:
	// AI_NOSTEP used so barrel dont get pushed upstairs(remove?)

	gi.linkentity (rocket);
}


/*
=================
fire_rail
=================
*/
static void parse_rail (edict_t *self, vec3_t start, vec3_t end, vec3_t dir, int damage, int kick)
{
	trace_t	tr;
	edict_t	*pass_ent;

	VectorCopy (start, tr.endpos);
	pass_ent = self;
	while (pass_ent != g_edicts)
	{
		tr = gi.trace (tr.endpos, NULL, NULL, end, pass_ent, MASK_SHOT);

		if ((tr.ent != self) && (tr.ent->takedamage))
			T_Damage (tr.ent, self, self, dir, tr.endpos, tr.plane.normal, damage, 0, kick, 0, MOD_RAILGUN);

		// SLUGFILLER--added so rail goes through entities
		pass_ent = tr.ent;
	}
}

static void draw_rail (edict_t *self, vec3_t start, vec3_t end)
{
	trace_t	tr;
	trace_t	trb;
	edict_t	*pass_ent;
	vec3_t	wall_start;

//	return;
	VectorCopy (start, tr.endpos);
	VectorCopy (start, wall_start);
	pass_ent = NULL;
	while (pass_ent != g_edicts)
	{
		tr = gi.trace (tr.endpos, NULL, NULL, end, pass_ent, MASK_OPAQUE);
		trb = gi.trace (tr.endpos, NULL, NULL, wall_start, tr.ent, MASK_OPAQUE);

		// SLUGFILLER--draw rail between walls
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_RAILTRAIL);
		gi.WritePosition (trb.endpos);
		gi.WritePosition (tr.endpos);
		gi.multicast (trb.endpos, MULTICAST_PHS);
		gi.multicast (tr.endpos, MULTICAST_PHS);

		// SLUGFILLER--in case the wall is temporary(exploding box)
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_RAILTRAIL);
		gi.WritePosition (trb.endpos);
		gi.WritePosition (wall_start);
		gi.multicast (trb.endpos, MULTICAST_PHS);
		gi.multicast (wall_start, MULTICAST_PHS);

/*		if (self->client)
		{
			PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
			PlayerNoise(self, trb.endpos, PNOISE_IMPACT);
			PlayerNoise(self, wall_start, PNOISE_IMPACT);
		}*/

		VectorCopy (tr.endpos, wall_start);
		
		// SLUGFILLER--added so rail goes through entities
		pass_ent = tr.ent;
	}
}

void fire_rail (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int range)
{
	vec3_t		from;
	vec3_t		end;
	vec3_t		dist;
	vec3_t		wall_start;
	trace_t		tr, trb;

	VectorMA (start, range, aimdir, end);
	VectorCopy (start, from);
	// SLUGFILLER--the rail starts from the weapon
	VectorCopy (from, wall_start);
	VectorSubtract (from, end, dist);
	while (range>0)
	{
		// SLUGFILLER--only walls no ents
		VectorCopy (from, tr.endpos);
		tr.ent = NULL;
		while (tr.ent != g_edicts)
		{
			tr = gi.trace (tr.endpos, NULL, NULL, end, tr.ent, MASK_SOLID|MASK_WATER);
		}

		// SLUGFILLER--send rail between walls and water
		if (!tr.allsolid)
		{
			// SLUGFILLER--find where the wall/water ends
			// 1. if the rail goes from the weapon
			// then it will reach wall_start which is the weapon's location
			// 2. if it goes through a wall or water
			// then we'll find the end of the wall/water
			// SLUGFILLER--only walls no ents
			VectorCopy (tr.endpos, trb.endpos);
			trb.ent = NULL;
			while (trb.ent != g_edicts)
			{
				trb = gi.trace (trb.endpos, NULL, NULL, wall_start, trb.ent, MASK_SOLID|MASK_WATER);
			}
			draw_rail (self, tr.endpos, trb.endpos);

			// SLUGFILLER--we hit water
			// draw rail through water
			if (trb.contents & MASK_WATER)
			{
				VectorCopy (trb.endpos, start);
				// SLUGFILLER--we know where the water ends, now to find where it starts
				// 1. if it starts where a solid ends
				// then it will hit that solid and draw from there
				// 2. if it starts from water
				// then it will reach wall_start which says where the water starts
				// SLUGFILLER--only walls no ents
				trb.ent = NULL;
				while (trb.ent != g_edicts)
				{
					trb = gi.trace (trb.endpos, NULL, NULL, wall_start, trb.ent, MASK_SOLID);
				}
				draw_rail (self, start, trb.endpos);
			}

			// SLUGFILLER--set so we'll know where the wall starts next trace
			VectorCopy (tr.endpos, wall_start);

			// SLUGFILLER--kill!
			parse_rail (self, trb.endpos, tr.endpos, aimdir, damage, kick);
		}

		// SLUGFILLER--wall slows down slugs
		if (tr.fraction < 1)
		{
			if (!(tr.contents & MASK_WATER))
			{
				// SLUGFILLER--check wall width by first tracing forward to the next wall
				// SLUGFILLER--only walls no ents
				VectorAdd (tr.endpos, aimdir, trb.endpos);
				trb.ent = NULL;
				while (trb.ent != g_edicts)
				{
					trb = gi.trace (trb.endpos, NULL, NULL, end, trb.ent, MASK_SOLID);
				}
				// SLUGFILLER--then tracing backwards to the end of this wall
				// SLUGFILLER--only walls no ents
				trb.ent = NULL;
				while (trb.ent != g_edicts)
				{
					trb = gi.trace (trb.endpos, NULL, NULL, tr.endpos, trb.ent, MASK_SOLID);
				}
				// reduce 5 times the width of this wall from the total range of the rail
				VectorSubtract (trb.endpos, tr.endpos, dist);
				VectorMA (end, -5, dist, end);
				range -= 5*VectorLength(dist);
			}
		}

		// SLUGFILLER--reduce distance travelled from the remaining distance
		VectorSubtract (tr.endpos, from, dist);
		range -= VectorLength(dist)+1;

		// SLUGFILLER--added so rail goes through walls
		VectorAdd (tr.endpos, aimdir, from);
	}
}


/*
=================
fire_bfg
=================
*/
void bfg_explode (edict_t *self)
{
	edict_t	*ent;
	float	points;
	vec3_t	v;
	float	dist;

	if (self->s.frame == 0)
	{
		// the BFG effect
		ent = NULL;
		while ((ent = findradius(ent, self->s.origin, self->dmg_radius)) != NULL)
		{
			if (!ent->takedamage)
				continue;
			if (ent == self->owner)
				continue;
			if (!CanDamage (ent, self))
				continue;
			if (!CanDamage (ent, self->owner))
				continue;

			VectorAdd (ent->mins, ent->maxs, v);
			VectorMA (ent->s.origin, 0.5, v, v);
			VectorSubtract (self->s.origin, v, v);
			dist = VectorLength(v);
			points = self->radius_dmg * (1.0 - sqrt(dist/self->dmg_radius));

			gi.WriteByte (svc_temp_entity);
			gi.WriteByte (TE_BFG_EXPLOSION);
			gi.WritePosition (ent->s.origin);
			gi.multicast (ent->s.origin, MULTICAST_PHS);
			T_Damage (ent, self, self->owner, self->velocity, ent->s.origin, vec3_origin, (int)points, (int)points, 0, DAMAGE_ENERGY | DAMAGE_NO_BLOOD, MOD_BFG_EFFECT);
		}
	}

	self->nextthink = level.time + FRAMETIME;
	self->s.frame++;
	if (self->s.frame == 5)
		self->think = G_FreeEdict;
}

void bfg_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	// core explosion - prevents firing it into the wall/floor
	if (other->takedamage)
		T_Damage (other, self, self->owner, self->velocity, self->s.origin, plane->normal, 200, 200, 0, DAMAGE_NO_BLOOD, MOD_BFG_BLAST);
	T_RadiusDamage(self, self->owner, 200, other, 100, MOD_BFG_BLAST);

	gi.sound (self, CHAN_VOICE, gi.soundindex ("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
	self->solid = SOLID_NOT;
	self->touch = NULL;
	VectorMA (self->s.origin, -1 * FRAMETIME, self->velocity, self->s.origin);
	VectorClear (self->velocity);
	self->s.modelindex = gi.modelindex ("sprites/s_bfg3.sp2");
	self->s.frame = 0;
	self->s.sound = 0;
	self->s.effects &= ~EF_ANIM_ALLFAST;
	self->think = bfg_explode;
	self->nextthink = level.time + FRAMETIME;
	self->enemy = other;

	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_BFG_BIGEXPLOSION);
	gi.WritePosition (self->s.origin);
	gi.multicast (self->s.origin, MULTICAST_PVS);
}


void bfg_think (edict_t *self)
{
	edict_t	*ent;
	edict_t	*ignore;
	vec3_t	point;
	vec3_t	dir;
	vec3_t	start;
	vec3_t	end;
	int		dmg;
	trace_t	tr;

	dmg = 10;

	ent = NULL;
	while ((ent = findradius(ent, self->s.origin, 256)) != NULL)
	{
		if (ent == self)
			continue;

		// SLUGFILLER--friendly fire
		if (ent == self->owner || OnSameTeam (ent, self->owner))
			continue;

		if (ent->takedamage != DAMAGE_AIM)
			continue;

		VectorMA (ent->absmin, 0.5, ent->size, point);

		VectorSubtract (point, self->s.origin, dir);
		VectorNormalize (dir);

		ignore = self;
		VectorCopy (self->s.origin, start);
		VectorMA (start, 2048, dir, end);
		while(1)
		{
			tr = gi.trace (start, NULL, NULL, end, ignore, CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_DEADMONSTER);

			if (!tr.ent)
				break;

			// hurt it if we can
			if ((tr.ent->takedamage) && !(tr.ent->flags & FL_IMMUNE_LASER) && (tr.ent != self->owner))
				T_Damage (tr.ent, self, self->owner, dir, tr.endpos, vec3_origin, dmg, 0, 1, DAMAGE_ENERGY | DAMAGE_NO_BLOOD, MOD_BFG_LASER);

			// if we hit something that's not a monster or player we're done
			if (!(tr.ent->svflags & SVF_MONSTER) && (!tr.ent->client))
			{
				gi.WriteByte (svc_temp_entity);
				gi.WriteByte (TE_LASER_SPARKS);
				gi.WriteByte (4);
				gi.WritePosition (tr.endpos);
				gi.WriteDir (tr.plane.normal);
				gi.WriteByte (self->s.skinnum);
				gi.multicast (tr.endpos, MULTICAST_PVS);
				break;
			}

			ignore = tr.ent;
			VectorCopy (tr.endpos, start);
		}

		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BFG_LASER);
		gi.WritePosition (self->s.origin);
		gi.WritePosition (tr.endpos);
		gi.multicast (self->s.origin, MULTICAST_PHS);
	}

	self->nextthink = level.time + FRAMETIME;
}


void fire_bfg (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius)
{
	edict_t	*bfg;

	bfg = G_Spawn();
	VectorCopy (start, bfg->s.origin);
	VectorCopy (dir, bfg->movedir);
	vectoangles (dir, bfg->s.angles);
	VectorScale (dir, speed, bfg->velocity);
	bfg->movetype = MOVETYPE_FLYMISSILE;
	bfg->clipmask = MASK_SHOT;
	bfg->solid = SOLID_BBOX;
	bfg->s.effects |= EF_BFG | EF_ANIM_ALLFAST;
	// SLUGFILLER--the bfg is too massive to be dot sized
	VectorSet (bfg->mins, -10, -10, -10);
	VectorSet (bfg->maxs, 10, 10, 10);
	bfg->s.modelindex = gi.modelindex ("sprites/s_bfg1.sp2");
	bfg->owner = self;
	bfg->touch = bfg_touch;
	bfg->radius_dmg = damage;
	bfg->dmg_radius = damage_radius;
	bfg->classname = "bfg blast";
	bfg->s.sound = gi.soundindex ("weapons/bfg__l1a.wav");
	// SLUGFILLER--warning
	bfg->prethink = projectile_think;

	bfg->think = bfg_think;
	bfg->nextthink = level.time + FRAMETIME;
	bfg->teammaster = bfg;
	bfg->teamchain = NULL;

	gi.linkentity (bfg);
}
