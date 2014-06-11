#pragma once

namespace G2 
{
	/// This class defines...
	/// @created:	2014/06/10
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	template<typename CONCRETE_PARSER, typename RESOURCE_TYPE, typename RESOURCE_BUILDER_TYPE>
	class BlockParser 
	{
		public:
			/// This constructs a new BlockParser.
			BlockParser();
			/// Copy constructor.
			BlockParser(BlockParser const& rhs);
			/// Move constructor.
			BlockParser(BlockParser && rhs);
			/// Basic assignment operator.
			BlockParser& operator=(BlockParser const& rhs);
			/// Basic move operator.
			BlockParser& operator=(BlockParser && rhs);
			/// normal destructor
			~BlockParser();
		protected:
		private:
	};
};