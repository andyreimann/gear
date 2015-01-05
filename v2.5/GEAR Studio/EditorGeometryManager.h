#pragma once
#include "EditorGrid.h"

#include <G2Core/Entity.h>
#include <G2/EffectImporter.h>
#include <G2Core/FrameInfo.h>

#include <list>

/** This class is responsible for the editorial content the user may interact with.
 * @created:	2014/01/05
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class EditorGeometryManager 
{
	public:
		/// This constructs a new EditorGeometryManager.
		EditorGeometryManager();

		~EditorGeometryManager();
	private:

		void _updateEditorGrid(G2::FrameInfo const& frame);

		G2::EffectImporter  mEffectImporter;
		std::shared_ptr<G2::Effect>	mSolidFx;			// The effect for solid ambient lighting

		std::list<EditorGrid>	mGrids;

};