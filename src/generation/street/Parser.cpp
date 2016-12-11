//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/Parser.h>

#include <generation/street/Modules.h>

#include <vector>

#define PI 3.14159265f

Parser::Parser(const GlobalGoals& globalGoals, const LocalConstraints& localConstraints)
    : globalGoals(globalGoals), localConstraints(localConstraints) {
    // Initialize using axiom (omega)
    DelayAttribute* delayAttribute = new DelayAttribute(0);
    RoadAttribute* roadAttribute = new RoadAttribute(1, 0, glm::vec2(0, 0)); // TODO wtf do I put here
    RuleAttribute* ruleAttribute = new RectangleRuleAttribute((rand() % 180 + 1) * PI/180.0f, 5, 10); // TODO: Pass in through constructor
    StateAttribute* stateAttribute = new StateAttribute(STATE_UNASSIGNED);

    modules.push_back(new RoadModule(delayAttribute, ruleAttribute));
    modules.push_back(new InquiryModule(roadAttribute, stateAttribute));
}

std::vector<Module*> Parser::substitution() {
    std::vector<Module*> newModules;
    for(int i = 0; i < (long)modules.size(); i++) {

        // p1
        if(dynamic_cast<RoadModule*>(modules[i]) &&
           dynamic_cast<RoadModule*>(modules[i])->getDelayAttribute()->delay < 0) {
            // Ignore and do not substitute (termination)
        }

        // p2
        else if(i < (long)modules.size() - 1 &&
                dynamic_cast<RoadModule*>(modules[i]) && dynamic_cast<InquiryModule*>(modules[i + 1]) &&
                dynamic_cast<InquiryModule*>(modules[i + 1])->getStateAttribute()->state == STATE_SUCCEEDED) {
            RoadModule* rm = dynamic_cast<RoadModule*>(modules[i]);
            InquiryModule* im = dynamic_cast<InquiryModule*>(modules[i + 1]);
            std::vector<DelayAttribute*> delayAttr;
            std::vector<RuleAttribute*> ruleAttr;
            std::vector<RoadAttribute*> roadAttr;


            globalGoals.getAttribs(im->getRoadAttribute(), rm->getRuleAttribute(),  delayAttr, ruleAttr, roadAttr);

            // Equivalent to +() and F()
            newModules.push_back( new DrawnRoadModule( new RoadAttribute(*im->getRoadAttribute()) ) );

            newModules.push_back(new BranchModule((DelayAttribute*)delayAttr[1],
                                                  (RuleAttribute*)ruleAttr[1],
                                                  (RoadAttribute*)roadAttr[1]));
            newModules.push_back(new BranchModule((DelayAttribute*)delayAttr[2],
                                                  (RuleAttribute*)ruleAttr[2],
                                                  (RoadAttribute*)roadAttr[2]));
            newModules.push_back(new RoadModule((DelayAttribute*)delayAttr[2],
                                                (RuleAttribute*)ruleAttr[2]));
            newModules.push_back(new InquiryModule((RoadAttribute*)roadAttr[0], new StateAttribute(STATE_UNASSIGNED)));
        }

        // p3
        else if(i < (long)modules.size() - 1 &&
                dynamic_cast<RoadModule*>(modules[i]) && dynamic_cast<InquiryModule*>(modules[i + 1]) &&
                dynamic_cast<InquiryModule*>(modules[i + 1])->getStateAttribute()->state == STATE_FAILED) {
            // Ignore and do not substitute (termination)
        }

        // p4
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->getDelayAttribute()->delay > 0) {
            BranchModule* bm = dynamic_cast<BranchModule*>(modules[i]);
            newModules.push_back( new BranchModule( new DelayAttribute(bm->getDelayAttribute()->delay - 1),
                                                    (RuleAttribute*)bm->getRuleAttribute()->copy(),
                                                    (RoadAttribute*)bm->getRoadAttribute()->copy() ) );
        }

        // p5
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->getDelayAttribute()->delay == 0) {
            BranchModule* bm = dynamic_cast<BranchModule*>(modules[i]);
            newModules.push_back( new StartModule );
            newModules.push_back( new RoadModule( new DelayAttribute(bm->getDelayAttribute()->delay),
                                                  (RuleAttribute*)bm->getRuleAttribute()->copy() ) );
            newModules.push_back( new InquiryModule( (RoadAttribute*)bm->getRoadAttribute()->copy(),
                                                     new StateAttribute(STATE_UNASSIGNED) ) );
            newModules.push_back( new EndModule );
        }

        // p6
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->getDelayAttribute()->delay < 0) {
            // Ignore and do not substitute (termination)
        }

        // p7
        else if(i > 0 &&
                dynamic_cast<RoadModule*>(modules[i - 1]) && dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<RoadModule*>(modules[i - 1])->getDelayAttribute()->delay < 0) {
            // Ignore and do not substitute (termination)
        }

        // p8
        else if(dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<InquiryModule*>(modules[i])->getStateAttribute()->state == STATE_UNASSIGNED) {
            InquiryModule* im = dynamic_cast<InquiryModule*>(modules[i]);
            const RoadAttribute* oldRoadAttr = im->getRoadAttribute();
            RoadAttribute newRoadAttr(0, 0, glm::vec2(0, 0));
            StateAttribute newStateAttr(STATE_UNASSIGNED);

            localConstraints(*oldRoadAttr, newRoadAttr, newStateAttr);

            newModules.push_back(new InquiryModule((RoadAttribute*)newRoadAttr.copy(),
                                                   (StateAttribute*)newStateAttr.copy()));
        }

        // p9
        else if(dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<InquiryModule*>(modules[i])->getStateAttribute()->state != STATE_UNASSIGNED) {
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
            const RoadAttribute* attr = drawnRoadModule->getRoadAttribute();
            glm::vec2 start = attr->start, end = attr->start + attr->length*glm::vec2(cos(attr->angle), sin(attr->angle));

            std::vector<glm::vec3> waypoints =
                    { glm::vec3(start.x, 0.0f, start.y),
                      glm::vec3(end.x, 0.0f, end.y) };
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
