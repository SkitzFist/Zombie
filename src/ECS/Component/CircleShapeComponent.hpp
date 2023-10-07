
#ifndef _CircleShapeComponent
#define _CircleShapeComponent

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "../../../include/raylib.h"
#include "../Entity/Entity.hpp"

struct CircleShapeComponent
{
    std::vector<Color> colors;
};

class CircleShapeComponents{
public:
    void insertComponent(unsigned const int entityID, Color color){
        unsigned int denseIndex = m_denseArray.colors.size();
        m_denseArray.colors.emplace_back(color);
        m_sparseMap.emplace(entityID, denseIndex);
    }

void removeComponent(unsigned const int entityID){


    unsigned int denseIndex = m_sparseMap[entityID];
    unsigned int lastDenseIndex = m_denseArray.colors.size() - 1;

    // Swap the element to remove with the last one
    std::swap(m_denseArray.colors[denseIndex], m_denseArray.colors[lastDenseIndex]);

    m_denseArray.colors.pop_back();

    // Erase the entityID from the sparse map right away
    m_sparseMap.erase(entityID);
    
    // Update the sparse map for the swapped entity
    for(auto& [id, index] : m_sparseMap){
        if(index == lastDenseIndex){
            m_sparseMap[id] = denseIndex;
            break;
        }
    }
}

    const bool hasEntity(unsigned const int entityID){
        return m_sparseMap.find(entityID) != m_sparseMap.end();
    }

    const std::size_t size() const{
        return m_denseArray.colors.size();
    }

    /*
    const CircleShapeComponents& getAllComponents() const;
    CircleShapeComponents& getAllcomponents();
    unsigned int getDenseIndexForEntity(unsigned int id) const;
    */

private:
    std::unordered_map<unsigned int, unsigned int> m_sparseMap;
    CircleShapeComponent m_denseArray;
};


#endif