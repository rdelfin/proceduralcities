#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <time.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "debuggl.h"
#include <Area.h>
#include <Floor.h>
#include <Renderable.h>
#include <TriangleMesh.h>
#include <sstream>
#include <camera.h>
#include <StreetMap.h>

using namespace std;

void load_teapot(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);


static int window_width = 800, window_height = 600;

static Camera camera;
bool g_ctrl_pressed;
bool g_shift_pressed;
bool g_alt_pressed;
bool fps = false;

void
ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << "\n";
}


int main() {
    srand (time(NULL));

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
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MousePosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
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
           fragmentShader("resources/shaders/default.frag"),
           floorFragmentShader("resources/shaders/floor.frag"),
           waterFragmentShader("resources/shaders/water.frag"),
           parksFragmentShader("resources/shaders/parks.frag"),
           streetFragmentShader("resources/shaders/streets.frag");

    auto view_matrix_data_source = []() -> const void* {
        return &camera.getViewMatrix();
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

    auto camera_data_source = []() -> const void* {
        return &camera.getCameraPosition();
    };

    std::vector<ShaderUniform> uniforms = { ShaderUniform("projection", BINDER_MATRIX4_F, proj_matrix_data_source),
                                            ShaderUniform("model", BINDER_MATRIX4_F, model_matrix_data_source),
                                            ShaderUniform("view", BINDER_MATRIX4_F, view_matrix_data_source),
                                            ShaderUniform("camera_position", BINDER_VECTOR3_F, camera_data_source) };

    camera.zoom(250.0f);
    camera.pitch((180.0f / M_PI) * -420 / window_width);

    Floor floor;
    Area area;
    StreetMap streetMap(ROAD_RECTANGULAR, area.populationCenters);

    TriangleMesh floorMesh(floor.vertices, floor.normals, floor.faces, vertexShader, geometryShader, floorFragmentShader, uniforms);
    TriangleMesh waterMesh(area.waterVertices, area.waterNormals, area.waterFaces, vertexShader, geometryShader, waterFragmentShader, uniforms);
    TriangleMesh parksMesh(area.parksVertices, area.parksNormals, area.parksFaces, vertexShader, geometryShader, parksFragmentShader, uniforms);
    TriangleMesh streetMesh(streetMap.vertices, streetMap.normals, streetMap.faces, vertexShader, geometryShader, streetFragmentShader, uniforms);
    //TriangleMesh mesh(vertices, normals, faces, vertexShader, geometryShader, fragmentShader, uniforms);

    while (!glfwWindowShouldClose(window)) {
        // Setup some basic window stuff.
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);
        glClearColor(0.390625f, 0.58203125f, 0.92578125f, 1.0f);           // Cornflower blue, according to microsoft, the best background color
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        //glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);

        floorMesh.draw();
        waterMesh.draw();
        parksMesh.draw();
        streetMesh.draw();
        //mesh.draw();

        // Poll and swap.
        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void
KeyCallback(GLFWwindow* window,
            int key,
            int scancode,
            int action,
            int mods) {
    // Note:
    // This is only a list of functions to implement.
    // you may want to re-organize this piece of code.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
        if(fps) camera.pan(glm::vec3(0, 0, 1));
        else camera.zoom(-1);
    } else if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
        if(fps) camera.pan(glm::vec3(0, 0, -1));
        else camera.zoom(1);
    } else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
        if(fps) camera.pan(glm::vec3(-1, 0, 0));
        else camera.strave(glm::vec3(-1, 0, 0));
    } else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
        if(fps) camera.pan(glm::vec3(1, 0, 0));
        else camera.strave(glm::vec3(1, 0, 0));
    } else if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
        camera.roll(-1);
    } else if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {
        camera.roll(1);
    } else if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {
        if(fps) camera.pan(glm::vec3(0, -1, 0));
        else camera.strave(glm::vec3(0, -1, 0));
    } else if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {
        if(fps) camera.pan(glm::vec3(0, 1, 0));
        else camera.strave(glm::vec3(0, 1, 0));
    } else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
        fps  = !fps;
        std::cout << "FPS: " << fps << std::endl;
    }


    // Used for ALT, CTRL and SHIFT
    if(action == GLFW_PRESS || action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            g_ctrl_pressed = action == GLFW_PRESS;
        } else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
            g_shift_pressed = action == GLFW_PRESS;
        } else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            g_alt_pressed = action == GLFW_PRESS;
        }
    }
}

int g_current_button;
bool g_prev_mouse_pressed;
bool g_mouse_pressed;
glm::vec2 prevMouse(0.0f, 0.0f);

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
    glm::vec2 mouse(mouse_x, mouse_y);
    glm::vec2 deltaMouse = mouse - prevMouse;

    if (g_mouse_pressed && g_prev_mouse_pressed) {
        if (g_current_button == GLFW_MOUSE_BUTTON_LEFT && !g_alt_pressed && !g_shift_pressed && !g_ctrl_pressed) {
            camera.pitch((180.0f / M_PI) * -deltaMouse.y / window_width);
            camera.yaw((180.0f /  M_PI) * -deltaMouse.x / window_height);
        } else if (g_current_button == GLFW_MOUSE_BUTTON_RIGHT || (g_current_button == GLFW_MOUSE_BUTTON_LEFT && (g_alt_pressed || g_shift_pressed))) {
            camera.zoom(10.0f * deltaMouse.y / window_height);
        } else if (g_current_button == GLFW_MOUSE_BUTTON_MIDDLE || (g_current_button == GLFW_MOUSE_BUTTON_LEFT && g_ctrl_pressed)) {
            camera.pan(25.0f*glm::vec3(-deltaMouse.x / window_width, deltaMouse.y / window_height, 0));
        }
    }

    prevMouse = glm::vec2(mouse_x, mouse_y);
    g_prev_mouse_pressed = g_mouse_pressed;
}

void
MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    g_mouse_pressed = (action == GLFW_PRESS);
    g_current_button = button;
}


void load_teapot(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals) {
    std::ifstream fileStream("resources/model/teapot.obj");

    std::string line;

    while(std::getline(fileStream, line)) {
        if(line[0] == 'v' && line[1] != 'n') {
            std::stringstream lineStream(line);

            std::string header;
            double x, y, z;

            if(!(lineStream >> header >> x >> y >> z))
                break;

            vertices.push_back(glm::vec4(x, y, z, 1.0f));
        }

        else if(line[0] == 'v' && line[1] == 'n') {
            std::stringstream lineStream(line);

            std::string header;
            double x, y, z;

            if(!(lineStream >> header >> x >> y >> z))
                break;

            normals.push_back(glm::vec4(x, y, z, 0.0f));
        }

        else if(line[0] == 'f') {
            std::stringstream lineStream(line);

            std::string header;
            int x, y, z;

            if(!(lineStream >> header >> x >> y >> z))
                break;

            faces.push_back(glm::uvec3(z - 1, y - 1, x - 1));
        }
    }
}