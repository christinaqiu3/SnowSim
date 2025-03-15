#include "vertex.h"

vertex::vertex()
    : position(glm::vec3(0.f, 0.f, 0.f)), incidentHalfEdge(nullptr), ind(0)
{}

vertex::vertex(glm::vec3 pos, halfedge* iHalfEdge, int id) :
    position(pos), incidentHalfEdge(iHalfEdge), ind(id)
{}

void vertex::setPosition(glm::vec3 pos) {
    position = pos;
}

glm::vec3 vertex::getPosition() {
    return position;
}

void vertex::setIncidentHalfEdge(halfedge* i) {
    incidentHalfEdge = i;
}

halfedge* vertex::getIncidentHalfEdge() {
    return incidentHalfEdge;
}

