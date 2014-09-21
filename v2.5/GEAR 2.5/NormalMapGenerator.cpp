#include "NormalMapGenerator.h"

#include <vector>

using namespace G2;

void cubicInterpolation(
		glm::mat4 const& matCubic,
		glm::vec4 const& geometry,
		float ip,
		float& heightOut,
		float& slopeOut) {

	glm::vec4 T = glm::vec4(0.f,0.f,0.f,0.f);
	glm::vec4 ABCD = glm::vec4(0.f,0.f,0.f,0.f);

	// Calculate Cubic Coefficients
	ABCD = matCubic * geometry;

	// T Vector
	T.w = 1;
	T.z = ip;
	T.y = T.z * T.z;
	T.x = T.y * T.z;

	// Solve Cubic for Height
	heightOut = glm::dot(T,ABCD);

	// T Vector for Derivative
	T.w = 0;
	T.z = 1;
	T.y = 2.0f * ip;
	T.x = 3.0f * ip * ip;

	// Solve Quadratic for Slope
	slopeOut = glm::dot(T,ABCD);
}

std::shared_ptr<Texture2D>
NormalMapGenerator::generateFromHeightMap(unsigned char* heightMap, int size, float maxHeight) 
{
	const static glm::mat4 catmullRomMatrix = glm::mat4(
		-0.5f,  1.0f, -0.5f,  0.0f,
		 1.5f, -2.5f,  0.0f,  1.0f,
		-1.5f,  2.0f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,  0.0f
	);
	
	std::vector<unsigned char> normals(size * size * 3);
	
	for(int y = 0; y < size; ++y)
	{
		for(int x = 0; x < size; ++x)
		{
			int ix0 = y*size+std::max(0,x-1);
			int ix1 = y*size+x;
			int ix2 = y*size+std::min(x+1,size-1);
			int ix3 = y*size+std::min(x+2,size-1);
			
			int iy0 = std::max(0,y-1)*size+x;
			int iy1 = y*size+x;
			int iy2 = std::min(y+1,size-1)*size+x;
			int iy3 = std::min(y+2,size-1)*size+x;
			
			float slopeX1 = 0.f;
			float heightX1 = 0.f;

			cubicInterpolation(
				catmullRomMatrix, 
				glm::vec4(
					((float)heightMap[ix0])/255.f*maxHeight,
					((float)heightMap[ix1])/255.f*maxHeight,
					((float)heightMap[ix2])/255.f*maxHeight,
					((float)heightMap[ix3])/255.f*maxHeight
				), 
				0.f, 
				heightX1, 
				slopeX1
			);
			
			float slopeY1 = 0.f;
			float heightY1 = 0.f;
			
			cubicInterpolation(
				catmullRomMatrix, 
				glm::vec4(
					((float)heightMap[iy0])/255.f*maxHeight,
					((float)heightMap[iy1])/255.f*maxHeight,
					((float)heightMap[iy2])/255.f*maxHeight,
					((float)heightMap[iy3])/255.f*maxHeight
				), 
				0.f, 
				heightY1, 
				slopeY1
			);
			
			// get the slope in texture space
			slopeX1 = std::max(-1.f, std::min(1.f, slopeX1));
			slopeY1 = std::max(-1.f, std::min(1.f, slopeY1));
			slopeX1 = -slopeX1;
			slopeY1 = -slopeY1;
			slopeX1 = slopeX1*0.5f+0.5f;
			slopeY1 = slopeY1*0.5f+0.5f;

			// calculate normal for current pixel
			normals[(y*size+x)*3]	= (unsigned char)(slopeX1*255.f);
			normals[(y*size+x)*3+1] = (unsigned char)((1.f-((slopeX1+slopeY1)/2.f))*255.f);
			normals[(y*size+x)*3+2] = (unsigned char)(slopeY1*255.f);
		}
	}
	return std::shared_ptr<G2::Texture2D>(new Texture2D(G2Core::FilterMode::NEAREST, G2Core::FilterMode::NEAREST, size, size, G2Core::DataFormat::RGB, G2Core::DataFormat::RGB, G2Core::WrapMode::CLAMP_TO_EDGE, G2Core::WrapMode::CLAMP_TO_EDGE, false, &normals[0]));			// The height map to use for this terrain
}