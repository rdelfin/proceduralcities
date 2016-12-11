//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/GlobalGoals.h>
#include <glm/vec2.hpp>
#include <cmath>

GlobalGoals::GlobalGoals() {

}

void GlobalGoals::operator()(const RoadAttribute& roadAttribute, const RuleAttribute& ruleAttribute,
                std::vector<DelayAttribute>& pDel, std::vector<RuleAttribute>& pRuleAttr, std::vector<RoadAttribute>& pRoadAttr) {
    getAttribs(roadAttribute, ruleAttribute, pDel, pRuleAttr, pRoadAttr);
}

void GlobalGoals::getAttribs(const RoadAttribute& roadAttribute, const RuleAttribute& ruleAttribute,
                std::vector<DelayAttribute>& pDel, std::vector<RuleAttribute>& pRuleAttr, std::vector<RoadAttribute>& pRoadAttr) {
    glm::vec2 roadEnd = roadAttribute.start + roadAttribute.length * glm::vec2(cos(roadAttribute.angle), sin(roadAttribute.angle));

    pDel.push_back(1); pDel.push_back(1); pDel.push_back(1);
    pRuleAttr.push_back(RuleAttribute()); pRuleAttr.push_back(RuleAttribute()); pRuleAttr.push_back(RuleAttribute());

    pRoadAttr.push_back(RoadAttribute(1, 0, roadEnd));
    pRoadAttr.push_back(RoadAttribute(1, 90, roadEnd));
    pRoadAttr.push_back(RoadAttribute(1, 270, roadEnd));
}

GlobalGoals::~GlobalGoals() {

}