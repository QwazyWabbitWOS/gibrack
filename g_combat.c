// g_combat.c

#include "g_local.h"

/*
============
CanDamage

Returns true if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
qboolean CanDamage (edict_t *targ, edict_t *inflictor)
{
	vec3_t	dest;
	trace_t	trace;

// bmodels need special checking because their origin is 0,0,0
	if (targ->movetype == MOVETYPE_PUSH)
	{
		VectorAdd (targ->absmin, targ->absmax, dest);
		VectorScale (dest, 0.5, dest);
		trace = gi.trace (inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
		if (trace.fraction == 1.0)
			return true;
		if (trace.ent == targ)
			return true;
		return false;
	}
	
	trace = gi.trace (inflictor->s.origin, vec3_origin, vec3_origin, targ->s.origin, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy (targ->s.origin, dest);
	dest[0] += 15.0;
	dest[1] += 15.0;
	trace = gi.trace (inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy (targ->s.origin, dest);
	dest[0] += 15.0;
	dest[1] -= 15.0;
	trace = gi.trace (inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy (targ->s.origin, dest);
	dest[0] -= 15.0;
	dest[1] += 15.0;
	trace = gi.trace (inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy (targ->s.origin, dest);
	dest[0] -= 15.0;
	dest[1] -= 15.0;
	trace = gi.trace (inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;


	return false;
}


void monster_end_deadpain (edict_t *self)
{
	self->s.frame++;
}

/*
============
Killed
============
*/
void Killed (edict_t *targ, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	if (targ->health < -999)
		targ->health = -999;

	targ->s.renderfx &= ~RF_IR_VISIBLE;

	if (attacker != targ)
		if (!(targ->svflags & SVF_MONSTER) || (attacker && (attacker->client || (attacker->svflags & SVF_MONSTER))))
		{
			targ->enemy = attacker;
			if (attacker->svflags & SVF_MONSTER)
			{
				// SLUGFILLER--whose fault is it
				if (attacker->chain)
				{
					// SLUGFILLER--the same one who made the one who shot us get shot
					targ->chain = attacker->chain;
				}
				else if (attacker->enemy && attacker->enemy->client)
				{
					// SLUGFILLER--he was aiming at a player so its the players fault
					targ->chain = attacker->enemy;
				}
				else
				{
					// SLUGFILLER--its no ones fault... he wasn't supposed to shoot anyway
					targ->chain = NULL;
				}
			}
			if ((targ->svflags & SVF_MONSTER) && !targ->deadflag)
				Reinforce (targ);	// SLUGFILLER--evange my death!
		}

	// SLUGFILLER--shooting a dead dude (try it)
	if (targ->flags & FL_NO_KNOCKBACK)
	{
		if ((targ->svflags & SVF_MONSTER) && !targ->nextthink)
		{
			targ->s.frame--;
			targ->nextthink = level.time + FRAMETIME;
			targ->think = monster_end_deadpain;
		}
		else if (targ->client && (targ->s.frame == targ->client->anim_end))
			targ->s.frame--;
	}

	if (((targ->svflags & SVF_MONSTER) || (targ->client)) && (targ->deadflag != DEAD_DEAD))
		ClientObituary (targ, inflictor, attacker);

	if ((targ->svflags & SVF_MONSTER) && (targ->deadflag != DEAD_DEAD))
	{
		targ->s.angles[PITCH] = 0;
		if (!(targ->monsterinfo.aiflags & AI_GOOD_GUY))
		{
			level.killed_monsters++;
			// medics won't heal monsters that they kill themselves
			if (strcmp(attacker->classname, "monster_medic") == 0)
				targ->owner = attacker;
		}
	}

	if (targ->movetype == MOVETYPE_PUSH || targ->movetype == MOVETYPE_STOP || targ->movetype == MOVETYPE_NONE)
	{	// doors, triggers, etc
		targ->die (targ, inflictor, attacker, damage, point);
		return;
	}

	if ((targ->svflags & SVF_MONSTER) && (targ->deadflag != DEAD_DEAD))
	{
		targ->touch = NULL;
		monster_death_use (targ);
	}

	targ->die (targ, inflictor, attacker, damage, point);
}


/*
================
SpawnBlood
================
*/
void SpawnDamage (int type, vec3_t origin, vec3_t normal, int damage)
{
	if (damage > 255)
		damage = 255;
	for (; damage > 0; damage -= 5) {
		if (level.blood_count > 50)
			return;
		level.blood_count++;
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (type);
//		gi.WriteByte (damage);
		gi.WritePosition (origin);
		gi.WriteDir (normal);
		gi.multicast (origin, MULTICAST_PVS);
	}
}


/*
============
T_Damage

targ		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: targ=monster, inflictor=rocket, attacker=player

dir			direction of the attack
point		point at which the damage is being inflicted
normal		normal vector from that point
damage		amount of damage being inflicted
knockback	force to be applied against targ as a result of the damage

dflags		these flags are used to control how T_Damage works
	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
	DAMAGE_NO_ARMOR			armor does not protect from this damage
	DAMAGE_ENERGY			damage is from an energy based weapon
	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
	DAMAGE_BULLET			damage is from a bullet (used for ricochets)
	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
	DAMAGE_NO_BLOOD			SLUGFILLER--no blood or sparks
============
*/
static int CheckPowerArmor (edict_t *ent, vec3_t point, vec3_t normal, int damage, int dflags)
{
	gclient_t	*client;
	int			save;
	int			power_armor_type;
	int			index;
	int			pa_te_type;
	int			power;
	int			power_used;

	if (!damage)
		return 0;

	client = ent->client;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	if (client)
	{
		power_armor_type = PowerArmorType (ent);
		if (power_armor_type != POWER_ARMOR_NONE)
		{
			index = ITEM_INDEX(FindItem("Cells"));
			power = client->pers.inventory[index];
		}
	}
	else if (ent->svflags & SVF_MONSTER)
	{
		power_armor_type = ent->monsterinfo.power_armor_type;
		power = ent->monsterinfo.power_armor_power;
	}
	else
		return 0;

	if (power_armor_type == POWER_ARMOR_NONE)
		return 0;
	if (!power)
		return 0;

	if (power_armor_type == POWER_ARMOR_SCREEN)
	{
		vec3_t		vec;
		float		dot;
		vec3_t		forward;

		// only works if damage point is in front
		AngleVectors (ent->s.angles, forward, NULL, NULL);
		VectorSubtract (point, ent->s.origin, vec);
		VectorNormalize (vec);
		dot = DotProduct (vec, forward);
		if (dot <= 0.3)
			return 0;

		pa_te_type = TE_SCREEN_SPARKS;
		if (!(dflags & DAMAGE_ENERGY))
			damage = damage * .80;
	}
	else
	{
		pa_te_type = TE_SHIELD_SPARKS;
		if (!(dflags & DAMAGE_ENERGY))
			damage = damage * .60;
	}

	save = power;
	if (!save)
		return 0;
	if (save > damage)
		save = damage;

	SpawnDamage (pa_te_type, point, normal, save);
	ent->powerarmor_time = level.time + 0.2;

	power_used = save;

	if (client)
		client->pers.inventory[index] -= power_used;
	else
		ent->monsterinfo.power_armor_power -= power_used;
	return save;
}

static int CheckArmor (edict_t *ent, vec3_t point, vec3_t normal, int damage, int te_sparks, int dflags)
{
	gclient_t	*client;
	int			save;
	int			index;
	gitem_t		*armor;

	if (!damage)
		return 0;

	client = ent->client;

	if (!client)
		return 0;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	index = ArmorIndex (ent);
	if (!index)
		return 0;

	armor = GetItemByIndex (index);

	if (dflags & DAMAGE_ENERGY)
		save = ceil(((gitem_armor_t *)armor->info)->energy_protection*damage);
	else
		save = ceil(((gitem_armor_t *)armor->info)->normal_protection*damage);
	if (save >= client->pers.inventory[index])
		save = client->pers.inventory[index];

	if (!save)
		return 0;

	client->pers.inventory[index] -= save;
	SpawnDamage (te_sparks, point, normal, save);

	return save;
}

void M_ReactToDamage (edict_t *targ, edict_t *inflictor, edict_t *attacker)
{
	if (!(attacker->client) && !(attacker->svflags & SVF_MONSTER))
		return;

	if (attacker == targ || attacker == targ->enemy)
		return;

	// if we are a good guy monster and our attacker is a player
	// or another good guy, do not get mad at them
	if (targ->monsterinfo.aiflags & AI_GOOD_GUY)
	{
		if (attacker->client || (attacker->monsterinfo.aiflags & AI_GOOD_GUY))
			return;
	}

	// we now know that we are not both good guys

	// if attacker is a client, get mad at them because he's good and we're not
	if (attacker->client)
	{
		targ->monsterinfo.aiflags &= ~AI_SOUND_TARGET;

		// this can only happen in coop (both new and old enemies are clients)
		// only switch if can't see the current enemy
		if (targ->enemy && targ->enemy->client)
		{
			if (visible(targ, targ->enemy))
			{
				targ->oldenemy = attacker;
				return;
			}
			targ->oldenemy = targ->enemy;
		}
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
		return;
	}

	// it's the same base (walk/swim/fly) type and a different classname and it's not a tank
	// (they spray too much), get mad at them
	// SLUGFILLER--ground monsters can fight fly monsters vice-versa
	// that does not apply for swim monsters
	if (((targ->flags & FL_SWIM) == (attacker->flags & FL_SWIM)) &&
		 (strcmp (targ->classname, attacker->classname) != 0) &&
		 (strcmp (attacker->classname, "monster_tank") != 0) &&
		 (strcmp (attacker->classname, "monster_supertank") != 0) &&
		 (strcmp (attacker->classname, "monster_makron") != 0) &&
		 (strcmp (attacker->classname, "monster_jorg") != 0) )
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->monsterinfo.aiflags &= ~AI_SOUND_TARGET;
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
	}
	// if they *meant* to shoot us, SLUGFILLER--or one of our buddies, then shoot back
	else if (attacker->enemy && strcmp (targ->classname, attacker->enemy->classname) == 0)
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->monsterinfo.aiflags &= ~AI_SOUND_TARGET;
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
	}
	// otherwise get mad at whoever they are mad at (help our buddy) unless it is us!
	else if (attacker->enemy && attacker->enemy != targ)
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->monsterinfo.aiflags &= ~AI_SOUND_TARGET;
		targ->enemy = attacker->enemy;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
	}

	// SLUGFILLER--if we didnt get an enemy then were done
	if (!targ->enemy)
		return;
	
	// SLUGFILLER--we got mad at the one that shot us
	if (targ->enemy == attacker)
	{
		if (visible (targ, attacker))
			VectorCopy (attacker->s.origin, targ->monsterinfo.last_sighting);
		else if (visible (targ, inflictor))
			VectorCopy (inflictor->s.origin, targ->monsterinfo.last_sighting);
		else
			VectorCopy (targ->s.origin, targ->monsterinfo.last_sighting);
		if (!(attacker->svflags & SVF_MONSTER))
		{
			targ->chain = NULL;
			return;
		}
		// SLUGFILLER--whose fault is it
		if (attacker->chain)
		{
			// SLUGFILLER--the same one who made the one who shot us get shot
			targ->chain = attacker->chain;
		}
		else if (attacker->enemy && attacker->enemy->client)
		{
			// SLUGFILLER--he was aiming at a player so its the players fault
			targ->chain = attacker->enemy;
		}
		else
		{
			// SLUGFILLER--its no ones fault... he wasn't supposed to shoot anyway
			targ->chain = NULL;
		}
	}
	else if (targ->enemy == attacker->enemy)
	{
		if (visible (targ, attacker->enemy))
			VectorCopy (attacker->enemy->s.origin, targ->monsterinfo.last_sighting);
		else if (visible (targ, attacker))
			VectorCopy (attacker->s.origin, targ->monsterinfo.last_sighting);
		else if (visible (targ, inflictor))
			VectorCopy (inflictor->s.origin, targ->monsterinfo.last_sighting);
		else
			VectorCopy (targ->s.origin, targ->monsterinfo.last_sighting);
		if (!(attacker->enemy->svflags & SVF_MONSTER))
		{
			targ->chain = NULL;
			return;
		}
		// SLUGFILLER--we are helping our buddy to kill another monster
		// the same guy who caused our buddy to attack that monster
		// caused us to attack that monster
		targ->chain = attacker->chain;
	}
}

qboolean CheckTeamDamage (edict_t *targ, edict_t *attacker)
{
		//FIXME make the next line real and uncomment this block
		// if ((ability to damage a teammate == OFF) && (targ's team == attacker's team))
	return false;
}

// SLUGFILLER--Head shot does more damage
qboolean CheckHeadShot (edict_t *targ, vec3_t point, vec3_t dir)
{
	vec3_t	forward, right, up, org, org2, dir2;
	int		i;
	float	scale, scale1;

	// SLUGFILLER--only if the target HAS a head
	if (!(targ->svflags & SVF_MONSTER) && !targ->client)
		return false;

	if (targ->health <= 0)
		return false;

	// SLUGFILLER--project the point on the target
	VectorSubtract (point, targ->s.origin, org);
	AngleVectors (targ->s.angles, forward, right, up);
	org2[0] = DotProduct (org, forward);
	org2[1] = -DotProduct (org, right);
	org2[2] = DotProduct (org, up);
	dir2[0] = DotProduct (dir, forward);
	dir2[1] = -DotProduct (dir, right);
	dir2[2] = DotProduct (dir, up);

	// SLUGFILLER--clip our point to the target's bbox
	scale1 = 0;
	for (i=0 ; i<3 ; i++)
	{
		if (!org2[i])
			continue;
		if (dir2[i] < 0)
		{
			scale = (targ->maxs[i]-org2[i]) / dir2[i];
			if (scale > scale1)
				scale1 = scale;
		}
		else
		{
			scale = (targ->mins[i]-org2[i]) / dir2[i];
			if (scale > scale1)
				scale1 = scale;
		}
	}
	VectorMA (org2, scale1, dir2, org);
	
	// SLUGFILLER--check if head was hit
	for (i=0 ; i<3 ; i++)
	{
		if (org[i] > targ->headmaxs[i] || org[i] < targ->headmins[i])
			return false;
	}

	return true;
}

void GetHeadOrigin (edict_t *targ, vec3_t org)
{
	vec3_t	org2, forward, right;

	AngleVectors (targ->s.angles, forward, right, NULL);
	VectorAdd (targ->headmaxs, targ->headmins, org);
	VectorScale (org, 0.5, org2);
	G_ProjectSource (targ->s.origin, org2, forward, right, org);
}

void T_Damage (edict_t *targ, edict_t *inflictor, edict_t *attacker, vec3_t dir, vec3_t point, vec3_t normal, int damage, int gib_damage, int knockback, int dflags, int mod)
{
	gclient_t	*client;
	int			take;
	int			gib_take;
	int			save;
	int			asave;
	int			psave;
	int			te_sparks;

	if (!targ->takedamage)
		return;

	if ((int)(gibflags->value) & GF_REAL_DAMAGE)
	{
		damage *= 4;
		knockback *= 2;
	}

	// friendly fire avoidance
	// if enabled you can't hurt teammates (but you can hurt yourself)
	// knockback still occurs
	if (targ->client && attacker->client && targ != attacker)
	{
		if (OnSameTeam (targ, attacker))
		{
			if ((int)(dmflags->value) & DF_NO_FRIENDLY_FIRE)
			{
				damage = 0;
				gib_damage = 0;
			}
			else
				mod |= MOD_FRIENDLY_FIRE;
		}
	}
	meansOfDeath = mod;

	// easy mode takes half damage
	if (skill->value == 0 && targ->client && !attacker->client)
	{
		damage *= 0.5;
		if (!damage)
			damage = 1;
	}

	client = targ->client;

	if (dflags & DAMAGE_BULLET)
		te_sparks = TE_BULLET_SPARKS;
	else
		te_sparks = TE_SPARKS;

	VectorNormalize(dir);

	// bonus damage for suprising a monster
	if (!(dflags & DAMAGE_RADIUS) && (targ->svflags & SVF_MONSTER) && (attacker->client) && (!targ->enemy) && (targ->health > 0))
		damage *= 2;

	// SLUGFILLER--moved the FL_NO_KNOCKBACK effect so the knockback value isn't reset

	// figure momentum add
	if (!(dflags & DAMAGE_NO_KNOCKBACK) && !(targ->flags & FL_NO_KNOCKBACK))
	{
		if ((knockback) && (targ->movetype != MOVETYPE_NONE) && (targ->movetype != MOVETYPE_BOUNCE) && (targ->movetype != MOVETYPE_PUSH) && (targ->movetype != MOVETYPE_STOP))
		{
			vec3_t	kvel;
			float	mass;

			if (targ->mass < 50)
				mass = 50;
			else
				mass = targ->mass;

			VectorCopy (dir, kvel);
			
			VectorScale (kvel, 1000.0 * (float)knockback / mass, kvel);	// SLUGFILLER--no rocket jump hack...

			VectorAdd (targ->velocity, kvel, targ->velocity);
		}
	}

	// SLUGFILLER--new gib system
	gib_take = damage;
	take = damage;
	save = 0;

	// check for godmode
	if ( (targ->flags & FL_GODMODE) && !(dflags & DAMAGE_NO_PROTECTION) )
	{
		take = 0;
		save = damage;
		SpawnDamage (te_sparks, point, normal, save);
	}

	// check for invincibility
	if ((client && client->pers.itemused[ITEM_INDEX(FindItem("Invulnerability"))] ) && !(dflags & DAMAGE_NO_PROTECTION))
	{
		if (targ->pain_debounce_time < level.time)
		{
			gi.sound(targ, CHAN_ITEM, gi.soundindex("items/protect4.wav"), 1, ATTN_NORM, 0);
			targ->pain_debounce_time = level.time + 2;
		}
		take = 0;
		save = damage;
	}

	psave = CheckPowerArmor (targ, point, normal, take, dflags);
	take -= psave;

	// SLUGFILLER--head shots
	if (!(dflags & DAMAGE_RADIUS) && CheckHeadShot (targ, point, dir))
		meansOfDeath |= MOD_HEAD_SHOT;

	asave = CheckArmor (targ, point, normal, take, te_sparks, dflags);
	take -= asave;

	if (meansOfDeath & MOD_HEAD_SHOT)
		take *= 4;

	//treat cheat/powerup savings the same as armor
	asave += save;

	// team damage avoidance
	if (!(dflags & DAMAGE_NO_PROTECTION) && CheckTeamDamage (targ, attacker))
		return;

	// do the damage
	if (take)
	{
		// SLUGFILLER--head splatter
		if (meansOfDeath & MOD_HEAD_SHOT)
		{
			vec3_t	org;

			GetHeadOrigin (targ, org);
			SpawnDamage (TE_BLOOD, org, normal, take);
			SpawnDamage (TE_BLOOD, org, normal, take);
			SpawnDamage (TE_BLOOD, org, normal, take);

			gi.sound (targ, CHAN_BODY, gi.soundindex ("misc/hshot.wav"), 1, ATTN_NORM, 0);
		}

		if (!(dflags & DAMAGE_NO_BLOOD))
			if ((targ->svflags & SVF_MONSTER) || (client))
			{
				vec3_t	vec, forward, right, up;

				SpawnDamage (TE_BLOOD, point, normal, take);
				VectorScale (dir, 500, vec);
				vec[2] += 150;
				ThrowBlood (point, vec);
				vectoangles (dir, vec);
				AngleVectors (vec, forward, right, up);
				VectorScale (dir, -30, vec);
				vec[2] += 150;
				VectorMA (vec, crandom()*40, right, vec);
				VectorMA (vec, crandom()*40, up, vec);
				ThrowBlood (point, vec);
			}
			else
				SpawnDamage (te_sparks, point, normal, take);

		targ->health -= take;
		targ->gib_health -= gib_damage;
			
		if (targ->health <= 0)
		{
			Killed (targ, inflictor, attacker, take, point);
			return;
		}
	}

	if (targ->svflags & SVF_MONSTER)
	{
		M_ReactToDamage (targ, inflictor, attacker);
		if (!(targ->monsterinfo.aiflags & AI_DUCKED) && (take))
		{
			targ->pain (targ, attacker, knockback, take);
			// nightmare mode monsters don't go into pain frames often
			if (skill->value == 3)
				targ->pain_debounce_time = level.time + 5;
		}
	}
	else if (client)
	{
		if (!(targ->flags & FL_GODMODE) && (take))
			targ->pain (targ, attacker, knockback, take);
	}
	else if (take)
	{
		if (targ->pain)
			targ->pain (targ, attacker, knockback, take);
	}

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if (client)
	{
		client->damage_parmor += psave;
		client->damage_armor += asave;
		client->damage_blood += take;
		client->damage_knockback += knockback;
		VectorCopy (point, client->damage_from);
	}
}


/*
============
T_RadiusDamage
============
*/
void T_RadiusDamage (edict_t *inflictor, edict_t *attacker, float damage, edict_t *ignore, float radius, int mod)
{
	float	points;
	edict_t	*ent = NULL;
	vec3_t	v;
	vec3_t	dir;

	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		VectorAdd (ent->mins, ent->maxs, v);
		VectorMA (ent->s.origin, 0.5, v, v);
		VectorSubtract (inflictor->s.origin, v, v);
		points = damage - 0.5 * VectorLength (v);
		if (points > 0)
		{
			if (CanDamage (ent, inflictor))
			{
				VectorSubtract (ent->s.origin, inflictor->s.origin, dir);
				T_Damage (ent, inflictor, attacker, dir, inflictor->s.origin, vec3_origin, (int)points, (int)points, (int)points, DAMAGE_RADIUS | DAMAGE_NO_BLOOD, mod);
			}
		}
	}
}
