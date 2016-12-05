//
// Created by rdelfin on 12/4/16.
//

#include <GL/glew.h>
#include <debuggl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "TriangleMesh.h"

#include "Program.h"

const std::string TriangleMesh::VERTEX_VBO_NAME = "vertex_position";
const std::string TriangleMesh::FRAGMENT_COLOR_NAME = "vertex_color";


TriangleMesh::TriangleMesh() {

}

TriangleMesh::TriangleMesh(std::vector<glm::vec4> vertices, std::vector<glm::vec4> normals, std::vector<glm::uvec3> faces,
                           const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
                           std::vector<ShaderUniform> uniforms)
    : vertices(vertices), normals(normals), faces(faces), program(uniforms, vertexShader, geometryShader, fragmentShader) {
    GLint programId = program.getProgramId();

    // Generate VAO
    CHECK_GL_ERROR(glGenVertexArrays(1, (GLuint*)&vao));
    CHECK_GL_ERROR(glBindVertexArray(vao));

    // Generate VBO
    vbo.resize(3);
    CHECK_GL_ERROR(glGenBuffers(GL_ARRAY_BUFFER, vbo.data()));

    // Bind vertices
    GLuint verticesPosition = 0;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[verticesPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, vertices.data(), GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(verticesPosition, 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(verticesPosition));
    CHECK_GL_ERROR(glBindAttribLocation(programId, verticesPosition, VERTEX_VBO_NAME.c_str()));

    // Bind normals
    GLuint normalsPosition = 1;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[normalsPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, normals.data(), GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(normalsPosition, 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(normalsPosition));
    CHECK_GL_ERROR(glBindAttribLocation(programId, normalsPosition, VERTEX_VBO_NAME.c_str()));


    // Add a vertex_color output
    CHECK_GL_ERROR(glBindFragDataLocation(programId, 0, FRAGMENT_COLOR_NAME.c_str()));

    // Link the program
    program.link();

    // Add the index (aka the faces)
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * 3, faces.data(), GL_STATIC_DRAW));

    // Add uniform locations
    program.addUniformLocations();

}

void TriangleMesh::draw() {
    CHECK_GL_ERROR(glBindVertexArray(vao));
    program.use();
    program.bindUniforms();

    // Draw vertices
    CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0));
}

TriangleMesh::~TriangleMesh() {

}