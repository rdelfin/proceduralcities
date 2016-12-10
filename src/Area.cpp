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
                waterVertices.push_back(glm::vec4(x - 1.0f, -1.95f, z - 1.0f, 1.0f));
                waterVertices.push_back(glm::vec4(x + 2.0f, -1.95f, z - 1.0f, 1.0f));
                waterVertices.push_back(glm::vec4(x + 2.0f, -1.95f, z + 2.0f, 1.0f));

                waterVertices.push_back(glm::vec4(x - 1.0f, -1.95f, z - 1.0f, 1.0f));
                waterVertices.push_back(glm::vec4(x + 2.0f, -1.95f, z + 2.0f, 1.0f));
                waterVertices.push_back(glm::vec4(x - 1.0f, -1.95f, z + 2.0f, 1.0f));

                for (int i = 0; i < 6; i++) {
                    waterNormals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                }

                waterFaces.push_back(glm::uvec3(waterIndex++, waterIndex++, waterIndex++));
                waterFaces.push_back(glm::uvec3(waterIndex++, waterIndex++, waterIndex++));
            }
            else if (line[i] == '2') {
                parksVertices.push_back(glm::vec4(x, -1.95f, z - 2.0f, 1.0f));
                parksVertices.push_back(glm::vec4(x + 2.0f, -1.95f, z - 2.0f, 1.0f));
                parksVertices.push_back(glm::vec4(x + 2.0f, -1.95f, z, 1.0f));

                parksVertices.push_back(glm::vec4(x, -1.95f, z - 2.0f, 1.0f));
                parksVertices.push_back(glm::vec4(x + 2.0f, -1.95f, z, 1.0f));
                parksVertices.push_back(glm::vec4(x, -1.95f, z, 1.0f));

                for (int i = 0; i < 6; i++) {
                    parksNormals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                }

                parksFaces.push_back(glm::uvec3(parksIndex++, parksIndex++, parksIndex++));
                parksFaces.push_back(glm::uvec3(parksIndex++, parksIndex++, parksIndex++));
            }

            x += 1.0f;
        }

        z += 1.0f;
    }
    arch.close();
}