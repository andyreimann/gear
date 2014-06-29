// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MD5Importer.h"
#include "Logger.h"
#include "Defines.h"

#ifdef _MSC_VER
	#pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
#endif

using namespace G2;

std::shared_ptr<MD5Mesh>
MD5Importer::importResource(std::string const& fileName,
							bool importNormals, bool importTexCoords, bool importAnimations,
							std::vector<std::string> const& animationFiles) 
{
	
	auto it = mCache.find(fileName);
	if(it != mCache.end())
	{
		// cache hit
		return it->second->build(importNormals, importTexCoords, importAnimations);
	}
	// should never occur
	return std::shared_ptr<MD5Mesh>();
}

std::pair<std::string,std::shared_ptr<MD5Mesh::Builder>> 
MD5Importer::produceResourceBuilder(std::string const& meshFileName,
							bool importNormals, bool importTexCoords, bool importAnimations,
							std::vector<std::string> const& animationFiles) 
{
	if(isCached(meshFileName))
	{
		return std::make_pair(meshFileName,std::shared_ptr<MD5Mesh::Builder>());
	}
	logger << "[MD5Importer] Import MD5 file " << meshFileName << endl;

	// Step 1: create builder and fill
	std::shared_ptr<MD5Mesh::Builder> builder = std::shared_ptr<MD5Mesh::Builder>(new MD5Mesh::Builder);

	int currentMesh = 0;
  
	FILE* fp = open(meshFileName);
	//FILE* fp2 = open(animationFileName);
	
	if(!fp) 
	{
		logger << "[MD5Importer] Error: MD5-Mesh file '" << meshFileName << "' not found!" << endl;
		return std::make_pair(meshFileName, std::shared_ptr<MD5Mesh::Builder>());
	}
	if(!_importMesh(builder, fp))
	{
		// some error occurred
		close(fp);
		return std::make_pair(meshFileName, std::shared_ptr<MD5Mesh::Builder>());
	}
	close(fp);

	// build the default pose using the base skeleton from the mesh file
	_createBasePose(builder);

	for (int i = 0; i < animationFiles.size() ; ++i) 
	{
		FILE* fp2 = open(animationFiles[0]);
		if(!fp2) 
		{
			logger << "[MD5Importer] Warning: MD5-Animation file '" << animationFiles[0] << "' not found!" << endl;
			continue;
		}
		_importAnimation(builder, fp2);
		close(fp2);
	}

	size_t pos = meshFileName.find_last_of("/");
	if( pos != std::string::npos)
	{
		builder->name = meshFileName.substr(pos+1);
	}
	else 
	{
		pos = meshFileName.find_last_of("\\");
		if( pos != std::string::npos)
		{
			builder->name = meshFileName.substr(pos+1);
		}
		else 
		{
			builder->name = meshFileName;
		}
	}

	return std::make_pair(meshFileName, builder);
}

bool
MD5Importer::_importMesh(std::shared_ptr<MD5Mesh::Builder> builder, FILE* file) 
{
	// create some global vars
	char buff[512];
	int version;
	int currentMesh = 0;
	int i;
	int numSubmeshes = 0;
	int numJoints = 0;

	while (!feof (file)) 
	{
		/* Read whole line */
		fgets (buff, sizeof (buff), file);

		if (sscanf (buff, " MD5Version %d", &version) == 1) 
		{
			logger << "[MD5Importer] MD5-Mesh file version: " << version << endl;
			//if (!validateVersion(version)) {
			//	return SmartPointer<Spatial>();
			//}
		}
		else if (sscanf (buff, " numJoints %d", &numJoints) == 1) 
		{
			if (numJoints > 0) 
			{
				/* Allocate memory for base skeleton joints */
				logger << "[MD5Importer] Num joints: " << numJoints << endl;
				builder->baseSkeleton.joints.resize(numJoints);
			}
		}
		else if (sscanf (buff, " numMeshes %d", &numSubmeshes) == 1) 
		{
			if (numSubmeshes > 0) 
			{
				/* Allocate memory for meshes */
				logger << "[MD5Importer] Num meshes: " << numSubmeshes << endl;
				builder->subMeshes.resize(numSubmeshes);
			}
		}
		else if (strncmp (buff, "joints {", 8) == 0) 
		{
			/* Read each joint */
			for (i = 0; i < numJoints; ++i) 
			{
				Joint& joint = builder->baseSkeleton.joints[i];

				/* Read whole line */
				fgets (buff, sizeof (buff), file);
				
				char name[64];
					
				if (sscanf (buff, "%s %d ( %f %f %f ) ( %f %f %f )",
					name, &joint.parentIndex, &joint.position.x,
					&joint.position.y, &joint.position.z, &joint.orientation.x,
					&joint.orientation.y, &joint.orientation.z) == 8) 
					{
						joint.name = std::string(name);
						// Compute the w component of the quaternion
						float w = 1.0f - (joint.orientation.x * joint.orientation.x) - (joint.orientation.y * joint.orientation.y) - (joint.orientation.z * joint.orientation.z);

						if (w < 0.0f)
							joint.orientation.w = 0.0f;
						else
							joint.orientation.w = -std::sqrt(w);
				}
			}
		}
		else if (strncmp (buff, "mesh {", 6) == 0) 
		{
			int numVertices = 0;
			int numTriangles = 0;
			int numWeights = 0;
			int vertexIndex = 0;
			int triangleIndex = 0;
			int weightIndex = 0;
			float fdata[4];
			int idata[3];
				
			std::vector<glm::vec3>&					vertices = builder->subMeshes[currentMesh].vertices;
			std::vector<unsigned int>&				indices = builder->subMeshes[currentMesh].indices;
			std::vector<glm::vec3>&					normals = builder->subMeshes[currentMesh].normals;
			std::vector<glm::vec3>&					uvs = builder->subMeshes[currentMesh].uvs;
			std::vector<VertexWeights>&	vertexWeights = builder->subMeshes[currentMesh].vertexWeights;
			std::vector<Weight>&			weights = builder->subMeshes[currentMesh].weights;

			while ((buff[0] != '}') && !feof (file)) 
			{
				/* Read whole line */
				fgets (buff, sizeof (buff), file);

				if (strstr (buff, "shader ")) 
				{
					int quote = 0, j = 0;

					/* Copy the shader name without the quote marks */
					/*
					for (i = 0; i < sizeof (buff) && (quote < 2); ++i) 
					{
						if (buff[i] == '\"')
							quote++;

						if ((quote == 1) && (buff[i] != '\"'))
						{
							// no shader support up to now
							//mesh->shader[j] = buff[i];
							j++;
						}
					}*/
					//mesh->shader[j] = '\0';
				}
				else if (sscanf (buff, " numverts %d", &numVertices) == 1) {
					if (numVertices > 0) {
						/* Allocate memory for vertices */
						vertices.resize(numVertices);
						normals.resize(numVertices);
						uvs.resize(numVertices);
						vertexWeights.resize(numVertices);
					}
				}
				else if (sscanf (buff, " numtris %d", &numTriangles) == 1) {
					if (numTriangles > 0) {
						/* Allocate memory for triangles */
						indices.resize(numTriangles*3);
					}
				}
				else if (sscanf (buff, " numweights %d", &numWeights) == 1) {
					if (numWeights > 0) {
						/* Allocate memory for vertex weights */
						weights.resize(numWeights);
					}
				}
				else if (sscanf (buff, " vert %d ( %f %f ) %d %d", &vertexIndex,
						&fdata[0], &fdata[1], &idata[0], &idata[1]) == 5) {
						uvs[vertexIndex].x = fdata[0];
						uvs[vertexIndex].y = 1.0f - fdata[1];
						vertexWeights[vertexIndex].startWeightIndex = idata[0];
						vertexWeights[vertexIndex].numWeightIndices = idata[1];
				}
				else if (sscanf (buff, " tri %d %d %d %d", &triangleIndex,
						&idata[0], &idata[1], &idata[2]) == 4) {
					/* Copy triangle data */
					int index = triangleIndex * 3;
					indices[index] = idata[0];
					indices[index+1] = idata[2];
					indices[index+2] = idata[1];
				}
				else if (sscanf (buff, " weight %d %d %f ( %f %f %f )",
						&weightIndex, &idata[0], &fdata[3],
						&fdata[0], &fdata[1], &fdata[2]) == 6) {
					/* Copy vertex data */
					weights[weightIndex].jointIndex = idata[0];
					weights[weightIndex].bias   = fdata[3];
					weights[weightIndex].position.x = fdata[0];
					weights[weightIndex].position.y = fdata[1];
					weights[weightIndex].position.z = fdata[2];
				}
			}
			currentMesh++;

			// create real geometry object and connect it to the animation
			/*std::stringstream name;
			name << file << "/" << currentMesh;
			SmartPointer<Spatial> triangleMesh = SmartPointer<Spatial>(new TriMesh(name.str(), vertices, normals, uvs, indices, weights, vertexWeights));
			rootSpatialNode->add(triangleMesh);
			animation->add(triangleMesh);*/
		}
	}
	return true;
}

void
MD5Importer::_importAnimation(std::shared_ptr<MD5Mesh::Builder> builder, FILE* file) 
{
	// create some global vars
	int numFrames = 0;
	int numJoints = 0;
	float frameRate = 24.0;

	char buff[512];
	struct AnimationJointInfo *jointInfos = NULL;
	struct AnimationBaseFrameJoint *baseFrame = NULL;
	float *animFrameData = NULL;
	int version;
	int numAnimatedComponents;
	int frameIndex;
	int i;

	while (!feof (file)) 
	{
		fgets (buff, sizeof (buff), file);

		if (sscanf (buff, " MD5Version %d", &version) == 1) 
		{
			logger << "[MD5Importer] MD5-Animation file version: " << version << endl;
			//if (!validateVersion(version)) {
			//	return;
			//}
		}
		else if (sscanf (buff, " numFrames %d", &numFrames) == 1) 
		{
			// Allocate memory for skeleton frames and bounding boxes 
			if (numFrames > 0) 
			{
				builder->animationData.numFrames = numFrames;
				builder->animationData.skeletonFrames.frames.resize(numFrames);
				//animation.bboxes = new MD5BBox[animation.numFrames];
			}
		}
		else if (sscanf (buff, " numJoints %d", &numJoints) == 1) 
		{
			if (numJoints > 0) 
			{

				builder->animationData.numJoints = numJoints;
				unsigned int numFrames = (unsigned int)builder->animationData.numFrames;
				for(unsigned int i = 0; i < numFrames; ++i) 
				{
					// Allocate memory for joints of each frame 
					builder->animationData.skeletonFrames.frames[i].joints.resize(builder->animationData.numJoints);
				}
				//mAnimatedSkeleton_.resize(mNumJoints_); LATER!!!!

				// Allocate temporary memory for building skeleton frames 
				jointInfos = new AnimationJointInfo[numJoints];

				baseFrame = new AnimationBaseFrameJoint[numJoints];
			}
		}
		else if (sscanf (buff, " frameRate %d", &frameRate) == 1) 
		{
			builder->animationData.frameRate = frameRate;
		}
		else if (sscanf (buff, " numAnimatedComponents %d", &numAnimatedComponents) == 1) 
		{
			if (numAnimatedComponents > 0) 
			{
				// Allocate memory for animation frame data
				animFrameData = new float[numAnimatedComponents];
			}
		}
		else if (strncmp (buff, "hierarchy {", 11) == 0)
		{
			for (i = 0; i < numJoints; ++i)
			{

				fgets (buff, sizeof (buff), file);


				sscanf (buff, " %s %d %d %d", jointInfos[i].name, &jointInfos[i].parent,
					&jointInfos[i].flags, &jointInfos[i].startIndex);
			}
		}
		else if (strncmp (buff, "bounds {", 8) == 0)
		{
			for (i = 0; i < numFrames; ++i) 
			{

				fgets (buff, sizeof (buff), file);


				//sscanf (buff, " ( %f %f %f ) ( %f %f %f )",
				//	&animation.bboxes[i].min.x, &animation.bboxes[i].min.y,
				//	&animation.bboxes[i].min.z, &animation.bboxes[i].max.x,
				//	&animation.bboxes[i].max.y, &animation.bboxes[i].max.z);
			}
		}
		else if (strncmp (buff, "baseframe {", 10) == 0) 
		{
			for (i = 0; i < numJoints; ++i)
			{
				fgets (buff, sizeof (buff), file);

				if (sscanf (buff, " ( %f %f %f ) ( %f %f %f )",
					&baseFrame[i].pos.x, &baseFrame[i].pos.y,
					&baseFrame[i].pos.z, &baseFrame[i].orient.x,
					&baseFrame[i].orient.y, &baseFrame[i].orient.z) == 6) 
				{
						// Compute the w component of the quaternion
						float t = 1.0f - (baseFrame[i].orient.x * baseFrame[i].orient.x) - (baseFrame[i].orient.y * baseFrame[i].orient.y) - (baseFrame[i].orient.z * baseFrame[i].orient.z);

						if (t < 0.0f)
							baseFrame[i].orient.w = 0.0f;
						else
							baseFrame[i].orient.w = -sqrt(t);
				}
			}
		}
		else if (sscanf (buff, " frame %d", &frameIndex) == 1) 
		{
			for (i = 0; i < numAnimatedComponents; ++i) 
			{
				fscanf (file, "%f", &animFrameData[i]);
			}
			// Build frame skeleton from the collected data
			_buildFrameSkeleton(jointInfos, baseFrame, animFrameData, builder->animationData.skeletonFrames.frames[frameIndex], numJoints);
		}
	}
}

void 
MD5Importer::_buildFrameSkeleton(const struct AnimationJointInfo* jointInfos, 
								 const struct AnimationBaseFrameJoint* baseFrame, 
								 const float* animFrameData, 
								 SkeletonFrame& skeletonFrames, 
								 int numJoints) 
{
	for(int i = 0; i < numJoints; ++i) 
	{
		const struct AnimationBaseFrameJoint* baseJoint = &baseFrame[i];
		glm::vec3 animatedPos = baseJoint->pos;
		glm::quat animatedOrient = baseJoint->orient;
		int j = 0;

		if (jointInfos[i].flags & 1) /* Tx */ {
			animatedPos.x = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if (jointInfos[i].flags & 2) /* Ty */ {
			animatedPos.y = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if (jointInfos[i].flags & 4) /* Tz */ {
			animatedPos.z = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if (jointInfos[i].flags & 8) /* Qx */ {
			animatedOrient.x = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if (jointInfos[i].flags & 16) /* Qy */ {
			animatedOrient.y = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if (jointInfos[i].flags & 32) /* Qz */ {
			animatedOrient.z = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		/* Compute the w component of the quaternion */
		float t = 1.0f - (animatedOrient.x * animatedOrient.x) - (animatedOrient.y * animatedOrient.y) - (animatedOrient.z * animatedOrient.z);

		if (t < 0.0f)
			animatedOrient.w = 0.0f;
		else
			animatedOrient.w = -sqrt (t);

		/******************************************************
		* NOTE: we assume that this joint's parent has		*
		* already been calculated, i.e. joint's ID should	*
		* never be smaller than its parent ID.				*
		******************************************************/
		Joint& thisJoint = skeletonFrames.joints[i];

		int parent = jointInfos[i].parent;
		thisJoint.parentIndex = parent;
		thisJoint.name = std::string(jointInfos[i].name);

		/* Has parent? */
		if (thisJoint.parentIndex < 0) {
			thisJoint.position = animatedPos;
			thisJoint.orientation = animatedOrient;
		}
		else {
			Joint& parentJoint = skeletonFrames.joints[parent];
			glm::vec3 rpos; /* Rotated position */

			/* Add positions */
			rpos = glm::rotate(parentJoint.orientation, animatedPos);
			thisJoint.position.x = rpos.x + parentJoint.position.x;
			thisJoint.position.y = rpos.y + parentJoint.position.y;
			thisJoint.position.z = rpos.z + parentJoint.position.z;

			/* Concatenate rotations */
			thisJoint.orientation = parentJoint.orientation * animatedOrient;
			thisJoint.orientation + glm::normalize(thisJoint.orientation);
		}
	}
}

void 
MD5Importer::_createBasePose(std::shared_ptr<MD5Mesh::Builder> const& builder) 
{
	glm::vec3 vertex, normal;

	for (int i = 0; i < builder->subMeshes.size() ; ++i) 
	{
		MD5Mesh::Builder::SubMesh& subMesh = builder->subMeshes[i];
		unsigned int numVertices = (unsigned int)subMesh.vertices.size();

		/* Setup vertices */
		for(unsigned int i = 0; i < numVertices; ++i) 
		{
			vertex = glm::vec3(0.f,0.f,0.f);
			normal = glm::vec3(0.f,0.f,0.f);

			/* Calculate final vertex to draw with weights */
			for (Int32 j = 0; j < subMesh.vertexWeights[i].numWeightIndices; ++j) 
			{
				Weight const& weight = subMesh.weights[subMesh.vertexWeights[i].startWeightIndex + j];
				Joint const& joint = builder->baseSkeleton.joints[weight.jointIndex];

				/* Calculate transformed vertex for this weight */
				glm::vec3 wv;//, wn;
				wv = glm::rotate(joint.orientation, weight.position);
				//wv = joint.orientation.rotate(weight.position);
				//wn = joint.orientation.rotate(weight.normal);

				/* The sum of all weight->bias should be 1.0 */
				vertex += (joint.position + wv) * weight.bias;
				normal += wv * weight.bias;
			}
			subMesh.vertices[i] = vertex;
			if(subMesh.normals.size() > 0) {
				subMesh.normals[i] = normal;
			}
		}
	}
}

FILE* 
MD5Importer::open(std::string const& fileName)
{
	return fopen (fileName.c_str(), "rb");
}

void
MD5Importer::close(FILE* file)
{
	fclose(file);
}