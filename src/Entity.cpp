#include <Entity.h>
#include <Component.h>

Entity nextEntityID = 0;
uint32_t components[MAX_ENTITIES];

Entity createEntity() {
    components[nextEntityID] = 0;
    addComponent<Transform>(nextEntityID);
    return nextEntityID++;
}


void addComponentBit(Entity id, ComponentFlag flag) {
    components[id] = components[id] | flag;
    return;
}

void subtractComponentBit(Entity id , ComponentFlag flag) {
    if ( (components[id] | flag) == components[id] ) {
        components[id] = (components[id] ^ flag);
    }
    return;
}

