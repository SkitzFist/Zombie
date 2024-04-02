#ifndef ZOMBIE_SETTINGS_H_
#define ZOMBIE_SETTINGS_H_

#include "ComponentTypes.h"

struct Settings {
    const bool big = true;
    const int MAX_ENTITIES = big ? 500000 : 100;
    const int MAX_COMPONENTS = (int)ComponentTypes::COUNT;

    const float ZOMBIE_RADIUS = big ? 32 : 8;
    const int WORLD_SCALE = big ? 250 : 1;
};

#endif
