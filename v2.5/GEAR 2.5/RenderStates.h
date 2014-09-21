#pragma once
#include "Defines.h"

#include <G2Core/Defines.h>

namespace G2 
{
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
			G2Core::PolygonDrawMode::Name const& getPolygonDrawMode() const { return mPolygonDrawMode; }
			/** This function will set the PolygonDrawMode to the given value.
			* @param value The PolygonDrawMode.
			*/
			void setPolygonDrawMode(G2Core::PolygonDrawMode::Name const& value) { mPolygonDrawMode = value; }
			/** This function will return the FaceCulling. 
			* @return The FaceCulling.
			*/
			G2Core::FaceCulling::Name const& getFaceCulling() const { return mFaceCulling; }
			/** This function will set the FaceCulling to the given value.
			* @param value The FaceCulling.
			*/
			void setFaceCulling(G2Core::FaceCulling::Name const& value) { mFaceCulling = value; }
			/** This function will return the SourceFactor. 
			* @return The SourceFactor.
			*/
			G2Core::BlendFactor::Name const& getSourceFactor() const { return mSourceFactor; }
			/** This function will set the SourceFactor to the given value.
			* @param value The SourceFactor.
			*/
			void setSourceFactor(G2Core::BlendFactor::Name const& value) { mSourceFactor = value; }
			/** This function will return the DestinationFactor. 
			* @return The DestinationFactor.
			*/
			G2Core::BlendFactor::Name const& getDestinationFactor() const { return mDestinationFactor; }
			/** This function will set the DestinationFactor to the given value.
			* @param value The DestinationFactor.
			*/
			void setDestinationFactor(G2Core::BlendFactor::Name const& value) { mDestinationFactor = value; }
			/** This function will apply all states to the renderer.
			 * @note Try to minimize the amount of calls to this function, since it triggers a bunch of state changes to the renderer.
			 */
			void applyStates(bool inPass) const;
		private:

			G2Core::FaceCulling::Name		mFaceCulling;			// Default: FaceCulling::BACK_FACE
			G2Core::PolygonDrawMode::Name	mPolygonDrawMode;		// Default: PolygonDrawMode::FILL
			float							mPolygonOffsetFactor;	// glPolygonOffset(X). Default: 0.0
			float							mPolygonOffsetUnits;	// glPolygonOffset(,X). Default: 0.0
			
			G2Core::BlendFactor::Name		mSourceFactor;			// The sFactor of the blend function
			G2Core::BlendFactor::Name		mDestinationFactor;		// The dFactor of the blend function
	};
};