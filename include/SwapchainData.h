#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <WindowData.h>
#ifndef N_DEBUG
    #include <iostream>
#endif

typedef struct SwapchainData {
    VkSwapchainKHR swapchain;
    VkImage images[2];
    VkImageView imageViews[2];
    VkImage depthImages[2];
    VkImageView depthImageViews[2];
    VkFramebuffer framebuffers[2];
    VkDeviceMemory depthMemory[2];
} SwapchainData;

void createSwapchainData( SwapchainData &swapchainData, 
                        WindowData &windowData, 
                        VkDevice device, 
                        VkPhysicalDevice physicalDevice, 
                        uint32_t graphicsFamily,
                        uint32_t memoryType);

void createFramebuffers(SwapchainData &swapchainData, VkDevice device, VkRenderPass renderPass, uint32_t width, uint32_t height );
