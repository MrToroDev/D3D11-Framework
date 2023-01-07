#include "RigidDynamic.h"

DX::RigidDynamic::RigidDynamic(
    physx::PxScene*& scene, 
    physx::PxPhysics*& physics, 
    physx::PxVec3 velocity,
    DirectX::XMMATRIX& model)
{
    DirectX::XMVECTOR pos, scale, rotQuad;
    DirectX::XMMatrixDecompose(&scale, &rotQuad, &pos, model);
    physx::PxMaterial* mMaterial = physics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxTransform t = physx::PxTransform(physx::PxVec3(
        DirectX::XMVectorGetX(pos),
        DirectX::XMVectorGetY(pos),
        DirectX::XMVectorGetZ(pos)
    ));
    m_dynamic = physics->createRigidDynamic(t);
    physx::PxRigidActorExt::createExclusiveShape(*m_dynamic, physx::PxBoxGeometry(0.5, 0.5, 0.5), *mMaterial);
    m_dynamic->setAngularDamping(0.5f);
    m_dynamic->setLinearVelocity(velocity);
    scene->addActor(*m_dynamic);
    mMaterial->release();
}

DX::RigidDynamic::~RigidDynamic()
{

}

void DX::RigidDynamic::update()
{
    
}

physx::PxRigidDynamic* DX::RigidDynamic::GetDynamic()
{
    return m_dynamic;
}

float* DX::RigidDynamic::getMatrix() const
{
    return nullptr;
}
