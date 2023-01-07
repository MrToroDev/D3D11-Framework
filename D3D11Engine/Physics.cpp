#include "Physics.h"
#include <Logger.h>

PhysicWorld::~PhysicWorld()
{
	mScene->release();
	mDispatcher->release();
	mPhysics->release();
	mFoundation->release();
}

void PhysicWorld::Initialize(float gravity)
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
	if (!mFoundation) {
		DX::LogError("PxCreateFoundation failed to create object!");
		return;
	}
	mPvd = PxCreatePvd(*mFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	mToleranceScale.length = 100;        // typical length of an object
	mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
	if (!mPhysics) {
		DX::LogError("PxCreatePhysics failed to create object!");
		return;
	}

	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, gravity, 0.0f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	mScene = mPhysics->createScene(sceneDesc);
	if (!mScene) {
		DX::LogError("PxPhysics::createScene failed to create object!");
		return;
	}

	physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	physx::PxMaterial* mat = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	physx::PxRigidStatic* ground = physx::PxCreatePlane(
		*mPhysics,
		physx::PxPlane(0, 1, 0, 0),
		*mat
	);
	mScene->addActor(*ground);

	DX::Log("PhysX initialized correctly! PhysX Version: 4.1");
}