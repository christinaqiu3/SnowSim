#include "terrain.h"
#include "cube.h"
#include <stdexcept>
#include <iostream>

#define CHUNKRANGE 3 //Also change in mygl.cpp. At least 2

Terrain::Terrain(OpenGLContext *context)
    : m_chunks(), m_generatedTerrain(), mp_context(context), isAether(false)
{}

//m_geomCube(context),

Terrain::~Terrain() {
    //m_geomCube.destroyVBOdata();

}

// Combine two 32-bit ints into one 64-bit int
// where the upper 32 bits are X and the lower 32 bits are Z
int64_t toKey(int x, int z) {
    int64_t xz = 0xffffffffffffffff;
    int64_t x64 = x;
    int64_t z64 = z;

    // Set all lower 32 bits to 1 so we can & with Z later
    xz = (xz & (x64 << 32)) | 0x00000000ffffffff;

    // Set all upper 32 bits to 1 so we can & with XZ
    z64 = z64 | 0xffffffff00000000;

    // Combine
    xz = xz & z64;
    return xz;
}

glm::ivec2 toCoords(int64_t k) {
    // Z is lower 32 bits
    int64_t z = k & 0x00000000ffffffff;
    // If the most significant bit of Z is 1, then it's a negative number
    // so we have to set all the upper 32 bits to 1.
    // Note the 8    V
    if(z & 0x0000000080000000) {
        z = z | 0xffffffff00000000;
    }
    int64_t x = (k >> 32);

    return glm::ivec2(x, z);
}

// Surround calls to this with try-catch if you don't know whether
// the coordinates at x, y, z have a corresponding Chunk
BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    if(hasChunkAt(x, z)) {
        // Just disallow action below or above min/max height,
        // but don't crash the game over it.
        if(y < 0 || y >= 256) {
            return EMPTY;
        }
        const uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        return c->getBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                             static_cast<unsigned int>(y),
                             static_cast<unsigned int>(z - chunkOrigin.y));
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

int Terrain::getLightLevelAt(int x, int y, int z) const
{
    if(hasChunkAt(x, z)) {
        // Just disallow action below or above min/max height,
        // but don't crash the game over it.
        if(y < 0 || y >= 256) {
            return EMPTY;
        }
        const uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        return c->getLightLevelAt(static_cast<unsigned int>(x - chunkOrigin.x),
                             static_cast<unsigned int>(y),
                             static_cast<unsigned int>(z - chunkOrigin.y));
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

BlockType Terrain::getBlockAt(glm::vec3 p) const {
    return getBlockAt(p.x, p.y, p.z);
}

bool Terrain::hasChunkAt(int x, int z) const {
    // Map x and z to their nearest Chunk corner
    // By flooring x and z, then multiplying by 16,
    // we clamp (x, z) to its nearest Chunk-space corner,
    // then scale back to a world-space location.
    // Note that floor() lets us handle negative numbers
    // correctly, as floor(-1 / 16.f) gives us -1, as
    // opposed to (int)(-1 / 16.f) giving us 0 (incorrect!).
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    return m_chunks.find(toKey(16 * xFloor, 16 * zFloor)) != m_chunks.end();
}


uPtr<Chunk>& Terrain::getChunkAt(int x, int z) {
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));

    return m_chunks[toKey(16 * xFloor, 16 * zFloor)];
}


const uPtr<Chunk>& Terrain::getChunkAt(int x, int z) const {
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    return m_chunks.at(toKey(16 * xFloor, 16 * zFloor));
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    if(hasChunkAt(x, z)) {
        uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        c->setBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                      static_cast<unsigned int>(y),
                      static_cast<unsigned int>(z - chunkOrigin.y),
                      t);
        c->createVBOdata();
        for (int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                uPtr<Chunk> &ch = getChunkAt(x + i * 16, z + j * 16);
                ch->createVBOdata();
            }
        }
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

void Terrain::setLightLevelAt(int x, int y, int z, int t)
{
    if(hasChunkAt(x, z)) {
        uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        c->setLightLevelAt(static_cast<unsigned int>(x - chunkOrigin.x),
                      static_cast<unsigned int>(y),
                      static_cast<unsigned int>(z - chunkOrigin.y),
                      t);
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

void Terrain::removeBlockAt(int x, int y, int z)
{
    if(hasChunkAt(x, z)) {
        uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        c->removeBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                      static_cast<unsigned int>(y),
                      static_cast<unsigned int>(z - chunkOrigin.y));
        c->createVBOdata();
        for (int i = -1; i <= 1; i++) { // watch for out of bounds
            for(int j = -1; j <= 1; j++) {
                uPtr<Chunk> &ch = getChunkAt(x + i * 16, z + j * 16);
                ch->createVBOdata();
            }
        }
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

Chunk* Terrain::instantiateChunkAt(int x, int z) {
    uPtr<Chunk> chunk = mkU<Chunk>(mp_context, x, z);
    chunk->isAether = isAether;
    Chunk *cPtr = chunk.get();
    m_chunks[toKey(x, z)] = move(chunk);
    // Set the neighbor pointers of itself and its neighbors
    if(hasChunkAt(x, z + 16)) {
        auto &chunkNorth = m_chunks[toKey(x, z + 16)];
        cPtr->linkNeighbor(chunkNorth, ZPOS);
    }
    if(hasChunkAt(x, z - 16)) {
        auto &chunkSouth = m_chunks[toKey(x, z - 16)];
        cPtr->linkNeighbor(chunkSouth, ZNEG);
    }
    if(hasChunkAt(x + 16, z)) {
        auto &chunkEast = m_chunks[toKey(x + 16, z)];
        cPtr->linkNeighbor(chunkEast, XPOS);
    }
    if(hasChunkAt(x - 16, z)) {
        auto &chunkWest = m_chunks[toKey(x - 16, z)];
        cPtr->linkNeighbor(chunkWest, XNEG);
    }
    return cPtr;
    return cPtr;
}

void Terrain::draw(int minX, int maxX, int minZ, int maxZ, ShaderProgram *shaderProgram, bool drawTrans) {
    //DRAW EACH CHUNK WITHIN THE RANGE
    for (int g = minX; g <= maxX; g+=64) {
        for (int h = minZ; h <= maxZ; h+=64) {
            int64_t id = toKey(g, h);
            if (m_generatedTerrain.find(id) != m_generatedTerrain.end()) {
                for (int i = g; i<g+64; i+=16) {
                    for (int j = h; j<h+64; j+=16) {
                        if (hasChunkAt(i, j)) {
                            Chunk* c = getChunkAt(i, j).get();
                            shaderProgram->drawChunk(*c, true);
                        }
                    }
                }
            }
        }
    }
    if (drawTrans) {
        for (int g = minX; g <= maxX; g+=64) {
            for (int h = minZ; h <= maxZ; h+=64) {
                int64_t id = toKey(g, h);
                if (m_generatedTerrain.find(id) != m_generatedTerrain.end()) {
                    for (int i = g; i<g+64; i+=16) {
                        for (int j = h; j<h+64; j+=16) {
                            if (hasChunkAt(i, j)) {
                                Chunk* c = getChunkAt(i, j).get();
                                shaderProgram->drawChunk(*c, false);
                            }
                        }
                    }
                }
            }
        }
    }
}


//WE DON"T USE THIS
void Terrain::CreateTestScene()
{
    // Create the Chunks that will
    // store the blocks for our
    // initial world space
    for(int x = 0; x < 64; x += 16) {
        for(int z = 0; z < 64; z += 16) {
            instantiateChunkAt(x, z);
        }
    }
    // Tell our existing terrain set that
    // the "generated terrain zone" at (0,0)
    // now exists.
    m_generatedTerrain.insert(toKey(0, 0));

//    setBlockAt(32, 130, 32, STONE);

#if 0
    // Create the basic terrain floor
    for(int x = 0; x < 64; ++x) {
        for(int z = 0; z < 64; ++z) {
            if((x + z) % 2 == 0) {
                setBlockAt(x, 128, z, STONE);
            }
            else {
                setBlockAt(x, 128, z, DIRT);
            }
        }
    }
    // Add "walls" for collision testing
    for(int x = 0; x < 64; ++x) {
        setBlockAt(x, 129, 0, GRASS);
        setBlockAt(x, 130, 0, GRASS);
        setBlockAt(x, 129, 63, GRASS);
        setBlockAt(0, 130, x, GRASS);
    }
    // Add a central column
    for(int y = 129; y < 140; ++y) {
        setBlockAt(32, y, 32, GRASS);
    }

    for(int x = 0; x < 64; x += 16) {
        for(int z = 0; z < 64; z += 16) {
            getChunkAt(x, z).get()->createVBOdata();
        }
    }
#endif
}


//THIS IS THE MAIN FUNCTION THAT INITIALIZES THE BASE 64X64 START AREA
void Terrain::TEST() {

    for(int x = 0; x < 64; x += 16) {
        for(int z = 0; z < 64; z += 16) {
            instantiateChunkAt(x, z);
        }
    }
    // Tell our existing terrain set that
    // the "generated terrain zone" at (0,0)
    // now exists.
    m_generatedTerrain.insert(toKey(0, 0));

    // Create the basic terrain floor
    for(int x = 0; x < 64; ++x) {
        for(int z = 0; z < 64; ++z) {
            if((x + z) % 2 == 0) {
                //setBlockAt(x, 128, z, STONE);
            }
            else {
                //setBlockAt(x, 128, z, DIRT);
            }
        }
    }
#if 0
    // Add "walls" for collision testing
    for(int x = 0; x < 64; ++x) {
        setBlockAt(x, 129, 0, GRASS);
        setBlockAt(x, 130, 0, GRASS);
        setBlockAt(x, 129, 63, GRASS);
        setBlockAt(0, 130, x, GRASS);
    }
    // Add a central column
    for(int y = 129; y < 140; ++y) {
        setBlockAt(32, y, 32, GRASS);
    }

    for(int x = 0; x < 64; x += 16) {
        for(int z = 0; z < 64; z += 16) {
            getChunkAt(x, z).get()->createVBOdata();
        }
    }
#endif
}

void Terrain::createFirstTGZ() {
    int range = CHUNKRANGE;
    for(int i = -range; i <= range; i++) {
        for(int j = -range; j <= range; j++) {
            spawnBDWorker(toKey(i*64, j*64));
        }
    }
}

void Terrain::redoFirstTGZ(float x, float z) {
    int range = CHUNKRANGE;
    for(int i = -range; i <= range; i++) {
        for(int j = -range; j <= range; j++) {
            spawnBDWorker(toKey(floor((i*64 + x)/ 64.f) * 64, floor((j*64 + z)/64.f) * 64));
        }
    }
}

void Terrain::tickCreateChunk(float x, float z) {
    Chunk* curChunk = getChunkAt(floor(x) - (int)floor(x)%16, floor(z) - (int)floor(z)%16).get();
    int nxtX = floor(x) - (int)floor(x)%16; //X position of the current chunk the player is in
    int nxtZ = floor(z) - (int)floor(z)%16; //Z position of the current chunk the player is in
    int range = 1; //DENOTES THE NUMBER OF CHUNKS (* 0.5 -1) GENERATED AROUND THE PLAYER WHEN NEW CHUNKS NEED TO BE LOADED

    for (int i = -range; i <= range; i++) {
        for (int j = -range; j <= range; j++) {
            nxtX += 16 * i;
            nxtZ += 16 * j;

            if (!hasChunkAt(nxtX, nxtZ) && getChunkAt(nxtX, nxtZ).get() != curChunk) {
                instantiateChunkAt(nxtX, nxtZ);
                getChunkAt(nxtX, nxtZ).get()->createVBOdata();

//                for (int k = 0; k<4; k++) {
//                    neighbX = nxtX;
//                    neighbZ = nxtZ;

//                    if (dir[k] == XPOS) {
//                        neighbX += 16;
//                    } else if (dir[k] == XNEG) {
//                        neighbX += -16;
//                    } else if (dir[k] == ZPOS) {
//                        neighbZ += 16;
//                    } else if (dir[k] == ZNEG) {
//                        neighbZ += -16;
//                    }
//                    if (hasChunkAt(neighbX, neighbZ)) {
//                        getChunkAt(neighbX, neighbZ).get()->createVBOdata();
//                    }
//                }
            }

            nxtX = floor(x) - (int)floor(x)%16;
            nxtZ = floor(z) - (int)floor(z)%16;
        }
    }
}

void Terrain::tickCreateChunk(float x, float z, float oldX, float oldZ) {
    //Chunk* curChunk = getChunkAt(floor(x) - (int)floor(x)%16, floor(z) - (int)floor(z)%16).get();
    int xTGZ = floor(x) - ((int)floor(x)%64 + 64) % 64; //X position of the current terriain generation zone the player is in
    int zTGZ = floor(z) - ((int)floor(z)%64 + 64) % 64; //Z position of the current TGZ the player is in

    int oldXTGZ = floor(oldX) - ((int)floor(oldX)%64 + 64) % 64;
    int oldZTGZ = floor(oldZ) - ((int)floor(oldZ)%64 + 64) % 64;

    int range = CHUNKRANGE; //DENOTES THE NUMBER OF TGZs (* 0.5 -1) GENERATED AROUND THE PLAYER

    int dx = (xTGZ - oldXTGZ);
    int dz = (zTGZ - oldZTGZ);

    if (dx != 0 || dz != 0) {
        for(int i = -range + 1; i <= range - 1; i++) { // range edit because VBO range is one less than total
            int xIs1 = (-abs(dx) + 64) * i;
            int zIs1 = (-abs(dz) + 64) * i;
            // this calculates TGZs that have just been left
            // if dx == 0, range is nullified and xIs1 will just be i
            // id dx == +\- 1, it will be on the correct side of the chunk and xIs1 will be 0
            glm::ivec2 coord = glm::ivec2(-dx * range + xIs1 + oldXTGZ, -dz * range + zIs1 + oldZTGZ);
            for(int xt = 0; xt < 64; xt += 16) {
                for(int zt = 0; zt < 64; zt += 16) {
                    Chunk* chunk = getChunkAt(xt + coord.x, zt + coord.y).get();
                    chunk->destroyVBOdata();
                    chunk->setCount(0);
                }
            }
        }

        for(int i = -range; i <= range; i++) {
            int xIs1 = (-abs(dx) + 64) * i;
            int zIs1 = (-abs(dz) + 64) * i;
            // this calculates TGZs that have just been entered
            glm::ivec2 coord = glm::ivec2(dx * range + xIs1 + xTGZ, dz * range + zIs1 + zTGZ);
            if (m_generatedTerrain.find(toKey(coord.x, coord.y)) != m_generatedTerrain.end() && abs(i) < range) {
                for(int xt = 0; xt < 64; xt += 16) {
                    for(int zt = 0; zt < 64; zt += 16) {
                        Chunk* chunk = getChunkAt(xt + coord.x, zt + coord.y).get();
                        spawnVBOWorker(chunk);
                    }
                }
            } else {
                spawnBDWorker(toKey(coord.x, coord.y));
            }
        }
    }
    // Check if any threads have finished
    m_freshlySpawnedChunksLock.lock();
    for (Chunk* ch : m_freshlySpawnedChunks) {
        spawnVBOWorker(ch);
    }
    m_freshlySpawnedChunks.clear();
    m_freshlySpawnedChunksLock.unlock();

    m_chunksWithVBOsLock.lock();
    for(ChunkVBOData &data : m_chunksWithVBOs) {
        data.m_chunk->pushChunkVBO(data.m_idxOpaque, data.m_vboOpaque, data.m_idxTransparent, data.m_vboTransparent);
    }
    m_chunksWithVBOs.clear();
    m_chunksWithVBOsLock.unlock();

# if 0
    if (xTGZ != oldXTGZ && zTGZ != oldZTGZ) {
        for (int i = -range; i <= range; i++) {
            for (int j = -range; j <= range; j++) {
                int nxtX = 64 * i + xTGZ;
                int nxtZ = 64 * j + zTGZ;

                if (!hasChunkAt(nxtX, nxtZ) && getChunkAt(nxtX, nxtZ).get() != curChunk) {
                    instantiateChunkAt(nxtX, nxtZ);
                    getChunkAt(nxtX, nxtZ).get()->createVBOdata();
                }
            }
        }
    }
# endif
}

void Terrain::spawnBDWorker(int64_t tgz) {
    m_generatedTerrain.insert(tgz);
    std::vector<Chunk*> workList;
    glm::ivec2 coords = toCoords(tgz);
    for(int x = 0; x < 64; x += 16) {
        for(int z = 0; z < 64; z += 16) {
            Chunk* c = instantiateChunkAt(coords.x + x, coords.y + z);
            c->setCount(0);
            // TODO: split transparent and opaque count and initialize them.
            workList.push_back(c);
        }
    }
    BDWorker* worker = new BDWorker(coords.x, coords.y, workList, &m_freshlySpawnedChunks, &m_freshlySpawnedChunksLock);
    QThreadPool::globalInstance()->start(worker);
}

void Terrain::spawnVBOWorker(Chunk* c) {
    VBOWorker* worker = new VBOWorker(c, &m_chunksWithVBOs, &m_chunksWithVBOsLock);
    QThreadPool::globalInstance()->start(worker);
}


void Terrain::clearThreads() {
    m_freshlySpawnedChunksLock.lock();
    m_freshlySpawnedChunks.clear();
    m_freshlySpawnedChunksLock.unlock();

    m_chunksWithVBOsLock.lock();
    m_chunksWithVBOs.clear();
    m_chunksWithVBOsLock.unlock();
}

