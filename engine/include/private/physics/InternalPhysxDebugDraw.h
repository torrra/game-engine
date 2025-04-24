#pragma once

#pragma region Engine

#include "CoreTypes.h"

#pragma endregion

#pragma region LibMath

#include "math/Matrix4.hpp"

#pragma endregion

#include "physics/Raycast.h"

namespace engine
{
    struct DebugDrawImpl;
    class ShaderProgram;

    class DebugDraw
    {
        friend class PhysicsEngine;
        friend class Raycast;

    private :

        /// Constructor
        // Initialize the DebugDrawImpl struct pointer
                        DebugDraw(void);

        /// Destructor
        // Delete the DebugDrawImpl struct pointer
                        ~DebugDraw(void);

        /// Getters
        // Get the DebugDrawImpl struct pointer
        DebugDrawImpl*  GetDebugDrawImpl(void) const;

        /// Functions
        // Initialize the vbo/vao and program for debug draw
        void            InitDebugDraw(void);
        // Update the debug draw in regard to the render buffer
        void            UpdateDebugDraw(const DebugDrawImpl& inDebugDrawImpl);
        // Render the debug draw
        void            RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount, const math::Vector4f& inColor);

        void AddDebugLine(const math::Vector3f& inStart, const math::Vector3f& inEnd, const uint32& inColor);


        const   ShaderProgram* m_debugDrawProgram   = nullptr;
                DebugDrawImpl* m_debugDrawImpl      = nullptr;
                uint32         m_debugDrawVAO       = 0;
                uint32         m_debugDrawVBO       = 0;

    }; // !Class DebugDraw
} // !Namespace engine