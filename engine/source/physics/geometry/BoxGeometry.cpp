#include "physics/geometry/BoxGeometry.h"

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

struct engine::GeometryImpl
{
	physx::PxGeometry* m_geometry = nullptr;
};

engine::BoxGeometry::BoxGeometry()
{
	m_geometryImpl = new GeometryImpl();

	if (m_geometryImpl)
	{
		m_geometryImpl->m_geometry = new physx::PxBoxGeometry();
	}
}

engine::BoxGeometry::BoxGeometry(float inWidth, float inHeight, float inDepth)
{
	m_geometryImpl = new GeometryImpl();

	if (m_geometryImpl)
	{
		m_geometryImpl->m_geometry = new physx::PxBoxGeometry(inWidth / 2, inHeight / 2, inDepth / 2);
	}
}

engine::BoxGeometry::~BoxGeometry()
{
	delete m_geometryImpl->m_geometry;
	m_geometryImpl->m_geometry = nullptr;
	delete m_geometryImpl;
	m_geometryImpl = nullptr;
}

void* engine::BoxGeometry::GetNativeGeometry() const
{
	return m_geometryImpl->m_geometry;
}

float engine::BoxGeometry::GetVolume() const
{
	const physx::PxVec3& halfExtents = static_cast<physx::PxBoxGeometry*>(m_geometryImpl->m_geometry)->halfExtents;
	return halfExtents.x * halfExtents.y * halfExtents.z * 8;
}
