#ifndef PROCEDURALCITIES_BUILDING_H
#define PROCEDURALCITIES_BUILDING_H

#include <vector>

#include <glm/glm.hpp>

#include <TriangleMesh.h>

using namespace std;

#define FLOOR_HEIGHT 1.0f

enum {
    SKYSCRAPER,
    BUILDING,
    HOUSE
} typedef buildingStyle;

class Building : public TriangleMesh {
    public:
        Building(float w, float l, float centerDistance, const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
                 const std::vector<ShaderUniform>& uniforms);
        void nextIteration();
        void generateRenderData();

        float w;
        float l;
        float centerDistance;
        buildingStyle style;
        int height;
        int type;
    
    private:
        void quad(glm::vec4 points[], int a, int b, int c, int d);
        void drawPrism(float minx, float miny, float minz, float maxx, float maxy, float maxz);
        void drawU(float minx, float miny, float minz, float maxx, float maxy, float maxz);
        void drawPyramid(float minx, float minz, float maxx, float maxz);
        void drawL(float minx, float miny, float minz, float maxx, float maxy, float maxz);
        void drawPointyTower(float minx, float minz, float maxx, float maxz);
        int index;
};

#endif //PROCEDURALCITIES_BUILDING_H