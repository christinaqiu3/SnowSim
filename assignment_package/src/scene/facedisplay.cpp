#include "facedisplay.h"

facedisplay::facedisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr)
{}

void facedisplay::create() {

    halfedge* current = representedFace->getIncidentHalfEdge();
        //do while push back vert pos and col
    std::vector<glm::vec4> pos;
    std::vector<GLuint> idx;
    glm::vec4 color = glm::vec4(glm::vec3(1, 1, 1) - representedFace->getColor(), 0);
    std::vector<glm::vec4> col;

    do {
        pos.push_back(glm::vec4(current->getNextVertex()->getPosition(), 1.0f));
        current = current->getNextHalfEdge();
        col.push_back(color);
    } while (current != representedFace->getIncidentHalfEdge());

    //in separate for loop fill out idx.push_back(i); from 0 to num of pos. triangluate

    for (int i = 0; i < (int) pos.size(); i++) {
        if (i == (int) pos.size() - 1) {
            idx.push_back(i);
            idx.push_back(0);
        } else {
        idx.push_back(i);
        idx.push_back(i+1);
        }
    }

    count = idx.size();

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);


}

void facedisplay::updateFace(face* face) {
    representedFace = face;
    create();
}

GLenum facedisplay::drawMode()
{
    return GL_LINES;
}

face* facedisplay::getFace() {
    return representedFace;
}
