#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "QtWidgets/qlistwidget.h"
#pragma once
#include "face.h"
#include "vertex.h"
class face;
class vertex;
class mesh;

class halfedge : public QListWidgetItem
{
private:
    halfedge* nextHalfEdge;
    halfedge* symHalfEdge;
    face* incidentFace;
    vertex* nextVertex;
    friend class mesh;

public:
    int ind;
    halfedge();
    halfedge(halfedge* nHalfEdge, halfedge* sHalfEdge, face* iFace, vertex* nVertex, int id);
    vertex* getNextVertex();
    halfedge* getNextHalfEdge();
    face* getIncidentFace();
    halfedge* getSymHalfEdge();
    void setNextVertex(vertex* v);
    void setNextHalfEdge(halfedge* n);
    void setSymHalfEdge(halfedge* s);
    void setIncidentFace(face* f);
};

#endif // HALFEDGE_H
