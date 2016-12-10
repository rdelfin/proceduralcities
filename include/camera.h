//
// Created by rdelfin on 12/7/16.
//

#ifndef PROCEDURALCITIES_CAMERA_H
#define PROCEDURALCITIES_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    void yaw(float dir);
    void pitch(float dir);
    void roll(float dir);
    void pan(glm::vec3 dir);
    void strave(glm::vec3 dir);
    void yPan(glm::vec3 dir);
    void yStrave(glm::vec3 dir);
    void zoom(float dir);

    const glm::mat4& getViewMatrix();
    const glm::vec3& getCameraPosition();

    ~Camera();
private:
    float camera_distance_ = 3.0;
    glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 center_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
    glm::vec3 right_ = glm::vec3(1.0f, 0.0, 0.0f);
    glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);

    glm::mat4 eyeTranslateMat = glm::mat4(1.0f);
    glm::mat4 centerTranslateMat = glm::mat4(1.0f);
    glm::mat4 rotateMat = glm::mat4(1.0f);

    glm::vec3 finalEye = glm::vec3(0.0f, 0.0f, camera_distance_);

    glm::mat4 viewMatrix = glm::mat4();
};


#endif //PROCEDURALCITIES_CAMERA_H
