#pragma once
#include "RenderStates.h"

#include <vector>

namespace G2 
{
	/** This class defines...
	 * @created:	2014/06/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class RenderStatesGroup 
	{
		public:
			/// This constructs a new RenderStatesGroup.
			RenderStatesGroup();
			/// This constructs a new RenderStatesGroup.
			RenderStatesGroup(unsigned int entityId, RenderStates const& renderStates);
			/// Copy constructor.
			RenderStatesGroup(RenderStatesGroup const& rhs);
			/// Move constructor.
			RenderStatesGroup(RenderStatesGroup && rhs);
			/// Basic assignment operator.
			RenderStatesGroup& operator=(RenderStatesGroup const& rhs);
			/// Basic move operator.
			RenderStatesGroup& operator=(RenderStatesGroup && rhs);

			bool consume(unsigned int entityId, RenderStates const& states);
			bool erase(unsigned int entityId);

			std::vector<unsigned int> const& getEntityIds() const { return mEntityIds; }
			/** This function will return the RenderStates. 
			* @return The RenderStates.
			*/
			G2::RenderStates const& getRenderStates() const { return mRenderStates; }

		private:

			RenderStates				mRenderStates;
			std::vector<unsigned int>	mEntityIds;
	};
};