#include "halfedge.h"

halfedge::halfedge()
    : nextHalfEdge(nullptr), symHalfEdge(nullptr), incidentFace(nullptr), nextVertex(nullptr), ind(0)
{}

//next HE, sym HE, incident Face, next Vertex, ind
halfedge::halfedge(halfedge* nHalfEdge, halfedge* sHalfEdge, face* iFace, vertex* nVertex, int id)
    : nextHalfEdge(nHalfEdge), symHalfEdge(sHalfEdge), incidentFace(iFace), nextVertex(nVertex), ind(id)
{}

vertex* halfedge::getNextVertex() {
    return nextVertex;
}

halfedge* halfedge::getNextHalfEdge() {
    return nextHalfEdge;
}

face* halfedge::getIncidentFace() {
    return incidentFace;
}

void halfedge::setNextHalfEdge(halfedge* n) {
    nextHalfEdge = n;
}

void halfedge::setSymHalfEdge(halfedge* s) {
    symHalfEdge = s;
}

halfedge* halfedge::getSymHalfEdge() {
    return symHalfEdge;
}

void halfedge::setIncidentFace(face* f) {
    incidentFace = f;
}

void halfedge::setNextVertex(vertex* v) {
    nextVertex = v;
}
