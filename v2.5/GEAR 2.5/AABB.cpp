// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "AABB.h"
#include <limits>

using namespace G2;

AABB::AABB() 
{
	this->clear();
}

AABB::AABB(AABB && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

AABB& AABB::operator=(AABB && rhs) 
{
	
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mCenter = std::move(rhs.mCenter);
	mHalfExtends = std::move(rhs.mHalfExtends);
	// 3. Stage: modify src to a well defined state
	// nothing here
	return *this;
}

AABB::AABB( glm::vec3 const& Min, glm::vec3 const& Max ) 
{
	
	setFromMinAndMax(Min, Max);
	//syncTransformComponent();
}

AABB& AABB::merge( glm::vec3 const& pt ) 
{
	if(isEmpty()) 
	{
		setFromMinAndMax(pt,pt);
	}
	else 
	{
		glm::vec3 Min = mCenter-mHalfExtends;
		glm::vec3 Max = mCenter+mHalfExtends;
	
		Min = glm::vec3(std::min<float>(Min.x,pt.x), std::min<float>(Min.y,pt.y), std::min<float>(Min.z,pt.z));
		Max = glm::vec3(std::max<float>(Max.x,pt.x), std::max<float>(Max.y,pt.y), std::max<float>(Max.z,pt.z));
		setFromMinAndMax(Min,Max);
	}
	//syncTransformComponent();
	return *this;
}

AABB& AABB::merge( AABB const& aabb ) 
{
	if(isEmpty()) 
	{
		setFromMinAndMax(aabb.getMin(),aabb.getMax());
	}
	else 
	{
		glm::vec3 Min = mCenter-mHalfExtends;
		glm::vec3 Max = mCenter+mHalfExtends;

		Min = glm::vec3(std::min<float>(Min.x,aabb.getMin().x), std::min<float>(Min.y,aabb.getMin().y), std::min<float>(Min.z,aabb.getMin().z));
		Max = glm::vec3(std::max<float>(Max.x,aabb.getMax().x), std::max<float>(Max.y,aabb.getMax().y), std::max<float>(Max.z,aabb.getMax().z));
		setFromMinAndMax(Min,Max);
	}
	//syncTransformComponent();

	return *this;
}



AABB& AABB::setFromMinAndMax( glm::vec3 const& Min, glm::vec3 const& Max) 
{
	mCenter = (Min + Max) * 0.5f;
	mHalfExtends = (Max - Min) * 0.5f;
	//syncTransformComponent();

	return *this;
}

AABB& AABB::setFromCenterAndHalfExtends( glm::vec3 const& center, glm::vec3 const& halfExtends) 
{
	this->mCenter = center;
	this->mHalfExtends = halfExtends;
	//syncTransformComponent();

	return *this;
}

bool AABB::intersects( Ray const& ray ) const 
{
	glm::vec3 minimum = getMin();
	glm::vec3 maximum = getMax();

	///////////////////////////////////////////////////////////////////////////////
	// AABB-Ray intersection - Really performance optimized implementation: @see http://courses.csusm.edu/cs697exz/ray_box.htm
	///////////////////////////////////////////////////////////////////////////////
	float tfar =  FLT_MAX;
	float tnear = FLT_MIN;
	float txMin, txMax, tyMin, tyMax, tzMin, tzMax;
	glm::vec3 const& o = ray.getOrigin();
	glm::vec4 const& d = ray.getDir();
	txMin = (minimum.x - o.x ) / d.x;
	txMax = (maximum.x - o.x ) / d.x;
	if(txMin > txMax) std::swap(txMin, txMax);
	tnear = txMin; 
	tfar = txMax;
	tyMin = (minimum.y - o.y ) / d.y;
	tyMax = (maximum.y - o.y ) / d.y;
	if(tyMin > tyMax) std::swap(tyMin, tyMax);
	if(tnear <= tyMin) tnear = tyMin;
	if(tfar > tyMax) tfar = tyMax;
	tzMin = (minimum.z - o.z ) / d.z;
	tzMax = (maximum.z - o.z ) / d.z;
	if(tzMin > tzMax) std::swap(tzMin, tzMax);
	if(tnear <= tzMin) tnear = tzMin;
	if(tfar > tzMax) tfar = tzMax;

	if(tnear <= tfar && (tnear > 0.0f || tfar > 0.0f) ) 
	{
		return true; // intersection - only if the target is in front of the ray
	}
	else 
	{ 
		return false; // no intersection
	}
}

AABB& AABB::clear() 
{
	mCenter = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	mHalfExtends = -glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	//syncTransformComponent();

	return *this;
}

bool AABB::isEmpty() const 
{
	return mHalfExtends.y < 0.0f;
}

glm::vec3 const& AABB::getCenter() const 
{
	return mCenter;
}

glm::vec3 const& AABB::getHalfExtends() const 
{
	return mHalfExtends;
}

glm::vec3 AABB::getMin() const 
{
	return mCenter - mHalfExtends;
}

glm::vec3 AABB::getMax() const 
{	
	return mCenter + mHalfExtends;
}

bool AABB::overlaps(AABB const& rhs) const 
{
	glm::vec3 T = rhs.getCenter() - mCenter;
	glm::vec3 const& otherHalfExtends = rhs.getHalfExtends();

	return fabs(T.x) <= (mHalfExtends.x + otherHalfExtends.x) &&
		   fabs(T.y) <= (mHalfExtends.y + otherHalfExtends.y) &&
		   fabs(T.z) <= (mHalfExtends.z + otherHalfExtends.z);

	/*
	// http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=3
	// http://devmaster.net/forums/topic/7934-aabb-collision/
	// http://stackoverflow.com/questions/6053522/how-to-recalculate-axis-aligned-bounding-box-after-translate-rotate
	return
		minX > rhs.maxX || maxX < rhs.minX ||
		minY > rhs.maxY || maxY < rhs.minY ||
		minZ > rhs.maxZ || maxZ < rhs.minZ;
		*/
	//return false;
}

//bool AABB::contains(AABB const& rhs) const 
//{
//	/*
//    return
//        minX >= rhs.minX && maxX <= rhs.maxX &&
//        minY >= rhs.minY && maxY <= rhs.maxY &&
//        minZ >= rhs.minZ && maxZ <= rhs.maxZ;
//		*/
//	return false;
//}

bool AABB::contains(glm::vec3 const& point) const 
{
	glm::vec3 min = getMin();
	glm::vec3 max = getMax();
	return
		min.x <= point.x && max.x >= point.x &&
		min.y <= point.y && max.y >= point.y &&
		min.z <= point.z && max.z >= point.z;
		
}
/*
void AABB::transform(glm::mat4 const& matrix) {

	mCenter = glm::vec3(matrix * glm::vec4(getCenter(),1.f));
	// since half extends is a vector but actually stored in 3 floats, we have to ensure 
	// that multiplication with a matrix uses the homogeneous
	// value of 0.0 as normal for vectors.
	mHalfExtends = glm::vec3(matrix * glm::vec4(getHalfExtends(),0.f));
	
	glm::vec3 minimum = getMin();
	glm::vec3 maximum = getMax();
	merge(minimum);
	merge(glm::vec3(minimum.x,minimum.y,maximum.z));
	merge(glm::vec3(minimum.x,maximum.y,maximum.z));
	merge(glm::vec3(maximum.x,minimum.y,maximum.z));
	merge(glm::vec3(maximum.x,minimum.y,minimum.z));
	merge(glm::vec3(minimum.x,maximum.y,minimum.z));
	merge(glm::vec3(maximum.x,maximum.y,minimum.z));
	merge(maximum);

	if(!hasComponent<TransformComponent>()) {
		auto* transformation = getComponent<TransformComponent>();
	}
}*/

void AABB::setCenter(glm::vec3 const& center) 
{
	mCenter = center;
	//syncTransformComponent();
}

void AABB::translate(glm::vec3 const& translation) 
{
	mCenter += translation;
	//syncTransformComponent();
}

unsigned int AABB::maxExtend() const 
{
	/*
	Vec3F diag = Vec3F(maxX,maxY,maxZ) - Vec3F(minX,minY,minZ);
	if(diag.x > diag.y && diag.x > diag.z) {
		return 0; // X
	}
	else if(diag.y > diag.z) {
		return 1; // Y
	}
	return 2; // Z
	*/
	return 0;
}

void
AABB::enableRendering() 
{
	//if(!hasComponent<RenderComponent>()) 
	//{
	//	// setup rendering as if the box is in center
	//	// transformation is done with the TransformComponent
	//	const int numVertices = 48;
	//	glm::vec4 vertices[numVertices];
	//	glm::vec4 color(0.75f,0.75f,1.f,1.f);

	//	float minX, minY, minZ, maxX, maxY, maxZ;

	//	glm::vec3 min = -mHalfExtends;
	//	glm::vec3 max = mHalfExtends;
	//
	//	minX = min.x;
	//	minY = min.y;
	//	minZ = min.z;
	//
	//	maxX = max.x;
	//	maxY = max.y;
	//	maxZ = max.z;
	//	
	//	float cornerWidth = (max.x - min.x) / 10.f;
	//	float cornerheight = (max.y - min.y) / 10.f;
	//	float cornerdepth = (max.z - min.z) / 10.f;

	//	//
	//	int i = 0;
	//	vertices[i++] = glm::vec4(min.x, min.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x+cornerWidth, min.y, min.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, min.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, min.y+cornerheight, min.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, min.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, min.y, min.z+cornerdepth, 1.f);

	//	
	//	vertices[i++] = glm::vec4(max.x, min.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x-cornerWidth, min.y, min.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, min.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, min.y+cornerheight, min.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, min.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, min.y, min.z+cornerdepth, 1.f);

	//	
	//	vertices[i++] = glm::vec4(max.x, min.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x-cornerWidth, min.y, max.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, min.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, min.y+cornerheight, max.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, min.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, min.y, max.z-cornerdepth, 1.f);

	//	
	//	vertices[i++] = glm::vec4(min.x, min.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x+cornerWidth, min.y, max.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, min.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, min.y+cornerheight, max.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, min.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, min.y, max.z-cornerdepth, 1.f);

	//	// top 4
	//	
	//	vertices[i++] = glm::vec4(min.x, max.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x+cornerWidth, max.y, min.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, max.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, max.y-cornerheight, min.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, max.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, max.y, min.z+cornerdepth, 1.f);

	//	
	//	vertices[i++] = glm::vec4(max.x, max.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x-cornerWidth, max.y, min.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, max.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, max.y-cornerheight, min.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, max.y, min.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, max.y, min.z+cornerdepth, 1.f);

	//	
	//	vertices[i++] = glm::vec4(max.x, max.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x-cornerWidth, max.y, max.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, max.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, max.y-cornerheight, max.z, 1.f);

	//	vertices[i++] = glm::vec4(max.x, max.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(max.x, max.y, max.z-cornerdepth, 1.f);

	//	
	//	vertices[i++] = glm::vec4(min.x, max.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x+cornerWidth, max.y, max.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, max.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, max.y-cornerheight, max.z, 1.f);

	//	vertices[i++] = glm::vec4(min.x, max.y, max.z, 1.f);
	//	vertices[i++] = glm::vec4(min.x, max.y, max.z-cornerdepth, 1.f);
	//	
	//	auto* renderComponent = addComponent<RenderComponent>(1);
	//	renderComponent->vaos[0].resize(numVertices)
	//							.writeData(Semantics::POSITION, vertices);
	//	renderComponent->material.setAmbientAndDiffuse(color)
	//							 .setSpecular(glm::vec4(0.f,0.f,0.f,0.f));
	//	renderComponent->drawMode = GL_LINES;
	//	TransformComponent* transformation = addComponent<TransformComponent>();
	//	syncTransformComponent();
	//}
}

void
AABB::disableRendering() 
{
	//removeComponent<RenderComponent>();
	//removeComponent<TransformComponent>();
}

float
AABB::getDistanceFromCenter(Ray const& ray) const 
{
	return glm::length(mCenter-ray.getOrigin());
}

G2::AABB
AABB::transform(glm::mat4 const& m) 
{
	glm::vec3 min = getMin();
	glm::vec3 max = getMax();

	AABB transformed;

	transformed.merge(m * glm::vec4( min.x, min.y, min.z, 1.f));
	transformed.merge(m * glm::vec4( max.x, min.y, min.z, 1.f));
	transformed.merge(m * glm::vec4( min.x, min.y, max.z, 1.f));
	transformed.merge(m * glm::vec4( max.x, min.y, max.z, 1.f));

	transformed.merge(m * glm::vec4( min.x, max.y, min.z, 1.f));
	transformed.merge(m * glm::vec4( max.x, max.y, min.z, 1.f));
	transformed.merge(m * glm::vec4( min.x, max.y, max.z, 1.f));
	transformed.merge(m * glm::vec4( max.x, max.y, max.z, 1.f));

	return std::move(transformed);
}
//void
//AABB::syncTransformComponent()
//{
//	TransformComponent* transformation = getComponent<TransformComponent>();
//	if(transformation != nullptr)
//	{
//		transformation->setPosition(mCenter);
//	}
//}