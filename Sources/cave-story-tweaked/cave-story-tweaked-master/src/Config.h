// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

#include "Input.h"

enum
{
	BINDING_UP,
	BINDING_DOWN,
	BINDING_LEFT,
	BINDING_RIGHT,
	BINDING_OK,
	BINDING_CANCEL,
	BINDING_JUMP,
	BINDING_SHOT,
	BINDING_STRAFE,
	BINDING_ARMSREV,
	BINDING_ARMS,
	BINDING_ITEM,
	BINDING_MAP,
	BINDING_PAUSE,
	BINDING_TOTAL
};

typedef struct CONFIG_BINDING
{
	int keyboard;
	unsigned char controller;
} CONFIG_BINDING;

struct CONFIGDATA
{
	char proof[0x20];
	char font_name[0x40];
	unsigned char display_mode;
	BOOL b60fps;
	BOOL bVsync;
	BOOL bSmoothScrolling;
	unsigned char soundtrack;
	CONFIG_BINDING bindings[BINDING_TOTAL];
	unsigned char font_select;
	BOOL bUseJoystick;

	// Tweaks Mode
	BOOL bNoExpDrops;
	unsigned char damage_modifier;
	unsigned char autoFire;
	BOOL bInfiniteAmmo;
	BOOL bScreenShake;
	unsigned char defaultBooster;
	BOOL bInfiniteBooster;
	BOOL bGraceJumps;
	unsigned char color_filter;
	unsigned char flash_mode;
	unsigned char game_speed_multiplier;
	unsigned char boss_health_multiplier;
	BOOL bExtendedUI;
	unsigned char backgroundBrightness;
	unsigned char graphics_set;
};

extern const char* const gConfigName;
extern const char* const gProof;

BOOL LoadConfigData(CONFIGDATA *conf);
BOOL SaveConfigData(const CONFIGDATA *conf);
void DefaultConfigData(CONFIGDATA *conf);
