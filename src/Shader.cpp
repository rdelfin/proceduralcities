//
// Created by rdelfin on 11/26/16.
//


#include <functional>
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <debuggl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

/*============================================================================================================
 *============================================ SHADER UNIFORM ================================================
 *============================================================================================================
 */
ShaderUniform::ShaderUniform() {

}

ShaderUniform::ShaderUniform(std::string name, std::function<void(int, const void*)> binder, std::function<const void*()> data_source)
    : name(name), binder(binder), data_source(data_source) {

}


ShaderUniform::ShaderUniform(std::string name, BinderType binderType, std::function<const void*()> data_source)
    : name(name), data_source(data_source) {
    // This switch sets up a couple of binders for uniforms: ie float, vector3, vector4, matrix3 and matrix4
    switch (binderType) {
        case BINDER_FLOAT:
            binder = [](int loc, const void* data) {
                glUniform1fv(loc, 1, (const GLfloat*) data);
            };
        case BINDER_VECTOR3_F:
            binder = [](int loc, const void* data) {
                glUniform3fv(loc, 1, (const GLfloat*) data);
            };
        case BINDER_VECTOR4_F:
            binder = [](int loc, const void* data) {
                glUniform4fv(loc, 1, (const GLfloat*) data);
            };
        case BINDER_MATRIX3_F:
            binder = [](int loc, const void* data) {
                glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat*) data);
            };
        case BINDER_MATRIX4_F:
            binder = [](int loc, const void* data) {
                glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*) data);
            };
    }
}

ShaderUniform::~ShaderUniform() { }

/*============================================================================================================
 *================================================ SHADER ====================================================
 *============================================================================================================
 */

Shader::Shader() {

}

Shader::Shader(std::vector<ShaderUniform> uniforms, int shaderType, std::string s, bool file)
    : uniforms(uniforms), type(shaderType) {
    if(file) {
        // If file is true, `s` is a file name. Read in from file
        std::ifstream reader(s);
        program = std::string(std::istreambuf_iterator<char>(reader),
                              std::istreambuf_iterator<char>());
        reader.close();
    } else {
        // Otherwise, this is program string
        program = s;
    }

    // Afterwards, compile shader and load programId accordingly
    compileShader();
}


void Shader::compileShader() {
    GLuint program_id = 0;
    CHECK_GL_ERROR(program_id = glCreateShader(type));
#if 1
    std::cerr << __func__ << " shader id " << program_id << " type " << type << "\tsource:\n" << program.c_str() << std::endl;
#endif
    const char* programPointer = program.c_str();
    CHECK_GL_ERROR(glShaderSource(program_id, 1, &programPointer, nullptr));
    glCompileShader(program_id);
    CHECK_GL_SHADER_ERROR(program_id);
    this->programId = program_id;
}

Shader::~Shader() {

}