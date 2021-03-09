// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "MiniMap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Escape.h"
#include "Flags.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Pause.h"
#include "Stage.h"

void WriteMiniMapLine(int line)
{
	int x;
	unsigned char a;

	int my_x;
	int my_y;

	RECT rcLevel[4] = {
		{228, 82, 232, 86},
		{232, 82, 236, 86},
		{236, 82, 240, 86},
		{240, 82, 244, 86},
	};

	my_x = ((gMC.x / 0x200) + 8) / 16;
	my_y = ((gMC.y / 0x200) + 8) / 16;

	for (x = 0; x < gMap.width; ++x)
	{
		a = GetAttribute(x, line);

		// Yup, this really is an if/else chain.
		// No switch here.
		if (a == 0)
			Surface2Surface(x*4, line*4, &rcLevel[0], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
		else if (a == 68 ||
			a == 1 ||
			a == 64 ||
			a == 128 ||
			a == 129 ||
			a == 130 ||
			a == 131 ||
			a == 81 ||
			a == 82 ||
			a == 85 ||
			a == 86 ||
			a == 2 ||
			a == 96 ||
			a == 113 ||
			a == 114 ||
			a == 117 ||
			a == 118 ||
			a == 160 ||
			a == 161 ||
			a == 162 ||
			a == 163)
			Surface2Surface(x*4, line*4, &rcLevel[1], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
		else if (a == 67 ||
			a == 99 ||
			a == 80 ||
			a == 83 ||
			a == 84 ||
			a == 87 ||
			// a == 96 ||	// This is already listed above, so that part of the expression is always false
			a == 112 ||
			a == 115 ||
			a == 116 ||
			a == 119)
			Surface2Surface(x, line, &rcLevel[2], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
		else
			Surface2Surface(x, line, &rcLevel[3], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
	}
}

int WriteMiniMap(int x, int y){
	unsigned char a;

	RECT rcLevel[9] = {
		// tileset
		{212, 103, 220, 110},
		{220, 103, 227, 110},
		{228, 103, 235, 110},
		{236, 103, 243, 110},
		// exits,
		{236, 111, 243, 118},
		// items,
		{228, 111, 235, 118},
		// save disk
		{236, 119, 243, 126},
		// life station
		{228, 119, 235, 126},
		//blank tile
		{212, 111, 219, 118}
	};

	for(int m_y = y - 11; m_y <= y + 11; m_y ++){
		for(int m_x = x - 16; m_x <= x + 16; m_x ++){
			int mini_x = m_x - (x - 16);
			int mini_y = m_y - (y - 11);

			if(m_x < 0 || m_x >= gMap.width || m_y < 0 || m_y >= gMap.length){
				Surface2Surface(mini_x * 7, mini_y * 7, &rcLevel[8], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
				continue;
			}

			a = GetAttribute(m_x, m_y);

			// Yup, this really is an if/else chain.
			// No switch here.
			if (a == 0)
				Surface2Surface(mini_x * 7, mini_y * 7, &rcLevel[0], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			else if (a == 68 ||
				a == 1 ||
				a == 64 ||
				a == 69 ||
				a == 128 ||
				a == 129 ||
				a == 130 ||
				a == 131 ||
				a == 81 ||
				a == 82 ||
				a == 85 ||
				a == 86 ||
				a == 2 ||
				a == 96 ||
				a == 113 ||
				a == 114 ||
				a == 117 ||
				a == 118 ||
				a == 160 ||
				a == 161 ||
				a == 162 ||
				a == 163)
				Surface2Surface(mini_x * 7, mini_y * 7, &rcLevel[1], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			else if (a == 67 ||
				a == 99 ||
				a == 80 ||
				a == 83 ||
				a == 84 ||
				a == 87 ||
				a == 96 ||	// This is already listed above, so that part of the expression is always false
				a == 112 ||
				a == 115 ||
				a == 116 ||
				a == 119)
				Surface2Surface(mini_x * 7, mini_y * 7, &rcLevel[2], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			else
				Surface2Surface(mini_x * 7, mini_y * 7, &rcLevel[3], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);

		}
	}

	for(int i = 0; i < NPC_MAX; i++){
		//doors
		if(gNPC[i].code_char == 0x12 || gNPC[i].code_char == 0x89){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - 11 && npc_y <= y + 11 && npc_x >= x - 16 && npc_x <= x + 16){
				int npc_max_y = npc_y - (y - 11);
				int npc_map_x = npc_x - (x - 16);

				Surface2Surface(npc_map_x * 7, npc_max_y * 7, &rcLevel[4], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);

				if(gNPC[i].code_char == 0x89){ //large door is wide
					Surface2Surface((npc_map_x + 1) * 7, npc_max_y * 7, &rcLevel[4], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
				}
			}
		}

		//invisible doors
		if(gNPC[i].code_char == 0x0 && gNPC[i].code_event >= 100 && gNPC[i].code_event <= 120 && (gNPC[i].bits & NPC_INTERACTABLE)){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			if(npc_y >= y - 11 && npc_y <= y + 11 && npc_x >= x - 16 && npc_x <= x + 16){
				int npc_max_y = npc_y - (y - 11);
				int npc_map_x = npc_x - (x - 16);

				Surface2Surface(npc_map_x * 7, npc_max_y * 7, &rcLevel[4], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
		}

		//chests, items, life capsules
		if(gNPC[i].code_char == 0xF || gNPC[i].code_char == 0x20 || gNPC[i].code_char == 0x46){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			// so many unused chests what
			if(gNPC[i].code_char == 0xF && gNPC[i].code_flag == 151 && gNPC[i].bits & NPC_APPEAR_WHEN_FLAG_SET){
				continue;
			}

			if(gNPC[i].code_flag == 410){
				continue;
			}

			// hide items if already collected
			if(gNPC[i].bits & NPC_HIDE_WHEN_FLAG_SET && (GetNPCFlag(gNPC[i].code_flag))){
				continue;
			}

			// hide items if not yet spawned
			if(gNPC[i].bits & NPC_APPEAR_WHEN_FLAG_SET && !(GetNPCFlag(gNPC[i].code_flag))){
				continue;
			}

			if(npc_y >= y - 11 && npc_y <= y + 11 && npc_x >= x - 16 && npc_x <= x + 16){
				int npc_max_y = npc_y - (y - 11);
				int npc_map_x = npc_x - (x - 16);

				Surface2Surface(npc_map_x * 7, npc_max_y * 7, &rcLevel[5], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
		}

		// save disk
		if(gNPC[i].code_char == 0x10){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			// hide if not yet spawned
			if(gNPC[i].bits & NPC_APPEAR_WHEN_FLAG_SET && !(GetNPCFlag(gNPC[i].code_flag))){
				continue;
			}

			//discard unused save scripts
			if(gNPC[i].code_event != 16){
				continue;
			}

			if(npc_y >= y - 11 && npc_y <= y + 11 && npc_x >= x - 16 && npc_x <= x + 16){
				int npc_max_y = npc_y - (y - 11);
				int npc_map_x = npc_x - (x - 16);

				Surface2Surface(npc_map_x * 7, npc_max_y * 7, &rcLevel[6], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
		}

		// life capsule
		if(gNPC[i].code_char == 0x11){
			int npc_x = gNPC[i].x / (0x10*0x200);
			int npc_y = gNPC[i].y / (0x10*0x200);

			// hide if not yet spawned
			if(gNPC[i].bits & NPC_APPEAR_WHEN_FLAG_SET && !(GetNPCFlag(gNPC[i].code_flag))){
				continue;
			}

			if(npc_y >= y - 11 && npc_y <= y + 11 && npc_x >= x - 16 && npc_x <= x + 16){
				int npc_max_y = npc_y - (y - 11);
				int npc_map_x = npc_x - (x - 16);

				Surface2Surface(npc_map_x * 7, npc_max_y * 7, &rcLevel[7], SURFACE_ID_MAP, SURFACE_ID_TEXT_BOX);
			}
		}
	}

	return enum_ESCRETURN_continue;
}

int MiniMapLoop(void)
{
	int f;
	RECT rcView;
	RECT rcMiniMap;

	int my_x;
	int my_y;
	unsigned char my_wait;
	RECT my_rect = {220, 111, 225, 116};
	RECT rcMapCaret = {0, 0, 253, 203};

	my_x = ((gMC.x / 0x200) + 8) / 16;
	my_y = ((gMC.y / 0x200) + 8) / 16;

	for (f = 0; f <= 8; ++f)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause(1))
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_MENU_OVERLAY);

		rcView.left = (WINDOW_WIDTH / 2) - (((42 * f) / 8) / 2)*6;
		rcView.right = (WINDOW_WIDTH / 2) + (((42 * f) / 8) / 2)*6;
		rcView.top = (WINDOW_HEIGHT / 2) - (((22.5 * f) / 8) / 2)*6;
		rcView.bottom = (WINDOW_HEIGHT / 2) + (((22.5 * f) / 8) / 2)*6;

		PutMapName(TRUE);
		CortBox(&rcView, 0);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	rcMiniMap.left = 0;
	rcMiniMap.right = 33*7;
	rcMiniMap.top = 0;
	rcMiniMap.bottom = 25*7;

	rcView.right = --rcView.left*7 + 2*7;
	rcView.bottom = --rcView.top*7 + 2*7;
	CortBox2(&rcMiniMap, 0, SURFACE_ID_MAP);

	int map_x = my_x;
	int map_y = my_y;

	my_wait = 0;
	while (1)
	{
		GetTrg();

		if (gKeyTrg & (gKeyOk | gKeyCancel))
			break;

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause(1))
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if(my_wait % 3 == 0){
			if(gKey & KEY_UP){
				map_y = map_y - 1;
			}

			if(gKey & KEY_DOWN){
				map_y = map_y + 1;
			}

			if(gKey & KEY_LEFT){
				map_x = map_x - 1;
			}

			if(gKey & KEY_RIGHT){
				map_x = map_x + 1;
			}
		}
		
		if(map_x - 10 < 0){
			map_x = 10;
		}
		
		if(map_x + 10 >= gMap.width){
			map_x = gMap.width - 10;
		}

		if(map_y - 7 < 0){
			map_y = 7;
		}
		
		if(map_y + 7 > gMap.length){
			map_y = gMap.length - 7;
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		PutBitmap3(&grcGame, 0, 0, &grcGame, SURFACE_ID_MENU_OVERLAY);
		// CortBox(&rcView, 0);

		WriteMiniMap(map_x, map_y);

		PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 110), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 84), &rcMiniMap, SURFACE_ID_MAP);
		PutBitmap3(&grcGame, PixelToScreenCoord((WINDOW_WIDTH / 2) - 110), PixelToScreenCoord((WINDOW_HEIGHT / 2) - 107), &rcMapCaret, SURFACE_ID_MAP_CARET);

		PutMapName(TRUE);

		if (++my_wait / 16 % 2){
			int my_char_map_x = WINDOW_WIDTH/2 + 3 - 7*(map_x - my_x);
			int my_char_map_y = WINDOW_HEIGHT/2 - 6 - 7* (map_y - my_y);
			if(my_char_map_x >= (WINDOW_WIDTH / 2) - 119 && my_char_map_x <= (WINDOW_WIDTH / 2) - 122 + 42*5 + 21){		
				if(my_char_map_y >= (WINDOW_HEIGHT / 2) - 76 && my_char_map_y <= (WINDOW_HEIGHT / 2) - 82 + 23*7){
					PutBitmap3(&grcGame, PixelToScreenCoord(my_char_map_x), PixelToScreenCoord(my_char_map_y), &my_rect, SURFACE_ID_TEXT_BOX);
				}
			}

		}
			
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	for (f = 8; f >= -1; --f)
	{
		GetTrg();

		if (gKey & KEY_PAUSE)
		{
			switch (Call_Pause(1))
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		if (gKey & KEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);

		rcView.left = (WINDOW_WIDTH / 2) - (((gMap.width * f) / 8) / 2);
		rcView.right = (WINDOW_WIDTH / 2) + (((gMap.width * f) / 8) / 2);
		rcView.top = (WINDOW_HEIGHT / 2) - (((gMap.length * f) / 8) / 2);
		rcView.bottom = (WINDOW_HEIGHT / 2) + (((gMap.length * f) / 8) / 2);

		PutMapName(TRUE);
		CortBox(&rcView, 0);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;
	}

	return enum_ESCRETURN_continue;
}

signed char gMapping[0x80];

BOOL IsMapping(void)
{
	if (!gMapping[gStageNo])
		return FALSE;
	else
		return TRUE;
}

void StartMapping(void)
{
	memset(gMapping, FALSE, sizeof(gMapping));
}

void SetMapping(int a)
{
	gMapping[a] = TRUE;
}
