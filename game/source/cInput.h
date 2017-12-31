#pragma once

#include <SDL.h>
#include <unordered_map>
#include "cMat.h"
#include "cVec.h"

enum Key
{
	key_invalid,
	key_a = SDL_SCANCODE_A,
	key_b = SDL_SCANCODE_B,
	key_c = SDL_SCANCODE_C,
	key_d = SDL_SCANCODE_D,
	key_e = SDL_SCANCODE_E,
	key_f = SDL_SCANCODE_F,
	key_g = SDL_SCANCODE_G,
	key_h = SDL_SCANCODE_H,
	key_i = SDL_SCANCODE_I,
	key_j = SDL_SCANCODE_J,
	key_k = SDL_SCANCODE_K,
	key_l = SDL_SCANCODE_L,
	key_m = SDL_SCANCODE_M,
	key_n = SDL_SCANCODE_N,
	key_o = SDL_SCANCODE_O,
	key_p = SDL_SCANCODE_P,
	key_q = SDL_SCANCODE_Q,
	key_r = SDL_SCANCODE_R,
	key_s = SDL_SCANCODE_S,
	key_t = SDL_SCANCODE_T,
	key_u = SDL_SCANCODE_U,
	key_v = SDL_SCANCODE_V,
	key_w = SDL_SCANCODE_W,
	key_x = SDL_SCANCODE_X,
	key_y = SDL_SCANCODE_Y,
	key_z = SDL_SCANCODE_Z,
	key_1 = SDL_SCANCODE_1,
	key_2 = SDL_SCANCODE_2,
	key_3 = SDL_SCANCODE_3,
	key_4 = SDL_SCANCODE_4,
	key_5 = SDL_SCANCODE_5,
	key_6 = SDL_SCANCODE_6,
	key_7 = SDL_SCANCODE_7,
	key_8 = SDL_SCANCODE_8,
	key_9 = SDL_SCANCODE_9,
	key_0 = SDL_SCANCODE_0,
	key_return = SDL_SCANCODE_RETURN,
	key_escape = SDL_SCANCODE_ESCAPE,
	key_backspace = SDL_SCANCODE_BACKSPACE,
	key_tab = SDL_SCANCODE_TAB,
	key_space = SDL_SCANCODE_SPACE,
	key_minus = SDL_SCANCODE_MINUS,
	key_equals = SDL_SCANCODE_EQUALS,
	key_leftbracket = SDL_SCANCODE_LEFTBRACKET,
	key_rightbracket = SDL_SCANCODE_RIGHTBRACKET,
	key_backslash = SDL_SCANCODE_BACKSLASH,
	key_nonushash = SDL_SCANCODE_NONUSHASH,
	key_semicolon = SDL_SCANCODE_SEMICOLON,
	key_apostrophe = SDL_SCANCODE_APOSTROPHE,
	key_grave = SDL_SCANCODE_GRAVE,
	key_comma = SDL_SCANCODE_COMMA,
	key_period = SDL_SCANCODE_PERIOD,
	key_slash = SDL_SCANCODE_SLASH,
	key_capslock = SDL_SCANCODE_CAPSLOCK,
	key_f1 = SDL_SCANCODE_F1,
	key_f2 = SDL_SCANCODE_F2,
	key_f3 = SDL_SCANCODE_F3,
	key_f4 = SDL_SCANCODE_F4,
	key_f5 = SDL_SCANCODE_F5,
	key_f6 = SDL_SCANCODE_F6,
	key_f7 = SDL_SCANCODE_F7,
	key_f8 = SDL_SCANCODE_F8,
	key_f9 = SDL_SCANCODE_F9,
	key_f10 = SDL_SCANCODE_F10,
	key_f11 = SDL_SCANCODE_F11,
	key_f12 = SDL_SCANCODE_F12,
	key_printscreen = SDL_SCANCODE_PRINTSCREEN,
	key_scrolllock = SDL_SCANCODE_SCROLLLOCK,
	key_pause = SDL_SCANCODE_PAUSE,
	key_insert = SDL_SCANCODE_INSERT,
	key_home = SDL_SCANCODE_HOME,
	key_pageup = SDL_SCANCODE_PAGEUP,
	key_delete = SDL_SCANCODE_DELETE,
	key_end = SDL_SCANCODE_END,
	key_pagedown = SDL_SCANCODE_PAGEDOWN,
	key_right = SDL_SCANCODE_RIGHT,
	key_left = SDL_SCANCODE_LEFT,
	key_down = SDL_SCANCODE_DOWN,
	key_up = SDL_SCANCODE_UP,
	key_numlockclear = SDL_SCANCODE_NUMLOCKCLEAR,
	key_num_divide = SDL_SCANCODE_KP_DIVIDE,
	key_num_multiply = SDL_SCANCODE_KP_MULTIPLY,
	key_num_minus = SDL_SCANCODE_KP_MINUS,
	key_num_plus = SDL_SCANCODE_KP_PLUS,
	key_num_enter = SDL_SCANCODE_KP_ENTER,
	key_num_1 = SDL_SCANCODE_KP_1,
	key_num_2 = SDL_SCANCODE_KP_2,
	key_num_3 = SDL_SCANCODE_KP_3,
	key_num_4 = SDL_SCANCODE_KP_4,
	key_num_5 = SDL_SCANCODE_KP_5,
	key_num_6 = SDL_SCANCODE_KP_6,
	key_num_7 = SDL_SCANCODE_KP_7,
	key_num_8 = SDL_SCANCODE_KP_8,
	key_num_9 = SDL_SCANCODE_KP_9,
	key_num_0 = SDL_SCANCODE_KP_0,
	key_num_period = SDL_SCANCODE_KP_PERIOD,
	key_nonusbackslash = SDL_SCANCODE_NONUSBACKSLASH,
	key_application = SDL_SCANCODE_APPLICATION,
	key_power = SDL_SCANCODE_POWER,
	key_num_equals = SDL_SCANCODE_KP_EQUALS,
	key_f13 = SDL_SCANCODE_F13,
	key_f14 = SDL_SCANCODE_F14,
	key_f15 = SDL_SCANCODE_F15,
	key_f16 = SDL_SCANCODE_F16,
	key_f17 = SDL_SCANCODE_F17,
	key_f18 = SDL_SCANCODE_F18,
	key_f19 = SDL_SCANCODE_F19,
	key_f20 = SDL_SCANCODE_F20,
	key_f21 = SDL_SCANCODE_F21,
	key_f22 = SDL_SCANCODE_F22,
	key_f23 = SDL_SCANCODE_F23,
	key_f24 = SDL_SCANCODE_F24,
	key_execute = SDL_SCANCODE_EXECUTE,
	key_help = SDL_SCANCODE_HELP,
	key_menu = SDL_SCANCODE_MENU,
	key_select = SDL_SCANCODE_SELECT,
	key_stop = SDL_SCANCODE_STOP,
	key_again = SDL_SCANCODE_AGAIN,
	key_undo = SDL_SCANCODE_UNDO,
	key_cut = SDL_SCANCODE_CUT,
	key_copy = SDL_SCANCODE_COPY,
	key_paste = SDL_SCANCODE_PASTE,
	key_find = SDL_SCANCODE_FIND,
	key_mute = SDL_SCANCODE_MUTE,
	key_volumeup = SDL_SCANCODE_VOLUMEUP,
	key_volumedown = SDL_SCANCODE_VOLUMEDOWN,
	key_num_comma = SDL_SCANCODE_KP_COMMA,
	key_num_equalsas400 = SDL_SCANCODE_KP_EQUALSAS400,
	key_international1 = SDL_SCANCODE_INTERNATIONAL1,
	key_international2 = SDL_SCANCODE_INTERNATIONAL2,
	key_international3 = SDL_SCANCODE_INTERNATIONAL3,
	key_international4 = SDL_SCANCODE_INTERNATIONAL4,
	key_international5 = SDL_SCANCODE_INTERNATIONAL5,
	key_international6 = SDL_SCANCODE_INTERNATIONAL6,
	key_international7 = SDL_SCANCODE_INTERNATIONAL7,
	key_international8 = SDL_SCANCODE_INTERNATIONAL8,
	key_international9 = SDL_SCANCODE_INTERNATIONAL9,
	key_lang1 = SDL_SCANCODE_LANG1,
	key_lang2 = SDL_SCANCODE_LANG2,
	key_lang3 = SDL_SCANCODE_LANG3,
	key_lang4 = SDL_SCANCODE_LANG4,
	key_lang5 = SDL_SCANCODE_LANG5,
	key_lang6 = SDL_SCANCODE_LANG6,
	key_lang7 = SDL_SCANCODE_LANG7,
	key_lang8 = SDL_SCANCODE_LANG8,
	key_lang9 = SDL_SCANCODE_LANG9,
	key_alterase = SDL_SCANCODE_ALTERASE,
	key_sysreq = SDL_SCANCODE_SYSREQ,
	key_cancel = SDL_SCANCODE_CANCEL,
	key_clear = SDL_SCANCODE_CLEAR,
	key_prior = SDL_SCANCODE_PRIOR,
	key_return2 = SDL_SCANCODE_RETURN2,
	key_separator = SDL_SCANCODE_SEPARATOR,
	key_out = SDL_SCANCODE_OUT,
	key_oper = SDL_SCANCODE_OPER,
	key_clearagain = SDL_SCANCODE_CLEARAGAIN,
	key_crsel = SDL_SCANCODE_CRSEL,
	key_exsel = SDL_SCANCODE_EXSEL,
	key_num_00 = SDL_SCANCODE_KP_00,
	key_num_000 = SDL_SCANCODE_KP_000,
	key_thousandsseparator = SDL_SCANCODE_THOUSANDSSEPARATOR,
	key_decimalseparator = SDL_SCANCODE_DECIMALSEPARATOR,
	key_currencyunit = SDL_SCANCODE_CURRENCYUNIT,
	key_currencysubunit = SDL_SCANCODE_CURRENCYSUBUNIT,
	key_num_leftparen = SDL_SCANCODE_KP_LEFTPAREN,
	key_num_rightparen = SDL_SCANCODE_KP_RIGHTPAREN,
	key_num_leftbrace = SDL_SCANCODE_KP_LEFTBRACE,
	key_num_rightbrace = SDL_SCANCODE_KP_RIGHTBRACE,
	key_num_tab = SDL_SCANCODE_KP_TAB,
	key_num_backspace = SDL_SCANCODE_KP_BACKSPACE,
	key_num_a = SDL_SCANCODE_KP_A,
	key_num_b = SDL_SCANCODE_KP_B,
	key_num_c = SDL_SCANCODE_KP_C,
	key_num_d = SDL_SCANCODE_KP_D,
	key_num_e = SDL_SCANCODE_KP_E,
	key_num_f = SDL_SCANCODE_KP_F,
	key_num_xor = SDL_SCANCODE_KP_XOR,
	key_num_power = SDL_SCANCODE_KP_POWER,
	key_num_percent = SDL_SCANCODE_KP_PERCENT,
	key_num_less = SDL_SCANCODE_KP_LESS,
	key_num_greater = SDL_SCANCODE_KP_GREATER,
	key_num_ampersand = SDL_SCANCODE_KP_AMPERSAND,
	key_num_dblampersand = SDL_SCANCODE_KP_DBLAMPERSAND,
	key_num_verticalbar = SDL_SCANCODE_KP_VERTICALBAR,
	key_num_dblverticalbar = SDL_SCANCODE_KP_DBLVERTICALBAR,
	key_num_colon = SDL_SCANCODE_KP_COLON,
	key_num_hash = SDL_SCANCODE_KP_HASH,
	key_num_space = SDL_SCANCODE_KP_SPACE,
	key_num_at = SDL_SCANCODE_KP_AT,
	key_num_exclam = SDL_SCANCODE_KP_EXCLAM,
	key_num_memstore = SDL_SCANCODE_KP_MEMSTORE,
	key_num_memrecall = SDL_SCANCODE_KP_MEMRECALL,
	key_num_memclear = SDL_SCANCODE_KP_MEMCLEAR,
	key_num_memadd = SDL_SCANCODE_KP_MEMADD,
	key_num_memsubtract = SDL_SCANCODE_KP_MEMSUBTRACT,
	key_num_memmultiply = SDL_SCANCODE_KP_MEMMULTIPLY,
	key_num_memdivide = SDL_SCANCODE_KP_MEMDIVIDE,
	key_num_plusminus = SDL_SCANCODE_KP_PLUSMINUS,
	key_num_clear = SDL_SCANCODE_KP_CLEAR,
	key_num_clearentry = SDL_SCANCODE_KP_CLEARENTRY,
	key_num_binary = SDL_SCANCODE_KP_BINARY,
	key_num_octal = SDL_SCANCODE_KP_OCTAL,
	key_num_decimal = SDL_SCANCODE_KP_DECIMAL,
	key_num_hexadecimal = SDL_SCANCODE_KP_HEXADECIMAL,
	key_lctrl = SDL_SCANCODE_LCTRL,
	key_lshift = SDL_SCANCODE_LSHIFT,
	key_lalt = SDL_SCANCODE_LALT,
	key_lgui = SDL_SCANCODE_LGUI,
	key_rctrl = SDL_SCANCODE_RCTRL,
	key_rshift = SDL_SCANCODE_RSHIFT,
	key_ralt = SDL_SCANCODE_RALT,
	key_rgui = SDL_SCANCODE_RGUI,
	key_mode = SDL_SCANCODE_MODE,
	key_audionext = SDL_SCANCODE_AUDIONEXT,
	key_audioprev = SDL_SCANCODE_AUDIOPREV,
	key_audiostop = SDL_SCANCODE_AUDIOSTOP,
	key_audioplay = SDL_SCANCODE_AUDIOPLAY,
	key_audiomute = SDL_SCANCODE_AUDIOMUTE,
	key_mediaselect = SDL_SCANCODE_MEDIASELECT,
	key_www = SDL_SCANCODE_WWW,
	key_mail = SDL_SCANCODE_MAIL,
	key_calculator = SDL_SCANCODE_CALCULATOR,
	key_computer = SDL_SCANCODE_COMPUTER,
	key_ac_search = SDL_SCANCODE_AC_SEARCH,
	key_ac_home = SDL_SCANCODE_AC_HOME,
	key_ac_back = SDL_SCANCODE_AC_BACK,
	key_ac_forward = SDL_SCANCODE_AC_FORWARD,
	key_ac_stop = SDL_SCANCODE_AC_STOP,
	key_ac_refresh = SDL_SCANCODE_AC_REFRESH,
	key_ac_bookmarks = SDL_SCANCODE_AC_BOOKMARKS,
	key_brightnessdown = SDL_SCANCODE_BRIGHTNESSDOWN,
	key_brightnessup = SDL_SCANCODE_BRIGHTNESSUP,
	key_displayswitch = SDL_SCANCODE_DISPLAYSWITCH,
	key_kbdillumtoggle = SDL_SCANCODE_KBDILLUMTOGGLE,
	key_kbdillumdown = SDL_SCANCODE_KBDILLUMDOWN,
	key_kbdillumup = SDL_SCANCODE_KBDILLUMUP,
	key_eject = SDL_SCANCODE_EJECT,
	key_sleep = SDL_SCANCODE_SLEEP,
	key_app1 = SDL_SCANCODE_APP1,
	key_app2 = SDL_SCANCODE_APP2,

	mouse_start = SDL_NUM_SCANCODES,
	mouse_button_left,
	mouse_button_middle,
	mouse_button_right,
	mouse_button_4,
	mouse_button_5,
	mouse_end = mouse_button_5,

	joystick_0_button_up,
	joystick_0_button_down,
	joystick_0_button_left,
	joystick_0_button_right,

	joystick_0_button_a,
	joystick_0_button_b,
	joystick_0_button_x,
	joystick_0_button_y,
	joystick_0_button_leftshoulder,
	joystick_0_button_rightshoulder,
	joystick_0_button_back,
	joystick_0_button_start,

	key_count,

	joystick_0_start = joystick_0_button_a,
	joystick_0_end = joystick_0_button_start,

	joystickhat_hat_0_start = joystick_0_button_up,
	joystickhat_hat_0_end = joystick_0_button_right,

};

class cInput
{
private:
	friend class Coral;
	void init();
	
	bool prevKeyStates[key_count];
	bool keyStates[key_count];
	float keyTime[key_count];

	void pressKey(int key);
	void releaseKey(int key);
	void mouseWhellMove(int x, int y);

	void setMousePos(const Vec2& pos, const Vec2& relativePos);

	friend void RunGame();
	Vec2 mousePos;
	Vec2 prevMousePos;
	bool mouseShown;
	bool ignoreNextMove;
	IntVec2 mouseWheel;

	std::string names[key_count];
	std::unordered_map<std::string, Key> nameMap;
	void setLuaKeys();

	Vec2 joyAxisPos;
	Vec2 joyAxisPos2;

	bool useEvents;
public:
	void tick();
	void preTick();

	Key getKey(const std::string& name)
	{
		return nameMap[name];
	}

	bool isKeyDown(int key)
	{
		return keyStates[key];
	}

	bool isKeyUp(int key)
	{
		return !keyStates[key];
	}

	bool isKeyPressed(int key)
	{
		return keyStates[key] && !prevKeyStates[key];
	}

	bool isKeyReleased(int key)
	{
		return !keyStates[key] && prevKeyStates[key];
	}

	float getKeyTime(int key)
	{
		return keyTime[key];
	}

	void clearKeyPress(int key)
	{
		keyStates[key] = false;
	}

	void clearKeyRelease(int key)
	{
		prevKeyStates[key] = false;
	}

	void showMouse();

	void hideMouse();

	const Vec2& getMousePos() const
	{
		return mousePos;
	}

	const Vec2& getJoystickAxisPos() const
	{
		return joyAxisPos;
	}

	const Vec2& getJoystickAxisPos2() const
	{
		return joyAxisPos2;
	}
	bool hasJoyStick() const;

	Vec2 getDeltaMousePos() const
	{
		return mousePos - prevMousePos;
	}

	bool isMouseVisible() const
	{
		return mouseShown;
	}

	void setMousePosition(int x, int y);
	void setJoystickAxis(int joystick, int axis, int value);
	const std::string& getKeyName(Key key);

	int getMouseWheel() const
	{
		return mouseWheel[1];
	}

	const IntVec2& getMouseWheel2d() const
	{
		return mouseWheel;
	}
	void clearWheel();

	void setUseEvents(bool useEvents);

	bool isUsingEvents() const
	{
		return useEvents;
	}
};