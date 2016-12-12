//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_PARSER_H
#define PROCEDURALCITIES_PARSER_H

#include <generation/street/Modules.h>
#include <generation/street/GlobalGoals.h>
#include <generation/street/LocalConstraints.h>

#include <generation/street/StreetSegment.h>
#include <generation/street/Intersection.h>

class Parser {
public:
    Parser(const GlobalGoals& globalGoals, const LocalConstraints& localConstraints);

    std::vector<Module*> substitution();

    void parse(std::vector<StreetSegment>& streets, std::vector<Intersection>& intersection);

    ~Parser();
private:
    std::vector<Module*> modules;
    GlobalGoals globalGoals;
    LocalConstraints localConstraints;
};


#endif //PROCEDURALCITIES_PARSER_H
