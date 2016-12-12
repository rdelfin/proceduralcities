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

enum {
    ROAD_BASIC,
    ROAD_RECTANGULAR,
    ROAD_RADIAL
} typedef RoadPattern;

class StreetLine {
public:
    StreetLine(glm::vec4 endpoint1, glm::vec4 endpoint2);

    // Drops y and w coordinates
    bool intersects(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, float width);

    glm::vec4 endpoint1;
    glm::vec4 endpoint2;
};

class StreetMap {
public:
    StreetMap(RoadPattern pattern, vector<glm::vec2> populationCenters, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints);

    void nextIteration(map<float, set<float>> waterPoints, map<float, set<float>> parksPoints);

    vector<glm::vec4> vertices;
    vector<glm::vec4> normals;
    vector<glm::uvec3> faces;

    vector<StreetLine> streetsInAngle1;
    vector<StreetLine> streetsInAngle2;

    glm::vec3 centerNormal;
    glm::vec3 centerTangent;

    float angle;

private:
    void calculateExtremes(glm::vec3 point, glm::vec3 centerTangent, glm::vec3 centerNormal, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints,
                           glm::vec4 &bottomleft, glm::vec4 &bottomright, glm::vec4 &topleft, glm::vec4 &topright, float dist);

    int index;
};

#endif // PROCEDURALCITIES_STREETMAP_H