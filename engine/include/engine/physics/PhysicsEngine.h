#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

namespace engine
{
	class PhysicsEngine
	{
	public :

		/// Getter
		// Get the instance of PhysicsEngine
		ENGINE_API static	PhysicsEngine& Get(void);

		/// Functions
		/*
			Initialize the physics engine by intializing
				- Foundation
				- Pvd
				- Transport
				- Physics
				- Scene
				- Material
		*/
		ENGINE_API void		Init(void);
		// Update the physics engine
		ENGINE_API void		StepSimulation(void);
		// Release all physx resources
		ENGINE_API void		CleanUp(void);

	private :

		/// Constructor
		// Empty private constructor to initialize the PhysicsEngineImpl structure
							PhysicsEngine(void);
		// Copy constructor deleted to make a singleton
							PhysicsEngine(const PhysicsEngine&) = delete;

		/// Destructor
		// Delete the PhyscsEngineImpl pointer
							~PhysicsEngine(void);

		/// Members
		// Pointer to the implementation of the physx structure
		struct PhysicsEngineImpl*	m_impl;
		// Instance of the physics engine
		static PhysicsEngine*		m_instance;

	}; // !Class PhysicsEngine
} // !Namespace engine