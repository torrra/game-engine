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
    ResourceManager::LoadShader("DebugDrawShader",
                                ".\\shaders\\DebugShader.vs",
                                ".\\shaders\\DebugShader.frag", true, true);

    // Store the debug draw program
    m_debugDrawProgram = ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader");
}

void engine::DebugDraw::UpdateDebugDraw(const DebugDrawImpl& inDebugDrawImpl)
{
    std::vector<physx::PxDebugLine> totalLines;

    const physx::PxDebugLine* physxLines = inDebugDrawImpl.m_renderBuffer->getLines();
    uint32 physxLineCount = inDebugDrawImpl.m_renderBuffer->getNbLines();

    totalLines.insert(totalLines.end(), physxLines, physxLines + physxLineCount);
    totalLines.insert(totalLines.end(), m_debugDrawImpl->m_customLines.begin(), 
                      m_debugDrawImpl->m_customLines.end());

    uint64 bufferSize = totalLines.size() * sizeof(physx::PxDebugLine);
    glNamedBufferData(m_debugDrawVBO, bufferSize, totalLines.data(), GL_DYNAMIC_DRAW);


    //// Check for openGL error
    OpenGLError();
}


void engine::DebugDraw::RenderDebugDraw(const math::Matrix4f* inProjViewMatrix, uint32 inLineCount, const math::Vector4f& inColor)
{
    // Use the debug draw program
    m_debugDrawProgram->Use();
    // Set the projection view matrix uniform
    m_debugDrawProgram->Set("projViewMatrix", inProjViewMatrix);
    m_debugDrawProgram->Set("aColor", inColor);

    // Bind the VAO
    glBindVertexArray(m_debugDrawVAO);
    // Draw the lines in regard of the number of lines
    glDrawArrays(GL_LINES, 0, inLineCount);
    // Unbind the VAO
    glBindVertexArray(0);

    // Check for openGL error
    OpenGLError();
}

void engine::DebugDraw::AddDebugLine(const math::Vector3f& inStart, const math::Vector3f& inEnd, const uint32& inColor)
{
    if (!m_debugDrawImpl)
    {
        return;
    }
    physx::PxDebugLine line(ToPxVec3(inStart), ToPxVec3(inEnd), inColor);
    m_debugDrawImpl->m_customLines.push_back(line);
}
