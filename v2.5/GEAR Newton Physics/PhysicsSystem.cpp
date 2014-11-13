#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include <G2/Logger.h>
#include <G2/TransformComponent.h>
#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>

#include <G2Core/EventDistributer.h>

#include <glm/ext.hpp>

#include <Newton/newton/Newton.h>
#include <Newton/dMath/dVector.h>
#include <Newton/dMath/dMatrix.h>
#include <Newton/dMath/dQuaternion.h>



using namespace G2::Physics;

PhysicsSystem::PhysicsSystem()
	: minStepWidth(1.0 / 60.0),
	timeSinceLastPhysicsUpdate(0.0),
	mDebugDrawEnabled(false)
{
	mDynamicsWorld = NewtonCreate();

	NewtonSetSolverModel (mDynamicsWorld, 5);

	//NewtonSetPerformanceClock(mDynamicsWorld, nullptr);

	G2::EventDistributer::onFrameRendered.hook(this, &PhysicsSystem::_onEmitCollision);
}


PhysicsSystem::~PhysicsSystem() 
{
	G2::EventDistributer::onFrameRendered.unHook(this, &PhysicsSystem::_onEmitCollision);
	// destroy all rigid bodies, this is no necessary because Newton Destroy world will also destroy all bodies
	// but if you want to change level and restart you can call this function to clean the world without destroying the world.
	NewtonDestroyAllBodies(mDynamicsWorld);
	// finally destroy the newton world 
	NewtonDestroy (mDynamicsWorld);
}

void
PhysicsSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo)
{
	if(mDebugDrawEnabled && name == "render")
	{
		glUseProgram(0);
		
		auto* cameraSystem = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>();
		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();

		CameraComponent* camera = cameraSystem->getRenderCamera();
		assert(camera != nullptr);
		
		glm::mat4 cameraSpaceMatrix;
		auto* cameraTransformation = transformSystem->get(camera->getEntityId());
		
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glMultMatrixf(glm::value_ptr(camera->getProjectionMatrix()));

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glMultMatrixf(glm::value_ptr(cameraTransformation->getWorldSpaceMatrix()));


		//glDisable (GL_LIGHTING);
		//glDisable(GL_TEXTURE_2D);

		//glColor3f(0.0f, 0.0f, 1.0f);

		//glBegin(GL_LINES);
		//for (NewtonBody* body = NewtonWorldGetFirstBody(mDynamicsWorld); body; body = NewtonWorldGetNextBody(mDynamicsWorld, body)) {
		//	dVector p0; 
		//	dVector p1; 
		//	dMatrix matrix;
		//	NewtonCollision* const collision = NewtonBodyGetCollision(body);
		//	NewtonBodyGetMatrix (body, &matrix[0][0]);
		//	NewtonCollisionCalculateAABB (collision, &matrix[0][0], &p0[0], &p1[0]);

		//	glVertex3f (p0.m_x, p0.m_y, p0.m_z);
		//	glVertex3f (p1.m_x, p0.m_y, p0.m_z);

		//	glVertex3f (p0.m_x, p1.m_y, p0.m_z);
		//	glVertex3f (p1.m_x, p1.m_y, p0.m_z);

		//	glVertex3f (p0.m_x, p1.m_y, p1.m_z);
		//	glVertex3f (p1.m_x, p1.m_y, p1.m_z);

		//	glVertex3f (p0.m_x, p0.m_y, p1.m_z);
		//	glVertex3f (p1.m_x, p0.m_y, p1.m_z);


		//	glVertex3f (p0.m_x, p0.m_y, p0.m_z);
		//	glVertex3f (p0.m_x, p1.m_y, p0.m_z);

		//	glVertex3f (p1.m_x, p0.m_y, p0.m_z);
		//	glVertex3f (p1.m_x, p1.m_y, p0.m_z);

		//	glVertex3f (p0.m_x, p0.m_y, p1.m_z);
		//	glVertex3f (p0.m_x, p1.m_y, p1.m_z);

		//	glVertex3f (p1.m_x, p0.m_y, p1.m_z);
		//	glVertex3f (p1.m_x, p1.m_y, p1.m_z);


		//	glVertex3f (p0.m_x, p0.m_y, p0.m_z);
		//	glVertex3f (p0.m_x, p0.m_y, p1.m_z);

		//	glVertex3f (p1.m_x, p0.m_y, p0.m_z);
		//	glVertex3f (p1.m_x, p0.m_y, p1.m_z);

		//	glVertex3f (p0.m_x, p1.m_y, p0.m_z);
		//	glVertex3f (p0.m_x, p1.m_y, p1.m_z);

		//	glVertex3f (p1.m_x, p1.m_y, p0.m_z);
		//	glVertex3f (p1.m_x, p1.m_y, p1.m_z);
		//}
		//glEnd();

		//
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}
#ifdef GEAR_SINGLE_THREADED
	else if(name == "preUpdate") 
#else
	else if(name == "updateSideThread") 
#endif
	{ 
		timeSinceLastPhysicsUpdate += frameInfo.timeSinceLastFrame;
		if(timeSinceLastPhysicsUpdate > minStepWidth)
		{
			// advance physics simulation
			NewtonUpdate(mDynamicsWorld, (float)(timeSinceLastPhysicsUpdate));
			timeSinceLastPhysicsUpdate = 0.0;
		}
	}
}

bool
PhysicsSystem::runsOnMainThread() 
{
#ifdef GEAR_SINGLE_THREADED
	return true;
#else
	return false;
#endif
}

bool
PhysicsSystem::runsOnSideThread() 
{
#ifdef GEAR_SINGLE_THREADED
	return false;
#else
	return true;
#endif
}

// Transform callback to set the matrix of a the visual entity
void PhysicsSystem::setTransformCallback (const NewtonBody* body, const float* matrix, int threadIndex)
{
	PhysicsComponentUserData* entity = (PhysicsComponentUserData*)NewtonBodyGetUserData(body);

	// Get the position from the matrix
	glm::vec3 position(matrix[12], matrix[13], matrix[14]);
	glm::quat rotation;

	// we will ignore the Rotation part of matrix and use the quaternion rotation stored in the body
	NewtonBodyGetRotation(body, glm::value_ptr(rotation));

	entity->transformSystem->lock();
	auto* transformComponent = entity->transformSystem->get(entity->entityId);
	if(transformComponent == nullptr)
	{
		entity->transformSystem->unlock();
		G2::BaseComponent<G2::TransformSystem>::create<G2::TransformComponent>(entity->entityId);
		entity->transformSystem->lock();
		// get transform component in a thread safe context again!
		transformComponent = entity->transformSystem->get(entity->entityId);
	}
	transformComponent->setPosition(
		position
	);
	transformComponent->setRotation(
		rotation
	);
	entity->transformSystem->unlock();
}

// callback to apply external forces to body
void PhysicsSystem::applyForceAndTorqueCallback (const NewtonBody* body, float timestep, int threadIndex)
{
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat mass;
	 
	// for now the only external force is the Gravity
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	PhysicsComponentUserData* entity = (PhysicsComponentUserData*)NewtonBodyGetUserData(body);
	NewtonBodySetForce(body, glm::value_ptr(entity->physicsSystem->get(entity->entityId)->mLocalGravity * mass));
}

void PhysicsSystem::genericContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	float contactBestSpeed = -FLT_MAX;
	glm::vec3 contactPosit;
	glm::vec3 normal;

	
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	
	PhysicsComponentUserData* entityA = (PhysicsComponentUserData*)NewtonBodyGetUserData(body0);
	PhysicsComponentUserData* entityB = (PhysicsComponentUserData*)NewtonBodyGetUserData(body1);

	PhysicsMaterialInteraction& materialInteraction = entityA->physicsSystem->getMaterialInteraction(entityA->materialId,entityB->materialId);

	for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact)) {
		dFloat contactNormalSpeed;
		NewtonMaterial* material;

		// get the material for this contact;
		material = NewtonContactGetMaterial (contact);
		// set the material properties
		NewtonMaterialSetContactFrictionCoef (material, materialInteraction.staticFriction, materialInteraction.kineticFriction, 0);
		NewtonMaterialSetContactFrictionCoef (material, materialInteraction.staticFriction, materialInteraction.kineticFriction, 1);
		NewtonMaterialSetContactElasticity (material, materialInteraction.restitution);
		NewtonMaterialSetContactSoftness (material, materialInteraction.softness);

		contactNormalSpeed = NewtonMaterialGetContactNormalSpeed (material);
		if (contactNormalSpeed > contactBestSpeed)
		{
			contactBestSpeed = contactNormalSpeed;
			NewtonMaterialGetContactPositionAndNormal (material, body0, (float*)&contactPosit, (float*)&normal);
		}
	}
	if(contactBestSpeed > -FLT_MAX)
	{
		// store collisions to emit later (thread safe)
		std::lock_guard<std::mutex> lock(entityA->physicsSystem->mCollisionMutex);
		entityA->physicsSystem->mPendingCollisions[entityA->entityId]
			.push_back(std::move(G2::Physics::Collision(entityA->entityId,entityB->entityId, contactPosit, normal, contactBestSpeed)));
		entityB->physicsSystem->mPendingCollisions[entityB->entityId]
			.push_back(std::move(G2::Physics::Collision(entityB->entityId,entityA->entityId, contactPosit, normal, contactBestSpeed)));
	}
}

unsigned long long
G2::Physics::PhysicsSystem::_combine(int id0, int id1) const 
{
	int minId = std::min(id0, id1);
	int maxId = std::max(id0, id1);
	return (((uint64_t)maxId) << 32) | ((uint64_t)minId);
}

void
PhysicsSystem::_onRemovePhysicsComponent(unsigned int entityId) 
{
	std::lock_guard<std::mutex> lock(mCollisionMutex);
	mPendingCollisions.erase(entityId);
	mCollisionCallbacks.erase(entityId);
}
void
PhysicsSystem::_onEmitCollision(G2::FrameInfo const&) 
{
	std::lock_guard<std::mutex> lock(mCollisionMutex);
	for(auto& it = mPendingCollisions.begin(); it != mPendingCollisions.end(); ++it)
	{
		for(auto& collisionIt = it->second.begin(); collisionIt != it->second.end(); ++collisionIt)
		{
			mCollisionCallbacks[it->first](std::move((*collisionIt)));
		}
		it->second.clear();
	}
}
G2::Physics::PhysicsMaterial
PhysicsSystem::createMaterial() 
{
	return NewtonMaterialCreateGroupID(mDynamicsWorld);
}
