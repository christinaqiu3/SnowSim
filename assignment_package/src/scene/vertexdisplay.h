#ifndef VERTEXDISPLAY_H
#define VERTEXDISPLAY_H

#include "drawable.h"
#include "vertex.h"


class vertexdisplay : public Drawable {
protected:
    vertex *representedVertex;

public:
    vertexdisplay(OpenGLContext* context);

    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;
    // Change which Vertex representedVertex points to
    void updateVertex(vertex*);
    vertex* getVertex();

    GLenum drawMode() override;
};


#endif // VERTEXDISPLAY_H
