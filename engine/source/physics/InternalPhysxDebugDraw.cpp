#include "physics/InternalPhysxDebugDraw.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"
#include "InternalOpenGLError.hpp"

#pragma endregion

#pragma region Resource

#include "engine/resource/shader/Shader.h"
#include "engine/resource/ResourceManager.h"

#pragma endregion

engine::DebugDraw::DebugDraw()
{
    // Initialize the debud draw implementation structure pointer
    m_debugDrawImpl = new engine::DebugDrawImpl();
}

engine::DebugDraw::~DebugDraw(void)
{
    // Unbind VBO and VAO
    glDeleteBuffers(1, &m_debugDrawVBO);
    glBindVertexArray(0);

    // Delete the debug draw implementation structure pointer
    delete m_debugDrawImpl;
    m_debugDrawImpl = nullptr;
}

engine::DebugDrawImpl* engine::DebugDraw::GetDebugDrawImpl(void) const
{
    // Return the debug draw implementation structure pointer
    return m_debugDrawImpl;
}

void engine::DebugDraw::InitDebugDraw(void)
{
    // Create VBO
    glCreateBuffers(1, &m_debugDrawVBO);
    glNamedBufferData(m_debugDrawVBO, 0, nullptr, GL_STATIC_DRAW);

    // Create VAO
    glCreateVertexArrays(1, &m_debugDrawVAO);
    glEnableVertexArrayAttrib(m_debugDrawVAO, 0);
    glVertexArrayAttribFormat(m_debugDrawVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_debugDrawVAO, 0, 0);

    // Bind VBO and VAO
    constexpr int32 lineVertexSize = sizeof(physx::PxDebugLine) / 2;
    glVertexArrayVertexBuffer(m_debugDrawVAO, 0, m_debugDrawVBO, 0, lineVertexSize);


    // Load the debug draw shader
    ResourceManager::LoadShader("DebugDrawShader", ".\\shaders\\DebugShader.vs", ".\\shaders\\DebugShader.frag");

    // Store the debug draw program
    m_debugDrawProgram = ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader");
}

void engine::DebugDraw::UpdateDebugDraw(const DebugDrawImpl& inDebugDrawImpl)
{
    uint64 bufferSize = inDebugDrawImpl.m_renderBuffer->getNbLines() * sizeof(physx::PxDebugLine);

    glNamedBufferData(m_debugDrawVBO, bufferSize, inDebugDrawImpl.m_renderBuffer->getLines(), GL_STATIC_DRAW);

    // Check for openGL error
    OpenGLError();
}


void engine::DebugDraw::RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount)
{
    // Use the debug draw program
    m_debugDrawProgram->Use();
    // Set the projection view matrix uniform
    ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader")->Set("projViewMatrix", inProjViewMatrix);
    // Bind the VAO
    glBindVertexArray(m_debugDrawVAO);
    // Draw the lines in regard of the number of lines
    glDrawArrays(GL_LINES, 0, inLineCount);
    // Unbind the VAO
    glBindVertexArray(0);

    // Check for openGL error
    OpenGLError();
}