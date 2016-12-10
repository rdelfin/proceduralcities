#ifndef PROCEDURALCITIES_AREALINE_H
#define PROCEDURALCITIES_AREALINE_H

#include <string>
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Line {
    public:
        Line();

        vector<glm::vec4> vertices;
        vector<glm::vec4> normals;
        vector<glm::uvec2> faces;

        glm::vec4 center;

        int index;

        void sort();
};

class AreaLine {
    public:
        AreaLine();

        float areaMap[200][200];

        vector<Line> waterLines;
        vector<Line> parksLines;

    private:
        void createLine(Line &line, int i, int j, int type);
        bool checkSurrounding(int i, int j, int type);
};

#endif // PROCEDURALCITIES_AREALINE_H