#ifndef MPMGRID_H
#define MPMGRID_H

#include <vector>
#include <la.h>
#include <QVector4D>
#include <QVector3D>
#include <glm/glm.hpp>

struct GridNode {
    glm::vec3 velocity;
    float mass;
    float density;
    glm::vec3 force;

    glm::vec3 worldPos;
    glm::ivec3 idx;
    GridNode();
};

class mpmgrid
{
private:

public:
    mpmgrid();
    mpmgrid(glm::vec3 dim, float spacing, glm::vec3 center);
    glm::vec3 dimension;
    float spacing;
    glm::vec3 center;
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
