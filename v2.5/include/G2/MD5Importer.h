// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Importer.h"
#include "MD5Mesh.h"

namespace G2 
{
// DATATYPES ONLY NEEDED FOR ANIMATION IMPORT!!!

/* Joint info */
struct AnimationJointInfo {
	char name[64];
	int parent;
	int flags;
	int startIndex;
};

/* Base frame joint */
struct AnimationBaseFrameJoint {
	glm::vec3 pos;
	glm::quat orient;
};
// END

	/** This class implements the generic resource importer interface
	 * to provide an importer for MD5Mesh objects from files.
	 * The MD5Importer uses caching to not load files more then once.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class MD5Importer : public Importer<MD5Importer,MD5Mesh, MD5Mesh::Builder>
	{
		public:
			/** This function will load and import the given MD5 files
			 * @note Don't use this function directly, use import() instead!
			 * @param meshFileName The filename of the MD5-Mesh file to import
			 * @param importNormals Flag indicating whether you want the resulting mesh to import normals if available.
			 * @param importNormals Flag indicating whether you want the resulting mesh to import texture coordinates if available.
			 * @param importNormals Flag indicating whether you want the resulting mesh to import animations if available.
			 * @param animationFiles A vector containing all MD5-Animation files
			 * to also import.
			 * @return a shared pointer pointing to the MD5Mesh or nullptr, if some error occurred.
			 * @note Requesting one mesh multiple times will result in cache hits.
			 */
			std::shared_ptr<MD5Mesh> importResource(std::string const& meshFileName, 
													bool importNormals = true, bool importTexCoords = true, bool importAnimations = true,
													std::vector<std::string> const& animationFiles = std::vector<std::string>());
			std::pair<std::string,std::shared_ptr<MD5Mesh::Builder>> produceResourceBuilder(std::string const& fileName,
													bool importNormals = true, bool importTexCoords = true, bool importAnimations = true,
													std::vector<std::string> const& animationFiles = std::vector<std::string>());

		private:
			/** This function imports a MD5-Mesh file and writes the data into the given Builder.
			 * @return True if no error occurred, false if some error occurred.
			 */
			bool _importMesh(std::shared_ptr<MD5Mesh::Builder> builder, FILE* file);
			/** This function prepares the normals and vertices in the given Builder
			 * to represent the MD5Mesh in it's base pose contained in the Builder.
			 */
			void _createBasePose(std::shared_ptr<MD5Mesh::Builder> const& builder);
			/** This function imports a MD5-Animation file and writes the data into the given Builder.
			 */
			void _importAnimation(std::shared_ptr<MD5Mesh::Builder> builder, FILE* file);

			void _buildFrameSkeleton(const struct AnimationJointInfo* jointInfos, 
									 const struct AnimationBaseFrameJoint* baseFrame, 
									 const float* animFrameData, 
									 SkeletonFrame& skeletonFrames, 
									 int numJoints);

			FILE* open(std::string const& fileName);
			void close(FILE* file);
	};
};