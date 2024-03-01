#ifndef ZOMBIE_SYSTEMS_H_
#define ZOMBIE_SYSTEMS_H_

/*
    Should sort systems dependant on priority

    datastructure suggestion

      |-------*|---------|
      |        |         |
    prio 1   prio 2    prio 4

    insert prio 3

      |--------|-------|-------|
      |        |       |       |
    prio 1   prio 2  prio 3  prio 4

    A prio should be able to spinup dynamically and fit into the correct order.
    Sort insertions.

    name suggestion: PrioQueue, PrioTree

    main node should of PrioType whilst each prio node should be of a vector<std::unique_ptr<T>>

    coud be a neat solution for updateSystems, but could potentially need something else for render systems.

    This would all be on the heap unfortunately.

    Maybe just add std::vectors<std::unique_ptr<System>> for each priority would be a neater option.

    another alternative is to handle every system in the scene. 
*/
struct Systems{
    
};

#endif //ZOMBIE_SYSTEMS_H_