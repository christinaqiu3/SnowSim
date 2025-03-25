#ifndef MPMGRID_H
#define MPMGRID_H

#include <vector>
#include <la.h>
#include <QVector4D>
#include <QVector3D>

struct GridNode {
    QVector3D velocity;
    float mass;
    float density;
    GridNode();
};

class mpmgrid
{
private:

public:
    mpmgrid();
    mpmgrid(QVector3D dim, float spacing, QVector3D center);
    QVector3D dimention;
    float spacing;
    QVector3D center;
    std::vector<GridNode> gridNodes;

    // NUMBER OF CELLS IN EACH DIM
    int nx;
    int ny;
    int nz;

    GridNode* getGridNode(float x, float y, float z);
    void clearGrid();
    void divideMass();
};

#endif // MPMGRID_H
