#pragma once

namespace DX
{
	enum class MemoryType
	{
		eNewPtr,
		eNewPtrArray,
		eMallocPtr
	};

	class Memory
	{
	public:
		template<typename T>
		static void Destroy(T object);
			
		template<MemoryType type = MemoryType::eNewPtr, typename T>
		static void Destroy(T* object);
	};
}

#include "Memory.inl"