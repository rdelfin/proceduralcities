//
// Created by rdelfin on 11/26/16.
//

#pragma once
#include "Renderable.h"

#include <glm/glm.hpp>

class Mesh : public Renderable {
public:
    Mesh();

    virtual draw();

    virtual ~Mesh();
protected:
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
};
