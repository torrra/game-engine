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
							PhysicsSystem(const PhysicsSystem& other) = delete;

		/// Destructor
		// Destroy all the physics system resources and the physics handler
							~PhysicsSystem(void);

	private :

		/// Private members
		// Forward declaration to do encapsulation
		struct PhysicsSystemHandle* m_PhysicHandle;

	}; // !Class PhysicsSystem
} // !Namespace engine