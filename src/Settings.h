#ifndef ZOMBIE_SETTINGS_H_
#define ZOMBIE_SETTINGS_H_

#include "ComponentTypes.h"

struct Settings {
    const int MAX_ENTITIES = 5000000;
    const int MAX_COMPONENTS = (int)ComponentTypes::COUNT;

    const float ZOMBIE_RADIUS = 32;
    const int WORLD_SCALE = 800;
};

#endif 
