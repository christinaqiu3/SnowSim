#ifndef MESH_H
#define MESH_H

#pragma once
#include "drawable.h"
#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
class vertex;
class face;
class halfedge;

class mesh : public Drawable
{


public:

    std::vector<vertex*> vertices;
    std::vector<halfedge*> halfedges;
    std::vector<face*> faces;

    std::string LoadObj(const char *filename);
    mesh();
    mesh(OpenGLContext* t);
    virtual void create();
    void helperTriangulate(std::vector<GLuint> &indices, int count, int totalV);

    std::vector<vertex*> getVertss();
    std::vector<halfedge*> getHalfEdgess();
    std::vector<face*> getFacess();

};

#endif // MESH_H
