#include "g_local.h"
#include "m_player.h"
#include "flashlight.h"


char* ClientTeam(edict_t* ent)
{
	char* p;
	static char	value[512];

	value[0] = 0;

	if (!ent->client)
		return value;

	if (teamplay->value)
	{
		// SLUGFILLER--team up using team/clan name
		strcpy(value, Info_ValueForKey(ent->client->pers.userinfo, "team"));
		return value;
	}

	strcpy(value, Info_ValueForKey(ent->client->pers.userinfo, "skin"));
	p = strchr(value, '/');
	if (!p)
		return value;

	if ((int)(dmflags->value) & DF_MODELTEAMS)
	{
		*p = 0;
		return value;
	}

	if ((int)(dmflags->value) & DF_SKINTEAMS)
		return ++p;

	// SLUGFILLER--no teamplay
	value[0] = 0;
	return value;
}

qboolean OnSameTeam(edict_t* ent1, edict_t* ent2)
{
	char	ent1Team[512];
	char	ent2Team[512];

	strcpy(ent1Team, ClientTeam(ent1));
	strcpy(ent2Team, ClientTeam(ent2));

	if (*ent1Team && strcmp(ent1Team, ent2Team) == 0)
		return true;
	return false;
}


void SelectNextItem(edict_t* ent, int itflags)
{
	gclient_t* cl;
	int			i, index;
	gitem_t* it;

	cl = ent->client;

	if (cl->chase_target) {
		ChaseNext(ent);
		return;
	}

	// scan  for the next valid one
	for (i = 1; i <= MAX_ITEMS; i++)
	{
		index = (cl->pers.selected_item + i) % MAX_ITEMS;
		if (!cl->pers.inventory[index])
			continue;
		it = &itemlist[index];
		if (!it->use && !it->drop)
			continue;
		if (!(it->flags & itflags))
			continue;

		cl->pers.selected_item = index;
		return;
	}

	cl->pers.selected_item = -1;
}

void SelectPrevItem(edict_t* ent, int itflags)
{
	gclient_t* cl;
	int			i, index;
	gitem_t* it;

	cl = ent->client;

	if (cl->chase_target) {
		ChasePrev(ent);
		return;
	}

	// scan  for the next valid one
	for (i = 1; i <= MAX_ITEMS; i++)
	{
		index = (cl->pers.selected_item + MAX_ITEMS - i) % MAX_ITEMS;
		if (!cl->pers.inventory[index])
			continue;
		it = &itemlist[index];
		if (!it->use && !it->drop)
			continue;
		if (!(it->flags & itflags))
			continue;

		cl->pers.selected_item = index;
		return;
	}

	cl->pers.selected_item = -1;
}

void ValidateSelectedItem(edict_t* ent)
{
	gclient_t* cl;

	cl = ent->client;

	if (cl->pers.selected_item != -1 && cl->pers.inventory[cl->pers.selected_item])
		return;		// valid

	SelectNextItem(ent, -1);
}


//=================================================================================

/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void Cmd_Give_f(edict_t* ent)
{
	char* name;
	gitem_t* it;
	int			index;
	int			i;
	qboolean	give_all;
	edict_t* it_ent;

	if ((deathmatch->value || coop->value == 2) && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	name = gi.args();

	if (Q_stricmp(name, "all") == 0)
		give_all = true;
	else
		give_all = false;

	if (give_all || Q_stricmp(gi.argv(1), "health") == 0)
	{
		if (gi.argc() == 3)
			ent->health = atoi(gi.argv(2));
		else
			ent->health = ent->max_health;
		if (ent->health <= 0)
		{
			ent->flags &= ~FL_GODMODE;
			meansOfDeath = MOD_SUICIDE;
			Killed(ent, ent, ent, 100000, vec3_origin);
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "weapons") == 0)
	{
		for (i = 0; i < game.num_items; i++)
		{
			it = itemlist + i;
			if (!it->pickup)
				continue;
			if (!(it->flags & IT_WEAPON))
				continue;
			ent->client->pers.inventory[i] += 1;
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "ammo") == 0)
	{
		for (i = 0; i < game.num_items; i++)
		{
			it = itemlist + i;
			if (!it->pickup)
				continue;
			if (!(it->flags & IT_AMMO))
				continue;
			Add_Ammo(ent, it, 1000);
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "armor") == 0)
	{
		gitem_armor_t* info;

		it = FindItem("Power Shield");
		it_ent = G_Spawn();
		it_ent->classname = it->classname;
		SpawnItem(it_ent, it);
		Touch_Item(it_ent, ent, NULL, NULL);
		if (it_ent->inuse)
			G_FreeEdict(it_ent);

		it = FindItem("Power Screen");
		it_ent = G_Spawn();
		it_ent->classname = it->classname;
		SpawnItem(it_ent, it);
		Touch_Item(it_ent, ent, NULL, NULL);
		if (it_ent->inuse)
			G_FreeEdict(it_ent);

		it = FindItem("Jacket Armor");
		ent->client->pers.inventory[ITEM_INDEX(it)] = 0;

		it = FindItem("Combat Armor");
		ent->client->pers.inventory[ITEM_INDEX(it)] = 0;

		it = FindItem("Body Armor");
		info = (gitem_armor_t*)it->info;
		ent->client->pers.inventory[ITEM_INDEX(it)] = info->max_count;

		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "items") == 0)
	{
		for (i = 0; i < game.num_items; i++)
		{
			it = itemlist + i;
			if (!it->pickup)
				continue;
			if (!(it->flags & IT_POWERUP))
				continue;
			if (it->flags & IT_TIMED)
				ent->client->pers.inventory[i] += 300;
			else
				ent->client->pers.inventory[i] += 30;
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "keys") == 0)
	{
		for (i = 0; i < game.num_items; i++)
		{
			it = itemlist + i;
			if (!it->pickup)
				continue;
			if (!(it->flags & IT_KEY))
				continue;
			ent->client->pers.inventory[i] += 1;
		}
		if (!give_all)
			return;
	}

	if (give_all)
	{
		for (i = 0; i < game.num_items; i++)
		{
			it = itemlist + i;
			if (!it->pickup)
				continue;
			if (it->flags & (IT_ARMOR | IT_WEAPON | IT_AMMO | IT_POWERUP | IT_KEY))
				continue;
			ent->client->pers.inventory[i] += 1;
		}
		return;
	}

	it = FindItem(name);
	if (!it)
	{
		name = gi.argv(1);
		it = FindItem(name);
		if (!it)
		{
			gi.cprintf(ent, PRINT_HIGH, "unknown item\n");
			return;
		}
	}

	if (!it->pickup)
	{
		gi.cprintf(ent, PRINT_HIGH, "non-pickup item\n");
		return;
	}

	index = ITEM_INDEX(it);

	if (it->flags & IT_AMMO)
	{
		if (gi.argc() == 3)
			ent->client->pers.inventory[index] = atoi(gi.argv(2));
		else
			ent->client->pers.inventory[index] += it->quantity;
	}
	else
	{
		it_ent = G_Spawn();
		it_ent->classname = it->classname;
		SpawnItem(it_ent, it);
		Touch_Item(it_ent, ent, NULL, NULL);
		if (it_ent->inuse)
			G_FreeEdict(it_ent);
	}
}


/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void Cmd_God_f(edict_t* ent)
{
	char* msg;

	if ((deathmatch->value || coop->value == 2) && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	ent->flags ^= FL_GODMODE;
	if (!(ent->flags & FL_GODMODE))
		msg = "godmode OFF\n";
	else
		msg = "godmode ON\n";

	ent->client->pers.savedFlags = (ent->flags & (FL_GODMODE | FL_NOTARGET));
	gi.cprintf(ent, PRINT_HIGH, msg);
}


/*
==================
Cmd_Notarget_f

Sets client to notarget

argv(0) notarget
==================
*/
void Cmd_Notarget_f(edict_t* ent)
{
	char* msg;

	if ((deathmatch->value || coop->value == 2) && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	ent->flags ^= FL_NOTARGET;
	if (!(ent->flags & FL_NOTARGET))
		msg = "notarget OFF\n";
	else
		msg = "notarget ON\n";

	ent->client->pers.savedFlags = (ent->flags & (FL_GODMODE | FL_NOTARGET));
	gi.cprintf(ent, PRINT_HIGH, msg);
}


/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void Cmd_Noclip_f(edict_t* ent)
{
	char* msg;

	if ((deathmatch->value || coop->value == 2) && !sv_cheats->value)
	{
		gi.cprintf(ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent->movetype == MOVETYPE_NOCLIP)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_NOCLIP;
		msg = "noclip ON\n";
	}

	gi.cprintf(ent, PRINT_HIGH, msg);
}


/*
==================
Cmd_Use_f

Use an inventory item
==================
*/
void Cmd_Use_f(edict_t* ent)
{
	int			index;
	gitem_t* it;
	char* s;

	s = gi.args();
	it = FindItem(s);
	if (!it)
	{
		gi.cprintf(ent, PRINT_HIGH, "unknown item: %s\n", s);
		return;
	}
	if (!it->use)
	{
		gi.cprintf(ent, PRINT_HIGH, "Item is not usable.\n");
		return;
	}
	index = ITEM_INDEX(it);
	if (!ent->client->pers.inventory[index])
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of item: %s\n", s);
		return;
	}

	it->use(ent, it);
}


/*
==================
Cmd_Drop_f

Drop an inventory item
==================
*/
void Cmd_Drop_f(edict_t* ent)
{
	int			index;
	gitem_t* it;
	char* s;

	s = gi.args();
	it = FindItem(s);
	if (!it)
	{
		gi.cprintf(ent, PRINT_HIGH, "unknown item: %s\n", s);
		return;
	}
	if (!it->drop)
	{
		gi.cprintf(ent, PRINT_HIGH, "Item is not dropable.\n");
		return;
	}
	index = ITEM_INDEX(it);
	if (!ent->client->pers.inventory[index])
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of item: %s\n", s);
		return;
	}

	it->drop(ent, it);
}


/*
=================
Cmd_Inven_f
=================
*/
void Cmd_Inven_f(edict_t* ent)
{
	gclient_t* cl;

	cl = ent->client;

	cl->showscores = false;
	cl->showhelp = false;

	if (cl->showinventory)
	{
		cl->showinventory = false;
		return;
	}

	cl->showinventory = true;
}

/*
=================
Cmd_InvUse_f
=================
*/
void Cmd_InvUse_f(edict_t* ent)
{
	gitem_t* it;

	ValidateSelectedItem(ent);

	if (ent->client->pers.selected_item == -1)
	{
		gi.cprintf(ent, PRINT_HIGH, "No item to use.\n");
		return;
	}

	it = &itemlist[ent->client->pers.selected_item];
	if (!it->use)
	{
		gi.cprintf(ent, PRINT_HIGH, "Item is not usable.\n");
		return;
	}
	it->use(ent, it);
}

/*
=================
Cmd_WeapPrev_f
=================
*/
void Cmd_WeapPrev_f(edict_t* ent)
{
	gclient_t* cl;
	int			i, index;
	gitem_t* it;
	int			selected_weapon;

	cl = ent->client;

	if (!cl->pers.weapon)
		return;

	selected_weapon = ITEM_INDEX(cl->pers.weapon);

	// scan  for the next valid one
	for (i = 1; i <= MAX_ITEMS; i++)
	{
		index = (selected_weapon + i) % MAX_ITEMS;
		if (!cl->pers.inventory[index])
			continue;
		it = &itemlist[index];
		if (!it->use)
			continue;
		if (!(it->flags & IT_WEAPON))
			continue;
		it->use(ent, it);
		if (cl->pers.weapon == it)
			return;	// successful
	}
}

/*
=================
Cmd_WeapNext_f
=================
*/
void Cmd_WeapNext_f(edict_t* ent)
{
	gclient_t* cl;
	int			i, index;
	gitem_t* it;
	int			selected_weapon;

	cl = ent->client;

	if (!cl->pers.weapon)
		return;

	selected_weapon = ITEM_INDEX(cl->pers.weapon);

	// scan  for the next valid one
	for (i = 1; i <= MAX_ITEMS; i++)
	{
		index = (selected_weapon + MAX_ITEMS - i) % MAX_ITEMS;
		if (!cl->pers.inventory[index])
			continue;
		it = &itemlist[index];
		if (!it->use)
			continue;
		if (!(it->flags & IT_WEAPON))
			continue;
		it->use(ent, it);
		if (cl->pers.weapon == it)
			return;	// successful
	}
}

/*
=================
Cmd_WeapLast_f
=================
*/
void Cmd_WeapLast_f(edict_t* ent)
{
	gclient_t* cl;
	int			index;
	gitem_t* it;

	cl = ent->client;

	if (!cl->pers.weapon || !cl->pers.lastweapon)
		return;

	index = ITEM_INDEX(cl->pers.lastweapon);
	if (!cl->pers.inventory[index])
		return;
	it = &itemlist[index];
	if (!it->use)
		return;
	if (!(it->flags & IT_WEAPON))
		return;
	it->use(ent, it);
}

/*
=================
Cmd_InvDrop_f
=================
*/
void Cmd_InvDrop_f(edict_t* ent)
{
	gitem_t* it;

	ValidateSelectedItem(ent);

	if (ent->client->pers.selected_item == -1)
	{
		gi.cprintf(ent, PRINT_HIGH, "No item to drop.\n");
		return;
	}

	it = &itemlist[ent->client->pers.selected_item];
	if (!it->drop)
	{
		gi.cprintf(ent, PRINT_HIGH, "Item is not dropable.\n");
		return;
	}
	it->drop(ent, it);
}

/*
=================
Cmd_Kill_f
=================
*/
void Cmd_Kill_f(edict_t* ent)
{
	if ((level.time - ent->client->respawn_time) < 5)
		return;
	ent->flags &= ~FL_GODMODE;
	ent->health = 0;
	meansOfDeath = MOD_SUICIDE;
	Killed(ent, ent, ent, 100000, vec3_origin);
}

/*
=================
Cmd_PutAway_f
=================
*/
void Cmd_PutAway_f(edict_t* ent)
{
	ent->client->showscores = false;
	ent->client->showhelp = false;
	ent->client->showinventory = false;
}


int PlayerSort(void const* a, void const* b)
{
	int		anum, bnum;

	anum = *(int*)a;
	bnum = *(int*)b;

	anum = game.clients[anum].ps.stats[STAT_FRAGS];
	bnum = game.clients[bnum].ps.stats[STAT_FRAGS];

	if (anum < bnum)
		return -1;
	if (anum > bnum)
		return 1;
	return 0;
}

/*
=================
Cmd_Players_f
=================
*/
void Cmd_Players_f(edict_t* ent)
{
	int		i;
	int		count;
	char	small[64];
	char	large[1280];
	int		index[256];

	count = 0;
	for (i = 0; i < maxclients->value; i++)
		if (game.clients[i].pers.connected)
		{
			index[count] = i;
			count++;
		}

	// sort by frags
	qsort(index, count, sizeof(index[0]), PlayerSort);

	// print information
	large[0] = 0;

	for (i = 0; i < count; i++)
	{
		Com_sprintf(small, sizeof(small), "%3i %s\n",
			game.clients[index[i]].ps.stats[STAT_FRAGS],
			game.clients[index[i]].pers.netname);
		if (strlen(small) + strlen(large) > sizeof(large) - 100)
		{	// can't print all of them in one packet
			strcat(large, "...\n");
			break;
		}
		strcat(large, small);
	}

	gi.cprintf(ent, PRINT_HIGH, "%s\n%i players\n", large, count);
}

/*
=================
Cmd_Wave_f
=================
*/
void Cmd_Wave_f(edict_t* ent)
{
	int		i;

	i = atoi(gi.argv(1));

	// can't wave when ducked
	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		return;

	if (ent->client->anim_priority > ANIM_WAVE)
		return;

	ent->client->anim_priority = ANIM_WAVE;

	switch (i)
	{
	case 0:
		gi.cprintf(ent, PRINT_HIGH, "flipoff\n");
		ent->s.frame = FRAME_flip01 - 1;
		ent->client->anim_end = FRAME_flip12;
		break;
	case 1:
		gi.cprintf(ent, PRINT_HIGH, "salute\n");
		ent->s.frame = FRAME_salute01 - 1;
		ent->client->anim_end = FRAME_salute11;
		break;
	case 2:
		gi.cprintf(ent, PRINT_HIGH, "taunt\n");
		ent->s.frame = FRAME_taunt01 - 1;
		ent->client->anim_end = FRAME_taunt17;
		break;
	case 3:
		gi.cprintf(ent, PRINT_HIGH, "wave\n");
		ent->s.frame = FRAME_wave01 - 1;
		ent->client->anim_end = FRAME_wave11;
		break;
	case 4:
	default:
		gi.cprintf(ent, PRINT_HIGH, "point\n");
		ent->s.frame = FRAME_point01 - 1;
		ent->client->anim_end = FRAME_point12;
		break;
	}
}

/*
==================
Cmd_Say_f
==================
*/
void Cmd_Say_f(edict_t* ent, qboolean say_team, qboolean arg0)
{
	int		i, j;
	edict_t* other;
	char* p;
	char	text[MAX_LAYOUT_LENGTH];
	gclient_t* cl;

	if (gi.argc() < 2 && !arg0)
		return;

	if (!((int)(dmflags->value) & (DF_MODELTEAMS | DF_SKINTEAMS)))
		say_team = false;

	if (say_team)
		Com_sprintf(text, sizeof(text), "(%s): ", ent->client->pers.netname);
	else
		Com_sprintf(text, sizeof(text), "%s: ", ent->client->pers.netname);

	if (arg0)
	{
		Com_strcat(text, sizeof(text), gi.argv(0));
		Com_strcat(text, sizeof(text), " ");
		Com_strcat(text, sizeof(text), gi.args());
	}
	else
	{
		p = gi.args();

		if (*p == '"')
		{
			p++;
			p[strlen(p) - 1] = 0;
		}
		Com_strcat(text, sizeof(text), p);
	}

	// don't let text be too long for malicious reasons
	if (strlen(text) > 150)
		text[150] = 0;

	Com_strcat(text, sizeof(text), "\n");

	if (flood_msgs->value) {
		cl = ent->client;

		if (level.time < cl->flood_locktill) {
			gi.cprintf(ent, PRINT_HIGH, "You can't talk for %d more seconds\n",
				(int)(cl->flood_locktill - level.time));
			return;
		}
		i = cl->flood_whenhead - flood_msgs->value + 1;
		if (i < 0)
			i = (sizeof(cl->flood_when) / sizeof(cl->flood_when[0])) + i;
		if (cl->flood_when[i] &&
			level.time - cl->flood_when[i] < flood_persecond->value) {
			cl->flood_locktill = level.time + flood_waitdelay->value;
			gi.cprintf(ent, PRINT_CHAT, "Flood protection:  You can't talk for %d seconds.\n",
				(int)flood_waitdelay->value);
			return;
		}
		cl->flood_whenhead = (cl->flood_whenhead + 1) %
			(sizeof(cl->flood_when) / sizeof(cl->flood_when[0]));
		cl->flood_when[cl->flood_whenhead] = level.time;
	}

	if (dedicated->value)
		gi.cprintf(NULL, PRINT_CHAT, "%s", text);

	for (j = 1; j <= maxclients->value; j++)
	{
		other = &g_edicts[j];
		if (!other->inuse)
			continue;
		if (!other->client)
			continue;
		if (say_team)
		{
			if (!OnSameTeam(ent, other))
				continue;
		}
		gi.cprintf(other, PRINT_CHAT, "%s", text);
	}
}

void Cmd_PlayerList_f(edict_t* ent)
{
	int i;
	char string[80];
	char text[MAX_LAYOUT_LENGTH] = { 0 };
	edict_t* ed = NULL;
	/* connect time, ping, score, name */

	// Start with a nice header.
	Com_sprintf(string, sizeof string, "%s\n", "entry ping score      name");
	Q_strncatz(text, sizeof text, string);
	Com_sprintf(string, sizeof string, "%s\n", "----- ---- -----  -------------");
	Q_strncatz(text, sizeof text, string);

	for (i = 0, ed = g_edicts + 1; i < maxclients->value; i++, ed++) {
		if (!ed->inuse)
			continue;

		Com_sprintf(string, sizeof string, "%02d:%02d %3d %4d    %-s %-12s\n",
			(level.framenum - ed->client->resp.enterframe) / 600,
			((level.framenum - ed->client->resp.enterframe) % 600) / 10,
			ed->client->ping,
			ed->client->resp.score,
			ed->client->pers.netname,
			ed->client->resp.spectator ? "(spectator)" : "");

		// While we might think we have the full frame to use
		// we can't take all of it for a console message, or it
		// gets dropped. 1000 seems a hard limit here.
		// The number of lines listed will depend on
		// lengths of names and spectator status. //QW//
		if (strlen(text) + strlen(string) > 1000)
		{
			Q_strncatz(text, sizeof text, "And more...\n");
			gi.cprintf(ent, PRINT_HIGH, "%s\n", text);
			return;
		}
		Q_strncatz(text, sizeof text, string);
	}
	gi.cprintf(ent, PRINT_HIGH, "%s\n", text);
}

temp_event_t tent = TE_GUNSHOT;
int tenttype = 3; // 0 = splash 1 = entity line 2 = line 3 = directional 4 = point 5 = offset line
int tentsplashcolor;

void FlushTEnt()
{
	switch (tent)
	{
	case TE_GUNSHOT:
		gi.dprintf("TE_GUNSHOT\n");
		tenttype = 3;
		break;
	case TE_BLOOD:
		gi.dprintf("TE_BLOOD\n");
		tenttype = 3;
		break;
	case TE_BLASTER:
		gi.dprintf("TE_BLASTER\n");
		tenttype = 3;
		break;
	case TE_RAILTRAIL:
		gi.dprintf("TE_RAILTRAIL\n");
		tenttype = 2;
		break;
	case TE_SHOTGUN:
		gi.dprintf("TE_SHOTGUN\n");
		tenttype = 3;
		break;
	case TE_EXPLOSION1:
		gi.dprintf("TE_EXPLOSION1\n");
		tenttype = 4;
		break;
	case TE_EXPLOSION2:
		gi.dprintf("TE_EXPLOSION2\n");
		tenttype = 4;
		break;
	case TE_ROCKET_EXPLOSION:
		gi.dprintf("TE_ROCKET_EXPLOSION\n");
		tenttype = 4;
		break;
	case TE_GRENADE_EXPLOSION:
		gi.dprintf("TE_GRENADE_EXPLOSION\n");
		tenttype = 4;
		break;
	case TE_SPARKS:
		gi.dprintf("TE_SPARKS\n");
		tenttype = 3;
		break;
	case TE_SPLASH:
		gi.dprintf("TE_SPLASH\n");
		tenttype = 0;
		break;
	case TE_BUBBLETRAIL:
		gi.dprintf("TE_BUBBLETRAIL\n");
		tenttype = 2;
		break;
	case TE_SCREEN_SPARKS:
		gi.dprintf("TE_SCREEN_SPARKS\n");
		tenttype = 3;
		break;
	case TE_SHIELD_SPARKS:
		gi.dprintf("TE_SHIELD_SPARKS\n");
		tenttype = 3;
		break;
	case TE_BULLET_SPARKS:
		gi.dprintf("TE_BULLET_SPARKS\n");
		tenttype = 3;
		break;
	case TE_LASER_SPARKS:
		gi.dprintf("TE_LASER_SPARKS\n");
		tenttype = 0;
		break;
	case TE_PARASITE_ATTACK:
		gi.dprintf("TE_PARASITE_ATTACK\n");
		tenttype = 1;
		break;
	case TE_ROCKET_EXPLOSION_WATER:
		gi.dprintf("TE_ROCKET_EXPLOSION_WATER\n");
		tenttype = 4;
		break;
	case TE_GRENADE_EXPLOSION_WATER:
		gi.dprintf("TE_GRENADE_EXPLOSION_WATER\n");
		tenttype = 4;
		break;
	case TE_MEDIC_CABLE_ATTACK:
		gi.dprintf("TE_MEDIC_CABLE_ATTACK\n");
		tenttype = 1;
		break;
	case TE_BFG_EXPLOSION:
		gi.dprintf("TE_BFG_EXPLOSION\n");
		tenttype = 4;
		break;
	case TE_BFG_BIGEXPLOSION:
		gi.dprintf("TE_BFG_BIGEXPLOSION\n");
		tenttype = 4;
		break;
	case TE_BOSSTPORT:
		gi.dprintf("TE_BOSSTPORT\n");
		tenttype = 4;
		break;
	case TE_BFG_LASER:
		gi.dprintf("TE_BFG_LASER\n");
		tenttype = 2;
		break;
	case TE_GRAPPLE_CABLE:
		gi.dprintf("TE_GRAPPLE_CABLE\n");
		tenttype = 5;
		break;
	case TE_WELDING_SPARKS:
		gi.dprintf("TE_WELDING_SPARKS\n");
		tenttype = 0;
		break;
	case TE_GREENBLOOD:
		gi.dprintf("TE_GREENBLOOD\n");
		tenttype = 3;
		break;
	case TE_BLUEHYPERBLASTER:
		gi.dprintf("TE_BLUEHYPERBLASTER\n");
		tenttype = 2;
		break;
	case TE_PLASMA_EXPLOSION:
		gi.dprintf("TE_PLASMA_EXPLOSION\n");
		tenttype = 4;
		break;
	case TE_TUNNEL_SPARKS:
		gi.dprintf("TE_TUNNEL_SPARKS\n");
		tenttype = 0;
		break;
	case TE_BLASTER2:
		gi.dprintf("TE_BLASTER2\n");
		tenttype = 3;
		break;
	case TE_RAILTRAIL2:
		gi.dprintf("TE_RAILTRAIL2\n");
		break;
	case TE_FLAME:
		gi.dprintf("TE_FLAME\n");
		break;
	case TE_LIGHTNING:
		gi.dprintf("TE_LIGHTNING\n");
		break;
	case TE_DEBUGTRAIL:
		gi.dprintf("TE_DEBUGTRAIL\n");
		tenttype = 2;
		break;
	case TE_PLAIN_EXPLOSION:
		gi.dprintf("TE_PLAIN_EXPLOSION\n");
		tenttype = 4;
		break;
	case TE_FLASHLIGHT:
		gi.dprintf("TE_FLASHLIGHT\n");
		break;
	case TE_FORCEWALL:
		gi.dprintf("TE_FORCEWALL\n");
		break;
	case TE_HEATBEAM:
		gi.dprintf("TE_HEATBEAM\n");
		break;
	case TE_MONSTER_HEATBEAM:
		gi.dprintf("TE_MONSTER_HEATBEAM\n");
		break;
	case TE_STEAM:
		gi.dprintf("TE_STEAM\n");
		break;
	case TE_BUBBLETRAIL2:
		gi.dprintf("TE_BUBBLETRAIL2\n");
		tenttype = 2;
		break;
	case TE_MOREBLOOD:
		gi.dprintf("TE_MOREBLOOD\n");
		tenttype = 3;
		break;
	case TE_HEATBEAM_SPARKS:
		gi.dprintf("TE_HEATBEAM_SPARKS\n");
		tenttype = 3;
		break;
	case TE_HEATBEAM_STEAM:
		gi.dprintf("TE_HEATBEAM_STEAM\n");
		tenttype = 3;
		break;
	case TE_CHAINFIST_SMOKE:
		gi.dprintf("TE_CHAINFIST_SMOKE\n");
		tenttype = 4;
		break;
	case TE_ELECTRIC_SPARKS:
		gi.dprintf("TE_ELECTRIC_SPARKS\n");
		tenttype = 3;
		break;
	case TE_TRACKER_EXPLOSION:
		gi.dprintf("TE_TRACKER_EXPLOSION\n");
		tenttype = 4;
		break;
	case TE_TELEPORT_EFFECT:
		gi.dprintf("TE_TELEPORT_EFFECT\n");
		tenttype = 4;
		break;
	case TE_DBALL_GOAL:
		gi.dprintf("TE_DBALL_GOAL\n");
		tenttype = 4;
		break;
	case TE_WIDOWBEAMOUT:
		gi.dprintf("TE_WIDOWBEAMOUT\n");
		break;
	case TE_NUKEBLAST:
		gi.dprintf("TE_NUKEBLAST\n");
		tenttype = 4;
		break;
	case TE_WIDOWSPLASH:
		gi.dprintf("TE_WIDOWSPLASH\n");
		tenttype = 4;
		break;
	case TE_EXPLOSION1_BIG:
		gi.dprintf("TE_EXPLOSION1_BIG\n");
		tenttype = 4;
		break;
	case TE_EXPLOSION1_NP:
		gi.dprintf("TE_EXPLOSION1_NP\n");
		tenttype = 4;
		break;
	case TE_FLECHETTE:
		gi.dprintf("TE_FLECHETTE\n");
		tenttype = 3;
		break;
	}
}

void FlushTEntType()
{
	switch (tenttype)
	{
	case 0:
		gi.dprintf("splash\n");
		break;
	case 1:
		gi.dprintf("entity line\n");
		break;
	case 2:
		gi.dprintf("line\n");
		break;
	case 3:
		gi.dprintf("directional\n");
		break;
	case 4:
		gi.dprintf("point\n");
		break;
	case 5:
		gi.dprintf("offset line\n");
		break;
	}
}

void Cmd_NextTEnt_f(edict_t* ent)
{
	if (tent < TE_FLECHETTE)
		tent++;
	FlushTEnt();
}

void Cmd_PrevTEnt_f(edict_t* ent)
{
	if (tent > TE_GUNSHOT)
		tent--;
	FlushTEnt();
}

void Cmd_NextTEntType_f(edict_t* ent)
{
	if (tenttype < 5)
		tenttype++;
	FlushTEntType();
}

void Cmd_PrevTEntType_f(edict_t* ent)
{
	if (tenttype > 0)
		tenttype--;
	FlushTEntType();
}

void Cmd_NextTEntColor_f(edict_t* ent)
{
	if (tentsplashcolor < 255)
		tentsplashcolor++;
	gi.dprintf("%d\n", tentsplashcolor);
}

void Cmd_PrevTEntColor_f(edict_t* ent)
{
	if (tentsplashcolor > 0)
		tentsplashcolor--;
	gi.dprintf("%d\n", tentsplashcolor);
}

void Cmd_ShowTEnt_f(edict_t* ent)
{
	trace_t tr;
	vec3_t forward, right, start, end;
	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorCopy(ent->s.origin, start);
	start[2] += ent->viewheight - 8;
	VectorMA(start, 1024, forward, end);
	tr = gi.trace(start, NULL, NULL, end, ent, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(tent);
		switch (tenttype)
		{
		case 0:
			gi.WriteByte(10);
			gi.WritePosition(tr.endpos);
			gi.WriteDir(tr.plane.normal);
			gi.WriteByte(tentsplashcolor);
			break;
		case 1:
			gi.WriteShort(ent - g_edicts);
			gi.WritePosition(start);
			gi.WritePosition(tr.endpos);
			gi.multicast(start, MULTICAST_PHS);
			gi.multicast(start, MULTICAST_PVS);
			break;
		case 2:
			gi.WritePosition(start);
			gi.WritePosition(tr.endpos);
			gi.multicast(start, MULTICAST_PHS);
			gi.multicast(start, MULTICAST_PVS);
			break;
		case 3:
			gi.WritePosition(tr.endpos);
			gi.WriteDir(tr.plane.normal);
			break;
		case 4:
			gi.WritePosition(tr.endpos);
			break;
		case 5:
			gi.WriteShort(ent - g_edicts);
			gi.WritePosition(start);
			gi.WritePosition(tr.endpos);
			gi.WritePosition(vec3_origin);
			gi.multicast(start, MULTICAST_PHS);
			gi.multicast(start, MULTICAST_PVS);
			break;
		}
		gi.multicast(tr.endpos, MULTICAST_PHS);
		gi.multicast(tr.endpos, MULTICAST_PVS);
	}
}



/*
=================
ClientCommand
=================
*/
void ClientCommand(edict_t* ent)
{
	char* cmd;

	if (!ent->client)
		return;		// not fully in game yet

	cmd = gi.argv(0);

	if (Q_stricmp(cmd, "players") == 0)
	{
		Cmd_Players_f(ent);
		return;
	}
	if (Q_stricmp(cmd, "say") == 0)
	{
		Cmd_Say_f(ent, false, false);
		return;
	}
	if (Q_stricmp(cmd, "say_team") == 0)
	{
		Cmd_Say_f(ent, true, false);
		return;
	}
	if (Q_stricmp(cmd, "score") == 0)
	{
		Cmd_Score_f(ent);
		return;
	}
	if (Q_stricmp(cmd, "help") == 0)
	{
		Cmd_Help_f(ent);
		return;
	}

	if (ent->client->resp.exitframe || ent->health <= 0)
		return;

	if (Q_stricmp(cmd, "use") == 0)
		Cmd_Use_f(ent);
	else if (Q_stricmp(cmd, "drop") == 0)
		Cmd_Drop_f(ent);
	else if (Q_stricmp(cmd, "give") == 0)
		Cmd_Give_f(ent);
	else if (Q_stricmp(cmd, "god") == 0)
		Cmd_God_f(ent);
	else if (Q_stricmp(cmd, "notarget") == 0)
		Cmd_Notarget_f(ent);
	else if (Q_stricmp(cmd, "noclip") == 0)
		Cmd_Noclip_f(ent);
	else if (Q_stricmp(cmd, "inven") == 0)
		Cmd_Inven_f(ent);
	else if (Q_stricmp(cmd, "invnext") == 0)
		SelectNextItem(ent, -1);
	else if (Q_stricmp(cmd, "invprev") == 0)
		SelectPrevItem(ent, -1);
	else if (Q_stricmp(cmd, "invnextw") == 0)
		SelectNextItem(ent, IT_WEAPON);
	else if (Q_stricmp(cmd, "invprevw") == 0)
		SelectPrevItem(ent, IT_WEAPON);
	else if (Q_stricmp(cmd, "invnextp") == 0)
		SelectNextItem(ent, IT_POWERUP);
	else if (Q_stricmp(cmd, "invprevp") == 0)
		SelectPrevItem(ent, IT_POWERUP);
	else if (Q_stricmp(cmd, "invuse") == 0)
		Cmd_InvUse_f(ent);
	else if (Q_stricmp(cmd, "invdrop") == 0)
		Cmd_InvDrop_f(ent);
	else if (Q_stricmp(cmd, "weapprev") == 0)
		Cmd_WeapPrev_f(ent);
	else if (Q_stricmp(cmd, "weapnext") == 0)
		Cmd_WeapNext_f(ent);
	else if (Q_stricmp(cmd, "weaplast") == 0)
		Cmd_WeapLast_f(ent);
	else if (Q_stricmp(cmd, "kill") == 0)
		Cmd_Kill_f(ent);
	else if (Q_stricmp(cmd, "putaway") == 0)
		Cmd_PutAway_f(ent);
	else if (Q_stricmp(cmd, "wave") == 0)
		Cmd_Wave_f(ent);
	else if (Q_stricmp(cmd, "chasecam") == 0)
		Cmd_Chasecam_Toggle(ent);
	else if (Q_stricmp(cmd, "playerlist") == 0)
		Cmd_PlayerList_f(ent);
	else if (Q_stricmp(cmd, "nexttent") == 0)
		Cmd_NextTEnt_f(ent);
	else if (Q_stricmp(cmd, "prevtent") == 0)
		Cmd_PrevTEnt_f(ent);
	else if (Q_stricmp(cmd, "nexttenttype") == 0)
		Cmd_NextTEntType_f(ent);
	else if (Q_stricmp(cmd, "prevtenttype") == 0)
		Cmd_PrevTEntType_f(ent);
	else if (Q_stricmp(cmd, "nexttentcolor") == 0)
		Cmd_NextTEntColor_f(ent);
	else if (Q_stricmp(cmd, "prevtentcolor") == 0)
		Cmd_PrevTEntColor_f(ent);
	else if (Q_stricmp(cmd, "showtent") == 0)
		Cmd_ShowTEnt_f(ent);
	else if (Q_stricmp(cmd, "flashlight") == 0)
		Cmd_Flashlight(ent);
	else	// anything that doesn't match a command will be a chat
		Cmd_Say_f(ent, false, true);
}
