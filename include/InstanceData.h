#pragma once

//dependency libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//std
#include <string>
#include <vector>

#include <BufferData.h>
#include <PipelineData.h>
#include <WindowData.h>
#include <SwapchainData.h>
#include <DescriptorSetsData.h>

#include <Component.h>


typedef struct InstanceData {
    //vulkan api handles
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffers[2];
    VkQueue queue;
    VkSemaphore semaphore; // timeline semaphore
    VkFence fences[2];

    //basic types
    uint32_t graphicsFamily;
    uint32_t memoryTypeMap;
    uint32_t memoryTypeTransfer;
    uint64_t semaphoreValue = 0;
    Transform camera;

    //useful data
    std::vector<const char*> instanceExtensionNames;
    std::vector<const char*> instanceLayerNames;
    std::vector<const char*> deviceExtensionNames;
    std::vector<const char*> deviceLayerNames;
    glm::mat4x4 perspectiveMatrix;

    //other struct data types
    WindowData windowData;
    SwapchainData swapchainData;
    DescriptorSetsData descriptorSetsData;
    PipelineData pipelineData;

    BufferData UBO;
} InstanceData;


VkBool32 myDebugCallback (
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData);


//creatInstanceData: inits glfw, creates instance, finds physical device, creates device
void createInstanceData(InstanceData &instanceData);
