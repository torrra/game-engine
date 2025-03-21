#include "engine/physics/Material.h"

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

#pragma region Physics

#include "physics/InternalPhysXStruct.hpp"

#pragma endregion

#pragma region Standard

#include <iostream>

#pragma endregion

engine::Material::Material(const PhysicsEngine& inPhysicsEngine, f32 inDynamicFriction,
						   f32 inStaticFriction, f32 inRestitution)
{
	m_materialImpl = new MaterialImpl();

	m_materialImpl->m_material = inPhysicsEngine.GetImpl().m_physics->createMaterial(
								 inDynamicFriction, inStaticFriction, inRestitution);

	std::cout << "Material created to given values" << std::endl;
}

engine::Material::~Material(void)
{
	delete m_materialImpl;
	m_materialImpl = nullptr;
}

engine::MaterialImpl& engine::Material::GetImpl(void) const
{
	return *m_materialImpl;
}

f32 engine::Material::GetDynamicFriction(void) const
{
	return m_materialImpl->m_material->getDynamicFriction();
}

f32 engine::Material::GetStaticFriction(void) const
{
	return m_materialImpl->m_material->getStaticFriction();
}

f32 engine::Material::GetRestitution(void) const
{
	return m_materialImpl->m_material->getRestitution();
}

void engine::Material::SetDynamicFriction(f32 inDynamicFriction)
{
	m_materialImpl->m_material->setDynamicFriction(inDynamicFriction);
}

void engine::Material::SetStaticFriction(f32 inStaticFriction)
{
	m_materialImpl->m_material->setStaticFriction(inStaticFriction);
}

void engine::Material::SetRestitution(f32 inRestitution)
{
	m_materialImpl->m_material->setRestitution(inRestitution);
}

void engine::Material::SetMaterial(f32 inDynamicFriction, f32 inStaticFriction,
								   f32 inRestitution)
{
	m_materialImpl->m_material->setDynamicFriction(inDynamicFriction);
	m_materialImpl->m_material->setStaticFriction(inStaticFriction);
	m_materialImpl->m_material->setRestitution(inRestitution);
}
