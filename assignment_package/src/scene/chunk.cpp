#include "chunk.h"
#include <array>
#include <iostream>

const static BlockType tree[4][5][5] = {
    {{LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, OAKLOG, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES}},

    {{LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, OAKLOG, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, LEAVES, OAKLOG, LEAVES, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, LEAVES, EMPTY, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, EMPTY, LEAVES, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
};

const static BlockType spruceTree[8][7][7] = {
    {{EMPTY, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, EMPTY},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, SPRUCELOG, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES},
     {EMPTY, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, LEAVES, LEAVES, LEAVES, EMPTY, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, LEAVES, LEAVES, SPRUCELOG, LEAVES, LEAVES, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, EMPTY, LEAVES, LEAVES, LEAVES, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, LEAVES, SPRUCELOG, LEAVES, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, SPRUCELOG, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, LEAVES, LEAVES, LEAVES, EMPTY, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, LEAVES, LEAVES, SPRUCELOG, LEAVES, LEAVES, EMPTY},
     {EMPTY, LEAVES, LEAVES, LEAVES, LEAVES, LEAVES, EMPTY},
     {EMPTY, EMPTY, LEAVES, LEAVES, LEAVES, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, LEAVES, SPRUCELOG, LEAVES, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, LEAVES, LEAVES, LEAVES, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, LEAVES, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},

    };

const static std::unordered_map<Biome, float, EnumHash> sparcity {
    {PLAINS, 0.2f}, {JUNGLE, 1.f}, {FOREST, 0.75f}, {BIRCH, 0.75f}, {TAIGA, 0.6f},
    {OCEAN, 0.f}, {MESA, 0.2f}, {MOUNTAINS, 0.2f}, {SAVANNAH, 0.5f}, {DESERT, 0.5f},
    {SWAMP, 0.6f}
};

// Noise functions from slides or mini-minecraft visualizer
glm::vec2 smoothF(glm::vec2 uv)
{
    return uv*uv*(glm::vec2(3.f, 3.f) - uv*2.f);
}

float noise(glm::vec2 uv)
{
    const float k = 257.;
    glm::vec4 l  = glm::vec4(glm::floor(uv),glm::fract(uv));
    float u = l.x + l.y * k;
    glm::vec4 v  = glm::vec4(u, u+1.,u+k, u+k+1.);
    v       = glm::fract(glm::fract(v*1.23456789f)*v * (1.f/.987654321f));
    l.z    = smoothF(glm::vec2(l.z, l.w)).r;
    l.w    = smoothF(glm::vec2(l.z, l.w)).g;
    l.x     = glm::mix(v.x, v.y, l.z);
    l.y     = glm::mix(v.z, v.w, l.z);
    return    glm::mix(l.x, l.y, l.w);
}

float fbm(const glm::vec2 uv)
{
    float a = 0.5;
    float f = 5.0;
    float n = 0.;
    int it = 8;
    for(int i = 0; i < 32; i++)
    {
        if(i<it)
        {
            n += noise(uv*f)*a;
            a *= .5;
            f *= 2.;
        }
    }
    return n;
}

float fbm(const glm::vec2 uv, float freq, int iter)
{
    float a = 0.5;
    float f = freq;
    float n = 0.;
    int it = iter;
    for(int i = 0; i < 32; i++)
    {
        if(i<it)
        {
            n += noise(uv*f)*a;
            a *= .5;
            f *= 2.;
        }
    }
    return n;
}

glm::vec2 random2( glm::vec2 p ) {
    return glm::normalize(glm::fract(glm::sin(glm::vec2(glm::dot(p,glm::vec2(127.1,311.7)),glm::dot(p,glm::vec2(269.5,183.3))))*43758.5453f) * 2.f - glm::vec2(1.f, 1.f));
}

glm::vec3 random3( glm::vec3 p ) {
    return glm::normalize(glm::fract(glm::sin(glm::vec3(
                                         glm::dot(p, glm::vec3(127.1, 311.7, 74.7)),
                                         glm::dot(p, glm::vec3(269.5, 183.3, 246.1)),
                                         glm::dot(p, glm::vec3(419.2, 371.9, 156.3))
                                         )) * 43758.5453f) * 2.f - glm::vec3(1.f, 1.f, 1.f));
}

float surflet(glm::vec2 P, glm::vec2 gridPoint)
{
    // Compute falloff function by converting linear distance to a polynomial (quintic smootherstep function)
    float distX = abs(P.x - gridPoint.x);
    float distY = abs(P.y - gridPoint.y);
    float tX = 1 - 6 * pow(distX, 5.0) + 15 * pow(distX, 4.0) - 10 * pow(distX, 3.0);
    float tY = 1 - 6 * pow(distY, 5.0) + 15 * pow(distY, 4.0) - 10 * pow(distY, 3.0);

    // Get the random vector for the grid point
    glm::vec2 gradient = random2(gridPoint);
    // Get the vector from the grid point to P
    glm::vec2 diff = P - gridPoint;
    // Get the value of our height field by dotting grid->P with our gradient
    float height = glm::dot(diff, gradient);
    // Scale our height field (i.e. reduce it) by our polynomial falloff function
    return height * tX * tY;
}

float surflet3(glm::vec3 p, glm::vec3 gridPoint) {
    // Compute the distance between p and the grid point along each axis, and warp it with a
    // quintic function so we can smooth our cells
    glm::vec3 t2 = glm::abs(p - gridPoint);
    glm::vec3 t = glm::vec3(1.f) - glm::vec3(6.f * pow(static_cast<float>(t2.x), 5.f), 15.f * pow(static_cast<float>(t2.y), 5.f), 10.f * pow(static_cast<float>(t2.z), 5.f));
    // Get the random vector for the grid point (assume we wrote a function random2
    // that returns a vec2 in the range [0, 1])
    glm::vec3 gradient = random3(gridPoint) * 2.f - glm::vec3(1., 1., 1.);
    // Get the vector from the grid point to P
    glm::vec3 diff = p - gridPoint;
    // Get the value of our height field by dotting grid->P with our gradient
    float height = glm::dot(diff, gradient);
    // Scale our height field (i.e. reduce it) by our polynomial falloff function
    return height * t.x * t.y * t.z;
}

float PerlinNoise(glm::vec2 uv)
{
    // Tile the space
    glm::vec2 uvXLYL = glm::floor(uv);
    glm::vec2 uvXHYL = uvXLYL + glm::vec2(1,0);
    glm::vec2 uvXHYH = uvXLYL + glm::vec2(1,1);
    glm::vec2 uvXLYH = uvXLYL + glm::vec2(0,1);

    return surflet(uv, uvXLYL) + surflet(uv, uvXHYL) + surflet(uv, uvXHYH) + surflet(uv, uvXLYH);
}

float perlinNoise3D(glm::vec3 p) {
    float surfletSum = 0.f;
    // Iterate over the eight integer corners surrounding uv
    for(int dx = 0; dx <= 1; ++dx) {
        for(int dy = 0; dy <= 1; ++dy) {
            for(int dz = 0; dz <= 1; ++dz) {
                surfletSum += surflet3(p, glm::floor(p) + glm::vec3(dx, dy, dz));
            }
        }
    }
    return surfletSum;
}

float WorleyNoise(glm::vec2 uv)
{
    glm::vec2 uvInt = glm::floor(uv);
    glm::vec2 uvFract = glm::fract(uv);
    float minDist = 10.0; // Minimum distance initialized to max.
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            glm::vec2 neighbor = glm::vec2(float(x), float(y)); // Direction in which neighbor cell lies
            glm::vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            glm::vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbors Voronoi point
            float dist = glm::length(diff);
            minDist = glm::min(minDist, dist);
        }
    }
    return minDist;

}

float WorleyCells(glm::vec2 uv)
{
    glm::vec2 uvInt = glm::floor(uv);
    glm::vec2 uvFract = glm::fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    float value = 0.f;
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            glm::vec2 neighbor = glm::vec2(float(x), float(y)); // Direction in which neighbor cell lies
            glm::vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            glm::vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbors Voronoi point
            float dist = glm::length(diff);
            value = (minDist < dist) ? value : abs(random2(uvInt + neighbor).x);
            minDist = glm::min(minDist, dist);
        }
    }
    return value;
}

float mountainsHeight(glm::vec2 xz)
{
    float h = 0;

    float amp = 0.5;
    float freq = 128;
    for(int i = 0; i < 4; ++i) {
        glm::vec2 offset = glm::vec2(fbm(xz / 256.f), fbm(xz / 300.f) + 100.f);
        float h1 = PerlinNoise((xz + offset * 75.f) / freq);
        h1 = glm::smoothstep(-0.5f, 1.f, h1);
        h += h1 * amp;

        amp *= 0.5;
        freq *= 0.5;
    }
    return h;
}

float plainsHeight(glm::vec2 xz)
{
    float h = 0;
    float amp = 1.f/32.f;
    h += fbm(xz/256.f);
    h *= amp;
    return h;
}



Chunk::Chunk(OpenGLContext *c, int x, int z) : Drawable(c), m_blocks(), minX(x), minZ(z),
    m_neighbors{{XPOS, nullptr}, {XNEG, nullptr}, {ZPOS, nullptr}, {ZNEG, nullptr}}
{
    std::fill_n(m_blocks.begin(), 65536, EMPTY);
    std::fill_n(m_lightLevels.begin(), 65536, EMPTY);
#if 0
    for (int xn = 0; xn < 16; xn++) {
        for (int zn = 0; zn < 16; zn++) {
            glm::vec2 xz = glm::vec2(xn + minX + 400.f, zn + minZ);
            float biomeMap = (fbm(xz * (1.f / 512.0f)));
            biomeMap = glm::smoothstep(0.47f, 0.7f, biomeMap);
            float f = mountainsHeight(xz);
            float g = plainsHeight(xz);
            float h = (1 - biomeMap) * g + biomeMap * f;
            int height = floor(135 + h * 200);

            for (int y = 0; y < std::max(std::min(height, 255), 139); y++) {
                glm::vec3 worldPos = glm::vec3(xn + minX + 400.f, y, zn + minZ);
                float noiseValue = perlinNoise3D(worldPos);

                if (y == 0) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = BEDROCK;
                }
                if (y < 25 && noiseValue < 0.0f) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = LAVA;
                } else if (y<128 && y>64 && noiseValue < 0.0f) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = EMPTY;
                } else if (y < 129) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = STONE;
                } else if (y == height - 1 && y > 137 && biomeMap < 0.4f) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = GRASS;
                } else if (biomeMap < 0.4f && y < height) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = DIRT;
                } else if (y == height - 1 && y >= 200) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = SNOW;
                } else if (y <= height - 1) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = STONE;
                } else {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = WATER;
                }
            }
        }
    }
#endif
}

void Chunk::initializeBlockData() {

    for (int xn = 0; xn < 16; xn++) {
        for (int zn = 0; zn < 16; zn++) {

            glm::vec2 xz = glm::vec2(xn + minX, zn + minZ);
            //float biomeMap = (fbm(xz * (1.f / 512.0f)));
            //biomeMap = glm::smoothstep(0.47f, 0.7f, biomeMap);
            float f = mountainsHeight(xz);
            //float g = plainsHeight(xz);
            //float h = (1 - biomeMap) * g + biomeMap * f;
            //int height = floor(135 + h * 200);

            glm::vec2 uv = (xz +glm::vec2(0.f, 4000.f))/1024.0f;
            float noise = fbm(uv, 10.f, 6);
            float continentalness = 0.16*glm::smoothstep(0.32f, 0.36f, noise) + 0.08*glm::smoothstep(0.39f,0.400f,noise)+ 0.38*glm::smoothstep(0.43f,0.47f,noise) + 0.38*glm::smoothstep(0.50f,0.543f,noise);
            noise = fbm(uv, 3.8f, 6);
            float erosion = 0.16*glm::smoothstep(0.346f, 0.386f, noise) + 0.08*glm::smoothstep(0.426f,0.446f,noise)+ 0.38*glm::smoothstep(0.498f,0.518f,noise) + 0.38*glm::smoothstep(0.526f,0.546f,noise);
            noise = fbm(uv, 8.3f, 3);
            float pv = glm::smoothstep(0.375f, 0.415f, noise) - glm::smoothstep(0.443f, 0.483f, noise);
            noise = fbm(uv + glm::vec2(1.0), 1.1f, 2);
            noise += 0.25;
            float temperature = 0.16*glm::smoothstep(0.361f, 0.421f, noise) + 0.08*glm::smoothstep(0.459f,0.519f,noise)+ 0.38*glm::smoothstep(0.563f,0.623f,noise) + 0.38*glm::smoothstep(0.616f,0.676f,noise);
            noise = fbm(uv + glm::vec2(1.0), 2.8f, 2);
            noise += 0.25;
            float humidity = 0.16*glm::smoothstep(0.361f, 0.421f, noise) + 0.08*glm::smoothstep(0.449f,0.519f,noise)+ 0.38*glm::smoothstep(0.563f,0.623f,noise) + 0.38*glm::smoothstep(0.616f,0.676f,noise);
            float final = 0.325 * continentalness + 0.675 * (1. - ( 1.-continentalness) * (1.-erosion));
            final =  0.125 * final + 0.875 * (1. - (1.-final) * (1.-pv));

            int height = glm::floor(140 + (1.0 - final + f) * 100 * (erosion+0.2));

            //int height = glm::floor(135 + 50 * continentalness);

            int biomeloc = xn + 16 * zn;
            float mix = 0.75 * continentalness + 0.25 * erosion;
            if (mix < 0.02) {
                m_biomes.at(biomeloc) = OCEAN;
            } else if (continentalness < 0.3) {
                if (humidity > 0.8) {
                    m_biomes.at(biomeloc) = SWAMP;
                } else if (temperature > 0.8) {
                    m_biomes.at(biomeloc) = DESERT;
                } else {
                    m_biomes.at(biomeloc) = PLAINS;
                }
            } else if (mix > 0.8) {
                if (temperature > 0.8) {
                    if (humidity > 0.5) {
                        m_biomes.at(biomeloc) = SAVANNAH;
                    } else {
                        m_biomes.at(biomeloc) = MESA;
                    }
                } else {
                    m_biomes.at(biomeloc) = MOUNTAINS;
                }
            } else {
                if (temperature > 0.8) {
                    if (humidity > 0.5) {
                        m_biomes.at(biomeloc) = JUNGLE;
                    } else {
                        m_biomes.at(biomeloc) = BIRCH;
                    }
                } else if (temperature < 0.18f) {
                    m_biomes.at(biomeloc) = TAIGA;
                }
                else {
                    m_biomes.at(biomeloc) = FOREST;
                }

            }
            Biome biome = m_biomes.at(biomeloc);
            m_temperature.at(biomeloc) = temperature;
            m_humidity.at(biomeloc) = humidity;
            m_heights.at(biomeloc) = height;
            int height2 = 152.f - (height - 152.f);
            m_heights2.at(biomeloc) = height2 * height2 / 152.f;

            for (int y = 0; y < std::max(std::min(height + 1, 255), 152); y++) {
                glm::vec3 worldPos = glm::vec3(xn + minX + 400.f, y, zn + minZ);
                float noiseValue = perlinNoise3D(worldPos / 16.f);
                //float noiseValue2 = PerlinNoise(glm::vec2(worldPos.x, worldPos.y) / 32.f);
                //float noise = noiseValue * noiseValue + noiseValue2 * noiseValue2;

                if (y < ((isAether) ? m_heights2.at(biomeloc) : 0)) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = EMPTY;
                } else if (y < 1) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = BEDROCK;
                }else if (y < 128 && y>75 && noiseValue < 0.0f) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = EMPTY;
                } else if (y <= 75 && y > 50 && noiseValue < 0.0f) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = LAVA;
                } else if (y < height - 4) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = isAether ? STONE : STONE;
                } else if (y == height - 1 && y > 150 && temperature < 0.18f) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = isAether ? FANCY_ICE : SNOW;
                } else if (y == height - 1 && y > 150) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = biome == DESERT ? (isAether ? LIGHT_SAND : SAND) : (biome == MESA ? (isAether ? DARK_SAND : CLAY) : (biome == MOUNTAINS ? (isAether? STONE : STONE) : (isAether ? ROCK_GRASS : GRASS)));
                } else if (y < height) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = biome == DESERT ? (isAether ? LIGHT_SAND : SAND) : (biome == MESA ? (isAether ? DARK_SAND : CLAY) : (biome == MOUNTAINS ? (isAether? STONE : STONE) : (isAether ? ROCK_DIRT : DIRT)));
                } else if (y == height && y > 152 && (biome != DESERT && biome != MESA)) { // && temperature > 0.18f)) {
                    float rand = random2(glm::vec2(xn + minX, zn + minZ)).x + 1.f;
                    if (rand < 0.01) {
                        m_blocks.at(xn + 16 * y + 16 * 256 * zn) = FLOWERY;
                    } else if (rand < 0.02) {
                        m_blocks.at(xn + 16 * y + 16 * 256 * zn) = isAether ? FLOWERR : RED_MUSH;
                    } else if (rand < 0.03) {
                        m_blocks.at(xn + 16 * y + 16 * 256 * zn) = isAether ? FLOWERB : BROWN_MUSH;
                    } else if (rand < 0.04) {
                        m_blocks.at(xn + 16 * y + 16 * 256 * zn) = isAether ? FLOWERW : TALL_GRASS;
                    } else if (rand < 0.05) {
                        m_blocks.at(xn + 16 * y + 16 * 256 * zn) = isAether ? FLOWERP : TALL_GRASS;
                    } else if (rand < 0.2) {
                        m_blocks.at(xn + 16 * y + 16 * 256 * zn) = TALL_GRASS;
                    }
                } else if (y == height - 1 && y >= 200) {
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = SNOW;
                } else if (y < 152){
                    m_blocks.at(xn + 16 * y + 16 * 256 * zn) = WATER;
                }
            }
            if (biome == MESA) {
//                BlockType block = m_blocks.at(xn + 16 * 120 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 121 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 132 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 142 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 143 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 144 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 163 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 167 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 168 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
//                block = m_blocks.at(xn + 16 * 182 + 16 * 256 * zn);
//                m_blocks.at(xn + 16 * 120 + 16 * 256 * zn) = block == EMPTY ? EMPTY : REDCLAY;
            }
        }
    }
    //Trees
    for (int it = -5; it < 21; it += 5) {
        for (int kt = -5; kt < 21; kt += 5) {
            int i = it - ((((it + minX) % 5) + 5) % 5);
            int k = kt - ((((kt + minZ) % 5) + 5) % 5);
                int place = abs(floor(25.f * random2(glm::vec2(i + minX, k + minZ)).x));
                int offsetX = place / 5;
                int offsetZ = place % 5;
                int y = 255;
                Biome biome = OCEAN;
                if (i + offsetX >= 0 && i + offsetX < 16 && k + offsetZ >= 0 && k + offsetZ < 16) {
                    y = m_heights.at(i + offsetX + 16 * (k + offsetZ));
                    biome = m_biomes.at(i + offsetX + 16 * (k + offsetZ));
                } else {
                    glm::vec2 xz = glm::vec2(i + offsetX + minX, k + offsetZ + minZ);
                    float f = mountainsHeight(xz);
                    glm::vec2 uv = (xz +glm::vec2(0.f, 4000.f))/1024.0f;
                    float noise = fbm(uv, 10.f, 6);
                    float continentalness = 0.16*glm::smoothstep(0.32f, 0.36f, noise) + 0.08*glm::smoothstep(0.39f,0.400f,noise)+ 0.38*glm::smoothstep(0.43f,0.47f,noise) + 0.38*glm::smoothstep(0.50f,0.543f,noise);
                    noise = fbm(uv, 3.8f, 6);
                    float erosion = 0.16*glm::smoothstep(0.346f, 0.386f, noise) + 0.08*glm::smoothstep(0.426f,0.446f,noise)+ 0.38*glm::smoothstep(0.498f,0.518f,noise) + 0.38*glm::smoothstep(0.526f,0.546f,noise);
                    noise = fbm(uv, 8.3f, 3);
                    float pv = glm::smoothstep(0.375f, 0.415f, noise) - glm::smoothstep(0.443f, 0.483f, noise);
                    noise = fbm(uv + glm::vec2(1.0), 1.1f, 2);
                    noise += 0.25;
                    float temperature = 0.16*glm::smoothstep(0.361f, 0.421f, noise) + 0.08*glm::smoothstep(0.459f,0.519f,noise)+ 0.38*glm::smoothstep(0.563f,0.623f,noise) + 0.38*glm::smoothstep(0.616f,0.676f,noise);
                    noise = fbm(uv + glm::vec2(1.0), 2.8f, 2);
                    noise += 0.25;
                    float humidity = 0.16*glm::smoothstep(0.361f, 0.421f, noise) + 0.08*glm::smoothstep(0.449f,0.519f,noise)+ 0.38*glm::smoothstep(0.563f,0.623f,noise) + 0.38*glm::smoothstep(0.616f,0.676f,noise);
                    float final = 0.325 * continentalness + 0.675 * (1. - ( 1.-continentalness) * (1.-erosion));
                    final =  0.125 * final + 0.875 * (1. - (1.-final) * (1.-pv));

                    y = glm::floor(140 + (1.0 - final + f) * 100 * (erosion+0.2));
                    //int height = glm::floor(135 + 50 * continentalness);

                    float mix = 0.75 * continentalness + 0.25 * erosion;
                    if (mix < 0.02) {
                        biome = OCEAN;
                    } else if (continentalness < 0.3) {
                        if (humidity > 0.8) {
                           biome = SWAMP;
                        } else if (temperature > 0.8) {
                            biome = DESERT;
                        } else {
                            biome = PLAINS;
                        }
                    } else if (mix > 0.8) {
                        if (temperature > 0.8) {
                            if (humidity > 0.5) {
                                biome = SAVANNAH;
                            } else {
                                biome = MESA;
                            }
                        } else {
                            biome = MOUNTAINS;
                        }
                    } else {
                        if (temperature > 0.8) {
                            if (humidity > 0.5) {
                                biome = JUNGLE;
                            } else {
                                biome = BIRCH;
                            }
                        } else if (temperature < 0.18) {
                           biome = TAIGA;
                        }
                        else {
                            biome = FOREST;
                        }

                    }
                }
                if (y > 152 && abs(random2(glm::vec2(i + offsetX + minX + 400.f, k + offsetZ + minZ)).x) < sparcity.at(biome)) {
                    switch(biome) {
                    case PLAINS:
                    case FOREST:
                    case SWAMP:
                    case MOUNTAINS:
                    case MESA:
                        plantTree(0, i + offsetX, y, k + offsetZ);
                        break;
                    case BIRCH:
                        plantTree(1, i + offsetX, y, k + offsetZ);
                        break;
                    case TAIGA:
                        plantTree(2, i + offsetX, y, k + offsetZ);
                        break;
                    case JUNGLE:
                        plantTree(3, i + offsetX, y, k + offsetZ);
                        break;
                    case SAVANNAH:
                        plantTree(4, i + offsetX, y, k + offsetZ);
                        break;
                    case OCEAN:
                    case DESERT:
                        break; //CACTUSES
                    default:
                        break;
                    }
                }

        }
    }
}

void Chunk::plantTree(int id, int x, int y, int z) {
    if (id == 0 || id == 1 || id == 4) {
        for (int i = 0; i <= (x + minX) % (id == 1 ? 8 : 3); i++) {
            if (x >= 0 && x < 16 && y < 255 && y+i < 255 && z >=0 && z < 16) {
                    m_blocks.at(x + 16 * (y + i) + 16 * 256 * (z)) = id == 1 ? (isAether ? LIGHT_WOOD : BIRCHLOG) : (isAether ? MID_WOOD : OAKLOG);
            }
        }
        int offset = (x + minX) % (id == 1 ? 8 : 3) + 1;
        for (int i = -2; i <= 2; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = -2; k <= 2; k++) {
                    if (x + i >= 0 && x + i < 16 && y + j + offset < 255 && z + k >=0 && z + k < 16 && m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) == EMPTY) {
                            if (isAether && tree[j][i + 2][k + 2] == LEAVES) {
                                m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) = id == 1 ? LIGHT_FLOWER_LEAVES : MID_FLOWER_LEAVES;
                            } else {
                                m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) = tree[j][i + 2][k + 2];

                            }
                    }
                }
            }
        }
    } else if (id == 2){
        for (int i = 0; i <= (x + minX) % 2 + 1; i++) {
            if (x >= 0 && x < 16 && y < 255 && y+i < 255 && z >=0 && z < 16) {
                m_blocks.at(x + 16 * (y + i) + 16 * 256 * (z)) = (isAether ? SPRUCELOG : DARK_WOOD);
            }
        }
        int offset = (x + minX) % 2 + 1 + 1;
        for (int i = -3; i <= 3; i++) {
            for (int j = 0; j < 8; j++) {
                for (int k = -3; k <= 3; k++) {
                    if (x + i >= 0 && x + i < 16 && y + j + offset < 255 && z + k >=0 && z + k < 16 && m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) == EMPTY) {
                            m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) = spruceTree[j][i + 3][k + 3] == LEAVES ? (isAether ? DARK_FLOWER_LEAVES : LEAVES) : spruceTree[j][i + 3][k + 3];
                    }
                }
            }
        }
    } else {
        for (int i = 0; i <= (((x + minX) % 10 <= 5) ? 0 : (x + minX) % 10); i++) {
            if (x >= 0 && x < 16 && y + i < 255 && y+i < 255 && z >=0 && z < 16) {
                m_blocks.at(x + 16 * (y + i) + 16 * 256 * (z)) = JUNGLELOG;
            }
        }
        int offset = (((x + minX) % 10 <= 5) ? 0 : (x + minX) % 10) + 1;
        for (int i = -2; i <= 2; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = -2; k <= 2; k++) {
                    if (x + i >= 0 && x + i < 16 && y + j + offset < 255 && z + k >=0 && z + k < 16 && m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) == EMPTY) {
                            m_blocks.at(x + i + 16 * (y + j + offset) + 16 * 256 * (z + k)) =  tree[j][i + 2][k + 2] == LEAVES ? (isAether ? FANCY_ICE :JUNGLELEAVES) : tree[j][i+2][k+2];
                    }
                }
            }
        }
    }
}

// Does bounds checking with at()
BlockType Chunk::getBlockAt(unsigned int x, unsigned int y, unsigned int z) const {\
        return m_blocks.at(x + 16 * y + 16 * 256 * z);
}


// Exists to get rid of compiler warnings about int -> unsigned int implicit conversion
BlockType Chunk::getBlockAt(int x, int y, int z) const {
    return getBlockAt(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z));
}

int Chunk::getLightLevelAt(int x, int y, int z) const {
    return static_cast<int>(m_lightLevels.at(x + 16 * y + 16 * 256 * z));
}

// Does bounds checking with at()
void Chunk::setBlockAt(unsigned int x, unsigned int y, unsigned int z, BlockType t) {
    m_blocks.at(x + 16 * y + 16 * 256 * z) = t;
}

void Chunk::setLightLevelAt(unsigned int x, unsigned int y, unsigned int z, int t) {
    m_lightLevels.at(x + 16 * y + 16 * 256 * z) = t;
}

void Chunk::removeBlockAt(unsigned int x, unsigned int y, unsigned int z) {
    m_blocks.at(x + 16 * y + 16 * 256 * z) = EMPTY;
}

const static std::unordered_map<Direction, Direction, EnumHash> oppositeDirection {
    {XPOS, XNEG},
    {XNEG, XPOS},
    {YPOS, YNEG},
    {YNEG, YPOS},
    {ZPOS, ZNEG},
    {ZNEG, ZPOS}
};

const static std::array<glm::vec4, 4> xPOS = {glm::vec4(1, 0, 0, 1),
                                              glm::vec4(1, 1, 0, 1),
                                              glm::vec4(1, 1, 1, 1),
                                              glm::vec4(1, 0, 1, 1)};
const static std::array<glm::vec4, 4> xNEG = {glm::vec4(0, 0, 0, 1),
                                              glm::vec4(0, 0, 1, 1),
                                              glm::vec4(0, 1, 1, 1),
                                              glm::vec4(0, 1, 0, 1)};
const static std::array<glm::vec4, 4> yPOS= {glm::vec4(0, 1, 0, 1),
                                              glm::vec4(0, 1, 1, 1),
                                              glm::vec4(1, 1, 1, 1),
                                              glm::vec4(1, 1, 0, 1)};
const static std::array<glm::vec4, 4> yNEG= {glm::vec4(0, 0, 0, 1),
                                              glm::vec4(0, 0, 1, 1),
                                              glm::vec4(1, 0, 1, 1),
                                              glm::vec4(1, 0, 0, 1)};
const static std::array<glm::vec4, 4> zPOS= {glm::vec4(1, 0, 1, 1),
                                              glm::vec4(1, 1, 1, 1),
                                              glm::vec4(0, 1, 1, 1),
                                              glm::vec4(0, 0, 1, 1)};
const static std::array<glm::vec4, 4> zNEG= {glm::vec4(1, 0, 0, 1),
                                              glm::vec4(1, 1, 0, 1),
                                              glm::vec4(0, 1, 0, 1),
                                              glm::vec4(0, 0, 0, 1)};


const static std::unordered_map<Direction, std::array<glm::vec4, 4>, EnumHash> dirPos {
    {XPOS, xPOS}, {XNEG, xNEG},
    {YPOS, yPOS}, {YNEG, yNEG},
    {ZPOS, zPOS}, {ZNEG, zNEG}
};

const static std::unordered_map<Direction, glm::vec4, EnumHash> dirNor {
                                                                       {XPOS, glm::vec4(1,0,0,1)},
                                                                       {XNEG, glm::vec4(-1,0,0,1)},
                                                                       {YPOS, glm::vec4(0,1,0,1)},
                                                                       {YNEG, glm::vec4(0,-1,0,1)},
                                                                       {ZPOS, glm::vec4(0,0,1,1)},
                                                                       {ZNEG, glm::vec4(0,0,-1,1)},
                                                                       };

const static std::vector<glm::vec2> uvBase {
    glm::vec2(0, 0),
    glm::vec2(0, 1),
    glm::vec2(1, 1),
    glm::vec2(1, 0),

    glm::vec2(0, 0),
    glm::vec2(1, 0),
    glm::vec2(1, 1),
    glm::vec2(0, 1)
};

const static std::unordered_map<BlockType, std::unordered_map<Direction, glm::vec2, EnumHash>, EnumHash> uvTexture {
    {GRASS, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.8125f)},
                                                               {YNEG, glm::vec2(0.125f, 0.9375f)},
                                                               {XPOS, glm::vec2(0.1875f, 0.9375f)},
                                                               {XNEG, glm::vec2(0.1875f, 0.9375f)},
                                                               {ZPOS, glm::vec2(0.1875f, 0.9375f)},
                                                               {ZNEG, glm::vec2(0.1875f, 0.9375f)}}},

    {STONE, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.0625f, 0.9375f)},
                                                               {YNEG, glm::vec2(0.0625f, 0.9375f)},
                                                               {XPOS, glm::vec2(0.0625f, 0.9375f)},
                                                               {XNEG, glm::vec2(0.0625f, 0.9375f)},
                                                               {ZPOS, glm::vec2(0.0625f, 0.9375f)},
                                                               {ZNEG, glm::vec2(0.0625f, 0.9375f)}}},

    {DIRT, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.125f, 0.9375f)},
                                                               {YNEG, glm::vec2(0.125f, 0.9375f)},
                                                               {XPOS, glm::vec2(0.125f, 0.9375f)},
                                                               {XNEG, glm::vec2(0.125f, 0.9375f)},
                                                               {ZPOS, glm::vec2(0.125f, 0.9375f)},
                                                               {ZNEG, glm::vec2(0.125f, 0.9375f)}}},

    {SNOW, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.125f, 0.6875f)},
                                                              {YNEG, glm::vec2(0.125f, 0.6875f)},
                                                              {XPOS, glm::vec2(0.125f, 0.6875f)},
                                                              {XNEG, glm::vec2(0.125f, 0.6875f)},
                                                              {ZPOS, glm::vec2(0.125f, 0.6875f)},
                                                              {ZNEG, glm::vec2(0.125f, 0.6875f)}}},

    {WATER, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.8125f, 0.1875f)},
                                                              {YNEG, glm::vec2(0.8125f, 0.1875f)},
                                                              {XPOS, glm::vec2(0.8125f, 0.1875f)},
                                                              {XNEG, glm::vec2(0.8125f, 0.1875f)},
                                                              {ZPOS, glm::vec2(0.8125f, 0.1875f)},
                                                              {ZNEG, glm::vec2(0.8125f, 0.1875f)}}},

    {LAVA, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.8125f, 0.0625f)},
                                                               {YNEG, glm::vec2(0.8125f, 0.0625f)},
                                                               {XPOS, glm::vec2(0.8125f, 0.0625f)},
                                                               {XNEG, glm::vec2(0.8125f, 0.0625f)},
                                                               {ZPOS, glm::vec2(0.8125f, 0.0625f)},
                                                               {ZNEG, glm::vec2(0.8125f, 0.0625f)}}},

    {SAND, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.125f, 0.875f)},
                                                              {YNEG, glm::vec2(0.125f, 0.875f)},
                                                              {XPOS, glm::vec2(0.125f, 0.875f)},
                                                              {XNEG, glm::vec2(0.125f, 0.875f)},
                                                              {ZPOS, glm::vec2(0.125f, 0.875f)},
                                                              {ZNEG, glm::vec2(0.125f, 0.875f)}}},

    {CLAY, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.6875f)},
                                                              {YNEG, glm::vec2(0.5f, 0.6875f)},
                                                              {XPOS, glm::vec2(0.5f, 0.6875f)},
                                                              {XNEG, glm::vec2(0.5f, 0.6875f)},
                                                              {ZPOS, glm::vec2(0.5f, 0.6875f)},
                                                              {ZNEG, glm::vec2(0.5f, 0.6875f)}}},

    {OAKLOG, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.25f, 0.875f)},
                                                              {YNEG, glm::vec2(0.25f, 0.875f)},
                                                              {XPOS, glm::vec2(0.25f, 0.875f)},
                                                              {XNEG, glm::vec2(0.25f, 0.875f)},
                                                              {ZPOS, glm::vec2(0.25f, 0.875f)},
                                                              {ZNEG, glm::vec2(0.25f, 0.875f)}}},

    {BIRCHLOG, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.3125f, 0.5f)},
                                                              {YNEG, glm::vec2(0.3125f, 0.5f)},
                                                              {XPOS, glm::vec2(0.3125f, 0.5f)},
                                                              {XNEG, glm::vec2(0.3125f, 0.5f)},
                                                              {ZPOS, glm::vec2(0.3125f, 0.5f)},
                                                              {ZNEG, glm::vec2(0.3125f, 0.5f)}}},

    {SPRUCELOG, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.5f)},
                                                              {YNEG, glm::vec2(0.25f, 0.5f)},
                                                              {XPOS, glm::vec2(0.25f, 0.5f)},
                                                              {XNEG, glm::vec2(0.25f, 0.5f)},
                                                              {ZPOS, glm::vec2(0.25f, 0.5f)},
                                                              {ZNEG, glm::vec2(0.25f, 0.5f)}}},

    {JUNGLELOG, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.375f, 0.375f)},
                                                              {YNEG, glm::vec2(0.5625f, 0.375f)},
                                                              {XPOS, glm::vec2(0.5625f, 0.375f)},
                                                              {XNEG, glm::vec2(0.5625f, 0.375f)},
                                                              {ZPOS, glm::vec2(0.5625f, 0.375f)},
                                                              {ZNEG, glm::vec2(0.5625f, 0.375f)}}},

    {LEAVES, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.25f, 0.75f)},
                                                              {YNEG, glm::vec2(0.25f, 0.75f)},
                                                              {XPOS, glm::vec2(0.25f, 0.75f)},
                                                              {XNEG, glm::vec2(0.25f, 0.75f)},
                                                              {ZPOS, glm::vec2(0.25f, 0.75f)},
                                                              {ZNEG, glm::vec2(0.25f, 0.75f)}}},

    {JUNGLELEAVES, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.25f, 0.75f)},
                                                                {YNEG, glm::vec2(0.25f, 0.75f)},
                                                                {XPOS, glm::vec2(0.25f, 0.75f)},
                                                                {XNEG, glm::vec2(0.25f, 0.75f)},
                                                                {ZPOS, glm::vec2(0.25f, 0.75f)},
                                                                {ZNEG, glm::vec2(0.25f, 0.75f)}}},

    {BEDROCK, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.0625f, 0.875f)},
                                                                 {YNEG, glm::vec2(0.0625f, 0.875f)},
                                                                 {XPOS, glm::vec2(0.0625f, 0.875f)},
                                                                 {XNEG, glm::vec2(0.0625f, 0.875f)},
                                                                 {ZPOS, glm::vec2(0.0625f, 0.875f)},
                                                                 {ZNEG, glm::vec2(0.0625f, 0.875f)}}},

    {GLOW, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5625f, 0.5625f)},
                                                                 {YNEG, glm::vec2(0.5625f, 0.5625f)},
                                                                 {XPOS, glm::vec2(0.5625f, 0.5625f)},
                                                                 {XNEG, glm::vec2(0.5625f, 0.5625f)},
                                                                 {ZPOS, glm::vec2(0.5625f, 0.5625f)},
                                                                 {ZNEG, glm::vec2(0.5625f, 0.5625f)}}},

    {RED_MUSH, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.75f, 0.875f)},
                                                                 {YNEG, glm::vec2(0.75f, 0.875f)},
                                                                 {XPOS, glm::vec2(0.75f, 0.875f)},
                                                                 {XNEG, glm::vec2(0.75f, 0.875f)},
                                                                 {ZPOS, glm::vec2(0.75f, 0.875f)},
                                                                 {ZNEG, glm::vec2(0.75f, 0.875f)}}},

    {BROWN_MUSH, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.8125f, 0.875f)},
                                                                 {YNEG, glm::vec2(0.8125f, 0.875f)},
                                                                 {XPOS, glm::vec2(0.8125f, 0.875f)},
                                                                 {XNEG, glm::vec2(0.8125f, 0.875f)},
                                                                 {ZPOS, glm::vec2(0.8125f, 0.875f)},
                                                                 {ZNEG, glm::vec2(0.8125f, 0.875f)}}},

    {FLOWERR, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.75f, 0.9375f)},
                                                              {YNEG, glm::vec2(0.75f, 0.9375f)},
                                                              {XPOS, glm::vec2(0.75f, 0.9375f)},
                                                              {XNEG, glm::vec2(0.75f, 0.9375f)},
                                                              {ZPOS, glm::vec2(0.75f, 0.9375f)},
                                                              {ZNEG, glm::vec2(0.75f, 0.9375f)}}},

    {FLOWERY, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.8125f, 0.9375f)},
                                                                 {YNEG, glm::vec2(0.8125f, 0.9375f)},
                                                                 {XPOS, glm::vec2(0.8125f, 0.9375f)},
                                                                 {XNEG, glm::vec2(0.8125f, 0.9375f)},
                                                                 {ZPOS, glm::vec2(0.8125f, 0.9375f)},
                                                                 {ZNEG, glm::vec2(0.8125f, 0.9375f)}}},

    {TALL_GRASS, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.4375f, 0.8125f)},
                                                                 {YNEG, glm::vec2(0.4375f, 0.8125f)},
                                                                 {XPOS, glm::vec2(0.4375f, 0.8125f)},
                                                                 {XNEG, glm::vec2(0.4375f, 0.8125f)},
                                                                 {ZPOS, glm::vec2(0.4375f, 0.8125f)},
                                                                 {ZNEG, glm::vec2(0.4375f, 0.8125f)}}},

    {FLOWERW, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.9375f, 0.25f)},
                                                                    {YNEG, glm::vec2(0.9375f, 0.25f)},
                                                                    {XPOS, glm::vec2(0.9375f, 0.25f)},
                                                                    {XNEG, glm::vec2(0.9375f, 0.25f)},
                                                                    {ZPOS, glm::vec2(0.9375f, 0.25f)},
                                                                    {ZNEG, glm::vec2(0.9375f, 0.25f)}}},

    {FLOWERB, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.875f, 0.25f)},
                                                                 {YNEG, glm::vec2(0.875f, 0.25f)},
                                                                 {XPOS, glm::vec2(0.875f, 0.25f)},
                                                                 {XNEG, glm::vec2(0.875f, 0.25f)},
                                                                 {ZPOS, glm::vec2(0.875f, 0.25f)},
                                                                 {ZNEG, glm::vec2(0.875f, 0.25f)}}},

    {FLOWERP, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.8125f, 0.25f)},
                                                                 {YNEG, glm::vec2(0.8125f, 0.25f)},
                                                                 {XPOS, glm::vec2(0.8125f, 0.25f)},
                                                                 {XNEG, glm::vec2(0.8125f, 0.25f)},
                                                                 {ZPOS, glm::vec2(0.8125f, 0.25f)},
                                                                 {ZNEG, glm::vec2(0.8125f, 0.25f)}}},

    {CORALY, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.75f, 0.0625f)},
                                                                 {YNEG, glm::vec2(0.75f, 0.0625f)},
                                                                 {XPOS, glm::vec2(0.75f, 0.0625f)},
                                                                 {XNEG, glm::vec2(0.75f, 0.0625f)},
                                                                 {ZPOS, glm::vec2(0.75f, 0.0625f)},
                                                                 {ZNEG, glm::vec2(0.75f, 0.0625f)}}},

    {CORALR, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.6875f, 0.0625f)},
                                                                {YNEG, glm::vec2(0.6875f, 0.0625f)},
                                                                {XPOS, glm::vec2(0.6875f, 0.0625f)},
                                                                {XNEG, glm::vec2(0.6875f, 0.0625f)},
                                                                {ZPOS, glm::vec2(0.6875f, 0.0625f)},
                                                                {ZNEG, glm::vec2(0.6875f, 0.0625f)}}},

    {CORALM, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.625f, 0.0625f)},
                                                                {YNEG, glm::vec2(0.625f, 0.0625f)},
                                                                {XPOS, glm::vec2(0.625f, 0.0625f)},
                                                                {XNEG, glm::vec2(0.625f, 0.0625f)},
                                                                {ZPOS, glm::vec2(0.625f, 0.0625f)},
                                                                {ZNEG, glm::vec2(0.625f, 0.0625f)}}},

    {CORALP, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5625f, 0.0625f)},
                                                                {YNEG, glm::vec2(0.5625f, 0.0625f)},
                                                                {XPOS, glm::vec2(0.5625f, 0.0625f)},
                                                                {XNEG, glm::vec2(0.5625f, 0.0625f)},
                                                                {ZPOS, glm::vec2(0.5625f, 0.0625f)},
                                                                {ZNEG, glm::vec2(0.5625f, 0.0625f)}}},

    {CORALO, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.625f, 0.125f)},
                                                                {YNEG, glm::vec2(0.625f, 0.125f)},
                                                                {XPOS, glm::vec2(0.625f, 0.125f)},
                                                                {XNEG, glm::vec2(0.625f, 0.125f)},
                                                                {ZPOS, glm::vec2(0.625f, 0.125f)},
                                                                {ZNEG, glm::vec2(0.625f, 0.125f)}}},

    {CORALB, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5625f, 0.0625f)},
                                                                {YNEG, glm::vec2(0.5625f, 0.0625f)},
                                                                {XPOS, glm::vec2(0.5625f, 0.0625f)},
                                                                {XNEG, glm::vec2(0.5625f, 0.0625f)},
                                                                {ZPOS, glm::vec2(0.5625f, 0.0625f)},
                                                                {ZNEG, glm::vec2(0.5625f, 0.0625f)}}},

    {PURPGRASS, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.75f, 0.1875f)},
                                                                {YNEG, glm::vec2(0.125f, 0.9375f)},
                                                                {XPOS, glm::vec2(0.6875f, 0.1875f)},
                                                                {XNEG, glm::vec2(0.6875f, 0.1875f)},
                                                                {ZPOS, glm::vec2(0.6875f, 0.1875f)},
                                                                {ZNEG, glm::vec2(0.6875f, 0.1875f)}}},

    {LIGHT_WOOD, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.6875, 0.3125f)},
                                                                  {YNEG, glm::vec2(0.6875, 0.3125f)},
                                                                  {XPOS, glm::vec2(0.625f, 0.3125f)},
                                                                  {XNEG, glm::vec2(0.625f, 0.3125f)},
                                                                  {ZPOS, glm::vec2(0.625f, 0.3125f)},
                                                                  {ZNEG, glm::vec2(0.625f, 0.3125f)}}},

    {MID_WOOD, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5625f, 0.1875f)},
                                                                   {YNEG, glm::vec2(0.5625f, 0.1875f)},
                                                                   {XPOS, glm::vec2(0.5625f, 0.125f)},
                                                                   {XNEG, glm::vec2(0.5625f, 0.125f)},
                                                                   {ZPOS, glm::vec2(0.5625f, 0.125f)},
                                                                   {ZNEG, glm::vec2(0.5625f, 0.125f)}}},

    {DARK_WOOD, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5625f, 0.25f)},
                                                                  {YNEG, glm::vec2(0.5625f, 0.25f)},
                                                                  {XPOS, glm::vec2(0.5625f, 0.3125f)},
                                                                  {XNEG, glm::vec2(0.5625f, 0.3125f)},
                                                                  {ZPOS, glm::vec2(0.5625f, 0.3125f)},
                                                                  {ZNEG, glm::vec2(0.5625f, 0.3125f)}}},

    {FANCY_ICE, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.75f, 0.3125f)},
                                                                   {YNEG, glm::vec2(0.75f, 0.3125f)},
                                                                   {XPOS, glm::vec2(0.8125f, 0.3125f)},
                                                                   {XNEG, glm::vec2(0.8125f, 0.3125f)},
                                                                   {ZPOS, glm::vec2(0.8125f, 0.3125f)},
                                                                   {ZNEG, glm::vec2(0.8125f, 0.3125f)}}},

    {LIGHT_SAND, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.6875f, 0.25f)},
                                                                 {YNEG, glm::vec2(0.6875f, 0.25f)},
                                                                 {XPOS, glm::vec2(0.6875f, 0.25f)},
                                                                 {XNEG, glm::vec2(0.6875f, 0.25f)},
                                                                 {ZPOS, glm::vec2(0.6875f, 0.25f)},
                                                                 {ZNEG, glm::vec2(0.6875f, 0.25f)}}},

    {DARK_SAND, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.625f, 0.25f)},
                                                                    {YNEG, glm::vec2(0.625f, 0.25f)},
                                                                    {XPOS, glm::vec2(0.625f, 0.25f)},
                                                                    {XNEG, glm::vec2(0.625f, 0.25f)},
                                                                    {ZPOS, glm::vec2(0.625f, 0.25f)},
                                                                    {ZNEG, glm::vec2(0.625f, 0.25f)}}},

    {DARK_FLOWER_LEAVES, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.3125f)},
                                                                    {YNEG, glm::vec2(0.5f, 0.3125f)},
                                                                    {XPOS, glm::vec2(0.5f, 0.3125f)},
                                                                    {XNEG, glm::vec2(0.5f, 0.3125f)},
                                                                    {ZPOS, glm::vec2(0.5f, 0.3125f)},
                                                                    {ZNEG, glm::vec2(0.5f, 0.3125f)}}},

    {MID_FLOWER_LEAVES, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.25f)},
                                                                            {YNEG, glm::vec2(0.5f, 0.25f)},
                                                                            {XPOS, glm::vec2(0.5f, 0.25f)},
                                                                            {XNEG, glm::vec2(0.5f, 0.25f)},
                                                                            {ZPOS, glm::vec2(0.5f, 0.25f)},
                                                                            {ZNEG, glm::vec2(0.5f, 0.25f)}}},

    {LIGHT_FLOWER_LEAVES, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.1875f)},
                                                                           {YNEG, glm::vec2(0.5f, 0.1875f)},
                                                                           {XPOS, glm::vec2(0.5f, 0.1875f)},
                                                                           {XNEG, glm::vec2(0.5f, 0.1875f)},
                                                                           {ZPOS, glm::vec2(0.5f, 0.1875f)},
                                                                           {ZNEG, glm::vec2(0.5f, 0.1875f)}}},

    {ROCKS, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.75f, 0.25f)},
                                                                    {YNEG, glm::vec2(0.75f, 0.25f)},
                                                                    {XPOS, glm::vec2(0.75f, 0.25f)},
                                                                    {XNEG, glm::vec2(0.75f, 0.25f)},
                                                                    {ZPOS, glm::vec2(0.75f, 0.25f)},
                                                                    {ZNEG, glm::vec2(0.75f, 0.25f)}}},

    {AMETHYST, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5625f, 0.125f)},
                                                                {YNEG, glm::vec2(0.5625f, 0.125f)},
                                                                {XPOS, glm::vec2(0.5625f, 0.125f)},
                                                                {XNEG, glm::vec2(0.5625f, 0.125f)},
                                                                {ZPOS, glm::vec2(0.5625f, 0.125f)},
                                                                {ZNEG, glm::vec2(0.5625f, 0.125f)}}},

    {ROCK_GRASS, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.5f, 0.8125f)},
                                                                  {YNEG, glm::vec2(0.4375f, 0.0625f)},
                                                                  {XPOS, glm::vec2(0.4375f, 0.125f)},
                                                                  {XNEG, glm::vec2(0.4375f, 0.125f)},
                                                                  {ZPOS, glm::vec2(0.4375f, 0.125f)},
                                                                  {ZNEG, glm::vec2(0.4375f, 0.125f)}}},
    {ROCK_DIRT, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.4375f, 0.0625f)},
                                                                    {YNEG, glm::vec2(0.4375f, 0.0625f)},
                                                                    {XPOS, glm::vec2(0.4375f, 0.0625f)},
                                                                    {XNEG, glm::vec2(0.4375f, 0.0625f)},
                                                                    {ZPOS, glm::vec2(0.4375f, 0.0625f)},
                                                                    {ZNEG, glm::vec2(0.4375f, 0.0625f)}}},

    {FLOWER_GRASS, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.625, 0.1875f)},
                                                                    {YNEG, glm::vec2(0.4375f, 0.0625f)},
                                                                    {XPOS, glm::vec2(0.4375f, 0.125f)},
                                                                    {XNEG, glm::vec2(0.4375f, 0.125f)},
                                                                    {ZPOS, glm::vec2(0.4375f, 0.125f)},
                                                                    {ZNEG, glm::vec2(0.4375f, 0.125f)}}},

    {REG_ICE, std::unordered_map<Direction, glm::vec2, EnumHash>{{YPOS, glm::vec2(0.8125f, 0.125f)},
                                                                      {YNEG, glm::vec2(0.8125f, 0.125f)},
                                                                      {XPOS, glm::vec2(0.8125f, 0.125f)},
                                                                      {XNEG, glm::vec2(0.8125f, 0.125f)},
                                                                      {ZPOS, glm::vec2(0.8125f, 0.125f)},
                                                                      {ZNEG, glm::vec2(0.8125f, 0.125f)}}}
};

//VECTOR OF FLOWER / GRASS OBJECTS TO BE SCATTERED AROUND ! ! UPDATE THIS WHEN ADDING NEW TYPES ! !
const static std::vector<BlockType> flowers = {TALL_GRASS, FLOWERR, FLOWERY, FLOWERW, FLOWERB, FLOWERP,
                                               RED_MUSH, BROWN_MUSH};

static bool isFlower(BlockType bt) {
    return std::find(flowers.begin(), flowers.end(), bt) != flowers.end();
}

struct AdjacentCell {
    Direction dir;
    glm::vec4 nor;
    std::array<glm::vec4, 4> pos;

    AdjacentCell(Direction d, glm::vec4 n, std::array<glm::vec4, 4> pos)
        : dir(d), nor(n), pos(pos)
    {}
};

const static std::array<AdjacentCell, 6> adjacentCells {
    AdjacentCell(XPOS, glm::vec4(1,0,0,0), xPOS),
    AdjacentCell(XNEG, glm::vec4(-1,0,0,0), xNEG),
    AdjacentCell(YPOS, glm::vec4(0,1,0,0), yPOS),
    AdjacentCell(YNEG, glm::vec4(0,-1,0,0), yNEG),
    AdjacentCell(ZPOS, glm::vec4(0,0,1,0), zPOS),
    AdjacentCell(ZNEG, glm::vec4(0,0,-1,0), zNEG)
};

//RETURNS A COLOR VECTOR BASED ON THE CURRENT BLOCK TYPE
static glm::vec4 getBlockColor(BlockType b) {
    if (b == GRASS) {
        return glm::vec4 (0,1,0,1); //GREEN :(
    } else  if (b == DIRT) {
        return glm::vec4(0.2,0.2,0,1); //BROWN ?
    } else if (b == STONE) {
        return glm::vec4(0.4, 0.4, 0.5, 1); //GRAY >8E
    } else if (b == WATER) {
        return glm::vec4(0.1, 0.1, 1, 1); //BLUE ^v^v^
    } else {
        //BLANK PINK COLOR IF BLOCK TYPE IS EMPTY
        //THIS COLOR SHOULD NEVER BE SEEN IF create() CODE CORRECTLY DOESN"T BUFFER EMPTY BLOCK DATA
        return glm::vec4(1,0,1,1);
    }
}

#if 1 //OLD VERSION BEFORE MULTI
//PUSHES VBO DATA FOR THE GIVEN FACE
static void vboBlockFace(Direction d, int x, int y, int z, glm::vec4 c, std::vector<glm::vec4> &chunk,
                         std::vector<GLuint> &idx, int &index, BlockType bt) {
    glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(x, y, z));
    std::unordered_map<Direction, glm::vec2, EnumHash> uvs = uvTexture.at(bt);
    for (int i = 0; i < dirPos.at(d).size(); i++) {
        glm::vec4 v = dirPos.at(d)[i];
        chunk.push_back(translate * v); //PUSHES BACK THE VERTEX POSITION
        chunk.push_back(dirNor.at(d)); //PUSHES BACK THE VERTEX NORMAL
        int magic = i;
        if (d == XNEG) {
            magic += 4;
        }
        if (bt == WATER || bt == LAVA) {
            chunk.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 1, 0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
        } else {
            chunk.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 0, 0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
        }
    }
    int curIdx = index + 1;
    //PUSHES BACK THE TRIANGLE FAN INDECIES
    idx.push_back(index);
    idx.push_back(curIdx);
    idx.push_back(curIdx + 1);
    curIdx++;
    idx.push_back(index);
    idx.push_back(curIdx);
    idx.push_back(curIdx + 1);
    index += 4;
}
#endif

//PUSHES OPAQUE VBO DATA FOR THE GIVEN FACE
void Chunk::vboBlockFace(Direction d, int x, int y, int z, glm::ivec2 chunkMins, ChunkVBOData &ch, int &index, BlockType bt, float temperature, float humidity, int lightlevel) {
    //x, y, and z are the WORLD SPACE coords of the current block
    //DO x - xMin and z - zMin to get CHUNK SPACE coords of the current block

    //AMBIENT OCCLUTION STUFF:
    //convert to chunk space
    int amX = x - minX;
    int amY = y;
    int amZ = z - minZ;
    if (d == XPOS || d == XNEG) {
        amX += dirNor.at(d)[0];
    } else if (d == YPOS || d == YNEG) {
        amY += dirNor.at(d)[1];
    } else {
        amZ += dirNor.at(d)[2];
    }

    int i = (amX) % 16;
    int k = (amZ) % 16;

    // Makes negative modulos wrap around, e.g. -1 -> 15 for x and z
    i = i < 0 ? 16 + i : i;
    k = k < 0 ? 16 + k : k;

    Chunk* neighbor = this;
    if (amX < 0 || amX > 15 || amZ < 0 || amZ > 15) {
        neighbor = m_neighbors[d];
        amX = i;
        amZ = k;
    }

    glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(x, y, z));
    std::unordered_map<Direction, glm::vec2, EnumHash> uvs = uvTexture.at(bt);
    int numOcl = 0;
    for (int i = 0; i < dirPos.at(d).size(); i++) {
        numOcl = 0;

        if (amY <= 255 && amY >= 0 ) { // && amX < 15 && amX > 0 && amZ < 15 && amZ > 0) {
                //WRITE A HELPER FUNCTION FOR THIS PART THAT TAKES IN TWO INTS AND A REFERENCE TO A THIRD
            if (d == YPOS || d == YNEG) {
                //THIS HELPER ONLY WORKS IN THIS CASE REDO IF SO IT WORKS FOR ALL
                std::pair<int, int> bounds1(0, 1);
                std::pair<int, int> bounds2(0, 1);
                std::pair<int, int> bounds3(0, 3);
                findOcVal(i, amX, bounds1, amY, bounds2, amZ, bounds3, 2, numOcl, neighbor);
            } else if (d == ZPOS || d == ZNEG) {
                std::pair<int, int> bounds1(2, 3);
                std::pair<int, int> bounds2(0, 3);
                std::pair<int, int> bounds3(0, 3);
                findOcVal(i, amX, bounds1, amY, bounds2, amZ, bounds3, 3, numOcl, neighbor);
            } else if (d == XPOS) {
                std::pair<int, int> bounds1(0, 1);
                std::pair<int, int> bounds2(0, 3);
                std::pair<int, int> bounds3(0, 1);
                findOcVal(i, amX, bounds1, amY, bounds2, amZ, bounds3, 1, numOcl, neighbor);
            } else if (d == XNEG) {
                std::pair<int, int> bounds1(0, 1);
                std::pair<int, int> bounds2(0, 1);
                std::pair<int, int> bounds3(0, 3);
                findOcVal(i, amX, bounds1, amY, bounds2, amZ, bounds3, 1, numOcl, neighbor);
            }
        }

        glm::vec4 v = dirPos.at(d)[i];
        ch.m_vboOpaque.push_back(translate * v); //PUSHES BACK THE VERTEX POSITION
        ch.m_vboOpaque.push_back(dirNor.at(d)); //PUSHES BACK THE VERTEX NORMAL
        //ch.m_vboOpaque.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[i][0] * 0.0625), uvs.at(d)[1] + (uvBase[i][1] * 0.0625), 0, 0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
        int magic = i;
        if (d == XNEG) {
            magic += 4;
        }
        if (bt == WATER || bt == LAVA) {
            ch.m_vboOpaque.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 1, numOcl)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
        } else if (bt == GRASS || bt == LEAVES || bt == TALL_GRASS) {
            ch.m_vboOpaque.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), -1.f * glm::floor(temperature * 255.0) + glm::clamp(humidity + 0.01, 0.0, 0.99), numOcl)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
        } else {
            ch.m_vboOpaque.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 0, numOcl)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
        }
        ch.m_vboOpaque.push_back(glm::vec4(static_cast<float>(lightlevel)));
    }

    int curIdx = index + 1;
    //PUSHES BACK THE TRIANGLE FAN INDECIES

    ch.m_idxOpaque.push_back(index);
    ch.m_idxOpaque.push_back(curIdx);
    ch.m_idxOpaque.push_back(curIdx + 1);
    curIdx++;
    ch.m_idxOpaque.push_back(index);
    ch.m_idxOpaque.push_back(curIdx);
    ch.m_idxOpaque.push_back(curIdx + 1);
    index += 4;
}

void Chunk::findOcVal(int i, int am1, std::pair<int, int> bounds1, int am2, std::pair<int, int> bounds2, int am3, std::pair<int, int> bounds3,
                      int magic, int& numOcl, Chunk* c) {
    std::pair<Direction, Direction> chunkDir;
    Chunk* xChunk = c;
    Chunk* zChunk = c;
    Chunk* cornerChunk = c;
    glm::vec3 curBlockCheck = glm::vec3();

    //SOMETHING WITH X IS GETTING MESSED UP ON CHUNK BOARDERS

    //CURRENT HYPOTHESIS: Consider two side by side chunks: A B
    //We are looking at the X border between A and B. If chunk B is loaded before chunk A, then A will do occlusion on the side
    //between them, but B will have the wrong curBlockCheck value and thus it won't be good

    //BECUASE NO NEIGHBOR CHUNK, xChunk and zChunk will use c, which expails seemingly "random" pattern
    if (i == bounds1.first || i == bounds1.second) {
        if (am1 - 1 < 0) {
            curBlockCheck[0] = 15;
            if (magic != 1) {
                xChunk = c->m_neighbors[XNEG];
            }
            chunkDir.first = XNEG;
        } else {
            curBlockCheck[0] = am1 - 1;
        }
    } else {
        if (am1 + 1 > 15) {
            curBlockCheck[0] = 0;
            if (magic != 1) {
                xChunk = c->m_neighbors[XPOS];
            }
            chunkDir.first = XPOS;
        } else {
            curBlockCheck[0] = am1 + 1;
        }
    }

    if (i == bounds2.first || i == bounds2.second) {
        if (am2 - 1 < 0) {
            curBlockCheck[1] = am2;
        } else {
            curBlockCheck[1] = am2 - 1;
        }
    } else {
        if (am2 + 1 > 255) {
            curBlockCheck[1] = am2;
        } else {
            curBlockCheck[1] = am2 + 1;
        }
    }

    if (i == bounds3.first || i == bounds3.second) {
        if (am3 - 1 < 0) {
            curBlockCheck[2] = 15;
            if (magic != 3) {
                zChunk = c->m_neighbors[ZNEG];
            }
            chunkDir.second = ZNEG;
        } else {
            curBlockCheck[2] = am3 - 1;
        }
    } else {
        if (am3 + 1 > 15) {
            curBlockCheck[2] = 0;
            if (magic != 3) {
                zChunk = c->m_neighbors[ZPOS];
            }
            chunkDir.second = ZPOS;
        } else {
            curBlockCheck[2] = am3 + 1;
        }
    }

    //BREAKS AT CHUNK BORdERS STILl

    if (xChunk != c && zChunk == c) {
        cornerChunk = xChunk;
    } else if (xChunk == c && zChunk != c) {
        cornerChunk = zChunk;
    } else if (xChunk != c && zChunk != c) {
        if (c->m_neighbors[chunkDir.first] && c->m_neighbors[chunkDir.first]->m_neighbors[chunkDir.second]) {
            cornerChunk = c->m_neighbors[chunkDir.first]->m_neighbors[chunkDir.second];
        }
    }

    //IM SORRY ADAM ;-;
    if (magic == 1) {
        if (cornerChunk && cornerChunk->getBlockAt(am1, curBlockCheck[1], curBlockCheck[2]) != EMPTY && cornerChunk->getBlockAt(am1, curBlockCheck[1], curBlockCheck[2]) != WATER &&
            !isFlower(cornerChunk->getBlockAt(am1, curBlockCheck[1], curBlockCheck[2]))) {
            numOcl++;
        }
        if (xChunk && xChunk->getBlockAt(am1, curBlockCheck[1], am3) != EMPTY && xChunk->getBlockAt(am1, curBlockCheck[1], am3) != WATER &&
            !isFlower(xChunk->getBlockAt(am1, curBlockCheck[1], am3))) {
            numOcl++;
        }
        if (zChunk && zChunk->getBlockAt(am1, am2, curBlockCheck[2]) != EMPTY && zChunk->getBlockAt(am1, am2, curBlockCheck[2]) != WATER &&
            !isFlower(zChunk->getBlockAt(am1, am2, curBlockCheck[2]))) {
            numOcl++;
        }
    } else if (magic == 2) {

        //corner block
        if (cornerChunk && cornerChunk->getBlockAt(curBlockCheck[0], am2, curBlockCheck[2]) != EMPTY && cornerChunk->getBlockAt(curBlockCheck[0], am2, curBlockCheck[2]) != WATER &&
            !isFlower(cornerChunk->getBlockAt(curBlockCheck[0], am2, curBlockCheck[2]))) {
            numOcl++;
        }
        //x side block
        if (xChunk && xChunk->getBlockAt(curBlockCheck[0], am2, am3) != EMPTY && xChunk->getBlockAt(curBlockCheck[0], am2, am3) != WATER &&
            !isFlower(xChunk->getBlockAt(curBlockCheck[0], am2, am3))) {
            numOcl++;
        }
        //z side block
        if (zChunk && zChunk->getBlockAt(am1, am2, curBlockCheck[2]) != EMPTY && zChunk->getBlockAt(am1, am2, curBlockCheck[2]) != WATER &&
            !isFlower(zChunk->getBlockAt(am1, am2, curBlockCheck[2]))) {
            numOcl++;
        }

    } else if (magic == 3) {
        if (cornerChunk && cornerChunk->getBlockAt(curBlockCheck[0], curBlockCheck[1], am3) != EMPTY && cornerChunk->getBlockAt(curBlockCheck[0], curBlockCheck[1], am3) != WATER &&
            !isFlower(cornerChunk->getBlockAt(curBlockCheck[0], curBlockCheck[1], am3))) {
            numOcl++;
        }
        if (xChunk && xChunk->getBlockAt(curBlockCheck[0], am2, am3) != EMPTY && xChunk->getBlockAt(curBlockCheck[0], am2, am3) != WATER &&
            !isFlower(xChunk->getBlockAt(curBlockCheck[0], am2, am3))) {
            numOcl++;
        }
        if (zChunk && zChunk->getBlockAt(am1, curBlockCheck[1], am3) != EMPTY && zChunk->getBlockAt(am1, curBlockCheck[1], am3) != WATER &&
            !isFlower(zChunk->getBlockAt(am1, curBlockCheck[1], am3))) {
            numOcl++;
        }
    }
}

static void pushFlowerFace(glm::mat4 tMat, glm::vec4 v, ChunkVBOData &ch, int magic, Direction d, glm::vec4 nor, BlockType bt) {
    std::unordered_map<Direction, glm::vec2, EnumHash> uvs = uvTexture.at(bt);
    ch.m_vboTransparent.push_back(tMat * v);
    ch.m_vboTransparent.push_back(nor);
    ch.m_vboTransparent.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 0, 0));
}

static void vboBlockFaceTrans(Direction d, int x, int y, int z, glm::vec4 c, ChunkVBOData &ch, int &index, BlockType bt, float temperature, float humidity, bool isAether) {
    glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(x, y, z));

    std::unordered_map<Direction, glm::vec2, EnumHash> uvs = uvTexture.at(bt);
    if (isFlower(bt)) {
        //SORRY ADAM HAD TO HARD CODE THE POSITIONS :(

        glm::vec4 nor = glm::vec4(1,1,0,1);
        pushFlowerFace(translate, dirPos.at(XNEG)[0], ch, 0, d, nor, bt);
        pushFlowerFace(translate, dirPos.at(XNEG)[3], ch, 1, d, nor, bt);
        pushFlowerFace(translate, dirPos.at(XPOS)[2], ch, 2, d, nor, bt);
        pushFlowerFace(translate, dirPos.at(XPOS)[3], ch, 3, d, nor, bt);

        int curIdx = index + 1;
        ch.m_idxTransparent.push_back(index);
        ch.m_idxTransparent.push_back(curIdx);
        ch.m_idxTransparent.push_back(curIdx + 1);
        curIdx++;
        ch.m_idxTransparent.push_back(index);
        ch.m_idxTransparent.push_back(curIdx);
        ch.m_idxTransparent.push_back(curIdx + 1);
        index += 4;

        nor = glm::vec4(-1,1,0,1);
        pushFlowerFace(translate, dirPos.at(XNEG)[1], ch, 0, d, nor, bt);
        pushFlowerFace(translate, dirPos.at(XNEG)[2], ch, 1, d, nor, bt);
        pushFlowerFace(translate, dirPos.at(XPOS)[1], ch, 2, d, nor, bt);
        pushFlowerFace(translate, dirPos.at(XPOS)[0], ch, 3, d, nor, bt);

        curIdx = index + 1;
        ch.m_idxTransparent.push_back(index);
        ch.m_idxTransparent.push_back(curIdx);
        ch.m_idxTransparent.push_back(curIdx + 1);
        curIdx++;
        ch.m_idxTransparent.push_back(index);
        ch.m_idxTransparent.push_back(curIdx);
        ch.m_idxTransparent.push_back(curIdx + 1);
        index += 4;


    } else {
      for (int i = 0; i < dirPos.at(d).size(); i++) {
          glm::vec4 v = dirPos.at(d)[i];
          ch.m_vboTransparent.push_back(translate * v); //PUSHES BACK THE VERTEX POSITION
          ch.m_vboTransparent.push_back(dirNor.at(d)); //PUSHES BACK THE VERTEX NORMAL
          //ch.m_vboOpaque.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[i][0] * 0.0625), uvs.at(d)[1] + (uvBase[i][1] * 0.0625), 0, 0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
          int magic = i;
          if (d == XNEG) {
              magic += 4;
          }
          if (bt == WATER || bt == LAVA) {
              ch.m_vboTransparent.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 1, 0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
          } else if (bt == GRASS || bt == LEAVES || bt == TALL_GRASS) {
              ch.m_vboTransparent.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), -1.f * glm::floor(temperature * 255.0) + glm::clamp(humidity + 0.01, 0.0, 0.99), 1.0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
          } else {
              ch.m_vboTransparent.push_back(glm::vec4(uvs.at(d)[0] + (uvBase[magic][0] * 0.0625), uvs.at(d)[1] + (uvBase[magic][1] * 0.0625), 0, 0)); //PUSHES BACK THE VERTEX COLOR (TURNED INTO UVs)
          }
      }
        int curIdx = index + 1;
        //PUSHES BACK THE TRIANGLE FAN INDECIES

        ch.m_idxTransparent.push_back(index);
        ch.m_idxTransparent.push_back(curIdx);
        ch.m_idxTransparent.push_back(curIdx + 1);
        curIdx++;
        ch.m_idxTransparent.push_back(index);
        ch.m_idxTransparent.push_back(curIdx);
        ch.m_idxTransparent.push_back(curIdx + 1);
        index += 4;
    }
}

void Chunk::linkNeighbor(uPtr<Chunk> &neighbor, Direction dir) {
    if(neighbor != nullptr) {
        this->m_neighbors[dir] = neighbor.get();
        neighbor->m_neighbors[oppositeDirection.at(dir)] = this;
    }
}

GLenum Chunk::drawMode() {
    return GL_TRIANGLES;
}

#if 1 //OLD VERSION BEFORE MULTITRHEADING     NOW FIXED
void Chunk::createVBOdata() {
    ChunkVBOData vboData(this);
    createVBOdata(vboData);
    //BUFFERS THE IDX AND FACE VBOS
    pushChunkVBO(vboData.m_idxOpaque, vboData.m_vboOpaque, vboData.m_idxTransparent, vboData.m_vboTransparent);
}
#endif

//This is a copy of createVBOData but it populates a ChunkVBOData struct, and doesn't push to GPU
// (cause only the main thread does). Copy the contents of createVBOData if you change it into here.
void Chunk::createVBOdata(ChunkVBOData& ch) {
    int index = 0;
    int indexTrans = 0;

    for (int y = 0; y < 256; y++ ) {
        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {
                BlockType curBlock = getBlockAt(x, y, z);
                glm::vec4 curCol = getBlockColor(curBlock);
                float humidity = (curBlock == GRASS || curBlock == LEAVES) ? m_humidity.at(x + 16*z) : -1;
                float temperature = (curBlock == GRASS || curBlock == LEAVES) ? m_temperature.at(x + 16*z) : -1;
                if (curBlock == EMPTY) {
                    continue;
                } else if (curBlock == WATER || curBlock == LAVA || isFlower(curBlock)) {
                    for(auto &cell : adjacentCells) {
                        int i = (x + (int)cell.nor.x) % 16;
                        int j = (y + (int)cell.nor.y) % 256;
                        int k = (z + (int)cell.nor.z) % 16;

                        // Makes negative modulos wrap around, e.g. -1 -> 15 for x and z
                        i = i < 0 ? 16 + i : i;
                        j = j < 0 ? 256 + j : j;
                        k = k < 0 ? 16 + k : k;

                        int a = (x + (int)cell.nor.x);
                        int c = (z + (int)cell.nor.z);

                        //ijk are the xyz of the neighboring block we are cheking relative to the chunk they are in
                        //a and c are the xz of the neighboring block relative to the current chunk

                        // If not inside chunk, need to examine neighboring chunk

                        //TECHINCALLY DON"T NEED ANYTHING
                        if (a < 0 || a > 15 || c < 0 || c > 15) {
                            auto& neighbor = m_neighbors[cell.dir];
                            if (neighbor) {
                                if(m_neighbors[cell.dir]->getBlockAt(i, j, k) == EMPTY ||  isFlower(m_neighbors[cell.dir]->getBlockAt(i, j, k))) {
                                    //vboBlockFaceTrans(cell.dir, x + minX, y, z + minZ, curCol, ch, indexTrans, curBlock);
                                }
                            }
                        } else {
                            // PUSHES THE FACES
                            BlockType neighbor = getBlockAt(i, j, k);
                            if(neighbor == EMPTY || isFlower(neighbor)) {
                                vboBlockFaceTrans(cell.dir, x + minX, y, z + minZ, curCol, ch, indexTrans, curBlock, -1, -1, isAether);
                            }
                        }
                    }
                } else {
                    for(auto &cell : adjacentCells) {
                        int i = (x + (int)cell.nor.x) % 16;
                        int j = (y + (int)cell.nor.y) % 256;
                        int k = (z + (int)cell.nor.z) % 16;

                        // Makes negative modulos wrap around, e.g. -1 -> 15 for x and z
                        i = i < 0 ? 16 + i : i;
                        j = j < 0 ? 256 + j : j;
                        k = k < 0 ? 16 + k : k;

                        int a = (x + (int)cell.nor.x);
                        int c = (z + (int)cell.nor.z);
                        if (a < 0 || a > 15 || c < 0 || c > 15) {
                            auto& neighbor = m_neighbors[cell.dir];
                            if (neighbor) {
                                int lightlevel = m_neighbors[cell.dir]->getLightLevelAt(i,j,k);
                                if(m_neighbors[cell.dir]->getBlockAt(i, j, k) == EMPTY || m_neighbors[cell.dir]->getBlockAt(i, j, k) == WATER ||
                                    m_neighbors[cell.dir]->getBlockAt(i, j, k) == LAVA || isFlower(m_neighbors[cell.dir]->getBlockAt(i, j, k))) {
                                    vboBlockFace(cell.dir, x + minX, y, z + minZ, getCoords(), ch, index, curBlock, temperature, humidity, lightlevel);
                                }
                            }
                        } else {
                            // PUSHES THE FACES
                            BlockType neighbor = getBlockAt(i, j, k);
                            int lightlevel = getLightLevelAt(i, j, k);
                            if(neighbor == EMPTY || neighbor == WATER || neighbor == LAVA || isFlower(neighbor)) {
                                vboBlockFace(cell.dir, x + minX, y, z + minZ, getCoords(), ch, index, curBlock, temperature, humidity, lightlevel);
                            }
                        }
                    }
                }

            }
        }
    }
}

void Chunk::pushChunkVBO(std::vector<GLuint> idx, std::vector<glm::vec4> chunk,
                          std::vector<GLuint> idxTrans, std::vector<glm::vec4> chunkTrans) {
    m_count = idx.size();
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateChunk();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufChunk);
    mp_context->glBufferData(GL_ARRAY_BUFFER, chunk.size() * sizeof(glm::vec4), chunk.data(), GL_STATIC_DRAW);

    m_countTrans = idxTrans.size();
    generateIdxTrans();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIdxTrans);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxTrans.size() * sizeof(GLuint), idxTrans.data(), GL_STATIC_DRAW);

    generateChunkTrans();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufChunkTrans);
    mp_context->glBufferData(GL_ARRAY_BUFFER, chunkTrans.size() * sizeof(glm::vec4), chunkTrans.data(), GL_STATIC_DRAW);
}

void Chunk::setCount(int i) {
    m_count = 0;
    m_countTrans = 0;
}

glm::ivec2 Chunk::getCoords() {
    return glm::ivec2(minX, minZ);
}


