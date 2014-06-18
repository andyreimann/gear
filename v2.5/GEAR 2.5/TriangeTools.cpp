#include "TriangeTools.h"
#include "Logger.h"

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