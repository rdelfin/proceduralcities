//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_PARSER_H
#define PROCEDURALCITIES_PARSER_H

#include <Parser.h>
#include <Modules.h>

class Parser {
private:
    Parser();

    std::vector<Module*> parse();

    ~Parser();
public:
    std::vector<Module*> modules;
};


#endif //PROCEDURALCITIES_PARSER_H
