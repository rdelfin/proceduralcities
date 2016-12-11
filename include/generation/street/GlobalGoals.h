//
// Created by rdelfin on 12/10/16.
//

#ifndef PROCEDURALCITIES_GLOBALGOALS_H
#define PROCEDURALCITIES_GLOBALGOALS_H

#include <generation/street/Attributes.h>

#include <StreetMap.h>

#include <vector>

class GlobalGoals {
public:
    GlobalGoals();

    void getAttribs(const RoadAttribute*, const RuleAttribute*,
                    std::vector<DelayAttribute*>& pDel, std::vector<RuleAttribute*>& pRuleAttr, std::vector<RoadAttribute*>& pRoadAttr);

    void operator()(const RoadAttribute*, const RuleAttribute*,
                    std::vector<DelayAttribute*>& pDel, std::vector<RuleAttribute*>& pRuleAttr, std::vector<RoadAttribute*>& pRoadAttr);

    ~GlobalGoals();
private:
    // TODO: Add global goal properties (population map, water map, road type/style, etc.)
};


#endif //PROCEDURALCITIES_GLOBALGOALS_H
