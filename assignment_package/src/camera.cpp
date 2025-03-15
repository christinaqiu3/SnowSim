#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    eye = glm::vec4(0,0,0,1);
    look = glm::vec4(0,0,1,0);
    up = glm::vec4(0,1,0,0);
    right = glm::vec4(1,0,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e,0.f),
    ref(r,1.f),
    world_up(worldUp,0.f)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::vec4(glm::normalize(glm::cross(glm::vec3(look), glm::vec3(world_up))), 0.f);
    up = glm::vec4(glm::cross(glm::vec3(right), glm::vec3(look)), 0.f);

    float tan_fovy = glm::tan(fovy/2);
    float len = glm::length(ref - eye);
    aspect = width/height;
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(fovy, width / (float)height, near_clip, far_clip) * glm::lookAt(glm::vec3(eye), glm::vec3(ref), glm::vec3(up));
}

void Camera::RotateAboutRight(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(right));
    eye = eye - ref;
    eye = rotation * eye;
    eye = eye + ref;
    RecomputeAttributes();
}

void Camera::RotateAboutUp(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(up));
    eye = eye - ref;
    eye = rotation * eye;
    eye = eye + ref;
    RecomputeAttributes();
}

void Camera::Zoom(float amt)
{
    glm::vec4 translation = look * amt;
    eye += translation;
}

void Camera::TranslateAlongLook(float amt)
{
    glm::vec4 translation = look * amt;
    eye += translation;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec4 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec4 translation = up * amt;
    eye += translation;
    ref += translation;
}
