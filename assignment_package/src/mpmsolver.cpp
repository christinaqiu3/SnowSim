#include "mpmsolver.h"
#include <QDebug>

const float dt = 0.01f; // Time step
const float gravity = -9.81f; // Gravity acceleration
const float cohesionStrength = 0.1f; // Cohesion force factor

MPMSolver::MPMSolver(QVector3D gridDim, float spacing, QVector3D gridOrigin)
    : grid(QVector3D(gridDim), spacing, QVector3D(gridOrigin)) {}

void MPMSolver::addParticle(const MPMParticle& particle) {
    particles.append(particle);
}

void MPMSolver::computeForcesAndIntegrate() {
    for (MPMParticle& p : particles) {
        QVector3D force = computeGravity(p) + computeCohesion(p);
        integrate(p, force);
    }
}

QVector3D MPMSolver::computeGravity(const MPMParticle& p) {
    return QVector3D(0.0, p.mass * gravity, 0.0);
}

QVector3D MPMSolver::computeCohesion(const MPMParticle& p) {
    QVector3D cohesionForce(0.0, 0.0, 0.0);
    for (const MPMParticle& neighbor : particles) {
        if (&p == &neighbor) continue;
        QVector3D diff = neighbor.position - p.position;
        float distance = diff.length();
        if (distance > 0 && distance < grid.spacing) {
            cohesionForce += diff.normalized() * cohesionStrength / distance;
        }
    }
    return cohesionForce;
}

void MPMSolver::integrate(MPMParticle& p, QVector3D force) {
    QVector3D acceleration = force / p.mass;
    p.velocity += acceleration * dt;
    p.position += p.velocity * dt;
}

// THIS IS HELPER FUNCTION FOR COMPUTING WEIGHTING
static float cubicSpline1D (float x) {
    x = abs(x);
    if (x >= 0.f && x < 1.f) {
        return 0.5 * (x*x*x) - (x*x) + 2.f/3.f;
    } else if (x >= 1.f && x < 2.f) {
        return -(1.f/6.f) * (x*x*x) + (x*x) - 2.f*x +(4.f/3.f);
    } else {
        return 0.0f;
    }
}

void MPMSolver::particleToGridTransfer() {
    float xMin = grid.center.x() - 0.5f * grid.dimention.x();
    float yMin = grid.center.y() - 0.5f * grid.dimention.y();
    float zMin = grid.center.z() - 0.5f * grid.dimention.z();

    grid.clearGrid();
    for (MPMParticle& p : particles) {
        // World space positions
        float x = p.position[0];
        float y = p.position[1];
        float z = p.position[2];

        int i = static_cast<int>(std::floor((x - xMin) / grid.spacing ));
        int j = static_cast<int>(std::floor((y - yMin) / grid.spacing ));
        int k = static_cast<int>(std::floor((z - zMin) / grid.spacing ));


        // YOU NEED TO DO THIS FOR ALL CELLS WITHIN SOME RADIUS
        for(int di = -2; di < 2; di++) {
            for(int dj = -2; dj < 2; dj++) {
                for(int dk = -2; dk < 2; dk++) {
                    // INDEX OF CURRENT NODE WE ARE LOOKING AT
                    int iNode = i + di;
                    int jNode = j + dj;
                    int kNode = k + dk;

                    // CLAM TO BE INSIDE THE GRID DOMAIN
                    if(iNode < 0 || iNode >= grid.nx) continue;
                    if(jNode < 0 || jNode >= grid.ny) continue;
                    if(kNode < 0 || kNode >= grid.nz) continue;

                    // CENTER POSITION OF CUR NODE IN WORLD SPACE
                    float xNode = float(xMin) + (iNode + 0.5f)*grid.spacing;
                    float yNode = float(yMin) + (jNode + 0.5f)*grid.spacing;
                    float zNode = float(zMin) + (kNode + 0.5f)*grid.spacing;

                    // POSITION IN GRID SPACE
                    float xGrid = (x - xNode)/grid.spacing;
                    float yGrid = (y - yNode)/grid.spacing;
                    float zGrid = (z - zNode)/grid.spacing;

                    // WEIGHT OF GRID CELL RELATICE TO PARTICLE GRID
                    float weight = cubicSpline1D(xGrid) * cubicSpline1D(yGrid) * cubicSpline1D(zGrid);
                    if (weight == 0.0) continue;

                    // CURRENT NODE WE ARE LOOKING AT
                    GridNode* curNode = grid.getGridNode(x+(grid.spacing*di), y+(grid.spacing*dj), z+(grid.spacing*di));
                    curNode->mass += p.mass * weight;
                    curNode->velocity += p.velocity * p.mass * weight;
                }
            }
        }
    }

    // Currently the velocity stored is actually the total weighted momentum
    // To convert it to actual vel we divide each gridCell by its mass
    grid.divideMass();

}

const QVector<MPMParticle>& MPMSolver::getParticles() const {
    return particles;
}


// THIS SHOULD ONLY BE CALLED ONCE AT t=0
void MPMSolver::computeInitialDensity() {
    float xMin = grid.center.x() - 0.5f * grid.dimention.x();
    float yMin = grid.center.y() - 0.5f * grid.dimention.y();
    float zMin = grid.center.z() - 0.5f * grid.dimention.z();

    // COMPUTE GRID DENSITY
    float volume = grid.spacing * grid.spacing * grid.spacing;
    for (GridNode& g : grid.gridNodes) {
        g.density = (g.mass / volume);
    }

    // TRANSFER GRID DENSITY TO PARTICLES
    for (MPMParticle& p : particles) {
        // World space positions
        float x = p.position[0];
        float y = p.position[1];
        float z = p.position[2];

        int i = static_cast<int>(std::floor((x - xMin) / grid.spacing ));
        int j = static_cast<int>(std::floor((y - yMin) / grid.spacing ));
        int k = static_cast<int>(std::floor((z - zMin) / grid.spacing ));


        // YOU NEED TO DO THIS FOR ALL CELLS WITHIN SOME RADIUS
        for(int di = -2; di < 2; di++) {
            for(int dj = -2; dj < 2; dj++) {
                for(int dk = -2; dk < 2; dk++) {
                    // INDEX OF CURRENT NODE WE ARE LOOKING AT
                    int iNode = i + di;
                    int jNode = j + dj;
                    int kNode = k + dk;

                    // CLAM TO BE INSIDE THE GRID DOMAIN
                    if(iNode < 0 || iNode >= grid.nx) continue;
                    if(jNode < 0 || jNode >= grid.ny) continue;
                    if(kNode < 0 || kNode >= grid.nz) continue;

                    // CENTER POSITION OF CUR NODE IN WORLD SPACE
                    float xNode = float(xMin) + (iNode + 0.5f)*grid.spacing;
                    float yNode = float(yMin) + (jNode + 0.5f)*grid.spacing;
                    float zNode = float(zMin) + (kNode + 0.5f)*grid.spacing;

                    // POSITION IN GRID SPACE
                    float xGrid = (x - xNode)/grid.spacing;
                    float yGrid = (y - yNode)/grid.spacing;
                    float zGrid = (z - zNode)/grid.spacing;

                    // WEIGHT OF GRID CELL RELATICE TO PARTICLE GRID
                    float weight = cubicSpline1D(xGrid) * cubicSpline1D(yGrid) * cubicSpline1D(zGrid);
                    if (weight == 0.0) continue;

                    // SUM UP DENSITY CONTRIBUTIONS
                    GridNode* curNode = grid.getGridNode(x+(grid.spacing*di), y+(grid.spacing*dj), z+(grid.spacing*di));
                    p.density += curNode->density * weight;
                }
            }
        }

        // DIVIDE TO GET INITIAL VOLUME OF EACH PARTICLE
        p.volume = p.mass / p.density;
    }
}

