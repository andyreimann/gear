#include "EditorGrid.h"

#include <G2/RenderComponent.h>

EditorGrid::EditorGrid(std::shared_ptr<G2::Effect> effect, float unitSize) :
	mUnitSize(unitSize)
{
	auto* grid = mEditorGrid.addComponent<G2::RenderComponent>(G2Core::RenderLayer::LAYER_30);

	grid->allocateVertexArrays(1);

	std::vector<glm::vec3> gridVertices;


	int numLinesPerPlane = 401;
	float numExtends = ((numLinesPerPlane - 1) / 2);

	for (int start = 0 - numExtends; start <= numExtends; ++start)
	{
		// line in Z-Direction
		gridVertices.push_back(glm::vec3(start * mUnitSize, 0.f, numExtends * mUnitSize));
		gridVertices.push_back(glm::vec3(start * mUnitSize, 0.f, -numExtends * mUnitSize));
		// line in X-Direction
		gridVertices.push_back(glm::vec3(numExtends * mUnitSize, 0.f, start * mUnitSize));
		gridVertices.push_back(glm::vec3(-numExtends * mUnitSize, 0.f, start * mUnitSize));
	}
	auto& gridVao = grid->getVertexArray(0);
	gridVao.resizeElementCount((int)gridVertices.size());
	gridVao.writeData(G2Core::Semantics::POSITION, &gridVertices[0], (int)gridVertices.size());

	grid->addDrawCall(G2::DrawCall().setDrawMode(G2Core::DrawMode::LINES).setVaoIndex(0));
	grid->setEffect(effect);

	grid->material.setAmbient(glm::vec4(0.3f, 0.3f, 0.4f, 1.f));
}

EditorGrid::EditorGrid(EditorGrid && rhs)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

EditorGrid& EditorGrid::operator=(EditorGrid && rhs)
{
	mUnitSize = rhs.mUnitSize;
	mEditorGrid = std::move(rhs.mEditorGrid);

	rhs.mUnitSize = 0.f;

	return *this;
}