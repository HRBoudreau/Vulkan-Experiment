#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <InstanceData.h>

#ifndef N_DEBUG
    #include <iostream>
#endif

extern bool leftMouseButtonPresseds;

void InitKeyboard(InstanceData &instanceData);
bool ProcessInputs(InstanceData &instanceData);
