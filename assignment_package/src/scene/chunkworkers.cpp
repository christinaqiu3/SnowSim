#include "chunkworkers.h"

BDWorker::BDWorker(int x, int z, std::vector<Chunk *> toProcess, std::unordered_set<Chunk*> *output, QMutex *outputLock) :
    m_x(x), m_z(z), m_toProcess(toProcess), m_output(output), m_outputLock(outputLock)
{}

void BDWorker::run() {
    for (Chunk* c : m_toProcess) {
        c->initializeBlockData();

        m_outputLock->lock();
        m_output->insert(c);
        m_outputLock->unlock();
    }
}


VBOWorker::VBOWorker(Chunk* c, std::vector<ChunkVBOData>* output, QMutex* outputLock) :
    m_chunk(c), m_output(output), m_outputLock(outputLock)
{}

void VBOWorker::run() {
    ChunkVBOData vboData(m_chunk);

    m_chunk->createVBOdata(vboData);

    m_outputLock->lock();
    m_output->push_back(vboData);
    m_outputLock->unlock();
}
