#ifndef _CircleComponentTest
#define _CircleComponentTest

#include <vector>
#include "../ECS/Component/CircleShapeComponent.hpp"
#include "../Log.hpp"


inline const bool addTest(){
    std::size_t nrOfEntities = 100;
    CircleShapeComponents components;
    for(std::size_t i = 0; i < nrOfEntities; ++i){
        components.insertComponent(i, WHITE);
    }
    bool result = components.size() == nrOfEntities;
    if(result){
        Log::info("Circle component add test successfull");
    }
    return result;
}

inline const bool removeTest(){
    std::vector<unsigned int> entities;
    std::vector<unsigned> entitiesToRemove;
    CircleShapeComponents component;
    unsigned int nrOfEntities = 1000;
    for(unsigned int i = 0; i < nrOfEntities; ++i){
        entities.emplace_back(i);
        component.insertComponent(i, WHITE);
    }
    entitiesToRemove.emplace_back(15);           //test duplicate
    entitiesToRemove.emplace_back(15);           //test duplicate
    entitiesToRemove.emplace_back(20000);        //test out of entitiesBounds
    entitiesToRemove.emplace_back(nrOfEntities); //test end of array
    entitiesToRemove.emplace_back(0);            //test first in array
    for(unsigned int i = 0; i < entitiesToRemove.size(); ++i){
        component.removeComponent(entitiesToRemove[i]);
    }
    for(unsigned int i = 0; i < entitiesToRemove.size(); ++i){
        if(component.hasEntity(entitiesToRemove[i])){
            Log::failedTest("CircleComponentTest remove test failed");
            return false;
        }
    }
    Log::info("CircleComponentTest remove test successfull");
    return true;
}

inline const bool circleComponentTests(){
    bool result = addTest();
    if(!result){
        Log::failedTest("CircleComponent Add test failed");
        return result;
    }
    result = removeTest();
    if(!result){
        Log::failedTest("CircleComponent remove test failed");
        return result;
    }
    return result;
}

#endif