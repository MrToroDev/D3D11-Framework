#pragma once

#include <PxPhysicsAPI.h>

struct PhysicWorld
{
	~PhysicWorld();

	physx::PxDefaultAllocator mDefaultAllocatorCallback;
	physx::PxDefaultErrorCallback mDefaultErrorCallback;
	physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
	physx::PxTolerancesScale mToleranceScale;

	physx::PxFoundation* mFoundation = NULL;
	physx::PxPhysics* mPhysics = NULL;
	physx::PxScene* mScene = NULL;
	physx::PxPvd* mPvd = NULL;

	void Initialize(float gravity);
};