#pragma once

/** This is a helper macro for suppressing signals while invoking a certain action.
 * @param w The QObject pointer to block the signals for.
 * @param a The action to perform.
 */
#define GEAR_QTNOEVENT(w,a) \
w->blockSignals(true); \
a; \
w->blockSignals(false);

namespace G2S
{
	namespace HandleMode
	{
		enum Name
		{
			NO_HANDLE = 0,
			TRANSLATION_HANDLE = 1,
			SCALE_HANDLE = 2,
			ROTATION_HANDLE = 3,
			HANDLE_INVALID = 4,
		};
	};

};
# define G2S_PI           3.14159265358979323846  /* pi */