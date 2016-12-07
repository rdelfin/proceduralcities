//
// Created by rdelfin on 12/7/16.
//

#include "camera.h"


#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace {
    float pan_speed = 0.1f;
    float roll_speed = 0.1f;
    float rotation_speed = 0.05f;
    float zoom_speed = 0.1f;
};

Camera::Camera() { }

void Camera::yaw(float dir) {
    // Rotate around up vector (for yaw)
    rotateMat = glm::rotate(rotation_speed*dir, up_) * rotateMat;

    glm::vec3 newEye(rotateMat * glm::vec4(eye_, 1));
    // Generate new look, right and up vectors based on new rotation
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}

void Camera::pitch(float dir) {
    // Rotate around right pointing vector (for pitch)
    rotateMat = glm::rotate(rotation_speed*dir, right_) * rotateMat;

    glm::vec3 newEye(rotateMat * glm::vec4(eye_, 1));
    // Generate new look, right and up vectors based on new rotation
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}


void Camera::roll(float dir) {
    glm::mat4 rollRotate = glm::rotate(dir*roll_speed, look_);
    right_ = glm::normalize(glm::vec3(rollRotate * glm::vec4(right_, 0.0f)));
    up_ = glm::normalize(glm::vec3(rollRotate * glm::vec4(up_, 0.0f)));
}


void Camera::strave(glm::vec3 dir) {
    centerTranslateMat  = glm::translate((dir.x*right_ + dir.y*up_ + dir.z*look_)*pan_speed) * centerTranslateMat;
}

void Camera::pan(glm::vec3 dir) {
    centerTranslateMat  = glm::translate((dir.x*right_ + dir.y*up_ + dir.z*look_)*pan_speed) * centerTranslateMat;
    eyeTranslateMat  = glm::translate((dir.x*right_ + dir.y*up_ + dir.z*look_)*pan_speed) * eyeTranslateMat;
}


void Camera::yPan(glm::vec3 dir) {
    centerTranslateMat  = glm::translate(glm::vec3(dir.x, dir.y, dir.z)*pan_speed) * centerTranslateMat;
    eyeTranslateMat  = glm::translate(glm::vec3(dir.x, dir.y, dir.z)*pan_speed) * eyeTranslateMat;
}

void Camera::yStrave(glm::vec3 dir) {
    centerTranslateMat  = glm::translate(glm::vec3(dir.x, dir.y, dir.z)*pan_speed) * centerTranslateMat;
}

void Camera::zoom(float dir) {
    camera_distance_ += dir*zoom_speed;
    eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
}

const glm::mat4& Camera::getViewMatrix()
{
    glm::vec3 newEye(eyeTranslateMat * rotateMat * glm::vec4(eye_, 1));
    glm::vec3 newCenter(centerTranslateMat * rotateMat * glm::vec4(center_, 1));

    glm::vec3 Z = glm::normalize(newEye - newCenter);
    glm::vec3 X = glm::cross(up_, Z);
    glm::vec3 Y = glm::normalize(glm::cross(Z, X));
    X = glm::normalize(X);

    // Yes the first three could have been in a loop but SHUSH!
    viewMatrix = glm::mat4(1.0f);
    viewMatrix[0] = glm::vec4(X.x, Y.x, Z.x, 0);
    viewMatrix[1] = glm::vec4(X.y, Y.y, Z.y, 0);
    viewMatrix[2] = glm::vec4(X.z, Y.z, Z.z, 0);
    viewMatrix[3] = glm::vec4(glm::dot(-X, newEye), glm::dot(-Y, newEye), glm::dot(-Z, newEye), 1);

    return viewMatrix;
}

Camera::~Camera() { }