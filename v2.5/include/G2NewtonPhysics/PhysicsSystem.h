#pragma once
#include "Defines.h"
#include "PhysicsMaterialInteraction.h"
#include "Collision.h"

#include <unordered_map>

#include <G2Core/BaseSystem.h>
#include <G2Core/Event.h>

#include <utility>
#include <list>

struct NewtonWorld;
struct NewtonBody;
struct NewtonJoint;

//#define GEAR_SINGLE_THREADED

namespace G2 
{
	namespace Physics 
	{
		class PhysicsComponent;
		/// This class defines a PhysicsSystem which takes care of calculating physics.
		/// @created:	2014/05/29
		/// @author Andy Reimann <a.reimann@moorlands-grove.de>
		class PhysicsSystem : public BaseSystem<PhysicsSystem,PhysicsComponent>
		{
			friend class PhysicsComponent;
			public:
				PhysicsSystem();
				void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);
				
				virtual bool runsOnMainThread() override;
				virtual bool runsOnSideThread() override;
				/// Returns the PhysicsMaterialInteraction associated with the two given PhysicsMaterial objects.
				/// @param materialIdA The first material.
				/// @param materialIdB The second material.
				/// @return The PhysicsMaterialInteraction associated with the two given PhysicsMaterial objects.
				PhysicsMaterialInteraction& getMaterialInteraction(PhysicsMaterial materialIdA, PhysicsMaterial materialIdB)
				{
					return mMaterialInteractions[_combine(materialIdA, materialIdB)];
				}
				/// Creates a new Material, which can be used for creating rigid bodies in PhysicsComponent objects.
				/// @return The newly created PhysicsMaterial.
				PhysicsMaterial createMaterial();
				/// Enables or disables the debug draw mode for rigid bodies
				/// @param enabled True if debug drawing should be enabled, false otherwise.
				void setDebugDrawMode(bool enabled) { mDebugDrawEnabled = enabled; }

				~PhysicsSystem();

			private:
				// Callback functions for Newton Game Dynamics
				static void setTransformCallback(const NewtonBody* body, const float* matrix, int threadIndex);
				static void genericContactProcess (const NewtonJoint* contactJoint, float timestep, int threadIndex);
				static void applyForceAndTorqueCallback(const NewtonBody* body, float timestep, int threadIndex);
				/// Callback hooked in onRenderFrame for emitting the collisions
				/// @note Thread safe
				void _onEmitCollision(G2::FrameInfo const&);
				unsigned long long _combine(int id0, int id1) const ;
				/// Called from the PhysicsComponent if it is completely removed.
				/// This function will cleanup all collision listeners to not emit collisions 
				/// for removed entities.
				/// @note Thread safe
				void _onRemovePhysicsComponent(unsigned int entityId);

				NewtonWorld* mDynamicsWorld;
				std::unordered_map<unsigned long long,PhysicsMaterialInteraction>	mMaterialInteractions;
				std::unordered_map<unsigned int,std::list<Collision>>				mPendingCollisions;
				std::unordered_map<unsigned int,G2::Event<Collision const&>>		mCollisionCallbacks;
				std::mutex															mCollisionMutex;
				bool																mDebugDrawEnabled;
			
				
				double minStepWidth;						
				double timeSinceLastPhysicsUpdate;

		};
	};
};
