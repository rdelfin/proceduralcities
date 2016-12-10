//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/GlobalGoals.h>

GlobalGoals::GlobalGoals() {

}

void GlobalGoals::getAttribs(const RoadAttribute&, const RuleAttribute&,
                std::vector<DelayAttribute>& pDel, std::vector<RuleAttribute>& pRuleAttr, std::vector<RoadAttribute>& pRoadAttr) {
    // TODO: Fill out global goals calculation (use pDel to delete unusable streets, use pRoadAttr to create grid pattern, etc.)
}

GlobalGoals::~GlobalGoals() {

}