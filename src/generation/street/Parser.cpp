//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/Parser.h>

#include <generation/street/Modules.h>

Parser::Parser() {
    DelayAttribute* delayAttribute = new DelayAttribute;
    RoadAttribute* roadAttribute = new RoadAttribute;
    RuleAttribute* ruleAttribute = new RuleAttribute;
    StateAttribute* stateAttribute = new StateAttribute;

    delayAttribute->delay = 0;
    roadAttribute->angle = 0; // TODO: What to put here?
    roadAttribute->length = 0; // TODO: Same tbh
    stateAttribute->state = STATE_SUCCEEDED;

    modules.push_back(new RoadModule(delayAttribute, ruleAttribute));
    modules.push_back(new InquiryModule(roadAttribute, stateAttribute));
}

std::vector<Module*> parse() {
    // TODO: fill out
}

Parser::~Parser() {
    for(Module* module : modules) {
        delete module;
    }
}
