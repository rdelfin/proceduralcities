#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

#define PI 3.14159265f

#include "StreetMap.h"

using namespace std;

StreetLine::StreetLine(glm::vec4 endpoint1, glm::vec4 endpoint2) {
    this->endpoint1 = endpoint1;
    this->endpoint2 = endpoint2;
}

StreetMap::StreetMap(RoadPattern pattern, vector<glm::vec2> populationCenters, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints) {
    index = 0;
    int i;
    int j;
    size_t size = populationCenters.size();
    float centerRadius = 10.0f;

    switch(pattern) {
        case ROAD_BASIC: {
            break;
        }
        case ROAD_RECTANGULAR: {

            int angle1 = rand() % 180 + 1;
            int angle2 = angle1 + 180;
            int angle3 = angle1 + 90;
            int angle4 = angle3 + 180;
            float deltaX1 = cos(angle1 * PI / 180.0f);
            float deltaZ1 = sin(angle1 * PI / 180.0f);
            float deltaX2 = cos(angle2 * PI / 180.0f);
            float deltaZ2 = sin(angle2 * PI / 180.0f);
            float deltaX3 = cos(angle3 * PI / 180.0f);
            float deltaZ3 = sin(angle3 * PI / 180.0f);
            float deltaX4 = cos(angle4 * PI / 180.0f);
            float deltaZ4 = sin(angle4 * PI / 180.0f);

            this->angle = angle1 * PI / 180.0f;
            
            glm::vec3 point1;
            glm::vec3 point2;
            glm::vec3 point3;
            glm::vec3 point4;

            float distance;
            float x;
            float z;
            glm::vec4 topleft;
            glm::vec4 topright;
            glm::vec4 bottomleft;
            glm::vec4 bottomright;

            centerNormal = glm::vec3(deltaX1, 0.0f, deltaZ1);
            glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            centerTangent = glm::cross(centerNormal, planeNormal);

            for (i = 0; i < size; i++) {
                point1 = glm::vec3(populationCenters[i][0] + deltaX1 * centerRadius, -1.95f, populationCenters[i][1] + deltaZ1 * centerRadius);
                point2 = glm::vec3(populationCenters[i][0] + deltaX2 * centerRadius, -1.95f, populationCenters[i][1] + deltaZ2 * centerRadius);
                point3 = glm::vec3(populationCenters[i][0] + deltaX3 * centerRadius, -1.95f, populationCenters[i][1] + deltaZ3 * centerRadius);
                point4 = glm::vec3(populationCenters[i][0] + deltaX4 * centerRadius, -1.95f, populationCenters[i][1] + deltaZ4 * centerRadius);

                calculateExtremes(point1, centerTangent, centerNormal, waterPoints, parksPoints, bottomleft, bottomright, topleft, topright, 1.0f);
                // cout << glm::to_string(bottomleft) << endl;
                // cout << glm::to_string(bottomright) << endl;
                // cout << glm::to_string(topleft) << endl;
                // cout << glm::to_string(topright) << endl << endl;

                vertices.push_back(bottomleft);
                vertices.push_back(bottomright);
                vertices.push_back(topright);

                vertices.push_back(bottomleft);
                vertices.push_back(topright);
                vertices.push_back(topleft);

                streetsInAngle1.push_back(StreetLine(bottomright, topright));

                calculateExtremes(point2, centerTangent, centerNormal, waterPoints, parksPoints, bottomleft, bottomright, topleft, topright, 1.0f);
                // cout << glm::to_string(bottomleft) << endl;
                // cout << glm::to_string(bottomright) << endl;
                // cout << glm::to_string(topleft) << endl;
                // cout << glm::to_string(topright) << endl << endl;

                vertices.push_back(bottomleft);
                vertices.push_back(bottomright);
                vertices.push_back(topright);

                vertices.push_back(bottomleft);
                vertices.push_back(topright);
                vertices.push_back(topleft);

                streetsInAngle1.push_back(StreetLine(bottomright, topright));

                calculateExtremes(point3, centerNormal, centerTangent, waterPoints, parksPoints, bottomleft, bottomright, topleft, topright, 1.0f);
                // cout << glm::to_string(bottomleft) << endl;
                // cout << glm::to_string(bottomright) << endl;
                // cout << glm::to_string(topleft) << endl;
                // cout << glm::to_string(topright) << endl << endl;

                vertices.push_back(bottomleft);
                vertices.push_back(bottomright);
                vertices.push_back(topright);

                vertices.push_back(bottomleft);
                vertices.push_back(topright);
                vertices.push_back(topleft);
                
                streetsInAngle2.push_back(StreetLine(bottomright, topright));

                calculateExtremes(point4, centerNormal, centerTangent, waterPoints, parksPoints, bottomleft, bottomright, topleft, topright, 1.0f);
                // cout << glm::to_string(bottomleft) << endl;
                // cout << glm::to_string(bottomright) << endl;
                // cout << glm::to_string(topleft) << endl;
                // cout << glm::to_string(topright) << endl << endl;

                vertices.push_back(bottomleft);
                vertices.push_back(bottomright);
                vertices.push_back(topright);

                vertices.push_back(bottomleft);
                vertices.push_back(topright);
                vertices.push_back(topleft);

                streetsInAngle2.push_back(StreetLine(bottomright, topright));

                for (j = 0; j < 24; j++) {
                    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                }

                for (j = 0; j < 8; j++) {
                    faces.push_back(glm::uvec3(index++, index++, index++));
                }
            }

            break;
        }
        case ROAD_RADIAL: {
            break;
        }
    }
}

void StreetMap::calculateExtremes(glm::vec3 point, glm::vec3 centerTangent, glm::vec3 centerNormal, map<float, set<float>> waterPoints, map<float, set<float>> parksPoints,
                                  glm::vec4 &bottomleft, glm::vec4 &bottomright, glm::vec4 &topleft, glm::vec4 &topright, float dist) {
    float distance;
    float x;
    float z;
    glm::vec4 bottomright2;
    glm::vec4 topright2;

    distance = 0.0f;
    bottomright = bottomright2 = glm::vec4(point, 1.0f) + glm::vec4(-distance * centerTangent, 0.0f);
    while (true) {
        x = round(bottomright[0]);
        z = round(bottomright[2]);
        if (waterPoints.count(x) != 0 && waterPoints[x].count(z) != 0) {
            break;
        }
        else if (parksPoints.count(x) != 0 && parksPoints[x].count(z) != 0) {
            break;
        }
        else if (x <= -100.0f || x >= 100.0f || z <= -100.0f || z >= 100.0f) {
            break;
        }

        distance += 0.25f;
        bottomright = bottomright2;
        bottomright2 = glm::vec4(point, 1.0f) + glm::vec4(-distance * centerTangent, 0.0f);
    }

    bottomleft = bottomright + glm::vec4(dist * centerNormal, 0.0f);

    distance = 0.0f;
    topright = topright2 = glm::vec4(point, 1.0f) + glm::vec4(distance * centerTangent, 0.0f);
    while (true) {
        x = round(topright[0]);
        z = round(topright[2]);
        if (waterPoints.count(x) != 0 && waterPoints[x].count(z) != 0) {
            break;
        }
        else if (parksPoints.count(x) != 0 && parksPoints[x].count(z) != 0) {
            break;
        }
        else if (x <= -100.0f || x >= 100.0f || z <= -100.0f || z >= 100.0f) {
            break;
        }

        distance += 0.25f;
        topright = topright2;
        topright2 = glm::vec4(point, 1.0f) + glm::vec4(distance * centerTangent, 0.0f);
    }

    topleft = topright + glm::vec4(dist * centerNormal, 0.0f);
}

void StreetMap::nextIteration(map<float, set<float>> waterPoints, map<float, set<float>> parksPoints) {
    int size1 = streetsInAngle1.size();
    int size2 = streetsInAngle2.size();
    int i;
    int j;
    int length;
    float distance;
    float base;
    bool safe;
    glm::vec3 pointTmp;
    glm::vec4 point;
    glm::vec4 dir;
    glm::vec3 va;
    glm::vec3 vb;

    glm::vec4 topleft;
    glm::vec4 topright;
    glm::vec4 bottomleft;
    glm::vec4 bottomright;

    for (i = 0; i < size1; i++) {
        if (glm::length(streetsInAngle1[i].endpoint2 - streetsInAngle1[i].endpoint1) < 0.0001) {
            continue;
        } 
        dir = glm::normalize(streetsInAngle1[i].endpoint2 - streetsInAngle1[i].endpoint1);
        length = floor(glm::length(streetsInAngle1[i].endpoint2 - streetsInAngle1[i].endpoint1));
        safe = false;
        point = streetsInAngle1[i].endpoint1 + 6.0f * dir;
        while (!safe) {
            point += 0.5f * dir;
            safe = true;
            for (j = 0; j < streetsInAngle2.size(); j++) {
                va[0] = streetsInAngle2[j].endpoint2[0] - streetsInAngle2[j].endpoint1[0];
                va[1] = streetsInAngle2[j].endpoint2[1] - streetsInAngle2[j].endpoint1[1];
                va[2] = streetsInAngle2[j].endpoint2[2] - streetsInAngle2[j].endpoint1[2];
                if (glm::length(va) < 0.0001) {
                    continue;
                } 

                vb[0] = point[0] - streetsInAngle2[j].endpoint1[0];
                vb[1] = point[1] - streetsInAngle2[j].endpoint1[1];
                vb[2] = point[2] - streetsInAngle2[j].endpoint1[2];
                distance = (glm::length(glm::cross(va, vb))) / glm::length(va);
                base = sqrt(glm::length(vb) * glm::length(vb) - distance * distance);
                // cout << distance << endl;
                // cout << glm::to_string(va) << endl;
                // cout << glm::to_string(vb) << endl;
                // cout << (glm::to_string(glm::cross(va, vb))) << endl;
                // cout << (glm::length(glm::cross(va, vb))) << endl;
                // cout << glm::length(va) << endl;
                // cout << glm::to_string(streetsInAngle1[i].endpoint1) << endl;
                // cout << diff << " " << glm::to_string(dir) << endl;
                // cout << glm::to_string(point) << endl;
                // break;
                if (distance <= 6.0f && base <= glm::length(va)) {
                    safe = false;
                    break;
                }
            }
            // cout << endl;

            if (glm::length(point - streetsInAngle1[i].endpoint2) < 0.5f) {
                break;
            }
        }

        if (glm::length(point - streetsInAngle1[i].endpoint2) < 0.5f) {
            continue;
        }

        pointTmp[0] = point[0];
        pointTmp[1] = point[1];
        pointTmp[2] = point[2];

        calculateExtremes(pointTmp, centerNormal, centerTangent, waterPoints, parksPoints, bottomleft, bottomright, topleft, topright, 0.5f);
        // cout << glm::to_string(streetsInAngle1[i].endpoint1) << endl;
        // cout << diff << " " << glm::to_string(dir) << endl;
        // cout << glm::to_string(point) << endl;
        // cout << glm::to_string(pointTmp) << endl;
        // cout << glm::to_string(bottomleft) << endl;
        // cout << glm::to_string(bottomright) << endl;
        // cout << glm::to_string(topleft) << endl;
        // cout << glm::to_string(topright) << endl << endl;

        vertices.push_back(bottomleft);
        vertices.push_back(bottomright);
        vertices.push_back(topright);

        vertices.push_back(bottomleft);
        vertices.push_back(topright);
        vertices.push_back(topleft);

        for (j = 0; j < 6; j++) {
            normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        }

        for (j = 0; j < 2; j++) {
            faces.push_back(glm::uvec3(index++, index++, index++));
        }

        streetsInAngle2.push_back(StreetLine(bottomright, topright));
    }

    for (i = 0; i < size2; i++) {
        if (glm::length(streetsInAngle2[i].endpoint2 - streetsInAngle2[i].endpoint1) < 0.0001) {
            continue;
        } 
        dir = glm::normalize(streetsInAngle2[i].endpoint2 - streetsInAngle2[i].endpoint1);
        length = floor(glm::length(streetsInAngle2[i].endpoint2 - streetsInAngle2[i].endpoint1));
        safe = false;
        point = streetsInAngle2[i].endpoint1 + 6.0f * dir;
        while (!safe) {
            point += 0.5f * dir;
            safe = true;
            for (j = 0; j < streetsInAngle1.size(); j++) {
                va[0] = streetsInAngle1[j].endpoint2[0] - streetsInAngle1[j].endpoint1[0];
                va[1] = streetsInAngle1[j].endpoint2[1] - streetsInAngle1[j].endpoint1[1];
                va[2] = streetsInAngle1[j].endpoint2[2] - streetsInAngle1[j].endpoint1[2];
                if (glm::length(va) < 0.0001) {
                    continue;
                } 

                vb[0] = point[0] - streetsInAngle1[j].endpoint1[0];
                vb[1] = point[1] - streetsInAngle1[j].endpoint1[1];
                vb[2] = point[2] - streetsInAngle1[j].endpoint1[2];
                distance = (glm::length(glm::cross(va, vb))) / glm::length(va);
                base = sqrt(glm::length(vb) * glm::length(vb) - distance * distance);
                // cout << distance << endl;
                // cout << glm::to_string(va) << endl;
                // cout << glm::to_string(vb) << endl;
                // cout << (glm::to_string(glm::cross(va, vb))) << endl;
                // cout << (glm::length(glm::cross(va, vb))) << endl;
                // cout << glm::length(va) << endl;
                // cout << glm::to_string(streetsInAngle1[i].endpoint1) << endl;
                // cout << diff << " " << glm::to_string(dir) << endl;
                // cout << glm::to_string(point) << endl;
                // break;
                if (distance <= 6.0f && base <= glm::length(va)) {
                    safe = false;
                    break;
                }
            }
            // cout << endl;

            if (glm::length(point - streetsInAngle2[i].endpoint2) < 0.5f) {
                break;
            }
        }

        if (glm::length(point - streetsInAngle2[i].endpoint2) < 0.5f) {
            continue;
        }

        pointTmp[0] = point[0];
        pointTmp[1] = point[1];
        pointTmp[2] = point[2];

        calculateExtremes(pointTmp, centerTangent, centerNormal, waterPoints, parksPoints, bottomleft, bottomright, topleft, topright, 0.5f);
        // cout << glm::to_string(streetsInAngle1[i].endpoint1) << endl;
        // cout << diff << " " << glm::to_string(dir) << endl;
        // cout << glm::to_string(point) << endl;
        // cout << glm::to_string(pointTmp) << endl;
        // cout << glm::to_string(bottomleft) << endl;
        // cout << glm::to_string(bottomright) << endl;
        // cout << glm::to_string(topleft) << endl;
        // cout << glm::to_string(topright) << endl << endl;

        vertices.push_back(bottomleft);
        vertices.push_back(bottomright);
        vertices.push_back(topright);

        vertices.push_back(bottomleft);
        vertices.push_back(topright);
        vertices.push_back(topleft);

        for (j = 0; j < 6; j++) {
            normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        }

        for (j = 0; j < 2; j++) {
            faces.push_back(glm::uvec3(index++, index++, index++));
        }

        streetsInAngle1.push_back(StreetLine(bottomright, topright));
    }
}


// Altered version of http://pastebin.com/03BigiCn
bool StreetLine::intersects(glm::vec2 avec, glm::vec2 bvec, glm::vec2 cvec, glm::vec2 dvec, float width) const {
    glm::vec2 ep1(endpoint1.x, endpoint1.z), ep2(endpoint2.x, endpoint2.z);
    glm::vec2 length = ep2 - ep1;
    glm::vec2 tangent = (glm::vec2(length.y, -length.x)*(1/sqrt(length.y*length.y + length.x*length.x)))*width;
    std::vector<glm::vec2> a = {avec, bvec, cvec, dvec};
    std::vector<glm::vec2> b = {ep1+tangent, ep2+tangent, ep2-tangent, ep1-tangent};

    for(int polyi = 0; polyi < 2; ++polyi)
    {
        const std::vector<glm::vec2>& polygon = polyi == 0 ? a : b;

        for(int i1 = 0; i1 < polygon.size(); ++i1)
        {
            const int i2 = (i1 + 1) % polygon.size();

            const double normalx = polygon[i2].y - polygon[i1].y;
            const double normaly = polygon[i2].x - polygon[i1].x;

            double minA = std::numeric_limits<double>::max();
            double maxA = std::numeric_limits<double>::min();
            for(int ai = 0; ai < a.size(); ++ai)
            {
                const double projected = normalx * a[ai].x +
                                         normaly * a[ai].y;
                if( projected < minA ) minA = projected;
                if( projected > maxA ) maxA = projected;
            }

            double minB = std::numeric_limits<double>::max();
            double maxB = std::numeric_limits<double>::min();
            for(int bi = 0; bi < b.size(); ++bi)
            {
                const double projected = normalx * b[bi].x +
                                         normaly * b[bi].y;
                if( projected < minB ) minB = projected;
                if( projected > maxB ) maxB = projected;
            }

            if( maxA < minB || maxB < minA )
                return false;
        }
    }

    return true;
}