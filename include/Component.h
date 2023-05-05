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
#include <glm/gtc/quaternion.hpp>

#include <BufferData.h>

typedef struct Transform {
    glm::vec4 position;
    glm::mat4x4 rotation;
} Transform;

typedef struct Mesh {
    BufferData vertices;
    BufferData indices;
    uint32_t indicesCount;
} Mesh;

typedef void(*ScriptType)(Entity);
typedef ScriptType *Script;

extern std::unordered_map<Entity, Transform> transformList;
extern std::unordered_map<Entity, Mesh> MeshList;
extern std::unordered_map<Entity, Script> ScriptList;

template<typename T>
void addComponent(Entity id ) {
    if ( strcmp(typeid(T).name(),typeid(Transform).name())==0 ) {
        Transform t = { {0.0f,0.0f,0.0f,1.0f}, glm::mat4_cast(glm::rotate(glm::quat(),0.0f,glm::vec3(.0f,.0f,1.0f) ) ) };
        transformList.insert( std::make_pair(id, t ) );
        addComponentBit(id, COMPONENT_TRANSFORM_BIT);
        #ifndef N_DEBUG
        std::cerr << "Added Transform to " << id << std::endl;
        #endif
    }
    else if ( strcmp(typeid(T).name(),typeid(Mesh).name())==0 ) {
        Mesh M = {};
        MeshList.insert( std::make_pair(id, M ) );
        addComponentBit(id, COMPONENT_MESH_BIT);
        #ifndef N_DEBUG
        std::cerr << "Added Mesh to " << id << std::endl;
        #endif
    }
    else if ( strcmp(typeid(T).name(),typeid(Script).name())==0 ) {
        Script M = {};
        ScriptList.insert( std::make_pair(id, M ) );
        addComponentBit(id, COMPONENT_SCRIPT_BIT);
        #ifndef N_DEBUG
        std::cerr << "Added Script to " << id << std::endl;
        #endif
    }
}


template<typename T>
void removeComponent(Entity id ) {
    if ( strcmp(typeid(T).name(),typeid(Transform).name())==0 ) {
        transformList.erase( id );
        subtractComponentBit(id, COMPONENT_TRANSFORM_BIT);
        #ifndef N_DEBUG
        std::cout << "Removed Transform of " << id << std::endl;
        #endif
    }
    if ( strcmp(typeid(T).name(),typeid(Mesh).name())==0 ) {
        MeshList.erase( id );
        subtractComponentBit(id, COMPONENT_MESH_BIT);
        #ifndef N_DEBUG
        std::cout << "Removed Mesh of " << id << std::endl;
        #endif
    }
    if ( strcmp(typeid(T).name(),typeid(Script).name())==0 ) {
        ScriptList.erase( id );
        subtractComponentBit(id, COMPONENT_SCRIPT_BIT);
        #ifndef N_DEBUG
        std::cout << "Removed Script of " << id << std::endl;
        #endif
    }
}

template<typename T>
T* getComponent(Entity id ) {
    std::string name = typeid(T).name();
    if ( strcmp( name.c_str(),typeid(Transform).name() )==0 ) {
        auto itr = transformList.find( id );
        if ( itr == transformList.end() ) {
        #ifndef N_DEBUG
            std::cout << "Failed Get Transform for " << id << std::endl;
        #endif
            return nullptr;
        }
        #ifndef N_DEBUG
        std::cout << "Get Transform for " << id << std::endl;
        #endif
        return (T*)&((*itr).second);
    }
    else if ( strcmp( name.c_str(),typeid(Mesh).name() )==0 ) {
        auto itr = MeshList.find( id );
        if ( itr == MeshList.end() ) {
        #ifndef N_DEBUG
            std::cout << "Failed Get Mesh for " << id << std::endl;
        #endif
            return nullptr;
        }
        #ifndef N_DEBUG
        std::cout << "Get Mesh for " << id << std::endl;
        #endif
        return (T*)&((*itr).second);
    }
    else if ( strcmp( name.c_str(),typeid(Script).name() )==0 ) {
        auto itr = ScriptList.find( id );
        if ( itr == ScriptList.end() ) {
        #ifndef N_DEBUG
            std::cout << "Failed Get Script for " << id << std::endl;
        #endif
            return nullptr;
        }
        #ifndef N_DEBUG
        std::cout << "Get Script for " << id << std::endl;
        #endif
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