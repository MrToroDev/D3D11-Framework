#pragma once

#include "../EntityComponentSystem.h"

namespace DX
{
	struct TransformPosition { TransformPosition(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {} float x, y, z; };
	struct TransformSize { TransformSize(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {} float x, y, z; };
	struct TransformRotation { TransformRotation(float _x, float _y, float _z, float _value) : x(_x), y(_y), z(_z), value(_value) {} float x, y, z, value; };

	class Transform : public Component
	{
	public:
		Transform() : Position(0, 0, 0), Size(1, 1, 1), Rotation(1, 0, 0, 0) {}
		Transform(TransformPosition pos) : Position(pos), Size(1, 1, 1), Rotation(1, 0, 0, 0) {}
		Transform(TransformPosition pos, TransformSize size) : Position(pos), Size(size), Rotation(1, 0, 0, 0) {}
		Transform(TransformPosition pos, TransformSize size, TransformRotation rotation) : Position(pos), Size(size), Rotation(rotation) {}

		TransformPosition Position;
		TransformSize Size;
		TransformRotation Rotation;
	};
}