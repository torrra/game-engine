#pragma once

namespace engine
{
	struct GeometryImpl;

	enum EGeometryType
	{
		BOX,
		SPHERE,
		CAPSULE,
		PLANE

	}; // !Enum EGeometryType

    enum EErrorGeometryType
    {
        EErrorGeometryType_None = -1,
        EErrorGeometryType_Invalid = 0
    };// !Enum EErrorGeometryType
} // !Namespace engine