#pragma once

#include <DirectXMath.h>

namespace DX 
{
	class Camera
	{
	public:
		Camera();
		void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

		const DirectX::XMMATRIX& GetViewMatrix() const;
		const DirectX::XMMATRIX& GetProjectionMatrix() const;

		const DirectX::XMVECTOR& GetPositionVector() const;
		const DirectX::XMFLOAT3& GetPositionFloat3() const;
		const DirectX::XMVECTOR& GetRotationVector() const;
		const DirectX::XMFLOAT3& GetRotationFloat3() const;

		void SetPosition(const DirectX::XMVECTOR& pos);
		void SetPosition(float x, float y, float z);
		void AdjustPosition(const DirectX::XMVECTOR& pos);
		void AdjustPosition(float x, float y, float z);
		void SetRotation(const DirectX::XMVECTOR& rot);
		void SetRotation(float x, float y, float z);
		void AdjustRotation(const DirectX::XMVECTOR& rot);
		void AdjustRotation(float x, float y, float z);

		const DirectX::XMVECTOR& GetForwardVector();
		const DirectX::XMVECTOR& GetBackwardVector();
		const DirectX::XMVECTOR& GetLeftVector();
		const DirectX::XMVECTOR& GetRightVector();
		
	private:
		void UpdateViewMatrix();
		DirectX::XMVECTOR posVector;
		DirectX::XMVECTOR rotVector;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 rot;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;

		const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		DirectX::XMVECTOR vec_forward;
		DirectX::XMVECTOR vec_right;
		DirectX::XMVECTOR vec_backward;
		DirectX::XMVECTOR vec_left;
	};
}