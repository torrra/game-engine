#pragma once

#pragma region Engine

#include "CoreTypes.h"

#pragma endregion

#pragma region LibMath

#include "math/Matrix4.hpp"

#pragma endregion

#pragma region GLAD

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma endregion

namespace engine
{
    struct DebugDrawImpl;
    class ShaderProgram;

    class DebugDraw
    {
    public :

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

    private :

        DebugDrawImpl* m_debugDrawImpl      = nullptr;
        ShaderProgram* m_debugShadeProgram  = nullptr;
        uint32         m_debugDrawVAO       = 0;
        uint32         m_debugDrawVBO       = 0;

    }; // !Class DebugDraw
} // !Namespace engine

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);