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

        /// ---------------- Create material ----------------
        engine::Material* floorMaterial = new engine::Material(0.5f, 0.5f, 0.f);

        /// ---------------- Create rigidbody dynamic ----------------
        engine::RigidBodyDynamic* rb = engine::RigidBodyDynamicFactory::Create(engine.GetGraph(), 
                                            engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine::SPHERE);
        //rb->SetGravityDisabled(true);
        //std::cout << rb->GetBoxHalfExtents() << std::endl;
        //std::cout << rb->GetCapsuleRadius() << std::endl;
        //std::cout << rb->GetSphereRadius() << std::endl;

        engine::printLog(engine::errorPreset(), "RigidBodyDynamic error !");
        engine::printLog(engine::warningPreset(), "RigidBodyDynamic warning !");
        engine::printLog(engine::infoPreset(), "RigidBodyDynamic info !");


        rb->SetBoxHalfExtents(math::Vector3f(2.f, 2.f, 2.f));

        /// ---------------- Create rigidbody static ----------------
        engine::RigidBodyStatic* floorRigidBody = new engine::RigidBodyStatic(floor, engine.GetGraph());
        floorRigidBody->CreatePlaneStaticRigidBody(engine::PhysicsEngine::Get(), *floorMaterial);

        engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(engine.GetGraph()->GetEntity("Camera")->GetHandle());

		while (!engine.GetWindow()->ShouldWindowClose())
		{
            math::Matrix4f projViewMatrix = camera->ViewProjection();
			project.Update(engine);
			engine.Update();
            engine::PhysicsEngine::Get().StepSimulation(0.001f);
            engine::PhysicsEngine::Get().UpdateDebugDraw(&projViewMatrix);
            rb->UpdateEntity(engine.GetGraph()->GetEntity("Padoru")->GetHandle());
            rb->UpdateRigidBody(*engine.GetGraph()->GetComponent<engine::Transform>(engine.GetGraph()->GetEntity("Padoru")->GetHandle()));

            engine.GetWindow()->Update();
		}

        /// ---------------- Clean ---------------- 
        delete rb;
        delete floorRigidBody;
        delete floorMaterial;
        engine::PhysicsEngine::Get().CleanUp();

		engine.ShutDown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks found\n");

	return 0;
}