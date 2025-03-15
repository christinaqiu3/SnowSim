#include "player.h"
#include <QString>
#include <iostream>
#include <ostream>
#include <cmath>

#define COLLISION_OFFSET 0.01f

Player::Player(glm::vec3 pos, const Terrain &terrain)
    : Entity(pos), m_velocity(0,0,0), m_acceleration(0,0,0),
    m_camera(pos + glm::vec3(0, 1.5f, 0)), mcr_terrain(terrain),
    mcr_camera(m_camera)
{}

Player::~Player()
{}

glm::vec3 Player::getPosition() {
    return m_position;
}

glm::vec3 Player::getDir() {
    return m_forward;
}

void Player::tick(float dT, InputBundle &input) {
    processInputs(input);
    computePhysics(dT, mcr_terrain);
}

void Player::processInputs(InputBundle &inputs) {

    // Handle movement inputs
    glm::vec3 moveDirection(0.0f);

    if (flightMode) {
        if (inputs.wPressed) moveDirection += m_forward;
        if (inputs.sPressed) moveDirection -= m_forward;
        if (inputs.aPressed) moveDirection -= m_right;
        if (inputs.dPressed) moveDirection += m_right;
        if (inputs.qPressed) moveDirection -= m_up;
        if (inputs.ePressed) moveDirection += m_up;
    } else {
        if (inputs.wPressed) moveDirection += glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));
        if (inputs.sPressed) moveDirection -= glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));
        if (inputs.aPressed) moveDirection -= glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z));
        if (inputs.dPressed) moveDirection += glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z));

        if (inputs.spacePressed) {
            if (LAVA == mcr_terrain.getBlockAt(m_position) || WATER == mcr_terrain.getBlockAt(m_position)) {
                moveDirection += glm::vec3(0.0f, 1.f, 0.0f);
                m_velocity.y = 5.f;
            } else if (isOnGround()) {
                //              float jumpVelocity = 5.f;
                //              m_velocity.y = jumpVelocity;
                m_isJumping = true;
            }
        }
    }
    // Apply movement acceleration based on flight mode
    float accelerationFactor = (flightMode) ? 1.5f : 0.5f;
    m_acceleration = moveDirection * accelerationFactor * 30.f;
}

bool Player::isOnGround() { //0 means no, 1 means yes, 2 means water
    // Raycast from the player's position downwards to check for ground collision
    float dist = COLLISION_OFFSET * 1.f;
    glm::vec3 rayOrigin = m_position;
    glm::vec3 rayDirection(0.0f, -0.3f, 0.0f); // Ray points downwards
    float out_iAxis;
    glm::vec3 out_iPos;
    BlockType BT;
    glm::ivec3 blockHit;
    bool res = gridMarch(rayOrigin, rayDirection, mcr_terrain, &dist, &blockHit, &out_iAxis, &out_iPos, &BT);

    return res;
}

void Player::computePhysics(float dT, const Terrain &terrain) {
    // Snap to ground
    //if player is on ground snap y position to ground and set y velocity to 0. not affected by gravity until is grounded
    if (!flightMode && isOnGround() ){//&& !m_isJumping) {
        std::cout << "On ground" << std::endl;
        //       m_position.y = floor(m_position.y) + COLLISION_OFFSET;
        m_acceleration.y = 0;
        m_velocity.y = 0;
    }

    // TODO: Update the Player's position based on its acceleration
    // and velocity, and also perform collision detection.
    // Update velocity based on acceleration
    if (!flightMode && !isOnGround()) {
        std::cout << "called onground for gravity" << std::endl;
        float gravity = 30.f;
        m_acceleration.y = -gravity;
        //        m_acceleration.y -= gravity;
    }

    if (m_isJumping) {
        //std::cout << "Jumping" << std::endl;
        float jumpVelocity = 10.f;
        m_velocity.y = jumpVelocity;
        m_isJumping = false;
    }

    m_velocity += m_acceleration * dT;

    // Apply friction (reduce velocity)
    float frictionFactor = (flightMode) ? .95f : 0.5f;
    m_velocity *= frictionFactor;

    // Calculate displacement for each axis using the basic physics equation
    float displacementX = m_velocity.x * dT;// + 0.5f * m_acceleration.x * dT * dT;
    float displacementY = m_velocity.y * dT;// + 0.5f * m_acceleration.y * dT * dT;
    float displacementZ = m_velocity.z * dT;// + 0.5f * m_acceleration.z * dT * dT;

    // makes it so theres no collisions in FM
    if (!flightMode) {
        handleCollision(dT, terrain, displacementX, displacementY, displacementZ);
    }

    // Update player position
    moveAlongVector(glm::vec3(displacementX, displacementY, displacementZ));
}

void Player::handleCollision(float dT, const Terrain &terrain, float &dX, float &dY, float &dZ) {

    std::vector<glm::vec3> bodyVertices;
    float halfBlockSize = 0.45f; // Assuming Minecraft block size is 1, so half size is 0.5
    for (float x : {-halfBlockSize, halfBlockSize}) {
        for (float y : {0.0f, .95f, 1.95f}) {
            for (float z : {-halfBlockSize, halfBlockSize}) {
                bodyVertices.emplace_back(m_position + glm::vec3(x, y, z));
            }
        }
    }

    // Iterate through body vertices and cast rays in each direction
    for (const auto &vertex : bodyVertices) {
        glm::vec3 rayOrigin = vertex;
        glm::vec3 rayDirection;
        float distX, distY, distZ;
        glm::ivec3 blockHitX, blockHitY, blockHitZ;
        float out_iAxis;
        glm::vec3 out_iPos;
        BlockType BT;

        // Cast rays in X, Y, and Z directions
        rayDirection = glm::vec3(dX, 0.0f, 0.0f);
        if (gridMarch(rayOrigin, rayDirection, terrain, &distX, &blockHitX, &out_iAxis, &out_iPos, &BT)) { //add input to gm if true tell its to treat water blcoks like empty for collision
            // Handle collision along X-axis

            dX = (distX - COLLISION_OFFSET) * glm::sign(dX);
            m_velocity.x = 0.0f;
            m_acceleration.x = 0.0f;
        }

        rayDirection = glm::vec3(0.0f, dY, 0.0f);
        if (gridMarch(rayOrigin, rayDirection, terrain, &distY, &blockHitY, &out_iAxis, &out_iPos, &BT)) {
            std::cout << "Colliding with floor, Y before is " << m_position.y;
                // Handle collision along Y-axis
            dY = (distY - COLLISION_OFFSET) * glm::sign(dY);
            m_velocity.y = 0.0f;
            m_acceleration.y = 0.0f;
            std::cout << ", Y after is " << m_position.y + dY << std::endl;
        }

        rayDirection = glm::vec3(0.0f, 0.0f, dZ);
        if (gridMarch(rayOrigin, rayDirection, terrain, &distZ, &blockHitZ, &out_iAxis, &out_iPos, &BT)) {
            // Handle collision along Z-axis
            dZ = (distZ - COLLISION_OFFSET) * glm::sign(dZ);
            m_velocity.z = 0.0f;
            m_acceleration.z = 0.0f;
        }

        if (WATER == terrain.getBlockAt(m_position) || LAVA == terrain.getBlockAt(m_position)) {
            m_velocity *= 0.99f;
        }
    }
}

bool Player::gridMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection, const Terrain &terrain, float *out_dist, glm::ivec3 *out_blockHit, float *out_iAxis, glm::vec3 *out_iPos, BlockType *BT) {
    float maxLen = glm::length(rayDirection); // Farthest we search
    glm::ivec3 currCell = glm::ivec3(floor(rayOrigin.x), floor(rayOrigin.y), floor(rayOrigin.z));
    rayDirection = glm::normalize(rayDirection); // Now all t values represent world dist.

    float curr_t = 0.f;
    while(curr_t < maxLen) {
        float min_t = glm::sqrt(3.f);
        float interfaceAxis = -1; // Track axis for which t is smallest
        for(int i = 0; i < 3; ++i) { // Iterate over the three axes
            if(rayDirection[i] != 0) { // Is ray parallel to axis i?
                float offset = glm::max(0.f, glm::sign(rayDirection[i])); // See slide 5
                // If the player is *exactly* on an interface then
                // they'll never move if they're looking in a negative direction
                if(currCell[i] == rayOrigin[i] && offset == 0.f) {
                    offset = -1.f;
                }
                int nextIntercept = currCell[i] + offset;
                float axis_t = (nextIntercept - rayOrigin[i]) / rayDirection[i];
                axis_t = glm::min(axis_t, maxLen); // Clamp to max len to avoid super out of bounds errors
                if(axis_t < min_t) {
                    min_t = axis_t;
                    interfaceAxis = i;
                }
            }
        }
        if(interfaceAxis == -1) {
            throw std::out_of_range("interfaceAxis was -1 after the for loop in gridMarch!");
        }
        curr_t += min_t; // min_t is declared in slide 7 algorithm
        rayOrigin += rayDirection * min_t;
        glm::ivec3 offset = glm::ivec3(0,0,0);
        // Sets it to 0 if sign is +, -1 if sign is -
        offset[interfaceAxis] = glm::min(0.f, glm::sign(rayDirection[interfaceAxis]));
        currCell = glm::ivec3(glm::floor(rayOrigin)) + offset;
        // If currCell contains something other than EMPTY, return
        // curr_t
        BlockType cellType = terrain.getBlockAt(currCell.x, currCell.y, currCell.z);
        if(cellType != EMPTY && cellType != WATER && cellType != LAVA && curr_t < maxLen) {
            *out_blockHit = currCell;
            *out_dist = glm::min(maxLen, curr_t);

            *out_iAxis = interfaceAxis;
            glm::vec3 relativePos = (rayOrigin + rayDirection * curr_t - glm::vec3(*out_blockHit));
            *out_iPos = relativePos;

            *BT = cellType;

            //right now doesnt treat water block like empty. but we need it to.
            //right now i just dont have collisions when theres water underneath but still need that.
            //then just do a check if the currentblockinside is water then slow.


            return true;
        }
    }
    *out_dist = glm::min(maxLen, curr_t);
    return false;
}

void Player::setCameraWidthHeight(unsigned int w, unsigned int h) {
    m_camera.setWidthHeight(w, h);
}

void Player::moveAlongVector(glm::vec3 dir) {
    Entity::moveAlongVector(dir);
    m_camera.moveAlongVector(dir);
}
void Player::moveForwardLocal(float amount) {
    Entity::moveForwardLocal(amount);
    m_camera.moveForwardLocal(amount);
}
void Player::moveRightLocal(float amount) {
    Entity::moveRightLocal(amount);
    m_camera.moveRightLocal(amount);
}
void Player::moveUpLocal(float amount) {
    Entity::moveUpLocal(amount);
    m_camera.moveUpLocal(amount);
}
void Player::moveForwardGlobal(float amount) {
    Entity::moveForwardGlobal(amount);
    m_camera.moveForwardGlobal(amount);
}
void Player::moveRightGlobal(float amount) {
    Entity::moveRightGlobal(amount);
    m_camera.moveRightGlobal(amount);
}
void Player::moveUpGlobal(float amount) {
    Entity::moveUpGlobal(amount);
    m_camera.moveUpGlobal(amount);
}
void Player::rotateOnForwardLocal(float degrees) {
    Entity::rotateOnForwardLocal(degrees);
    m_camera.rotateOnForwardLocal(degrees);
}
void Player::rotateOnRightLocal(float degrees) {
    Entity::rotateOnRightLocal(degrees);
    m_camera.rotateOnRightLocal(degrees);
}
void Player::rotateOnUpLocal(float degrees) {
    Entity::rotateOnUpLocal(degrees);
    m_camera.rotateOnUpLocal(degrees);
}
void Player::rotateOnForwardGlobal(float degrees) {
    Entity::rotateOnForwardGlobal(degrees);
    m_camera.rotateOnForwardGlobal(degrees);
}
void Player::rotateOnRightGlobal(float degrees) {
    Entity::rotateOnRightGlobal(degrees);
    m_camera.rotateOnRightGlobal(degrees);
}
void Player::rotateOnUpGlobal(float degrees) {
    Entity::rotateOnUpGlobal(degrees);
    m_camera.rotateOnUpGlobal(degrees);
}

//void Player::jump() {
//    Entity::jump();
//    m_camera.jump();
//}

void Player::toggleFlightMode() {
    Entity::toggleFlightMode();
    m_camera.toggleFlightMode();
}

QString Player::posAsQString() const {
    std::string str("( " + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z) + ")");
    return QString::fromStdString(str);
}
QString Player::velAsQString() const {
    std::string str("( " + std::to_string(m_velocity.x) + ", " + std::to_string(m_velocity.y) + ", " + std::to_string(m_velocity.z) + ")");
    return QString::fromStdString(str);
}
QString Player::accAsQString() const {
    std::string str("( " + std::to_string(m_acceleration.x) + ", " + std::to_string(m_acceleration.y) + ", " + std::to_string(m_acceleration.z) + ")");
    return QString::fromStdString(str);
}
QString Player::lookAsQString() const {
    std::string str("( " + std::to_string(m_forward.x) + ", " + std::to_string(m_forward.y) + ", " + std::to_string(m_forward.z) + ")");
    return QString::fromStdString(str);
}
