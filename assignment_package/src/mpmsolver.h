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
    MPMSolver(glm::vec3 gridDim, float spacing, glm::vec3 gridOrigin, float dt,
              float critCompression, float critStretch,
              float hardeningCoeff, float initialDensity, float youngsMod,
              float poissonRatio);
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

    float mu0;
    float lambda0;

    void computeInitialDensity();

    // THIS IS THE MAIN FUNCTION THAT SHOULD UPDATE THE SIM EACH FRAME
    void step();

private:
    QVector<MPMParticle> particles;
    mpmgrid grid;


    glm::vec3 computeGravity(const MPMParticle& p);
    glm::vec3 computeCohesion(const MPMParticle& p);
    void integrate(MPMParticle& p, glm::vec3 force);

    // PARTICLE FUNCTIONS
    void computeSigma();
    void updateParticleDefGrad();

    // GRID FUNCTIONS
    void particleToGridTransfer();
    void computeForce();
    void updateGridVel();

};

//#endif // MPMSOLVER_H
