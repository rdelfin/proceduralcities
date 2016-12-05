//
// Created by rdelfin on 12/4/16.
//

#ifndef PROCEDURALCITIES_TRIANGLEMESH_H
#define PROCEDURALCITIES_TRIANGLEMESH_H

#include <glm/glm.hpp>

#include "Renderable.h"
#include "Program.h"

class TriangleMesh : public Renderable {
public:
    TriangleMesh();
    TriangleMesh(std::vector<glm::vec4> vertices, std::vector<glm::uvec3> faces,
                 const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
                 std::vector<ShaderUniform> uniforms);

    virtual void draw();

    virtual ~TriangleMesh();
private:
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<unsigned> vbo;
    unsigned vao;

    Program program;

    static const std::string VERTEX_VBO_NAME;
    static const std::string FRAGMENT_COLOR_NAME;
};


#endif //PROCEDURALCITIES_TRIANGLEMESH_H
