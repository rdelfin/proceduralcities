//
// Created by rdelfin on 12/10/16.
//

#include <generation/street/GlobalGoals.h>
#include <glm/vec2.hpp>
#include <cmath>

#include <StreetMap.h>

#define PI 3.14159265f

// Useful function on [0, 2pi)
float constrainAngle(float x){
    x = x * 360.0f / (2*PI);
    x = fmod(x,360);
    if (x < 0)
        x += 360;
    return x * 2 * PI / 360.0f;
}

float angleDiff(float x, float y) {
    float result = atan2(sin(x - y), cos(x - y));
    return constrainAngle(result);
}

GlobalGoals::GlobalGoals() {

}

void GlobalGoals::operator()(const RoadAttribute* roadAttribute, const RuleAttribute* ruleAttribute,
                std::vector<DelayAttribute*>& pDel, std::vector<RuleAttribute*>& pRuleAttr, std::vector<RoadAttribute*>& pRoadAttr) {
    getAttribs(roadAttribute, ruleAttribute, pDel, pRuleAttr, pRoadAttr);
}

void GlobalGoals::getAttribs(const RoadAttribute* roadAttribute, const RuleAttribute* ruleAttribute,
                std::vector<DelayAttribute*>& pDel, std::vector<RuleAttribute*>& pRuleAttr, std::vector<RoadAttribute*>& pRoadAttr) {
    if(dynamic_cast<const RectangleRuleAttribute*>(ruleAttribute)) {
        glm::vec2 roadEnd = roadAttribute->end();

        const RectangleRuleAttribute* rectRuleAttr = dynamic_cast<const RectangleRuleAttribute*>(ruleAttribute);

        pDel.push_back(new DelayAttribute(1));
        pDel.push_back(new DelayAttribute(1));
        pDel.push_back(new DelayAttribute(1));
        pRuleAttr.push_back((RuleAttribute*)rectRuleAttr->copy());
        pRuleAttr.push_back((RuleAttribute*)rectRuleAttr->copy());
        pRuleAttr.push_back((RuleAttribute*)rectRuleAttr->copy());

        float widthAngleDiff = min(abs(angleDiff(roadAttribute->angle, rectRuleAttr->initialAngle)),
                                   abs(angleDiff(roadAttribute->angle, constrainAngle(rectRuleAttr->initialAngle + 90))));

        bool wideStreet = widthAngleDiff < 45;
        float roadLength = wideStreet ? rectRuleAttr->width : rectRuleAttr->height;
        float parallelLength = wideStreet ? rectRuleAttr->height : rectRuleAttr->width;

        pRoadAttr.push_back(new RoadAttribute(roadLength, constrainAngle(roadAttribute->angle), roadEnd));
        pRoadAttr.push_back(new RoadAttribute(parallelLength, constrainAngle(roadAttribute->angle + PI/2), roadEnd));
        pRoadAttr.push_back(new RoadAttribute(parallelLength, constrainAngle(roadAttribute->angle - PI/2), roadEnd));
    }
}

GlobalGoals::~GlobalGoals() {

}