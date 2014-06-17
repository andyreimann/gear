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
	namespace BlendFactor
	{
		enum Name {
			ONE = GL_ONE, // OpenGL default
			ZERO = GL_ZERO,
			SRC_COLOR = GL_SRC_COLOR,
			ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
			DST_COLOR = GL_DST_COLOR,
			ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
			SRC_ALPHA = GL_SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
			DST_ALPHA = GL_DST_ALPHA,
			ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
			CONSTANT_COLOR = GL_CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
			SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
			SRC1_COLOR = GL_SRC1_COLOR,
			ONE_MINUS_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
			SRC1_ALPHA = GL_SRC1_ALPHA,
			ONE_MINUS_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA,
		};
	};
	/** This class holds all different states passed to the rendering state machine
	 * while doing the rendering for meshes.
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
			/** This function will return the SourceFactor. 
			* @return The SourceFactor.
			*/
			BlendFactor::Name const& getSourceFactor() const { return mSourceFactor; }
			/** This function will set the SourceFactor to the given value.
			* @param value The SourceFactor.
			*/
			void setSourceFactor(BlendFactor::Name const& value) { mSourceFactor = value; }
			/** This function will return the DestinationFactor. 
			* @return The DestinationFactor.
			*/
			BlendFactor::Name const& getDestinationFactor() const { return mDestinationFactor; }
			/** This function will set the DestinationFactor to the given value.
			* @param value The DestinationFactor.
			*/
			void setDestinationFactor(BlendFactor::Name const& value) { mDestinationFactor = value; }
			/** This function will apply all states to the renderer.
			 * @note Try to minimize the amount of calls to this function, since it triggers a bunch of state changes to the renderer.
			 */
			void applyStates(bool inPass) const;
		private:

			FaceCulling::Name		mFaceCulling;			// glCullFace. Default: FaceCulling::BACK_FACE
			PolygonDrawMode::Name	mPolygonDrawMode;		// glPolygonMode. Default: PolygonDrawMode::FILL
			float					mPolygonOffsetFactor;	// glPolygonOffset(X). Default: 0.0
			float					mPolygonOffsetUnits;	// glPolygonOffset(,X). Default: 0.0
			
			BlendFactor::Name		mSourceFactor;			// The sFactor of the blend function
			BlendFactor::Name		mDestinationFactor;		// The dFactor of the blend function
	};
};