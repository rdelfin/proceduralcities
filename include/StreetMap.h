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
        StreetMap(enum roadPattern pattern, vector<glm::vec2> populationCenters, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints);

        vector<glm::vec4> vertices;
        vector<glm::vec4> normals;
        vector<glm::uvec3> faces;
    
    private:
        void calculateExtremes(glm::vec3 point, glm::vec3 centerTangent, glm::vec3 centerNormal, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints,
                               glm::vec4 &bottomleft, glm::vec4 &bottomright, glm::vec4 &topleft, glm::vec4 &topright);
};

#endif // PROCEDURALCITIES_STREETMAP_H