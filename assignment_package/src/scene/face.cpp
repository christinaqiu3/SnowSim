#include "face.h"
#include <glm/glm.hpp>

face::face()
    : incidentHalfEdge(nullptr), color(glm::vec3(0.f, 0.f, 0.f))
{}

face::face(halfedge* iHalfEdge, glm::vec3 col, int id)
    : incidentHalfEdge(iHalfEdge), color(col), ind(id)//lastIndF + 1)
{}

halfedge* face::getIncidentHalfEdge() {
    return incidentHalfEdge;
}

void face::setIncidentHalfEdge(halfedge* i) {
    incidentHalfEdge = i;
}

void face::setInd(int i) {
    ind = i;
}

glm::vec3 face::getColor(){
    return color;
}

void face::setColor(glm::vec3 col) {
    color = col;
}

std::vector<vertex*> face::getVertices() {
    std::vector<vertex*> vertices;
    if (!incidentHalfEdge) return vertices;

    halfedge* start = incidentHalfEdge;
    halfedge* he = start;

    do {
        vertices.push_back(he->getNextVertex()); // Get the vertex this half-edge points to
        he = he->getNextHalfEdge(); // Move to the next half-edge in the face
    } while (he != start);

    return vertices;
}

int face::getVertexCount() {
    if (!incidentHalfEdge) return 0;

    int count = 0;
    halfedge* start = incidentHalfEdge;
    halfedge* he = start;

    do {
        count++;
        he = he->getNextHalfEdge();
    } while (he != start);

    return count;
}



