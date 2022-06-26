#include "g_local.h"

void ChasecamThink(edict_t* ent);

// SLUGFILLER--spawn a chasecam
void ChasecamStart(edict_t* ent)
{
	edict_t* chasecam;
	ent->client->resp.chasetoggle = 1;
	ent->client->ps.gunindex = 0;
	ent->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION; // SLUGFILLER--no prediction in chasecam
	chasecam = G_Spawn();
	chasecam->owner = ent;
	chasecam->solid = SOLID_NOT;
	chasecam->movetype = MOVETYPE_NONE;
	VectorSet(chasecam->mins, -10, -10, -10);
	VectorSet(chasecam->maxs, 10, 10, 10);
	VectorCopy(ent->s.origin, chasecam->s.origin);
	chasecam->s.origin[2] += ent->viewheight;
	VectorCopy(chasecam->s.origin, chasecam->s.old_origin);
	chasecam->classname = "chasecam";
	chasecam->prethink = ChasecamThink;
	ent->client->resp.chasecam = chasecam;
	ent->client->resp.oldplayer = G_Spawn(); // SLUGFILLER--player image
}

void ChasecamRemove(edict_t* ent, char opt)
{
	if (opt == 2) // SLUGFILLER--put on hold
	{
		if (ent->client->pers.weapon)
			ent->client->ps.gunindex = gi.modelindex(ent->client->pers.weapon->view_model);
		ent->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
		ent->client->resp.chasetoggle = 2;
		G_FreeEdict(ent->client->resp.oldplayer);
		ent->client->resp.oldplayer = NULL;
	}
	else if (opt == 1) // SLUGFILLER--start removing chasecam
	{
		ent->client->resp.chasetoggle = 3;
	}
	else // SLUGFILLER--finish removing chasecam
	{
		if (ent->client->pers.weapon)
			ent->client->ps.gunindex = gi.modelindex(ent->client->pers.weapon->view_model);
		ent->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
		ent->client->resp.chasetoggle = 0;
		if (ent->client->resp.oldplayer)
		{
			G_FreeEdict(ent->client->resp.oldplayer);
			ent->client->resp.oldplayer = NULL;
		}
		G_FreeEdict(ent->client->resp.chasecam);
		ent->client->resp.chasecam = NULL;
	}
}

void ChasecamResume(edict_t* ent)
{
	ent->client->resp.chasetoggle = 1;
	ent->client->ps.gunindex = 0;
	ent->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION; // SLUGFILLER--no prediction in chasecam
	ent->client->resp.oldplayer = G_Spawn();
}

void ChasecamThink(edict_t* ent)
{
	trace_t	tr;
	vec3_t	spot, org;
	vec3_t	forward;
	float	dist;

	VectorCopy(ent->owner->s.origin, org);
	org[2] += ent->owner->viewheight;

	if (ent->owner->client->resp.chasetoggle == 3)
	{
		// SLUGFILLER--slide into player's head
		VectorCopy(org, ent->pos1);
	}
	else
	{
		// SLUGFILLER--set position
		AngleVectors(ent->owner->client->v_angle, forward, NULL, NULL);
		VectorMA(org, -100, forward, spot);
		tr = gi.trace(org, ent->mins, ent->maxs, spot, ent->owner, MASK_OPAQUE);
		VectorCopy(tr.endpos, ent->pos1);
	}

	// SLUGFILLER--move chasecam toward position
	VectorSubtract(ent->pos1, ent->s.origin, spot);
	dist = VectorLength(spot);

	if (dist <= 40)
		VectorAdd(ent->s.origin, spot, ent->s.origin);
	else
		VectorMA(ent->s.origin, 40 / dist, spot, ent->s.origin);

	VectorSubtract(ent->s.origin, org, spot);
	dist = VectorLength(spot);

	if (dist < 10)	// SLUGFILLER--put on hold
	{
		if (ent->owner->client->resp.chasetoggle == 3)
			ChasecamRemove(ent->owner, 0);
		else if (ent->owner->client->resp.chasetoggle != 2)
			ChasecamRemove(ent->owner, 2);
	}
	else			// SLUGFILLER--put out of hold
	{
		if (ent->owner->client->resp.chasetoggle == 2)
			ChasecamResume(ent->owner);
	}
}

void Cmd_Chasecam_Toggle(edict_t* ent)
{
	if (ent->movetype == MOVETYPE_NOCLIP)
		return;
	if (ent->client->resp.chasetoggle)
		ChasecamRemove(ent, 1); // SLUGFILLER--chasecam off
	else
		ChasecamStart(ent); // SLUGFILLER--chasecam on
}

// SLUGFILLER--update chasecam
void CheckChasecam_Viewent(edict_t* ent)
{
	VectorCopy(ent->velocity, ent->client->resp.oldplayer->velocity);
	ent->client->resp.oldplayer->s = ent->s;
	ent->client->resp.oldplayer->s.number = ent->client->resp.oldplayer - g_edicts;
	//	ent->client->resp.oldplayer->s.renderfx |= RF_TRANSLUCENT;
	gi.linkentity(ent->client->resp.oldplayer);
}
