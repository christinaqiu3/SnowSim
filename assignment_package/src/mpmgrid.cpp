#include "mpmgrid.h"
#include <cmath>

GridNode::GridNode()
    : velocity(0.f, 0.f, 0.f), mass(0.f), density(0.f)
{}

mpmgrid::mpmgrid()
    : dimention(QVector3D(3.0f, 3.0f, 3.0f)),
    spacing(1.f),
    center(QVector3D(0.f, 0.f, 0.f))
{}


mpmgrid::mpmgrid(QVector3D dim, float spc, QVector3D cent)
    : dimention(dim),
    spacing(spc),
    center(cent)
{
    nx = static_cast<int>(std::floor(dimention.x() / spacing));
    ny = static_cast<int>(std::floor(dimention.y() / spacing));
    nz = static_cast<int>(std::floor(dimention.z() / spacing));

    if(nx < 1) nx = 1;
    if(ny < 1) ny = 1;
    if(nz < 1) nz = 1;

    gridNodes.resize(nx * ny * nz);
}


GridNode* mpmgrid::getGridNode(float x, float y, float z)
{
    float xMin = center.x() - 0.5f * dimention.x();
    float yMin = center.y() - 0.5f * dimention.y();
    float zMin = center.z() - 0.5f * dimention.z();

    int i = static_cast<int>(std::floor((x - xMin) / spacing));
    int j = static_cast<int>(std::floor((y - yMin) / spacing));
    int k = static_cast<int>(std::floor((z - zMin) / spacing));

    if(i < 0)    i = 0;
    if(i >= nx)  i = nx - 1;
    if(j < 0)    j = 0;
    if(j >= ny)  j = ny - 1;
    if(k < 0)    k = 0;
    if(k >= nz)  k = nz - 1;

    int idx = i + nx * (j + ny * k);

    return &gridNodes[idx];
}

void mpmgrid::clearGrid() {
    // CLEAR GRID FOR NEW VALUES
    for (GridNode& node : gridNodes) {
        node.mass = 0.0f;
        node.velocity = QVector3D(0.f, 0.f, 0.f);
    }
}

// THIS FUNCTION IS USED WHEN TRANSFERING PARTICLES TO THE GRID
// THIS DIVIDES THE VELOCITY OF EACH NODE BY ITS MASS
//
// ONLY CALL THIS WHEN YOU ARE SURE THE STORED VELOCITY IS MOMENTUM
void mpmgrid::divideMass() {
    for (GridNode& n : gridNodes) {
        if (n.mass != 0.0) {
            n.velocity /= n.mass;
        }
    }
}

