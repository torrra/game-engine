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
		// Initialize the physics engine
		ENGINE_API void		Init(void);

	private :

		/// Constructor
		// Empty private constructor
							PhysicsEngine(void);
		// Copy constructor deleted to make a singleton
							PhysicsEngine(const PhysicsEngine&) = delete;

		/// Destructor
							~PhysicsEngine(void);

		/// Members
		struct PhysicsEngineImpl*	m_impl;

		static PhysicsEngine*		m_instance;

	}; // !Class PhysicsEngine
} // !Namespace engine