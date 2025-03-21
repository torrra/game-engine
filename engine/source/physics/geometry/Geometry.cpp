#include "engine/physics/geometry/Geometry.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"

#pragma endregion

engine::Geometry::Geometry(void)
{
	m_geometryImpl = new GeometryImpl();
	m_geometryImpl->m_geometry = new physx::PxBoxGeometry(1.0f, 1.0f, 1.0f);
}

engine::Geometry::Geometry(EGeometryType geometryType)
{
	m_geometryImpl = new GeometryImpl();
	
	switch (geometryType)
	{
		case EGeometryType::BOX :
			m_geometryImpl->m_geometry = new physx::PxBoxGeometry(1.0f, 1.0f, 1.0f);
			break;
		case EGeometryType::SPHERE :
			m_geometryImpl->m_geometry = new physx::PxSphereGeometry(1.0f);
			break;
		case EGeometryType::CAPSULE :
			m_geometryImpl->m_geometry = new physx::PxCapsuleGeometry(1.0f, 1.0f);
			break;
	}
}

engine::Geometry::~Geometry(void)
{
	delete m_geometryImpl->m_geometry;
	delete m_geometryImpl;
	m_geometryImpl = nullptr;
}

engine::GeometryImpl& engine::Geometry::GetGeometryImpl(void) const
{
	return *m_geometryImpl;
}
