#ifndef _Transform
#define _Transform

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "raylib.h"

//debug
#include "../../Log.hpp"


class TransformComponent{
public:
    TransformComponent(){
        Log::info("Initiating transform component");
        m_sparseMap.reserve(10000000);
        m_positions.reserve(10000000);
        m_sizes.reserve(10000000);
    }

    void insertComponent(unsigned const int entityID, Vector2 position, Vector2 size){
        unsigned int denseIndex = m_positions.size();
        m_positions.emplace_back(position);
        m_sizes.emplace_back(size);
        m_sparseMap.emplace(entityID, denseIndex);
    }

    void removeComponent(unsigned const int entityID){
        unsigned int denseIndex = m_sparseMap[entityID];
        unsigned int lastDenseIndex = m_positions.size();

        std::swap(m_positions[denseIndex], m_positions[lastDenseIndex]);
        std::swap(m_sizes[denseIndex], m_sizes[denseIndex]);

        m_positions.pop_back();
        m_sizes.pop_back();

        m_sparseMap.erase(entityID);

        for(auto& [id, index] : m_sparseMap){
            if(index == lastDenseIndex){
                m_sparseMap[id] = denseIndex;
                break;
            }
        }
    }

    const std::size_t size() const{
        return m_positions.size();
    }

    const std::vector<Vector2>& getAllPositions() const{
        return m_positions;
    }

    std::vector<Vector2> getAllPositions(){
        return m_positions;
    }

    unsigned int getDenseIndexForEntity(unsigned int entityID) const{
        return m_sparseMap.at(entityID);
    }

    const Vector2 getPosition(unsigned int entityID) const{
        return m_positions[getDenseIndexForEntity(entityID)];
    }

    void setPosition(unsigned int entityID, Vector2 position){
        if(hasEntity(entityID)){
            m_positions[getDenseIndexForEntity(entityID)] = position;
        }
    }

    const bool hasEntity(unsigned const int entityID)const{
        return m_sparseMap.find(entityID) != m_sparseMap.end();
    }
    
private:
    std::vector<Vector2> m_positions;
    std::vector<Vector2> m_sizes;
    std::unordered_map<unsigned int, unsigned int> m_sparseMap;
};

#endif