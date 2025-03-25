#pragma once
#include <QVector2D>

class MPMParticle
{
public:
    QVector3D position;
    QVector3D velocity;
    float mass;
    float density;
    float volume;

    MPMParticle(QVector3D pos, QVector3D vel, float m);
};

