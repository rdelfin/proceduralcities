//
// Created by rdelfin on 12/7/16.
//

#include "camera.h"


#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace {
    float pan_speed = 1.0f;
    float roll_speed = 0.1f;
    float rotation_speed = 0.05f;
    float zoom_speed = 1.0f;
};

Camera::Camera() { }

void Camera::yaw(float dir) {
    // Rotate around up vector (for yaw)
    eyeRotateMat = glm::rotate(rotation_speed*dir, up_) * eyeRotateMat;
    centerRotateMat = glm::rotate(rotation_speed*dir, up_) * centerRotateMat;

    glm::vec3 newEye(eyeRotateMat * glm::vec4(eye_, 1));
    // Generate new look, right and up vectors based on new rotation
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}

void Camera::pitch(float dir) {
    // Rotate around right pointing vector (for pitch)
    eyeRotateMat = glm::rotate(rotation_speed*dir, right_) * eyeRotateMat;
    centerRotateMat = glm::rotate(rotation_speed*dir, right_) * centerRotateMat;

    glm::vec3 newEye(eyeRotateMat * glm::vec4(eye_, 1));
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

void Camera::fpsPitch(float dir) {
    glm::mat4 pitchRotate = glm::rotate(dir*roll_speed, right_);
    right_ = glm::normalize(glm::vec3(pitchRotate * glm::vec4(right_, 0.0f)));
    look_ = glm::normalize(glm::vec3(pitchRotate * glm::vec4(look_, 0.0f)));
    up_ = glm::normalize(glm::vec3(pitchRotate * glm::vec4(up_, 0.0f)));
    centerRotateMat = pitchRotate * centerRotateMat;
}

void Camera::fpsYaw(float dir) {
    glm::mat4 yawRotate = glm::rotate(dir*roll_speed, up_);
    right_ = glm::normalize(glm::vec3(yawRotate * glm::vec4(right_, 0.0f)));
    look_ = glm::normalize(glm::vec3(yawRotate * glm::vec4(look_, 0.0f)));
    up_ = glm::normalize(glm::vec3(yawRotate * glm::vec4(up_, 0.0f)));
    centerRotateMat = yawRotate * centerRotateMat;
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
    glm::vec3 eye = glm::vec3(eyeTranslateMat * eyeRotateMat * glm::vec4(eye_, 1));
    glm::vec3 center = glm::vec3(centerTranslateMat * centerRotateMat * glm::vec4(center_, 1));

    viewMatrix = glm::lookAt(eye, center, up_);

    return viewMatrix;
}


const glm::vec3& Camera::getCameraPosition() {
    finalEye = glm::vec3(eyeTranslateMat * eyeRotateMat * glm::vec4(eye_, 1));
    return finalEye;
}

Camera::~Camera() { }