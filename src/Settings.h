#ifndef ZOMBIE_SETTINGS_H_
#define ZOMBIE_SETTINGS_H_

#include "ComponentTypes.h"

struct Settings {
    const bool big = true;
    const int MAX_ENTITIES = big ? 500000 : 100;
    const int MAX_COMPONENTS = (int)ComponentTypes::COUNT;

    const float ZOMBIE_RADIUS = big ? 16 : 4;
    const int WORLD_SCALE = big ? MAX_ENTITIES / 2000 : 2;
};

#endif
