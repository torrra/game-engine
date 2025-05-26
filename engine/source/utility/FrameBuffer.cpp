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
    // Abort rescale if no changes in width and height
    if (width == m_size.GetX() && height == m_size.GetY())
        return;

    m_size = {width, height};
    /*
        Rescaling Framebuffer via DSA
        As the engine uses OpenGL 4.5 and hence DSA we have to use the function
        'glTextureStorage2D', contrary to previous versions of OpenGL this function
        is immutable and therefore removes the possibility to modify the framebuffer's
        texture. Consequently, in order to resize the framebuffer the current framebuffer most
        be deleted and replaced by a new framebuffer with the correct size.
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

uint32 engine::FrameBuffer::GetFBO(void) const
{
    return m_fbo;
}

uint64 engine::FrameBuffer::GetFrameTexture(void) const noexcept
{
    return m_colorTex;
}

void engine::FrameBuffer::DeleteFrameBuffer(void)
{
    glDeleteTextures(1, &m_colorTex);
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteFramebuffers(1, &m_fbo);

    m_fbo, m_rbo, m_colorTex = 0;
}
