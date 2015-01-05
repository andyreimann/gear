#pragma once
#include <G2Core/Entity.h>
#include <G2/EffectImporter.h>
#include <G2Core/FrameInfo.h>

#include <unordered_map>

/** This class is responsible for the editorial content the user may interact with.
 * @created:	2014/01/05
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class EditorGrid
{
	public:
		EditorGrid() {}
		/// This constructs a new EditorGeometryManager.
		EditorGrid(std::shared_ptr<G2::Effect> effect, float unitSize);
		/** Move ctor.
		*/
		EditorGrid(EditorGrid && rhs);
		/** Move assignment operator.
		*/
		EditorGrid& operator=(EditorGrid && rhs);

		operator G2::Entity*() { return &mEditorGrid;  }

		float getUnitSize() const { return mUnitSize; }
	private:

		float				mUnitSize;					// The size of one unit of the grid
		G2::Entity			mEditorGrid;				// The entity of the editor grid in 3D
};