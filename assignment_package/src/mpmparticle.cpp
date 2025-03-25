#include "mpmparticle.h"

MPMParticle::MPMParticle(glm::vec3 pos, glm::vec3 vel, float m)
    : position(pos), velocity(vel), mass(m), FE(glm::mat3()), FP(glm::mat3()), sigma(glm::mat3()) {}
