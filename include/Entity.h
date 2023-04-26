#ifndef S_ENTITY_H
#define S_ENTITY_H

#include <stdint.h>

typedef uint32_t Entity;
extern Entity nextEntityID;
const uint32_t MAX_ENTITIES = 500;
extern uint32_t components[MAX_ENTITIES];

typedef enum ComponentBit {
    COMPONENT_TRANSFORM_BIT = 0x00000001,
    COMPONENT_MESH_BIT = 0x00000002,
    COMPONENT_SCRIPT_BIT = 0x00000004
} ComponentBit;
typedef uint32_t ComponentFlag;

void addComponentBit(Entity, ComponentFlag);
void subtractComponentBit(Entity, ComponentFlag);


Entity createEntity();
// void addComponent( EntityID Entity, ComponentFlag componentFlag ); 
// void removeComponent( EntityID Entity, ComponentFlag componentFlag ); 
// void replaceComponent( EntityID Entity, ComponentFlag componentFlag, void* componentStruct ); 

#endif //S_ENTITY_H