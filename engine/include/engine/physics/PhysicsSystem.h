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

		/// Getter
		class PxPhysics*	getPhysics(void) const;

	private :

		/// Private members
		// Forward declaration to do encapsulation
		// PxFoundation, PxPvd, PxPhysics
		class physx::PxFoundation*	m_Foundation	= nullptr;
		class physx::PxPhysics*		m_Physics		= nullptr;
		class physx::PxPvd*			m_Pvd			= nullptr;

	}; // !Class PhysicsSystem
} // !Namespace engine