#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Area {
    public:
        Area();
        vector<glm::vec4> waterVertices;
        vector<glm::vec4> waterNormals;
        vector<glm::uvec3> waterFaces;
        vector<glm::vec4> parksVertices;
        vector<glm::vec4> parksNormals;
        vector<glm::uvec3> parksFaces;
};