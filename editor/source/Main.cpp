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
        //engine::EntityHandle object = engine.GetGraph()->CreateEntity("First");
        //engine.GetGraph()->CreateComponent<engine::Transform>(object)->SetTransform(
        //    math::Vector3f(0.f, 20.f, -20.f),
        //    math::Quatf(1.f, 0.f, 0.f, 0.f));
        engine::EntityHandle floor = engine.GetGraph()->CreateEntity("Floor");

        /// ---------------- PhysicsEngine use ---------------- 
        engine::PhysicsEngine::Get().Init();

        /// ---------------- Create material ----------------
        engine::Material* material = new engine::Material(engine::PhysicsEngine::Get());
        material->SetMaterial(0.5f, 0.5f, 0.6f);

        engine::Material* floorMaterial = new engine::Material(engine::PhysicsEngine::Get());
        floorMaterial->SetMaterial(0.5f, 0.5f, 0.f);

        /// ---------------- Create rigidbody ----------------
        engine::RigidBodyDynamic* rigidBody = new engine::RigidBodyDynamic(engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine.GetGraph());
        rigidBody->CreateDynamicRigidBody(engine::PhysicsEngine::Get(), *material, engine::CAPSULE);

        engine::RigidBodyStatic* floorRigidBody = new engine::RigidBodyStatic(floor, engine.GetGraph());
        floorRigidBody->CreateStaticRigidBody(engine::PhysicsEngine::Get(), *floorMaterial, engine::PLANE);
		
		while (!engine.GetWindow()->ShouldWindowClose())
		{
			project.Update(engine);
			engine.Update();

            engine::PhysicsEngine::Get().StepSimulation(1.f / 60.f);
            rigidBody->UpdateEntity(engine.GetGraph()->GetEntity("Padoru")->GetHandle());
            rigidBody->UpdateRigidBody(*engine.GetGraph()->GetComponent<engine::Transform>(engine.GetGraph()->GetEntity("Padoru")->GetHandle()));
        }

        /// ---------------- Clean ---------------- 
        delete rigidBody;
        delete floorRigidBody;
        engine::PhysicsEngine::Get().CleanUp();
        delete material;
        delete floorMaterial;

		engine.ShutDown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks found\n");

	return 0;
}