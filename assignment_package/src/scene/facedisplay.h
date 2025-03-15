#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H


#include "drawable.h"
#include "openglcontext.h"
#include "scene/face.h"
class facedisplay : public Drawable
{
protected:
    face* representedFace;

public:
    facedisplay(OpenGLContext* context);
    void create() override;
    void updateFace(face* face);

    face* getFace();
    GLenum drawMode() override;
};

#endif // FACEDISPLAY_H
