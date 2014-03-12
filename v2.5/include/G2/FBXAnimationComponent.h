// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseComponent.h"
#include "FBXAnimationSystem.h"
#include "FBXMesh.h"
#include "FBXAnimationData.h"
#include "FBXAnimationState.h"

#define USE_META_DATA 1

namespace G2 
{
	/** This struct contains the meta data for animating an FBXMesh.
	 * It is stored in the user data pointer of the FbxMesh of the FBX SDK 
	 * With this meta data, a connection between an FbxMesh and a VAO is created.
	 */
	struct FbxMetaData {
		FbxMetaData(unsigned int vaoOffset)
			: vaoOffset(vaoOffset)
		{ }
		//unsigned int metaDataType; // later
		unsigned int vaoOffset;
		bool		 allByControlPoint; 

		std::vector<glm::vec4> vertexCache;	// used while animation
	};

	/** This class holds all informations needed to animate a FBXMesh
	 * @created:	2014/02/19
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class FBXAnimationComponent : public BaseComponent<FBXAnimationSystem>
	{
		
			friend class FBXAnimationSystem;
			friend struct FBXMesh::Builder;

		public:
			/** This constructs a new MD5AnimationComponent.
			 */
			FBXAnimationComponent();
			/** This constructs a new MD5AnimationComponent.
			 */
			FBXAnimationComponent(FbxScene* fbxScene, 
								  FBXAnimationState const& animationState,
								  FBXAnimationData const& animationData);
			
			FBXAnimationComponent(FBXAnimationComponent && rhs);
			FBXAnimationComponent& operator=(FBXAnimationComponent && rhs);

			void tempSetPoseIndex(unsigned int poseIndex);


		private:
			// we only support move semantic!
			FBXAnimationComponent(FBXAnimationComponent const&) {}
			// we only support move semantic!
			FBXAnimationComponent& operator=(FBXAnimationComponent const&) { return *this; }
			
			FBXAnimationState	animationState;			// The state which drives the animation of the FBXMesh
			FBXAnimationData	animationData;				// The data needed to animate the FBXMesh
			FbxScene*			fbxScene;						// The FBX Scene holding all the relevant data - the FBXMesh::Builder takes care of deleting this pointer
			
			

	};
};