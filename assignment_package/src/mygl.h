#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mainwindow.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include "scene/mesh.h"
#include "scene/vertexdisplay.h"
#include "scene/halfedgedisplay.h"
#include "scene/facedisplay.h"
#include "scene/joint.h"
//#include <ui_mainwindow.h>


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_prog_skeleton;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    MPMSolver solver;
    ParticleDrawable* particleDrawable;


public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeMPM(); // Function to start MPM simulation
    void updateSimulation(); // Called each frame

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    mesh m_mesh;
    void on_loadButton_clicked();
    //void on_loadJSON_clicked();
    //void on_loadButton2_clicked(QTreeWidget* j);
    bool read;
    void populateLists(int a, int b, int c);

    //void parseJson(const QJsonObject& json, Joint* s, QTreeWidgetItem* q);

    uPtr<Joint> m_joint;
    Joint *mp_selectedJoint;

        // Have an instance of VertexDisplay that is
        // drawn in paintGL, and has VBO data representing
        // the position of the currently selected Vertex so
        // it can be drawn as a GL_POINTS
    vertexdisplay m_vertDisplay;
    halfedgedisplay m_halfedgeDisplay;
    facedisplay m_faceDisplay;

    double vx;
    double vy;
    double vz;

    double jx;
    double jy;
    double jz;

    double rx;
    double ry;
    double rz;

    void setRot(double a, double b, double c);

public slots:
    void slot_setVX(double x);
    void slot_setVY(double x);
    void slot_setVZ(double x);

    void slot_setR(double a);
    void slot_setG(double a);
    void slot_setB(double a);

    void slot_setJX(double x);
    void slot_setJY(double x);
    void slot_setJZ(double x);



    void slot_setSelectedJoint(QTreeWidgetItem*);

    void helperDraw(Joint* joint);

    void skinMesh();

    void helperSkin(vertex* vert, Joint* j);

protected:
    void keyPressEvent(QKeyEvent *e);

};


#endif // MYGL_H
