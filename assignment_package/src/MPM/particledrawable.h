#ifndef PARTICLEDRAWABLE_H
#define PARTICLEDRAWABLE_H


#pragma once

#include "drawable.h"
#include <vector>
#include <la.h>
#include <QVector4D>

class ParticleDrawable : public Drawable {
private:
    std::vector<QVector4D> particlePositions;

public:
    ParticleDrawable(OpenGLContext* context);

    void create() override;
    void updateParticles(const std::vector<QVector4D>& newPositions); // Update particle positions
    GLenum drawMode() override;
};


#endif // PARTICLEDRAWABLE_H
