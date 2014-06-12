#pragma once
#include "EditorSystem.h"

#include <G2Core/BaseComponent.h>

namespace G2 
{
	namespace Editor 
	{
		/// This class defines...
		/// @created:	2014/06/12
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class EditorComponent : public BaseComponent<EditorSystem>
		{
			public:

				EDITORDLL_API EditorComponent() : targetEntityId(0) {}

				unsigned int targetEntityId; // The entity id of the entity, this EditorComponent adds editor content observs

			private:
		};
	};
};