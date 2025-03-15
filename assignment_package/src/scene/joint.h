#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "drawable.h"
#include <QTreeWidgetItem>
#include <vector>
#include "smartpointerhelp.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QTreeWidgetItem>
//#include "scene/vertex.h"


class Joint : public Drawable, public QTreeWidgetItem {
public:
    QString name;
    Joint* parent;

    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 bindMatrix;
    std::vector<uPtr<Joint>> children;
    Joint* selectedJoint;

    ///constructors
    Joint();
    Joint(OpenGLContext* t);
    Joint(QString& n, Joint* p, const glm::vec3& pos, const glm::quat& rot);
    //Joint(const Joint &Joint);
    void create() override;
    GLenum drawMode() override;
    //std::string LoadJson(const char *filename);
    void parseJson(const QJsonObject& json);

    void calculateBindMatrix();

    void setRotation(double rx, double ry, double rz);
    void setPosition(double x, double y, double z);

    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();
    glm::mat4 getBindMatrix();
    QString getName();

    void setParent(Joint* p);

    Joint* getParent();
    bool isSelectedJoint();

    void updateSelectedJoint(Joint* j);
    Joint* getSelectedJoint();

    void createWireframe(std::vector<glm::vec4>* positions,
                         std::vector<int>* indices,
                         std::vector<glm::vec4>* colors);

    // Joint& operator=(const Joint &Joint);
    //destructors
    virtual ~Joint();

};
