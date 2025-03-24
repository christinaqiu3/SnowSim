#include "mpmparticle.h"

MPMParticle::MPMParticle(QVector3D pos, QVector3D vel, float m)
    : position(pos), velocity(vel), mass(m) {}
