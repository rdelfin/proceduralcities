//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/Parser.h>

#include <generation/street/Modules.h>


Parser::Parser(const GlobalGoals& globalGoals, const LocalConstraints& localConstraints)
    : globalGoals(globalGoals), localConstraints(localConstraints) {
    // Initialize using axiom (omega)
    DelayAttribute* delayAttribute = new DelayAttribute(0);
    RoadAttribute* roadAttribute = new RoadAttribute(0, 0); // TODO wtf do I put here
    RuleAttribute* ruleAttribute = new RuleAttribute;
    StateAttribute* stateAttribute = new StateAttribute(STATE_UNASSIGNED);

    modules.push_back(new RoadModule(delayAttribute, ruleAttribute));
    modules.push_back(new InquiryModule(roadAttribute, stateAttribute));
}

std::vector<Module*> Parser::parse() {
    std::vector<Module*> newModules;
    for(int i = 0; i < (long)modules.size(); i++) {

        // p1
        if(dynamic_cast<RoadModule*>(modules[i]) &&
           dynamic_cast<RoadModule*>(modules[i])->getDelayAttribute()->delay < 0) {
            RoadModule* rm = (RoadModule*)modules[i];
            if(rm->getDelayAttribute()->delay < 0) {
                newModules.push_back(new TerminationModule);
            }
        }

        // p2
        else if(i < (long)modules.size() - 1 &&
                dynamic_cast<RoadModule*>(modules[i]) && dynamic_cast<InquiryModule*>(modules[i + 1]) &&
                dynamic_cast<InquiryModule*>(modules[i + 1])->getStateAttribute()->state == STATE_SUCCEEDED) {
            RoadModule* rm = dynamic_cast<RoadModule*>(modules[i]);
            InquiryModule* im = dynamic_cast<InquiryModule*>(modules[i + 1]);
            std::vector<DelayAttribute> delayAttr;
            std::vector<RuleAttribute> ruleAttr;
            std::vector<RoadAttribute> roadAttr;


            globalGoals(*im->getRoadAttribute(), *rm->getRuleAttribute(),  delayAttr, ruleAttr, roadAttr);

            // Equivalent to +() and F()
            newModules.push_back( new DrawnRoadModule( new RoadAttribute(*im->getRoadAttribute()) ) );

            newModules.push_back(new BranchModule(new DelayAttribute(delayAttr[1]),
                                               new RuleAttribute(ruleAttr[1]),
                                               new RoadAttribute(roadAttr[1])));
            newModules.push_back(new BranchModule(new DelayAttribute(delayAttr[2]),
                                               new RuleAttribute(ruleAttr[2]),
                                               new RoadAttribute(roadAttr[2])));
            newModules.push_back(new RoadModule(new DelayAttribute(delayAttr[2]),
                                             new RuleAttribute(ruleAttr[2])));
            newModules.push_back(new InquiryModule(new RoadAttribute(roadAttr[0]), new StateAttribute(STATE_UNASSIGNED)));
        }

        // p3
        else if(i < (long)modules.size() - 1 &&
                dynamic_cast<RoadModule*>(modules[i]) && dynamic_cast<InquiryModule*>(modules[i + 1]) &&
                dynamic_cast<InquiryModule*>(modules[i + 1])->getStateAttribute()->state == STATE_FAILED) {
            newModules.push_back(new TerminationModule);
        }

        // p4
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->getDelayAttribute()->delay > 0) {
            BranchModule* bm = dynamic_cast<BranchModule*>(modules[i]);
            newModules.push_back( new BranchModule( new DelayAttribute(bm->getDelayAttribute()->delay - 1),
                                                    new RuleAttribute(*bm->getRuleAttribute()),
                                                    new RoadAttribute(*bm->getRoadAttribute()) ) );
        }

        // p5
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->getDelayAttribute()->delay == 0) {
            BranchModule* bm = dynamic_cast<BranchModule*>(modules[i]);
            newModules.push_back( new StartModule );
            newModules.push_back( new RoadModule( new DelayAttribute(bm->getDelayAttribute()->delay),
                                                  new RuleAttribute(*bm->getRuleAttribute()) ) );
            newModules.push_back( new InquiryModule( new RoadAttribute(*bm->getRoadAttribute()),
                                                     new StateAttribute(STATE_UNASSIGNED) ) );
            newModules.push_back( new EndModule );
        }

        // p6
        else if(dynamic_cast<BranchModule*>(modules[i]) &&
                dynamic_cast<BranchModule*>(modules[i])->getDelayAttribute()->delay < 0) {
            newModules.push_back(new TerminationModule);
        }

        // p7
        else if(i > 0 &&
                dynamic_cast<RoadModule*>(modules[i - 1]) && dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<RoadModule*>(modules[i - 1])->getDelayAttribute()->delay < 0) {
            newModules.push_back(new TerminationModule);
        }

        // p8
        else if(dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<InquiryModule*>(modules[i])->getStateAttribute()->state == STATE_UNASSIGNED) {
            InquiryModule* im = dynamic_cast<InquiryModule*>(modules[i]);
            const RoadAttribute* oldRoadAttr = im->getRoadAttribute();
            RoadAttribute newRoadAttr(0, 0);
            StateAttribute newStateAttr(STATE_UNASSIGNED);

            localConstraints(*oldRoadAttr, newRoadAttr, newStateAttr);

            newModules.push_back(new InquiryModule(new RoadAttribute(newRoadAttr),
                                                   new StateAttribute(newStateAttr)));
        }

        // p9
        else if(dynamic_cast<InquiryModule*>(modules[i]) &&
                dynamic_cast<InquiryModule*>(modules[i])->getStateAttribute()->state != STATE_UNASSIGNED) {
            newModules.push_back(new TerminationModule);
        }

        // No substitution
        else {
            if(dynamic_cast<InquiryModule*>(modules[i]))
                newModules.push_back(new InquiryModule(*(InquiryModule*)modules[i]));
            if(dynamic_cast<RoadModule*>(modules[i]))
                newModules.push_back(new RoadModule(*(RoadModule*)modules[i]));
            if(dynamic_cast<BranchModule*>(modules[i]))
                newModules.push_back(new BranchModule(*(BranchModule*)modules[i]));
            if(dynamic_cast<TerminationModule*>(modules[i]))
                newModules.push_back(new TerminationModule(*(TerminationModule*)modules[i]));
        }
    }

    for(Module* mod : modules) {
        delete mod;
    }

    modules = newModules;

    return newModules;
}

Parser::~Parser() {
    for(Module* module : modules) {
        delete module;
    }
}
