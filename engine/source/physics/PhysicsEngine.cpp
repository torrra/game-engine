#include "physics/PhysicsEngine.h"

#pragma region Standard

#include <iostream>

#pragma endregion

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"
#include "physics/InternalPhysxDebugDraw.h"
#include "physics/SimulationEventCallback.h"

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

engine::PhysicsEngine::PhysicsEngine(void)
{
    // Create a pointer to structures of physX elements
    m_impl = new PhysicsEngineImpl();
    // Create a pointer to DebugDraw class
    m_debugDraw = new DebugDraw();
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

engine::PhysicsEngineImpl& engine::PhysicsEngine::GetImpl(void) const
{
    // Return the pointer to the structure
    return *m_impl;
}

void engine::PhysicsEngine::SetDebugLinearVelocity(f32 inScale)
{
    m_impl->m_scene->setVisualizationParameter(PxVisualParam::eBODY_LIN_VELOCITY, inScale);
}

void engine::PhysicsEngine::SetDebugContactPoint(f32 inScale)
{
    m_impl->m_scene->setVisualizationParameter(PxVisualParam::eCONTACT_POINT, inScale);
}

void engine::PhysicsEngine::SetDebugActorAxes(f32 inScale)
{
    m_impl->m_scene->setVisualizationParameter(PxVisualParam::eACTOR_AXES, inScale);
}

void engine::PhysicsEngine::Init(void)
{
    /*
        Function to create the base of physX, everything of physx is based on foundation
        Foundation manage the initialization of the memory management, prepare error
        management, and make sure physx has all what it needs to work properly.
        It prepares the environment for future objects.
        <param> Version : PhysX SDK version
        <param> allocator : Let us to not use malloc/new or free/delete directly
        <param> errorCallback : Let us to not use printf/cout directly
        Can overload those classes to have our own allocator or error log
    */
    m_impl->m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
                           gDefaultErrorCallback);

    // Initialize pvd and check if the pvd is open, if it is, then connect the pvd, else not,
    // then it's not using the pvd
    bool isPvdInitialized = InitPvd();

    // Initialize physics in regard to the pvd, if pvd is connected then use pvd
    InitPhysics(isPvdInitialized);
    
    // Initialize the scene
    InitScene();

    // Initialize the debug draw
    m_debugDraw->InitDebugDraw();

    /// TODO : Delete cout debug
    std::cout << "Physics engine initialized" << std::endl;
}

bool engine::PhysicsEngine::InitPvd(void)
{
    static bool isConnected = false;

    if (m_impl->m_foundation != nullptr)
    {
        /*
            Create an instance of physx visual debugger which is the interface of 
            communication
                - Recover internal data of physx engine
                - Send to pvd in real time
            <param> Foundation : The base of physX
        */
        m_impl->m_pvd = physx::PxCreatePvd(*m_impl->m_foundation);

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
            isConnected = m_impl->m_pvd->connect(*transport, PxInstruFlag::eALL);
        }
    }

    std::cout << "Pvd " << (isConnected ? "connected" : "not connected") << std::endl;

    if (!isConnected)
    {
        return false;
    }

    return true;
}

void engine::PhysicsEngine::InitPhysics(bool inIsPvdConnected)
{
    /*
        Will manage every physics scene, every physics object(rigid bodies, collision shapes,
        etc.), manage materials, manage constraints, etc.
        <param> Version : PhysX SDK version
        <param> Foundation : The base of physX
        <param> Scale : Define physic tolerance (size, speed, mass, etc.)
        <param> Track out standing allocation (optional) : Follow memory allocation
        <param> Pvd (optional) : Let to connect pvd for debug.
    */
    if (m_impl->m_foundation != nullptr)
    {
        if (inIsPvdConnected)
        {
            m_impl->m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_impl->m_foundation,
                                                PxTolerances(), true, m_impl->m_pvd);
        }
        else
        {
            m_impl->m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_impl->m_foundation,
                                                PxTolerances(), true);
        }
    }
}

physx::PxFilterFlags CustomFilterShader(
    physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
    physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
    (void)attributes0;          // To avoid warning
    (void)attributes1;          // To avoid warning
    (void)constantBlock;        // To avoid warning
    (void)constantBlockSize;    // To avoid warning
    // Test to check if objects are collidable
    if ((filterData0.word0 & filterData1.word1) == 0 &&
        (filterData1.word0 & filterData0.word1) == 0)
    {
        return physx::PxFilterFlag::eSUPPRESS;
    }

    // To enable contact generation
    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT |
        physx::PxPairFlag::eNOTIFY_TOUCH_FOUND |
        physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
        physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

    return physx::PxFilterFlag::eDEFAULT;
}

void engine::PhysicsEngine::InitScene(void)
{
    if (m_impl->m_physics != nullptr)
    {
        /*
            values for the tolerances in the scene, these must be the same values passed into
            PxCreatePhysics(). The affected tolerances are bounceThresholdVelocity and
            frictionOffsetThreshold
            <param> Scale : Define physic tolerance (size, speed, mass, etc.)
        */
        physx::PxSceneDesc sceneDesc(m_impl->m_physics->getTolerancesScale());
        // Set the value of the gravity by default to -9.81f
        sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

        /// TODO : Check how many threads to use in regard to our own thread pool
        /*
            Create an administrator to manage the cpu threads to execute physics simulation
            on multiple threads. Allows to distribute tasks on multiple threads
            <param> Nb threads : Number of threads
        */
        m_impl->m_dispatcher    = physx::PxDefaultCpuDispatcherCreate(2);
        sceneDesc.cpuDispatcher = m_impl->m_dispatcher;
        /*
            Decide how to manage the interaction between physics objects as
                - how they will collides
                - Have they to generate contact events
                - Have they to generate trigger events
            We use it to personalize the collision logic between objects
            <param> Attributes : Attributes of the objects (dynamic, static, etc.)
            <param> Filter : Personalized data
            <param> Flags : To determine what we want to do between the two objects
                    (trigger, collide, etc.)
            <param> Data : Constant data (mostly unused)
        */ 
        //sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
        sceneDesc.filterShader = CustomFilterShader;
        m_eventCallback = new SimulationEventCallback();
        sceneDesc.simulationEventCallback = m_eventCallback;

        /*
            Create an instance of a physic scene where all the simulation run. It's the
            simulation space.
            Every physic object will be created in this scene.
            Every interaction between objects will be calculate in this scene.
            <param> SceneDesc : The description of the scene
        */
        m_impl->m_scene = m_impl->m_physics->createScene(sceneDesc);


        /*
            Allow to create a physic material to personalize the physic behavior of the 
            objects.
            Determine wich type of objects will collide, what is the friction, what is 
            the restitution
            <param> Dynamic friction : Resistance to start a movement
            <param> Static friction : Resistance during the movement
            <param> Restitution : Bounce capacity (elasticity)
        */
        m_impl->m_material = m_impl->m_physics->createMaterial(0.5f, 0.5f, 0.6f);

        /// TODO : Check what visualization parameter we want to use
        /// TO KEEP
        /*
            Set multiple visualization parameters to display information in the pvd scene
            and OpenGL scene
        */
        // Set the debug draw scale of all the objects in the physics scene to 1 by default
        m_impl->m_scene->setVisualizationParameter(PxVisualParam::eSCALE, 1.0f);
        // Set the debug draw of the collision shapes active with their own scale to 1 by default
        m_impl->m_scene->setVisualizationParameter(PxVisualParam::eCOLLISION_SHAPES, 1.0f);
    }
}

math::Vector4f engine::PhysicsEngine::ConvertPhysxColorToVector4f(uint32 inColor)
{
    constexpr f32 denominator = 1 / 255.f;

    f32 r = (inColor & 0xFF) * denominator;
    f32 g = ((inColor >> 8) & 0xFF) * denominator;
    f32 b = ((inColor >> 16) & 0xFF) * denominator;
    f32 a = ((inColor >> 24) & 0xFF) * denominator;

    return math::Vector4f(r, g, b, a);
}



void engine::PhysicsEngine::StepSimulation(f32 inDeltaTime)
{
    /*
        Advances the simulation by an elapsedTime time.
        Do some processing until physics is computed
        <param> [in] elapsedTime : Amount of time to advance simulation by. The parameter has to be
                                   larger than 0, else the resulting behavior will be undefined. 
                                   Range: (0, PX_MAX_F32)
        <param> [in] completionTask : if non-NULL, this task will have its refcount incremented in 
                                      simulate(), then decremented when the scene is ready to have
                                      fetchResults called. So the task will not run until the 
                                      application also calls removeReference().
        <param> [in] scratchMemBlock : a memory region for physx to use for temporary data during 
                                       simulation. This block may be reused by the application 
                                       after fetchResults returns. Must be aligned on a 16-byte 
                                       boundary
        <param> [in] scratchMemBlockSize : the size of the scratch memory block. Must be a multiple
                                           of 16K.
        <param> [in] controlSimulation : if true, the scene controls its PxTaskManager simulation 
                                         state. Leave true unless the application is calling the 
                                         PxTaskManager start/stopSimulation() methods itself.
    */
    m_impl->m_scene->simulate(inDeltaTime);
    /*
        Now results of run may be retrieved.
        <param> [in] block : When set to true will block until results are available.
        <param> [out] errorState : Used to retrieve hardware error codes. A non zero value 
                                   indicates an error.

    */
    m_impl->m_scene->fetchResults(true);
}

void engine::PhysicsEngine::UpdateDebugDraw(const math::Matrix4f* inProjViewMatrix)
{
    /*
        Get the render buffer of the scene to retrieve all the lines of the body in the scene
        <param> [out] renderBuffer : Interface for points, lines, triangles, and text buffer
    */
    m_debugDraw->GetDebugDrawImpl()->m_renderBuffer = &m_impl->m_scene->getRenderBuffer();

    /*
        Update the debug draw in regard of the position of all the lines in the render buffer, and
        update their position
        <param> [in] DebugDrawImpl : the pointeur to the structure impl
    */

    Raycast::DrawAllRays();

    m_debugDraw->UpdateDebugDraw(*m_debugDraw->GetDebugDrawImpl());
    /*
        Display the lines of the render buffer into the openGL scene
        <param> [in] projViewMatrix : The projection and view matrix
        <param> [in] nbLines : The number of lines to draw multiply by 2 to retrieve the start
                               point and the end point of each line (multiply by 2 because each
                               point is the start and the end of a line)
    */

    math::Vector4f color = math::Vector4f(1.f, 0.f, 0.f, 1.f);

    if (m_debugDraw->GetDebugDrawImpl()->m_renderBuffer->getLines() != nullptr)
    {
        color = ConvertPhysxColorToVector4f(m_debugDraw->GetDebugDrawImpl()->m_renderBuffer->getLines()->color0);
    }
    if (m_debugDraw->GetDebugDrawImpl()->m_customLines.size() > 0)
    {
        math::Vector4f color2 = ConvertPhysxColorToVector4f(m_debugDraw->GetDebugDrawImpl()->m_customLines.data()->color0);
        m_debugDraw->RenderDebugDraw(inProjViewMatrix,
            (m_debugDraw->GetDebugDrawImpl()->m_renderBuffer->getNbLines() +
                (uint32)m_debugDraw->GetDebugDrawImpl()->m_customLines.size()) * 2,
            color2);
    }
    else
    {
        m_debugDraw->RenderDebugDraw(inProjViewMatrix, m_debugDraw->GetDebugDrawImpl()->m_renderBuffer->getNbLines() * 2, color);
    }

    m_debugDraw->m_debugDrawImpl->m_customLines.clear();
}

void engine::PhysicsEngine::CleanUp(void)
{
    Raycast::CleanupRays();
    Raycast::m_existingRays.~vector();

    // Delete the debug draw pointer
    delete m_debugDraw;

    // Release all physX elements in the good order
    PX_RELEASE(m_impl->m_material);

    delete m_eventCallback;
    m_eventCallback = nullptr;

    PX_RELEASE(m_impl->m_scene);
    PX_RELEASE(m_impl->m_dispatcher);
    PX_RELEASE(m_impl->m_physics);
    // Disconnect transport and pvd before realeasing them
    m_impl->m_pvd->getTransport()->disconnect();
    m_impl->m_pvd->disconnect();
    m_impl->m_pvd->getTransport()->release();
    PX_RELEASE(m_impl->m_pvd);
    PX_RELEASE(m_impl->m_foundation);

    // Delete the instance
    delete m_instance;
    m_instance = nullptr;

    /// TODO : Delete debug cout
    std::cout << "Physics engine cleaned up" << std::endl;
}

void engine::PhysicsEngine::AddDebugLine(const math::Vector3f& inStart, const math::Vector3f& inEnd, uint32 inColor)
{
    if (m_debugDraw)
    {
        m_debugDraw->AddDebugLine(inStart, inEnd, inColor);
    }
}
