#pragma once

namespace engine
{
	class PhysicsSystem
	{
	public :

		/// Constructor
		// Empty constructor to initialize the physics system
		// PxFoundation, PxPvd, PxPhysics
		PhysicsSystem(void);

		/// Destructor
		// Destroy all the physics system resources
		~PhysicsSystem(void);

	private :

		/// Private members
		// Forward declaration to do encapsulation
		// PxFoundation, PxPvd, PxPhysics
		struct Impl;
		Impl* m_Impl = nullptr;

	}; // !Class PhysicsSystem
} // !Namespace engine