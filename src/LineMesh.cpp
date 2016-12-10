//
// Created by rdelfin on 12/4/16.
//

#include <GL/glew.h>
#include <debuggl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LineMesh.h"

#include "Program.h"

using namespace std;

const std::string LineMesh::VERTEX_VBO_NAME = "vertex_position";
const std::string LineMesh::NORMAL_VBO_NAME = "normal";
const std::string LineMesh::FRAGMENT_COLOR_NAME = "vertex_color";


LineMesh::LineMesh() {

}

LineMesh::LineMesh(const std::vector<glm::vec4>& vertices, const std::vector<glm::vec4>& normals, const std::vector<glm::uvec2>& faces,
                   const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
                   const std::vector<ShaderUniform>& uniforms)
    : vertices(vertices), normals(normals), faces(faces) {

    // glLineWidth(2.0f);

    // Generate VAO
    CHECK_GL_ERROR(glGenVertexArrays(1, &vao));
    CHECK_GL_ERROR(glBindVertexArray(vao));

    program = Program(uniforms, vertexShader, geometryShader, fragmentShader);
    GLint programId = program.getProgramId();

    // Generate VBO
    vbo.resize(3);
    CHECK_GL_ERROR(glGenBuffers(3, vbo.data()));
    for (int i = 0; i < vbo.size(); i++) {
        cout << "1 " << vbo[i] << endl;
    }

    // Bind vertices
    GLuint verticesPosition = 0;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[verticesPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->vertices.size() - 2, &this->vertices[1], GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(vbo[verticesPosition], 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(vbo[verticesPosition]));
    CHECK_GL_ERROR(glBindAttribLocation(programId, vbo[verticesPosition], VERTEX_VBO_NAME.c_str()));

    // Bind normals
    GLuint normalsPosition = 1;
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vbo[normalsPosition]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->normals.size() - 2, &this->normals[1], GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(vbo[normalsPosition], 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(vbo[normalsPosition]));
    CHECK_GL_ERROR(glBindAttribLocation(programId, vbo[normalsPosition], NORMAL_VBO_NAME.c_str()));


    // Add a vertex_color output
    CHECK_GL_ERROR(glBindFragDataLocation(programId, 0, FRAGMENT_COLOR_NAME.c_str()));

    // Link the program
    program.link();

    // Add the index (aka the faces)
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec2) * this->faces.size() - 2, this->faces.data(), GL_STATIC_DRAW));

    // Add uniform locations
    program.addUniformLocations();
    cout << "bye" << endl;
}

void LineMesh::draw() {
    CHECK_GL_ERROR(glBindVertexArray(vao));
    program.use();
    program.bindUniforms();

    // Draw vertices
    CHECK_GL_ERROR(glDrawElements(GL_LINES, faces.size() * 2, GL_UNSIGNED_INT, 0));
}

LineMesh::~LineMesh() {

}