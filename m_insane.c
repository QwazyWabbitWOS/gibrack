/*
==============================================================================

insane

==============================================================================
*/

#include "g_local.h"
#include "m_insane.h"


static int	sound_fist;
static int	sound_shake;
static int	sound_moan;
static int	sound_scream[8];

void fix_bbox1(edict_t* self)
{
	self->maxs[2] = 26;
	self->headmins[2] = 16;
	self->headmaxs[2] = 26;
	gi.linkentity(self);
}

void fix_bbox2(edict_t* self)
{
	self->maxs[2] = 24;
	self->headmins[2] = 14;
	self->headmaxs[2] = 24;
	gi.linkentity(self);
}

void fix_bbox3(edict_t* self)
{
	self->maxs[2] = 20;
	self->headmins[2] = 10;
	self->headmaxs[2] = 20;
	gi.linkentity(self);
}

void fix_bbox4(edict_t* self)
{
	self->maxs[2] = 14;
	self->headmins[0] = 6;
	self->headmins[2] = 4;
	self->headmaxs[2] = 14;
	gi.linkentity(self);
}

void fix_bbox5(edict_t* self)
{
	self->maxs[2] = 10;
	self->headmins[2] = 0;
	self->headmaxs[2] = 10;
	gi.linkentity(self);
}

void fix_bbox6(edict_t* self)
{
	self->maxs[2] = 10;
	self->headmins[0] = 10;
	self->headmins[2] = -2;
	self->headmaxs[2] = 6;
	gi.linkentity(self);
}

void fix_bbox7(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 2;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 12;
	self->headmins[2] = 0;
	self->headmaxs[2] = 10;
	gi.linkentity(self);
}

void fix_bbox8(edict_t* self)
{
	self->maxs[2] = 8;
	self->headmins[2] = -4;
	self->headmaxs[2] = 4;
	gi.linkentity(self);
}

void fix_bbox9(edict_t* self)
{
	self->maxs[2] = 0;
	self->headmins[2] = -18;
	self->headmaxs[2] = -10;
	gi.linkentity(self);
}

void fix_bbox10(edict_t* self)
{
	self->maxs[2] = 0;
	self->headmins[2] = -10;
	self->headmaxs[2] = 0;
	gi.linkentity(self);
}

void fix_bbox11(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 10;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 10;
	self->headmins[0] = 4;
	self->headmins[2] = 0;
	self->headmaxs[2] = 10;
	gi.linkentity(self);
}

void fix_bbox12(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 4;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 14;
	self->headmins[2] = 4;
	self->headmaxs[2] = 14;
	gi.linkentity(self);
}

void fix_bbox13(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 4;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 18;
	self->headmins[2] = 8;
	self->headmaxs[2] = 18;
	gi.linkentity(self);
}

void fix_bbox14(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 4;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 22;
	self->headmins[0] = -16;
	self->headmins[2] = 12;
	self->headmaxs[2] = 22;
	gi.linkentity(self);
}

void fix_bbox15(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 6;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 28;
	self->headmins[2] = 18;
	self->headmaxs[2] = 28;
	gi.linkentity(self);
}

void fix_bbox16(edict_t* self)
{
	self->maxs[2] = 26;
	self->headmins[2] = 16;
	self->headmaxs[2] = 26;
	gi.linkentity(self);
}

void fix_bbox17(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 2;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 28;
	self->headmins[2] = 18;
	self->headmaxs[2] = 28;
	gi.linkentity(self);
}

void fix_bbox18(edict_t* self)
{
	self->maxs[2] = 24;
	self->headmins[2] = 10;
	self->headmaxs[2] = 24;
	gi.linkentity(self);
}

void fix_bbox19(edict_t* self)
{
	self->maxs[2] = 20;
	self->headmins[2] = 10;
	self->headmaxs[2] = 20;
	gi.linkentity(self);
}

void fix_bbox20(edict_t* self)
{
	self->maxs[2] = 14;
	self->headmins[2] = 4;
	self->headmaxs[2] = 14;
	gi.linkentity(self);
}

void fix_bbox21(edict_t* self)
{
	self->maxs[2] = 10;
	self->headmins[0] = 10;
	self->headmins[2] = 0;
	self->headmaxs[2] = 10;
	gi.linkentity(self);
}

void fix_bbox22(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 6;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 16;
	self->headmins[2] = 6;
	self->headmaxs[2] = 16;
	gi.linkentity(self);
}

void fix_bbox23(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 4;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 20;
	self->headmins[2] = 10;
	self->headmaxs[2] = 20;
	gi.linkentity(self);
}

void fix_bbox24(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 8;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 28;
	self->headmins[0] = -16;
	self->headmins[2] = 18;
	self->headmaxs[2] = 28;
	gi.linkentity(self);
}

void fix_bbox25(edict_t* self)
{
	self->maxs[2] = 14;
	self->headmins[2] = 4;
	self->headmaxs[2] = 14;
	gi.linkentity(self);
}

void fix_bbox26(edict_t* self)
{
	self->maxs[2] = 4;
	self->headmins[0] = 10;
	self->headmins[2] = -8;
	self->headmaxs[2] = 2;
	gi.linkentity(self);
}

void fix_bbox27(edict_t* self)
{
	self->maxs[2] = -2;
	self->headmins[2] = -16;
	self->headmaxs[2] = -6;
	gi.linkentity(self);
}

void fix_bbox28(edict_t* self)
{
	self->maxs[2] = -6;
	self->headmins[0] = 10;
	self->headmins[2] = -22;
	self->headmaxs[2] = -12;
	gi.linkentity(self);
}

void fix_bbox29(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 4;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = -2;
	self->headmins[2] = -16;
	self->headmaxs[2] = -6;
	gi.linkentity(self);
}

void fix_bbox30(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 8;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 4;
	self->headmins[0] = -16;
	self->headmins[2] = -4;
	self->headmaxs[2] = 4;
	gi.linkentity(self);
}

void fix_bbox31(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 2;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = -4;
	self->headmins[2] = -18;
	self->headmaxs[2] = -10;
	gi.linkentity(self);
}

void fix_bbox32(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 10;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 6;
	self->headmins[0] = -16;
	self->headmins[2] = -4;
	self->headmaxs[2] = 6;
	gi.linkentity(self);
}

void fix_bbox33(edict_t* self)
{
	self->maxs[2] = 8;
	self->headmins[2] = -2;
	self->headmaxs[2] = 8;
	gi.linkentity(self);
}

void fix_bbox34(edict_t* self)
{
	self->maxs[2] = 0;
	self->headmins[0] = 10;
	self->headmins[2] = -12;
	self->headmaxs[2] = -2;
	gi.linkentity(self);
}

void fix_bbox35(edict_t* self)
{
	self->maxs[2] = 22;
	self->headmins[2] = 12;
	self->headmaxs[2] = 22;
	gi.linkentity(self);
}

void fix_bbox36(edict_t* self)
{
	self->maxs[2] = 16;
	self->headmins[0] = 10;
	self->headmins[2] = 6;
	self->headmaxs[2] = 14;
	gi.linkentity(self);
}

void fix_bbox37(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 6;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 22;
	self->headmins[2] = 10;
	self->headmaxs[2] = 18;
	gi.linkentity(self);
}

void fix_bbox38(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 4;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 26;
	self->headmins[0] = -16;
	self->headmins[2] = 16;
	self->headmaxs[2] = 26;
	gi.linkentity(self);
}

void fix_bbox39(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 2;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 28;
	self->headmins[2] = 18;
	self->headmaxs[2] = 28;
	gi.linkentity(self);
}

void fix_bbox40(edict_t* self)
{
	self->maxs[2] = 24;
	self->headmins[2] = 14;
	self->headmaxs[2] = 24;
	gi.linkentity(self);
}

void fix_bbox41(edict_t* self)
{
	self->maxs[2] = 20;
	self->headmins[2] = 10;
	self->headmaxs[2] = 20;
	gi.linkentity(self);
}

void fix_bbox42(edict_t* self)
{
	vec3_t	end;
	trace_t	trace;
	VectorCopy(self->s.origin, end);
	end[2] += 8;
	trace = gi.trace(self->s.origin, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);
	if (trace.fraction < 0.1)
	{
		self->monsterinfo.aiflags |= AI_HOLD_FRAME;
		return;
	}
	self->monsterinfo.aiflags &= ~AI_HOLD_FRAME;
	self->maxs[2] = 28;
	self->headmins[2] = 18;
	self->headmaxs[2] = 28;
	gi.linkentity(self);
}

void fix_bbox43(edict_t* self)
{
	self->headmins[2] = -18;
	self->headmaxs[2] = -8;
	gi.linkentity(self);
}

void insane_fist(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_fist, 1, ATTN_IDLE, 0);
}

void insane_shake(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_shake, 1, ATTN_IDLE, 0);
}

void insane_moan(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_moan, 1, ATTN_IDLE, 0);
}

void insane_scream(edict_t* self)
{
	gi.sound(self, CHAN_VOICE, sound_scream[rand() % 8], 1, ATTN_IDLE, 0);
}


void insane_stand(edict_t* self);
void insane_dead(edict_t* self);
void insane_cross(edict_t* self);
void insane_walk(edict_t* self);
void insane_run(edict_t* self);
void insane_checkdown(edict_t* self);
void insane_checkup(edict_t* self);
void insane_onground(edict_t* self);


mframe_t insane_frames_stand_normal[] =
{
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, NULL,
	MANIM_STAND, 0, insane_checkdown
};
mmove_t insane_move_stand_normal = { FRAME_stand60, FRAME_stand65, insane_frames_stand_normal, insane_stand };

mframe_t insane_frames_stand_insane[] =
{
	MANIM_STAND,	0,	insane_shake,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox16,
	MANIM_STAND,	0,	fix_bbox17,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox16,
	MANIM_STAND,	0,	fix_bbox17,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox16,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox18,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox19,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox20,
	MANIM_STAND,	0,	fix_bbox21,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	fix_bbox22,
	MANIM_STAND,	0,	fix_bbox23,
	MANIM_STAND,	0,	fix_bbox24,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	NULL,
	MANIM_STAND,	0,	insane_checkdown
};
mmove_t insane_move_stand_insane = { FRAME_stand65, FRAME_stand94, insane_frames_stand_insane, insane_stand };

mframe_t insane_frames_uptodown[] =
{
	MANIM_STAND,	0,		fix_bbox1,
	MANIM_STAND,	0,		fix_bbox2,
	MANIM_STAND,	0,		fix_bbox3,
	MANIM_STAND,	0,		fix_bbox4,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox5,
	MANIM_STAND,	0,		fix_bbox6,
	MANIM_STAND,	0,		insane_moan,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,

	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox7,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox8,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,

	MANIM_STAND,	2.7,	NULL,
	MANIM_STAND,	4.1,	NULL,
	MANIM_STAND,	6,		NULL,
	MANIM_STAND,	7.6,	fix_bbox9,
	MANIM_STAND,	3.6,	NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		insane_fist,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,

	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		insane_fist,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox10,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL
};
mmove_t insane_move_uptodown = { FRAME_stand1, FRAME_stand40, insane_frames_uptodown, insane_onground };


mframe_t insane_frames_downtoup[] =
{
	MANIM_STAND,	-0.7,	NULL,		// 41
	MANIM_STAND,	-1.2,	NULL,		// 42
	MANIM_STAND,	-1.5,	NULL,		// 43
	MANIM_STAND,	-4.5,	NULL,		// 44
	MANIM_STAND,	-3.5,	NULL,		// 45
	MANIM_STAND,	-0.2,	NULL,		// 46
	MANIM_STAND,	0,		NULL,		// 47
	MANIM_STAND,	-1.3,	NULL,		// 48
	MANIM_STAND,	-3,		fix_bbox11,	// 49
	MANIM_STAND,	-2,		NULL,		// 50
	MANIM_STAND,	0,		fix_bbox12,	// 51
	MANIM_STAND,	0,		fix_bbox13,	// 52
	MANIM_STAND,	0,		fix_bbox14,	// 53
	MANIM_STAND,	-3.3,	fix_bbox15,	// 54
	MANIM_STAND,	-1.6,	NULL,		// 55
	MANIM_STAND,	-0.3,	NULL,		// 56
	MANIM_STAND,	0,		NULL,		// 57
	MANIM_STAND,	0,		NULL,		// 58
	MANIM_STAND,	0,		NULL		// 59
};
mmove_t insane_move_downtoup = { FRAME_stand41, FRAME_stand59, insane_frames_downtoup, insane_stand };

mframe_t insane_frames_jumpdown[] =
{
	MANIM_WALK,	-1.8,	fix_bbox16,
	MANIM_WALK,	9.5,	fix_bbox25,
	MANIM_WALK,	3.1,	fix_bbox26,
	MANIM_WALK,	5.1,	fix_bbox10,
	MANIM_WALK,	-2,	NULL
};
mmove_t insane_move_jumpdown = { FRAME_stand96, FRAME_stand100, insane_frames_jumpdown, insane_onground };


mframe_t insane_frames_down[] =
{
	MANIM_STAND,	0,		NULL,			// 100
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox27,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox28,
	MANIM_STAND,	0,		NULL,			// 110
	MANIM_STAND,	-1.7,	fix_bbox29,
	MANIM_STAND,	-1.6,	fix_bbox30,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox28,
	MANIM_STAND,	0,		insane_fist,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,			// 120
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,			// 130
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		insane_moan,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,			// 140
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		fix_bbox31,			// 150
	MANIM_STAND,	0.5,	fix_bbox32,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	-0.2,	insane_scream,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0.2,	NULL,
	MANIM_STAND,	0.4,	NULL,
	MANIM_STAND,	0.6,	NULL,
	MANIM_STAND,	0.8,	fix_bbox33,
	MANIM_STAND,	0.7,	fix_bbox34,
	MANIM_STAND,	0,		insane_checkup	// 160
};
mmove_t insane_move_down = { FRAME_stand100, FRAME_stand160, insane_frames_down, insane_onground };

mframe_t insane_frames_walk_normal[] =
{
	MANIM_WALK,	-2,		insane_scream,
	MANIM_WALK,	0.5,	NULL,
	MANIM_WALK,	1.5,	NULL,
	MANIM_WALK,	-0.3,	NULL,
	MANIM_WALK,	0.3,	NULL,
	MANIM_WALK,	0.4,	NULL,
	MANIM_WALK,	0.2,	NULL,
	MANIM_WALK,	2.2,	NULL,
	MANIM_WALK,	3.6,	NULL,
	MANIM_WALK,	1.3,	NULL,
	MANIM_WALK,	0.4,	NULL,
	MANIM_WALK,	-1.1,	NULL,
	MANIM_WALK,	-2,		NULL
};
mmove_t insane_move_walk_normal = { FRAME_walk27, FRAME_walk39, insane_frames_walk_normal, insane_walk };
mmove_t insane_move_run_normal = { FRAME_walk27, FRAME_walk39, insane_frames_walk_normal, insane_run };

mframe_t insane_frames_walk_insane[] =
{
	MANIM_WALK,	-2,		insane_scream,		// walk 1
	MANIM_WALK,	1.4,	NULL,		// walk 2
	MANIM_WALK,	1.6,	NULL,		// 3
	MANIM_WALK,	0.9,	NULL,		// 4
	MANIM_WALK,	0.2,	NULL,		// 5
	MANIM_WALK,	0.6,	NULL,		// 6
	MANIM_WALK,	-2,		NULL,		// 7
	MANIM_WALK,	-1.3,	NULL,		// 8
	MANIM_WALK,	2.8,	NULL,		// 9
	MANIM_WALK,	3.3,	NULL,		// 10
	MANIM_WALK,	-0.9,	NULL,		// 11
	MANIM_WALK,	0,		NULL,		// 12
	MANIM_WALK,	-1.5,	fix_bbox35,	// 13
	MANIM_WALK,	-2,		fix_bbox36,	// 14
	MANIM_WALK,	-2,		NULL,		// 15
	MANIM_WALK,	2.9,	fix_bbox37,	// 16
	MANIM_WALK,	4.7,	fix_bbox38,	// 17
	MANIM_WALK,	1.8,	fix_bbox39,	// 18
	MANIM_WALK,	0,		NULL,		// 19
	MANIM_WALK,	-1.8,	fix_bbox40,	// 20
	MANIM_WALK,	-2,		fix_bbox41,	// 21
	MANIM_WALK,	1.4,	NULL,		// 22
	MANIM_WALK,	5.4,	NULL,		// 23
	MANIM_WALK,	3,		NULL,		// 24
	MANIM_WALK,	-0.2,	NULL,		// 25
	MANIM_WALK,	-2,		fix_bbox42	// 26
};
mmove_t insane_move_walk_insane = { FRAME_walk1, FRAME_walk26, insane_frames_walk_insane, insane_walk };
mmove_t insane_move_run_insane = { FRAME_walk1, FRAME_walk26, insane_frames_walk_insane, insane_run };

mframe_t insane_frames_stand_pain[] =
{
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL
};
mmove_t insane_move_stand_pain = { FRAME_st_pain2, FRAME_st_pain12, insane_frames_stand_pain, insane_run };

mframe_t insane_frames_stand_death[] =
{
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL
};
mmove_t insane_move_stand_death = { FRAME_st_death2, FRAME_st_death18, insane_frames_stand_death, insane_dead };

mframe_t insane_frames_crawl[] =
{
	MANIM_WALK,	-2,		insane_scream,
	MANIM_WALK,	-0.5,	NULL,
	MANIM_WALK,	0.1,	NULL,
	MANIM_WALK,	1.6,	NULL,
	MANIM_WALK,	0,		NULL,
	MANIM_WALK,	-1.1,	NULL,
	MANIM_WALK,	1,		NULL,
	MANIM_WALK,	1.4,	NULL,
	MANIM_WALK,	0.4,	NULL
};
mmove_t insane_move_crawl = { FRAME_crawl1, FRAME_crawl9, insane_frames_crawl, NULL };
mmove_t insane_move_runcrawl = { FRAME_crawl1, FRAME_crawl9, insane_frames_crawl, NULL };

mframe_t insane_frames_crawl_pain[] =
{
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		fix_bbox43,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		fix_bbox10
};
mmove_t insane_move_crawl_pain = { FRAME_cr_pain2, FRAME_cr_pain10, insane_frames_crawl_pain, insane_run };

mframe_t insane_frames_crawl_death[] =
{
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL,
	MANIM_DEATH,	0,		NULL
};
mmove_t insane_move_crawl_death = { FRAME_cr_death10, FRAME_cr_death16, insane_frames_crawl_death, insane_dead };

mframe_t insane_frames_cross[] =
{
	MANIM_STAND,	0,		insane_moan,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL,
	MANIM_STAND,	0,		NULL
};
mmove_t insane_move_cross = { FRAME_cross1, FRAME_cross15, insane_frames_cross, insane_cross };

mframe_t insane_frames_struggle_cross[] =
{
	MANIM_MISC,	0,		insane_scream,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL,
	MANIM_MISC,	0,		NULL
};
mmove_t insane_move_struggle_cross = { FRAME_cross16, FRAME_cross30, insane_frames_struggle_cross, insane_cross };

void insane_cross(edict_t* self)
{
	if (random() < 0.8)
		self->monsterinfo.currentmove = &insane_move_cross;
	else
		self->monsterinfo.currentmove = &insane_move_struggle_cross;
}

void insane_walk(edict_t* self)
{
	if (self->spawnflags & 16)			// Hold Ground?
		if (self->s.frame == FRAME_cr_pain10)
		{
			self->monsterinfo.currentmove = &insane_move_down;
			return;
		}
	if (self->spawnflags & 4)
		self->monsterinfo.currentmove = &insane_move_crawl;
	else
		if (random() <= 0.5)
			self->monsterinfo.currentmove = &insane_move_walk_normal;
		else
			self->monsterinfo.currentmove = &insane_move_walk_insane;
}

void insane_run(edict_t* self)
{
	if (self->spawnflags & 16)			// Hold Ground?
		if (self->s.frame == FRAME_cr_pain10)
		{
			self->monsterinfo.currentmove = &insane_move_down;
			return;
		}
	if (self->spawnflags & 4)				// Crawling?
		self->monsterinfo.currentmove = &insane_move_runcrawl;
	else
		if (random() <= 0.5)				// Else, mix it up
			self->monsterinfo.currentmove = &insane_move_run_normal;
		else
			self->monsterinfo.currentmove = &insane_move_run_insane;
}


void insane_pain(edict_t* self, edict_t* other, float kick, int damage)
{
	int	l, r;

	//	if (self->gib_health < (self->max_gib_health / 2))
	//		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + PAINTIME;

	r = 1 + (rand() & 1);
	if (self->health < 25)
		l = 25;
	else if (self->health < 50)
		l = 50;
	else if (self->health < 75)
		l = 75;
	else
		l = 100;
	gi.sound(self, CHAN_VOICE, gi.soundindex(va("player/male/pain%i_%i.wav", l, r)), 1, ATTN_IDLE, 0);

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	// Don't go into pain frames if crucified.
	if (self->spawnflags & 8)
	{
		self->monsterinfo.currentmove = &insane_move_struggle_cross;
		return;
	}

	if (((self->s.frame >= FRAME_crawl1) && (self->s.frame <= FRAME_crawl9)) || ((self->s.frame >= FRAME_stand99) && (self->s.frame <= FRAME_stand160)))
	{
		self->monsterinfo.currentmove = &insane_move_crawl_pain;
	}
	else
		self->monsterinfo.currentmove = &insane_move_stand_pain;

}

void insane_onground(edict_t* self)
{
	self->monsterinfo.currentmove = &insane_move_down;
}

void insane_checkdown(edict_t* self)
{
	//	if ( (self->s.frame == FRAME_stand94) || (self->s.frame == FRAME_stand65) )
	if (self->spawnflags & 32)				// Always stand
		return;
	if (random() < 0.3)
		if (random() < 0.5)
			self->monsterinfo.currentmove = &insane_move_uptodown;
		else
			self->monsterinfo.currentmove = &insane_move_jumpdown;
}

void insane_checkup(edict_t* self)
{
	// If Hold_Ground and Crawl are set
	if ((self->spawnflags & 4) && (self->spawnflags & 16))
		return;
	if (random() < 0.5)
		self->monsterinfo.currentmove = &insane_move_downtoup;

}

void insane_stand(edict_t* self)
{
	if (self->spawnflags & 8)			// If crucified
	{
		self->monsterinfo.currentmove = &insane_move_cross;
		self->monsterinfo.aiflags |= AI_STAND_GROUND;
	}
	// If Hold_Ground and Crawl are set
	else if ((self->spawnflags & 4) && (self->spawnflags & 16))
		self->monsterinfo.currentmove = &insane_move_down;
	else
		if (random() < 0.5)
			self->monsterinfo.currentmove = &insane_move_stand_normal;
		else
			self->monsterinfo.currentmove = &insane_move_stand_insane;
}

void insane_dead(edict_t* self)
{
	self->flags |= FL_NO_KNOCKBACK;

	if (self->spawnflags & 8)
	{
		self->flags |= FL_FLY;
	}
	else
	{
		VectorSet(self->mins, -16, -16, -24);
		VectorSet(self->maxs, 16, 16, -8);
		self->movetype = MOVETYPE_TOSS;
	}
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity(self);
}


void insane_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	int		n;

	if (self->gib_health <= 0)
	{
		gi.sound(self, CHAN_VOICE, gi.soundindex("misc/udeath.wav"), 1, ATTN_IDLE, 0);
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

	gi.sound(self, CHAN_VOICE, gi.soundindex(va("player/male/death%i.wav", (rand() % 4) + 1)), 1, ATTN_IDLE, 0);

	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	if (self->spawnflags & 8)
	{
		insane_dead(self);
	}
	else
	{
		if (((self->s.frame >= FRAME_crawl1) && (self->s.frame <= FRAME_crawl9)) || ((self->s.frame >= FRAME_stand99) && (self->s.frame <= FRAME_stand160)))
			self->monsterinfo.currentmove = &insane_move_crawl_death;
		else
			self->monsterinfo.currentmove = &insane_move_stand_death;
	}
}


/*QUAKED misc_insane (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn CRAWL CRUCIFIED STAND_GROUND ALWAYS_STAND
*/
void SP_misc_insane(edict_t* self)
{
	//	static int skin = 0;	//@@

	if (deathmatch->value && deathmatch->value != 3 && !((int)(gibflags->value) & GF_WITH_MONSTERS))
	{
		G_FreeEdict(self);
		return;
	}

	sound_fist = gi.soundindex("insane/insane11.wav");
	sound_shake = gi.soundindex("insane/insane5.wav");
	sound_moan = gi.soundindex("insane/insane7.wav");
	sound_scream[0] = gi.soundindex("insane/insane1.wav");
	sound_scream[1] = gi.soundindex("insane/insane2.wav");
	sound_scream[2] = gi.soundindex("insane/insane3.wav");
	sound_scream[3] = gi.soundindex("insane/insane4.wav");
	sound_scream[4] = gi.soundindex("insane/insane6.wav");
	sound_scream[5] = gi.soundindex("insane/insane8.wav");
	sound_scream[6] = gi.soundindex("insane/insane9.wav");
	sound_scream[7] = gi.soundindex("insane/insane10.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/insane/tris.md2");

	VectorSet(self->mins, -16, -16, -24);
	VectorSet(self->maxs, 16, 16, 28);
	VectorSet(self->headmins, -16, -16, 18);
	VectorSet(self->headmaxs, 16, 16, 28);

	self->health = 100;
	self->gib_health = 150;
	self->mass = 300;

	self->pain = insane_pain;
	self->die = insane_die;

	self->monsterinfo.stand = insane_stand;
	self->monsterinfo.walk = insane_walk;
	self->monsterinfo.run = insane_run;
	self->monsterinfo.aiflags |= AI_GOOD_GUY;

	self->monsterinfo.speed = 2;
	self->monsterinfo.walkspeed = 2;

	//@@
	//	self->s.skinnum = skin;
	//	skin++;
	//	if (skin > 12)
	//		skin = 0;

	gi.linkentity(self);

	if (self->spawnflags & 16)				// Stand Ground
		self->monsterinfo.aiflags |= AI_STAND_GROUND;

	self->monsterinfo.currentmove = &insane_move_stand_normal;

	self->monsterinfo.scale = MODEL_SCALE;

	if (self->spawnflags & 8)					// Crucified ?
	{
		VectorSet(self->mins, -16, 0, 0);
		VectorSet(self->maxs, 16, 8, 32);
		VectorSet(self->headmins, -6, 0, 26);
		VectorSet(self->headmaxs, 6, 8, 32);
		self->flags |= FL_NO_KNOCKBACK;
		flymonster_start(self);
	}
	else
	{
		walkmonster_start(self);
		self->s.skinnum = rand() % 3;
	}
}
