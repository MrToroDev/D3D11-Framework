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

	namespace hlsl
	{
		struct float2 { float2(float _x, float _y) : x(_x), y(_y) {} float x, y; };
		struct float3 { float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {} float x, y, z; };
		struct float4 { float x, y, z, w; };
	}
}