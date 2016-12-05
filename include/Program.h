//
// Created by rdelfin on 11/26/16.
//

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <GL/glew.h>

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

struct Shader {
public:
    /**
     * Create an empty shader
     */
    Shader() { }

    /**
     * Creates a non-empty shader object
     * @param program String representing either the program or the path to the shader file
     * @param isFile True if program represents a file path, and false if it contains the program
     */
    Shader(const std::string& program, bool isFile = true) : program(program) {
        if(isFile) {
            // If file is true, `s` is a file name. Read in from file
            std::ifstream reader(program, std::ios_base::in);
            this->program = std::string(std::istreambuf_iterator<char>(reader),
                                        std::istreambuf_iterator<char>());
            reader.close();
        }
    }

    void compile(int type);

    ~Shader() { }

    std::string program;
    int shader_id;
};

class Program {
public:
    /**
     * Initializes an empty program
     */
    Program();

    /**
     * Main constructor for shader. Creates the shader with a program and a set of uniforms.
     * @param uniforms A vector of uniforms to be passed into the shader
     * @param s The string representing either a file name or the shader program itself
     * @param file If true, `s` represents a file name. Otherwise, `s` represents a string with the program
     */
    Program(std::vector<ShaderUniform> uniforms, Shader vertexShader, Shader geometryShader, Shader fragmentShader);

    GLint getProgramId() { return shaderProgram; }

    void addUniformLocations();

    void bindUniforms();

    void link();
    void use();

    ~Program();
private:
    /**
     * Compiles the shader on the GPU. Lifted mostly from project 3
     */
    void compile();

    Shader vertexShader, geometryShader, fragmentShader;
    std::vector<ShaderUniform> uniforms;
    std::vector<GLint> uniformIds;
    GLint shaderProgram;
};
