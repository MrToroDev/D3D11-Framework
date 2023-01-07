#include "RigidStatic.h"

DX::RigidStatic::RigidStatic(physx::PxScene*& scene, physx::PxPhysics*& physic, DirectX::XMMATRIX& model)
	: m_matrix(model)
{
    DirectX::XMVECTOR pos, scale, rotQuad;
    DirectX::XMMatrixDecompose(&scale, &rotQuad, &pos, model);
    physx::PxMaterial* mat = physic->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxTransform t = physx::PxTransform(physx::PxVec3(
        DirectX::XMVectorGetX(pos),
        DirectX::XMVectorGetY(pos),
        DirectX::XMVectorGetZ(pos)
    ));
    m_static = physx::PxCreateStatic(*physic, t, physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *mat);
    scene->addActor(*m_static);
    mat->release();
}

DX::RigidStatic::~RigidStatic()
{
}

void DX::RigidStatic::UpdatePos()
{
}
