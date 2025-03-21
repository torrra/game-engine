#include <engine/utility/MemoryCheck.h>

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/rigidbody/RigidBodyDynamic.h"
#include "engine/physics/rigidbody/RigidBodyStatic.h"

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
		engine::EntityHandle object = scene->CreateEntity("First");
		scene->CreateComponent<engine::Transform>(object)->SetTransform(
								math::Vector3f(0.f, 5.f, 0.f), 
								math::Quatf(1.f, 0.f, 0.f, 0.f));
		engine::EntityHandle floor = scene->CreateEntity("Floor");

		/// ---------------- PhysicsEngine use ---------------- 
		engine::PhysicsEngine* physics;
		physics->Get().Init();

		/// ---------------- Create material ----------------
		engine::Material* material = new engine::Material(physics->Get());
		material->SetRestitution(1.f);

		engine::Material* floorMaterial = new engine::Material(physics->Get());

		/// ---------------- Create rigidbody ----------------
		engine::RigidBodyDynamic* rigidBody = new engine::RigidBodyDynamic(object, scene);
		rigidBody->CreateDynamicRigidBody(physics->Get(), *material, engine::CAPSULE);

		engine::RigidBodyStatic* floorRigidBody = new engine::RigidBodyStatic(floor, scene);
		floorRigidBody->CreateStaticRigidBody(physics->Get(), *floorMaterial, engine::PLANE);

		/// ---------------- Simulation loop ---------------- 
		for (int i = 0; i < 300; ++i)
		{
			physics->Get().StepSimulation(1.f / 60.f);
			rigidBody->UpdateEntity();
			scene->GetComponent<engine::Transform>(object)->AddRotation(0.f, 0.f, 10.f);
			rigidBody->UpdateRigidBody(*scene->GetComponent<engine::Transform>(object));

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