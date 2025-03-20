#include <iostream>

#include <engine/utility/MemoryCheck.h>

#include "engine/physics/geometry/GeometryTest.h"
#include "engine/physics/PhysicsEngine.h"

#include <math/VectorGeneric.hpp>
#include <math/Vector3.hpp>

extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	std::printf("Starting editor\n");

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		/// Geometry Test
		/// TODO : Delete while rigid body are implemented
		//engine::Physics physics;

		//engine::RigidBody box(physics.GetPhysics(), physics.GetScene(), physics.GetMaterial(), math::Vector3f(-10.f, 2.f, 0.f));
	
		//box.SetVelocity(math::Vector3f(10.f, 0.f, 0.f));

		//for (int i = 0; i < 300; ++i)
		//{
		//	physics.StepSimulation(1.f / 60.f);
		//}

		/// PhysicsEngine use
		engine::PhysicsEngine* physics;
		physics->Get().Init();

		for (int i = 0; i < 300; ++i)
		{
			physics->Get().StepSimulation(1.f / 60.f);
		}

		physics->Get().CleanUp();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}
