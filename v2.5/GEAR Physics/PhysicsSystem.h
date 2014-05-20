#pragma once

#include <G2Core/BaseSystem.h>

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include <Bullet/btBulletCollisionCommon.h>

namespace G2 
{
	namespace Physics 
	{
		class PhysicsComponent;
		/// This class defines...
		/// @created:	2014/05/07
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsSystem : public BaseSystem<PhysicsSystem,PhysicsComponent> 
		{
			public:
				PhysicsSystem();
				void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

				virtual bool runsOnMainThread() override;

			private:
				btDefaultCollisionConfiguration* mCollisionConfiguration;
		};
	};
};