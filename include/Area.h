#ifndef PROCEDURALCITIES_AREA_H
#define PROCEDURALCITIES_AREA_H

#include <string>
#include <functional>
#include <vector>
#include <set>
#include <map>
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

        vector<glm::vec2> populationCenters;

    private:
        map<float, set<float>> waterPoints;
        map<float, set<float>> parksPoints;
};

#endif // PROCEDURALCITIES_AREA_H