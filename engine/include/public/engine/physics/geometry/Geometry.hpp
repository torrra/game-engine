#pragma once

namespace engine
{
	struct GeometryImpl;

	enum EGeometryType : uint64
	{
		BOX = 0,
		SPHERE = 1,
		CAPSULE = 2,
		PLANE = 3

	}; // !Enum EGeometryType

    enum EShapeType : uint64
    {
        DYNAMIC = 0,
        STATIC = 1
    };

    enum EErrorGeometryType
    {
        EErrorGeometryType_None = -1,
        EErrorGeometryType_Invalid = 0
    };// !Enum EErrorGeometryType
} // !Namespace engine