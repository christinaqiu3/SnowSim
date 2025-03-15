#pragma once

#include "shaderprogram.h"
#include "framebuffer.h"

class PostProcessShader : public ShaderProgram
{
public:
    //GLint attrPos;
    GLint attrUV;
    GLint unifSampler2D;
    GLint unifDimensions;

    PostProcessShader(OpenGLContext *context);
    ~PostProcessShader();

    void setupMemberVars();
    void draw(Drawable& d, int textureSlot);
    void setDimensions(glm::ivec2 dims);
    void applyPostProcess(FrameBuffer& inputFrameBuffer, FrameBuffer& outputFrameBuffer);
};
