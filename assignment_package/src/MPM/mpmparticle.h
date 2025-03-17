#ifndef MPMPARTICLE_H
#define MPMPARTICLE_H

#include <QVector2D>

class MPMParticle
{
public:
    QVector2D position;
    QVector2D velocity;
    float mass;

    MPMParticle(QVector2D pos, QVector2D vel, float m);
};

#endif // MPMPARTICLE_H
