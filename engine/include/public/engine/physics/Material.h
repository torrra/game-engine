#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

namespace engine
{
	struct MaterialImpl;

	class Material
	{
	public :

		/// Constructor
		// Default constructor deleted
		ENGINE_API					Material(void) = delete;
		// Initialize the struct MaterialImpl pointer and create a material with the 
		// given parameters set by default to 0
		ENGINE_API					Material(const PhysicsEngine& inPhysicsEngine, 
											 f32 inDynamicFriction = 0.f, 
											 f32 inStaticFriction = 0.f, 
											 f32 inRestitution = 0.f);

		/// Destructor
		// Delete the struct MaterialImpl pointer
		ENGINE_API					~Material(void);

		/// Getter
		// Return the struct MaterialImpl
		ENGINE_API MaterialImpl&	GetImpl(void) const;

		// Return the dynamic friction
		ENGINE_API f32				GetDynamicFriction(void) const;
		// Return the static friction
		ENGINE_API f32				GetStaticFriction(void) const;
		// Return the restitution
		ENGINE_API f32				GetRestitution(void) const;

		/// Setter
		// Set the dynamic friction
		ENGINE_API void				SetDynamicFriction(f32 inDynamicFriction);
		// Set the static friction
		ENGINE_API void				SetStaticFriction(f32 inStaticFriction);
		// Set the restitution
		ENGINE_API void				SetRestitution(f32 inRestitution);
		// Set all the parameters
		ENGINE_API void				SetMaterial(f32 inDynamicFriction = 0.f,
												f32 inStaticFriction = 0.f,
												f32 inRestitution = 0.f);


	private :

		MaterialImpl*	m_materialImpl		= nullptr;

	}; // !Class Material
} // !Namespace engine