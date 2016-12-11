//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/Parser.h>

#include <generation/street/Modules.h>

#include <vector>
#include <iostream>

#define PI 3.14159265f

Parser::Parser(const GlobalGoals& globalGoals, const LocalConstraints& localConstraints)
    : globalGoals(globalGoals), localConstraints(localConstraints) {
    // Initialize using axiom (omega)
    DelayAttribute delayAttribute(0);
    RoadAttribute roadAttribute(5, (rand() % 180 + 1) * PI/180.0f, glm::vec2(0, 0)); // TODO wtf do I put here
    RuleAttribute ruleAttribute(roadAttribute.angle, 15, 30); // TODO: Pass in through constructor

    StateAttribute stateAttribute = STATE_UNASSIGNED;        // Yay implicit casts!

    modules.push_back(new RoadModule(delayAttribute, ruleAttribute));
    modules.push_back(new InquiryModule(roadAttribute, stateAttribute));
}

std::vector<Module*> Parser::substitution() {
    std::vector<Module*> newModules;
    for(int i = 0; i < (long)modules.size(); i++) {

        // p1
        if(dynamic_cast<RoadModule*>(modules[i]) &&
           dynamic_cast<RoadModule*>(modules[i])->delayAttribute.delay < 0) {
            // Ignore and do not substitute (termination)
        }

        // p2
        else if(i < (long)modules.size() - 1 &&
                dynamic_cast<RoadModule*>(modules[i]) && dynamic_cast<InquiryModule*>(modules[i + 1]) &&
                dynamic_cast<InquiryModule*>(modules[i + 1])->stateAttribute.state == STATE_SUCCEEDED) {
            RoadModule* rm = dynamic_cast<RoadModule*>(modules[i]);
            InquiryModule* im = dynamic_cast<InquiryModule*>(modules[i + 1]);
            std::vector<DelayAttribute> delayAttr;
            std::vector<RuleAttribute> ruleAttr;
            std::vector<RoadAttribute> roadAttr;


            globalGoals.getAttribs(im->roadAttribute, rm->ruleAttribute,  delayAttr, ruleAttr, roadAttr);

            // Equivalent to +() and F()
            newModules.push_back( new DrawnRoadModule( im->roadAttribute ) );

            newModules.push_back(new BranchModule(delayAttr[1], ruleAttr[1], roadAttr[1]));
            newModules.push_back(new BranchModule(delayAttr[2], ruleAttr[2], roadAttr[2]));
            newModules.push_back(new RoadModule(delayAttr[2], ruleAttr[2]));
            newModules.push_back(new InquiryModule(roadAttr[0], STATE_UNASSIGNED));
        }

        // p3
        else if(i < (long)modules.size() - 1 &&
                dynamic_cast<RoadModule*>(modules[i]) && dynamic_cast<InquiryModule*>(modules[i + 1]) &&
                dynamic_cast<InquiryModule*>(modules[i + 1])->stateAttribute.state == STATE_FAILED) {
            // Ignore and do not substitute (termination)
        }

        // p4
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->delayAttribute.delay > 0) {
            BranchModule* bm = dynamic_cast<BranchModule*>(modules[i]);
            newModules.push_back( new BranchModule( DelayAttribute(bm->delayAttribute.delay - 1),
                                                    bm->ruleAttribute, bm->roadAttribute ) );
        }

        // p5
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->delayAttribute.delay == 0) {
            BranchModule* bm = dynamic_cast<BranchModule*>(modules[i]);
            newModules.push_back( new StartModule );
            newModules.push_back( new RoadModule( DelayAttribute(bm->delayAttribute.delay) ,
                                                  bm->ruleAttribute ) );
            newModules.push_back( new InquiryModule( bm->roadAttribute,
                                                     STATE_UNASSIGNED ) );
            newModules.push_back( new EndModule );
        }

        // p6
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->delayAttribute.delay < 0) {
            // Ignore and do not substitute (termination)
        }

        // p7
        else if(i > 0 &&
                dynamic_cast<RoadModule*>(modules[i - 1]) && dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<RoadModule*>(modules[i - 1])->delayAttribute.delay < 0) {
            // Ignore and do not substitute (termination)
        }

        // p8
        else if(dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<InquiryModule*>(modules[i])->stateAttribute.state == STATE_UNASSIGNED) {
            InquiryModule* im = dynamic_cast<InquiryModule*>(modules[i]);
            RoadAttribute oldRoadAttr = im->roadAttribute;
            RoadAttribute newRoadAttr(0, 0, glm::vec2(0, 0));
            StateAttribute newStateAttr(STATE_UNASSIGNED);

            localConstraints(oldRoadAttr, newRoadAttr, newStateAttr);

            newModules.push_back(new InquiryModule(newRoadAttr,
                                                   newStateAttr));
        }

        // p9
        else if(dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<InquiryModule*>(modules[i])->stateAttribute.state != STATE_UNASSIGNED) {
            // Ignore and do not substitute (termination)
        }

        // No substitution
        else {
            newModules.push_back(modules[i]->copy());
        }
    }

    for(Module* mod : modules) {
        delete mod;
    }

    modules = newModules;

    return newModules;
}


std::vector<StreetSegment> Parser::parser() {
    std::vector<StreetSegment> streets;

    for(Module* module : modules) {
        if(dynamic_cast<DrawnRoadModule*>(module)) {
            DrawnRoadModule* drawnRoadModule = dynamic_cast<DrawnRoadModule*>(module);
            RoadAttribute attr = drawnRoadModule->roadAttribute;
            glm::vec2 start = attr.start, end = attr.end();

            std::vector<glm::vec3> waypoints =
                    { glm::vec3(start.x, -1.90f, start.y),
                      glm::vec3(end.x, -1.90f, end.y) };
            streets.push_back(StreetSegment(waypoints, nullptr, nullptr));
        }
    }

    return streets;
}

Parser::~Parser() {
    for(Module* module : modules) {
        delete module;
    }
}
