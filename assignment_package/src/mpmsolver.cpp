#include "mpmsolver.h"
#include <QDebug>
#include <Eigen/Dense>

const float dt = 0.01f; // Time step
const float gravity = 9.81f; // Gravity acceleration
const float cohesionStrength = 0.1f; // Cohesion force factor

MPMSolver::MPMSolver(glm::vec3 gridDim, float spacing, glm::vec3 gridOrigin, float dt,
                     float critCompression, float critStretch, float hardeningCoeff,
                     float initialDensity, float youngsMod, float poissonRatio)
    : stepSize(dt), grid(glm::vec3(gridDim), spacing, glm::vec3(gridOrigin)),
    critCompression(critCompression), critStretch(critStretch), hardeningCoeff(hardeningCoeff),
    initialDensity(initialDensity), youngsMod(youngsMod), poissonRatio(poissonRatio)
{
    mu0 = youngsMod / (2.f * (1.f + poissonRatio));
    lambda0 = (youngsMod * poissonRatio) / ((1.f+poissonRatio) + (1.f - 2.f*poissonRatio));
}

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

void MPMSolver::step() {
    grid.clearGrid();
    particleToGridTransfer();
    computeForce();
    updateGridVel();
    updateParticleDefGrad();
}

// THIS IS HELPER FUNCTION FOR COMPUTING WEIGHTING
// TODO : ENSURE ALL WEIGHTS IN KERNEL ADD UP TO 1
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
    if (x > 0.f && x < 1.f) {
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
        // CONVERT GLM MATRIX TO EIGEN
        Eigen::Matrix3f Fe = Eigen::Matrix3f::Identity();
        Eigen::Matrix3f Fp = Eigen::Matrix3f::Identity();

        for (int col = 0; col < 3; ++col) {
            for (int row = 0; row < 3; ++row) {
                Fe(row, col) = p.FE[row][col];
                Fp(row, col) = p.FP[row][col];
            }
        }

        // COMPUTE POLAR DECOMP TO GET ROTATIONAL PART OF F
        Eigen::JacobiSVD<Eigen::Matrix3f> svd(Fe, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Eigen::Matrix3f U = svd.matrixU();
        Eigen::Matrix3f V = svd.matrixV();
        Eigen::Matrix3f R = U * V.transpose();

        float Je = Fe.determinant();  // Elastic volume
        float Jp = Fp.determinant();  // Plastic volume

        float xi = 10.0f; // default value, tweak as needed

        // Plastic-hardening-modified Lame parameters
        float mu = mu0 * std::exp(xi * (1.0f - Jp));
        float lambda = lambda0 * std::exp(xi * (1.0f - Jp));

        Eigen::Matrix3f strain = Fe - R;
        Eigen::Matrix3f sigma = 2.f * mu * strain + lambda * (Je - 1.0f) * Eigen::Matrix3f::Identity();
        sigma *= (1.f / Je); // Cauchy stress

        // CONVERT BACK TO GLM
        p.sigma = glm::mat3(sigma(0,0), sigma(0,1), sigma(0,2),
                            sigma(1,0), sigma(1,1), sigma(1,2),
                            sigma(2,0), sigma(2,1), sigma(2,2));
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

        glm::vec3 vPic = glm::vec3(0.f);
        glm::vec3 vFlip = glm::vec3(0.f);
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
                    if (glm::any(glm::isnan(curNode.velocity))) continue; // THIS IS WHERE NANS ARE COMING FROM


                    glm::vec3 gradWeight;
                    gradWeight[0] = 1.f / grid.spacing * weightFunGradient(xGrid) * weightFun(yGrid) * weightFun(zGrid);
                    gradWeight[1] = 1.f / grid.spacing * weightFun(xGrid) * weightFunGradient(yGrid) * weightFun(zGrid);
                    gradWeight[2] = 1.f / grid.spacing * weightFun(xGrid) * weightFun(yGrid) * weightFunGradient(zGrid);

                    // THIS IS v * gradW^T
                    velGrad += glm::outerProduct(curNode.velocity, gradWeight);

                    vPic += curNode.velocity * weight;
                    vFlip += (curNode.velocity - curNode.prevVelocity) * weight;

                    // THIS IS THE OLD INCCORECT WAY OF UPDATING VELOCITY
                    // IT GIVES MOTION, BUT IS WRONG
                    //p.velocity += curNode.velocity * weight;
                }
            }
        }

        vFlip += p.velocity;
        float alpha = 0.95f; // Recommended 0.95

        p.velocity = (1.f - alpha) * vPic + alpha * vFlip;

        // UPDATE DEFORMATION GRADIENT
        p.FE = (glm::mat3(1.f) + stepSize * velGrad) * p.FE;


        // Predict the new total deformation gradient (FE * FP)
        glm::mat3 F_total = (glm::mat3(1.0f) + stepSize * velGrad) * p.FE * p.FP;

        // Predict the new elastic deformation gradient
        glm::mat3 FE_hat = (glm::mat3(1.0f) + stepSize * velGrad) * p.FE;

        // Convert to Eigen for SVD
        Eigen::Matrix3f FE_hat_eigen;
        for (int col = 0; col < 3; ++col)
            for (int row = 0; row < 3; ++row)
                FE_hat_eigen(row, col) = FE_hat[col][row];

        // SVD: FE_hat = U * Σ * V^T
        Eigen::JacobiSVD<Eigen::Matrix3f> svd(FE_hat_eigen, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Eigen::Matrix3f U = svd.matrixU();
        Eigen::Matrix3f V = svd.matrixV();
        Eigen::Vector3f sigma_hat = svd.singularValues(); // Σ̂

        // Clamp singular values to [1 - θc, 1 + θs]
        Eigen::Vector3f sigma_clamped = sigma_hat;
        for (int i = 0; i < 3; ++i)
            sigma_clamped[i] = std::clamp(sigma_hat[i], 1.0f - critCompression, 1.0f + critStretch);

        // Reconstruct clamped FE
        Eigen::Matrix3f Sigma_clamped = Eigen::Matrix3f::Zero();
        for (int i = 0; i < 3; ++i)
            Sigma_clamped(i, i) = sigma_clamped[i];

        Eigen::Matrix3f FE_new = U * Sigma_clamped * V.transpose();
        Eigen::Matrix3f F_total_eigen;
        for (int col = 0; col < 3; ++col)
            for (int row = 0; row < 3; ++row)
                F_total_eigen(row, col) = F_total[col][row];

        // Update FP using: FP = V * Σ⁻¹ * Uᵀ * F_total
        Eigen::Matrix3f Sigma_inv = Eigen::Matrix3f::Zero();
        for (int i = 0; i < 3; ++i)
            Sigma_inv(i, i) = 1.0f / sigma_clamped[i];

        Eigen::Matrix3f FP_new = V * Sigma_inv * U.transpose() * F_total_eigen;

        // Store back FE and FP
        for (int col = 0; col < 3; ++col)
            for (int row = 0; row < 3; ++row) {
                p.FE[col][row] = FE_new(row, col);
                p.FP[col][row] = FP_new(row, col);
            }





        // UPDATE POINT POSITIONS
        p.position += stepSize * p.velocity;

    }

    glm::vec3 minCorner = grid.center - 0.5f * grid.dimension;
    glm::vec3 maxCorner = grid.center + 0.5f * grid.dimension;

    float damping = 0.001f; // or try 0.01f, 0.1f for bounciness
    for (MPMParticle &p : particles) {
        for (int axis = 0; axis < 3; ++axis) {
            if (p.position[axis] < minCorner[axis]) {
                p.position[axis] = minCorner[axis];
                if (p.velocity[axis] < 0.f) {
                    p.velocity[axis] *= -damping;
                }
            }

            if (p.position[axis] > maxCorner[axis]) {
                p.position[axis] = maxCorner[axis];
                if (p.velocity[axis] > 0.f) {
                    p.velocity[axis] *= -damping;
                }
            }
        }
    }
}



// [======] GRID FUNCTIONS [======]

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

                    if (glm::length(p.velocity) > 1e-6f) { // Only if velocity is non-zero
                        curNode.velocityMass += curNode.mass;
                    }
                }
            }
        }
    }

    // Normalize grid node velocities using only effective mass
    for (GridNode& node : grid.gridNodes) {
        if (node.velocityMass > 0.f) {
            node.velocity /= node.velocityMass;
        } else {
            node.velocity = glm::vec3(0.f);
        }
    }

    // Currently the velocity stored is actually the total weighted momentum
    // To convert it to actual vel we divide each gridCell by its mass
    //grid.divideMass();
}

// THIS SHOULD ONLY BE CALLED ONCE AT t=0
void MPMSolver::computeInitialDensity() {
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
                }
            }
        }
    }

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

    // UPDATE CAUCHY STRESS FOR EACH PARTICLE
    computeSigma();

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

                    curNode.force -= (p.volume * p.sigma * gradWeight + (glm::vec3(0.f, gravity, 0.f) * p.mass));
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
            g.prevVelocity = g.velocity;
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

    // const int n = grid.gridNodes.size();
    // std::vector<Vector3> b(n);          // RHS vector (h * force)
    // std::vector<Vector3> deltaV(n, Vector3(0.0f)); // Unknown we're solving for

    // // Assemble right-hand side: b = h * f
    // for (int i = 0; i < n; ++i) {
    //     GridNode& g = grid.gridNodes[i];
    //     if (g.mass > 0.f) {
    //         b[i] = stepSize * g.force;
    //     }
    // }


    // // Conjugate Gradient Solver to solve:
    // // (M - h² * K) * deltaV = b
    // // Matrix-vector multiply is done by a custom function applySystemMatrix()

    // const int maxIters = 100;
    // const float tol = 1e-5;

    // conjugateGradientSolve(
    //     deltaV,
    //     b,
    //     [&](const std::vector<Vector3>& x) {
    //         return applySystemMatrix(x);
    //     },
    //     maxIters,
    //     tol
    //     );

    // // Apply deltaV to get new velocities
    // for (int i = 0; i < n; ++i) {
    //     GridNode& g = grid.gridNodes[i];
    //     if (g.mass > 0.f) {
    //         g.velocity += deltaV[i];

    //         // BOUNDING COLLISION (as before)
    //         if (g.worldPos.x <= minCorner.x || g.worldPos.x >= maxCorner.x)
    //             g.velocity.x = 0.f;
    //         if (g.worldPos.y <= minCorner.y || g.worldPos.y >= maxCorner.y)
    //             g.velocity.y = 0.f;
    //         if (g.worldPos.z <= minCorner.z || g.worldPos.z >= maxCorner.z)
    //             g.velocity.z = 0.f;
    //     }
    // }
}


