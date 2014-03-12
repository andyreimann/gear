#pragma once
#include "ResourceBuilder.h"

namespace G2 
{
	/// This class holds all informations needed to describe and use an
	/// effect.
	/// @created:	2014/03/11
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class Effect 
	{
		public:

			struct Builder : public ResourceBuilder<Builder,Effect>
			{

				std::shared_ptr<Effect> buildResource();
				~Builder();

				unsigned id; // The IL image id
			};
			/// This constructs a new Effect.
			Effect();
			/// Copy constructor.
			Effect(Effect const& rhs);
			/// Basic assignment operator.
			Effect& operator=(Effect const& rhs);
			/// normal destructor
			~Effect();
		protected:
		private:
	};
};