#pragma once
#include "smartpointerhelp.h"
#include "glm_includes.h"
#include "drawable.h"
#include <array>
#include <unordered_map>
#include <cstddef>


//using namespace std;

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.
enum BlockType : unsigned char
{
    EMPTY, GRASS, DIRT, STONE, WATER, SNOW, LAVA, BEDROCK, GLOW, FLOWERR, FLOWERY, TALL_GRASS,  CLAY,
    REDCLAY, SAND, OAKLOG, BIRCHLOG, SPRUCELOG, JUNGLELOG, LEAVES, JUNGLELEAVES, FLOWERW, FLOWERB, FLOWERP,
    CORALY, CORALR, CORALM, CORALP, CORALB, CORALO, RED_MUSH, BROWN_MUSH,

    //ETHER BLOCKS
    PURPGRASS, LIGHT_WOOD, MID_WOOD, DARK_WOOD, FANCY_ICE, LIGHT_SAND, DARK_SAND,
    DARK_FLOWER_LEAVES, MID_FLOWER_LEAVES, LIGHT_FLOWER_LEAVES, ROCKS, AMETHYST,
    ROCK_GRASS, ROCK_DIRT, FLOWER_GRASS, REG_ICE

};

enum Biome : unsigned char
{
    PLAINS, MOUNTAINS, SWAMP, DESERT, SAVANNAH, MESA, BIRCH, TAIGA, SNOWYPLAINS, SNOWYMOUTAINS, JUNGLE, FOREST, OCEAN
};
// The six cardinal directions in 3D space
enum Direction : unsigned char
{
    XPOS, XNEG, YPOS, YNEG, ZPOS, ZNEG
};

// Lets us use any enum class as the key of a
// std::unordered_map
struct EnumHash {
    template <typename T>
    size_t operator()(T t) const {
        return static_cast<size_t>(t);
    }
};

class Chunk;

struct ChunkVBOData {
    Chunk* m_chunk;
    std::vector<glm::vec4> m_vboOpaque;
    std::vector<GLuint> m_idxOpaque;
    std::vector<glm::vec4> m_vboTransparent;
    std::vector<GLuint> m_idxTransparent;

    ChunkVBOData(Chunk* c) :
        m_chunk(c), m_vboOpaque{}, m_idxOpaque{}, m_vboTransparent{}, m_idxTransparent{}
    {}
};


// One Chunk is a 16 x 256 x 16 section of the world,
// containing all the Minecraft blocks in that area.
// We divide the world into Chunks in order to make
// recomputing its VBO data faster by not having to
// render all the world at once, while also not having
// to render the world block by block.

// TODO have Chunk inherit from Drawable
class Chunk : public Drawable
{
private:
    // All of the blocks contained within this Chunk
    std::array<BlockType, 65536> m_blocks;
    int minX, minZ;
    std::array<Biome, 256> m_biomes;
    std::array<float, 256> m_temperature;
    std::array<float, 256> m_humidity;
    std::array<int, 256> m_heights;
    std::array<int, 256> m_heights2;
    std::array<unsigned char, 65536> m_lightLevels;
    // This Chunk's four neighbors to the north, south, east, and west
    // The third input to this map just lets us use a Direction as
    // a key for this map.
    // These allow us to properly determine
    std::unordered_map<Direction, Chunk*, EnumHash> m_neighbors;

public:
    bool isAether;
    Chunk(OpenGLContext *c);
    Chunk(OpenGLContext *c, int x, int z);
    void initializeBlockData();

    BlockType getBlockAt(unsigned int x, unsigned int y, unsigned int z) const;
    BlockType getBlockAt(int x, int y, int z) const;
    int getLightLevelAt(int x, int y, int z) const;
    void setBlockAt(unsigned int x, unsigned int y, unsigned int z, BlockType t);
    void setLightLevelAt(unsigned int x, unsigned int y, unsigned int z, int t);
    void removeBlockAt(unsigned int x, unsigned int y, unsigned int z);
    void linkNeighbor(uPtr<Chunk>& neighbor, Direction dir);

    std::unordered_map<Direction, Chunk*, EnumHash> getNeighbors();

    void plantTree(int id, int x, int y, int z);

    void createVBOdata();
    void createVBOdata(ChunkVBOData& c);
    void vboBlockFace(Direction d, int x, int y, int z, glm::ivec2 chunkMins, ChunkVBOData &ch, int &index, BlockType bt, float temperature, float humidity, int lightlevel);

    void pushChunkVBO(std::vector<GLuint> idx, std::vector<glm::vec4> chunk,
                      std::vector<GLuint> idxTrans, std::vector<glm::vec4> chunkTrans);
    GLenum drawMode();
    void setCount(int i);
    glm::ivec2 getCoords();

    void findOcVal(int i, int am1, std::pair<int, int> bounds1, int am2, std::pair<int, int> bounds2,
                   int am3, std::pair<int, int> bounds3, int magic, int& numOcl, Chunk* c);
};
