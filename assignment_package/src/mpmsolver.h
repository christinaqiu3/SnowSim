#pragma once

// #ifndef MPMSOLVER_H
// #define MPMSOLVER_H

#include "MPMParticle.h"
#include "mpmgrid.h"
#include <QVector>
#include <glm/glm.hpp>


class MPMSolver
{
public:
    MPMSolver(glm::vec3 gridDim, float spacing, glm::vec3 gridOrigin, float dt);
    void addParticle(const MPMParticle& particle);
    void computeForcesAndIntegrate();

    const QVector<MPMParticle>& getParticles() const;

    float stepSize;
    float critCompression;
    float critStretch;
    float hardeningCoeff;
    float initialDensity;
    float youngsMod;
    float poissonRatio;

private:
    QVector<MPMParticle> particles;
    mpmgrid grid;


    glm::vec3 computeGravity(const MPMParticle& p);
    glm::vec3 computeCohesion(const MPMParticle& p);
    void integrate(MPMParticle& p, glm::vec3 force);


    // PARTICLE FUNCTIONS
    void computeSigma();
    void updateParticleVel();
    void updateParticleDefGrad();

    // GRID FUNCTIONS
    void particleToGridTransfer();
    void computeInitialDensity();
    void computeForce();
    void updateGridVel();

};

//#endif // MPMSOLVER_H
