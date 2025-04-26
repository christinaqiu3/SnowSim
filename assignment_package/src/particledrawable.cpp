#include "particledrawable.h"
#include <iostream>


ParticleDrawable::ParticleDrawable(OpenGLContext* context)
    : Drawable(context), positions(std::vector<glm::vec4>()) {}

void ParticleDrawable::create() {
    count = positions.size(); // Number of particles
    std::vector<GLuint> indices;
    for (int i = 0; i<positions.size(); i++) {
        indices.push_back(i);
    }

    // NEEDED TO PUSH BACK INDICES AS WELL AS POSITIONS :P
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    //Generate a buffer for particle positions
    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(QVector4D),
                             positions.data(), GL_DYNAMIC_DRAW);
}

void ParticleDrawable::updateParticles(const std::vector<glm::vec4>& newPositions) {
    //std::cout << "Size1 = " << newPositions.size() << std::endl;
    //std::cout << "Size2 = " << positions.size() << std::endl;
    positions = newPositions;
    //std::cout << "Size2 = " << positions.size() << std::endl;
    create();
}

GLenum ParticleDrawable::drawMode() {
    return GL_POINTS;
}
