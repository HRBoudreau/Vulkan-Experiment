#include <WindowData.h>

//std
#include <string>
#include <stdexcept>
#ifndef N_DEBUG
    #include <iostream>
#endif


void createWindowData( WindowData &windowData, VkInstance instance, VkPhysicalDevice physicalDevice, int screenWidth, int screenHeight ) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWmonitor* monitor = NULL;
    // monitor = glfwGetPrimaryMonitor();
    windowData.glfwWindow = glfwCreateWindow(screenWidth,screenHeight,"myWindow",monitor,NULL);
    if ( windowData.glfwWindow == NULL ) {
        std::string s = "Failed to Create glfwWindow!";
        throw std::runtime_error( s.c_str() );
    }
    VkResult err = glfwCreateWindowSurface(instance, windowData.glfwWindow, nullptr, &windowData.surface); 
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to Create Surface! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    VkSurfaceCapabilitiesKHR capabilities = {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, windowData.surface, &capabilities);
    windowData.currentSurfaceFramebufferWidth = capabilities.currentExtent.width;
    windowData.currentSurfaceFramebufferHeight = capabilities.currentExtent.height;
    #ifndef N_DEBUG
        std::cout << "current framebuf extent W: " << std::dec << windowData.currentSurfaceFramebufferWidth << " H: " << windowData.currentSurfaceFramebufferHeight << std::endl;
    #endif
}

