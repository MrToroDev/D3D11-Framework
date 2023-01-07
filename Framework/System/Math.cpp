#include "Math.h"

DirectX::XMMATRIX DX::Math::Inverse(const DirectX::CXMMATRIX M)
{
	DirectX::XMVECTOR det = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	return DirectX::XMMatrixInverse(&det, M);
}

DirectX::XMMATRIX DX::Math::Billboard(DirectX::XMFLOAT3 cameraPos, DirectX::XMFLOAT3 cameraUp, DirectX::XMFLOAT3* cameraForward, DirectX::XMFLOAT3 modelPosition)
{
    using namespace DirectX;
    XMVECTOR O = XMLoadFloat3(&modelPosition);
    XMVECTOR C = XMLoadFloat3(&cameraPos);
    XMVECTOR Z = XMVectorSubtract(O, C);

    XMVECTOR N = XMVector3LengthSq(Z);
    if (XMVector3Less(N, g_XMEpsilon))
    {
        if (cameraForward)
        {
            XMVECTOR F = XMLoadFloat3(cameraForward);
            Z = XMVectorNegate(F);
        }
        else
            Z = g_XMNegIdentityR2;
    }
    else
    {
        Z = XMVector3Normalize(Z);
    }

    XMVECTOR up = XMLoadFloat3(&cameraUp);
    XMVECTOR X = XMVector3Cross(up, Z);
    X = XMVector3Normalize(X);

    XMVECTOR Y = XMVector3Cross(Z, X);

    XMMATRIX M;
    M.r[0] = X;
    M.r[1] = Y;
    M.r[2] = Z;
    M.r[3] = XMVectorSetW(O, 1.f);

    return M;
}
