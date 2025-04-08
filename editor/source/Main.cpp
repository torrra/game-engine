#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/rigidbody/RigidBodyDynamic.h"
#include "engine/physics/rigidbody/RigidBodyStatic.h"

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#include "engine/core/SceneGraph.h"
#include "engine/core/Entity.h"
#include "engine/core/components/Transform.h"

#include "engine/core/systems/ScriptSystem.h"

#include "Example.h"

#include "engine/resource/ResourceManager.h"
#include "engine/resource/model/Model.h"

#include "engine/ConsoleLog.hpp"

#define MODEL_FILE "..\\assets\\bunny.obj"

// Use dedicated graphics card
extern "C" 
{
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	// Memory check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		engine::Engine engine;
		
		if (engine.Startup("Editor", nullptr))
			return -1;

		ExampleProject project;
		project.StartUp(engine);

        /// ---------------- Create entity ---------------- 
        engine::EntityHandle floor = engine.GetGraph()->CreateEntity("Floor");

        /// ---------------- PhysicsEngine use ---------------- 
        engine::PhysicsEngine::Get().Init();

        /// ---------------- Create rigidbody dynamic ----------------
        std::cout << "\t" << std::endl;

        engine::RigidBodyDynamic* rb = engine::RigidBodyDynamicFactory::CreateDynamic(engine.GetGraph(), 
                                            engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine::BOX);
        rb->SetDebugVisualization(true);

        /// ---------------- Create rigidbody static ----------------
        std::cout << "\t" << std::endl;
        
        engine::RigidBodyStatic* floorRb = engine::RigidBodyStaticFactory::CreateStatic(
                                                engine.GetGraph(), floor, engine::BOX);
        floorRb->SetBoxHalfExtents(math::Vector3f (5.f, 0.5f, 5.f));
        floorRb->SetDebugVisualization(true);

        engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(engine.GetGraph()->GetEntity("Camera")->GetHandle());

		while (!engine.GetWindow()->ShouldWindowClose())
		{
            math::Matrix4f projViewMatrix = camera->ViewProjection();
            // Update physics
            engine::PhysicsEngine::Get().StepSimulation(1 / 600.f);
            // Update the entity in regard to the rigid body (gravity for example)
            rb->UpdateEntity(engine.GetGraph()->GetEntity("Padoru")->GetHandle());
			project.Update(engine);
			engine.Update();
            // Update the debug draw
            engine::PhysicsEngine::Get().UpdateDebugDraw(&projViewMatrix);
            // Update the rigid body in regard to the entity (movement by keyboard input for example)
            rb->UpdateRigidBody(engine.GetGraph()->GetEntity("Padoru")->GetHandle());

            engine.GetWindow()->Update();
		}

        /// ---------------- Clean ---------------- 
        delete rb;
        delete floorRb;
        engine::PhysicsEngine::Get().CleanUp();

		engine.ShutDown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		engine::PrintLog(engine::SuccessPreset(), "No memory leak detected.");

	return 0;
}