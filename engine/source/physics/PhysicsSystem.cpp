#include "physics/PhysicsSystem.h"

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static physx::PxDefaultErrorCallback gDefaultErrorCallback;

engine::PhysicsSystem::PhysicsSystem(void)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
									  gDefaultErrorCallback);
	m_Pvd = physx::PxCreatePvd(*m_Foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 
																				5425, 10);
	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(),
								true, m_Pvd);
}
