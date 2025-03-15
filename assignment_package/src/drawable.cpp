#include "drawable.h"
#include <glm_includes.h>

Drawable::Drawable(OpenGLContext* context)
    : m_count(-1), m_countTrans(-2), m_bufUV(), m_bufChunk(), m_bufIdx(), m_bufPos(), m_bufNor(), m_bufCol(),
    m_uvGenerated(false), m_chunkGenerated(false), m_idxGenerated(false), m_posGenerated(false), m_norGenerated(false), m_colGenerated(false),
    m_chunkGeneratedTrans(false), m_idxGeneratedTrans(false), m_bufChunkTrans(), m_bufIdxTrans(), m_bufLL(), m_llGenerated(false),
    mp_context(context)
{}

Drawable::~Drawable()
{}


void Drawable::destroyVBOdata()
{
    mp_context->glDeleteBuffers(1, &m_bufUV);
    mp_context->glDeleteBuffers(1, &m_bufChunk);
    mp_context->glDeleteBuffers(1, &m_bufIdx);
    mp_context->glDeleteBuffers(1, &m_bufPos);
    mp_context->glDeleteBuffers(1, &m_bufNor);
    mp_context->glDeleteBuffers(1, &m_bufCol);
    //mp_context->glDeleteBuffers(1, &m_bufUV);
    m_chunkGenerated = m_idxGenerated = m_posGenerated = m_norGenerated = m_colGenerated = m_uvGenerated = false;
    m_count = -1;
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return m_count;
}

int Drawable::elemCountTrans()
{
    return m_countTrans;
}
void Drawable::generateUV() {
    m_uvGenerated = true;
    mp_context->glGenBuffers(1, &m_bufUV);
}

void Drawable::generateChunkTrans() {
    m_chunkGeneratedTrans = true;
    mp_context->glGenBuffers(1, &m_bufChunkTrans);
}

void Drawable::generateIdxTrans()
{
    m_idxGeneratedTrans = true;
    mp_context->glGenBuffers(1, &m_bufIdxTrans);
}

void Drawable::generateChunk() {
    m_chunkGenerated = true;
    mp_context->glGenBuffers(1, &m_bufChunk);
}

void Drawable::generateIdx()
{
    m_idxGenerated = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    mp_context->glGenBuffers(1, &m_bufIdx);
}

void Drawable::generatePos()
{
    m_posGenerated = true;
    // Create a VBO on our GPU and store its handle in bufPos
    mp_context->glGenBuffers(1, &m_bufPos);
}

void Drawable::generateNor()
{
    m_norGenerated = true;
    // Create a VBO on our GPU and store its handle in bufNor
    mp_context->glGenBuffers(1, &m_bufNor);
}

void Drawable::generateCol()
{
    m_colGenerated = true;
    // Create a VBO on our GPU and store its handle in bufCol
    mp_context->glGenBuffers(1, &m_bufCol);
}

void Drawable::generateLL()
{
    m_colGenerated = true;
    // Create a VBO on our GPU and store its handle in bufCol
    mp_context->glGenBuffers(1, &m_bufLL);
}

bool Drawable::bindUV() {
    if(m_uvGenerated) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufUV);
    }
    return m_uvGenerated;
}

bool Drawable::bindLL() {
    if(m_llGenerated) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufLL);
    }
    return m_llGenerated;
}

bool Drawable::bindChunkTrans() {
    if(m_chunkGeneratedTrans) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufChunkTrans);
    }
    return m_chunkGeneratedTrans;
}

bool Drawable::bindIdxTrans()
{
    if(m_idxGeneratedTrans) {
        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIdxTrans);
    }
    return m_idxGeneratedTrans;
}

bool Drawable::bindChunk() {
    if(m_chunkGenerated) {
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufChunk);
    }
    return m_chunkGenerated;
}

bool Drawable::bindIdx()
{
    if(m_idxGenerated) {
        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIdx);
    }
    return m_idxGenerated;
}

bool Drawable::bindPos()
{
    if(m_posGenerated){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufPos);
    }
    return m_posGenerated;
}

bool Drawable::bindNor()
{
    if(m_norGenerated){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufNor);
    }
    return m_norGenerated;
}

bool Drawable::bindCol()
{
    if(m_colGenerated){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufCol);
    }
    return m_colGenerated;
}

InstancedDrawable::InstancedDrawable(OpenGLContext *context)
    : Drawable(context), m_numInstances(0), m_bufPosOffset(-1), m_offsetGenerated(false)
{}

InstancedDrawable::~InstancedDrawable(){}

int InstancedDrawable::instanceCount() const {
    return m_numInstances;
}

void InstancedDrawable::generateOffsetBuf() {
    m_offsetGenerated = true;
    mp_context->glGenBuffers(1, &m_bufPosOffset);
}

bool InstancedDrawable::bindOffsetBuf() {
    if(m_offsetGenerated){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufPosOffset);
    }
    return m_offsetGenerated;
}

void InstancedDrawable::clearOffsetBuf() {
    if(m_offsetGenerated) {
        mp_context->glDeleteBuffers(1, &m_bufPosOffset);
        m_offsetGenerated = false;
    }
}
void InstancedDrawable::clearColorBuf() {
    if(m_colGenerated) {
        mp_context->glDeleteBuffers(1, &m_bufCol);
        m_colGenerated = false;
    }
}
