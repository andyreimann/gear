// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseComponent.h"
#include "MD5AnimationSystem.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace G2 
{	
	struct Joint { 
		std::string			name;	//!< The name of the bone
		glm::vec3			position;
		glm::quat			orientation;
		int					parentIndex;
	};

	struct Weight {
		int jointIndex;
		float bias;

		glm::vec3 position;
		glm::vec3 normal;
	};

	struct VertexWeights {
		int startWeightIndex;
		int numWeightIndices;
	};
	
	struct SkeletonFrame 
	{
		std::vector<Joint>	joints;
	};

	struct SkeletonFrames 
	{
		std::vector<SkeletonFrame>	frames;
	};

	struct Skeleton
	{
		std::vector<Joint>	joints;
	};

	// this data is needed during animation
	struct MeshAnimationData
	{
		std::vector<Weight>				weights;
		std::vector<VertexWeights>		vertexWeights;
		
		std::vector<glm::vec3>			vertexCache;
		std::vector<glm::vec3>			normalCache;
	};

	struct AnimationData
	{
		AnimationData()
			: numFrames(0),
			numJoints(0),
			frameRate(24.0) {}
		int				numFrames; 
		int				numJoints; 
		double			frameRate; 

		std::vector<MeshAnimationData> meshAnimationData;

		SkeletonFrames	skeletonFrames; // all available static frames of the skeleton
	};

	struct AnimationState 
	{
		unsigned int	animMode;
		bool			animated;

		int	maxFrame;
		int	startFrame;
		int	currentFrame;
		int	nextFrame;

		double lastTime;
		double invFrameRate;		// The inverse frame rate
	};

	/** This class holds all informations needed to animate a MD5Mesh
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class MD5AnimationComponent : public BaseComponent<MD5AnimationSystem>
	{
		public:
			/** This constructs a new MD5AnimationComponent.
			 */
			MD5AnimationComponent();
			/** This constructs a new MD5AnimationComponent with the given 
			 * number of joints for the skeleton.
			 * @param numJointsInSkeleton The number of joints, the animated skeleton will have.
			 */
			MD5AnimationComponent(unsigned int numJointsInSkeleton);

			MD5AnimationComponent(MD5AnimationComponent && rhs);
			MD5AnimationComponent& operator=(MD5AnimationComponent && rhs);
			
			AnimationData	animationData;
			AnimationState	animationState;

			SkeletonFrame	interpolatedSkeleton;	// The skeleton, which is interpolated during animation
		private:
			// we only support move semantic!
			MD5AnimationComponent(MD5AnimationComponent const& rhs) {}
			// we only support move semantic!
			MD5AnimationComponent& operator=(MD5AnimationComponent const& rhs) { return *this; }


			// Animation data
	};
};
