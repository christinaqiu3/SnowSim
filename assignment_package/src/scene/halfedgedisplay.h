#ifndef HALFEDGEDISPLAY_H
#define HALFEDGEDISPLAY_H


#pragma once
#include "drawable.h"
#include "halfedge.h"

class halfedgedisplay : public Drawable {
protected:
    halfedge* representedHalfEdge;

public:
    halfedgedisplay(OpenGLContext* context);
    void create() override;
    void updateHalfEdge(halfedge* halfEdge);
    halfedge* getHalfEdge();

    GLenum drawMode() override;
};

#endif // HALFEDGEDISPLAY_H
