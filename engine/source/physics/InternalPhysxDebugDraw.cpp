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

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

engine::DebugDraw::DebugDraw()
{
    m_debugDrawImpl = new engine::DebugDrawImpl();
}

engine::DebugDraw::~DebugDraw(void)
{
    glBindBuffer(0, 0);
    glBindVertexArray(0);

    delete m_debugDrawImpl;
    m_debugDrawImpl = nullptr;
}

engine::DebugDrawImpl* engine::DebugDraw::GetDebugDrawImpl(void) const
{
    return m_debugDrawImpl;
}

void engine::DebugDraw::InitDebugDraw(void)
{
    glCreateBuffers(1, &m_debugDrawVBO);
    glNamedBufferData(m_debugDrawVBO, 0, nullptr, GL_STATIC_DRAW);


    glCreateVertexArrays(1, &m_debugDrawVAO);
    glEnableVertexArrayAttrib(m_debugDrawVAO, 0);
    glVertexArrayAttribFormat(m_debugDrawVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_debugDrawVAO, 0, 0);

    glVertexArrayVertexBuffer(m_debugDrawVAO, 0, m_debugDrawVBO, 0, sizeof(math::Vector3f));

    ResourceManager::LoadShader("DebugDrawShader", "..\\engineShader\\DebugShader.vs", "..\\engineShader\\DebugShader.frag");

}

void engine::DebugDraw::UpdateDebugDraw(const DebugDrawImpl& inDebugDrawImpl)
{
    std::vector<math::Vector3f> lineVertices;

    static bool firstLine = true;

    for (physx::PxU32 lineNum = 0; lineNum < inDebugDrawImpl.m_renderBuffer->getNbLines();
        ++lineNum)
    {
        const physx::PxDebugLine& line = inDebugDrawImpl.m_renderBuffer->getLines()[lineNum];
        lineVertices.push_back(ToVector3f(line.pos0));
        lineVertices.push_back(ToVector3f(line.pos1));
    }

    glBindVertexArray(m_debugDrawVAO);
    glNamedBufferData(m_debugDrawVBO, lineVertices.size() * sizeof(math::Vector3f), lineVertices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void engine::DebugDraw::RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount)
{
    ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader")->Use();
    ResourceManager::GetResource<engine::ShaderProgram>("DebugDrawShader")->Set("projViewMatrix", inProjViewMatrix);
    glBindVertexArray(m_debugDrawVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
    glDrawArrays(GL_LINES, 0, inLineCount);
    glBindVertexArray(0);

    OpenGLError();
}