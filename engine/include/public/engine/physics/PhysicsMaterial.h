#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

namespace engine
{
    struct MaterialImpl;

    class Material
    {
    public :

        /// Constructor
        /*
            Create default material constructor with value set to default
            static friction = 0.5f
            dynamic friction = 0.5f
            restitution = 0.6f
        */
        ENGINE_API					Material(void);
        /*
             Initialize the struct MaterialImpl pointer and create a material with the 
             given parameters set by default to 0
             <param> [in] inPhysicsEngine : The physics engine to use
             <param> [in] inDynamicFriction : The dynamic friction
             <param> [in] inStaticFriction : The static friction
             <param> [in] inRestitution : The restitution
        */
        ENGINE_API					Material(f32 inStaticFriction, 
                                             f32 inDynamicFriction = 0.5f,
                                             f32 inRestitution = 0.6f);

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
        /*
            Set the dynamic friction
            <param> [in] inDynamicFriction : The dynamic friction
        */
        ENGINE_API void				SetDynamicFriction(f32 inDynamicFriction);
        /*
            Set the static friction
            <param> [in] inStaticFriction : The static friction
        */
        ENGINE_API void				SetStaticFriction(f32 inStaticFriction);
        /*
            Set the restitution
            <param> [in] inRestitution : The restitution
        */
        ENGINE_API void				SetRestitution(f32 inRestitution);
        /*
            Set all the parameters
            <param> [in] inDynamicFriction : The dynamic friction
            <param> [in] inStaticFriction : The static friction
            <param> [in] inRestitution : The restitution
        */
        ENGINE_API void				SetMaterial(f32 inDynamicFriction = 0.f,
                                                f32 inStaticFriction = 0.f,
                                                f32 inRestitution = 0.f);


    private :

        MaterialImpl*	m_materialImpl		= nullptr;

    }; // !Class Material
} // !Namespace engine