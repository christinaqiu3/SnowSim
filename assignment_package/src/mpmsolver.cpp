#include "mpmsolver.h"
#include <QDebug>
#include <Eigen/Dense>

const float dt = 0.01f; // Time step
const float gravity = -9.81f; // Gravity acceleration
const float cohesionStrength = 0.1f; // Cohesion force factor

MPMSolver::MPMSolver(glm::vec3 gridDim, float spacing, glm::vec3 gridOrigin, float dt)
    : stepSize(dt), grid(glm::vec3(gridDim), spacing, glm::vec3(gridOrigin)) {}

void MPMSolver::addParticle(const MPMParticle& particle) {
    particles.append(particle);
}


// THIS IS ORIGINAL PARTICLE STUFF ?? IDK IT WAS HERE WHEN I GOT HERE
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

const QVector<MPMParticle>& MPMSolver::getParticles() const {
    return particles;
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

// [======] PARTICLE FUNCTIONS [======]

void MPMSolver::computeSigma() {
    for (MPMParticle& p : particles) {
        glm::mat3 F = p.FE;

        // Polar decomposition: F = R * S
        glm::mat3 R, S;
        polarDecompose(F, R, S); // Or use SVD to get R from U*Sigma*V^T

        float J = glm::determinant(F); // For volume correction

        glm::mat3 strain = F - R;
        //glm::mat3 stress = 2.f * mu * strain + lambda * glm::trace(strain) * glm::mat3(1.f);

        p.sigma =  (1.f / J) * stress;
    }
}

void MPMSolver::updateParticleDefGrad() {
    //Fn+1 = (I + ∆t∇v^n+1_p)Fn

    float xMin = grid.center.x - 0.5f * grid.dimension.x;
    float yMin = grid.center.y - 0.5f * grid.dimension.y;
    float zMin = grid.center.z - 0.5f * grid.dimension.z;

    for (MPMParticle& p : particles) {
        glm::mat3 velGrad = glm::mat3(0.f);

        // WORLD SPACE POSITIONS
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

                    // CLAMP TO BE INSIDE THE GRID DOMAIN
                    if(iNode < 0 || iNode >= grid.nx) continue;
                    if(jNode < 0 || jNode >= grid.ny) continue;
                    if(kNode < 0 || kNode >= grid.nz) continue;

                    int idx = iNode + grid.nx*(jNode + grid.ny*kNode);
                    GridNode &curNode = grid.gridNodes[idx];

                    // POSITION IN GRID SPACE
                    float xGrid = (x - curNode.worldPos.x)/grid.spacing;
                    float yGrid = (y - curNode.worldPos.y)/grid.spacing;
                    float zGrid = (z - curNode.worldPos.z)/grid.spacing;

                    // WEIGHT OF GRID CELL RELATIVE TO PARTICLE GRID
                    float weight = weightFun(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    if (weight == 0.0) continue;

                    // TRANSFER WEIGHTED VELOCITY FROM GRID TO PARTICLE
                    //p.velocity += curNode.velocity * weight;

                    glm::vec3 gradWeight;
                    gradWeight[0] = 1.f / grid.spacing * weightFunGradient(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    gradWeight[1] = 1.f / grid.spacing * weightFun(xGrid) * weightFunGradient(yGrid) * weightFun(zGrid);
                    gradWeight[2] = 1.f / grid.spacing * weightFun(xGrid) * weightFun(yGrid) * weightFunGradient(zGrid);

                    // THIS IS v * gradW^T
                    velGrad += glm::outerProduct(curNode.velocity, gradWeight);

                    // NAIVE WAY OF UPDATING VELOCITY
                    // GO BACK AND DO THE PIC/FLIP METHOD FOR BETTER RESUTLS
                    p.velocity += curNode.velocity * weight;
                }
            }
        }

        // UPDATE DEFORMATION GRADIENT
        p.FE = (glm::mat3(1.f) + stepSize * velGrad) * p.FE;
        p.position += stepSize * p.velocity;
    }
}



// [======] GIRD FUNCTIONS [======]

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

                    // CLAMP TO BE INSIDE THE GRID DOMAIN
                    if(iNode < 0 || iNode >= grid.nx) continue;
                    if(jNode < 0 || jNode >= grid.ny) continue;
                    if(kNode < 0 || kNode >= grid.nz) continue;


                    int idx = iNode + grid.nx*(jNode + grid.ny*kNode);
                    GridNode &curNode = grid.gridNodes[idx];

                    // POSITION IN GRID SPACE
                    float xGrid = (x - curNode.worldPos.x)/grid.spacing;
                    float yGrid = (y - curNode.worldPos.y)/grid.spacing;
                    float zGrid = (z - curNode.worldPos.z)/grid.spacing;

                    // WEIGHT OF GRID CELL RELATICE TO PARTICLE GRID
                    float weight = weightFun(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    if (weight == 0.0) continue;

                    // UPDATE CURRENT NODE WE ARE LOOKING AT
                    curNode.mass += p.mass * weight;
                    curNode.velocity += p.velocity * p.mass * weight;
                }
            }
        }
    }

    // Currently the velocity stored is actually the total weighted momentum
    // To convert it to actual vel we divide each gridCell by its mass
    grid.divideMass();

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

                    int idx = iNode + grid.nx*(jNode + grid.ny*kNode);
                    GridNode &curNode = grid.gridNodes[idx];

                    // POSITION IN GRID SPACE
                    float xGrid = (x - curNode.worldPos.x)/grid.spacing;
                    float yGrid = (y - curNode.worldPos.y)/grid.spacing;
                    float zGrid = (z - curNode.worldPos.z)/grid.spacing;

                    // WEIGHT OF GRID CELL RELATICE TO PARTICLE GRID
                    float weight = weightFun(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    if (weight == 0.0) continue;

                    // SUM UP DENSITY CONTRIBUTIONS
                    p.density += curNode.density * weight;
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

                    int idx = iNode + grid.nx*(jNode + grid.ny*kNode);
                    GridNode &curNode = grid.gridNodes[idx];

                    // POSITION IN GRID SPACE
                    float xGrid = (x - curNode.worldPos.x)/grid.spacing;
                    float yGrid = (y - curNode.worldPos.y)/grid.spacing;
                    float zGrid = (z - curNode.worldPos.z)/grid.spacing;

                    glm::vec3 gradWeight;
                    gradWeight[0] = 1.f / grid.spacing * weightFunGradient(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    gradWeight[1] = 1.f / grid.spacing * weightFun(xGrid) * weightFunGradient(yGrid) * weightFun(zGrid);
                    gradWeight[2] = 1.f / grid.spacing * weightFun(xGrid) * weightFun(yGrid) * weightFunGradient(zGrid);

                    curNode.force -= p.volume * p.sigma * gradWeight + computeGravity(p);
                }
            }
        }
    }
}


// QUICK NOTE ABOUT THIS: The full implementation described in the paper uses a semi-implicit method for solving
//                        which results in a more stable and complex system. Since this is a push to get something out
//                        I am just doing an explicit solve with the most basic collision handling possible.
//                        For future implementation, fix this so that it fully implements the paper
void MPMSolver::updateGridVel() {
    glm::vec3 minCorner = grid.center - 0.5f * grid.dimension;
    glm::vec3 maxCorner = grid.center + 0.5f * grid.dimension;
    // EXPLICIT UPDATE JUST TO TEST
    for (GridNode& g : grid.gridNodes) {
        if (g.mass > 0.f) {
            // COMPUTE VEL FROM GRID FORCES
            g.velocity += stepSize * (1.f/g.mass) * g.force;

            // VERY SIMPLE BOUNDING COLLISION
            // CLAMP VELOCITY AT BOUNDS
            if (g.worldPos.x <= minCorner.x || g.worldPos.x >= maxCorner.x) {
                g.velocity.x = 0.f;
            }

            if (g.worldPos.y <= minCorner.y || g.worldPos.y >= maxCorner.y) {
                g.velocity.y = 0.f;
            }

            if (g.worldPos.z <= minCorner.z || g.worldPos.z >= maxCorner.z) {
                g.velocity.z = 0.f;
            }
        }
    }
}

