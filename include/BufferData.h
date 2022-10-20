#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifndef N_DEBUG
    #include <iostream>
#endif

typedef struct BufferData {
    VkBuffer buffer;
    VkDeviceMemory deviceMemory;
    VkDeviceSize size;
} BufferData;

void createBufferData(VkDevice device, uint32_t queueFamily, VkBufferUsageFlags usage, BufferData &bufferStruct);

void writeDeviceMemory(VkDevice device, BufferData &bufferStruct, void* copyData);

void createBufferDataWithData(VkDevice device, uint32_t queueFamily, VkBufferUsageFlags usage, BufferData &bufferStruct, void* copyData);
