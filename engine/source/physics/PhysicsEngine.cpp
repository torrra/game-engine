#include "physics/PhysicsEngine.h"

#pragma region Standard

#include <iostream>

#pragma endregion

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

/*
	PxDefaultAllocator : memory allocation management
	PxDefaultErrorCallback : Error/internal log management of physx
	Use to allocate and free memory, receive error, warning, assertion etc.
*/
physx::PxDefaultAllocator		gDefaultAllocatorCallback;
physx::PxDefaultErrorCallback	gDefaultErrorCallback;

// Set the instance to nullptr
engine::PhysicsEngine* engine::PhysicsEngine::m_instance = nullptr;

// Create a structure of physX elements to store them
struct engine::PhysicsEngineImpl 
{
	physx::PxFoundation*			m_foundation	= nullptr;
	physx::PxPhysics*				m_physics		= nullptr;
	physx::PxScene*					m_scene			= nullptr;
	physx::PxDefaultCpuDispatcher*	m_dispatcher	= nullptr;
	physx::PxMaterial*				m_material		= nullptr;
	physx::PxPvd*					m_pvd			= nullptr;

}; // !Struct PhysicsEngineImpl

engine::PhysicsEngine::PhysicsEngine()
{
	// Create a pointer to the structure of physX elements
	m_impl = new PhysicsEngineImpl();
}

engine::PhysicsEngine::~PhysicsEngine(void)
{
	// Delete the pointer to the structure of physX elements
	delete m_impl;
}

engine::PhysicsEngine& engine::PhysicsEngine::Get(void)
{
	if (m_instance == nullptr)
	{
		// Create the instance if was not created
		m_instance = new engine::PhysicsEngine();
	}

	return *m_instance;
}

void engine::PhysicsEngine::Init(void)
{
	PhysicsEngineImpl& impl = *m_impl;

	/*
		Founction to create the base of physX, everything of physx is based on foundation
		Foundation manage the initialization of the memory management, prepare error
		management, and make sure physx has all what it needs to work properly.
		It prepares the environment for future objects.
		<param> Version : PhysX SDK version
		<param> allocator : Let us to not use malloc/new or free/delete directly
		<param> errorCallback : Let us to not use printf/cout directly
		Can overload those classes to have our own allocator or error log
	*/
	impl.m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
										   gDefaultErrorCallback);

	/*
		Create an instance of physx visual debugger which is the interface of communication
			- Recover internal data of physx engine
			- Send to pvd in real time
		<param> Foundation : The base of physX
	*/
	if (impl.m_foundation != nullptr)
	{
		impl.m_pvd = physx::PxCreatePvd(*impl.m_foundation);
	}

	/*
		Allows to send to the pvd in real time to be able to understand errors
			- Rigid bodies position
			- Collision shapes
			- Constraints applied
			- etc
		<param> Host : Address IP (most of the time 127.0.0.1)
		<param> Port : Port number (most of the time 5425)
		<param> Timeout in milliseconds : Timeout of the connection
	*/
	physx::PxPvdTransport* transport;
	transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	/*
		Activate the connection to the pvd, determine what type of data to send and check
		if the connection is successful
		<param> Transport : The transport to connect previously created
		<param> Flags : What type of data to send (eALL to send everything)
	*/
	if (transport != nullptr)
	{
		impl.m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	}

	/*
		Will manage every physics scene, every physics object(rigid bodies, collision shapes, 
		etc.), manage materials, manage constraints, etc.
		<param> Version : PhysX SDK version
		<param> Foundation : The base of physX
		<param> Scale : Define physic tolerance (size, speed, mass, etc.)
		<param> Track out standing allocation (optional) : Follow memory allocation
		<param> Pvd (optional) : Let to connect pvd for debug.
	*/
	impl.m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *impl.m_foundation,
									 physx::PxTolerancesScale(), true, impl.m_pvd);

	if (impl.m_physics != nullptr)
	{
		/*
			values for the tolerances in the scene, these must be the same values passed into
			PxCreatePhysics(). The affected tolerances are bounceThresholdVelocity and
			frictionOffsetThreshold
			<param> Scale : Define physic tolerance (size, speed, mass, etc.)
		*/
		physx::PxSceneDesc sceneDesc(impl.m_physics->getTolerancesScale());
		// Set the value of the gravity by default to -9.81f
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

		/// TODO : Check how many threads to use in regard to our own thread pool
		/*
			Create an administrator to manage the cpu threads to execute physics simulation
			on multiple threads. Allows to distribute tasks on multiple threads
			<param> Nb threads : Number of threads
		*/
		impl.m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = impl.m_dispatcher;
		/*
			Decide how to manage the interaction between physics objects as
				- how the will collides
				- Have they to generate contact events
				- Have they to generate trigger events
			We use it to personalize the collision logic between objects
			<param> Attributes : Attributes of the objects (dynamic, static, etc.)
			<param> Filter : Personalized data
			<param> Flags : To determine what we want to do between the two objects
					(trigger, collide, etc.)
			<param> Data : Constant data (mostly unused)
		*/
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

		/*
			Create an instance of a physic scene where all the simulation run. It's the
			simulation space.
			Every physic object will be created in this scene.
			Every interaction between objects will be calculate in this scene.
			<param> SceneDesc : The description of the scene
		*/
		impl.m_scene = impl.m_physics->createScene(sceneDesc);
	}
	
	/*
		Allow to create a physic material to personalize the physic behavior of the objects.
		Determine wich type of objects will collide, what is the friction, what is the restitution
		<param> Dynamic friction : Resistance to start a movement
		<param> Static friction : Resistance during the movement
		<param> Restitution : Bounce capacity (elasticity)
	*/
	impl.m_material = impl.m_physics->createMaterial(0.5f, 0.5f, 0.6f);
}

void engine::PhysicsEngine::CleanUp(void)
{
	// Release all physX elements in the good order
	PhysicsEngineImpl& impl = *m_impl;
	PX_RELEASE(impl.m_material);
	PX_RELEASE(impl.m_scene);
	PX_RELEASE(impl.m_dispatcher);
	PX_RELEASE(impl.m_physics);
	// Disconnect transport and pvd before realeasing them
	impl.m_pvd->getTransport()->disconnect();
	impl.m_pvd->disconnect();
	impl.m_pvd->getTransport()->release();
	PX_RELEASE(impl.m_pvd);
	PX_RELEASE(impl.m_foundation);

	// Delete the instance
	delete m_instance;
	m_instance = nullptr;
}