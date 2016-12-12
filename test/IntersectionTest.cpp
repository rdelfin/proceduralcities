//
// Created by rdelfin on 12/12/16.
//

#include <iostream>
#include <StreetMap.h>

int main() {
    std::cout << "TESTING RECTANGLE INTERSECTION" << std::endl;

    StreetLine sl(glm::vec4(3.0f, 0.0f, 1.0f, 1.0f), glm::vec4(7.0f, 0.0f, 1.0f, 1.0f));
    bool int1 = sl.intersects(glm::vec2(1.0f, 1.0f), glm::vec2(2.0f, 0.0f), glm::vec2(3.0f, 2.0f), glm::vec2(3.0f, 3.0f), 1.0f);

    std::cout << "INTERSECT 1: " << int1 << " (should be true)" << std::endl;
}