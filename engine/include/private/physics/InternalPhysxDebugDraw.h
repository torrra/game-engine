#pragma once

#pragma region Engine

#include "CoreTypes.h"

#pragma endregion

#pragma region LibMath

#include "math/Matrix4.hpp"

#pragma endregion

namespace engine
{
    struct DebugDrawImpl;
    class ShaderProgram;

    class DebugDraw
    {
        friend class PhysicsEngine;

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
        void            RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount);

        const   ShaderProgram* m_debugDrawProgram   = nullptr;
                DebugDrawImpl* m_debugDrawImpl      = nullptr;
                uint32         m_debugDrawVAO       = 0;
                uint32         m_debugDrawVBO       = 0;

    }; // !Class DebugDraw
} // !Namespace engine