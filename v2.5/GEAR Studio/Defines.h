#pragma once

/** This is a helper macro for suppressing signals while invoking a certain action.
 * @param w The QObject pointer to block the signals for.
 * @param a The action to perform.
 */
#define GEAR_QTNOEVENT(w,a) \
w->blockSignals(true); \
a; \
w->blockSignals(false);