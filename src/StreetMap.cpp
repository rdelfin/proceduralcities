#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265

#include "StreetMap.h"

StreetMap::StreetMap(enum roadPattern pattern, vector<glm::vec2> populationCenters) {
    srand (time(NULL));
    
    int index = 0;
    int i;
    int j;
    int size = populationCenters.size();
    int centerRadius = 10.0f;

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
            glm::vec3 point1;
            glm::vec3 point2;
            glm::vec3 point3;
            glm::vec3 point4;

            glm::vec3 centerNormal = glm::vec3(deltaX1, 0.0f, deltaZ1);
            glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 centerTangent = glm::cross(centerNormal, planeNormal);

            for (i = 0; i < size; i++) {
                point1 = glm::vec3(populationCenters[i][0] + deltaX1 * centerRadius, 0.0f, populationCenters[i][1] + deltaZ1 * centerRadius);
                point2 = glm::vec3(populationCenters[i][0] + deltaX2 * centerRadius, 0.0f, populationCenters[i][1] + deltaZ2 * centerRadius);
                point3 = glm::vec3(populationCenters[i][0] + deltaX3 * centerRadius, 0.0f, populationCenters[i][1] + deltaZ3 * centerRadius);
                point4 = glm::vec3(populationCenters[i][0] + deltaX4 * centerRadius, 0.0f, populationCenters[i][1] + deltaZ4 * centerRadius);

                vertices.push_back(glm::vec4(point1, 1.0f) + glm::vec4(2.0f * centerNormal, 0.0f) + glm::vec4(-50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point1, 1.0f) + glm::vec4(-50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point1, 1.0f) + glm::vec4(50.0f * centerTangent, 0.0f));

                vertices.push_back(glm::vec4(point1, 1.0f) + glm::vec4(2.0f * centerNormal, 0.0f) + glm::vec4(-50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point1, 1.0f) + glm::vec4(50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point1, 1.0f) + glm::vec4(2.0f * centerNormal, 0.0f) + glm::vec4(50.0f * centerTangent, 0.0f));

                vertices.push_back(glm::vec4(point2, 1.0f) + glm::vec4(-2.0f * centerNormal, 0.0f) + glm::vec4(-50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point2, 1.0f) + glm::vec4(-50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point2, 1.0f) + glm::vec4(50.0f * centerTangent, 0.0f));

                vertices.push_back(glm::vec4(point2, 1.0f) + glm::vec4(-2.0f * centerNormal, 0.0f) + glm::vec4(-50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point2, 1.0f) + glm::vec4(50.0f * centerTangent, 0.0f));
                vertices.push_back(glm::vec4(point2, 1.0f) + glm::vec4(-2.0f * centerNormal, 0.0f) + glm::vec4(50.0f * centerTangent, 0.0f));

                vertices.push_back(glm::vec4(point3, 1.0f) + glm::vec4(2.0f * centerTangent, 0.0f) + glm::vec4(-50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point3, 1.0f) + glm::vec4(-50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point3, 1.0f) + glm::vec4(50.0f * centerNormal, 0.0f));

                vertices.push_back(glm::vec4(point3, 1.0f) + glm::vec4(2.0f * centerTangent, 0.0f) + glm::vec4(-50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point3, 1.0f) + glm::vec4(50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point3, 1.0f) + glm::vec4(2.0f * centerTangent, 0.0f) + glm::vec4(50.0f * centerNormal, 0.0f));

                vertices.push_back(glm::vec4(point4, 1.0f) + glm::vec4(-2.0f * centerTangent, 0.0f) + glm::vec4(-50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point4, 1.0f) + glm::vec4(-50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point4, 1.0f) + glm::vec4(50.0f * centerNormal, 0.0f));

                vertices.push_back(glm::vec4(point4, 1.0f) + glm::vec4(-2.0f * centerTangent, 0.0f) + glm::vec4(-50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point4, 1.0f) + glm::vec4(50.0f * centerNormal, 0.0f));
                vertices.push_back(glm::vec4(point4, 1.0f) + glm::vec4(-2.0f * centerTangent, 0.0f) + glm::vec4(50.0f * centerNormal, 0.0f));

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