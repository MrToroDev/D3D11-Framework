#pragma once

#include <PxPhysicsAPI.h>
#include <DirectXMath.h>

namespace DX {
	class RigidDynamic
	{
	public:
		RigidDynamic(physx::PxScene*& scene, physx::PxPhysics*& physics, physx::PxVec3 velocity, DirectX::XMMATRIX& model);
		~RigidDynamic();

		void update();

		physx::PxRigidDynamic* GetDynamic();
		float* getMatrix() const;

	private:
		physx::PxRigidDynamic* m_dynamic;
	};
}