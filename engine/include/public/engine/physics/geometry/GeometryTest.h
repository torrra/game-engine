#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

#include "math/VectorGeneric.hpp"
#include "math/Vector3.hpp"

namespace physx 
{
	class PxFoundation;
	class PxPhysics;
	class PxScene;
	class PxDefaultCpuDispatcher;
	class PxMaterial;
	class PxPvd;
	class PxPvdTransport;
	class PxBoxGeometry;
	class PxRigidDynamic;
	template<typename T>
	class PxVec3T;

} // !Namespace physx

namespace engine
{
	class Physics
	{
	public :

		ENGINE_API Physics(void);
		Physics(const Physics& other) = delete;

		ENGINE_API ~Physics(void);

		ENGINE_API void StepSimulation(f32 inDeltaTime);

		ENGINE_API physx::PxPhysics* GetPhysics(void) const;
		ENGINE_API physx::PxScene* GetScene(void) const;
		ENGINE_API physx::PxMaterial* GetMaterial(void) const;

	private :


		physx::PxFoundation* m_foundation = nullptr;
		physx::PxPhysics* m_physics = nullptr;
		physx::PxScene* m_scene = nullptr;
		physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
		physx::PxMaterial* m_material = nullptr;
		physx::PxPvd* m_pvd = nullptr;
		physx::PxPvdTransport* m_transport = nullptr;

	}; // !Class Physics

	class RigidBody
	{
	public :

		ENGINE_API RigidBody(physx::PxPhysics* inPhysics, physx::PxScene* inScene, 
			physx::PxMaterial* inMaterial, math::Vector3f inPosition/*,
			f32 inMass*/);

		ENGINE_API void SetVelocity(const math::Vector3f& inVelocity);

		ENGINE_API physx::PxRigidDynamic* GetRigidDynamicBody(void) const;

		ENGINE_API math::Vector3f GetPosition(void) const;

	private :

		physx::PxRigidDynamic* m_rigidDynamicBody = nullptr;

	}; // !Class RigidBody
} // !Namespace engine

ENGINE_API physx::PxVec3T<float> ToPxVec3(const math::Vector3f& inVector);
ENGINE_API math::Vector3f ToVector3(const physx::PxVec3T<float>& inVector);