#pragma once

// #ifndef PARTICLEDRAWABLE_H
// #define PARTICLEDRAWABLE_H


#pragma once

#include "drawable.h"
#include <vector>
#include <la.h>
#include <QVector4D>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


class ParticleDrawable : public Drawable {
private:
    std::vector<QVector4D> positions;

public:
    ParticleDrawable(OpenGLContext* context);

    void create() override;
    void updateParticles(const std::vector<QVector4D>& newPositions); // Update particle positions
    GLenum drawMode() override;
};


//#endif // PARTICLEDRAWABLE_H
