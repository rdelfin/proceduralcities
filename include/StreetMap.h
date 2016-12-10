#ifndef PROCEDURALCITIES_STREETMAP_H
#define PROCEDURALCITIES_STREETMAP_H

#include <string>
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

enum roadPattern {
    ROAD_BASIC,
    ROAD_RECTANGULAR,
    ROAD_RADIAL
};

class StreetMap {
    public:
        StreetMap(enum roadPattern pattern, vector<glm::vec2> populationCenters);

        vector<glm::vec4> vertices;
        vector<glm::vec4> normals;
        vector<glm::uvec3> faces;
};

#endif // PROCEDURALCITIES_STREETMAP_H