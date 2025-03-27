#include "physics/InternalPhysxDebugDraw.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region GLAD

#include <glad/glad.h>

#pragma endregion

#pragma region Resource

#include "engine/resource/shader/Shader.h"

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
    delete m_debugDrawImpl;
    m_debugDrawImpl = nullptr;
}

engine::DebugDrawImpl* engine::DebugDraw::GetDebugDrawImpl(void) const
{
    return m_debugDrawImpl;
}

void engine::DebugDraw::InitDebugDraw(void)
{
    glGenVertexArrays(1, &m_debugDrawVAO);
    glGenBuffers(1, &m_debugDrawVBO);

    glBindVertexArray(m_debugDrawVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(math::Vector3f), (void*)0);

    glBindVertexArray(0);

    m_debugShadeProgram = new ShaderProgram("..\\engineShader\\DebugShader.vs",
        "..\\engineShader\\DebugShader.frag");
}

void engine::DebugDraw::UpdateDebugDraw(const DebugDrawImpl& inDebugDrawImpl)
{
    std::vector<math::Vector3f> lineVertices;

    for (physx::PxU32 lineNum = 0; lineNum < inDebugDrawImpl.m_renderBuffer->getNbLines(); 
        ++lineNum)
    {
        const physx::PxDebugLine& line = inDebugDrawImpl.m_renderBuffer->getLines()[lineNum];
        lineVertices.push_back(ToVector3fPos0(line));
        lineVertices.push_back(ToVector3fPos1(line));
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(math::Vector3f),
        lineVertices.data());
}

void engine::DebugDraw::RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount)
{
    m_debugShadeProgram->Use();
    m_debugShadeProgram->Set("projViewMatrix", inProjViewMatrix);
    glBindVertexArray(m_debugDrawVAO);
    glDrawArrays(GL_LINES, 0, inLineCount);
    glBindVertexArray(0);
}