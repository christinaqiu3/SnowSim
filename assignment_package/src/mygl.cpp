#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
//#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include <ui_mainwindow.h>
#include "scene/mesh.h"
#include "scene/joint.h"


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
    m_progLambert(this), m_progFlat(this), m_prog_skeleton(this),
    m_glCamera(), m_mesh(this),
    solver(glm::vec3(5.0, 5.0, 5.0), 1.0, glm::vec3(0.0f, 0.0f, 0.0f), 0.5f),
    m_joint(nullptr), m_vertDisplay(this), m_halfedgeDisplay(this),
    m_faceDisplay(this),
    vx(0), vy(0), vz(0),
    jx(0), jy(0), jz(0),
    rx(0), ry(0), rz(0)

{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_mesh.destroy();
    m_joint->destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    m_geomSquare.create();
    //m_mesh.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    m_prog_skeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_prog_skeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(glm::vec3(m_glCamera.eye));
    m_progFlat.setModelMatrix(glm::mat4(1.f));

    // m_prog_skeleton.setViewProjMatrix(m_glCamera.getViewProj());
    // m_prog_skeleton.setModelMatrix(glm::mat4(1.f));//does model matriux need overall transform?


//    if(m_joint->elemCount() > 0) {
//        m_prog_skeleton.draw(*m_joint);
//    }

    // THIS DRAWS THE LOADED OBJ
    // if(m_mesh.elemCount() > 0) {
    //     m_progFlat.draw(m_mesh);
    // }


    //start doing the selected drawing parts
    glDisable(GL_DEPTH_TEST);

    // THESE DRAW THE SELECTED PIECE

    // if (m_vertDisplay.getVertex() != nullptr) {
    //     m_vertDisplay.create();
    //     m_progFlat.draw(m_vertDisplay);
    // }

    // if (m_faceDisplay.getFace() != nullptr) {
    // m_faceDisplay.create();
    // m_progFlat.draw(m_faceDisplay);
    // }
    // if (m_halfedgeDisplay.getHalfEdge() != nullptr) {
    // m_halfedgeDisplay.create();
    // m_progFlat.draw(m_halfedgeDisplay);
    // }

    // if(m_joint && m_joint->children.size() > 0){
    //     helperDraw(m_joint.get());
    // }

    //glEnable(GL_DEPTH_TEST);
    // PARTICLE RENDER TEST:
    // std::vector<QVector4D> particlePositions;
    // float spacing = 0.2f;
    // glm::vec3 dim = glm::vec3(8, 8,  8);
    // glm::vec3 origin = glm::vec3(float(dim.x), float(dim.y), float(dim.z));
    // origin *= spacing * -0.5;
    // for (int i = 0; i < dim.x; ++i)
    // {
    //     for (int j = 0; j < dim.y; ++j)
    //     {
    //         for (int k = 0; k < dim.z; ++k)
    //         {
    //             float x = origin.x + i * spacing;
    //             float y = origin.y + j * spacing;
    //             float z = origin.z + k * spacing;
    //             // Store as a 4D vector with w=1.0f
    //             particlePositions.emplace_back(x, y, z, 1.0f);
    //         }
    //     }
    // }

    //MPM STUFF
    //updateSimulation(); // Update physics

    // for (const auto& p : solver.getParticles()) {
    //     particlePositions.push_back(QVector4D(p.position.x(), p.position.y(), 0.0f, 1.0f));
    // }

    initializeMPM();
    //std::cout << "Size1 = " << particlePositions.size() << std::endl;
    std::vector<glm::vec4> particlePositions;
    for (int i = 0; i<solver.getParticles().size(); i++) {
        particlePositions.emplace_back(solver.getParticles()[i].position, 1.0f);
    }
    particleDrawable->updateParticles(particlePositions); // Update OpenGL buffer

    particleDrawable->create();
    m_progFlat.draw(*particleDrawable);

}

void MyGL::helperDraw(Joint* joint) {
    m_progFlat.draw(*joint);
}

void MyGL::on_loadButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "", tr("OBJ Files (*.obj)"));

    if (!filePath.isEmpty()) {
        m_mesh.LoadObj(filePath.toStdString().c_str());
        m_mesh.create();
        update();
    }
}


void MyGL::initializeMPM() {
    solver = MPMSolver(glm::vec3(5.0, 5.0, 5.0), 1.0, glm::vec3(0.0f, 0.0f, 0.0f), 1.f);  // Reset simulation

    std::vector<QVector4D> particlePositions;
    float spacing = 0.2f;
    glm::vec3 dim = glm::vec3(8, 8,  8);
    glm::vec3 origin = glm::vec3(float(dim.x), float(dim.y), float(dim.z));
    origin *= spacing * -0.5;
    for (int i = 0; i < dim.x; ++i)
    {
        for (int j = 0; j < dim.y; ++j)
        {
            for (int k = 0; k < dim.z; ++k)
            {
                float x = origin.x + i * spacing;
                float y = origin.y + j * spacing;
                float z = origin.z + k * spacing;
                // Store as a 4D vector with w=1.0f
                //particlePositions.emplace_back(x, y, z, 1.0f);

                solver.addParticle(MPMParticle(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));

            }
        }
    }
    //if (!particleDrawable) {
        particleDrawable = new ParticleDrawable(this);
    //}
}

void MyGL::updateSimulation() {
    solver.computeForcesAndIntegrate(); // Step simulation forward
}


// *FIZZY* Idk what this stuff is but I think it was already commented out

//void MyGL::on_loadButton2_clicked(QTreeWidget* j) {

//    QString filename = QFileDialog::getOpenFileName(this, tr("Load Scene File"), QDir::currentPath().append(QString("../..")), tr("*.json"));

//    QFile file(filename);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qWarning("Could not open the JSON file.");
//        return;
//    }

//    QByteArray jsonData = file.readAll();

//    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
//    if (doc.isNull()) {
//        qWarning("Invalid JSON data.");
//        return;
//    }

//    QJsonObject rootJointObject = doc.object()["root"].toObject();


//    QJsonArray pos = rootJointObject["pos"].toArray();
//    glm::vec3 position = glm::vec3(pos[0].toDouble(), pos[1].toDouble(), pos[2].toDouble());
//    std::cout << "ROOT POSITION" <<std::endl;
//    std::cout << pos[0].toDouble() << ", " << pos[1].toDouble() << ", " << pos[2].toDouble() << std::endl;
////    QJsonArray rot = rootJointObject["rot"].toArray();
////    double angle = rot[0].toDouble();
////    double axisX = rot[1].toDouble();
////    double axisY = rot[2].toDouble();
////    double axisZ = rot[3].toDouble();
////    glm::quat rotation = glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(axisX, axisY, axisZ));

//    m_joint = mkU<Joint>();
//    Joint* skeleton = m_joint.get();
//    skeleton->position = position;
////    skeleton->rotation = rotation;

//    //slot_addRootToTreeWidget
//    j->addTopLevelItem((QTreeWidgetItem*) &skeleton);

//    m_joint->parseJson(rootJointObject);
//    skeleton->setText(0, rootJointObject["name"].toString());

////    std::cout << position[0] << std::endl;

//    skeleton->create();

//    file.close();

//}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        m_halfedgeDisplay.updateHalfEdge(m_halfedgeDisplay.getHalfEdge()->getNextHalfEdge());
    } else if (e->key() == Qt::Key_M) {
        m_halfedgeDisplay.updateHalfEdge(m_halfedgeDisplay.getHalfEdge()->getSymHalfEdge());
    } else if (e->key() == Qt::Key_F) {
        m_faceDisplay.updateFace(m_halfedgeDisplay.getHalfEdge()->getIncidentFace());
    } else if (e->key() == Qt::Key_V) {
        m_vertDisplay.updateVertex(m_halfedgeDisplay.getHalfEdge()->getNextVertex());
    } else if (e->key() == Qt::Key_H) {
        m_halfedgeDisplay.updateHalfEdge(m_vertDisplay.getVertex()->getIncidentHalfEdge());
    } else if ((e->modifiers() & Qt::ShiftModifier) && e->key() == Qt::Key_H) {
        m_halfedgeDisplay.updateHalfEdge(m_vertDisplay.getVertex()->getIncidentHalfEdge());
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::slot_setVX(double x) {
    double diff = x - vx;
    vx = x;
    std::cout << diff << std::endl;
    if (m_vertDisplay.getVertex() != nullptr) {
        glm::vec3 poss = m_vertDisplay.getVertex()->getPosition();
        m_vertDisplay.getVertex()->setPosition(glm::vec3(poss.x + diff, poss.y, poss.z));
        m_mesh.create();
        update();
        std::cout << m_vertDisplay.getVertex()->getPosition()[0] << ", " << m_vertDisplay.getVertex()->getPosition()[1] << ", " << m_vertDisplay.getVertex()->getPosition()[2] << std::endl;

    }
}

void MyGL::slot_setVY(double x) {
    double diff = x - vy;
    vy = x;
    if (m_vertDisplay.getVertex() != nullptr) {
        glm::vec3 poss = m_vertDisplay.getVertex()->getPosition();
        m_vertDisplay.getVertex()->setPosition(glm::vec3(poss.x, poss.y + diff, poss.z));
        m_mesh.create();
        update();
    }
}

void MyGL::slot_setVZ(double x) {
    double diff = x - vz;
    vz = x;
    if (m_vertDisplay.getVertex() != nullptr) {
        glm::vec3 poss = m_vertDisplay.getVertex()->getPosition();
        m_vertDisplay.getVertex()->setPosition(glm::vec3(poss.x, poss.y, poss.z + diff));
        m_mesh.create();
        update();
    }
}

void MyGL::slot_setR(double a) {
    if (m_faceDisplay.getFace() != nullptr) {
        m_faceDisplay.getFace()->setColor(m_faceDisplay.getFace()->getColor() + glm::vec3(a, 0, 0));
        m_mesh.create();
        update();
    }
}

void MyGL::slot_setG(double a) {
    if (m_faceDisplay.getFace() != nullptr) {
        m_faceDisplay.getFace()->setColor(m_faceDisplay.getFace()->getColor() + glm::vec3(0, a, 0));
        m_mesh.create();
        update();
    }
}
void MyGL::slot_setB(double a) {
    if (m_faceDisplay.getFace() != nullptr) {
        m_faceDisplay.getFace()->setColor(m_faceDisplay.getFace()->getColor() + glm::vec3(0, 0, a));
        m_mesh.create();
        update();
    }
}

void MyGL::slot_setJX(double a) {
    double diff = a - jx;
    jx = a;
    if (m_joint->getSelectedJoint() != nullptr) {
        m_joint->getSelectedJoint()->setPosition(diff, 0, 0);
        m_joint->create();
        update();
    }
}

void MyGL::slot_setJY(double a) {
    double diff = a - jy;
    jy = a;
    if (m_joint->getSelectedJoint() != nullptr) {
        m_joint->getSelectedJoint()->setPosition(0, diff, 0);
        m_joint->create();
        update();
    }
}

void MyGL::slot_setJZ(double a) {
    double diff = a - jz;
    jz = a;
    if (m_joint->getSelectedJoint() != nullptr) {
        m_joint->getSelectedJoint()->setPosition(0, 0, diff);
        m_joint->create();
        update();
    }
}

void MyGL::setRot(double a, double b, double c) {
//    double diffA = a - rx;
//    rx = a;
//    double diffB = b - ry;
//    ry = b;
//    double diffC = c - rz;
//    rz = c;
    if (m_joint->getSelectedJoint()) {

        m_joint->getSelectedJoint()->setRotation(a, b, c);//diffA, diffB, diffC);
        //m_joint->destroy();
        m_joint->create();

        update();
    }
    std::cout << "rot clicked" << std::endl;
}

void MyGL::slot_setSelectedJoint(QTreeWidgetItem *i) {
    mp_selectedJoint = static_cast<Joint*>(i);
}

void MyGL::skinMesh() {
    for (vertex* vert : m_mesh.vertices) {
        float minDistance = glm::length(vert->getPosition() - m_joint->position);
        vert->jointInfluences[0] = std::make_pair(m_joint.get(), minDistance);

        // Find the closest joint for the current vertex
        for (auto& child: m_joint->children) {
            helperSkin(vert, child.get());
        }

        // Assign weight to the closest joint (simplest case: 100% weight to the closest joint)
        // if (closestJointIndex != -1) {
             // Perform skinning calculation (e.g., transform the vertex position based on joint)
             // transformVertex(vert, joints[closestJointIndex]);
        // }
        vert->jointWeight1 = 1 - (vert->jointInfluences[0].second / (vert->jointInfluences[0].second + vert->jointInfluences[1].second));
        vert->jointWeight2 = 1 - (vert->jointInfluences[1].second / (vert->jointInfluences[0].second + vert->jointInfluences[1].second));

        std::cout << "joint Weight1 " << vert->jointWeight1 << std::endl;
        std::cout << "joint Weight2 " << vert->jointWeight2 << std::endl;

    }
}

void MyGL::helperSkin(vertex* vert, Joint* j) {
    float distance = glm::length(vert->getPosition() - (glm::vec3(j->getOverallTransformation() * glm::vec4(0, 0, 0, 1))));
    if (vert->jointInfluences[0].second > distance) {
        std::pair<Joint*, float> temp = vert->jointInfluences[0];
        vert->jointInfluences[0] = (std::make_pair(j, distance));
        vert->jointInfluences[1] = temp;
    } else if (vert->jointInfluences[1].second > distance) {
        vert->jointInfluences[1] = (std::make_pair(j, distance));
    }

    if (!vert->jointInfluences[1].first) {
        vert->jointInfluences[1] = (std::make_pair(j, distance));
    }

    for (auto& child: j->children) {
        helperSkin(vert, child.get());
    }

    std::cout << "trying print" << std::endl;

    std::cout << vert->jointInfluences[0].first->getName().toStdString() << std::endl;
    std::cout << vert->jointInfluences[1].first->getName().toStdString() << std::endl;


    //vert->jointInfluences[0] = (std::make_pair(m_joint.get(), minDistance));
}
