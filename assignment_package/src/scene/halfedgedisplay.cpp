#include "halfedgedisplay.h"

halfedgedisplay::halfedgedisplay(OpenGLContext* context)
    : Drawable(context), representedHalfEdge(nullptr) {}

void halfedgedisplay::create() {

    std::vector<glm::vec4> pos {glm::vec4(representedHalfEdge->getNextVertex()->getPosition(), 1.0f),
                               glm::vec4(representedHalfEdge->getSymHalfEdge()->getNextVertex()->getPosition(), 1.0f)};

    //red to yellow
    std::vector<glm::vec4> col {glm::vec4(1, 1, 0, 1), glm::vec4(1, 0, 0, 1)};

    count = 2;

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}

void halfedgedisplay::updateHalfEdge(halfedge* halfEdge) {
    representedHalfEdge = halfEdge;
    create();
}

GLenum halfedgedisplay::drawMode()
{
    return GL_LINES;
}


halfedge* halfedgedisplay::getHalfEdge() {
    return representedHalfEdge;
}
