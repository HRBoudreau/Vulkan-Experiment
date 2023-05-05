#include <InstanceData.h>

//std

#include <stdexcept>
#ifndef N_DEBUG
    #include <iostream>
    #include <iomanip>
#endif

#include <BufferData.h>
#include <WindowData.h>
#include <SwapchainData.h>
#include <DescriptorSetsData.h>
#include <PipelineData.h>

void createInstanceData(InstanceData &instanceData) {
    if ( GLFW_TRUE != glfwInit() ) {
        std::string s = "Failed glfwInit!";
        throw std::runtime_error( s.c_str() );
    }

    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = "None";
    applicationInfo.applicationVersion = 1;
    applicationInfo.pEngineName = "Sugma";
    applicationInfo.engineVersion = 1;
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    // code to get instance Extensions
    uint32_t glfwInstanceExtensionsCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwInstanceExtensionsCount);

    for( int i = 0; i < glfwInstanceExtensionsCount; ++i )
    instanceData.instanceExtensionNames.push_back(glfwExtensions[i]);
    instanceData.instanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    instanceData.instanceExtensionNames.push_back(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME);
    #ifndef N_DEBUG
        std::cout << "Instance Extensions:" << std::endl;
        for( const char* str : instanceData.instanceExtensionNames ) {
            std::cout << "\t" << str << std::endl;
        }
    #endif

    instanceData.instanceLayerNames.push_back("VK_LAYER_KHRONOS_validation");

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
    debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsMessengerCreateInfo.pNext = VK_NULL_HANDLE;
    debugUtilsMessengerCreateInfo.flags = 0;
    debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugUtilsMessengerCreateInfo.pfnUserCallback = myDebugCallback;
    debugUtilsMessengerCreateInfo.pUserData = nullptr;

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = instanceData.instanceExtensionNames.size();
    instanceCreateInfo.ppEnabledExtensionNames = instanceData.instanceExtensionNames.data();
    instanceCreateInfo.enabledLayerCount = instanceData.instanceLayerNames.size();
    instanceCreateInfo.ppEnabledLayerNames = instanceData.instanceLayerNames.data();

    VkResult err = vkCreateInstance(&instanceCreateInfo, nullptr, &instanceData.instance);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to Create Instance! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    PFN_vkCreateDebugUtilsMessengerEXT pfnCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instanceData.instance, "vkCreateDebugUtilsMessengerEXT");
    if ( pfnCreateDebugUtilsMessengerEXT == nullptr ) {
        std::string s = "Failed to get createDebugUtilsMessenger!";
        throw std::runtime_error( s.c_str() );
    }

    err = pfnCreateDebugUtilsMessengerEXT(instanceData.instance, &debugUtilsMessengerCreateInfo, nullptr, &instanceData.debugUtilsMessenger);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to Create DebugUtilsMessenger! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    uint32_t deviceCount;
    err = vkEnumeratePhysicalDevices(instanceData.instance, &deviceCount, nullptr);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to EnumeratePhysicalDevices! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    VkPhysicalDevice enumeratedDevices[deviceCount];
    err = vkEnumeratePhysicalDevices(instanceData.instance, &deviceCount, enumeratedDevices);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to EnumeratePhysicalDevices! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    VkPhysicalDeviceProperties physicalDeviceProperties = {};
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = {};

    bool foundDevice = false;
    //searches physical devices for memoryTypeMap, memoryTypeTransfer, and graphicsFamilyQueue
    for ( int i = 0; i < deviceCount && foundDevice == false ; ++i ) { 
    vkGetPhysicalDeviceMemoryProperties(enumeratedDevices[i], &physicalDeviceMemoryProperties);
        #ifndef N_DEBUG
            for ( int j = 0; j < physicalDeviceMemoryProperties.memoryTypeCount; ++j ) {
                std::cout << j << " flags: 0x" << std::hex << std::setw(8) << std::setfill('0') << physicalDeviceMemoryProperties.memoryTypes[j].propertyFlags
                    << std::dec << "\theap Index: " << physicalDeviceMemoryProperties.memoryTypes[j].heapIndex << std::endl;
            }
            for ( int j = 0; j < physicalDeviceMemoryProperties.memoryHeapCount; ++j ) {
                std::cout << j << " flags: 0x" << std::hex << std::setw(8) << std::setfill('0') << physicalDeviceMemoryProperties.memoryHeaps[j].flags
                    << std::dec << "\theap Size: " << physicalDeviceMemoryProperties.memoryHeaps[j].size/1000000 << "Mb" << std::endl;
            }
        #endif
            bool foundMemoryMapType = false;
            for ( int j = 0; j < physicalDeviceMemoryProperties.memoryTypeCount && foundMemoryMapType == false ; ++j ) {
                if ( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT & physicalDeviceMemoryProperties.memoryTypes[j].propertyFlags ) {
                    instanceData.memoryTypeMap = j;
                    foundMemoryMapType = true;
                }
            }

            bool foundMemoryTransferType = false;
            for ( int j = 0; j < physicalDeviceMemoryProperties.memoryTypeCount && foundMemoryTransferType == false ; ++j ) {
                if ( VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT & physicalDeviceMemoryProperties.memoryTypes[j].propertyFlags ) {
                    instanceData.memoryTypeTransfer = j;
                    foundMemoryTransferType = true;
                }
            }

            uint32_t queueFamilyPropertyCount;
            vkGetPhysicalDeviceQueueFamilyProperties(enumeratedDevices[i], &queueFamilyPropertyCount, nullptr);
            VkQueueFamilyProperties queueFamilyProperties[queueFamilyPropertyCount];
            vkGetPhysicalDeviceQueueFamilyProperties(enumeratedDevices[i], &queueFamilyPropertyCount, queueFamilyProperties);
            bool foundGraphicsFamily = false;
            for ( int j = 0; j < queueFamilyPropertyCount && foundGraphicsFamily == false; ++j ) {
                #ifndef N_DEBUG
                    std::cout << i << "\tqueue count: " << queueFamilyProperties[i].queueCount << "\t" << std::hex << std::setw(8) << std::setfill('0') << queueFamilyProperties[i].queueFlags << std::endl;
                #endif
                if ( queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    instanceData.graphicsFamily = j;
                    foundGraphicsFamily = true;
                }
            }

        std::cout << foundMemoryMapType << " " << foundMemoryTransferType << " " << foundGraphicsFamily << std::endl;
        if ( foundMemoryMapType == true && foundMemoryTransferType == true && foundGraphicsFamily == true) {
            instanceData.physicalDevice = enumeratedDevices[i];
            foundDevice = true;
        }
    }
    if ( foundDevice == false ) {
        std::string s = "Failed to find device!";
        throw std::runtime_error( s.c_str() );
    }

    #ifndef N_DEBUG
        std::cout << "graphicsFamily#: " << instanceData.graphicsFamily << std::endl;
    #endif 
    
    //getting device extensions
    instanceData.deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    instanceData.deviceExtensionNames.push_back(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);

    VkPhysicalDeviceVulkan12Features features = {};
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    features.pNext = nullptr;
    features.timelineSemaphore =  VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = &features;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
        VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
        graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphicsQueueCreateInfo.pNext = nullptr;
        graphicsQueueCreateInfo.queueFamilyIndex = instanceData.graphicsFamily;
        graphicsQueueCreateInfo.queueCount = 1;
        graphicsQueueCreateInfo.flags = 0;
        float queuePriority = 1.0f;
        graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;
    deviceCreateInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = instanceData.deviceExtensionNames.data();

    //create Device with chosen physical device and queues
    err = vkCreateDevice(instanceData.physicalDevice, &deviceCreateInfo, nullptr, &instanceData.device );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to Create device! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    createWindowData( instanceData.windowData, instanceData.instance, instanceData.physicalDevice, 1280, 720 );
    float aspect = (float)instanceData.windowData.currentSurfaceFramebufferWidth / (float)instanceData.windowData.currentSurfaceFramebufferHeight;
    instanceData.fov = 75.0f*(3.14159265f/180.0f);
    instanceData.perspectiveMatrix = glm::perspectiveFov(instanceData.fov, (float)instanceData.windowData.currentSurfaceFramebufferWidth, (float)instanceData.windowData.currentSurfaceFramebufferHeight, 0.1f, 100.0f);
    createSwapchainData( instanceData.swapchainData, instanceData.windowData, instanceData.device, instanceData.physicalDevice, instanceData.graphicsFamily, instanceData.memoryTypeTransfer );
    instanceData.UBO.size = sizeof(glm::mat4x4)*3;
    createBufferDataWithData( instanceData.device, instanceData.graphicsFamily, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, instanceData.UBO, (void*)(&instanceData.perspectiveMatrix) );
    

    VkCommandPoolCreateInfo graphicsCommandPoolCreateInfo = {};
    graphicsCommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    graphicsCommandPoolCreateInfo.pNext = nullptr;
    graphicsCommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    graphicsCommandPoolCreateInfo.queueFamilyIndex = instanceData.graphicsFamily;
    err = vkCreateCommandPool(instanceData.device, &graphicsCommandPoolCreateInfo, nullptr, &instanceData.commandPool );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to graphics CommandPool! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkCommandBufferAllocateInfo graphicsCommandBufferAllocateInfo = {};
    graphicsCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    graphicsCommandBufferAllocateInfo.pNext = nullptr;
    graphicsCommandBufferAllocateInfo.commandPool = instanceData.commandPool;
    graphicsCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY ;
    graphicsCommandBufferAllocateInfo.commandBufferCount = 2;
    err = vkAllocateCommandBuffers( instanceData.device, &graphicsCommandBufferAllocateInfo, instanceData.commandBuffers );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to graphics commandBuffer! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    createDescriptorSetLayout( instanceData.descriptorSetsData, instanceData.device);
    createPipeline( instanceData.pipelineData, instanceData.device, instanceData.descriptorSetsData, instanceData.windowData);
    allocateDescriptorSets( instanceData.descriptorSetsData, instanceData.device, instanceData.graphicsFamily );
    writeDescriptorSets( instanceData.descriptorSetsData, instanceData.device,
                        0, 0, instanceData.UBO );
    createFramebuffers(instanceData.swapchainData, instanceData.device, instanceData.pipelineData.renderPass,
                        instanceData.windowData.currentSurfaceFramebufferWidth, instanceData.windowData.currentSurfaceFramebufferHeight);
    vkGetDeviceQueue(instanceData.device, instanceData.graphicsFamily, 0, &instanceData.queue);

    VkSemaphoreTypeCreateInfo semaphoreTypeCreateInfo = {};
    semaphoreTypeCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    semaphoreTypeCreateInfo.pNext = nullptr;
    semaphoreTypeCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    semaphoreTypeCreateInfo.initialValue = 0;

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = &semaphoreTypeCreateInfo;
    semaphoreCreateInfo.flags = 0;

    err = vkCreateSemaphore(instanceData.device, &semaphoreCreateInfo, nullptr, &instanceData.semaphore );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create timeline semaphore! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;

    err = vkCreateFence( instanceData.device, &fenceCreateInfo, nullptr, &(instanceData.fences[0]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create fence! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    err = vkCreateFence( instanceData.device, &fenceCreateInfo, nullptr, &(instanceData.fences[1]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create fence! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    instanceData.camera.position = {0.0f,0.0f,3.0f,1.0f};
    instanceData.camera.rotation = glm::mat4_cast(glm::rotate(glm::quat(),0.0f,glm::vec3(0.0f,1.0f,0.0f)) );
}

VkBool32 myDebugCallback (
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData) {
        std::cout << pCallbackData->flags;
        std::cout << "; " << pCallbackData->pMessageIdName;
        std::cout << "; " << pCallbackData->messageIdNumber <<std::endl;
        std::cout << "\t" << pCallbackData->pMessage << std::endl;
        // std::cout << "; " << pCallbackData->queueLabelCount;
        // std::cout << "; " << pCallbackData->pQueueLabels;
        // std::cout << "; " << pCallbackData->cmdBufLabelCount;
        // std::cout << "; " << pCallbackData->pCmdBufLabels;
        // std::cout << "; " << pCallbackData->objectCount;
        if (pCallbackData->pObjects->pObjectName != nullptr)
        std::cout << "; /t" << pCallbackData->pObjects->pObjectName;
        std::cout << std::endl;
        return 0;
}