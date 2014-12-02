#pragma once

#include "Defines.h"

#include <G2/RenderComponent.h>

#include <LinearMath/btIDebugDraw.h>

#include <vector>
#include <glm/glm.hpp>
#include <mutex>

namespace G2 
{
	namespace Physics 
	{
		struct DebugLines
		{
			DebugLines()
			{
				vertices.resize(2048);
				colors.resize(2048);
			}
			void add(glm::vec3 const& from, glm::vec3 const& to, glm::vec3 const& color)
			{
				vertices.push_back(from);
				vertices.push_back(to);
				colors.push_back(color);
				colors.push_back(color);
			}
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> colors;
		};
		/// This class provides a physics debug drawer.
		/// @created:	2014/05/24
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class DebugDrawer : public btIDebugDraw
		{
			public:
				/// This constructs a new PhysicsDebugDrawer.
				DebugDrawer();

				void transport(G2::RenderComponent* target);
				

				virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);

				virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

				virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color);

				virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
	
				virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

				virtual void	reportErrorWarning(const char* warningString);

				virtual void	draw3dText(const btVector3& location,const char* textString);

				virtual void	setDebugMode(int debugMode);

				virtual int		getDebugMode() const { return mDebugMode;}
				/// normal destructor
				~DebugDrawer();
			private:

				DebugLines	mLines;
				std::mutex	mAccessMutex;
				
				int mDebugMode;

		};
	};
};