#include "engine/physics/PhysicsMaterial.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"

#pragma endregion

engine::Material::Material(const PhysicsEngine& inPhysicsEngine, f32 inStaticFriction, 
                           f32 inDynamicFriction, f32 inRestitution)
{
    // Create the pointer to the structure of material implementation
    m_materialImpl = new MaterialImpl();
    /*
        Create the material with the parameters asked
        <param> [in] StaticFriction : coefficient of static friction
        <param> [in] DynamicFriction : coefficient of dynamic friction
        <param> [in] Restitution : coefficient of restitution
    */
    m_materialImpl->m_material = inPhysicsEngine.GetImpl().m_physics->createMaterial(
                                        inStaticFriction, inDynamicFriction, inRestitution);
}

engine::Material::~Material(void)
{
    // Delete the pointer to the structure
    delete m_materialImpl;
    m_materialImpl = nullptr;
}

engine::MaterialImpl& engine::Material::GetImpl(void) const
{
    // Return the pointer to the structure
    return *m_materialImpl;
}

f32 engine::Material::GetDynamicFriction(void) const
{
    // Return the coefficient of dynamic friction
    return m_materialImpl->m_material->getDynamicFriction();
}

f32 engine::Material::GetStaticFriction(void) const
{
    // Return the coefficient of static friction
    return m_materialImpl->m_material->getStaticFriction();
}

f32 engine::Material::GetRestitution(void) const
{
    // Return the coefficient of restitution
    return m_materialImpl->m_material->getRestitution();
}

void engine::Material::SetDynamicFriction(f32 inDynamicFriction)
{
    // Set the coefficient of dynamic friction
    m_materialImpl->m_material->setDynamicFriction(inDynamicFriction);
}

void engine::Material::SetStaticFriction(f32 inStaticFriction)
{
    // Set the coefficient of static friction
    m_materialImpl->m_material->setStaticFriction(inStaticFriction);
}

void engine::Material::SetRestitution(f32 inRestitution)
{
// Set the coefficient of restitution
    m_materialImpl->m_material->setRestitution(inRestitution);
}

void engine::Material::SetMaterial(f32 inDynamicFriction, f32 inStaticFriction,
                                   f32 inRestitution)
{
    /*
        Set the material with the parameters asked
        <param> [in] StaticFriction : coefficient of static friction
        <param> [in] DynamicFriction : coefficient of dynamic friction
        <param> [in] Restitution : coefficient of restitution
    */
    m_materialImpl->m_material->setStaticFriction(inStaticFriction);
    m_materialImpl->m_material->setDynamicFriction(inDynamicFriction);
    m_materialImpl->m_material->setRestitution(inRestitution);
}
