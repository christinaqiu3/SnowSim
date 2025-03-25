#include "mpmsolver.h"
#include <QDebug>

const float dt = 0.01f; // Time step
const float gravity = -9.81f; // Gravity acceleration
const float cohesionStrength = 0.1f; // Cohesion force factor

MPMSolver::MPMSolver(glm::vec3 gridDim, float spacing, glm::vec3 gridOrigin, float dt)
    : grid(glm::vec3(gridDim), spacing, glm::vec3(gridOrigin)), stepSize(dt) {}

void MPMSolver::addParticle(const MPMParticle& particle) {
    particles.append(particle);
}

void MPMSolver::computeForcesAndIntegrate() {
    for (MPMParticle& p : particles) {
        glm::vec3 force = computeGravity(p) + computeCohesion(p);
        integrate(p, force);
    }
}

glm::vec3 MPMSolver::computeGravity(const MPMParticle& p) {
    return glm::vec3(0.0, p.mass * gravity, 0.0);
}

glm::vec3 MPMSolver::computeCohesion(const MPMParticle& p) {
    glm::vec3 cohesionForce(0.0, 0.0, 0.0);
    for (const MPMParticle& neighbor : particles) {
        if (&p == &neighbor) continue;
        glm::vec3 diff = neighbor.position - p.position;
        float distance = diff.length();
        if (distance > 0 && distance < grid.spacing) {
            cohesionForce += (diff * (1.f / diff.length())) * cohesionStrength / distance;
        }
    }
    return cohesionForce;
}

void MPMSolver::integrate(MPMParticle& p, glm::vec3 force) {
    glm::vec3 acceleration = force / p.mass;
    p.velocity += acceleration * dt;
    p.position += p.velocity * dt;
}

// THIS IS HELPER FUNCTION FOR COMPUTING WEIGHTING
static float weightFun (float x) {
    x = abs(x);
    if (x >= 0.f && x < 1.f) {
        return 0.5f * (x*x*x) - (x*x) + 2.f/3.f;
    } else if (x >= 1.f && x < 2.f) {
        return -(1.f/6.f) * (x*x*x) + (x*x) - 2.f*x +(4.f/3.f);
    } else {
        return 0.0f;
    }
}

static float weightFunGradient (float x) {
    x = abs(x);
    if (x >= 0.f && x < 1.f) {
        return 3.f*abs(x)/(2.f*x) - 2.f*x;
    } else if (x >= 1.f && x < 2.f) {
        return -abs(x)/(2.f*x)*x*x + 2.f*x - 2.f*(abs(x)/x);
    } else {
        return 0.0f;
    }
}


void MPMSolver::particleToGridTransfer() {
    float xMin = grid.center.x - 0.5f * grid.dimension.x;
    float yMin = grid.center.y - 0.5f * grid.dimension.y;
    float zMin = grid.center.z - 0.5f * grid.dimension.z;

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
                    float weight = weightFun(xGrid) * weightFun(yGrid) * weightFun(zGrid);
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
    float xMin = grid.center.x - 0.5f * grid.dimension.x;
    float yMin = grid.center.y - 0.5f * grid.dimension.y;
    float zMin = grid.center.z - 0.5f * grid.dimension.z;

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
                    float weight = weightFun(xGrid) * weightFun(yGrid) * weightFun(zGrid);
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

void MPMSolver::computeForce() {
    // ZERO OUT FORCE FOR EACH GRID CELL
    for (GridNode& g : grid.gridNodes) {
        g.force = glm::vec3(0.f, 0.f, 0.f);
    }

    float xMin = grid.center.x - 0.5f * grid.dimension.x;
    float yMin = grid.center.y - 0.5f * grid.dimension.y;
    float zMin = grid.center.z - 0.5f * grid.dimension.z;

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

                    glm::vec3 gradWeight;
                    gradWeight[0] = 1.f / grid.spacing * weightFunGradient(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    gradWeight[1] = 1.f / grid.spacing * weightFun(xGrid) * weightFunGradient(yGrid) * weightFun(zGrid);
                    gradWeight[2] = 1.f / grid.spacing * weightFun(xGrid) * weightFun(yGrid) * weightFunGradient(zGrid);

                    GridNode* curNode = grid.getGridNode(x+(grid.spacing*di), y+(grid.spacing*dj), z+(grid.spacing*di));
                    curNode->force -= p.volume * p.sigma * gradWeight + computeGravity(p);
                }
            }
        }
    }
}

void MPMSolver::updateGridVel() {
    // EXPLICIT UPDATE JUST TO TEST
    for (GridNode& g : grid.gridNodes) {
        if (g.mass > 0.f) {

            glm::vec3 minCorner = grid.center - 0.5f * grid.dimension;
            glm::vec3 maxCorner = grid.center + 0.5f * grid.dimension;

            g.velocity += stepSize * (1.f/g.mass) * g.force;
        }
    }
}

