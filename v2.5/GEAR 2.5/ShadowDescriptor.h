#pragma once

#include "Frustum.h"
#include "Logger.h"
#include <glm/glm.hpp>


namespace G2 
{
	/// Describes the configuration of shadows and how they behave at runtime.
	/// @created:	2014/06/02
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	struct ShadowDescriptor 
	{
		/// This constructs a new ShadowDescriptor.
		ShadowDescriptor() 
			: numCascades(0),
			customEffect("") {}
		/// This function will set the height in pixels of the shadow texture to use
		/// @param v The height in pixels of the shadow texture to use
		ShadowDescriptor& setNumCascades(unsigned int const& v) { numCascades = v; return *this; }
		/// This function will override the effect to use for rendering the shadows.
		/// Normally the engine has predefined internal shader to use for rendering the shadows.
		/// However it might be the case that you need a modified shader instead to achieve what you want.
		/// @param v The path to the effect file to load and use for rendering the shadows
		ShadowDescriptor& setCustomEffect(std::string const& v) { customEffect = v; return *this; }
		static ShadowDescriptor noShadows() { return ShadowDescriptor().setNumCascades(0); }

		std::string	 customEffect;	// The path to a custom effect to load and use for rendering the shadows
		unsigned int numCascades;	// The number of cascades to use (not needed for every type of shadow mapping implemented). Default value is 0

		// DEBUG USED BY RenderSystem

		void setupSplitDistance(float zNear, float zFar)
		{
			float ratio = zFar/zNear;
			nearClips.resize(numCascades);
			farClips.resize(numCascades);
			frusta.resize(numCascades);
			orthoFrusta.resize(numCascades);
	
			// set the first nearplane to the full nearplane of the user camera
			nearClips[0] = zNear; 
	
			for( unsigned int i = 1; i < numCascades; ++i ) {
				float si = i / (float)numCascades;

				nearClips[i] = splitWeight*(zNear*powf(ratio, si)) + (1-splitWeight)*(zNear + (zFar - zNear)*si);
				farClips[i-1] = nearClips[i] * splitDistFactor;

				G2::logger << "[ShadowDescriptor] Cascade " << i << " -> Near:" << nearClips[i-1] << " Far:" << farClips[i-1] << "\n";
			}
			// set the last farplane to the full farplane of the user camera
			farClips[numCascades-1] = zFar;
			G2::logger << "[ShadowDescriptor] Cascade " << numCascades << " -> Near:" << nearClips[numCascades-1] << " Far:" << farClips[numCascades-1] << "\n";
		}

		void setupFrustumPoints(int pass, int width, int height, float fovY, glm::mat4 modelView, glm::mat4 const& invCameraTransformation, glm::mat4 const& invCameraRotation)
		{
			frusta[pass].setup(glm::perspective(fovY, width / (float)height, nearClips[pass], farClips[pass]) * modelView);

			glm::vec3 view_dir = glm::vec3(invCameraRotation * glm::vec4(0.f,0.f,-1.f,0.f));
			view_dir = glm::normalize(view_dir);

			glm::vec3 center = glm::vec3(invCameraTransformation * glm::vec4(0.f,0.f,0.f,1.f));

			glm::vec3 up(0.0f, 1.0f, 0.0f);
			glm::vec3 right = glm::cross(view_dir,up);

			// compute the center points of the near clip plane and the far clip plane
			glm::vec3 fc = center + view_dir*farClips[pass];
			glm::vec3 nc = center + view_dir*nearClips[pass];

			// compute the right orientation vector
			right = glm::normalize(right);
			up = glm::cross(right,view_dir);
			up = glm::normalize(up);

			float viewportRatio = (float)width/(float)height;

			// these heights and widths are half the heights and widths of
			float tan_fov_1_2 = (float)tan((fovY/57.2957795)/2.0f); // in radians
			// the near and far plane rectangles
			float near_height = tan_fov_1_2 * nearClips[pass];
			float near_width = near_height * viewportRatio;
			float far_height = tan_fov_1_2 * farClips[pass];
			float far_width = far_height * viewportRatio;

			frusta[pass].mCorners[0] = glm::vec4(glm::vec3(nc - up*near_height - right*near_width), 1.f);
			frusta[pass].mCorners[1] = glm::vec4(glm::vec3(nc + up*near_height - right*near_width), 1.f);
			frusta[pass].mCorners[2] = glm::vec4(glm::vec3(nc + up*near_height + right*near_width), 1.f);
			frusta[pass].mCorners[3] = glm::vec4(glm::vec3(nc - up*near_height + right*near_width), 1.f);

			frusta[pass].mCorners[4] = glm::vec4(glm::vec3(fc - up*far_height - right*far_width), 1.f);
			frusta[pass].mCorners[5] = glm::vec4(glm::vec3(fc + up*far_height - right*far_width), 1.f);
			frusta[pass].mCorners[6] = glm::vec4(glm::vec3(fc + up*far_height + right*far_width), 1.f);
			frusta[pass].mCorners[7] = glm::vec4(glm::vec3(fc - up*far_height + right*far_width), 1.f);
		}

		void setOrthoFrustum(int pass, glm::mat4 const& mvp)
		{
			orthoFrusta[pass].setup(mvp);
		}

		float splitWeight;		//!< This factor tells where the hole Frustum has to be split. A higher value will split the Frustum near the Camera and a lower value will split the frustum farer away from the Camera
		float splitDistFactor;	//!< This factor tells how many the single cascades will overlap each other
		std::vector<float> nearClips;		//!< This array contains the newarPlane for each subfrustum
		std::vector<float> farClips;		//!< This array contains the farplane for each subfrustum
		
		std::vector<Frustum> frusta; //!< The Frusta to use
		std::vector<Frustum> orthoFrusta; //!< The Frusta to use
		std::vector<glm::mat4> shadowCMPMatrix;	//!< This is the full shadow transformation matrix = cropmatrix * projection * modelview
		// DEBUG USED BY RenderSystem END
	};
};