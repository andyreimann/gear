// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "ProjectionTools.h"

#include <glm/ext.hpp>

using namespace G2;
using namespace Tools;

int
Projection::project(glm::vec3 const& obj, glm::mat4 const& modelviewMat, glm::mat4 const& projectionMat, glm::detail::tvec4<int> const& viewport, glm::vec3& windowCoordinates)
{
	float const* const modelview = glm::value_ptr(modelviewMat);
	float const* const projection = glm::value_ptr(projectionMat);
	//Transformation vectors
	float fTempo[8];
	//Modelview transform
	fTempo[0] = modelview[0] * obj.x + modelview[4] * obj.y + modelview[8] * obj.z + modelview[12];  //w is always 1
	fTempo[1] = modelview[1] * obj.x + modelview[5] * obj.y + modelview[9] * obj.z + modelview[13];
	fTempo[2] = modelview[2] * obj.x + modelview[6] * obj.y + modelview[10] * obj.z + modelview[14];
	fTempo[3] = modelview[3] * obj.x + modelview[7] * obj.y + modelview[11] * obj.z + modelview[15];
	//Projection transform, the final row of projection matrix is always [0 0 -1 0]
	//so we optimize for that.
	fTempo[4] = projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8] * fTempo[2] + projection[12] * fTempo[3];
	fTempo[5] = projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9] * fTempo[2] + projection[13] * fTempo[3];
	fTempo[6] = projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
	fTempo[7] = -fTempo[2];
	//The result normalizes between -1 and 1
	if (fTempo[7] == 0.0)	//The w value
		return 0;
	fTempo[7] = 1.0 / fTempo[7];
	//Perspective division
	fTempo[4] *= fTempo[7];
	fTempo[5] *= fTempo[7];
	fTempo[6] *= fTempo[7];
	//Window coordinates
	//Map x, y to range 0-1
	windowCoordinates.x = (fTempo[4] * 0.5 + 0.5)*viewport[2] + viewport[0];
	windowCoordinates.y = (fTempo[5] * 0.5 + 0.5)*viewport[3] + viewport[1];
	//This is only correct when glDepthRange(0.0, 1.0)
	windowCoordinates.z = (1.0 + fTempo[6])*0.5;	//Between 0 and 1
	return 1;
}

int
G2::Tools::Projection::unProject(glm::vec3 const& win, glm::mat4 const& modelviewMat, glm::mat4 const& projectionMat, glm::detail::tvec4<int> const& viewport, glm::vec3& obj)
{
	glm::mat4 finalMatrix = projectionMat * modelviewMat;
	glm::vec4 in;
	glm::vec4 out;

	finalMatrix = glm::inverse(finalMatrix);

	in.x = win.x;
	in.y = win.y;
	in.z = win.z;
	in.w = 1.0;

	/* Map x and y from window coordinates */
	in.x = (in.x - viewport.x) / (float)viewport.z * 2.f - 1.f;
	in.y = (in.y - viewport.y) / (float)viewport.w * 2.f - 1.f;

	/* Map to range -1 to 1 */
	in.z = in.z * 2.f - 1.f;

	out = finalMatrix * in;

	if (out.w == 0.f) return(false);
	out.w = 1.f / out.w;

	obj.x = out.x*out.w;
	obj.y = out.y*out.w;
	obj.z = out.z*out.w;
	return(true);
}