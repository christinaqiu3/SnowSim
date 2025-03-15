#ifndef CHUNKWORKERS_H
#define CHUNKWORKERS_H

#include "terrain.h"
class BDWorker : public QRunnable {
private:
    int m_x;
    int m_z;
    std::vector<Chunk*> m_toProcess;
    std::unordered_set<Chunk*>* m_output;
    QMutex* m_outputLock;
public:
    BDWorker(int x, int z, std::vector<Chunk*> toProcess, std::unordered_set<Chunk*>* output, QMutex* outputLock);
    void run() override;
};


class VBOWorker : public QRunnable {
private:
    Chunk* m_chunk;
    std::vector<ChunkVBOData>* m_output;
    QMutex* m_outputLock;
public:
    VBOWorker(Chunk* c, std::vector<ChunkVBOData>* output, QMutex* outputLock);
    void run() override;
};

#endif // CHUNKWORKERS_H
