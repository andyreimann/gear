#pragma once
#include "Defines.h"

#include <vector>

namespace G2Core
{
	struct VertexInputElement
	{
		Semantics::Name semantic;
		//unsigned int semanticIndex;
		SemanticDataTypes::Name format;
		//unsigned int inputSlot;
		//unsigned int alignedByteOffset;
	};

	struct VertexInputLayout
	{
		std::vector<VertexInputElement> elements;
	};
};