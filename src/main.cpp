#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "debuggl.h"
#include <Renderable.h>
#include <TriangleMesh.h>
#include <sstream>

void load_teapot(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals);


static int window_width = 800, window_height = 600;

void
ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << "\n";
}


int main() {
    std::string window_title = "City Generator";
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwSetErrorCallback(ErrorCallback);

    // Ask an OpenGL 3.3 core profile context
    // It is required on OSX and non-NVIDIA Linux
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          &window_title[0], nullptr, nullptr);
    CHECK_SUCCESS(window != nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    CHECK_SUCCESS(glewInit() == GLEW_OK);
    glGetError();  // clear GLEW's error for it
    //glfwSetKeyCallback(window, KeyCallback);
    //glfwSetCursorPosCallback(window, MousePosCallback);
    //glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSwapInterval(1);
    const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);    // version as a string
    std::cout << "Renderer: " << renderer << "\n";
    std::cout << "OpenGL version supported:" << version << "\n";

    // Create objects
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<glm::vec4> normals;

    load_teapot(vertices, faces, normals);

    Shader vertexShader("resources/shaders/default.vert"),
           geometryShader("resources/shaders/default.geom"),
           fragmentShader("resources/shaders/default.frag");

    auto view_matrix_data_source = []() -> const void* {
        static glm::mat4 proj = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        return &proj;
    };

    auto model_matrix_data_source = []() -> const void* {
        static glm::mat4 model(1.0f);
        return &model;
    };

    auto proj_matrix_data_source = []() -> const void* {
        static float aspect_ = static_cast<float>(window_width) / window_height;
        static glm::mat4 model = glm::perspective((float) (45.0f * (M_PI / 180.0f)), aspect_, 0.1f, 1000.0f);
        return &model;
    };

    std::vector<ShaderUniform> uniforms = { ShaderUniform("projection", BINDER_MATRIX4_F, proj_matrix_data_source),
                                            ShaderUniform("model", BINDER_MATRIX4_F, model_matrix_data_source),
                                            ShaderUniform("view", BINDER_MATRIX4_F, view_matrix_data_source) };

    TriangleMesh mesh(vertices, normals, faces, vertexShader, geometryShader, fragmentShader, uniforms);

    while (!glfwWindowShouldClose(window)) {
        // Setup some basic window stuff.
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);
        glClearColor(0.390625f, 0.58203125f, 0.92578125f, 1.0f);           // Cornflower blue, according to microsoft, the best background color
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);

        mesh.draw();

        // Poll and swap.
        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}


void load_teapot(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals) {
    std::ifstream fileStream("resources/model/teapot_0.obj");

    std::string line;

    while(std::getline(fileStream, line)) {
        if(line[0] == 'v') {
            std::stringstream lineStream(line);

            std::string header;
            double x, y, z;

            if(!(lineStream >> header >> x >> y >> z))
                break;

            vertices.push_back(glm::vec4(x, y, z, 1.0f));
            normals.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        }

        else if(line[0] == 'f') {
            std::stringstream lineStream(line);

            std::string header;
            int x, y, z;

            if(!(lineStream >> header >> x >> y >> z))
                break;

            faces.push_back(glm::uvec3(x, y, z));
        }
    }

}