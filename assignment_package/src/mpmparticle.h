#pragma once
#include <QVector3D>
#include <QMatrix3x3>
#include <glm/glm.hpp>

class MPMParticle
{
public:
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float density;
    float volume;

    glm::mat3 FE;
    glm::mat3 FP;
    glm::mat3 sigma;

    MPMParticle(glm::vec3 pos, glm::vec3 vel, float m);

    void computeFE();
    void computeFP();
};



