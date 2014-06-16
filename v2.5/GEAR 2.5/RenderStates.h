#pragma once
#include "Defines.h"

namespace G2 
{
	namespace FaceCulling
	{
		enum Name {
			BACK_FACE = GL_BACK,
			FRONT_FACE = GL_FRONT,
			FRONT_AND_BACK_FACE = GL_FRONT_AND_BACK,
		};
	};
	namespace PolygonDrawMode
	{
		enum Name {
			FILL = GL_FILL,
			LINE = GL_LINE,
			POINT = GL_POINT,
		};
	};
	/** This class holds all different states passed to the rendering state machine
	 * while doing the normal rendering for meshes. 
	 * @note RenderStates are only used for doing the normal rendering of a mesh.
	 * @created:	2014/06/16
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderStates 
	{
		public:
			/// This constructs a new RenderStates.
			RenderStates();
			/// Copy constructor.
			RenderStates(RenderStates const& rhs);
			/// Move constructor.
			RenderStates(RenderStates && rhs);
			/// Basic assignment operator.
			RenderStates& operator=(RenderStates const& rhs);
			/// Basic move operator.
			RenderStates& operator=(RenderStates && rhs);
			/// Basic assignment operator.
			bool operator==(RenderStates const& rhs);
			
			/** This function will return the PolygonOffsetUnits. 
			* @return The PolygonOffsetUnits.
			*/
			float const& getPolygonOffsetUnits() const { return mPolygonOffsetUnits; }
			/** This function will set the PolygonOffsetUnits to the given value.
			* @param value The PolygonOffsetUnits.
			*/
			void setPolygonOffsetUnits(float const& value) { mPolygonOffsetUnits = value; }
			/** This function will return the PolygonOffsetFactor. 
			* @return The PolygonOffsetFactor.
			*/
			float const& getPolygonOffsetFactor() const { return mPolygonOffsetFactor; }
			/** This function will set the PolygonOffsetFactor to the given value.
			* @param value The PolygonOffsetFactor.
			*/
			void setPolygonOffsetFactor(float const& value) { mPolygonOffsetFactor = value; }
			/** This function will return the PolygonDrawMode. 
			* @return The PolygonDrawMode.
			*/
			PolygonDrawMode::Name const& getPolygonDrawMode() const { return mPolygonDrawMode; }
			/** This function will set the PolygonDrawMode to the given value.
			* @param value The PolygonDrawMode.
			*/
			void setPolygonDrawMode(PolygonDrawMode::Name const& value) { mPolygonDrawMode = value; }
			/** This function will return the FaceCulling. 
			* @return The FaceCulling.
			*/
			FaceCulling::Name const& getFaceCulling() const { return mFaceCulling; }
			/** This function will set the FaceCulling to the given value.
			* @param value The FaceCulling.
			*/
			void setFaceCulling(FaceCulling::Name const& value) { mFaceCulling = value; }
		private:

			FaceCulling::Name		mFaceCulling;			// glCullFace. Default: FaceCulling::BACK_FACE
			PolygonDrawMode::Name	mPolygonDrawMode;		// glPolygoneMode. Default: PolygonDrawMode::FILL
			float					mPolygonOffsetFactor;	// glPolygonOffset(X). Default: 0.0
			float					mPolygonOffsetUnits;	// glPolygonOffset(,X). Default: 0.0
	};
};