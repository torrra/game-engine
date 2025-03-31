#include "physics/InternalPhysxDebugDraw.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region GLAD

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma endregion

#pragma region Resource

#include "engine/resource/shader/Shader.h"

#pragma endregion

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

#include <iostream>



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
    glCreateBuffers(1, &m_debugDrawVBO);
    glNamedBufferData(m_debugDrawVBO, 0, nullptr, GL_STATIC_DRAW);


    glCreateVertexArrays(1, &m_debugDrawVAO);
    glEnableVertexArrayAttrib(m_debugDrawVAO, 0);
    glVertexArrayAttribFormat(m_debugDrawVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_debugDrawVAO, 0, 0);

    glVertexArrayVertexBuffer(m_debugDrawVAO, 0, m_debugDrawVBO, 0, sizeof(math::Vector3f));

    m_debugShadeProgram = new ShaderProgram("..\\engineShader\\DebugShader.vs",
        "..\\engineShader\\DebugShader.frag");

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
    m_debugShadeProgram->Use();
    m_debugShadeProgram->Set("projViewMatrix", inProjViewMatrix);
    glBindVertexArray(m_debugDrawVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
    glDrawArrays(GL_LINES, 0, inLineCount);
    glBindVertexArray(0);
    //auto debugDraw = glGetError();
    //if (debugDraw)
    //{
    //    glEnable(GL_DEBUG_OUTPUT);
    //    glDebugMessageCallback(message_callback, nullptr);
    //    __debugbreak();
    //}
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    user_param;
    length;
    auto const src_str = [source]() {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        default: return "NONE";
        }
        }();

    auto const type_str = [type]() {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        default: return "NONE";
        }
        }();

    auto const severity_str = [severity]() {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        default: return "NONE";
        }
        }();
    printf("%s, %s, %s, %d, %s\n", src_str, type_str, severity_str, (int)id, message);
}