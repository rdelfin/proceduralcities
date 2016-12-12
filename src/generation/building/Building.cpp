#include <generation/building/Building.h>
#include <iostream>

#include <glm/gtx/string_cast.hpp>

using namespace std;

Building::Building(float w, float l, float centerDistance, const Shader& vertexShader, const Shader& geometryShader, const Shader& fragmentShader,
                   const std::vector<ShaderUniform>& uniforms) {
    this->w = w;
    this->l = l;
    this->centerDistance = centerDistance;
    index = 0;
    
    nextIteration();
    generateRenderData();

    constructorImpl(vertices, normals, faces, vertexShader, geometryShader, fragmentShader, uniforms);
}

void Building::nextIteration() {
    if (centerDistance <= 20 + (rand() % 10 + 1)) {
        style = SKYSCRAPER;
    }
    else if (centerDistance <= 40 + (rand() % 10 + 1)) {
        style = BUILDING;
    }
    else {
        style = HOUSE;
    }

    switch (style) {
        case SKYSCRAPER: {
            height = rand() % 22 + 9;
            type = rand() % 8 + 1;
            break;
        }
        case BUILDING: {
            height = rand() % 5 + 4;
            type = rand() % 5 + 1;
            break;
        }
        case HOUSE: {
            height = rand() % 3 + 1;
            type = rand() % 3 + 1;
            break;
        }
    }

    type = 1;
    height = 30;
}

void Building::quad(glm::vec4 points[], int a, int b, int c, int d) {
	// Generate first triangle
	vertices.push_back(points[a]);
	vertices.push_back(points[b]);
	vertices.push_back(points[c]);

	// Generate second triangle
	vertices.push_back(points[a]);
	vertices.push_back(points[c]);
	vertices.push_back(points[d]);
}

void Building::drawPrism(float minx, float miny, float minz, float maxx, float maxy, float maxz) {
	// Define vertices
	glm::vec4 points[8] = {
		glm::vec4(minx, miny, maxz, 1.0f),
		glm::vec4(minx, maxy, maxz, 1.0f),
		glm::vec4(maxx, maxy, maxz, 1.0f),
		glm::vec4(maxx, miny, maxz, 1.0f),
		glm::vec4(minx, miny, minz, 1.0f),
		glm::vec4(minx, maxy, minz, 1.0f),
		glm::vec4(maxx, maxy, minz, 1.0f),
		glm::vec4(maxx, miny, minz, 1.0f),
	};

	// Generate all the cubes faces
	// Back
	quad(points, 1, 0, 3, 2);
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

	// Right
    quad(points, 2, 3, 7, 6);
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Top
    quad(points, 6, 5, 1, 2); 
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Front
    quad(points, 4, 5, 6, 7);
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Left
    quad(points, 5, 4, 0, 1);
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
}

void Building::drawU(float minx, float miny, float minz, float maxx, float maxy, float maxz) {
	float x1 = minx + (maxx - minx) / 3;
    float z1 = minz + (maxz - minz) / 3;
    float x2 = minx + 2 *(maxx - minx) / 3;

    // Define vertices
	glm::vec4 points[20] = {
		glm::vec4(minx, miny, maxz, 1.0f),          // 0
		glm::vec4(minx, maxy, maxz, 1.0f),          // 1
		glm::vec4(maxx, maxy, maxz, 1.0f),          // 2
		glm::vec4(maxx, miny, maxz, 1.0f),          // 3
		glm::vec4(minx, miny, minz, 1.0f),          // 4
		glm::vec4(minx, maxy, minz, 1.0f),          // 5
		glm::vec4(maxx, maxy, minz, 1.0f),          // 6
		glm::vec4(maxx, miny, minz, 1.0f),          // 7
        glm::vec4(x1, miny, minz, 1.0f),            // 8
		glm::vec4(x1, miny, z1, 1.0f),              // 9
		glm::vec4(x2, miny, z1, 1.0f),              // 10
		glm::vec4(x2, miny, minz, 1.0f),            // 11
		glm::vec4(x1, maxy, minz, 1.0f),            // 12
		glm::vec4(x1, maxy, z1, 1.0f),              // 13
		glm::vec4(x2, maxy, z1, 1.0f),              // 14
		glm::vec4(x2, maxy, minz, 1.0f),            // 15
        glm::vec4(x1, maxy, maxz, 1.0f),            // 16
        glm::vec4(x2, maxy, maxz, 1.0f),            // 17
        glm::vec4(x1, miny, maxz, 1.0f),            // 18
        glm::vec4(x2, miny, maxz, 1.0f),            // 19
	};

	// Generate all the cubes faces
	// Back
	quad(points, 1, 0, 3, 2);
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

	// Right
    quad(points, 2, 3, 7, 6);
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Left
    quad(points, 5, 4, 0, 1);
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Front-Left
    quad(points, 12, 8, 4, 5); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Front-center
    quad(points, 14, 10, 9, 13); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Front-right
    quad(points, 6, 7, 11, 15);
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // U-left
    quad(points, 13, 9, 8, 12);
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // U-right
    quad(points, 15, 11, 10, 14);
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Top-left
    quad(points, 1, 5, 12, 16);
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Top-center
    quad(points, 16, 13, 14, 17);
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Top-right
    quad(points, 17, 15, 6, 2);
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
}

void Building::drawPyramid(float minx, float minz, float maxx, float maxz) {
    float h1 = height * FLOOR_HEIGHT;
    float h2 = h1 + (h1 / 2.5f);
    float middleX = minx + (maxx - minx) / 2.0f;
    float middleZ = minz + (maxz - minz) / 2.0f;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 point1 = glm::vec3(minx, h1, minz);
    glm::vec3 point2 = glm::vec3(maxx, h1, minz);
    glm::vec3 point3 = glm::vec3(maxx, h1, maxz);
    glm::vec3 point4 = glm::vec3(minx, h1, maxz);
    glm::vec3 peak = glm::vec3(middleX, h2, middleZ);

    vertices.push_back(glm::vec4(point1, 1.0f));
    vertices.push_back(glm::vec4(point2, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point1 - point2;
    v2 = peak - glm::vec3(middleX, h1, minz);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    vertices.push_back(glm::vec4(point2, 1.0f));
    vertices.push_back(glm::vec4(point3, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point2 - point3;
    v2 = peak - glm::vec3(maxx, h1, middleZ);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    vertices.push_back(glm::vec4(point3, 1.0f));
    vertices.push_back(glm::vec4(point4, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point3 - point4;
    v2 = peak - glm::vec3(middleX, h1, maxz);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    vertices.push_back(glm::vec4(point4, 1.0f));
    vertices.push_back(glm::vec4(point1, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point4 - point1;
    v2 = peak - glm::vec3(minx, h1, middleZ);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
}

void Building::drawL(float minx, float miny, float minz, float maxx, float maxy, float maxz) {
    // Define vertices
    float x1 = minx + (maxx - minx) * .25f;
    float z1 = minz + (maxz - minz) * .75f;
	glm::vec4 points[13] = {
        glm::vec4(minx, miny, maxz, 1.0f),  // 0
		glm::vec4(minx, maxy, maxz, 1.0f),  // 1
		glm::vec4(maxx, maxy, maxz, 1.0f),  // 2
		glm::vec4(maxx, miny, maxz, 1.0f),  // 3
		glm::vec4(minx, miny, minz, 1.0f),  // 4
		glm::vec4(minx, maxy, minz, 1.0f),  // 5
        glm::vec4(x1, miny, minz, 1.0f),    // 6
		glm::vec4(x1, maxy, minz, 1.0f),    // 7
		glm::vec4(maxx, miny, z1, 1.0f),    // 8
		glm::vec4(maxx, maxy, z1, 1.0f),    // 9
        glm::vec4(x1, miny, z1, 1.0f),      // 10
		glm::vec4(x1, maxy, z1, 1.0f),      // 11
        glm::vec4(x1, maxy, maxz, 1.0f),    // 12
	};

	// Generate all the cubes faces
	// Back
	quad(points, 1, 0, 3, 2);
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

	// Right back
    quad(points, 3, 2, 9, 8);
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Right front
    quad(points, 11, 10, 6, 7);
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Top Left
    quad(points, 1, 5, 7, 12); 
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Top Right
    quad(points, 12, 11, 9, 2); 
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Front Left
    quad(points, 5, 4, 6, 7);
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));

    // Front Right
    quad(points, 11, 10, 8, 9);
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); 
    normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    
    // Left
    quad(points, 5, 4, 0, 1);
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
}

void Building::drawPointyTower(float minx, float minz, float maxx, float maxz) {
    float h1 = height * FLOOR_HEIGHT;
    float h2 = h1 + (h1 / 1.5f);
    float x1 = minx + (maxx - minx) / 4.0f;
    float z1 = minz + (maxz - minz) / 4.0f;
    float x2 = minx + 3.0f * (maxx - minx) / 4.0f;
    float z2 = minz + 3.0f * (maxz - minz) / 4.0f;
    float middleX = minx + (maxx - minx) / 2.0f;
    float middleZ = minz + (maxz - minz) / 2.0f;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 point1 = glm::vec3(x1, h1, z1);
    glm::vec3 point2 = glm::vec3(x2, h1, z1);
    glm::vec3 point3 = glm::vec3(x2, h1, z2);
    glm::vec3 point4 = glm::vec3(x1, h1, z2);
    glm::vec3 peak = glm::vec3(middleX, h2, middleZ);

    vertices.push_back(glm::vec4(point1, 1.0f));
    vertices.push_back(glm::vec4(point2, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point1 - point2;
    v2 = peak - glm::vec3(middleX, h1, minz);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    vertices.push_back(glm::vec4(point2, 1.0f));
    vertices.push_back(glm::vec4(point3, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point2 - point3;
    v2 = peak - glm::vec3(maxx, h1, middleZ);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    vertices.push_back(glm::vec4(point3, 1.0f));
    vertices.push_back(glm::vec4(point4, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point3 - point4;
    v2 = peak - glm::vec3(middleX, h1, maxz);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    vertices.push_back(glm::vec4(point4, 1.0f));
    vertices.push_back(glm::vec4(point1, 1.0f));
    vertices.push_back(glm::vec4(peak, 1.0f));
    v1 = point4 - point1;
    v2 = peak - glm::vec3(minx, h1, middleZ);
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));
    normals.push_back(glm::vec4(glm::normalize(glm::cross(v1, v2)), 0.0f));

    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
    faces.push_back(glm::uvec3(index++, index++, index++));
	faces.push_back(glm::uvec3(index++, index++, index++));
}

void Building::generateRenderData() {
    switch (type) {
        // Rectangular building
        case 1: {
            drawPrism(-3.0f, 0, -3.0f, 3.0f, height * FLOOR_HEIGHT, 3.0f);

            break;
        }
        // U-Shaped building
        case 2: {
            drawU(-10.0f, 0, -10.0f, 10.0f, height * FLOOR_HEIGHT, 10.0f);

            break;
        }
        // Pointy building
        case 3: {
            drawPrism(-10.0f, 0, -10.0f, 10.0f, height * FLOOR_HEIGHT, 10.0f);
            drawPyramid(-10.0f, -10.0f, 10.0f, 10.0f);
            break;
        }
        // L building
        case 4: { 
            drawL(-10.0f, 0, -10.0f, 10.0f, height * FLOOR_HEIGHT, 10.0f);
            break;
        }
        // Rectangular and L combination
        case 5: {
            float percentage = ((rand() % 21) + 60) / 100.0f;
            float h1 = height * percentage;
            float h2 = height;
            drawPrism(-10.0f, 0, -10.0f, 10.0f, h1 * FLOOR_HEIGHT, 10.0f);
            drawL(-10.0f, h1 * FLOOR_HEIGHT, -10.0f, 10.0f, h2 * FLOOR_HEIGHT, 10.0f);
            break;
        }
        // Pointy tower
        case 6: {
            drawPrism(-10.0f, 0, -10.0f, 10.0f, height * FLOOR_HEIGHT, 10.0f);
            drawPointyTower(-10.0f, -10.0f, 10.0f, 10.0f);
            break;
        }
        // Reducing building
        case 7: {
            int iterations = ceil(height * FLOOR_HEIGHT / 3.0f);
            float minx = -10.0f;
            float minz = -10.0f;
            float maxx = 10.0f;
            float maxz = 10.0f;
            float h1;
            float h2;
            for (int i = 0; i < iterations; i++) {
                h1 = i * 3 * FLOOR_HEIGHT;
                h2 = (i + 1) * 3 * FLOOR_HEIGHT;
                drawPrism(minx, h1, minz, maxx, h2, maxz);

                minx += 0.5f;
                minz += 0.5f;
                maxx -= 0.5f;
                maxz -= 0.5f;
            }
            break;
        }
        case 8: {
            int iterations = ceil(height * FLOOR_HEIGHT / 3.0f);
            float minx = -10.0f;
            float minz = -10.0f;
            float maxx = 10.0f;
            float maxz = 10.0f;
            float h1;
            float h2;
            for (int i = 0; i < iterations; i++) {
                h1 = i * 3 * FLOOR_HEIGHT;
                h2 = (i + 1) * 3 * FLOOR_HEIGHT;
                drawPrism(minx, h1, minz, maxx, h2, maxz);

                minx += 0.5f;
                minz += 0.5f;
                maxx -= 0.5f;
                maxz -= 0.5f;
            }
            drawPointyTower(minx, minz, maxx, maxz);
            break;
        }
        default: {
            break;
        }
    }
}