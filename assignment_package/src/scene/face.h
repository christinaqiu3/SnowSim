#ifndef FACE_H
#define FACE_H
#pragma once
#include <glm/glm.hpp>
#include "halfedge.h"
class mesh;
class halfedge;
class vertex;

class face : public QListWidgetItem
{
private:
    halfedge* incidentHalfEdge;
    glm::vec3 color;
    friend class mesh;

public:
    int ind;
    face();
    face(halfedge* iHalfEdge, glm::vec3 col, int id);
    halfedge* getIncidentHalfEdge();
    void setIncidentHalfEdge(halfedge* i);
    void setInd(int i);
    void setColor(glm::vec3 col);
    glm::vec3 getColor();

    std::vector<vertex*> getVertices();
    int getVertexCount();
};

#endif // FACE_H
