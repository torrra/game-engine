#include <engine/utility/MemoryCheck.h>

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/rigidbody/RigidBodyDynamic.h"
#include "engine/physics/Material.h"

#include <math/VectorGeneric.hpp>
#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#include "engine/core/SceneGraph.h"
#include "engine/core/Entity.h"
#include "engine/core/components/Transform.h"

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

		/// ---------------- Create scene ----------------
		engine::SceneGraph* scene = new engine::SceneGraph();

		/// ---------------- Create entity ---------------- 
		engine::EntityHandle entityHandle = scene->CreateEntity("First");
		scene->CreateComponent<engine::Transform>(entityHandle)->SetPosition(math::Vector3f(0.f, 5.f, 0.f));

		/// ---------------- PhysicsEngine use ---------------- 
		engine::PhysicsEngine* physics;
		physics->Get().Init();

		/// ---------------- Create material ----------------
		engine::Material* material = new engine::Material(physics->Get());
		material->SetRestitution(1.f);

		/// ---------------- Create rigidbody ----------------
		engine::RigidBodyDynamic* rigidBody = new engine::RigidBodyDynamic(entityHandle, scene);
		engine::Transform* transform = scene->GetComponent<engine::Transform>(entityHandle);
		if (transform != nullptr)
		{
			rigidBody->CreateDynamicRigidBody(physics->Get(), *transform, *material);
			std::cout << "RigidBodyDynamic created, with existing transform" << std::endl;
		}
		else
		{
			engine::Transform* transform2 = scene->CreateComponent<engine::Transform>(entityHandle);
			rigidBody->CreateDynamicRigidBody(physics->Get(), *transform2, *material);
			std::cout << "RigidBodyDynamic created, with created transform" << std::endl;
		}

		/// ---------------- Simulation loop ---------------- 
		for (int i = 0; i < 300; ++i)
		{
			physics->Get().StepSimulation(1.f / 60.f);
			//rigidBody->UpdateEntity();
			scene->GetComponent<engine::Transform>(entityHandle)->AddRotation(0.f, 0.f, 10.f);
			rigidBody->UpdateRigidBody(*scene->GetComponent<engine::Transform>(entityHandle));

		}

		/// ---------------- Clean ---------------- 
		delete rigidBody;
		physics->Get().CleanUp();
		delete material;
		delete scene;

		engine::ScriptSystem::Shutdown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

		return 0;
}