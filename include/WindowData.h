#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


typedef struct WindowData {
    GLFWwindow* glfwWindow;
    VkSurfaceKHR surface;
    int currentSurfaceFramebufferHeight;
    int currentSurfaceFramebufferWidth;
    int currentScreenCoordsSurfaceHeight;
    int currentScreenCoordsSurfaceWidth;
} WindowData;

void createWindowData( WindowData &windowData, VkInstance instance, VkPhysicalDevice physicalDevice, int screenHeight, int screenWidth );

void updateWindowData( WindowData &windowData );