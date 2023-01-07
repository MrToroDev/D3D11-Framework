#pragma once

#include <DirectXMath.h>
#include <cmath>
#include <algorithm>

namespace DX
{
	class Math
	{
	public:
		static DirectX::XMMATRIX Inverse(const DirectX::CXMMATRIX M);

		static DirectX::XMMATRIX Billboard(DirectX::XMFLOAT3 cameraPos, DirectX::XMFLOAT3 cameraUp, DirectX::XMFLOAT3* cameraForward, DirectX::XMFLOAT3 modelPosition);
	
		template<typename T>
		static inline T Random(T min, T max)
		{
			assert(max > min);
			T random = ((T)rand()) / (T)RAND_MAX;

			T range = max - min;
			return (random * range) + min;
		}
	};
}