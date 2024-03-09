#ifndef ZOMBIE_ENTITY_H_
#define ZOMBIE_ENTITY_H_

#include <bitset>

#include "Settings.h"
#include "ComponentTypes.h"

struct Entity{
    int id;
    std::bitset<MAX_COMPONENTS> components;

    Entity(int id) : id(id) {}

    void addComponent(ComponentTypes component){
        components.set((int)component);
    }

    bool hasComponent(ComponentTypes component)const{
        return components.test((int)component);
    }

    void removeComponent(ComponentTypes component){
        components.reset((int)component);
    }
};

#endif
