#include <engine/utility/MemoryCheck.h>

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/rigidbody/RigidBodyDynamic.h"

#include <math/VectorGeneric.hpp>
#include <math/Vector3.hpp>

#include "engine/core/SceneGraph.h"
#include "engine/core/Entity.h"

#include "engine/core/systems/ScriptSystem.h"

extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	std::printf("Starting editor\n");

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		engine::ScriptSystem::Startup();

		engine::SceneGraph* scene = new engine::SceneGraph();

		engine::EntityHandle entityHandle = scene->CreateEntity("First");

		/// PhysicsEngine use
		engine::PhysicsEngine* physics;
		physics->Get().Init();

		engine::RigidBodyDynamic* rigidBody = new engine::RigidBodyDynamic(entityHandle, scene);

		for (int i = 0; i < 300; ++i)
		{
			physics->Get().StepSimulation(1.f / 60.f);
		}

		delete rigidBody;
		physics->Get().CleanUp();

		delete scene;

		engine::ScriptSystem::Shutdown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

		return 0;
}