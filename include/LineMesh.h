//
// Created by rdelfin on 12/4/16.
//

#ifndef PROCEDURALCITIES_LINEMESH_H
#define PROCEDURALCITIES_LINEMESH_H

#include <glm/glm.hpp>

#include "Renderable.h"
#include "Program.h"

class LineMesh : public Renderable {
public:
    LineMesh();
    LineMesh(const std::vector<glm::vec4>& vertices, const std::vector<glm::vec4>& normals, const std::vector<glm::uvec2>& faces,
             const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
             const std::vector<ShaderUniform>& uniforms);

    virtual void draw();

    virtual ~LineMesh();
private:
    std::vector<glm::vec4> vertices, normals;
    std::vector<glm::uvec2> faces;
    std::vector<unsigned> vbo;
    unsigned vao;

    Program program;

    static const std::string VERTEX_VBO_NAME;
    static const std::string FRAGMENT_COLOR_NAME;
    static const std::string NORMAL_VBO_NAME;
};


#endif //PROCEDURALCITIES_LINEMESH_H
