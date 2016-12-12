//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/Parser.h>

#include <generation/street/Modules.h>

#include <generation/street/StreetSegment.h>
#include <generation/street/Intersection.h>

#include <vector>
#include <iostream>
#include <unordered_map>

#define PI 3.14159265f

Parser::Parser(const GlobalGoals& globalGoals, const LocalConstraints& localConstraints)
    : globalGoals(globalGoals), localConstraints(localConstraints), angle((rand() % 180 + 1) * PI/180.0f) {
    // Initialize using axiom (omega)
    DelayAttribute delayAttribute(0);
    RoadAttribute roadAttribute(5, angle, glm::vec2(0, 0));
    RuleAttribute ruleAttribute(roadAttribute.angle, 15, 30);

    StateAttribute stateAttribute = STATE_UNASSIGNED;        // Yay implicit casts!

    modules.push_back(new RoadModule(delayAttribute, ruleAttribute));
    modules.push_back(new InquiryModule(roadAttribute, stateAttribute));
}

std::vector<Module*> Parser::substitution() {
    bool parsedStreets = false;
    std::vector<StreetSegment*> streets;
    std::vector<Intersection*> intersections;

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

            if(!parsedStreets) {
                parse(streets, intersections);
                parsedStreets = true;
            }

            localConstraints.getAttributes(oldRoadAttr, streets, intersections, newRoadAttr, newStateAttr);

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

    // delete street segments and intersections
    if(parsedStreets) {
        for (StreetSegment* segment : streets)
            delete segment;
        for (Intersection* intersection : intersections)
            delete intersection;

    }

    modules = newModules;

    return newModules;
}


void Parser::parse(std::vector<StreetSegment*>& streets, std::vector<Intersection*>& intersections) {
    for(Module* module : modules) {
        if(dynamic_cast<DrawnRoadModule*>(module)) {
            DrawnRoadModule* drawnRoadModule = dynamic_cast<DrawnRoadModule*>(module);
            RoadAttribute attr = drawnRoadModule->roadAttribute;
            glm::vec2 start = attr.start, end = attr.end();

            std::vector<glm::vec2> waypoints = { start, end };

            streets.push_back(new StreetSegment(waypoints, getIntersection(waypoints[0], intersections), getIntersection(waypoints[1], intersections)));
        }
    }
}

Intersection* Parser::getIntersection(glm::vec2 p, std::vector<Intersection*>& intersection) {
    for(int i = 0; i < intersection.size(); i++) {
        if(glm::length(intersection[i]->getPosition() - p) < 0.01f) {
            return intersection[i];
        }
    }

    intersection.push_back(new Intersection(p));
    return intersection[intersection.size() - 1];
}

Parser::~Parser() {
    for(Module* module : modules) {
        delete module;
    }
}
