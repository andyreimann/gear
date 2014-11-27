#include "TriangeTools.h"
#include "Logger.h"

#include <algorithm>

using namespace G2;

void
TriangeTools::calculateTangentAndBinormalForTriangle(
	unsigned int i1, 
	unsigned int i2,
	unsigned int i3,
	glm::vec3 const& p1, 
	glm::vec3 const& p2, 
	glm::vec3 const& p3, 
	glm::vec2 const& t1, 
	glm::vec2 const& t2, 
	glm::vec2 const& t3, 
	std::vector<glm::vec3>& binormals, 
	std::vector<glm::vec3>& tangents) 
{
	
	if(TriangeTools::isDegenerated(p1,p2,p3) ) 
	{
		logger << "[TriangleTools] Detected degenerated triangle!" << endl;
		tangents[i1]  = tangents[i2]  = tangents[i3]  = glm::vec3(1.f,0.f,0.f);
		binormals[i1] = binormals[i2] = binormals[i3] = glm::vec3(0.f,0.f,1.f);
		return;
	}
	glm::vec3 p2p1, p3p1;
	float c2c1_T, c2c1_B, c3c1_T, c3c1_B;
	glm::vec3 T, B, N;
	float fDenominator;

	p2p1 = p3 - p1;
	p3p1 = p2 - p1;
	// Calculate the "direction" of the triangle based on texture coordinates.
	c2c1_T = t3.x - t1.x;
	c2c1_B = t3.y - t1.y;

	// Calculate c3c1_T and c3c1_B
	c3c1_T = t2.x - t1.x;
	c3c1_B = t2.y - t1.y;

	//Look at the references for more explanation for this one.
	fDenominator = c2c1_T * c3c1_B - c3c1_T * c2c1_B;  
	if ( fDenominator == 0.0f) {
		/* We won't risk a divide by zero, so set the tangent matrix to the
			identity matrix */
		tangents[i1]  = tangents[i2]  = tangents[i3]  = glm::vec3(1.f,0.f,0.f);
		binormals[i1] = binormals[i2] = binormals[i3] = glm::vec3(0.f,0.f,1.f);
	}
	else {
		// Calculate the reciprocal value once and for all (to achieve speed)
		float fScale1 = 1.0f / fDenominator;

		/* Time to calculate the tangent, binormal, and normal.
			Look at Søren’s article for more information. */
		T = glm::vec3((c3c1_B * p2p1.x - c2c1_B * p3p1.x) * fScale1,
						(c3c1_B * p2p1.y - c2c1_B * p3p1.y) * fScale1,
						(c3c1_B * p2p1.z - c2c1_B * p3p1.z) * fScale1);

		B = glm::vec3((-c3c1_T * p2p1.x + c2c1_T * p3p1.x) * fScale1,
						(-c3c1_T * p2p1.y + c2c1_T * p3p1.y) * fScale1,
						(-c3c1_T * p2p1.z + c2c1_T * p3p1.z) * fScale1);
		N = glm::cross(T,B); 

		float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + 
								(B.x * N.y * T.z - B.z * N.y * T.x) + 
								(N.x * T.y * B.z - N.z * T.y * B.x));
		glm::vec3 InvTangent((glm::cross(B,N)).x * fScale2,
						(glm::cross(-N,T)).x * fScale2,
						(glm::cross(T,B)).x * fScale2);
		InvTangent = glm::normalize(InvTangent);	

		glm::vec3 InvNormal((glm::cross(B,N)).z * fScale2,
						(glm::cross(-N,T)).z * fScale2,
						(glm::cross(T,B)).z * fScale2);
		InvNormal = glm::normalize(InvNormal);	
 
		glm::vec3 InvBinormal((glm::cross(-B,N)).y * fScale2,
							(glm::cross(N,T)).y * fScale2,
							(glm::cross(-T,B)).y * fScale2);
		InvBinormal = glm::normalize(InvBinormal);	

		// write the calculated values into the array
		tangents[i1]  = tangents[i2]  = tangents[i3]  = InvTangent;
		binormals[i1] = binormals[i2] = binormals[i3] = InvBinormal;
	}
}

bool
TriangeTools::isDegenerated(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3) 
{
	return (p1 == p2 || p1 == p3 || p2 == p3);
}


std::pair<glm::vec4, bool> 
TriangeTools::intersectRayTriangle(G2::Ray const& ray, glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3)
{
	glm::vec4 rayDir = ray.getDir();
	// get the indices for the triangle vertices
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 n = glm::cross(v1,v2);
	n = glm::normalize(n);
	n *= -1;
	float dist = -glm::dot(ray.getOrigin() - p1, n) / glm::dot(glm::vec3(rayDir),n);
	if (dist > 0.0f)
	{
		float bx, by, cx, cy, px, py;
		glm::vec3 intersection = ray.getOrigin() + (glm::vec3(rayDir) * dist);
		glm::vec3 p = intersection - p1;
		float nmax = std::max<float>(fabs(n.x), std::max<float>(fabs(n.y), fabs(n.z)));
		if (fabs(n.x) == nmax) 
		{
			bx = v1.y;
			by = v1.z;
			cx = v2.y;
			cy = v2.z;
			px = p.y;
			py = p.z;
		}
		else if (fabs(n.y) == nmax) 
		{
			bx = v1.x;
			by = v1.z;
			cx = v2.x;
			cy = v2.z;
			px = p.x;
			py = p.z;
		}
		else {
			bx = v1.x;
			by = v1.y;
			cx = v2.x;
			cy = v2.y;
			px = p.x;
			py = p.y;
		}
		float u = (py * cx - px * cy) / (by * cx - bx * cy);
		float v = (py * bx - px * by) / (cy * bx - cx * by);

		if (u > 0.0f - std::numeric_limits<float>::epsilon() && v > 0.0f - std::numeric_limits<float>::epsilon() && (u + v) < 1.0f + std::numeric_limits<float>::epsilon()) 
		{
			return std::make_pair<glm::vec4, bool>(glm::vec4(intersection,1.f), true);
		}
	}
	return std::make_pair<glm::vec4, bool>(glm::vec4(0.f), false);
}