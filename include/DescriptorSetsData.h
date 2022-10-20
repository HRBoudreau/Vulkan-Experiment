#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <BufferData.h>
#ifndef N_DEBUG
    #include <iostream>
#endif

const static uint32_t numberOfSets = 1;

typedef struct DescriptorSetsData {
    VkDescriptorSetLayout setLayout;
    VkDescriptorPool pool;
    VkDescriptorSet sets[numberOfSets];
} DescriptorSetsData;

void createDescriptorSetLayout(DescriptorSetsData &descriptorSetsData, VkDevice device);

void allocateDescriptorSets(DescriptorSetsData &descriptorSetsData, VkDevice device, uint32_t graphicsFamily);

void writeDescriptorSets(DescriptorSetsData &descriptorSetsData, VkDevice device, uint32_t set, uint32_t binding, BufferData &bufferData );
