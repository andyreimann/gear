// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

namespace G2 
{
	/** A FrameInfo object holds all needed informations about a frame, which is rendered.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	struct FrameInfo 
	{
		/** Creates a new default FrameInfo object.
		 */
		COREDLL_API FrameInfo()
			: frame(0),
			timeSinceRenderStart(0.0),
			timeSinceLastFrame(0.0),
			stopRenderingAfterThisFrame(false) {}

		long	frame;								// the current frame
		double	timeSinceRenderStart;				// The time in seconds since the start of the rendering
		double	timeSinceLastFrame;					// The time in seconds elapsed since the last frame
		mutable bool stopRenderingAfterThisFrame;	// Indicates whether the engine should stop rendering after this frame.
	};
};