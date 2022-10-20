#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <DescriptorSetsData.h>
#include <WindowData.h>
#ifndef N_DEBUG
    #include <iostream>
#endif

typedef struct Vertex {
    float pos[3];
    float color[3];
} Vertex;

typedef struct PipelineData {
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    VkRenderPass renderPass;
    VkShaderModule vertexModule;
    VkShaderModule fragmentModule;
    VkViewport viewport;
    VkRect2D scissor;
    BufferData depthBuffer;
} PipelineData;

void createPipeline( PipelineData &pipelineData, 
                    VkDevice device, 
                    DescriptorSetsData &descriptorSetsData, 
                    WindowData &windowData );

