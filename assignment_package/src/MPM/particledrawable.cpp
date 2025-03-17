#include "particledrawable.h"

ParticleDrawable::ParticleDrawable(OpenGLContext* context)
    : Drawable(context) {}

void ParticleDrawable::create() {
    count = particlePositions.size(); // Number of particles

    // Generate a buffer for particle positions
    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(QVector4D),
                             particlePositions.data(), GL_DYNAMIC_DRAW);
}

void ParticleDrawable::updateParticles(const std::vector<QVector4D>& newPositions) {
    particlePositions = newPositions;
    create();
}

GLenum ParticleDrawable::drawMode() {
    return GL_POINTS;
}
