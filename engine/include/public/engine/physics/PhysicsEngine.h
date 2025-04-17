#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

#pragma region LibMath

#include "math/Matrix4.hpp"

#pragma endregion

namespace engine
{
    /// Forward declaration
    struct PhysicsEngineImpl;
    class DebugDraw;

    class PhysicsEngine
    {
    public :

        /// Getter
        // Get the instance of PhysicsEngine
        ENGINE_API static	PhysicsEngine& Get(void);
        // Get the pointer to the structure implementation of physx
        ENGINE_API          PhysicsEngineImpl& GetImpl(void) const;

        /// Setter
        /*
            Set the debug draw of the linear velocity active with their own scale to 0 by default
            <param> [in] inScale : The scale : 0 by default : not visible
                                             : 1            : visible with a normal scale
                                             : > 1          : visible with a bigger scale
        */
        ENGINE_API  void    SetDebugLinearVelocity(f32 inScale = 0.f);
        /*
            Set the debug draw of the contact point active with their own scale to 0 by default
            <param> [in] inScale : The scale : 0 by default : not visible
                                             : 1            : visible with a normal scale
                                             : > 1          : visible with a bigger scale
        */
        ENGINE_API  void    SetDebugContactPoint(f32 inScale = 0.f);
        /*
            Set the debug draw of the actor axes active with their own scale to 0 by default
            <param> [in] inScale : The scale : 0 by default : not visible
                                             : 1            : visible with a normal scale
                                             : > 1          : visible with a bigger scale
        */
        ENGINE_API  void    SetDebugActorAxes(f32 inScale = 0.f);

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
        /*
            Update the physics simulation into the physx scene
            <param> [in] inDeltaTime : The delta time since the last frame
        */
        ENGINE_API void		StepSimulation(f32 inDeltaTime);
        /*
            Update the debug draw from the physx scene into the openGL scene
            <param> [in] inProjViewMatrix : The projection view matrix
        */
        ENGINE_API void     UpdateDebugDraw(math::Matrix4f* inProjViewMatrix);
        // Release all physx resources
        ENGINE_API void		CleanUp(void);
        ENGINE_API void AddDebugLine(const math::Vector3f& inStart, const math::Vector3f& inEnd, uint32 inColor);

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
        /*
            Initialize physics in regard to the pvd connection
            <param> [in] inIsPvdConnected : Is the pvd connected
        */
        void 				InitPhysics(bool inIsPvdConnected);
        // Initialize the scene/material/dispatcher
        void				InitScene(void);
        math::Vector4f ConvertPhysxColorToVector4f(uint32 inColor);

        /// Members
        // Pointer to the implementation of the physx structure
                PhysicsEngineImpl*	m_impl      = nullptr;
        // Instance of the physics engine
        static  PhysicsEngine*	    m_instance;
        // Debug draw
                DebugDraw*			m_debugDraw = nullptr;

    }; // !Class PhysicsEngine
} // !Namespace engine