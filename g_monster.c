#include "g_local.h"


//
// monster weapons
//

//FIXME mosnters should call these with a totally accurate direction
// and we can mess it up based on skill.  Spread should be for normal
// and we can tighten or loosen based on skill.  We could muck with
// the damages too, but I'm not sure that's such a good idea.
void monster_fire_bullet(edict_t* self, vec3_t start, vec3_t dir, int damage, int kick, int hspread, int vspread, int flashtype, qboolean chain)
{
	if (chain)
		fire_bullet(self, start, dir, damage, kick, hspread, vspread, MOD_CHAINGUN);
	else
		fire_bullet(self, start, dir, damage, kick, hspread, vspread, MOD_MACHINEGUN);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}

void monster_fire_shotgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int flashtype)
{
	fire_shotgun(self, start, aimdir, damage, kick, hspread, vspread, count, MOD_SHOTGUN);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}

void monster_fire_blaster(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype, int effect, qboolean hyper)
{
	fire_blaster(self, start, dir, damage, speed, effect, hyper);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}

void monster_fire_grenade(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, int flashtype)
{
	fire_grenade(self, start, aimdir, damage, speed, 2.5, damage + 40);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}

void monster_fire_rocket(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype)
{
	fire_rocket(self, start, dir, damage, speed, damage + 20, damage + 20);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}

void monster_fire_railgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int flashtype)
{
	fire_rail(self, start, aimdir, damage, kick, 300);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}

void monster_fire_bfg(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, int kick, float damage_radius, int flashtype)
{
	fire_bfg(self, start, aimdir, damage, speed, damage_radius);

	gi.WriteByte(svc_muzzleflash2);
	gi.WriteShort(self - g_edicts);
	gi.WriteByte(flashtype);
	gi.multicast(start, MULTICAST_PVS);
}



//
// Monster utility functions
//

static void M_FliesOff(edict_t* self)
{
	self->s.effects &= ~EF_FLIES;
	self->s.sound = 0;
}

static void M_FliesOn(edict_t* self)
{
	if (self->waterlevel)
		return;
	self->s.effects |= EF_FLIES;
	self->s.sound = gi.soundindex("infantry/inflies1.wav");
	self->think = M_FliesOff;
	self->nextthink = level.time + 60;
}

void M_FlyCheck(edict_t* self)
{
	if (self->waterlevel)
		return;

	if (random() > 0.5)
		return;

	self->think = M_FliesOn;
	self->nextthink = level.time + 5 + 10 * random();
}


void M_CheckGround(edict_t* ent)
{
	vec3_t		point;
	trace_t		trace;

	if (ent->flags & (FL_SWIM | FL_FLY))
		return;

	if (ent->velocity[2] > 100)
	{
		ent->groundentity = NULL;
		return;
	}

	// if the hull point one-quarter unit down is solid the entity is on ground
	point[0] = ent->s.origin[0];
	point[1] = ent->s.origin[1];
	point[2] = ent->s.origin[2] - 0.25;

	trace = gi.trace(ent->s.origin, ent->mins, ent->maxs, point, ent, MASK_MONSTERSOLID);

	// check steepness
	if (trace.plane.normal[2] < 0.7 && !trace.startsolid)
	{
		ent->groundentity = NULL;
		return;
	}

	//	ent->groundentity = trace.ent;
	//	ent->groundentity_linkcount = trace.ent->linkcount;
	//	if (!trace.startsolid && !trace.allsolid)
	//		VectorCopy (trace.endpos, ent->s.origin);
	if (!trace.startsolid && !trace.allsolid)
	{
		VectorCopy(trace.endpos, ent->s.origin);
		ent->groundentity = trace.ent;
		ent->groundentity_linkcount = trace.ent->linkcount;
		ent->velocity[2] = 0;
	}
}


void M_CatagorizePosition(edict_t* ent)
{
	vec3_t		point;
	int			cont;

	//
	// get waterlevel
	//
	point[0] = ent->s.origin[0];
	point[1] = ent->s.origin[1];
	point[2] = ent->s.origin[2] + ent->mins[2] + 1;
	cont = gi.pointcontents(point);

	if (!(cont & MASK_WATER))
	{
		ent->waterlevel = 0;
		ent->watertype = 0;
		return;
	}

	ent->watertype = cont;
	ent->waterlevel = 1;
	point[2] += 26;
	cont = gi.pointcontents(point);
	if (!(cont & MASK_WATER))
		return;

	ent->waterlevel = 2;
	point[2] += 22;
	cont = gi.pointcontents(point);
	if (cont & MASK_WATER)
		ent->waterlevel = 3;
}


void M_WorldEffects(edict_t* ent)
{
	int		dmg;

	if (ent->health > 0)
	{
		if (!(ent->flags & FL_SWIM))
		{
			if (ent->waterlevel < 3)
			{
				ent->air_finished = level.time + 12;
			}
			else if (ent->air_finished < level.time)
			{	// drown!
				if (ent->pain_debounce_time < level.time)
				{
					dmg = 2 + 2 * floorf(level.time - ent->air_finished);
					if (dmg > 15)
						dmg = 15;
					T_Damage(ent, world, world, vec3_origin, ent->s.origin, vec3_origin, dmg, 0, 0, DAMAGE_NO_ARMOR | DAMAGE_NO_BLOOD, MOD_WATER);
					ent->pain_debounce_time = level.time + 1;
				}
			}
		}
		else
		{
			if (ent->waterlevel > 0)
			{
				ent->air_finished = level.time + 9;
			}
			else if (ent->air_finished < level.time)
			{	// suffocate!
				if (ent->pain_debounce_time < level.time)
				{
					dmg = 2 + 2 * floorf(level.time - ent->air_finished);
					if (dmg > 15)
						dmg = 15;
					T_Damage(ent, world, world, vec3_origin, ent->s.origin, vec3_origin, dmg, 0, 0, DAMAGE_NO_ARMOR | DAMAGE_NO_BLOOD, MOD_WATER);
					ent->pain_debounce_time = level.time + 1;
				}
			}
		}
	}

	if (ent->waterlevel == 0)
	{
		if (ent->flags & FL_INWATER)
		{
			gi.sound(ent, CHAN_BODY, gi.soundindex("player/watr_out.wav"), 1, ATTN_NORM, 0);
			ent->flags &= ~FL_INWATER;
		}
		return;
	}

	if ((ent->watertype & CONTENTS_LAVA) && !(ent->flags & FL_IMMUNE_LAVA))
	{
		if (ent->damage_debounce_time < level.time)
		{
			ent->damage_debounce_time = level.time + 0.2;
			T_Damage(ent, world, world, vec3_origin, ent->s.origin, vec3_origin, 10 * ent->waterlevel, 10 * ent->waterlevel, 0, DAMAGE_NO_BLOOD, MOD_LAVA);
		}
	}
	if ((ent->watertype & CONTENTS_SLIME) && !(ent->flags & FL_IMMUNE_SLIME))
	{
		if (ent->damage_debounce_time < level.time)
		{
			ent->damage_debounce_time = level.time + 1;
			T_Damage(ent, world, world, vec3_origin, ent->s.origin, vec3_origin, 4 * ent->waterlevel, 4 * ent->waterlevel, 0, DAMAGE_NO_BLOOD, MOD_SLIME);
		}
	}

	if (!(ent->flags & FL_INWATER))
	{
		if (!(ent->svflags & SVF_DEADMONSTER))
		{
			if (ent->watertype & CONTENTS_LAVA)
				if (random() <= 0.5)
					gi.sound(ent, CHAN_BODY, gi.soundindex("player/lava1.wav"), 1, ATTN_NORM, 0);
				else
					gi.sound(ent, CHAN_BODY, gi.soundindex("player/lava2.wav"), 1, ATTN_NORM, 0);
			else if (ent->watertype & CONTENTS_SLIME)
				gi.sound(ent, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
			else if (ent->watertype & CONTENTS_WATER)
				gi.sound(ent, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		}

		ent->flags |= FL_INWATER;
		ent->damage_debounce_time = 0;
	}
}


void M_droptofloor(edict_t* ent)
{
	vec3_t		end;
	trace_t		trace;

	ent->s.origin[2] += 1;
	VectorCopy(ent->s.origin, end);
	end[2] -= 256;

	trace = gi.trace(ent->s.origin, ent->mins, ent->maxs, end, ent, MASK_MONSTERSOLID);

	if (trace.fraction == 1 || trace.allsolid)
		return;

	VectorCopy(trace.endpos, ent->s.origin);

	gi.linkentity(ent);
	M_CheckGround(ent);
	M_CatagorizePosition(ent);
}


void M_SetEffects(edict_t* ent)
{
	ent->s.effects &= ~(EF_COLOR_SHELL | EF_POWERSCREEN);
	ent->s.renderfx &= ~(RF_SHELL_RED | RF_SHELL_GREEN | RF_SHELL_BLUE);

	if (ent->monsterinfo.aiflags & AI_RESURRECTING)
	{
		ent->s.effects |= EF_COLOR_SHELL;
		ent->s.renderfx |= RF_SHELL_RED;
	}

	if (ent->health <= 0)
		return;

	if (ent->powerarmor_time > level.time)
	{
		if (ent->monsterinfo.power_armor_type == POWER_ARMOR_SCREEN)
		{
			ent->s.effects |= EF_POWERSCREEN;
		}
		else if (ent->monsterinfo.power_armor_type == POWER_ARMOR_SHIELD)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_GREEN;
		}
	}
}


void M_MoveFrame(edict_t* self)
{
	mmove_t* move;
	int		index;

	move = self->monsterinfo.currentmove;
	self->nextthink = level.time + FRAMETIME;

	if ((self->monsterinfo.nextframe) && (self->monsterinfo.nextframe >= move->firstframe) && (self->monsterinfo.nextframe <= move->lastframe))
	{
		self->s.frame = self->monsterinfo.nextframe;
		self->monsterinfo.nextframe = 0;
	}
	else
	{
		if (self->s.frame == move->lastframe)
		{
			if (move->endfunc)
			{
				move->endfunc(self);

				// regrab move, endfunc is very likely to change it
				move = self->monsterinfo.currentmove;

				// check for death
				if (self->svflags & SVF_DEADMONSTER)
					return;
			}
		}

		if (self->s.frame < move->firstframe || self->s.frame > move->lastframe)
		{
			self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
			self->s.frame = move->firstframe;
		}
		else
		{
			if (!(self->monsterinfo.aiflags & AI_HOLD_FRAME))
			{
				self->s.frame++;
				if (self->s.frame > move->lastframe)
					self->s.frame = move->firstframe;
			}
		}
	}

	index = self->s.frame - move->firstframe;
	if (!(self->monsterinfo.aiflags & AI_HOLD_FRAME))
		M_walkmove(self, self->s.angles[YAW], move->frame[index].dist);

	if (move->frame[index].thinkfunc)
		move->frame[index].thinkfunc(self);
}


void monster_think(edict_t* self)
{
	M_MoveFrame(self);
	M_RunThink(self);
	if (self->linkcount != self->monsterinfo.linkcount)
	{
		self->monsterinfo.linkcount = self->linkcount;
		M_CheckGround(self);
	}
	M_CatagorizePosition(self);
	M_WorldEffects(self);
	M_SetEffects(self);
}


/*
================
monster_use

Using a monster makes it angry at the current activator
================
*/
void monster_use(edict_t* self, edict_t* other, edict_t* activator)
{
	if (self->enemy)
		return;
	if (self->health <= 0)
		return;
	if (activator->flags & FL_NOTARGET)
		return;
	if (!(activator->client) && !(activator->monsterinfo.aiflags & AI_GOOD_GUY))
		return;

	// delay reaction so if the monster is teleported, its sound is still heard
	self->enemy = activator;
	// SLUGFILLER--it's his own fault
	self->chain = NULL;
	VectorCopy(self->enemy->s.origin, self->monsterinfo.last_sighting);
	FoundTarget(self);
}


void monster_start_go(edict_t* self);


void monster_triggered_spawn(edict_t* self)
{
	self->s.origin[2] += 1;
	KillBox(self);

	self->solid = SOLID_BBOX;
	self->movetype = MOVETYPE_STEP;
	self->svflags &= ~SVF_NOCLIENT;
	self->air_finished = level.time + 12;
	gi.linkentity(self);

	monster_start_go(self);

	if (self->enemy && !(self->spawnflags & 1) && !(self->enemy->flags & FL_NOTARGET))
	{
		// SLUGFILLER--it's his own fault
		self->chain = NULL;
		VectorCopy(self->enemy->s.origin, self->monsterinfo.last_sighting);
		FoundTarget(self);
	}
	else
	{
		self->enemy = NULL;
	}
}

void monster_triggered_spawn_use(edict_t* self, edict_t* other, edict_t* activator)
{
	// we have a one frame delay here so we don't telefrag the guy who activated us
	self->think = monster_triggered_spawn;
	self->nextthink = level.time + FRAMETIME;
	if (activator->client)
		self->enemy = activator;
	self->use = monster_use;
}

void monster_triggered_start(edict_t* self)
{
	self->solid = SOLID_NOT;
	self->movetype = MOVETYPE_NONE;
	self->svflags |= SVF_NOCLIENT;
	self->nextthink = 0;
	self->use = monster_triggered_spawn_use;
}

void ED_CallSpawn(edict_t* ent);

void monster_respawn(edict_t* self) {
	trace_t tr;
	edict_t* respawner;
	edict_t* targcheck;

	// Create source teleportation
	respawner = G_Spawn();
	respawner->s.event = EV_PLAYER_TELEPORT;
	VectorCopy(self->s.origin, respawner->s.origin);
	VectorCopy(self->s.origin, respawner->s.old_origin);
	VectorCopy(self->s.angles, respawner->s.angles);
	respawner->nextthink = level.time + 0.1;
	respawner->think = G_FreeEdict;
	gi.linkentity(respawner);

	// Reinit monster;
	respawner = self->mynoise;
	G_InitEdict(self);
	self->mynoise = respawner;
	self->classname = respawner->classname;
	VectorCopy(respawner->s.origin, self->s.origin);
	VectorCopy(respawner->s.origin, self->s.old_origin);
	VectorCopy(respawner->s.angles, self->s.angles);
	if (respawner->combattarget) {
		targcheck = G_PickTarget(respawner->combattarget);
		if (targcheck) {
			if (strcmp(targcheck->classname, "point_combat") == 0)
				self->combattarget = respawner->combattarget;
		}
	}
	if (respawner->target) {
		targcheck = G_PickTarget(respawner->target);
		if (targcheck) {
			if (strcmp(targcheck->classname, "point_combat") == 0)
				self->combattarget = respawner->target;
			else if (strcmp(targcheck->classname, "path_corner") == 0)
				self->target = respawner->target;
		}
	}
	ED_CallSpawn(self);
	if (self->nextthink > 0)
	{
		self->nextthink = 0;
		self->think(self);
	}

	// Create destination teleportation and telefrag
	self->s.event = EV_PLAYER_TELEPORT;
	self->s.origin[2] += 1;
	self->s.old_origin[2] += 1;
	while (1)
	{
		tr = gi.trace(self->s.origin, self->mins, self->maxs, self->s.origin, self, MASK_PLAYERSOLID);
		if (!tr.ent)
			break;

		// nail it
		if (!tr.ent->client)
			T_Damage(tr.ent, self, self, vec3_origin, self->s.origin, vec3_origin, 100000, 100000, 0, DAMAGE_NO_PROTECTION | DAMAGE_NO_BLOOD, MOD_TELEFRAG);
		// it's a player - nail us
		else
			T_Damage(self, tr.ent, tr.ent, vec3_origin, tr.ent->s.origin, vec3_origin, 100000, 100000, 0, DAMAGE_NO_PROTECTION | DAMAGE_NO_BLOOD, MOD_TELEFRAG);

		// we've failed, let's leave it at that
		if (tr.ent->solid)
			break;
	}
}

void monster_respawner_think(edict_t* self) {
	// Owner may have been removed - in that case, spawn a new one
	if (!self->owner || self->owner->mynoise != self) {
		self->owner = G_Spawn();
		self->owner->mynoise = self;
		VectorCopy(self->s.origin, self->owner->s.origin);
	}
	monster_respawn(self->owner);
}

/*
==========================
SLUGFILLER--monster_dead_dead

Causes the monster ai to stop
==========================
*/
void monster_dead_dead(edict_t* self)
{
	self->nextthink = 0;
}

/*
================
monster_death_use

When a monster dies, it fires all of its targets with the current
enemy as activator.
================
*/
void monster_death_use(edict_t* self)
{
	self->flags &= ~(FL_FLY | FL_SWIM);
	// SLUGFILLER--last chance
	if (self->monsterinfo.aiflags & AI_DUCKED)
		self->monsterinfo.unduck(self);
	self->monsterinfo.aiflags &= AI_GOOD_GUY;

	if (self->mynoise) {
		// Respawn in 10 seconds
		self->mynoise->think = monster_respawner_think;
		self->mynoise->nextthink = level.time + 10;
	}

	if (self->item)
	{
		Drop_Item(self, self->item);
		self->item = NULL;
	}

	if (self->deathtarget)
		self->target = self->deathtarget;

	if (!self->target)
		return;

	G_UseTargets(self, self->enemy);
}


//============================================================================

qboolean monster_start(edict_t* self)
{
	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return false;
	}

	if ((self->spawnflags & 4) && !(self->monsterinfo.aiflags & AI_GOOD_GUY))
	{
		self->spawnflags &= ~4;
		self->spawnflags |= 1;
		//		gi.dprintf("fixed spawnflags on %s at %s\n", self->classname, vtos(self->s.origin));
	}

	if (!(self->monsterinfo.aiflags & AI_GOOD_GUY))
		level.total_monsters++;

	self->nextthink = level.time + FRAMETIME;
	self->svflags |= SVF_MONSTER;
	self->s.renderfx |= RF_FRAMELERP | RF_IR_VISIBLE;
	self->takedamage = DAMAGE_AIM;
	self->air_finished = level.time + 12;
	self->use = monster_use;
	self->max_health = self->health;
	self->max_gib_health = self->gib_health;
	self->clipmask = MASK_MONSTERSOLID;

	self->deadflag = DEAD_NO;
	self->svflags &= ~SVF_DEADMONSTER;

	if (!self->monsterinfo.checkattack)
		self->monsterinfo.checkattack = M_CheckAttack;
	VectorCopy(self->s.origin, self->s.old_origin);
	VectorCopy(self->s.angles, self->aimangles);
	self->ideal_yaw = self->s.angles[YAW];

	if (deathmatch->value) {
		self->spawnflags &= ~1;	// SLUGFILLER--no deaf monsters in fox-hunt
		self->targetname = NULL;
		self->deathtarget = NULL;
	}
	if ((deathmatch->value == 3 || skill->value == 3) && !(self->monsterinfo.aiflags & AI_GOOD_GUY) && !self->mynoise) {
		self->mynoise = G_Spawn();
		self->mynoise->classname = self->classname;
		VectorCopy(self->s.origin, self->mynoise->s.origin);
		VectorCopy(self->s.angles, self->mynoise->s.angles);
		self->mynoise->target = self->target;
		self->mynoise->combattarget = self->combattarget;
		self->mynoise->owner = self;
	}

	if (st.item)
	{
		self->item = FindItemByClassname(st.item);
		if (!self->item)
			gi.dprintf("%s at %s has bad item: %s\n", self->classname, vtos(self->s.origin), st.item);
	}

	// randomize what frame they start on
	if (self->monsterinfo.currentmove)
		self->s.frame = self->monsterinfo.currentmove->firstframe + (rand() % (self->monsterinfo.currentmove->lastframe - self->monsterinfo.currentmove->firstframe + 1));

	return true;
}

void monster_start_go(edict_t* self)
{
	vec3_t	v;

	if (self->health <= 0)
		return;

	// check for target to combat_point and change to combattarget
	if (self->target)
	{
		qboolean	notcombat;
		qboolean	fixup;
		edict_t* target;

		target = NULL;
		notcombat = false;
		fixup = false;
		while ((target = G_Find(target, FOFS(targetname), self->target)) != NULL)
		{
			if (strcmp(target->classname, "point_combat") == 0)
			{
				self->combattarget = self->target;
				fixup = true;
			}
			else
			{
				notcombat = true;
			}
		}
		if (notcombat && self->combattarget)
			gi.dprintf("%s at %s has target with mixed types\n", self->classname, vtos(self->s.origin));
		if (fixup)
			self->target = NULL;
	}

	// validate combattarget
	if (self->combattarget)
	{
		edict_t* target;

		target = NULL;
		while ((target = G_Find(target, FOFS(targetname), self->combattarget)) != NULL)
		{
			if (strcmp(target->classname, "point_combat") != 0)
			{
				gi.dprintf("%s at (%i %i %i) has a bad combattarget %s : %s at (%i %i %i)\n",
					self->classname, (int)self->s.origin[0], (int)self->s.origin[1], (int)self->s.origin[2],
					self->combattarget, target->classname, (int)target->s.origin[0], (int)target->s.origin[1],
					(int)target->s.origin[2]);
			}
		}
	}

	if (self->target)
	{
		self->goalentity = self->movetarget = G_PickTarget(self->target);
		if (!self->movetarget)
		{
			gi.dprintf("%s can't find target %s at %s\n", self->classname, self->target, vtos(self->s.origin));
			self->target = NULL;
			self->monsterinfo.pausetime = 0;
			self->monsterinfo.stand(self);
		}
		else if (strcmp(self->movetarget->classname, "path_corner") == 0)
		{
			VectorSubtract(self->goalentity->s.origin, self->s.origin, v);
			self->ideal_pitch = self->s.angles[PITCH] = 0;
			self->ideal_yaw = self->s.angles[YAW] = vectoyaw(v);
			self->monsterinfo.walk(self);
			self->target = NULL;
		}
		else
		{
			if (deathmatch->value)
				self->target = NULL;
			self->goalentity = self->movetarget = NULL;
			self->monsterinfo.pausetime = 0;
			self->monsterinfo.stand(self);
		}
	}
	else
	{
		self->monsterinfo.pausetime = 0;
		self->monsterinfo.stand(self);
	}

	self->think = monster_think;
	self->nextthink = level.time + FRAMETIME;
}


void walkmonster_start_go(edict_t* self)
{
	if (!(self->spawnflags & 2) && level.time < 1)
	{
		M_droptofloor(self);

		if (self->groundentity)
			if (!M_walkmove(self, 0, 0))
				gi.dprintf("%s in solid at %s\n", self->classname, vtos(self->s.origin));
	}

	if (!self->yaw_speed)
		self->yaw_speed = 14;
	self->viewheight = (self->headmins[2] + self->headmaxs[2]) / 2;

	monster_start_go(self);

	if (self->spawnflags & 2)
		monster_triggered_start(self);
}

void walkmonster_start(edict_t* self)
{
	self->think = walkmonster_start_go;
	monster_start(self);
}


void flymonster_start_go(edict_t* self)
{
	if (!M_walkmove(self, 0, 0))
		gi.dprintf("%s in solid at %s\n", self->classname, vtos(self->s.origin));

	if (!self->yaw_speed)
		self->yaw_speed = 10;
	self->viewheight = (self->headmins[2] + self->headmaxs[2]) / 2;

	monster_start_go(self);

	if (self->spawnflags & 2)
		monster_triggered_start(self);
}


void flymonster_start(edict_t* self)
{
	self->flags |= FL_FLY;
	self->think = flymonster_start_go;
	monster_start(self);
}


void swimmonster_start_go(edict_t* self)
{
	if (!self->yaw_speed)
		self->yaw_speed = 10;
	self->viewheight = (self->headmins[2] + self->headmaxs[2]) / 2;

	monster_start_go(self);

	if (self->spawnflags & 2)
		monster_triggered_start(self);
}

void swimmonster_start(edict_t* self)
{
	self->flags |= FL_SWIM;
	self->think = swimmonster_start_go;
	monster_start(self);
}
