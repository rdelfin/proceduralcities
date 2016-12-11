//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/Parser.h>
#include <generation/street/Modules.h>
#include <iostream>

int main() {
    GlobalGoals globalGoals;
    LocalConstraints localConstraints;
    Parser parser(globalGoals, localConstraints);
glm::vec2() + glm::vec2();
    for(int i = 0; i < 10; i++) {
        std::vector<Module *> iteration = parser.parse();
        std::cout << "Iteration " << i << ": ";
        for (Module *module : iteration) {
            if (dynamic_cast<RoadModule *>(module)) std::cout << "{ROAD MODULE} ";
            if (dynamic_cast<InquiryModule *>(module)) std::cout << "{INQUIRY MODULE} ";
            if (dynamic_cast<BranchModule *>(module)) std::cout << "{BRANCH MODULE} ";
            if (dynamic_cast<TerminationModule *>(module)) std::cout << "{TERMINATION MODULE} ";
            if (dynamic_cast<DrawnRoadModule *>(module)) std::cout << "{DRAWN ROAD MODULE} ";
            if (dynamic_cast<StartModule *>(module)) std::cout << "{START MODULE} ";
            if (dynamic_cast<EndModule *>(module)) std::cout << "{END MODULE} ";
        }
        std::cout << std::endl;
    }
}