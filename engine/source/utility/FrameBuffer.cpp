#include "utility/FrameBuffer.h"

#include <glad/glad.h>

engine::FrameBuffer::FrameBuffer(void)
    : m_fbo(0), m_rbo(0), m_colorTex(0), m_size{0, 0}
{
}

engine::FrameBuffer::~FrameBuffer(void)
{
    DeleteFrameBuffer();
}

void engine::FrameBuffer::Init(int32 width, int32 height)
{
    m_size = {width, height};

    // Init FBO
    glCreateFramebuffers(1, &m_fbo);

    // Init RBO 
    glCreateRenderbuffers(1, &m_rbo);
    glNamedRenderbufferStorage(m_rbo, GL_DEPTH24_STENCIL8, width, height);

    // Init color texture
    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTex);
    glTextureParameteri(m_colorTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_colorTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_colorTex, 1, GL_RGB8, width, height);
    
    // Attach RBO & texture to FBO
    glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_colorTex, 0);

    // Framebuffer error handling
#ifdef _DEBUG
    GLenum status = glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        printf("[FrameBuffer ERROR]: %d\n", status);
#endif
}

void engine::FrameBuffer::RescaleFBO(int32 width, int32 height)
{
    if (width == m_size.GetX() || height == m_size.GetY())
        return;

    m_size = {width, height};

    /*
        Destroy & init new frame buffer
        We do this because of DSA making use of the 'glTextureStorage2D'
        this function is immutable and therefore removes the possibility to
        modify the framebuffer's texture. Hence why a new framebuffer must
        be created
    */

    DeleteFrameBuffer();
    Init(width, height);
}

void engine::FrameBuffer::Bind(void) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void engine::FrameBuffer::Unbind(void) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint64 engine::FrameBuffer::GetFrameTexture(void) const noexcept
{
    return m_colorTex;
}

void engine::FrameBuffer::DeleteFrameBuffer(void)
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_colorTex);
    glDeleteRenderbuffers(1, &m_rbo);

    m_fbo, m_rbo, m_colorTex = 0;
}
