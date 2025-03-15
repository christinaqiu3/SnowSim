#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include "glm/detail/type_vec.hpp"
#include "openglcontext.h"
class FrameBuffer
{
private:
    OpenGLContext *mp_context;
    GLuint m_frameBuffer;
    GLuint m_outputTexture;
    GLuint m_depthRenderBuffer;

    unsigned int m_width, m_height;
    float m_devicePixelRatio;
    bool m_created;

    unsigned int m_textureSlot;

public:
    FrameBuffer(OpenGLContext *context, unsigned int width, unsigned int height, float devicePixel);
    void resize(unsigned int width, unsigned int height, float devicePixelRatio);
    void create();
    void createShadowMap();
    void destroy();
    void bindFrameBuffer();
    void bindToTextureSlot(unsigned int slot);
    unsigned int getTextureSlot() const;
    glm::ivec2 getDimensions() const;
};

#endif // FRAMEBUFFER_H
