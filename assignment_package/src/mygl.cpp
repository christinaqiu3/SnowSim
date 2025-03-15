#include "mygl.h"
#include <glm_includes.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QDateTime>

#define CHUNKRANGE 3 //Also change in terrain.cpp. At least 2

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent), text(this), norMapText(this), m_worldAxes(this), m_Shadow(this), m_textureTest(this),
    m_progLambert(this), m_progFlat(this), m_progInstanced(this), m_buffer(this, this->width(), this->height(), this->devicePixelRatio()),
    m_bloom(this),
    m_shadowBuffer(this, this->width(), this->height(), this->devicePixelRatio()),
    m_terrain(this),
    m_player(glm::vec3(48.f, 170.f, 48.f), m_terrain), prev_time(QDateTime::currentMSecsSinceEpoch()),
    myQuad(this), m_postprocessShaders(),
    mp_progPostprocessCurrent(nullptr), mp_progPostprocessNoOp(nullptr), mp_progPostprocessWater(nullptr),
    mp_progPostprocessLava(nullptr),
    mp_progSky(this), /*mp_geomQuad(new Quad(this)),*/
    m_frameBuffer(-1), m_bloomBuffer(this, this->width(), this->height(), this->devicePixelRatio()),

    m_renderedTexture(-1),
    m_depthRenderBuffer(-1), m_Time(0), m_portal()

{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    m_timer.start(16);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible
}

MyGL::~MyGL() {
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
}


void MyGL::moveMouseToCenter() {
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
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
    glDepthFunc(GL_LEQUAL);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of the world axes
    m_worldAxes.createVBOdata();

    //Read in the texture
    text.create(":/textures/minecraft_textures_all_New.png");
    text.load(0);

    norMapText.create(":/textures/minecraft_normals_all.png");
    norMapText.load(4);

    m_buffer.create();
    m_bloomBuffer.create();
    m_shadowBuffer.createShadowMap();

    // Create and set up the shadow shader
    m_Shadow.create(":/glsl/shadow.vert", ":/glsl/shadow.frag");

    m_textureTest.create(":/glsl/textureTest.vert", ":/glsl/textureTest.frag");

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    m_bloom.create(":/glsl/bloom.vert", ":/glsl/bloom.frag");
    m_progInstanced.create(":/glsl/instanced.vert.glsl", ":/glsl/lambert.frag.glsl");

    //std::shared_ptr<ShaderProgram> mp_progSky = std::make_shared<ShaderProgram>(this);
    mp_progSky.create(":/glsl/sky.vert.glsl", ":/glsl/sky.frag.glsl");// for making sky
    //mp_geomQuad->createVBOdata();

    createShaders();
    //createRenderBuffers();
    myQuad.createVBOdata();

    // Set a color with which to draw geometry.
    // This will ultimately not be used when you change
    // your program to render Chunks with vertex colors
    // and UV coordinates
    m_progLambert.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    //m_terrain.CreateTestScene();

    m_terrain.TEST();
    m_terrain.createFirstTGZ();
}

void MyGL::resizeGL(int w, int h) {
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_player.setCameraWidthHeight(static_cast<unsigned int>(w), static_cast<unsigned int>(h));
    glm::mat4 viewproj = m_player.mcr_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_Shadow.setViewProjMatrix(viewproj);
    m_bloom.setViewProjMatrix(viewproj);

    // is inverse bc the sky is a background that remains fixed relative to the camera while the rest of the scene (terrain, objects, etc.) is transformed by the view-projection matrix.
    // Inverting the view-projection matrix allows the sky to look static as a distant background that moves with the camera's orientation.
    mp_progSky.setViewProjMatrix(glm::inverse(viewproj)); //

    //activate the sky shader program, set its dimensions uniform to the current width and height, and provide the camera's eye position as a uniform vector for rendering the sky background.
    mp_progSky.useMe();
    mp_progSky.setPlayerPos(m_player.getPosition());
    mp_progSky.setTime(m_Time);
    mp_progSky.setDimensions(glm::ivec2(width(), height()));

    mp_progPostprocessNoOp->setDimensions(glm::ivec2(w * this->devicePixelRatio(), h * this->devicePixelRatio()));
    mp_progPostprocessWater->setDimensions(glm::ivec2(w * this->devicePixelRatio(), h * this->devicePixelRatio()));
    mp_progPostprocessLava->setDimensions(glm::ivec2(w * this->devicePixelRatio(), h * this->devicePixelRatio()));

    m_buffer.resize(this->width(), this->height(), this->devicePixelRatio());
    m_buffer.create();

    m_bloomBuffer.resize(this->width(), this->height(), this->devicePixelRatio());
    m_bloomBuffer.create();

    m_shadowBuffer.resize(this->width(), this->height(), this->devicePixelRatio());
    m_shadowBuffer.createShadowMap();

    printGLErrorLog();
}


// MyGL's constructor links tick() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to perform
// all per-frame actions here, such as performing physics updates on all
// entities in the scene.
void MyGL::tick() {
    glm::vec3 oldPos = m_player.mcr_position;

    uint64_t currTime = QDateTime::currentMSecsSinceEpoch();
    float dT = (currTime - prev_time) * 0.001;
    prev_time = currTime;
    m_player.tick(dT, m_inputs);

    if (WATER == m_terrain.getBlockAt(m_player.mcr_camera.mcr_position)) {
        slot_setCurrentPostprocessShaderProgram(1);
    } else if (LAVA == m_terrain.getBlockAt(m_player.mcr_camera.mcr_position)) {
        slot_setCurrentPostprocessShaderProgram(2);
    } else {
        slot_setCurrentPostprocessShaderProgram(0);
    }

    m_terrain.tickCreateChunk(m_player.mcr_position[0], m_player.mcr_position[2], oldPos.x, oldPos.z);

    for (glm::vec3 &pos : m_portal) {
        if (m_player.mcr_position[0] - floor(pos.x) < 0.1f && m_player.mcr_position[2] - floor(pos.y) < 0.1f) {
            m_portal.clear();
            m_terrain.clearThreads();
            m_terrain.isAether = !m_terrain.isAether;
            m_terrain.redoFirstTGZ(m_player.getPosition().x, m_player.getPosition().z);
            break;
        }
    }
    update(); // Calls paintGL() as part of a larger QOpenGLWidget pipeline
    sendPlayerDataToGUI(); // Updates the info in the secondary window displaying player data
}

void MyGL::sendPlayerDataToGUI() const {
    emit sig_sendPlayerPos(m_player.posAsQString());
    emit sig_sendPlayerVel(m_player.velAsQString());
    emit sig_sendPlayerAcc(m_player.accAsQString());
    emit sig_sendPlayerLook(m_player.lookAsQString());
    glm::vec2 pPos(m_player.mcr_position.x, m_player.mcr_position.z);
    glm::ivec2 chunk(16 * glm::ivec2(glm::floor(pPos / 16.f)));
    glm::ivec2 zone(64 * glm::ivec2(glm::floor(pPos / 64.f)));
    emit sig_sendPlayerChunk(QString::fromStdString("( " + std::to_string(chunk.x) + ", " + std::to_string(chunk.y) + " )"));
    emit sig_sendPlayerTerrainZone(QString::fromStdString("( " + std::to_string(zone.x) + ", " + std::to_string(zone.y) + " )"));
}

// Function to rotate a vector around the Y-axis
glm::mat3 rotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return glm::mat3(
        1, 0, 0,
        0, c, -s,
        0, s, c
        );
}

// Function to calculate the sun direction based on the time of day
glm::vec3 MyGL::calculateSunDirection() {
    float timeOfDay = glm::mod(float(m_Time)/3.0, 360.0) * 2.f;


    // Calculate the angle of rotation based on the time of day
    float rotationAngleY = glm::radians(timeOfDay);

    // Rotate the initial sun direction vector around the Y-axis
    glm::vec3 initialSunDir = glm::vec3(0, 0.1, 1.0);
    //glm::vec3 rotatedSunDirY = rotateY(rotationAngleY) * initialSunDir;
    glm::vec4 rotatedSunDirY = glm::rotate(glm::mat4(), rotationAngleY, glm::vec3(-1,0,0)) * glm::vec4(initialSunDir, 1);

    return glm::normalize(glm::vec3(rotatedSunDirY[0], rotatedSunDirY[1], rotatedSunDirY[2]));
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL() {
#if 1

    m_shadowBuffer.bindFrameBuffer();
    glViewport(0,0,this->width()*this->devicePixelRatio(), this->height()*this->devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //glm::vec3 lightDir = glm::normalize(glm::vec3(0.5, 1, 0.75)) * glm::vec3(100);  // The direction of our virtual light
    //glm::vec3 lightDir = glm::normalize(glm::vec3(0.1, 0.5, 0.2)) * glm::vec3(255);

    glm::vec3 lightDir = calculateSunDirection() * glm::vec3(100);

    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-220, 220, -200, 200, -50, 400);

    //THIS IS BASED ON THE STARTING POSITION OF THE PLAYER ! ! !
    //IF YOU CHANGE THAT THIS WILL BREAK
    glm::vec3 lightCamOffset = glm::vec3(m_player.mcr_position.x, 0, m_player.mcr_position.z) - glm::vec3(48.f, 0, 48.f);

    glm::vec3 llookPos = lightCamOffset + glm::vec3(32,129,32); //CENTER OF DRAWN WORLD AXIS
    //glm::vec3(m_player.mcr_position.x, 0, m_player.mcr_position.z);
    glm::vec3 lightCamPos = llookPos + lightDir;
    if (lightCamPos.y > 255) {
        lightCamPos.y = 255;
    }
    glm::mat4 depthViewMatrix = glm::lookAt(lightCamPos, llookPos, glm::vec3(0,1,0));
    //glm::mat4 depthViewMatrix = glm::lookAt(lightDir, glm::vec3(0), glm::vec3(0,1,0));
    depthViewMatrix[3][0] -= glm::mod(depthViewMatrix[3][0], 440.f / (float) (this->width()*this->devicePixelRatio()));
    depthViewMatrix[3][1] -= glm::mod(depthViewMatrix[3][1], 400.f / (float) (this->height()*this->devicePixelRatio()));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);

    m_Shadow.setShadowMVP(depthProjectionMatrix * depthViewMatrix * depthModelMatrix);

    //Draws the terrain from the lights point of view
    int x = floor(m_player.mcr_position.x) - ((int)floor(m_player.mcr_position.x)%64 + 64) % 64;
    int z = floor(m_player.mcr_position.z) - ((int)floor(m_player.mcr_position.z)%64 + 64) % 64;
    int range = CHUNKRANGE - 1;

    m_terrain.draw(x-64*range, x+64*range, z-64*range, z+64*range, &m_Shadow, false);

    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    //glViewport(0,0,this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
    glViewport(0,0, this->height() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shadowBuffer.bindToTextureSlot(2);

    m_progLambert.setShadowText(2);
#endif

#if 1
    //THIS IS THE STUFF THAT RENDERS THE TERRAIN FROM THE CAMERA POV

    m_buffer.bindFrameBuffer();
    glViewport(0,0,this->width()*this->devicePixelRatio(), this->height()*this->devicePixelRatio());

    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_progFlat.setViewProjMatrix(m_player.mcr_camera.getViewProj());
    m_bloom.setViewProjMatrix(m_player.mcr_camera.getViewProj());
    m_progInstanced.setViewProjMatrix(m_player.mcr_camera.getViewProj());
    m_progInstanced.setLight(calculateSunDirection());

    // RENDER FROM CAM PERSPECTIVE
    m_progLambert.setViewProjMatrix(m_player.mcr_camera.getViewProj());
    // RENDER FROM LIGHT PERSPECTIVE
    //m_progLambert.setViewProjMatrix(depthProjectionMatrix * depthViewMatrix * depthModelMatrix);

    m_progLambert.setModelMatrix(glm::mat4());
    m_progLambert.setShadowMVP(depthProjectionMatrix * depthViewMatrix);
    m_Shadow.setViewProjMatrix(m_player.mcr_camera.getViewProj());
    m_progLambert.setPlayerPos(m_player.getPosition());

    m_progLambert.setTime(m_Time);
    m_Time++;
    m_progLambert.setLight(calculateSunDirection());

    //mp_progPostprocessNoOp->setPlayerPos(m_player.getPosition());


    //sky: sets inverse so the sky looks static. Activates the sky shader program.
    //Provides the camera's eye position as a uniform vector for rendering the sky background.
    //Increments the time uniform variable in the sky shader for time-dependent effects.
    //Draws the quad geometry using the sky shader.
    mp_progSky.setViewProjMatrix(glm::inverse(m_player.mcr_camera.getViewProj()));
    mp_progSky.useMe();
    mp_progSky.setPlayerPos(m_player.getPosition());
    mp_progSky.setTime(m_Time);
    mp_progSky.setLight(calculateSunDirection());


    //DIFFUSE TEXTURE
    text.bind(0);
    //NORMAL MAP TEXTURE
    norMapText.bind(4);


    //DRAWS THE SKY QUAD
    mp_progSky.draw(myQuad);

    //SETS NORMAL MAP FOR LAMBERT SHADER
    m_progLambert.setNorText(4);

    //m_progPostNoOp.useMe();
    //this->glUniform1f(m_progPostOp.unifTime);

    //CHANGE THIS TO DRAW WITH m_Shadow TO DRAW GEOMETRY WITH DEPTH COLORING
    renderTerrain();

    glDisable(GL_DEPTH_TEST);

    // 1. Make a shader that just applies a texture (could just use post process shader you already have for water overlay)
    // 2. Bind your shadow map texture to some texture slot, e.g. 2
    // 3. Set your shader's texture handle using glUniform1i to the same # as the slot you bound to in step 2
    // 4. Draw your quad using the shader in order to visualize the shadow map

    //DRAWS CENTER QUAD WITH SHADOW MAP
    //CHANGE flat.frag.glsl TO HAVE THIS DRAW THE SHADOW MAP
//    glm::mat4 quadScale = glm::scale(glm::mat4(), glm::vec3(this->width() * 0.05,this->height() * 0.05,1));
//    m_progFlat.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(m_player.mcr_camera.mcr_position) + glm::vec3(0,0,-200)) * quadScale);
//    m_progFlat.setShadowText(2);
//    m_progFlat.draw(myQuad);


    //DRAWS WORLD AXIS
//    m_progFlat.setModelMatrix(glm::mat4());
//    m_progFlat.setViewProjMatrix(m_player.mcr_camera.getViewProj());
//    m_progLambert.setPlayerPos(m_player.getPosition());
//    m_progFlat.draw(m_worldAxes);
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    glViewport(0,0,this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_buffer.bindToTextureSlot(0);


//    m_bloomBuffer.bindFrameBuffer();
//    glViewport(0,0,this->width()*this->devicePixelRatio(), this->height()*this->devicePixelRatio());
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    m_bloom.setShadowText(0);
//    m_bloom.setPlayerPos(m_player.mcr_position);
//    x = floor(m_player.mcr_position.x) - ((int)floor(m_player.mcr_position.x)%64 + 64) % 64;
//    z = floor(m_player.mcr_position.z) - ((int)floor(m_player.mcr_position.z)%64 + 64) % 64;
//    range = CHUNKRANGE - 1;
//    text.bind(0);
//    m_terrain.draw(x-64*range, x+64*range, z-64*range, z+64*range, &m_progLambert, true);


//    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
//    glViewport(0,0,this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    m_bloomBuffer.bindToTextureSlot(5);

//    mp_progPostprocessCurrent->setShadowText(5);
    mp_progPostprocessCurrent->drawPost(myQuad, m_buffer.getTextureSlot());
    //m_buffer.bindToTextureSlot(1);

    //glUniform1i() need sampler2d
    //texture(u_Texture, fs_UV)
#endif
}

//void MyGL::render3DScene()
//{
//    // Render the 3D scene to our frame buffer
//    // Render to our framebuffer rather than the viewport
//    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
//    // Render on the whole framebuffer, complete from the lower left corner to the upper right
//    glViewport(0,0,this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
//    // Clear the screen so that we only see newly drawn images
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//}

//void MyGL::performPostprocessRenderPass()
//{
//    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
//    // Render on the whole framebuffer, complete from the lower left corner to the upper right
//    glViewport(0,0,this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
//    // Clear the screen
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    // Bind our texture in Texture Unit 0
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);

//    //m_buffer.bindToTextureSlot(1);

//    printGLErrorLog();

//    mp_progPostprocessCurrent->draw(myQuad);
//}

//void MyGL::createRenderBuffers()
//{
//    // Initialize the frame buffers and render textures
//    glGenFramebuffers(1, &m_frameBuffer);
//    glGenTextures(1, &m_renderedTexture);
//    glGenRenderbuffers(1, &m_depthRenderBuffer);

//    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
//    // Bind our texture so that all functions that deal with textures will interact with this one
//    glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
//    // Give an empty image to OpenGL ( the last "0" )
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio(), 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)0);

//    // Set the render settings for the texture we've just created.
//    // Essentially zero filtering on the "texture" so it appears exactly as rendered
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    // Clamp the colors at the edge of our texture
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    // Initialize our depth buffer
//    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

//    // Set m_renderedTexture as the color output of our frame buffer
//    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);

//    // Sets the color output of the fragment shader to be stored in GL_COLOR_ATTACHMENT0, which we previously set to m_renderedTextures[i]
//    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//    glDrawBuffers(1, drawBuffers); // "1" is the size of drawBuffers

//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//    {
//        std::cout << "Frame buffer did not initialize correctly..." << std::endl;
//        printGLErrorLog();
//    }
//}

void MyGL::createShaders()
{
    std::shared_ptr<ShaderProgram> noOp = std::make_shared<ShaderProgram>(this);
    noOp->create(":/glsl/passthrough.vert.glsl", ":/glsl/noOp.frag.glsl");
    m_postprocessShaders.push_back(noOp);
    mp_progPostprocessNoOp = noOp.get();

    // Post-process shaders
    std::shared_ptr<ShaderProgram> water = std::make_shared<ShaderProgram>(this);
    water->create(":/glsl/passthrough.vert.glsl", ":/glsl/water.frag.glsl");
    m_postprocessShaders.push_back(water);
    mp_progPostprocessWater = water.get();

    std::shared_ptr<ShaderProgram> lava = std::make_shared<ShaderProgram>(this);
    lava->create(":/glsl/passthrough.vert.glsl", ":/glsl/lava.frag.glsl");
    m_postprocessShaders.push_back(lava);
    mp_progPostprocessLava = lava.get();

    slot_setCurrentPostprocessShaderProgram(0);
}

// TODO: Change this so it renders the nine zones of generated
// terrain that surround the player (refer to Terrain::m_generatedTerrain
// for more info)
void MyGL::renderTerrain() {
#if 0
    glm::vec3 pos = m_player.mcr_position;
    //DRAWS OPQUQEUEU

    for (float x = pos.x - 16; x <= pos.x + 16; x+= 16) {
        for (float z = pos.z - 16; z <= pos.z + 16; z += 16) {
            if (m_terrain.hasChunkAt(x, z)) {
                m_terrain.getChunkAt(x, z).get()->createVBOdata();
                //CHANGE THIS SO THAT IT DRAWS BASED ON OPAQUE VBOs OR TRANS VBOs
                m_progLambert.drawChunk(*m_terrain.getChunkAt(x,z).get(), true);
            }
        }
    }
    //DRAWS TRANSPARENT
    for (float x = pos.x - 16; x <= pos.x + 16; x+= 16) {
        for (float z = pos.z - 16; z <= pos.z + 16; z += 16) {
            if (m_terrain.hasChunkAt(x, z)) {
                //m_terrain.getChunkAt(x, z).get()->createVBOdata();
                m_progLambert.drawChunk(*m_terrain.getChunkAt(x,z).get(), false);
            }
        }
    }

#endif
    int x = floor(m_player.mcr_position.x) - ((int)floor(m_player.mcr_position.x)%64 + 64) % 64;
    int z = floor(m_player.mcr_position.z) - ((int)floor(m_player.mcr_position.z)%64 + 64) % 64;
    int range = CHUNKRANGE - 1;
    m_terrain.draw(x-64*range, x+64*range, z-64*range, z+64*range, &m_progLambert, true);
    //DRAWS TERRAIN WITH Z DEPTH COLORING
    //m_terrain.draw(x-64*range, x+64*range, z-64*range, z+64*range, &m_Shadow, true);
}


void MyGL::keyPressEvent(QKeyEvent *e) {

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_W) {
        m_inputs.wPressed = true;
    } else if (e->key() == Qt::Key_S) {
        m_inputs.sPressed = true;
    } else if (e->key() == Qt::Key_D) {
        m_inputs.dPressed = true;
    } else if (e->key() == Qt::Key_A) {
        m_inputs.aPressed = true;
    } else if (e->key() == Qt::Key_Q) {
        m_inputs.qPressed = true;
    } else if (e->key() == Qt::Key_E) {
        m_inputs.ePressed = true;
    } else if (e->key() == Qt::Key_Space) {
        m_inputs.spacePressed = true;
    } else if (e->key() == Qt::Key_F) {
        m_player.toggleFlightMode();
    } else if (e->key() == Qt::Key_Z) {
        m_terrain.isAether = !m_terrain.isAether;
        m_terrain.redoFirstTGZ(m_player.getPosition().x, m_player.getPosition().z);
    }
    //toggle flight mode
    //add key release to set space pressed as false when released
    //if want continuous jumo then have space only affect when feet r on ground

}

void MyGL::keyReleaseEvent(QKeyEvent *e) {
    //InputBundle inputs;

    // Reset acceleration when keys are released
    if (e->key() == Qt::Key_W) {
        m_inputs.wPressed = false;
    } else if (e->key() == Qt::Key_S) {
        m_inputs.sPressed = false;
    } else if (e->key() == Qt::Key_A) {
        m_inputs.aPressed = false;
    } else if (e->key() == Qt::Key_D) {
        m_inputs.dPressed = false;
    } else if (e->key() == Qt::Key_Space) {
        m_inputs.spacePressed = false;
    } else if (e->key() == Qt::Key_Q) {
        m_inputs.qPressed = false;
    } else if (e->key() == Qt::Key_E) {
        m_inputs.ePressed = false;
    }

    // Pass inputs to Player's processInputs function
    //m_player.processInputs(inputs);
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    //moveMouseToCenter();

    float sensitivity = 0.1f;

//    float deltaX = (e->x() - m_inputs.mouseX) * width()/360;
//    float deltaY = (e->y() - m_inputs.mouseY) * height()/360;

    float deltaX = e->x() -  width()/2;
    float deltaY = e->y() - height()/2;

    //deltaY = glm::clamp(deltaY, -89.0f, 89.0f);

    //deltaX = std::fmod(deltaX, 360.0f);

    // Store current mouse position for the next frame
    //m_inputs.mouseX = e->x();
    //m_inputs.mouseY = e->y();

    // Apply rotation for each axis separately
    m_player.rotateOnUpGlobal(-deltaX * sensitivity); // Rotate about up axis (Y-axis)
    m_player.rotateOnRightLocal(-deltaY * sensitivity); // Rotate about right axis (X-axis)

    moveMouseToCenter();
}

void MyGL::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::RightButton) {
        addBlock();
    }
    if(e->button() == Qt::LeftButton) {
        removeBlock();
    }
}

void MyGL::removeBlock() {
    float dist = 5.f;
    glm::vec3 rayOrigin = m_player.getPosition() + glm::vec3(0.f, 1.5f, 0.f); //account for height
    glm::vec3 rayDirection = m_player.getDir() * dist;
    glm::ivec3 blockHit;
    float out_iAxis;
    glm::vec3 out_iPos;
    BlockType BT;
    if (m_player.gridMarch(rayOrigin, rayDirection, m_terrain, &dist, &blockHit, &out_iAxis, &out_iPos, &BT) && BT != BEDROCK) {
        std::cout << "removing block" << std::endl;
        m_terrain.removeBlockAt(blockHit.x, blockHit.y, blockHit.z);
    }
}

static bool portal[4][5] = {
    {false, true, true, true, false},
    {true, false, false, false, true},
    {true, false, false, false, true},
    {false, true, true, true, false}
};

void MyGL::addBlock() {
    float dist = 5.f;
    glm::vec3 rayOrigin = m_player.getPosition() + glm::vec3(0.f, 1.5f, 0.f);
    glm::vec3 rayDirection = m_player.getDir() * dist;
    glm::ivec3 blockHit;
    float out_iAxis;
    glm::vec3 out_iPos;
    BlockType BT;
    if (m_player.gridMarch(rayOrigin, rayDirection, m_terrain, &dist, &blockHit, &out_iAxis, &out_iPos, &BT)) {
        glm::vec3 result = glm::vec3();
        if (out_iAxis == 0) {
            if (out_iPos.x > 0.5) {
                result = glm::vec3(blockHit.x - 1.f, blockHit.y, blockHit.z);
            } else {
                result = glm::vec3(blockHit.x + 1.f, blockHit.y, blockHit.z);
            }
        } else if (out_iAxis == 1) {
            if (out_iPos.y > 0.5) {
                result = glm::vec3(blockHit.x, blockHit.y - 1.f, blockHit.z);
            } else {
                result = glm::vec3(blockHit.x, blockHit.y + 1.f, blockHit.z);
            }
        } else if (out_iAxis == 2) {
            if (out_iPos.z > 0.5) {
                result = glm::vec3(blockHit.x, blockHit.y, blockHit.z - 1.f);
            } else {
                result = glm::vec3(blockHit.x, blockHit.y, blockHit.z + 1.f);
            }
        }
        propagateLight(result, 15);
        m_terrain.setBlockAt(result.x, result.y, result.z, GLOW);

        bool flag = false;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 5; j++) {
                bool subflagX = true;
                bool subflagZ = true;
                for (int k = 0; k < 4; k++) {
                    for (int l = 0; l < 5; l++) {
                        if (portal[k][l] == true && m_terrain.getBlockAt(result.x - i + k, result.y - j + l, result.z) != GLOW) {
                            subflagX = false;
                        }
                        if (portal[k][l] == true && m_terrain.getBlockAt(result.x, result.y - j + l, result.z - i + k) != GLOW) {
                            subflagZ = false;
                        }
                    }
                }
                if (subflagX) {
                    flag = true;
                    m_terrain.setBlockAt(result.x- i + 1, result.y -j + 1, result.z, WATER);
                    m_terrain.setBlockAt(result.x- i + 1, result.y -j + 2, result.z, WATER);
                    m_terrain.setBlockAt(result.x- i + 1, result.y -j + 3, result.z, WATER);
                    m_terrain.setBlockAt(result.x- i + 2, result.y -j + 1, result.z, WATER);
                    m_terrain.setBlockAt(result.x- i + 2, result.y -j + 2, result.z, WATER);
                    m_terrain.setBlockAt(result.x- i + 2, result.y -j + 3, result.z, WATER);
                    m_portal.push_back(glm::vec3(result.x- i + 1, result.y -j + 1, result.z));
                    m_portal.push_back(glm::vec3(result.x- i + 1, result.y -j + 2, result.z));
                    m_portal.push_back(glm::vec3(result.x- i + 1, result.y -j + 3, result.z));
                    m_portal.push_back(glm::vec3(result.x- i + 2, result.y -j + 1, result.z));
                    m_portal.push_back(glm::vec3(result.x- i + 2, result.y -j + 2, result.z));
                    m_portal.push_back(glm::vec3(result.x- i + 2, result.y -j + 3, result.z));
                    break;
                }
                if (subflagZ) {
                    flag = true;
                    m_terrain.setBlockAt(result.x, result.y -j + 1, result.z - i + 1, WATER);
                    m_terrain.setBlockAt(result.x, result.y -j + 2, result.z - i + 1, WATER);
                    m_terrain.setBlockAt(result.x, result.y -j + 3, result.z - i + 1, WATER);
                    m_terrain.setBlockAt(result.x, result.y -j + 1, result.z - i + 2, WATER);
                    m_terrain.setBlockAt(result.x, result.y -j + 2, result.z - i + 2, WATER);
                    m_terrain.setBlockAt(result.x, result.y -j + 3, result.z - i + 2, WATER);
                    m_portal.push_back(glm::vec3(result.x, result.y -j + 1, result.z - i + 1));
                    m_portal.push_back(glm::vec3(result.x, result.y -j + 2, result.z - i + 1));
                    m_portal.push_back(glm::vec3(result.x, result.y -j + 3, result.z - i + 1));
                    m_portal.push_back(glm::vec3(result.x, result.y -j + 1, result.z - i + 2));
                    m_portal.push_back(glm::vec3(result.x, result.y -j + 2, result.z - i + 2));
                    m_portal.push_back(glm::vec3(result.x, result.y -j + 3, result.z - i + 2));
                    break;
                }
            }
        }
        if (flag) {
            //m_terrain.isAether = !m_terrain.isAether;
            //m_terrain.redoFirstTGZ(m_player.getPosition().x, m_player.getPosition().z);
        }

        //std::cout << "adding block" << std::endl;
        //m_terrain.setBlockAt(blockHit.x, blockHit.y, blockHit.z, WATER);
    }
}

const static std::vector<BlockType> flowers = {TALL_GRASS, FLOWERR, FLOWERY, FLOWERW, FLOWERB, FLOWERP,
                                               RED_MUSH, BROWN_MUSH};

static bool isFlower(BlockType bt) {
    return std::find(flowers.begin(), flowers.end(), bt) != flowers.end();
}

void MyGL::propagateLight(glm::vec3 result, int level)
{
    if (level == 0) {
        return;
    }
    if (result.y > 255 || result.y < 0) {
        return;
    }
    if ((m_terrain.getBlockAt(result) == EMPTY || m_terrain.getBlockAt(result) == WATER || isFlower(m_terrain.getBlockAt(result)))
        && (m_terrain.getLightLevelAt(result.x, result.y, result.z) < level)) {
    m_terrain.setLightLevelAt(result.x, result.y, result.z, level); // Watch for out of range here

        propagateLight(result + glm::vec3(1.f, 0.f, 0.f), level - 1);
        propagateLight(result + glm::vec3(-1.f, 0.f, 0.f), level - 1);
        propagateLight(result + glm::vec3(0.f, 1.f, 0.f), level - 1);
        propagateLight(result + glm::vec3(0.f, -1.f, 0.f), level - 1);
        propagateLight(result + glm::vec3(0.f, 0.f, 1.f), level - 1);
        propagateLight(result + glm::vec3(0.f, 0.f, -1.f), level - 1);

    }
}

void MyGL::slot_setCurrentPostprocessShaderProgram(int s)
{
    if (s == 0) {
        mp_progPostprocessCurrent = mp_progPostprocessNoOp;
    } else if (s==1) {
        mp_progPostprocessCurrent = mp_progPostprocessWater;
    } else if (s==2) {
        mp_progPostprocessCurrent = mp_progPostprocessLava;
    }

}
