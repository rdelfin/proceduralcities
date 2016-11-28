//
// Created by rdelfin on 11/26/16.
//

#pragma once

#include <string>
#include <functional>
#include <vector>

typedef enum {
    BINDER_MATRIX4_F,
    BINDER_MATRIX3_F,
    BINDER_VECTOR4_F,
    BINDER_VECTOR3_F,
    BINDER_FLOAT
} BinderType;

/*
 * ShaderUniform: description of a uniform in a shader program.
 *      name: name
 *      binder: function to bind the uniform
 *      data_source: function to get the data for the uniform
 *
 * Lifted from project 3
 */
class ShaderUniform {
public:
    ShaderUniform();
    ShaderUniform(std::string name, std::function<void(int, const void*)> binder, std::function<const void*()> data_source);
    ShaderUniform(std::string name, BinderType binderType, std::function<const void*()> data_source);

    ~ShaderUniform();

    std::string name;
    /*
     * binder
     *      argument 0: the location of the uniform
     *      argument 1: the data pointer returned by data_source function.
     */
    std::function<void(int, const void*)> binder;
    /*
     * data_source:
     *      return: the pointer to the uniform data
     *
     * Hint: DON'T DO THIS
     *       data_source = []() -> void* { float value = 1.0f; return &f; };
     *       the value variable will become invalid after returning from
     *       the lambda function
     */
    std::function<const void*()> data_source;
};

class Shader {
public:
    /**
     * Initializes an empty shader
     */
    Shader();

    /**
     * Main constructor for shader. Creates the shader with a program and a set of uniforms.
     * @param uniforms A vector of uniforms to be passed into the shader
     * @param s The string representing either a file name or the shader program itself
     * @param file If true, `s` represents a file name. Otherwise, `s` represents a string with the program
     */
    Shader(std::vector<ShaderUniform> uniforms, int shaderType, std::string s, bool file = false);

    ~Shader();
private:
    /**
     * Compiles the shader on the GPU. Lifted mostly from project 3
     */
    void compileShader();

    std::string program;
    std::vector<ShaderUniform> uniforms;
    int type;
    GLint programId;
};
