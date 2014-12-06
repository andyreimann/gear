#pragma once
#include <glm/glm.hpp>

namespace G2Core
{
	namespace ShaderView
	{
		/** This struct holds the datastructure for a material in the same
		* layout it has in shaders.
		* The layout in the structs all follow the std140 Uniform Buffer Layout http://ptgmedia.pearsoncmg.com/images/9780321552624/downloads/0321552628_AppL.pdf
		* @created:	2014/12/04
		* @author Andy Reimann <a.reimann@moorlands-grove.de>
		*/
		struct Material
		{
			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;
			float shininess;
		};
		/** This struct holds the datastructure for a light source in the same
		* layout it has in shaders.
		* @created:	2014/12/04
		* @author Andy Reimann <a.reimann@moorlands-grove.de>
		*/
		//struct Light
		//{
		//	glm::vec4 ambient;				// The ambient color of the light
		//	glm::vec4 diffuse;				// The diffuse color of the light
		//	glm::vec4 specular;				// The specular color of the light
		//	glm::vec4 attenuation;			// [x=constant,y=linear,z=quadric]
		//	float zFar[4];					// The far clip planes used
		//	glm::mat4 eyeToLightClip[4];	// The bias*crop*proj*modelview matrix (only set if type = 1)
		//	glm::vec4 position;				// Local position - if(position.w == 0.0) -> directional light, else -> point or spot light
		//	glm::vec3 direction;			// Light direction
		//	float cosCutoff;				// if(cosCutoff >=0 ) spotlight else point or directional light
		//	int type;						// 0 = no shadow, 1 = CSM
		//	int numCascades;				// The number of shadow cascades
		//	float range;					// The distance or the range of the light cone
		//	bool enabled;
		//};

		//struct G2Light
		//{
		//	//int activeLights;
		//	Light light[8];
		//};

		struct Light
		{
			glm::vec4 ambient; 
			glm::vec4 diffuse;				// The diffuse color of the light
			glm::vec4 specular;				// The specular color of the light
			glm::vec4 attenuation;			// [x=constant,y=linear,z=quadric]
			float zFar[4];					// The far clip planes used
			glm::mat4 eyeToLightClip[4];	// The bias*crop*proj*modelview matrix (only set if type = 1)
			glm::vec4 position;				// Local position - if(position.w == 0.0) -> directional light, else -> point or spot light
			glm::vec3 direction;			// Light direction
			float cosCutoff;				// if(cosCutoff >=0 ) spotlight else point or directional light
			int type;						// 0 = no shadow, 1 = CSM
			int numCascades;				// The number of shadow cascades
			float range;					// The distance or the range of the light cone
			float _postStructPaddingToN4;
		};

		struct Lights
		{
			int activeLights;
			glm::vec3 _preStructPaddingToN4;
			Light light[8];
		};


		/** This struct holds the datastructure for the matrices in the same
		* layout it has in shaders.
		* @created:	2014/12/04
		* @author Andy Reimann <a.reimann@moorlands-grove.de>
		*/
		struct Matrices
		{
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 modelMatrix;
			glm::mat4 modelViewMatrix;
			glm::mat4 normalMatrix; // is mat3 in shader but needs to be uploaded as a mat4
		};
		/** This struct holds the datastructure for the post processing information in the same
		* layout it has in shaders.
		* @created:	2014/12/04
		* @author Andy Reimann <a.reimann@moorlands-grove.de>
		*/
		struct PostProcessingInfo
		{
			glm::vec2 pixelSize;
			glm::vec2 windowSize;
		};
	};
};