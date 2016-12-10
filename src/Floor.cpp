#include "Floor.h"

Floor::Floor() {
    vertices.push_back(glm::vec4(-100.0f, -2.0f, -100.0f, 1.0f));
    vertices.push_back(glm::vec4(100.0f, -2.0f, -100.0f, 1.0f));
    vertices.push_back(glm::vec4(100.0f, -2.0f, 100.0f, 1.0f));

    vertices.push_back(glm::vec4(-100.0f, -2.0f, -100.0f, 1.0f));
    vertices.push_back(glm::vec4(100.0f, -2.0f, 100.0f, 1.0f));
    vertices.push_back(glm::vec4(-100.0f, -2.0f, 100.0f, 1.0f));

    for (int i = 0; i < 6; i++) {
        normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    }

    faces.push_back(glm::uvec3(0, 1, 2));
    faces.push_back(glm::uvec3(3, 4, 5));
}