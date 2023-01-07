#pragma once

#include <PxPhysicsAPI.h>
#include <DirectXMath.h>

namespace DX {
	class RigidStatic
	{
	public:
		RigidStatic(physx::PxScene*& scene, physx::PxPhysics*& physic, DirectX::XMMATRIX& model);
		~RigidStatic();

		void UpdatePos();

	private:
		DirectX::XMMATRIX& m_matrix;
		physx::PxRigidStatic* m_static = nullptr;
	};
}