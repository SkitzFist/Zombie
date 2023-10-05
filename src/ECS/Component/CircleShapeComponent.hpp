#ifndef _CircleShapeComponentManager
#define _CircleShapeComponenManager

#include <vector>
#include <unordered_map>
#include "../../../include/raylib.h"
#include "../Entity/Entity.hpp"

struct CircleShapeComponents
{
    std::vector<Color> colors;
};


class CircleShapeComponentManager{
public:
    void insertComponent(unsigned const int entityID, const Color color){
        unsigned int denseIndex = m_denseArray.colors.size();

        m_denseArray.colors.emplace_back(denseIndex);
        m_sparseMap.insert(std::pair<unsigned int , unsigned int>(entityID, denseIndex));
    }

    void removeComponent(unsigned const int entityID){
        unsigned int denseIndex = m_sparseMap[entityID];
        unsigned int lastDenseIndex = m_denseArray.colors.size() - 1;

        m_denseArray.colors[denseIndex] = m_denseArray.colors[lastDenseIndex];
        m_denseArray.colors.pop_back();

        //unsigned int swappedEntityID = 
    }

    const CircleShapeComponents& getAllComponents() const;

    CircleShapeComponents& getAllcomponents();

    unsigned int getDenseIndexForEntity(unsigned int id) const;

private:
    std::unordered_map<unsigned int, unsigned int> m_sparseMap;
    CircleShapeComponents m_denseArray;
};


#endif