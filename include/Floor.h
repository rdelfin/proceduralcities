#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Floor {
    public:
        Floor();
        vector<glm::vec4> vertices;
        vector<glm::vec4> normals;
        vector<glm::uvec3> faces;
};