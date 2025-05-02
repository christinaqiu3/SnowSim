#include "mpmparticle.h"
#include <Eigen/Dense>
MPMParticle::MPMParticle(glm::vec3 pos, glm::vec3 vel, float m)
    : position(pos), velocity(vel), mass(m), FE(glm::mat3(1.f)), FP(glm::mat3(1.f)), sigma(glm::mat3(1.f)) {}
