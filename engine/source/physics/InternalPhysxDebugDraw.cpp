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
    glVertexArrayVertexBuffer(m_debugDrawVAO, 0, m_debugDrawVBO, 0, sizeof(math::Vector3f));

    // Load the debug draw shader
    ResourceManager::LoadShader("DebugDrawShader", ".\\shaders\\DebugShader.vs", ".\\shaders\\DebugShader.frag");

    // Store the debug draw program
    m_debugDrawProgram = ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader");
}

void engine::DebugDraw::UpdateDebugDraw(const DebugDrawImpl& inDebugDrawImpl)
{
    std::vector<math::Vector3f> lineVertices;

    // For each line in the render buffer
    for (physx::PxU32 lineNum = 0; lineNum < inDebugDrawImpl.m_renderBuffer->getNbLines();
         ++lineNum)
    {
        // Retrieve the start and end points of the line and store them in the vector
        const physx::PxDebugLine& line = inDebugDrawImpl.m_renderBuffer->getLines()[lineNum];
        lineVertices.push_back(ToVector3f(line.pos0));
        lineVertices.push_back(ToVector3f(line.pos1));
    }

    // Update the VBO with the data to draw
    glBindVertexArray(m_debugDrawVAO);
    glNamedBufferData(m_debugDrawVBO, lineVertices.size() * sizeof(math::Vector3f), lineVertices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void engine::DebugDraw::RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount)
{
    // Use the debug draw program
    m_debugDrawProgram->Use();
    // Set the projection view matrix uniform
    ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader")->Set("projViewMatrix", inProjViewMatrix);
    // Bind the VAO
    glBindVertexArray(m_debugDrawVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
    // Draw the lines in regard of the number of lines
    glDrawArrays(GL_LINES, 0, inLineCount);
    // Unbind the VAO
    glBindVertexArray(0);

    // Check for openGL error
    OpenGLError();
}