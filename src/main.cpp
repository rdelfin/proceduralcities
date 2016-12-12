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
#include <AreaLine.h>
#include <Floor.h>
#include <Renderable.h>
#include <LineMesh.h>
#include <TriangleMesh.h>
#include <sstream>
#include <camera.h>
#include <StreetMap.h>
#include <generation/street/Parser.h>

#define PARSE_LEVEL 10

#include <generation/building/Building.h>

#include <glm/gtx/rotate_vector.hpp>

using namespace std;


#define PI 3.14159265f

void load_teapot(std::vector<glm::vec4>& vertices, std::vector<glm::uvec3>& faces, std::vector<glm::vec4>& normals);
void append_mesh_data(std::vector<glm::vec4>& verticesTotal, std::vector<glm::vec4>& normalsTotal, std::vector<glm::uvec3>& facesTotal,
                      const std::vector<glm::vec4>& verticesNew, const std::vector<glm::vec4>& normalsnew, const std::vector<glm::uvec3>& facesNew);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);


static int window_width = 1024, window_height = 800;

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

bool checkWaterParkCollision(int x, int z, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints) {
    if (waterPoints.count(x) != 0) {
        if (waterPoints[x].count(z) != 0) {
            return true;
        }
        if (waterPoints[x].count(z - 2) != 0) {
            return true;
        }
        if (waterPoints[x].count(z + 2) != 0) {
            return true;
        }
    }
    if (waterPoints.count(x - 2) != 0) {
        if (waterPoints[x - 2].count(z) != 0) {
            return true;
        }
        if (waterPoints[x - 2].count(z - 2) != 0) {
            return true;
        }
        if (waterPoints[x - 2].count(z + 2) != 0) {
            return true;
        }
    }
    if (waterPoints.count(x + 2) != 0) {
        if (waterPoints[x + 2].count(z) != 0) {
            return true;
        }
        if (waterPoints[x + 2].count(z - 2) != 0) {
            return true;
        }
        if (waterPoints[x + 2].count(z + 2) != 0) {
            return true;
        }
    }
    
    if (parksPoints.count(x) != 0) {
        if (parksPoints[x].count(z) != 0) {
            return true;
        }
        if (parksPoints[x].count(z - 2) != 0) {
            return true;
        }
        if (parksPoints[x].count(z + 2) != 0) {
            return true;
        }
    }
    if (parksPoints.count(x - 2) != 0) {
        if (parksPoints[x - 2].count(z) != 0) {
            return true;
        }
        if (parksPoints[x - 2].count(z - 2) != 0) {
            return true;
        }
        if (parksPoints[x - 2].count(z + 2) != 0) {
            return true;
        }
    }
    if (parksPoints.count(x + 2) != 0) {
        if (parksPoints[x + 2].count(z) != 0) {
            return true;
        }
        if (parksPoints[x + 2].count(z - 2) != 0) {
            return true;
        }
        if (parksPoints[x + 2].count(z + 2) != 0) {
            return true;
        }
    }

    else if (x - 2 <= -100.0f || x + 2 >= 100.0f || z - 2 <= -100.0f || z + 2 >= 100.0f) {
        return true;
    }
    return false;
}

bool checkBuildingsCollision(int x, int z, map<float, set<float>> buildingPoints) {
    if (buildingPoints.count(x) != 0) {
        if (buildingPoints[x].count(z) != 0) {
            return true;
        }
        if (buildingPoints[x].count(z - 1) != 0) {
            return true;
        }
        if (buildingPoints[x].count(z - 2) != 0) {
            return true;
        }
        if (buildingPoints[x].count(z + 1) != 0) {
            return true;
        }
        if (buildingPoints[x].count(z + 2) != 0) {
            return true;
        }
    }
    if (buildingPoints.count(x - 2) != 0) {
        if (buildingPoints[x - 2].count(z) != 0) {
            return true;
        }
        if (buildingPoints[x - 2].count(z - 1) != 0) {
            return true;
        }
        if (buildingPoints[x - 2].count(z - 2) != 0) {
            return true;
        }
        if (buildingPoints[x - 2].count(z + 1) != 0) {
            return true;
        }
        if (buildingPoints[x - 2].count(z + 2) != 0) {
            return true;
        }
    }
    if (buildingPoints.count(x + 2) != 0) {
        if (buildingPoints[x + 2].count(z) != 0) {
            return true;
        }
        if (buildingPoints[x + 2].count(z - 1) != 0) {
            return true;
        }
        if (buildingPoints[x + 2].count(z - 2) != 0) {
            return true;
        }
        if (buildingPoints[x + 2].count(z + 1) != 0) {
            return true;
        }
        if (buildingPoints[x + 2].count(z + 2) != 0) {
            return true;
        }
    }

    return false;
}

bool checkRoadCollision(float x, float z, float width, float height, float theta, const std::vector<StreetLine>& lines) {
    glm::vec2 a(width / 2, -height / 2), b(width / 2, height / 2), c(-width / 2, height / 2), d(-width / 2,
                                                                                                -height / 2);
    glm::vec2 translation = glm::vec2(x, z);

    a = glm::rotate(a, theta) + translation;
    b = glm::rotate(b, theta) + translation;
    c = glm::rotate(c, theta) + translation;
    d = glm::rotate(d, theta) + translation;

    for (auto it = lines.begin(); it != lines.end(); ++it) {
        if (it->intersects(a, b, c, d, 0.75f)) {
            return true;
        }
    }

    return false;
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

    /*// Create objects
    std::vector<glm::vec4> vertices;
    std::vector<glm::uvec3> faces;
    std::vector<glm::vec4> normals;

    load_teapot(vertices, faces, normals);*/

    Shader vertexShader("resources/shaders/default.vert"),
           geometryShader("resources/shaders/default.geom"),
           linesGeometryShader("resources/shaders/lines.geom"),
           fragmentShader("resources/shaders/default.frag"),
           floorFragmentShader("resources/shaders/floor.frag"),
           waterFragmentShader("resources/shaders/water.frag"),
           parksFragmentShader("resources/shaders/parks.frag"),
           streetFragmentShader("resources/shaders/streets.frag"),
           buildingFragmentShader("resources/shaders/buildings.frag"),
           streetGeometryShader("resources/shaders/street.geom");

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

    std::vector<ShaderUniform> buildingUniforms = { ShaderUniform("projection", BINDER_MATRIX4_F, proj_matrix_data_source),
                                                    ShaderUniform("view", BINDER_MATRIX4_F, view_matrix_data_source),
                                                    ShaderUniform("camera_position", BINDER_VECTOR3_F, camera_data_source) };

    camera.zoom(280.0f);
    camera.pitch((180.0f / M_PI) * -420 / window_width);
    camera.strave(glm::vec3(0, 1, 0));

    Floor floor;
    Area area;
    AreaLine areaLine;
    int i, j;
    //vector<LineMesh> waterMeshes;
    //vector<LineMesh> parksMeshes;
    StreetMap streetMap(ROAD_RECTANGULAR, area.populationCenters, area.waterPoints, area.parksPoints);
    for (i = 0; i < 20; i++) {
        streetMap.nextIteration(area.waterPoints, area.parksPoints);
    }

    // std::vector<glm::vec4> streetVertices, streetNormals;
    // std::vector<glm::uvec3> streetFaces;

    // GlobalGoals globalGoals;
    // LocalConstraints localConstraints(area.waterPoints, area.parksPoints);
    // Parser parser(globalGoals, localConstraints);

    // std::cerr << "BEGINNING PARSE..." << std::endl;

    // // Parse 10 times
    // for(int i = 0; i < PARSE_LEVEL; i++) {
    //     std::cerr << "\tlevel " << i << std::endl;
    //     parser.substitution();
    // }

    // std::vector<StreetSegment> streets = parser.parser();

    // for(StreetSegment ss : streets) {
    //     ss.addLines(streetVertices, streetFaces, glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);
    // }

    // for(auto i = 0; i < streetVertices.size(); i++) {
    //     streetNormals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    // }

    int max_attribs;
    glGetIntegerv (GL_MAX_VERTEX_ATTRIBS, &max_attribs);

    std::cout << "GL_MAX_VERTEX_ATTRIBS: " << max_attribs << std::endl;


    std::vector<glm::vec4> buildingVert, buildingNormals;
    std::vector<glm::uvec3> buildingFaces;

    std::vector<Building> buildings;
    int x, z;
    int numberOfCenters = area.populationCenters.size();
    map<float, set<float>> buildingPoints;
    float closestDistance;
    float distance;
    bool created;

    std::vector<StreetLine> lines;
    lines.insert(lines.end(), streetMap.streetsInAngle1.begin(), streetMap.streetsInAngle1.end());
    lines.insert(lines.end(), streetMap.streetsInAngle2.begin(), streetMap.streetsInAngle2.end());


    for (i = 0; i < 50; i++) {
        created = false;
        while (!created) {
            closestDistance = 999999;
            x = rand() % 201 - 100;
            z = rand() % 201 - 100;
            if(!checkWaterParkCollision(x, z, area.waterPoints, area.parksPoints) && !checkBuildingsCollision(x, z, buildingPoints) && !checkRoadCollision(x, z, 0.05f, 0.05f, PI-streetMap.angle, lines)) {
                for (j = 0; j < numberOfCenters; j++) {
                    distance = glm::length(area.populationCenters[j] - glm::vec2(x, z));
                    if (distance < closestDistance) {
                        closestDistance = distance;
                    }
                }
                buildings.push_back(Building(0.05f, 0.05f, closestDistance, glm::vec3(x, -1.9f, z), PI - streetMap.angle));
                if (buildingPoints.count(x) == 0) {
                    buildingPoints[x] = set<float>();
                }
                if (buildingPoints.count(x - 2) == 0) {
                    buildingPoints[x - 2] = set<float>();
                }
                if (buildingPoints.count(x + 2) == 0) {
                    buildingPoints[x + 2] = set<float>();
                }
                buildingPoints[x].insert(z);
                buildingPoints[x].insert(z - 1);
                buildingPoints[x].insert(z - 2);
                buildingPoints[x].insert(z + 1);
                buildingPoints[x].insert(z + 2);
                buildingPoints[x - 1].insert(z);
                buildingPoints[x - 1].insert(z - 1);
                buildingPoints[x - 1].insert(z - 2);
                buildingPoints[x - 1].insert(z + 1);
                buildingPoints[x - 1].insert(z + 2);
                buildingPoints[x - 2].insert(z);
                buildingPoints[x - 2].insert(z - 1);
                buildingPoints[x - 2].insert(z - 2);
                buildingPoints[x - 2].insert(z + 1);
                buildingPoints[x - 2].insert(z + 2);
                buildingPoints[x + 1].insert(z);
                buildingPoints[x + 1].insert(z - 1);
                buildingPoints[x + 1].insert(z - 2);
                buildingPoints[x + 1].insert(z + 1);
                buildingPoints[x + 1].insert(z + 2);
                buildingPoints[x + 2].insert(z);
                buildingPoints[x + 2].insert(z - 1);
                buildingPoints[x + 2].insert(z - 2);
                buildingPoints[x + 2].insert(z + 1);
                buildingPoints[x + 2].insert(z + 2);
                created = true;
            }
        }
    }

    for(auto it = buildings.begin(); it != buildings.end(); ++it) {
        it->update();
        append_mesh_data(buildingVert, buildingNormals, buildingFaces, it->transformedVertices, it->transformedNormals, it->faces);
    }



    TriangleMesh floorMesh(floor.vertices, floor.normals, floor.faces, vertexShader, geometryShader, floorFragmentShader, uniforms);
    TriangleMesh waterMesh(area.waterVertices, area.waterNormals, area.waterFaces, vertexShader, geometryShader, waterFragmentShader, uniforms);
    TriangleMesh parksMesh(area.parksVertices, area.parksNormals, area.parksFaces, vertexShader, geometryShader, parksFragmentShader, uniforms);
    TriangleMesh buildingsMesh(buildingVert, buildingNormals, buildingFaces, vertexShader, geometryShader, buildingFragmentShader, uniforms);
    TriangleMesh streetMesh(streetMap.vertices, streetMap.normals, streetMap.faces, vertexShader, geometryShader, streetFragmentShader, uniforms);
    //TriangleMesh mesh(vertices, normals, faces, vertexShader, geometryShader, fragmentShader, uniforms);
    //TriangleMesh streetMesh(streetVertices, streetNormals, streetFaces, vertexShader, geometryShader, streetFragmentShader, uniforms);

    while (!glfwWindowShouldClose(window)) {
        // Setup some basic window stuff.
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);
        glClearColor(0.390625f, 0.58203125f, 0.92578125f, 1.0f);           // Cornflower blue which, according to Microsoft, is the best background color
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        //glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);

        floorMesh.draw();
        streetMesh.draw();
        waterMesh.draw();
        parksMesh.draw();
        buildingsMesh.draw();

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
            float pitchDegrees = (180.0f / M_PI) * -deltaMouse.y / window_width;
            float yawDegrees = (180.0f /  M_PI) * -deltaMouse.x / window_height;
            fps ? camera.fpsPitch(pitchDegrees) : camera.pitch(pitchDegrees);
            fps ? camera.fpsYaw(yawDegrees) : camera.yaw(yawDegrees);
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

void append_mesh_data(std::vector<glm::vec4>& verticesTotal, std::vector<glm::vec4>& normalsTotal, std::vector<glm::uvec3>& facesTotal,
                      const std::vector<glm::vec4>& verticesNew, const std::vector<glm::vec4>& normalsNew, const std::vector<glm::uvec3>& facesNew) {
    size_t startIdx = verticesTotal.size();

    verticesTotal.insert(verticesTotal.end(), verticesNew.begin(), verticesNew.end());
    normalsTotal.insert(normalsTotal.end(), normalsNew.begin(), normalsNew.end());

    for(auto it = facesNew.begin(); it != facesNew.end(); ++it) {
        facesTotal.push_back(glm::uvec3(it->x + startIdx, it->y + startIdx, it->z + startIdx));
    }
}