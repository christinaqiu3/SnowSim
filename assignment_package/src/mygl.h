#ifndef MYGL_H
#define MYGL_H

#include "framebuffer.h"
#include "openglcontext.h"
#include "shaderprogram.h"
#include "scene/worldaxes.h"
#include "scene/camera.h"
#include "scene/terrain.h"
#include "scene/player.h"
#include "quad.h"
#include "texture.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <smartpointerhelp.h>


class MyGL : public OpenGLContext
{
    Q_OBJECT
private:
    Texture text;
    Texture norMapText;
    WorldAxes m_worldAxes; // A wireframe representation of the world axes. It is hard-coded to sit centered at (32, 128, 32).

    ShaderProgram m_Shadow;
    ShaderProgram m_textureTest;

    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progInstanced;// A shader program that is designed to be compatible with instanced rendering
    ShaderProgram m_bloom;
    FrameBuffer m_buffer;

    FrameBuffer m_bloomBuffer;

    FrameBuffer m_shadowBuffer;



    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Terrain m_terrain; // All of the Chunks that currently comprise the world.
    Player m_player; // The entity controlled by the user. Contains a camera to display what it sees as well.
    InputBundle m_inputs; // A collection of variables to be updated in keyPressEvent, mouseMoveEvent, mousePressEvent, etc.

    QTimer m_timer; // Timer linked to tick(). Fires approximately 60 times per second.
    uint64_t prev_time;

    void moveMouseToCenter(); // Forces the mouse position to the screen's center. You should call this
                              // from within a mouse move event after reading the mouse movement so that
                              // your mouse stays within the screen bounds and is always read.

    void sendPlayerDataToGUI() const;

    int m_Time; //mmmmmmm time

    Quad myQuad;

    // The collection of post-process shaders available to the user. This vector is only
    // ever modified once, in createShaders().
    //JUST MAKE 3 PPS of noop, water, lava
    std::vector<std::shared_ptr<ShaderProgram>> m_postprocessShaders;
    ShaderProgram* mp_progPostprocessCurrent;
    ShaderProgram* mp_progPostprocessNoOp;
    ShaderProgram* mp_progPostprocessWater;
    ShaderProgram* mp_progPostprocessLava;

    ShaderProgram mp_progSky; // A screen-space shader for creating the sky background
    //Quad* mp_geomQuad; //quad to render the sky

    // A collection of handles to the five frame buffers we've given
    // ourselves to perform render passes. The 0th frame buffer is always
    // written to by the render pass that uses the currently bound surface shader.
    GLuint m_frameBuffer;
    // A collection of handles to the textures used by the frame buffers.
    // m_frameBuffers[i] writes to m_renderedTextures[i].
    GLuint m_renderedTexture;
    // A collection of handles to the depth buffers used by our frame buffers.
    // m_frameBuffers[i] writes to m_depthRenderBuffers[i].
    GLuint m_depthRenderBuffer;
    std::vector<glm::vec3> m_portal;

    // Sets up the arrays of frame buffers
    // used to store render passes. Invoked
    // once in initializeGL().
    void createRenderBuffers();
    // Sets up the different shaders used to
    // render the scene. Invoked once in
    // initializeGL().
    void createShaders();

    void render3DScene();

    // A helper function that iterates through
    // each of the render passes required by the
    // currently bound post-process shader and
    // invokes them.
    void performPostprocessRenderPass();

    glm::vec3 calculateSunDirection();


public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    // Called once when MyGL is initialized.
    // Once this is called, all OpenGL function
    // invocations are valid (before this, they
    // will cause segfaults)
    void initializeGL() override;
    // Called whenever MyGL is resized.
    void resizeGL(int w, int h) override;
    // Called whenever MyGL::update() is called.
    // In the base code, update() is called from tick().
    void paintGL() override;
    void propagateLight(glm::vec3 result, int level);
    // Called from paintGL().
    // Calls Terrain::draw().
    void renderTerrain();

protected:
    // Automatically invoked when the user
    // presses a key on the keyboard
    void keyPressEvent(QKeyEvent *e);
    // Automatically invoked when the user
    // moves the mouse
    void mouseMoveEvent(QMouseEvent *e);
    // Automatically invoked when the user
    // presses a mouse button
    void mousePressEvent(QMouseEvent *e);

    void keyReleaseEvent(QKeyEvent *e);

    void removeBlock();
    void addBlock();

private slots:
    void tick(); // Slot that gets called ~60 times per second by m_timer firing.

public slots:
    void slot_setCurrentPostprocessShaderProgram(int);

signals:
    void sig_sendPlayerPos(QString) const;
    void sig_sendPlayerVel(QString) const;
    void sig_sendPlayerAcc(QString) const;
    void sig_sendPlayerLook(QString) const;
    void sig_sendPlayerChunk(QString) const;
    void sig_sendPlayerTerrainZone(QString) const;
};


#endif // MYGL_H
