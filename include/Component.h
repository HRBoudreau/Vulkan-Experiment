#ifndef S_COMPONENT_H
#define S_COMPONENT_H

#include <unordered_map>
#include <iostream>
#include <typeinfo>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Entity.h>
#include <glm/vec4.hpp>

#include <BufferData.h>

typedef struct Transform {
    glm::vec4 position;
    //glm::vec4 rotation;
} Transform;

typedef struct Mesh {
    BufferData vertices;
    BufferData indices;
    uint32_t indicesCount;
} Mesh;

extern std::unordered_map<Entity, Transform> transformList;
extern std::unordered_map<Entity, Mesh> MeshList;

template<typename T>
void addComponent(Entity id ) {
    if ( strcmp(typeid(T).name(),typeid(Transform).name())==0 ) {
        Transform t = { {0.0f,0.0f,0.0f,1.0f} };
        transformList.insert( std::make_pair(id, t ) );
        addComponentBit(id, COMPONENT_TRANSFORM_BIT);
        std::cout << "Added Transform to " << id << std::endl;
    }
    else if ( strcmp(typeid(T).name(),typeid(Mesh).name())==0 ) {
        Mesh M = {};
        MeshList.insert( std::make_pair(id, M ) );
        addComponentBit(id, COMPONENT_MESH_BIT);
        std::cout << "Added Mesh to " << id << std::endl;
    }
}

template<typename T>
void removeComponent(Entity id ) {
    if ( strcmp(typeid(T).name(),typeid(Transform).name())==0 ) {
        transformList.erase( id );
        subtractComponentBit(id, COMPONENT_TRANSFORM_BIT);
        std::cout << "Removed Transform to " << id << std::endl;
    }
}

template<typename T>
T* getComponent(Entity id ) {
    std::string name = typeid(T).name();
    if ( strcmp( name.c_str(),typeid(Transform).name() )==0 ) {
        auto itr = transformList.find( id );
        if ( itr == transformList.end() ) {
            std::cout << "Failed Get Transform for " << id << std::endl;
            return nullptr;
        }
        std::cout << "Get Transform for " << id << std::endl;
        return (T*)&((*itr).second);
    }
    else if ( strcmp( name.c_str(),typeid(Mesh).name() )==0 ) {
        auto itr = MeshList.find( id );
        if ( itr == MeshList.end() ) {
            std::cout << "Failed Get Mesh for " << id << std::endl;
            return nullptr;
        }
        std::cout << "Get Mesh for " << id << std::endl;
        return (T*)&((*itr).second);
    }
    return nullptr;
}

// template<typename C>
// void replaceComponent();

// typedef struct Mesh {
//     VkBuffer buffer;
//     uint32_t sizeInBytes;
// } Mesh;


// extern Mesh meshList[MAX_ENTITIES];

#endif //S_COMPONENT_H