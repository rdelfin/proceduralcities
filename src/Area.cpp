#include <fstream>
#include <iostream>
#include <string.h>

#include "Area.h"

using namespace std;

Area::Area() {
    int parksIndex = 0;
    int waterIndex = 0;
    int i;

    float x;
    float z = -100.0f;

    ifstream arch;
    arch.open("resources/model/area.map");
    string line;
    while (getline(arch, line)) {
        x = -100.0f;
        for (i = 0; i < 200; i++) {
            if (line[i] == '1') {
                waterVertices.push_back(glm::vec4(x - 0.2f, -1.90f, z - 0.2f, 1.0f));
                waterVertices.push_back(glm::vec4(x + 1.2f, -1.90f, z - 0.2f, 1.0f));
                waterVertices.push_back(glm::vec4(x + 1.2f, -1.90f, z + 1.2f, 1.0f));

                waterVertices.push_back(glm::vec4(x - 0.2f, -1.90f, z - 0.2f, 1.0f));
                waterVertices.push_back(glm::vec4(x + 1.2f, -1.90f, z + 1.2f, 1.0f));
                waterVertices.push_back(glm::vec4(x - 0.2f, -1.90f, z + 1.2f, 1.0f));

                for (int i = 0; i < 6; i++) {
                    waterNormals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                }

                waterFaces.push_back(glm::uvec3(waterIndex++, waterIndex++, waterIndex++));
                waterFaces.push_back(glm::uvec3(waterIndex++, waterIndex++, waterIndex++));

                if (waterPoints.count(x) == 0) {
                    waterPoints[x] = set<float>();
                }
                
                waterPoints[x].insert(z);
            }
            else if (line[i] == '2') {
                parksVertices.push_back(glm::vec4(x - 0.2f, -1.90f, z - 0.2f, 1.0f));
                parksVertices.push_back(glm::vec4(x + 1.2f, -1.90f, z - 0.2f, 1.0f));
                parksVertices.push_back(glm::vec4(x + 1.2f, -1.90f, z + 1.2f, 1.0f));

                parksVertices.push_back(glm::vec4(x - 0.2f, -1.90f, z - 0.2f, 1.0f));
                parksVertices.push_back(glm::vec4(x + 1.2f, -1.90f, z + 1.2f, 1.0f));
                parksVertices.push_back(glm::vec4(x - 0.2f, -1.90f, z + 1.2f, 1.0f));

                for (int i = 0; i < 6; i++) {
                    parksNormals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                }

                parksFaces.push_back(glm::uvec3(parksIndex++, parksIndex++, parksIndex++));
                parksFaces.push_back(glm::uvec3(parksIndex++, parksIndex++, parksIndex++));

                if (parksPoints.count(x) == 0) {
                    parksPoints[x] = set<float>();
                }
                
                parksPoints[x].insert(z);
            }

            x += 1.0f;
        }

        z += 1.0f;
    }
    arch.close();

    arch.open("resources/model/population.map");
    z = -100.0f;
    while (getline(arch, line)) {
        x = -100.0f;
        for (i = 0; i < 200; i++) {
            if (line[i] == '1' && waterPoints[x].count(z) == 0 && parksPoints[x].count(z) == 0) {
                populationCenters.push_back(glm::vec2(x, z));
            }
            x += 1.0f;
        }

        z += 1.0f;
    }
    arch.close();
}