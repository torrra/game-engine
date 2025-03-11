#include "physics/PhysicsSystem.h"

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

// Struct to do forward declaration to perfect encapsulation
// PxFoundation, PxPvd, PxPhysics
struct engine::PhysicsSystemHandle
{
	physx::PxFoundation*	m_Foundation = nullptr;
	physx::PxPhysics*		m_Physics = nullptr;
	physx::PxPvd*			m_Pvd = nullptr;
};

#pragma region Standard

#include <iostream>

#pragma endregion

static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static physx::PxDefaultErrorCallback gDefaultErrorCallback;

engine::PhysicsSystem::PhysicsSystem(void)
{
	m_PhysicHandle = new PhysicsSystemHandle();
	/*
		Create an instance of the foundation class
		Need to be created before instance cause of an error message
		<param> version of the API
		<param> allocator interface required by the SDK
		<param> error callback provide to enable the SDK to be started with the
				minimum of user code
	*/
	if ((m_PhysicHandle->m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION,
		gDefaultAllocatorCallback, gDefaultErrorCallback)) == NULL)
	{
		std::cout << "PxCreateFoundation failed" << std::endl;
		return;
	}
	/*
		Create a Pvd instance
		<param> foundation is the foundation instance that stores the allocator and
				error callback
	*/
	m_PhysicHandle->m_Pvd = physx::PxCreatePvd(*m_PhysicHandle->m_Foundation);
	/*
		Create a default socket transport
		<param> host, host address of the pvd application
		<param> port, ip port used for pvd, should same as the port setting in pvd application
		<param> timeout in milliseconds timeout when connect to pvd host
	*/
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 
																				5425, 10);
	/*
		Connects the SDK to the PhysX Visual Debugger application
		<param> transport for pvd captured data
		<param> flags to set
		<return> True if success
	*/
	m_PhysicHandle->m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	/*
		Create an instance of the physics SDK
		<param> version of the API
		<param> foundation instance
		<param> physics tolerances used to determine default tolerances for object at creation
		<param> trackOutstandingAllocations true to track memory allocations
		<param> pvd points to a valid pvd instance, could be null if doesn't want a connection
		<param> omniPvd points to a valid omnipvd instance will sample its internal structures 
				to the defined OmniPvd output streams
		<return> PxPhysics instance on success, NULL if operation failed
	*/
	m_PhysicHandle->m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, 
												*m_PhysicHandle->m_Foundation,
												physx::PxTolerancesScale(), 
												true, m_PhysicHandle->m_Pvd);
}

engine::PhysicsSystem::~PhysicsSystem(void)
{
	m_PhysicHandle->m_Pvd->release();
	m_PhysicHandle->m_Physics->release();
	m_PhysicHandle->m_Foundation->release();

	delete m_PhysicHandle;
}
