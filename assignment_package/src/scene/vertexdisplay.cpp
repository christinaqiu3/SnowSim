#include "vertexdisplay.h"
#include <iostream>

vertexdisplay::vertexdisplay(OpenGLContext* context)
    : Drawable(context), representedVertex(nullptr) {}

void vertexdisplay::create() {

    std::vector<glm::vec4> pos {glm::vec4(representedVertex->getPosition(), 1.0f)};

    std::vector<glm::vec4> col {glm::vec4(1, 1, 1, 1)};

    count = 1;

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}

void vertexdisplay::updateVertex(vertex* vertex) {
    representedVertex = vertex;
    std::cout << vertex->jointInfluences[0].first->getName().toStdString() << std::endl;
    create();
}

GLenum vertexdisplay::drawMode()
{
    return GL_POINTS;
}

vertex* vertexdisplay::getVertex() {
    return representedVertex;
}
