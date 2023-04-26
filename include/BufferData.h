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

typedef struct ImageData {
    BufferData bufferData;
    VkImage image;
    VkImageView imageView;
    VkSampler sampler;
} ImageData;

void createBufferData(VkDevice device, uint32_t queueFamily, VkBufferUsageFlags usage, BufferData &bufferStruct);

void writeDeviceMemory(VkDevice device, BufferData &bufferStruct, void* copyData);

void createBufferDataWithData(VkDevice device, uint32_t queueFamily, VkBufferUsageFlags usage, BufferData &bufferStruct, void* copyData);


void createImageData(VkDevice device, uint32_t queueFamily, std::string fileName, ImageData &imageData);