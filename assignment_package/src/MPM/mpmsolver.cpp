#include "mpmsolver.h"
#include <QDebug>

const float dt = 0.01f; // Time step
const float gravity = -9.81f; // Gravity acceleration
const float cohesionStrength = 0.1f; // Cohesion force factor

GridNode::GridNode() : velocity(0, 0), mass(0) {}

MPMSolver::MPMSolver(int gridX, int gridY, float spacing)
    : gridSizeX(gridX), gridSizeY(gridY), gridSpacing(spacing) {
    grid.resize(gridSizeX, QVector<GridNode>(gridSizeY));
}

void MPMSolver::addParticle(const MPMParticle& particle) {
    particles.append(particle);
}

void MPMSolver::computeForcesAndIntegrate() {
    for (MPMParticle& p : particles) {
        QVector2D force = computeGravity(p) + computeCohesion(p);
        integrate(p, force);
    }
}

QVector2D MPMSolver::computeGravity(const MPMParticle& p) {
    return QVector2D(0, p.mass * gravity);
}

QVector2D MPMSolver::computeCohesion(const MPMParticle& p) {
    QVector2D cohesionForce(0, 0);
    for (const MPMParticle& neighbor : particles) {
        if (&p == &neighbor) continue;
        QVector2D diff = neighbor.position - p.position;
        float distance = diff.length();
        if (distance > 0 && distance < gridSpacing) {
            cohesionForce += diff.normalized() * cohesionStrength / distance;
        }
    }
    return cohesionForce;
}

void MPMSolver::integrate(MPMParticle& p, QVector2D force) {
    QVector2D acceleration = force / p.mass;
    p.velocity += acceleration * dt;
    p.position += p.velocity * dt;
}

void MPMSolver::particleToGridTransfer() {
    //TODO FIZZY
}

const QVector<MPMParticle>& MPMSolver::getParticles() const {
    return particles;
}

