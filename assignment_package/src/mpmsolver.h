#pragma once

// #ifndef MPMSOLVER_H
// #define MPMSOLVER_H

#include "MPMParticle.h"
#include "mpmgrid.h"
#include <QVector>


class MPMSolver
{
public:
    MPMSolver(QVector3D gridDim, float spacing, QVector3D gridOrigin);
    void addParticle(const MPMParticle& particle);
    void computeForcesAndIntegrate();

    const QVector<MPMParticle>& getParticles() const;

private:
    QVector<MPMParticle> particles;
    mpmgrid grid;

    QVector3D computeGravity(const MPMParticle& p);
    QVector3D computeCohesion(const MPMParticle& p);
    void integrate(MPMParticle& p, QVector3D force);

    void particleToGridTransfer();
    void computeInitialDensity();
};

//#endif // MPMSOLVER_H
