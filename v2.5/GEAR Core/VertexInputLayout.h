#pragma once
#include "Defines.h"

#include <vector>

namespace G2Core
{
	struct VertexInputElement
	{
		Semantics::Name semantic;		// defined by shader
		unsigned int semanticIndex;		// defined by vertex array
		SemanticDataTypes::Name format; // defined by vertex array
		unsigned int inputSlot;			// defined by vertex array
		unsigned int alignedByteOffset; // defined by vertex array
	};

	struct VertexInputLayout
	{
		std::vector<VertexInputElement> elements;
	};
};