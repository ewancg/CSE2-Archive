
#include "Pause.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "Back.h"
#include "Backends/Controller.h"
#include "Backends/Misc.h"
#include "CommonDefines.h"
#include "Config.h"
#include "Bitmap.h"
#include "Boss.h"
#include "Draw.h"
#include "Escape.h"
#include "Flash.h"
#include "Game.h"
#include "Generic.h"
#include "KeyControl.h"
#include "Main.h"
#include "Organya.h"
#include "Sound.h"
#include "Stage.h"
#include "NpcHit.h"
#include "MyChar.h"
#include "MycHit.h"
#include "Shoot.h"
#include "ArmsItem.h"
#include "Frame.h"
#include "GenericLoad.h"

#define MAX_OPTIONS ((WINDOW_HEIGHT / 20) - 4)	// The maximum number of options we can fit on-screen at once

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

enum
{
	CALLBACK_CONTINUE = -1,
	CALLBACK_PREVIOUS_MENU = -2,
	CALLBACK_RESET = -3,
	CALLBACK_EXIT = -4,
};

typedef enum CallbackAction
{
	ACTION_INIT,
	ACTION_DEINIT,
	ACTION_UPDATE,
	ACTION_OK,
	ACTION_LEFT,
	ACTION_RIGHT
} CallbackAction;

typedef struct Option
{
	const char *name;
	int (*callback)(struct OptionsMenu *parent_menu, size_t this_option, CallbackAction action);
	void *user_data;
	const char *value_string;
	long value;
	BOOL disabled;
	long attribute_size;
	long attribute_index;
} Option;

typedef struct OptionsMenu
{
	const char *title;
	const char *subtitle;
	struct Option *options;
	size_t total_options;
	int x_offset;
	BOOL submenu;
} OptionsMenu;

static BOOL restart_required;
static BOOL tweaks_restart_required;

static const RECT rcMyChar[4] = {
	{0, 16, 16, 32},
	{16, 16, 32, 32},
	{0, 16, 16, 32},
	{32, 16, 48, 32},
};

unsigned int bMode;

static const char* GetKeyName(int key)
{
	switch (key)
	{
		case BACKEND_KEYBOARD_A:
			return "A";

		case BACKEND_KEYBOARD_B:
			return "B";

		case BACKEND_KEYBOARD_C:
			return "C";

		case BACKEND_KEYBOARD_D:
			return "D";

		case BACKEND_KEYBOARD_E:
			return "E";

		case BACKEND_KEYBOARD_F:
			return "F";

		case BACKEND_KEYBOARD_G:
			return "G";

		case BACKEND_KEYBOARD_H:
			return "H";

		case BACKEND_KEYBOARD_I:
			return "I";

		case BACKEND_KEYBOARD_J:
			return "J";

		case BACKEND_KEYBOARD_K:
			return "K";

		case BACKEND_KEYBOARD_L:
			return "L";

		case BACKEND_KEYBOARD_M:
			return "M";

		case BACKEND_KEYBOARD_N:
			return "N";

		case BACKEND_KEYBOARD_O:
			return "O";

		case BACKEND_KEYBOARD_P:
			return "P";

		case BACKEND_KEYBOARD_Q:
			return "Q";

		case BACKEND_KEYBOARD_R:
			return "R";

		case BACKEND_KEYBOARD_S:
			return "S";

		case BACKEND_KEYBOARD_T:
			return "T";

		case BACKEND_KEYBOARD_U:
			return "U";

		case BACKEND_KEYBOARD_V:
			return "V";

		case BACKEND_KEYBOARD_W:
			return "W";

		case BACKEND_KEYBOARD_X:
			return "X";

		case BACKEND_KEYBOARD_Y:
			return "Y";

		case BACKEND_KEYBOARD_Z:
			return "Z";

		case BACKEND_KEYBOARD_0:
			return "0";

		case BACKEND_KEYBOARD_1:
			return "1";

		case BACKEND_KEYBOARD_2:
			return "2";

		case BACKEND_KEYBOARD_3:
			return "3";

		case BACKEND_KEYBOARD_4:
			return "4";

		case BACKEND_KEYBOARD_5:
			return "5";

		case BACKEND_KEYBOARD_6:
			return "6";

		case BACKEND_KEYBOARD_7:
			return "7";

		case BACKEND_KEYBOARD_8:
			return "8";

		case BACKEND_KEYBOARD_9:
			return "9";

		case BACKEND_KEYBOARD_F1:
			return "F1";

		case BACKEND_KEYBOARD_F2:
			return "F2";

		case BACKEND_KEYBOARD_F3:
			return "F3";

		case BACKEND_KEYBOARD_F4:
			return "F4";

		case BACKEND_KEYBOARD_F5:
			return "F5";

		case BACKEND_KEYBOARD_F6:
			return "F6";

		case BACKEND_KEYBOARD_F7:
			return "F7";

		case BACKEND_KEYBOARD_F8:
			return "F8";

		case BACKEND_KEYBOARD_F9:
			return "F9";

		case BACKEND_KEYBOARD_F10:
			return "F10";

		case BACKEND_KEYBOARD_F11:
			return "F11";

		case BACKEND_KEYBOARD_F12:
			return "F12";

		case BACKEND_KEYBOARD_UP:
			return "Up";

		case BACKEND_KEYBOARD_DOWN:
			return "Down";

		case BACKEND_KEYBOARD_LEFT:
			return "Left";

		case BACKEND_KEYBOARD_RIGHT:
			return "Right";

		case BACKEND_KEYBOARD_ESCAPE:
			return "Escape";

		case BACKEND_KEYBOARD_BACK_QUOTE:
			return "`";

		case BACKEND_KEYBOARD_TAB:
			return "Tab";

		case BACKEND_KEYBOARD_CAPS_LOCK:
			return "Caps Lock";

		case BACKEND_KEYBOARD_LEFT_SHIFT:
			return "Left Shift";

		case BACKEND_KEYBOARD_LEFT_CTRL:
			return "Left Ctrl";

		case BACKEND_KEYBOARD_LEFT_ALT:
			return "Left Alt";

		case BACKEND_KEYBOARD_SPACE:
			return "Space";

		case BACKEND_KEYBOARD_RIGHT_ALT:
			return "Right Alt";

		case BACKEND_KEYBOARD_RIGHT_CTRL:
			return "Right Ctrl";

		case BACKEND_KEYBOARD_RIGHT_SHIFT:
			return "Right Shift";

		case BACKEND_KEYBOARD_ENTER:
			return "Enter";

		case BACKEND_KEYBOARD_BACKSPACE:
			return "Backspace";

		case BACKEND_KEYBOARD_MINUS:
			return "-";

		case BACKEND_KEYBOARD_EQUALS:
			return "=";

		case BACKEND_KEYBOARD_LEFT_BRACKET:
			return "[";

		case BACKEND_KEYBOARD_RIGHT_BRACKET:
			return "]";

		case BACKEND_KEYBOARD_BACK_SLASH:
			return "\\";

		case BACKEND_KEYBOARD_SEMICOLON:
			return ";";

		case BACKEND_KEYBOARD_APOSTROPHE:
			return "'";

		case BACKEND_KEYBOARD_COMMA:
			return ",";

		case BACKEND_KEYBOARD_PERIOD:
			return ".";

		case BACKEND_KEYBOARD_FORWARD_SLASH:
			return "/";
	}

	return "Unknown";
}

static void PutTextCentred(int x, int y, const char *text, unsigned long color)
{
	size_t string_width = 0;
	size_t string_height = font_height;

	for (const char *text_pointer = text; *text_pointer != '\0';)
	{
		if (IsShiftJIS(*text_pointer))
		{
			text_pointer += 2;
			string_width += font_width * 2;
		}
		else
		{
			text_pointer += 1;
			string_width += font_width/2;
		}
	}

	PutText(x - string_width / 2, y - string_height / 2, text, color);
}

static void PutTitleTextCentered(int x, int y, const char *text, unsigned long color)
{
	size_t string_width = 0;
	size_t string_height = title_height;

	for (const char *text_pointer = text; *text_pointer != '\0';)
	{
		if (IsShiftJIS(*text_pointer))
		{
			text_pointer += 2;
			string_width += font_width * 2;
		}
		else
		{
			text_pointer += 1;
			string_width += font_width;
		}
	}

	PutTitleText(x - string_width / 2, y - string_height / 2, text, color);
}


static int EnterOptionsMenu(OptionsMenu *options_menu, size_t selected_option)
{
	int scroll = 0;
	int frame_x;
	int frame_y;

	unsigned int anime = 0;

	int return_value;

	// Initialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback(options_menu, i, ACTION_INIT);

	RECT rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	for (;;)
	{
		// Get pressed keys
		GetTrg();

		// Allow unpausing by pressing the pause button only when in the main pause menu (not submenus)
		if (!options_menu->submenu && gKeyTrg & KEY_PAUSE)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Go back one submenu when the 'cancel' button is pressed
		if (gKeyTrg & gKeyCancel)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Handling up/down input
		if (gKeyTrg & (gKeyUp | gKeyDown))
		{
			const size_t old_selection = selected_option;

			if (gKeyTrg & gKeyDown)
				if (selected_option++ == options_menu->total_options - 1)
					selected_option = 0;

			if (gKeyTrg & gKeyUp)
				if (selected_option-- == 0)
					selected_option = options_menu->total_options - 1;

			// Update the menu-scrolling, if there are more options than can be fit on the screen
			if (selected_option < old_selection)
				scroll = MAX(0, MIN(scroll, (int)selected_option - 1));

			if (selected_option > old_selection)
				scroll = MIN(MAX(0, (int)options_menu->total_options - MAX_OPTIONS), MAX(scroll, (int)selected_option - (MAX_OPTIONS - 2)));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);
		}

		// Run option callbacks
		for (size_t i = 0; i < options_menu->total_options; ++i)
		{
			if (!options_menu->options[i].disabled)
			{
				CallbackAction action = ACTION_UPDATE;

				if (i == selected_option)
				{
					if (gKeyTrg & gKeyOk)
						action = ACTION_OK;
					else if (gKeyTrg & gKeyLeft)
						action = ACTION_LEFT;
					else if (gKeyTrg & gKeyRight)
						action = ACTION_RIGHT;
				}

				return_value = options_menu->options[i].callback(options_menu, i, action);

				// If the callback returned something other than CALLBACK_CONTINUE, it's time to exit this submenu
				if (return_value != CALLBACK_CONTINUE)
					break;
			}
		}

		if (return_value != CALLBACK_CONTINUE)
			break;

		// Update Quote animation counter
		if (++anime >= 80)
			anime = 0;

		// Draw screen
		if (bMode == 1) { // Action Mode
			PutBitmap3(&rcView, 0, 0, &pauseView, SURFACE_ID_SCREEN_GRAB);
			PutBitmap3(&rcView, 0, 0, &pauseView, SURFACE_ID_MENU_OVERLAY);
		}
		else if (bMode == 2){
			CortBox(&rcView,0x202020);
		}
		else{
			if(anime % 2 == 0)
				ActBack();
			
			GetFramePosition(&frame_x, &frame_y);
			PutBack(frame_x, frame_y);
			PutBitmap3(&rcView, 0, 0, &pauseView, SURFACE_ID_MENU_OVERLAY);
		}

		const size_t visible_options = MIN(MAX_OPTIONS, options_menu->total_options);

		int y = (WINDOW_HEIGHT / 2) - ((visible_options * 20) / 2) - (40 / 2);

		// Draw title
		PutTitleTextCentered(WINDOW_WIDTH / 2, y, options_menu->title, RGB(0x73, 0x73, 0x73));

		// Draw subtitle
		if (options_menu->subtitle != NULL)
			PutTextCentred(WINDOW_WIDTH / 2, y + 14, options_menu->subtitle, RGB(0xFF, 0xFF, 0xFF));

		y += 40;

		RECT top_arrow_view = {181,124,195,138};
		RECT bottom_arrow_view = {196,124,210,138};
		

		if(scroll != 0){
			PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) + options_menu->x_offset - 19), PixelToScreenCoord( (WINDOW_HEIGHT / 2) + (10 * 1) - (((visible_options - 1) * 20) / 2) - 22 - abs(static_cast<int>(anime / 20 % 4 - 1))), &top_arrow_view, SURFACE_ID_CARET);
		}

		if(scroll + visible_options < options_menu->total_options && options_menu->total_options > MAX_OPTIONS){
			PutBitmap3(&rcView, PixelToScreenCoord((WINDOW_WIDTH / 2) + options_menu->x_offset - 19), PixelToScreenCoord((WINDOW_HEIGHT / 2) + (10 * 1) - (((visible_options - 1) * 20) / 2) + 133 + abs(static_cast<int>(anime / 20 % 4 - 1))), &bottom_arrow_view, SURFACE_ID_CARET);
		}

		for (size_t i = scroll; i < scroll + visible_options; ++i)
		{
			const int x = (WINDOW_WIDTH / 2) + options_menu->x_offset;
			const int y = (WINDOW_HEIGHT / 2) + (10 * 1) - (((visible_options - 1) * 20) / 2) + ((i - scroll) * 20) - 10;

			// Draw Quote next to the selected option
			if (i == selected_option)
				PutBitmap3(&rcView, PixelToScreenCoord(x - 20), PixelToScreenCoord(y - 6), &rcMyChar[anime / 10 % 4], SURFACE_ID_MY_CHAR);

			unsigned long option_colour = options_menu->options[i].disabled ? RGB(0x80, 0x80, 0x80) : (i == selected_option? RGB(0xF5, 0xFE, 0x56): RGB(0xF7, 0xF7, 0xEA));

			// Draw option name
			PutText(x, y - font_height / 2, options_menu->options[i].name, option_colour);

			// Draw option value, if it has one
			if (options_menu->options[i].value_string != NULL){
				const char* attribute_format = "%s %s %s";
				const char* left_move_arrow = options_menu->options[i].attribute_index > 0 ? "<" : " ";
				const char* right_move_arrow = options_menu->options[i].attribute_index < options_menu->options[i].attribute_size -1? ">" : " ";
				
				char attribute_str[80] = { };
				sprintf(attribute_str, attribute_format, left_move_arrow, options_menu->options[i].value_string, right_move_arrow);
				
				PutText(x + 130, y - (font_height / 2), attribute_str, option_colour);
			}
		}

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return_value = CALLBACK_EXIT;
			break;
		}
	}

	// Deinitialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback(options_menu, i, ACTION_DEINIT);

	return return_value;
}

///////////////////
// Controls menu //
///////////////////

typedef struct Control
{
	const char *name;
	size_t binding_index;
	unsigned char groups;
} Control;

// The bitfield on the right determines which 'group' the
// control belongs to - if two controls are in the same group,
// they cannot be bound to the same key.
static const Control controls[] = {
	{menu_strings_table[CONTROLS_UP],              	BINDING_UP,     (1 << 0) | (1 << 1)},
	{menu_strings_table[CONTROLS_DOWN],            	BINDING_DOWN,   (1 << 0) | (1 << 1)},
	{menu_strings_table[CONTROLS_LEFT],            	BINDING_LEFT,   (1 << 0) | (1 << 1)},
	{menu_strings_table[CONTROLS_RIGHT],           	BINDING_RIGHT,  (1 << 0) | (1 << 1)},
	{menu_strings_table[CONTROLS_OK],              	BINDING_OK,      1 << 1},
	{menu_strings_table[CONTROLS_CANCEL],          	BINDING_CANCEL,  1 << 1},
	{menu_strings_table[CONTROLS_JUMP],            	BINDING_JUMP,    1 << 0},
	{menu_strings_table[CONTROLS_SHOOT],           	BINDING_SHOT,    1 << 0},
	{menu_strings_table[CONTROLS_NEXT_WEAPON],     	BINDING_ARMS,    1 << 0},
	{menu_strings_table[CONTROLS_PREV_WEAPON],		BINDING_ARMSREV, 1 << 0},
	{menu_strings_table[CONTROLS_INVENTORY],		BINDING_ITEM,    1 << 0},
	{menu_strings_table[CONTROLS_MAP],             	BINDING_MAP,     1 << 0},
	{menu_strings_table[CONTROLS_PAUSE],           	BINDING_PAUSE,   1 << 0},
	{menu_strings_table[CONTROLS_STRAFE],          	BINDING_STRAFE,  1 << 0}

};

static char bound_name_buffers[sizeof(controls) / sizeof(controls[0])][20];

static int Callback_ControlsKeyboard_Rebind(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	switch (action)
	{
		default:
			break;

		case ACTION_INIT:
			strncpy(bound_name_buffers[this_option], GetKeyName(bindings[controls[this_option].binding_index].keyboard), sizeof(bound_name_buffers[0]) - 1);
			break;

		case ACTION_OK:
			PlaySoundObject(5, SOUND_MODE_PLAY);

			char timeout_string[2];
			timeout_string[1] = '\0';

			bool previous_keyboard_state[BACKEND_KEYBOARD_TOTAL];
			memcpy(previous_keyboard_state, gKeyboardState, sizeof(gKeyboardState));

			// Time-out and exit if the user takes too long (they probably don't want to rebind)
			for (unsigned int timeout = (60 * 5) - 1; timeout != 0; --timeout)
			{
				for (int scancode = 0; scancode < BACKEND_KEYBOARD_TOTAL; ++scancode)
				{
					// Wait for user to press a key
					if (!previous_keyboard_state[scancode] && gKeyboardState[scancode])
					{
						const char *key_name = GetKeyName(scancode);

						// If another control in the same group uses this key, swap them
						for (size_t other_option = 0; other_option < parent_menu->total_options; ++other_option)
						{
							if (other_option != this_option && controls[other_option].groups & controls[this_option].groups && bindings[controls[other_option].binding_index].keyboard == scancode)
							{
								bindings[controls[other_option].binding_index].keyboard = bindings[controls[this_option].binding_index].keyboard;
								memcpy(bound_name_buffers[other_option], bound_name_buffers[this_option], sizeof(bound_name_buffers[0]));
								break;
							}
						}

						// Otherwise just overwrite the selected control
						bindings[controls[this_option].binding_index].keyboard = scancode;
						strncpy(bound_name_buffers[this_option], key_name, sizeof(bound_name_buffers[0]) - 1);

						PlaySoundObject(18, SOUND_MODE_PLAY);

						gKeyTrg = gKey = 0;	// Prevent weird input-ghosting by doing this
						return CALLBACK_CONTINUE;
					}
				}

				memcpy(previous_keyboard_state, gKeyboardState, sizeof(gKeyboardState));

				// Draw screen
				CortBox(&grcFull, 0x000000);

				const char *string = menu_strings_table[CONTROLS_PUSH_KEY];
				PutTextCentred((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2) - 10, string, RGB(0xFF, 0xFF, 0xFF));
				PutTextCentred((WINDOW_WIDTH / 2) , (WINDOW_HEIGHT / 2) + 10, parent_menu->options[this_option].name, RGB(0xFF, 0xFF, 0xFF));

				timeout_string[0] = '0' + (timeout / 60) + 1;
				PutTextCentred(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 60, timeout_string, RGB(0xFF, 0xFF, 0xFF));

				PutFramePerSecound();

				if (!Flip_SystemTask())
				{
					// Quit if window is closed
					return CALLBACK_EXIT;
				}
			}

			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ControlsKeyboard(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	Option options[sizeof(controls) / sizeof(controls[0])];

	for (size_t i = 0; i < sizeof(controls) / sizeof(controls[0]); ++i)
	{
		options[i].name = controls[i].name;
		options[i].callback = Callback_ControlsKeyboard_Rebind;
		options[i].value_string = bound_name_buffers[i];
		options[i].disabled = FALSE;
		options[i].attribute_index = -1;
		options[i].attribute_size = 0;

		strncpy(bound_name_buffers[i], GetKeyName(bindings[controls[i].binding_index].keyboard), sizeof(bound_name_buffers[0]) - 1);
	}

	OptionsMenu options_menu = {
		menu_strings_table[CONTROLS_KEYBOARD_TITLE],
		NULL,
		options,
		sizeof(options) / sizeof(options[0]),
		-60,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	return return_value;
}

static int Callback_ControlsController_Rebind(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	switch (action)
	{
		default:
			break;

		case ACTION_INIT:
			// Name each bound button
			if (bindings[controls[this_option].binding_index].controller == 0xFF)
				strncpy(bound_name_buffers[this_option], menu_strings_table[CONTROLS_UNBOUND], sizeof(bound_name_buffers[0]));
			else
				strncpy(bound_name_buffers[this_option], ControllerBackend_GetButtonName(bindings[controls[this_option].binding_index].controller), sizeof(bound_name_buffers[0]));

			break;

		case ACTION_OK:
			PlaySoundObject(5, SOUND_MODE_PLAY);

			DIRECTINPUTSTATUS old_state = gJoystickState;

			char timeout_string[2];
			timeout_string[1] = '\0';

			// Time-out and exit if the user takes too long (they probably don't want to rebind)
			for (unsigned int timeout = (60 * 5) - 1; timeout != 0; --timeout)
			{
				for (int button = 0; button < sizeof(gJoystickState.bButton) / sizeof(gJoystickState.bButton[0]); ++button)
				{
					// Wait for user to press a button
					if (!old_state.bButton[button] && gJoystickState.bButton[button])
					{
						// If another control in the same group uses this button, swap them
						for (size_t other_option = 0; other_option < parent_menu->total_options; ++other_option)
						{
							if (other_option != this_option && controls[other_option].groups & controls[this_option].groups && bindings[controls[other_option].binding_index].controller == button)
							{
								bindings[controls[other_option].binding_index].controller = bindings[controls[this_option].binding_index].controller;
								memcpy(bound_name_buffers[other_option], bound_name_buffers[this_option], sizeof(bound_name_buffers[0]));
								break;
							}
						}

						// Otherwise just overwrite the selected control
						bindings[controls[this_option].binding_index].controller = button;
						strncpy(bound_name_buffers[this_option], ControllerBackend_GetButtonName(button), sizeof(bound_name_buffers[0]));

						PlaySoundObject(18, SOUND_MODE_PLAY);

						gKeyTrg = gKey = 0;	// Prevent weird input-ghosting by doing this
						return CALLBACK_CONTINUE;
					}
				}

				old_state = gJoystickState;

				// Draw screen
				CortBox(&grcFull, 0x000000);

				const char *string = menu_strings_table[CONTROLS_PUSH_BUTTON];
				PutTextCentred((WINDOW_WIDTH / 2) , (WINDOW_HEIGHT / 2) - 10, string, RGB(0xFF, 0xFF, 0xFF));
				PutTextCentred((WINDOW_WIDTH / 2) , (WINDOW_HEIGHT / 2) + 10, parent_menu->options[this_option].name, RGB(0xFF, 0xFF, 0xFF));

				timeout_string[0] = '0' + (timeout / 60) + 1;
				PutTextCentred(WINDOW_WIDTH, WINDOW_HEIGHT / 2 + 60, timeout_string, RGB(0xFF, 0xFF, 0xFF));

				PutFramePerSecound();

				if (!Flip_SystemTask())
				{
					// Quit if window is closed
					return CALLBACK_EXIT;
				}
			}

			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ControlsController(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	Option options[sizeof(controls) / sizeof(controls[0])];

	for (size_t i = 0; i < sizeof(controls) / sizeof(controls[0]); ++i)
	{
		options[i].name = controls[i].name;
		options[i].callback = Callback_ControlsController_Rebind;
		options[i].value_string = bound_name_buffers[i];
		options[i].disabled = FALSE;
		options[i].attribute_index = -1;
		options[i].attribute_size = 0;
	}

	OptionsMenu options_menu = {
	#if !defined(__WIIU__) && !defined(_3DS)
		menu_strings_table[CONTROLS_TITLE],
		NULL,
	#else
		menu_strings_table[CONTROLS_GAMEPAD_TITLE],
		ControllerBackend_GetControllerName(),
	#endif
		options,
		sizeof(options) / sizeof(options[0]),
		-70,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

/////////////////////
// Soundtrack menu //
/////////////////////

static int Callback_Soundtrack_Option(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	switch (action)
	{
		default:
			break;

		case ACTION_INIT:
			parent_menu->options[this_option].disabled = !CheckSoundtrackExists(this_option);
			break;

		case ACTION_OK:
			conf->soundtrack = gSoundtrack = this_option;

			// Restart the songs, with the new soundtrack setting
			MusicID current_song = gMusicNo;
			MusicID previous_song = gOldNo;
			gMusicNo = gOldNo = MUS_SILENCE;
			ChangeMusic(previous_song);
			ChangeMusic(current_song);

			return CALLBACK_PREVIOUS_MENU;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Soundtrack(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	const char *strings[] = {"Organya", "NES", "SNES", "Arranged", "New", "Remastered", "Famitracks", "Ridiculon"};

	Option options[sizeof(strings) / sizeof(strings[0])];

	for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); ++i)
	{
		options[i].name = strings[i];
		options[i].callback = Callback_Soundtrack_Option;
		options[i].user_data = conf;
		options[i].value_string = NULL;
	}

	OptionsMenu options_menu = {
		menu_strings_table[SELECT_SOUNDTRACK],
		"Check data/Soundtracks to obtain greyed-out soundtracks",
		options,
		sizeof(options) / sizeof(options[0]),
		-30,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, conf->soundtrack);

	// Check if we just want to go back to the previous menu
	if (return_value == CALLBACK_PREVIOUS_MENU)
	{
		return_value = CALLBACK_CONTINUE;

		PlaySoundObject(18, SOUND_MODE_PLAY);
	}
	else
	{
		PlaySoundObject(5, SOUND_MODE_PLAY);
	}

	return return_value;
}

//////////////////
// Options menu //
//////////////////

static int Callback_Framerate(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[FRAMERATE_50], menu_strings_table[FRAMERATE_60]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->b60fps;
			parent_menu->options[this_option].value_string = strings[conf->b60fps];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->b60fps = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gb60fps = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Font(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {"OpenDyslexic", "Monogram", "Lekton", "Courier New"};

	const FONT_DATA f_data[] = {
		{"opendyslexic", 15, 15, 29, 30},
		{"monogram", 13, 13, 26, 26},
		{"lekton", 12, 12, 26, 26},
		{"courier", 12, 12, 26, 26},
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->font_select;
			parent_menu->options[this_option].value_string = strings[conf->font_select];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->font_select = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value =  (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);
			
			gbFontData = f_data[parent_menu->options[this_option].value];

#if !defined(_3DS)
			InitTextObject("hi");
#else
			parent_menu->subtitle = menu_strings_table[RESTART_REQUIRED];
#endif

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Vsync(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bVsync;
			parent_menu->options[this_option].value_string = strings[conf->bVsync];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bVsync = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = TRUE;
			parent_menu->subtitle = menu_strings_table[RESTART_REQUIRED];

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ScreenShake(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bScreenShake;
			parent_menu->options[this_option].value_string = strings[conf->bScreenShake];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bScreenShake = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			gbScreenShake = parent_menu->options[this_option].value;

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Resolution(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {
		menu_strings_table[RESOLUTION_FULL], 
		menu_strings_table[RESOLUTION_240], 
		menu_strings_table[RESOLUTION_480],
		menu_strings_table[RESOLUTION_720],
		menu_strings_table[RESOLUTION_960]
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->display_mode;
			parent_menu->options[this_option].value_string = strings[conf->display_mode];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->display_mode = parent_menu->options[this_option].value;
			break;


		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			restart_required = TRUE;
			parent_menu->subtitle = menu_strings_table[RESTART_REQUIRED];

			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_SmoothScrolling(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bSmoothScrolling;
			parent_menu->options[this_option].value_string = strings[conf->bSmoothScrolling];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bSmoothScrolling = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbSmoothScrolling = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ColorFilters(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {
		menu_strings_table[COLOR_NONE],
		menu_strings_table[COLOR_PROTANOMALY],
		menu_strings_table[COLOR_PROTANOPIA],
		menu_strings_table[COLOR_DEUTERANOMALY],
		menu_strings_table[COLOR_DEUTERANOPIA],
		menu_strings_table[COLOR_TRITANOMALY],
		menu_strings_table[COLOR_TRITANOPIA],
		menu_strings_table[COLOR_GRAYSCALE],
		menu_strings_table[COLOR_GENESIS],
		menu_strings_table[COLOR_MSX],
		menu_strings_table[COLOR_GAME_BOY]
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->color_filter;
			parent_menu->options[this_option].value_string = strings[conf->color_filter];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->color_filter = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			gFilterIndex = parent_menu->options[this_option].value;
			ReloadAll();
			if (bMode == 1){
				Reload_CurrentStageTiles();
			}
			else if (bMode == 0){
				Reload_MenuBackground();
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_FlashMode(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {
		menu_strings_table[PERCENT_100],
		menu_strings_table[PERCENT_50],
		menu_strings_table[PERCENT_0]
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->flash_mode;
			parent_menu->options[this_option].value_string = strings[conf->flash_mode];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->flash_mode = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value =  (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			gFlashMode = parent_menu->options[this_option].value;
			InitFlash();

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ExtendedUI(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bExtendedUI;
			parent_menu->options[this_option].value_string = strings[conf->bExtendedUI];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bExtendedUI = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gExtendedUI = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_EnableController(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bUseJoystick;
			parent_menu->options[this_option].value_string = strings[conf->bUseJoystick];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bUseJoystick = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbUseJoystick = parent_menu->options[this_option].value;
			parent_menu->options[this_option + 1].disabled = !gbUseJoystick;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_BackgroundBrightness(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {"100%", "75%", "50%", "25%"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->backgroundBrightness;
			parent_menu->options[this_option].value_string = strings[conf->backgroundBrightness];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->backgroundBrightness = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value =  (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			gDimmingFactor = (4.0 - parent_menu->options[this_option].value)/6;
			ReloadAll();
			if (bMode == 1){
				Reload_CurrentStageTiles();
			}
			else if (bMode == 0){
				Reload_MenuBackground();
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Graphics(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {"Original", "Upscaled"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->graphics_set;
			parent_menu->options[this_option].value_string = strings[conf->graphics_set];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->graphics_set = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value =  (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			gSpriteScale = 1 + parent_menu->options[this_option].value;
			gSpritePath = gDataPath + (gSpriteScale == 2? "/sprites_up": "/sprites_og");
			ReloadAll();
			if (bMode == 1){
				Reload_CurrentStageTiles();
			}
			else if (bMode == 0){
				Reload_MenuBackground();
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Options(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	// Make the options match the configuration data
	CONFIGDATA conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

#if !defined(__WIIU__) && !defined(_3DS)
	Option options[] = {
			{menu_strings_table[CONTROLS_KEYBOARD], Callback_ControlsKeyboard, NULL, NULL, 0, FALSE},
			{menu_strings_table[CONTROLS_ENABLE_GAMEPAD], Callback_EnableController, &conf, NULL, 0, FALSE, 0, 0},
			{menu_strings_table[CONTROLS_GAMEPAD], Callback_ControlsController, NULL, NULL, 0, !gbUseJoystick, 0, 0},
			{menu_strings_table[SOUNDTRACK], Callback_Soundtrack, &conf, NULL, 0, FALSE},
			{menu_strings_table[FRAMERATE], Callback_Framerate, &conf, NULL, 0, FALSE},
			{menu_strings_table[V_SYNC], Callback_Vsync, &conf, NULL, 0, FALSE},
			{menu_strings_table[RESOLUTION], Callback_Resolution, &conf, NULL, 0, FALSE},
			{menu_strings_table[SMOOTH_SCROLLING], Callback_SmoothScrolling, &conf, NULL, 0, FALSE},
			{menu_strings_table[SCREEN_SHAKE], Callback_ScreenShake, &conf, NULL, 0, FALSE, 0, 0},
	#if GAMELANG != JP 
			{"Font", Callback_Font, &conf, NULL, 0, FALSE, 0, 0},
	#endif
			{menu_strings_table[COLOR_FILTERS], Callback_ColorFilters, &conf, NULL, 0, FALSE, 0, 0},
			{menu_strings_table[SCREEN_FLASHING], Callback_FlashMode, &conf, NULL, 0, FALSE, 0, 0},
			{"Background Brightness", Callback_BackgroundBrightness, &conf, NULL, 0, FALSE, 0, 0},
			{menu_strings_table[EXTENDED_UI], Callback_ExtendedUI, &conf, NULL, 0, FALSE, 0, 0}
		};
#else
	Option options[] = {
		{menu_strings_table[CONTROLS], Callback_ControlsController, NULL, NULL, 0, FALSE},
		{menu_strings_table[SOUNDTRACK], Callback_Soundtrack, &conf, NULL, 0, FALSE},
		{menu_strings_table[FRAMERATE], Callback_Framerate, &conf, NULL, 0, FALSE},
		{menu_strings_table[SMOOTH_SCROLLING], Callback_SmoothScrolling, &conf, NULL, 0, FALSE},
		{menu_strings_table[SCREEN_SHAKE], Callback_ScreenShake, &conf, NULL, 0, FALSE, 0, 0},
#if GAMELANG != JP 
		{"Font", Callback_Font, &conf, NULL, 0, FALSE, 0, 0},
#endif
		{menu_strings_table[COLOR_FILTERS], Callback_ColorFilters, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SCREEN_FLASHING], Callback_FlashMode, &conf, NULL, 0, FALSE, 0, 0},
		{"Background Brightness", Callback_BackgroundBrightness, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[EXTENDED_UI], Callback_ExtendedUI, &conf, NULL, 0, FALSE, 0, 0}
	};
#endif
	
	OptionsMenu options_menu = {
		menu_strings_table[OPTIONS_TITLE],
		restart_required ? menu_strings_table[RESTART_REQUIRED] : NULL,
		options,
		(sizeof(options) / sizeof(options[0])),
		-130,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	return return_value;
}

/////////////////
// Tweaks Menu //
/////////////////

static int Callback_NoExpDrops(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;

	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bNoExpDrops;
			parent_menu->options[this_option].value_string = strings[conf->bNoExpDrops];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bNoExpDrops = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbNoExpDrops = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_DamageModifier(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {
		menu_strings_table[PERCENT_100],
		menu_strings_table[PERCENT_200],
		menu_strings_table[DAMAGE_OHKO],
		menu_strings_table[PERCENT_0],
		menu_strings_table[PERCENT_50]
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->damage_modifier;
			parent_menu->options[this_option].value_string = strings[conf->damage_modifier];
			parent_menu->options[this_option].attribute_index  = (parent_menu->options[this_option].value +2) % 5;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->damage_modifier = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			switch(parent_menu->options[this_option].value){
				case 0:
					gbDamageModifier = 1;
					break;
				case 1:
					gbDamageModifier = 2;
					break;
				case 2:
					gbDamageModifier = -1;
					break;
				case 3:
					gbDamageModifier = 0;
					break;
				case 4:
					gbDamageModifier = 0.5;
					break;
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = (parent_menu->options[this_option].value +2) % 5;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_AutoFire(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {
		menu_strings_table[OFF],
		menu_strings_table[AUTOFIRE_HOLD],
		menu_strings_table[AUTOFIRE_SWITCH]
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->autoFire;
			parent_menu->options[this_option].value_string = strings[conf->autoFire];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->autoFire = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			gbAutoFire = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_InfiniteAmmo(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bInfiniteAmmo;
			parent_menu->options[this_option].value_string = strings[conf->bInfiniteAmmo];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bInfiniteAmmo = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbInfiniteAmmo = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_DefaultBooster(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {
		menu_strings_table[DEFAULT_BOOSTER_NONE],
		menu_strings_table[DEFAULT_BOOSTER_08],
		menu_strings_table[DEFAULT_BOOSTER_20]
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->defaultBooster;
			parent_menu->options[this_option].value_string = strings[conf->defaultBooster];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->defaultBooster = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			gDefaultBooster = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_InfiniteBooster(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bInfiniteBooster;
			parent_menu->options[this_option].value_string = strings[conf->bInfiniteBooster];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bInfiniteBooster = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbInfiniteBoost = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_GraceJump(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {menu_strings_table[OFF], menu_strings_table[ON]};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bGraceJumps;
			parent_menu->options[this_option].value_string = strings[conf->bGraceJumps];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->bGraceJumps = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			gbGraceJump = parent_menu->options[this_option].value;

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_GameSpeed(OptionsMenu *parent_menu, size_t this_option, CallbackAction action){
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {
		menu_strings_table[PERCENT_50], 
		menu_strings_table[PERCENT_60],
		menu_strings_table[PERCENT_70],
		menu_strings_table[PERCENT_80],
		menu_strings_table[PERCENT_90],
		menu_strings_table[PERCENT_100],
		menu_strings_table[PERCENT_110],
		menu_strings_table[PERCENT_120],
		menu_strings_table[PERCENT_130],
		menu_strings_table[PERCENT_140],
		menu_strings_table[PERCENT_150],
	};

	const double values[] = {0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->game_speed_multiplier;
			parent_menu->options[this_option].value_string = strings[conf->game_speed_multiplier];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->game_speed_multiplier = parent_menu->options[this_option].value;
			gCounter = gCorrectedCounter = 0;
			gSpeedMultiplier = values[parent_menu->options[this_option].value];
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			// Increment value (with wrapping)
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index  = parent_menu->options[this_option].value;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_BossHealthModifier(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIGDATA *conf = (CONFIGDATA*)parent_menu->options[this_option].user_data;
	
	const char *strings[] = {
		menu_strings_table[PERCENT_100], 
		menu_strings_table[PERCENT_150],
		menu_strings_table[PERCENT_200],
		menu_strings_table[BOSS_1_HP],
		menu_strings_table[PERCENT_50],
	};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->boss_health_multiplier;
			parent_menu->options[this_option].value_string = strings[conf->boss_health_multiplier];
			parent_menu->options[this_option].attribute_index  = (parent_menu->options[this_option].value + 2) % 5;
			parent_menu->options[this_option].attribute_size  = sizeof(strings)/sizeof(strings[0]);
			break;

		case ACTION_DEINIT:
			conf->boss_health_multiplier = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = (sizeof(strings) / sizeof(strings[0])) - 1;
			}
			else
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > (sizeof(strings) / sizeof(strings[0])) - 1)
					parent_menu->options[this_option].value = 0;
			}

			switch(parent_menu->options[this_option].value){
				case 0:
					gBossHPMultiplier = 1;
					break;
				case 1:
					gBossHPMultiplier = 1.5;
					break;
				case 2:
					gBossHPMultiplier = 2;
					break;
				case 3:
					gBossHPMultiplier = -1;
					break;
				case 4:
					gBossHPMultiplier = 0.5;
					break;
			}

			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			parent_menu->options[this_option].attribute_index = (parent_menu->options[this_option].value + 2) % 5;
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Tweaks(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	// Make the options match the configuration data
	CONFIGDATA conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	Option submenu_options[] = {
		{menu_strings_table[DAMAGE_RECEIVED], Callback_DamageModifier, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[BOSS_HP], Callback_BossHealthModifier, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[AUTOFIRE], Callback_AutoFire, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[INFINITE_AMMO], Callback_InfiniteAmmo, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[DEFAULT_BOOSTER], Callback_DefaultBooster, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[INFINITE_BOOSTER], Callback_InfiniteBooster, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[GRACE_JUMPS], Callback_GraceJump, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[NO_EXPERIENCE_DROPS], Callback_NoExpDrops, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[GAME_SPEED], Callback_GameSpeed, &conf, NULL, 0, FALSE, 0, 0}
	};

	OptionsMenu options_menu = {
		menu_strings_table[TWEAKS_TITLE],
		tweaks_restart_required ? menu_strings_table[RESTART_REQUIRED] : NULL,
		submenu_options,
		(sizeof(submenu_options) / sizeof(submenu_options[0])),
		-130,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	return return_value;
}


////////////////
// Pause menu //
////////////////

static int PromptAreYouSure(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{menu_strings_table[YES], FunctionHolder::Callback_Yes, NULL, NULL, 0, FALSE},
		{menu_strings_table[NO], FunctionHolder::Callback_No, NULL, NULL, 0, FALSE}
	};

	OptionsMenu options_menu = {
		menu_strings_table[ARE_YOU_SURE],
		menu_strings_table[UNSAVED_PROGRESS],
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 1);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

static int Callback_Resume(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	PlaySoundObject(18, SOUND_MODE_PLAY);
	return enum_ESCRETURN_continue;
}

static int Callback_Reset(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			return_value = CALLBACK_RESET;	// Restart game
			break;
	}

	return return_value;
}

static int Callback_Quit(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			return_value = CALLBACK_EXIT;	// Exit game
			break;
	}

	return return_value;
}

int Call_Pause(unsigned int mode_action)
{
	Option options[] = {
		{menu_strings_table[RESUME], Callback_Resume, NULL, NULL, 0, FALSE},
		{menu_strings_table[RESET], Callback_Reset, NULL, NULL, 0, FALSE},
		{menu_strings_table[OPTIONS], Callback_Options, NULL, NULL, 0, FALSE},
		{menu_strings_table[TWEAKS], Callback_Tweaks, NULL, NULL, 0, FALSE},
		{menu_strings_table[QUIT], Callback_Quit, NULL, NULL, 0, FALSE}
	};

	OptionsMenu options_menu = {
		menu_strings_table[PAUSED],
		NULL,
		options,
		sizeof(options) / sizeof(options[0]),
		-24,
		FALSE
	};

	bMode = mode_action;

	int return_value = EnterOptionsMenu(&options_menu, 0);

	// Filter internal return values to something Cave Story can understand
	switch (return_value)
	{
		case CALLBACK_CONTINUE:
			return_value = enum_ESCRETURN_continue;
			break;

		case CALLBACK_RESET:
			return_value = enum_ESCRETURN_restart;
			break;

		case CALLBACK_EXIT:
			return_value = enum_ESCRETURN_exit;
			break;
	}

	gKeyTrg = gKey = 0;	// Avoid input-ghosting

	return return_value;
}

int Call_Options(unsigned int mode_action)
{
	// Make the options match the configuration data
	CONFIGDATA conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

#if !defined(__WIIU__) && !defined(_3DS)
	Option options[] = {
		{menu_strings_table[CONTROLS_KEYBOARD], Callback_ControlsKeyboard, NULL, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[CONTROLS_ENABLE_GAMEPAD], Callback_EnableController, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[CONTROLS_GAMEPAD], Callback_ControlsController, NULL, NULL, 0, !gbUseJoystick, 0, 0},
		{menu_strings_table[SOUNDTRACK], Callback_Soundtrack, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[FRAMERATE], Callback_Framerate, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[V_SYNC], Callback_Vsync, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[RESOLUTION], Callback_Resolution, &conf, NULL, 0, FALSE, 0, 0},
	#if !defined(JAPANESE) && !defined(SPANISH) 
		{"Graphics Set", Callback_Graphics, &conf, NULL, 0, FALSE, 0, 0},
	#endif
		{menu_strings_table[SMOOTH_SCROLLING], Callback_SmoothScrolling, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SCREEN_SHAKE], Callback_ScreenShake, &conf, NULL, 0, FALSE, 0, 0},
	#ifndef JAPANESE 
		{"Font", Callback_Font, &conf, NULL, 0, FALSE, 0, 0},
	#endif
		{menu_strings_table[COLOR_FILTERS], Callback_ColorFilters, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SCREEN_FLASHING], Callback_FlashMode, &conf, NULL, 0, FALSE, 0, 0},
		{"Background Brightness", Callback_BackgroundBrightness, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[EXTENDED_UI], Callback_ExtendedUI, &conf, NULL, 0, FALSE, 0, 0}
	};
#else
	Option options[] = {
		{menu_strings_table[CONTROLS], Callback_ControlsController, NULL, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SOUNDTRACK], Callback_Soundtrack, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[FRAMERATE], Callback_Framerate, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SMOOTH_SCROLLING], Callback_SmoothScrolling, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SCREEN_SHAKE], Callback_ScreenShake, &conf, NULL, 0, FALSE, 0, 0},
	#ifndef JAPANESE 
		{"Font", Callback_Font, &conf, NULL, 0, FALSE, 0, 0},
	#endif
		{menu_strings_table[COLOR_FILTERS], Callback_ColorFilters, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[SCREEN_FLASHING], Callback_FlashMode, &conf, NULL, 0, FALSE, 0, 0},
		{"Background Brightness", Callback_BackgroundBrightness, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[EXTENDED_UI], Callback_ExtendedUI, &conf, NULL, 0, FALSE, 0, 0}
	};
#endif

	
	OptionsMenu options_menu = {
		menu_strings_table[OPTIONS_TITLE],
		restart_required ? menu_strings_table[RESTART_REQUIRED] : NULL,
		options,
		(sizeof(options) / sizeof(options[0])),
		-130,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	bMode = mode_action;

	int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	// Filter internal return values to something Cave Story can understand
	switch (return_value)
	{
		case CALLBACK_CONTINUE:
			return_value = enum_ESCRETURN_continue;
			break;

		case CALLBACK_RESET:
			return_value = enum_ESCRETURN_restart;
			break;

		case CALLBACK_EXIT:
			return_value = enum_ESCRETURN_exit;
			break;
	}

	gKeyTrg = gKey = 0;	// Avoid input-ghosting

	return return_value;
}

int Call_Tweaks(unsigned int mode_action)
{
	// Make the options match the configuration data
	CONFIGDATA conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	Option submenu_options[] = {
		{menu_strings_table[DAMAGE_RECEIVED], Callback_DamageModifier, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[BOSS_HP], Callback_BossHealthModifier, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[AUTOFIRE], Callback_AutoFire, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[INFINITE_AMMO], Callback_InfiniteAmmo, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[DEFAULT_BOOSTER], Callback_DefaultBooster, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[INFINITE_BOOSTER], Callback_InfiniteBooster, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[GRACE_JUMPS], Callback_GraceJump, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[NO_EXPERIENCE_DROPS], Callback_NoExpDrops, &conf, NULL, 0, FALSE, 0, 0},
		{menu_strings_table[GAME_SPEED], Callback_GameSpeed, &conf, NULL, 0, FALSE, 0, 0}
	};

	OptionsMenu options_menu = {
		menu_strings_table[TWEAKS_TITLE],
		tweaks_restart_required ? menu_strings_table[RESTART_REQUIRED] : NULL,
		submenu_options,
		(sizeof(submenu_options) / sizeof(submenu_options[0])),
		-130,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	SaveConfigData(&conf);

	// Filter internal return values to something Cave Story can understand
	switch (return_value)
	{
		case CALLBACK_CONTINUE:
			return_value = enum_ESCRETURN_continue;
			break;

		case CALLBACK_RESET:
			return_value = enum_ESCRETURN_restart;
			break;

		case CALLBACK_EXIT:
			return_value = enum_ESCRETURN_exit;
			break;
	}

	gKeyTrg = gKey = 0;	// Avoid input-ghosting

	return return_value;
}