#ifndef VERTEX_H
#define VERTEX_H
#include "scene/joint.h"
#pragma once
#include <glm/glm.hpp>
#include "halfedge.h"
class mesh;
class halfedge;

class vertex : public QListWidgetItem
{
private:
    glm::vec3 position;
    halfedge* incidentHalfEdge;
    friend class mesh;



public:
    int ind;
    vertex();
    vertex(glm::vec3 pos, halfedge* iHalfEdge, int id);
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setIncidentHalfEdge(halfedge* i);
    halfedge* getIncidentHalfEdge();
    std::array<std::pair<Joint*, float>, 2> jointInfluences;
    float jointWeight1;
    float jointWeight2;

};

#endif // VERTEX_H
