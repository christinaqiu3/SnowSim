
#include "joint.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <ostream>


Joint::Joint() : Drawable(nullptr), name(), parent(), position(), rotation(), selectedJoint(nullptr) {
    children = std::vector<uPtr<Joint>>();
    calculateBindMatrix();
    QTreeWidgetItem::setText(0, getName());
}

Joint::Joint(OpenGLContext* t)
    : Drawable(t), name(), parent(), position(), rotation(), selectedJoint(nullptr) {
    children = std::vector<uPtr<Joint>>();
    calculateBindMatrix();
    QTreeWidgetItem::setText(0, getName());
}

Joint::Joint(QString& n, Joint* p, const glm::vec3& pos, const glm::quat& rot)
    : Drawable(nullptr), name(n), parent(p), position(pos), rotation(rot), selectedJoint(nullptr) {
    children = std::vector<uPtr<Joint>>();
    calculateBindMatrix();
    QTreeWidgetItem::setText(0, getName());
}

// Function to calculate the bind matrix based on position and rotation
void Joint::calculateBindMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    bindMatrix = glm::inverse(translationMatrix * rotationMatrix);
}

// Function to get local transformation matrix (position + rotation)
glm::mat4 Joint::getLocalTransformation() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    return translationMatrix * rotationMatrix;
}

// Function to get overall transformation matrix (local transformation + parent's transformation)
glm::mat4 Joint::getOverallTransformation() {
    if(parent == nullptr) {
        return getLocalTransformation();
    }
    glm::mat4 localTransform = getLocalTransformation();
//    Joint* temp = this;
//    while () {
//        glm::mat4 parentTransform = parent->getOverallTransformation();
//        localTransform = parentTransform * localTransform;
//        temp = static_cast<Joint*>(temp->getParent());
//    }
    return parent->getOverallTransformation() * localTransform;
}

// Getter for bind matrix
glm::mat4 Joint::getBindMatrix() {
    return bindMatrix;
}

Joint* Joint::getParent() {
    return parent;
}

QString Joint::getName() {
    return name;
}

void Joint::setParent(Joint* p) {
    parent = p;
}

void Joint::createWireframe(std::vector<glm::vec4>* positions, std::vector<int>* indices, std::vector<glm::vec4>* colors) {

    bool selected = isSelected();
    int idxCounter = positions->size();

    int tempCount = idxCounter;
    float angle = glm::radians(45.f);
    for (int i = 0; i<8; i++) {
        positions->push_back(getOverallTransformation() * glm::vec4(0.f, sin(i*angle) * 0.5f, cos(i*angle) * .5f, 1.f)) ;
        if(selected) {
            colors->push_back(glm::vec4(1.f, 1.f, 1.f, 1.f));
        } else {
        colors->push_back(glm::vec4(1.f, 1.f, 0.f, 1.f));
        }
        indices->push_back(idxCounter);
        indices->push_back(idxCounter + 1);
        //indices->push_back((idxCounter + 1) % 8 + tempCount);
        idxCounter++;
    }
    indices->back() -= 8;

    // point of possible failure
    tempCount = idxCounter;
    for (int i = 0; i<8; i++) {
        positions->push_back(getOverallTransformation() * glm::vec4(sin(i*angle) * 0.5f, 0.f, cos(i*angle) * .5f, 1.f));
        if(selected) {
            colors->push_back(glm::vec4(1.f, 1.f, 1.f, 1.f));
        } else {
            colors->push_back(glm::vec4(0.f, 0.f, 1.f, 1.f));
        }
        indices->push_back(idxCounter);
        indices->push_back((idxCounter + 1));
        idxCounter++;
    }
    indices->back() -= 8;

    tempCount = idxCounter;
    for (int i = 0; i<8; i++) {
        positions->push_back(getOverallTransformation() * glm::vec4(sin(i*angle) * 0.5f, cos(i*angle) * .5f, 0.f, 1.f));
        if(selected) {
            colors->push_back(glm::vec4(1.f, 1.f, 1.f, 1.f));
        } else {
            colors->push_back(glm::vec4(0.f, 1.f, 0.f, 1.f));
        }
        indices->push_back(idxCounter);
        indices->push_back((idxCounter + 1));
        idxCounter++;
    }
    indices->back() -= 8;

    // add line to parent
    if (parent != nullptr) {
        positions->push_back(getOverallTransformation() * glm::vec4(0, 0, 0, 1)) ;
        positions->push_back(parent->getOverallTransformation() * glm::vec4(0, 0, 0, 1) );//check last two elems of positions is normal
        colors->push_back(glm::vec4(1, 1, 0, 1));
        colors->push_back(glm::vec4(1, 0, 0, 1));
        indices->push_back(idxCounter);
        indices->push_back(idxCounter+1);
        std::cout << "should have added line" <<std::endl;
        idxCounter++;
        idxCounter++;
    }

    for (auto& child : children)
    {
        child->createWireframe(positions, indices, colors);
        std::cout << "added child" << std::endl;
    }

}

void Joint::create() {
    std::vector<std::string> names;
    std::vector<glm::vec4> positions;
    std::vector<int> indices;
    std::vector<glm::vec4> colors;

    //each joint cares abt three circles plus line.
    //indices of position just must be correct
    //piush back pos and color at same tiem for wireframe.
    //first 36, if not root node then 38.

    createWireframe(&positions, &indices, &colors);

    if (indices.size() > 0) {

        count = indices.size();

        generatePos();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
        mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

        generateIdx();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufIdx);
        mp_context->glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(glm::vec4), indices.data(), GL_STATIC_DRAW);

        generateCol();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        mp_context->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);

    }


}

GLenum Joint::drawMode()
{
    return GL_LINES;
}

void Joint::updateSelectedJoint(Joint* j) {
    selectedJoint = j;
    create();
}

Joint* Joint::getSelectedJoint() {
    return selectedJoint;
}

bool Joint::isSelectedJoint() {
    return selectedJoint == this;
}

void Joint::setRotation(double rx, double ry, double rz) {
    float angle = 5.f;
    double axisX = rx;
    double axisY = ry;
    double axisZ = rz;
    glm::quat newRotation = glm::angleAxis(angle, glm::vec3(axisX, axisY, axisZ));

    rotation = newRotation * rotation;//change it
    std::cout << "rotating" << std::endl;

}

void Joint::setPosition(double x, double y, double z) {
    position = glm::vec3(position.x + x, position.y + y, position.z + z);
}

void Joint::parseJson(const QJsonObject& json) {
    this->setText(0, json["name"].toString());

    // Extract position data
    QJsonArray pos = json["pos"].toArray();
    position = glm::vec3(pos[0].toDouble(), pos[1].toDouble(), pos[2].toDouble());

    // Extract rotation data and create a quaternion
    QJsonArray rot = json["rot"].toArray();
    double angle = rot[0].toDouble();
    double axisX = rot[1].toDouble();
    double axisY = rot[2].toDouble();
    double axisZ = rot[3].toDouble();
    rotation = glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(axisX, axisY, axisZ));

    // Create a glm::quat using rotArray values (you'll need to convert angle-axis to quaternion)
    //    Do the
    //        1 0 0 x
    //        0 1 0 y
    //        0 0 1 z
    //        0 0 0 1
    //    and then u mutliply this thingy with the rotation

    // Iterate over children and create Joint objects for each child
    QJsonArray jsonChildren = json["children"].toArray();
    for (auto child : jsonChildren) {
        uPtr<Joint> childJointUptr = mkU<Joint>();
        Joint* childJoint = childJointUptr.get();
        children.push_back(std::move(childJointUptr));

        childJoint->setParent(this);
        childJoint->name = child.toObject()["name"].toString();
        QTreeWidgetItem::addChild((QTreeWidgetItem*) childJoint);
        std::cout<< "adding child to ui" << std::endl;

        childJoint->parseJson(child.toObject()); // Recursive call
    }
}

Joint::~Joint()
{}

//add ids
//ppos rot child
//qjsonarray
//for each child call joint, while giving this as parent

//helper func for wireframe, do rt=otation of x y z. give them colors
//fill the idx, has to be 0-1, 1-2, 2-3,...
//if node has a parent, make kines by creating line from ist position to its parent position
//do all this in local space then transfer it later

//pos.push_back(glm::inverse(this->getLocalTransformation()) * glm::vec4(0,0,0,1)));
//override draw to take in a shader (make sure to instantiate), setmodelmatrix to overaltransformratoion. then drawthis. only call once for root node and then it draws recursively for children
