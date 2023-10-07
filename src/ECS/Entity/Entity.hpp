#ifndef _Entity
#define _Entity

struct Entity
{
    unsigned int id;
    
    Entity(unsigned int id){
        this->id = id;
    }   
};

#endif