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

    enum EShapeType : uint32
    {
        DYNAMIC = 0,
        STATIC = 1
    };

    enum EErrorGeometryType
    {
        EErrorGeometryType_None = -1,
        EErrorGeometryType_Invalid = 0
    };// !Enum EErrorGeometryType

    enum EForceMode : uint32
    {
        FORCE = 0,
        IMPULSE = 1 << 1,
        VELOCITY_CHANGE = 1 << 2,
        ACCELERATION = 1 << 3
    }; // !Enum EForceMode

    struct RigidBodyData
    {
        uint32 m_index;
        uint32 m_type;

    }; // !Struct RigidBodyDynamicData

    inline uint64 MakeRigidBodyIndex(uint32 inIndex, EShapeType inShapeType)
    {
        if (inIndex >= ULONG_MAX || inShapeType >= ULONG_MAX)
            return ULLONG_MAX;

        return (static_cast<uint64>(inShapeType) << 32) | static_cast<uint64>(inIndex);
    }

    inline uint32 GetRigidBodyIndex(uint64 inIndex)
    {
        return static_cast<uint32>(inIndex & 0xFFFFFFFF);
    }

    inline uint32 GetShapeType(uint64 inShapeType)
    {
        return static_cast<uint32>((inShapeType >> 32) & 0xFFFFFFFF);
    }

} // !Namespace engine