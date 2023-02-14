#include "Memory.h"
template<typename T>
inline void DX::Memory::Destroy(T object)
{
	if (object) {
		object.Reset();
		object = nullptr;
	}
}

template<DX::MemoryType type, typename T>
inline void DX::Memory::Destroy(T* object)
{
	switch (type)
	{
	case DX::MemoryType::eNewPtr:
		if (object) delete object;
		break;
	case DX::MemoryType::eNewPtrArray:
		if (object) delete[] object;
		break;
	case DX::MemoryType::eMallocPtr:
		if (object) free(object);
		break;
	}
}