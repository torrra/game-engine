#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

namespace engine
{
	struct PhysicsEngineImpl;

	class PhysicsEngine
	{
	public :

		/// Getter
		// Get the instance of PhysicsEngine
		ENGINE_API static	PhysicsEngine& Get(void);

		ENGINE_API PhysicsEngineImpl& GetImpl(void) const;

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
		// 
		ENGINE_API void		StepSimulation(f32 inDeltaTime);
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

		/// Functions
		// Initialize PVD/transport
		bool				InitPvd(void);
		// Initialize physics
		void 				InitPhysics(bool inIsPvdConnected);
		// Initialize the scene/material/dispatcher
		void				InitScene(void);


		/// Members
		// Pointer to the implementation of the physx structure
		PhysicsEngineImpl*		m_impl;
		// Instance of the physics engine
		static PhysicsEngine*	m_instance;

	}; // !Class PhysicsEngine
} // !Namespace engine