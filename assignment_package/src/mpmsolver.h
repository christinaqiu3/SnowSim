#ifndef MPMSOLVER_H
#define MPMSOLVER_H

#include "MPMParticle.h"
#include <QVector>

struct GridNode {
    QVector2D velocity;
    float mass;

    GridNode();
};

class MPMSolver
{
public:
    MPMSolver(int gridX, int gridY, float spacing);
    void addParticle(const MPMParticle& particle);
    void computeForcesAndIntegrate();

private:
    QVector<MPMParticle> particles;
    QVector<QVector<GridNode>> grid;
    int gridSizeX, gridSizeY;
    float gridSpacing;

    QVector2D computeGravity(const MPMParticle& p);
    QVector2D computeCohesion(const MPMParticle& p);
    void integrate(MPMParticle& p, QVector2D force);

    void particleToGridTransfer();

};

#endif // MPMSOLVER_H
