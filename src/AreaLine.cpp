#include <fstream>
#include <iostream>
#include <string.h>
#include <math.h>

#include <glm/gtx/string_cast.hpp>

#include "AreaLine.h"

using namespace std;

Line::Line() {
    index = 0;
    center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Line::sort() {
    float angleA;
    float angleB;
    float xA;
    float zA;
    float xB;
    float zB;
    int size = vertices.size();
    int i;
    int j;
    glm::vec4 tmp;
    
    for (i = 0; i < size - 1; i++) {
        xA = vertices[i][0] - center[0];
        zA = vertices[i][2] - center[2];
        angleA = atan2(zA, xA);
        for (j = i + 1; j < size; j++) {
            xB = vertices[j][0] - center[0];
            zB = vertices[j][2] - center[2];
            angleB = atan2(zB, xB);

            if (angleB < angleA) {
                tmp = vertices[j];
                vertices[j] = vertices[i];
                vertices[i] = tmp;
            }
        }
    }
}

AreaLine::AreaLine() {
    int lineNumber = 0;
    int i;

    ifstream arch;
    arch.open("resources/model/area.map");
    string line;
    while (getline(arch, line)) {
        if (lineNumber == 200) {
            break;
        }

        for (i = 0; i < 200; i++) {
            areaMap[lineNumber][i] = (line[i] - '0');
        }

        lineNumber++;
    }
    arch.close();

    for (i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            int type = areaMap[i][j];
            if (type != 0 && type != -1 && checkSurrounding(i, j, type)) {
                Line ln;
                createLine(ln, i, j, type);
                ln.center[0] /= ln.vertices.size();
                ln.center[2] /= ln.vertices.size();
                ln.sort();
                if (type == 1) {
                    waterLines.push_back(ln);
                }
                else {
                    parksLines.push_back(ln);
                }
            }
        }
    }
}

void AreaLine::createLine(Line &line, int i, int j, int type) {
    areaMap[i][j] = -1;
    int found = false;

    if (i - 1 >= 0) {
        if (areaMap[i - 1][j] != type && areaMap[i - 1][j] != - 1) {
            found = true;
        }
        else if (checkSurrounding(i - 1, j, type)){
            createLine(line, i - 1, j, type);
        }
        if (j - 1 >= 0) {
            if (areaMap[i - 1][j - 1] != type && areaMap[i - 1][j - 1] != - 1) {
                found = true;
            }
            else if (checkSurrounding(i - 1, j - 1, type)) {
                createLine(line, i - 1, j - 1, type);
            }
        }
        if (j + 1 < 200) {
            if (areaMap[i - 1][j + 1] != type && areaMap[i - 1][j + 1] != -1) {
                found = true;
            }
            else if (checkSurrounding(i - 1, j + 1, type)){
                createLine(line, i - 1, j + 1, type);
            }
        }
    }
    if (i + 1 < 200) {
        if (areaMap[i + 1][j] != type && areaMap[i + 1][j] != -1) {
            found = true;
        }
        else if (checkSurrounding(i + 1, j, type)){
            createLine(line, i + 1, j, type);
        }
        if (j - 1 >= 0) {
            if (areaMap[i + 1][j - 1] != type && areaMap[i + 1][j - 1] != -1) {
                found = true;
            }
            else if (checkSurrounding(i + 1, j - 1, type)) {
                createLine(line, i + 1, j - 1, type);
            }
        }
        if (j + 1 < 200) {
            if (areaMap[i + 1][j + 1] != type && areaMap[i + 1][j + 1] != -1) {
                found = true;
            }
            else if (checkSurrounding(i + 1, j + 1, type)){
                createLine(line, i + 1, j + 1, type);
            }
        }
    }
    if (j - 1 >= 0) {
        if (areaMap[i][j - 1] != type && areaMap[i][j - 1] != -1) {
            found = true;
        }
        else if (checkSurrounding(i, j - 1, type)){
            createLine(line, i, j - 1, type);
        }
    }
    if (j + 1 < 200) {
        if (areaMap[i][j + 1] != type && areaMap[i][j + 1] != -1) {
            found = true;
        }
        else if (checkSurrounding(i, j + 1, type)){
            createLine(line, i, j + 1, type);
        }
    }


    if (found) {
        line.vertices.push_back(glm::vec4(j - 100, -1.90f, i - 100, 1.0f));
        line.vertices.push_back(glm::vec4(j - 100, -1.90f, i - 100, 1.0f));

        line.normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        line.normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

        line.faces.push_back(glm::uvec2(line.index++, line.index++));

        line.center[0] += 2 * (j - 100);
        line.center[2] += 2 * (i - 100);
    }
}

bool AreaLine::checkSurrounding(int i, int j, int type) {
    if (areaMap[i][j] == -1) {
        return false;
    }

    if (i - 1 >= 0) {
        if (areaMap[i - 1][j] != type && areaMap[i - 1][j] != -1) {
            return true;
        }
        if (j - 1 >= 0 && areaMap[i - 1][j - 1] != type && areaMap[i - 1][j - 1] != -1) {
            return true;
        }
        if (j + 1 < 200 && areaMap[i - 1][j + 1] != type && areaMap[i - 1][j + 1] != -1) {
            return true;
        }
    }
    if (i + 1 < 200) {
        if (areaMap[i + 1][j] != type && areaMap[i + 1][j] != -1) {
            return true;
        }
        if (j - 1 >= 0 && areaMap[i + 1][j - 1] != type && areaMap[i + 1][j - 1] != -1) {
            return true;
        }
        if (j + 1 < 200 && areaMap[i + 1][j + 1] != type && areaMap[i + 1][j + 1] != -1) {
            return true;
        }
    }
    if (j - 1 >= 0 && areaMap[i][j - 1] != type && areaMap[i][j - 1] != -1) {
        return true;
    }
    if (j + 1 < 200 && areaMap[i][j + 1] != type && areaMap[i][j + 1] != -1) {
        return true;
    }

    return false;
}