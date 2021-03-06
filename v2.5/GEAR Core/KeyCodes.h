// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

#define SDLK_SCANCODE_MASK (1<<30)
#define SDL_SCANCODE_TO_KEYCODE(X)  (X | SDLK_SCANCODE_MASK)

namespace G2 
{
	
	typedef int KeyCode;
	/**
	 *  \brief The SDL keyboard scancode representation.
	 *
	 *  Values of this type are used to represent keyboard keys, among other places
	 *  in the \link SDL_Keysym::scancode key.keysym.scancode \endlink field of the
	 *  SDL_Event structure.
	 *
	 *  The values in this enumeration are based on the USB usage page standard:
	 *  http://www.usb.org/developers/devclass_docs/Hut1_12v2.pdf
	 */
	typedef enum 
	{
		SDL_SCANCODE_UNKNOWN = 0,

		/**
		 *  \name Usage page 0x07
		 *
		 *  These values are from usage page 0x07 (USB keyboard page).
		 */
		/* @{ */

		SDL_SCANCODE_A = 4,
		SDL_SCANCODE_B = 5,
		SDL_SCANCODE_C = 6,
		SDL_SCANCODE_D = 7,
		SDL_SCANCODE_E = 8,
		SDL_SCANCODE_F = 9,
		SDL_SCANCODE_G = 10,
		SDL_SCANCODE_H = 11,
		SDL_SCANCODE_I = 12,
		SDL_SCANCODE_J = 13,
		SDL_SCANCODE_K = 14,
		SDL_SCANCODE_L = 15,
		SDL_SCANCODE_M = 16,
		SDL_SCANCODE_N = 17,
		SDL_SCANCODE_O = 18,
		SDL_SCANCODE_P = 19,
		SDL_SCANCODE_Q = 20,
		SDL_SCANCODE_R = 21,
		SDL_SCANCODE_S = 22,
		SDL_SCANCODE_T = 23,
		SDL_SCANCODE_U = 24,
		SDL_SCANCODE_V = 25,
		SDL_SCANCODE_W = 26,
		SDL_SCANCODE_X = 27,
		SDL_SCANCODE_Y = 28,
		SDL_SCANCODE_Z = 29,

		SDL_SCANCODE_1 = 30,
		SDL_SCANCODE_2 = 31,
		SDL_SCANCODE_3 = 32,
		SDL_SCANCODE_4 = 33,
		SDL_SCANCODE_5 = 34,
		SDL_SCANCODE_6 = 35,
		SDL_SCANCODE_7 = 36,
		SDL_SCANCODE_8 = 37,
		SDL_SCANCODE_9 = 38,
		SDL_SCANCODE_0 = 39,

		SDL_SCANCODE_RETURN = 40,
		SDL_SCANCODE_ESCAPE = 41,
		SDL_SCANCODE_BACKSPACE = 42,
		SDL_SCANCODE_TAB = 43,
		SDL_SCANCODE_SPACE = 44,

		SDL_SCANCODE_MINUS = 45,
		SDL_SCANCODE_EQUALS = 46,
		SDL_SCANCODE_LEFTBRACKET = 47,
		SDL_SCANCODE_RIGHTBRACKET = 48,
		SDL_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
									  *   key on ISO keyboards and at the right end
									  *   of the QWERTY row on ANSI keyboards.
									  *   Produces REVERSE SOLIDUS (backslash) and
									  *   VERTICAL LINE in a US layout, REVERSE
									  *   SOLIDUS and VERTICAL LINE in a UK Mac
									  *   layout, NUMBER SIGN and TILDE in a UK
									  *   Windows layout, DOLLAR SIGN and POUND SIGN
									  *   in a Swiss German layout, NUMBER SIGN and
									  *   APOSTROPHE in a German layout, GRAVE
									  *   ACCENT and POUND SIGN in a French Mac
									  *   layout, and ASTERISK and MICRO SIGN in a
									  *   French Windows layout.
									  */
		SDL_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
									  *   instead of 49 for the same key, but all
									  *   OSes I've seen treat the two codes
									  *   identically. So, as an implementor, unless
									  *   your keyboard generates both of those
									  *   codes and your OS treats them differently,
									  *   you should generate SDL_SCANCODE_BACKSLASH
									  *   instead of this code. As a user, you
									  *   should not rely on this code because SDL
									  *   will never generate it with most (all?)
									  *   keyboards.
									  */
		SDL_SCANCODE_SEMICOLON = 51,
		SDL_SCANCODE_APOSTROPHE = 52,
		SDL_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
								  *   and ISO keyboards). Produces GRAVE ACCENT and
								  *   TILDE in a US Windows layout and in US and UK
								  *   Mac layouts on ANSI keyboards, GRAVE ACCENT
								  *   and NOT SIGN in a UK Windows layout, SECTION
								  *   SIGN and PLUS-MINUS SIGN in US and UK Mac
								  *   layouts on ISO keyboards, SECTION SIGN and
								  *   DEGREE SIGN in a Swiss German layout (Mac:
								  *   only on ISO keyboards), CIRCUMFLEX ACCENT and
								  *   DEGREE SIGN in a German layout (Mac: only on
								  *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
								  *   French Windows layout, COMMERCIAL AT and
								  *   NUMBER SIGN in a French Mac layout on ISO
								  *   keyboards, and LESS-THAN SIGN and GREATER-THAN
								  *   SIGN in a Swiss German, German, or French Mac
								  *   layout on ANSI keyboards.
								  */
		SDL_SCANCODE_COMMA = 54,
		SDL_SCANCODE_PERIOD = 55,
		SDL_SCANCODE_SLASH = 56,

		SDL_SCANCODE_CAPSLOCK = 57,

		SDL_SCANCODE_F1 = 58,
		SDL_SCANCODE_F2 = 59,
		SDL_SCANCODE_F3 = 60,
		SDL_SCANCODE_F4 = 61,
		SDL_SCANCODE_F5 = 62,
		SDL_SCANCODE_F6 = 63,
		SDL_SCANCODE_F7 = 64,
		SDL_SCANCODE_F8 = 65,
		SDL_SCANCODE_F9 = 66,
		SDL_SCANCODE_F10 = 67,
		SDL_SCANCODE_F11 = 68,
		SDL_SCANCODE_F12 = 69,

		SDL_SCANCODE_PRINTSCREEN = 70,
		SDL_SCANCODE_SCROLLLOCK = 71,
		SDL_SCANCODE_PAUSE = 72,
		SDL_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
									   does send code 73, not 117) */
		SDL_SCANCODE_HOME = 74,
		SDL_SCANCODE_PAGEUP = 75,
		SDL_SCANCODE_DELETE = 76,
		SDL_SCANCODE_END = 77,
		SDL_SCANCODE_PAGEDOWN = 78,
		SDL_SCANCODE_RIGHT = 79,
		SDL_SCANCODE_LEFT = 80,
		SDL_SCANCODE_DOWN = 81,
		SDL_SCANCODE_UP = 82,

		SDL_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
										 */
		SDL_SCANCODE_KP_DIVIDE = 84,
		SDL_SCANCODE_KP_MULTIPLY = 85,
		SDL_SCANCODE_KP_MINUS = 86,
		SDL_SCANCODE_KP_PLUS = 87,
		SDL_SCANCODE_KP_ENTER = 88,
		SDL_SCANCODE_KP_1 = 89,
		SDL_SCANCODE_KP_2 = 90,
		SDL_SCANCODE_KP_3 = 91,
		SDL_SCANCODE_KP_4 = 92,
		SDL_SCANCODE_KP_5 = 93,
		SDL_SCANCODE_KP_6 = 94,
		SDL_SCANCODE_KP_7 = 95,
		SDL_SCANCODE_KP_8 = 96,
		SDL_SCANCODE_KP_9 = 97,
		SDL_SCANCODE_KP_0 = 98,
		SDL_SCANCODE_KP_PERIOD = 99,

		SDL_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
											*   keyboards have over ANSI ones,
											*   located between left shift and Y.
											*   Produces GRAVE ACCENT and TILDE in a
											*   US or UK Mac layout, REVERSE SOLIDUS
											*   (backslash) and VERTICAL LINE in a
											*   US or UK Windows layout, and
											*   LESS-THAN SIGN and GREATER-THAN SIGN
											*   in a Swiss German, German, or French
											*   layout. */
		SDL_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
		SDL_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
								   *   not a physical key - but some Mac keyboards
								   *   do have a power key. */
		SDL_SCANCODE_KP_EQUALS = 103,
		SDL_SCANCODE_F13 = 104,
		SDL_SCANCODE_F14 = 105,
		SDL_SCANCODE_F15 = 106,
		SDL_SCANCODE_F16 = 107,
		SDL_SCANCODE_F17 = 108,
		SDL_SCANCODE_F18 = 109,
		SDL_SCANCODE_F19 = 110,
		SDL_SCANCODE_F20 = 111,
		SDL_SCANCODE_F21 = 112,
		SDL_SCANCODE_F22 = 113,
		SDL_SCANCODE_F23 = 114,
		SDL_SCANCODE_F24 = 115,
		SDL_SCANCODE_EXECUTE = 116,
		SDL_SCANCODE_HELP = 117,
		SDL_SCANCODE_MENU = 118,
		SDL_SCANCODE_SELECT = 119,
		SDL_SCANCODE_STOP = 120,
		SDL_SCANCODE_AGAIN = 121,   /**< redo */
		SDL_SCANCODE_UNDO = 122,
		SDL_SCANCODE_CUT = 123,
		SDL_SCANCODE_COPY = 124,
		SDL_SCANCODE_PASTE = 125,
		SDL_SCANCODE_FIND = 126,
		SDL_SCANCODE_MUTE = 127,
		SDL_SCANCODE_VOLUMEUP = 128,
		SDL_SCANCODE_VOLUMEDOWN = 129,
	/* not sure whether there's a reason to enable these */
	/*     SDL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
	/*     SDL_SCANCODE_LOCKINGNUMLOCK = 131, */
	/*     SDL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
		SDL_SCANCODE_KP_COMMA = 133,
		SDL_SCANCODE_KP_EQUALSAS400 = 134,

		SDL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
												footnotes in USB doc */
		SDL_SCANCODE_INTERNATIONAL2 = 136,
		SDL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
		SDL_SCANCODE_INTERNATIONAL4 = 138,
		SDL_SCANCODE_INTERNATIONAL5 = 139,
		SDL_SCANCODE_INTERNATIONAL6 = 140,
		SDL_SCANCODE_INTERNATIONAL7 = 141,
		SDL_SCANCODE_INTERNATIONAL8 = 142,
		SDL_SCANCODE_INTERNATIONAL9 = 143,
		SDL_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
		SDL_SCANCODE_LANG2 = 145, /**< Hanja conversion */
		SDL_SCANCODE_LANG3 = 146, /**< Katakana */
		SDL_SCANCODE_LANG4 = 147, /**< Hiragana */
		SDL_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
		SDL_SCANCODE_LANG6 = 149, /**< reserved */
		SDL_SCANCODE_LANG7 = 150, /**< reserved */
		SDL_SCANCODE_LANG8 = 151, /**< reserved */
		SDL_SCANCODE_LANG9 = 152, /**< reserved */

		SDL_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
		SDL_SCANCODE_SYSREQ = 154,
		SDL_SCANCODE_CANCEL = 155,
		SDL_SCANCODE_CLEAR = 156,
		SDL_SCANCODE_PRIOR = 157,
		SDL_SCANCODE_RETURN2 = 158,
		SDL_SCANCODE_SEPARATOR = 159,
		SDL_SCANCODE_OUT = 160,
		SDL_SCANCODE_OPER = 161,
		SDL_SCANCODE_CLEARAGAIN = 162,
		SDL_SCANCODE_CRSEL = 163,
		SDL_SCANCODE_EXSEL = 164,

		SDL_SCANCODE_KP_00 = 176,
		SDL_SCANCODE_KP_000 = 177,
		SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
		SDL_SCANCODE_DECIMALSEPARATOR = 179,
		SDL_SCANCODE_CURRENCYUNIT = 180,
		SDL_SCANCODE_CURRENCYSUBUNIT = 181,
		SDL_SCANCODE_KP_LEFTPAREN = 182,
		SDL_SCANCODE_KP_RIGHTPAREN = 183,
		SDL_SCANCODE_KP_LEFTBRACE = 184,
		SDL_SCANCODE_KP_RIGHTBRACE = 185,
		SDL_SCANCODE_KP_TAB = 186,
		SDL_SCANCODE_KP_BACKSPACE = 187,
		SDL_SCANCODE_KP_A = 188,
		SDL_SCANCODE_KP_B = 189,
		SDL_SCANCODE_KP_C = 190,
		SDL_SCANCODE_KP_D = 191,
		SDL_SCANCODE_KP_E = 192,
		SDL_SCANCODE_KP_F = 193,
		SDL_SCANCODE_KP_XOR = 194,
		SDL_SCANCODE_KP_POWER = 195,
		SDL_SCANCODE_KP_PERCENT = 196,
		SDL_SCANCODE_KP_LESS = 197,
		SDL_SCANCODE_KP_GREATER = 198,
		SDL_SCANCODE_KP_AMPERSAND = 199,
		SDL_SCANCODE_KP_DBLAMPERSAND = 200,
		SDL_SCANCODE_KP_VERTICALBAR = 201,
		SDL_SCANCODE_KP_DBLVERTICALBAR = 202,
		SDL_SCANCODE_KP_COLON = 203,
		SDL_SCANCODE_KP_HASH = 204,
		SDL_SCANCODE_KP_SPACE = 205,
		SDL_SCANCODE_KP_AT = 206,
		SDL_SCANCODE_KP_EXCLAM = 207,
		SDL_SCANCODE_KP_MEMSTORE = 208,
		SDL_SCANCODE_KP_MEMRECALL = 209,
		SDL_SCANCODE_KP_MEMCLEAR = 210,
		SDL_SCANCODE_KP_MEMADD = 211,
		SDL_SCANCODE_KP_MEMSUBTRACT = 212,
		SDL_SCANCODE_KP_MEMMULTIPLY = 213,
		SDL_SCANCODE_KP_MEMDIVIDE = 214,
		SDL_SCANCODE_KP_PLUSMINUS = 215,
		SDL_SCANCODE_KP_CLEAR = 216,
		SDL_SCANCODE_KP_CLEARENTRY = 217,
		SDL_SCANCODE_KP_BINARY = 218,
		SDL_SCANCODE_KP_OCTAL = 219,
		SDL_SCANCODE_KP_DECIMAL = 220,
		SDL_SCANCODE_KP_HEXADECIMAL = 221,

		SDL_SCANCODE_LCTRL = 224,
		SDL_SCANCODE_LSHIFT = 225,
		SDL_SCANCODE_LALT = 226, /**< alt, option */
		SDL_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
		SDL_SCANCODE_RCTRL = 228,
		SDL_SCANCODE_RSHIFT = 229,
		SDL_SCANCODE_RALT = 230, /**< alt gr, option */
		SDL_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

		SDL_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
									 *   by any of the above, but since there's a
									 *   special KMOD_MODE for it I'm adding it here
									 */

		/* @} *//* Usage page 0x07 */

		/**
		 *  \name Usage page 0x0C
		 *
		 *  These values are mapped from usage page 0x0C (USB consumer page).
		 */
		/* @{ */

		SDL_SCANCODE_AUDIONEXT = 258,
		SDL_SCANCODE_AUDIOPREV = 259,
		SDL_SCANCODE_AUDIOSTOP = 260,
		SDL_SCANCODE_AUDIOPLAY = 261,
		SDL_SCANCODE_AUDIOMUTE = 262,
		SDL_SCANCODE_MEDIASELECT = 263,
		SDL_SCANCODE_WWW = 264,
		SDL_SCANCODE_MAIL = 265,
		SDL_SCANCODE_CALCULATOR = 266,
		SDL_SCANCODE_COMPUTER = 267,
		SDL_SCANCODE_AC_SEARCH = 268,
		SDL_SCANCODE_AC_HOME = 269,
		SDL_SCANCODE_AC_BACK = 270,
		SDL_SCANCODE_AC_FORWARD = 271,
		SDL_SCANCODE_AC_STOP = 272,
		SDL_SCANCODE_AC_REFRESH = 273,
		SDL_SCANCODE_AC_BOOKMARKS = 274,

		/* @} *//* Usage page 0x0C */

		/**
		 *  \name Walther keys
		 *
		 *  These are values that Christian Walther added (for mac keyboard?).
		 */
		/* @{ */

		SDL_SCANCODE_BRIGHTNESSDOWN = 275,
		SDL_SCANCODE_BRIGHTNESSUP = 276,
		SDL_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
											   switch, video mode switch */
		SDL_SCANCODE_KBDILLUMTOGGLE = 278,
		SDL_SCANCODE_KBDILLUMDOWN = 279,
		SDL_SCANCODE_KBDILLUMUP = 280,
		SDL_SCANCODE_EJECT = 281,
		SDL_SCANCODE_SLEEP = 282,

		SDL_SCANCODE_APP1 = 283,
		SDL_SCANCODE_APP2 = 284,

		/* @} *//* Walther keys */

		/* Add any other keys here. */

		SDL_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
									 for array bounds */
	} SDL_Scancode;




	enum
	{
		KC_UNKNOWN = 0,

		KC_RETURN = '\r',
		KC_ESCAPE = '\033',
		KC_BACKSPACE = '\b',
		KC_TAB = '\t',
		KC_SPACE = ' ',
		KC_EXCLAM = '!',
		KC_QUOTEDBL = '"',
		KC_HASH = '#',
		KC_PERCENT = '%',
		KC_DOLLAR = '$',
		KC_AMPERSAND = '&',
		KC_QUOTE = '\'',
		KC_LEFTPAREN = '(',
		KC_RIGHTPAREN = ')',
		KC_ASTERISK = '*',
		KC_PLUS = '+',
		KC_COMMA = ',',
		KC_MINUS = '-',
		KC_PERIOD = '.',
		KC_SLASH = '/',
		KC_0 = '0',
		KC_1 = '1',
		KC_2 = '2',
		KC_3 = '3',
		KC_4 = '4',
		KC_5 = '5',
		KC_6 = '6',
		KC_7 = '7',
		KC_8 = '8',
		KC_9 = '9',
		KC_COLON = ':',
		KC_SEMICOLON = ';',
		KC_LESS = '<',
		KC_EQUALS = '=',
		KC_GREATER = '>',
		KC_QUESTION = '?',
		KC_AT = '@',
			/*
			   Skip uppercase letters
			 */
		KC_LEFTBRACKET = '[',
		KC_BACKSLASH = '\\',
		KC_RIGHTBRACKET = ']',
		KC_CARET = '^',
		KC_UNDERSCORE = '_',
		KC_BACKQUOTE = '`',
		KC_A = 'a',
		KC_B = 'b',
		KC_C = 'c',
		KC_D = 'd',
		KC_E = 'e',
		KC_F = 'f',
		KC_G = 'g',
		KC_H = 'h',
		KC_I = 'i',
		KC_J = 'j',
		KC_K = 'k',
		KC_L = 'l',
		KC_M = 'm',
		KC_N = 'n',
		KC_O = 'o',
		KC_P = 'p',
		KC_Q = 'q',
		KC_R = 'r',
		KC_S = 's',
		KC_T = 't',
		KC_U = 'u',
		KC_V = 'v',
		KC_W = 'w',
		KC_X = 'x',
		KC_Y = 'y',
		KC_Z = 'z',

		KC_CAPSLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

		KC_F1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
		KC_F2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
		KC_F3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
		KC_F4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
		KC_F5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
		KC_F6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
		KC_F7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
		KC_F8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
		KC_F9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
		KC_F10 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
		KC_F11 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
		KC_F12 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),

		KC_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
		KC_SCROLLLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
		KC_PAUSE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
		KC_INSERT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
		KC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
		KC_PAGEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
		KC_DELETE = '\177',
		KC_END = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
		KC_PAGEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
		KC_RIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
		KC_LEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
		KC_DOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
		KC_UP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),

		KC_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
		KC_NUM_DIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
		KC_NUM_MULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
		KC_NUM_MINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
		KC_NUM_PLUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
		KC_NUM_ENTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
		KC_NUM_1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
		KC_NUM_2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
		KC_NUM_3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
		KC_NUM_4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
		KC_NUM_5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
		KC_NUM_6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
		KC_NUM_7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
		KC_NUM_8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
		KC_NUM_9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
		KC_NUM_0 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
		KC_NUM_PERIOD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

		KC_APPLICATION = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
		KC_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
		KC_KP_EQUALS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
		KC_F13 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
		KC_F14 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
		KC_F15 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
		KC_F16 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
		KC_F17 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
		KC_F18 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
		KC_F19 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
		KC_F20 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
		KC_F21 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
		KC_F22 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
		KC_F23 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
		KC_F24 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
		KC_EXECUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
		KC_HELP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
		KC_MENU = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
		KC_SELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
		KC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
		KC_AGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
		KC_UNDO = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
		KC_CUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
		KC_COPY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
		KC_PASTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
		KC_FIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
		KC_MUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
		KC_VOLUMEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
		KC_VOLUMEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
		KC_NUM_COMMA = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
		KC_NUM_EQUALSAS400 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

		KC_ALTERASE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
		KC_SYSREQ = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
		KC_CANCEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
		KC_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
		KC_PRIOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
		KC_RETURN2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
		KC_SEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
		KC_OUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
		KC_OPER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
		KC_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
		KC_CRSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
		KC_EXSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

		KC_THOUSANDSSEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
		KC_DECIMALSEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
		KC_CURRENCYUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
		KC_CURRENCYSUBUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
		KC_KP_LEFTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
		KC_KP_RIGHTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
		KC_KP_LEFTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
		KC_KP_RIGHTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
		KC_KP_TAB = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
		KC_KP_BACKSPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
		KC_KP_A = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
		KC_KP_B = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
		KC_KP_C = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
		KC_KP_D = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
		KC_KP_E = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
		KC_KP_F = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
		KC_KP_XOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
		KC_KP_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
		KC_KP_PERCENT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
		KC_KP_LESS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
		KC_KP_GREATER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
		KC_KP_AMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
		KC_KP_DBLAMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
		KC_KP_VERTICALBAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
		KC_KP_DBLVERTICALBAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
		KC_KP_COLON = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
		KC_KP_HASH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
		KC_KP_SPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
		KC_KP_AT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
		KC_KP_EXCLAM = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
		KC_KP_MEMSTORE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
		KC_KP_MEMRECALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
		KC_KP_MEMCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
		KC_KP_MEMADD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
		KC_KP_MEMSUBTRACT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
		KC_KP_MEMMULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
		KC_KP_MEMDIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
		KC_KP_PLUSMINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
		KC_KP_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
		KC_KP_CLEARENTRY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
		KC_KP_BINARY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
		KC_KP_OCTAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
		KC_KP_DECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
		KC_KP_HEXADECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

		KC_LCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
		KC_LSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
		KC_LALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
		KC_LGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
		KC_RCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
		KC_RSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
		KC_RALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
		KC_RGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),

		KC_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

		KC_AUDIONEXT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
		KC_AUDIOPREV = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
		KC_AUDIOSTOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
		KC_AUDIOPLAY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
		KC_AUDIOMUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
		KC_MEDIASELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
		KC_WWW = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
		KC_MAIL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
		KC_CALCULATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
		KC_COMPUTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
		KC_AC_SEARCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
		KC_AC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
		KC_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
		KC_AC_FORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
		KC_AC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
		KC_AC_REFRESH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
		KC_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

		KC_BRIGHTNESSDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
		KC_BRIGHTNESSUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
		KC_DISPLAYSWITCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
		KC_KBDILLUMTOGGLE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
		KC_KBDILLUMDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
		KC_KBDILLUMUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
		KC_EJECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
		KC_SLEEP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP)
	};

};