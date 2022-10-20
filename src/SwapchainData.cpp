#include <SwapchainData.h>


void createSwapchainData( SwapchainData &swapchainData, WindowData &windowData, VkDevice device, VkPhysicalDevice physicalDevice, uint32_t graphicsFamily, uint32_t memoryType) {
    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.minImageCount = 2;
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, windowData.surface, &formatCount, nullptr );
    VkSurfaceFormatKHR surfaceFormats[formatCount];
    swapchainCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        VkExtent2D imageExtent = {};
        imageExtent.height = windowData.currentSurfaceFramebufferHeight;
        imageExtent.width = windowData.currentSurfaceFramebufferWidth;
    swapchainCreateInfo.imageExtent = imageExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 1;
        const uint32_t queueFamilyIndices[1] = {graphicsFamily};
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    swapchainCreateInfo.surface = windowData.surface;


    VkResult err = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchainData.swapchain);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to Create swapchain! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    uint32_t swapchainImageCount;
    err = vkGetSwapchainImagesKHR(device, swapchainData.swapchain, &swapchainImageCount, nullptr );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to getSwapchainImages! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    err = vkGetSwapchainImagesKHR(device, swapchainData.swapchain, &swapchainImageCount, swapchainData.images );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to getSwapchainImages! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    #ifndef N_DEBUG
        std::cout << swapchainImageCount << std::endl;
    #endif

    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = swapchainData.images[0];
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    #ifndef N_DEBUG
        std::cout << "create image view" << std::endl;
    #endif
    err = vkCreateImageView( device, &imageViewCreateInfo, nullptr, &(swapchainData.imageViews[0]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create imageView[0]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    imageViewCreateInfo.image = swapchainData.images[1];

    std::cout << "create image view 2" << std::endl;
    err = vkCreateImageView( device, &imageViewCreateInfo, nullptr, &(swapchainData.imageViews[1]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create imageView[1]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = VK_FORMAT_D32_SFLOAT;
    VkExtent3D extent3d;
    extent3d.height = imageExtent.height;
    extent3d.width = imageExtent.width;
    extent3d.depth = 1;
    imageCreateInfo.extent = extent3d;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.pQueueFamilyIndices = &graphicsFamily;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    err = vkCreateImage( device, &imageCreateInfo, nullptr, &swapchainData.depthImages[0] );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create swapchainData.depthImages[0]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    
    err = vkCreateImage( device, &imageCreateInfo, nullptr, &swapchainData.depthImages[1] );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create swapchainData.depthImages[1]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    
    VkMemoryRequirements depthMemRequirements;
    vkGetImageMemoryRequirements(device, swapchainData.depthImages[0], &depthMemRequirements );

    VkMemoryAllocateInfo memAllocateInfo = {};
    memAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocateInfo.pNext = nullptr;
    memAllocateInfo.allocationSize = depthMemRequirements.size;
    memAllocateInfo.memoryTypeIndex = memoryType;
    err = vkAllocateMemory(device, &memAllocateInfo, nullptr, &(swapchainData.depthMemory[0]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to allocate imageMemory[0]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    err = vkBindImageMemory (device, swapchainData.depthImages[0], swapchainData.depthMemory[0], 0 );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to bind imageMemory[0]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    vkGetImageMemoryRequirements(device, swapchainData.depthImages[1], &depthMemRequirements );
    memAllocateInfo.allocationSize = depthMemRequirements.size;
    err = vkAllocateMemory(device, &memAllocateInfo, nullptr, &(swapchainData.depthMemory[1]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to allocate imageMemory[1]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    err = vkBindImageMemory (device, swapchainData.depthImages[1], swapchainData.depthMemory[1], 0 );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to bind imageMemory[1]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkImageViewCreateInfo depthImageViewCreateInfo = {};
    depthImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthImageViewCreateInfo.pNext = nullptr;
    depthImageViewCreateInfo.flags = 0;
    depthImageViewCreateInfo.image = swapchainData.depthImages[0];
    depthImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthImageViewCreateInfo.format = VK_FORMAT_D32_SFLOAT;
    depthImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    depthImageViewCreateInfo.subresourceRange.levelCount = 1;
    depthImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    depthImageViewCreateInfo.subresourceRange.layerCount = 1;

    err = vkCreateImageView( device, &depthImageViewCreateInfo, nullptr, &(swapchainData.depthImageViews[0]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create depthImageView[0]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    depthImageViewCreateInfo.image = swapchainData.depthImages[1];
    err = vkCreateImageView( device, &depthImageViewCreateInfo, nullptr, &(swapchainData.depthImageViews[1]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create depthImageView[1]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
}

void createFramebuffers(SwapchainData &swapchainData, VkDevice device, VkRenderPass renderPass, uint32_t width, uint32_t height ) {
    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.pNext = nullptr;
    framebufferCreateInfo.flags = 0;
    framebufferCreateInfo.renderPass = renderPass;
    framebufferCreateInfo.attachmentCount = 2;
    VkImageView attachments[2] = {swapchainData.imageViews[0],swapchainData.depthImageViews[0]};
    framebufferCreateInfo.pAttachments = attachments;
    framebufferCreateInfo.width = width;
    framebufferCreateInfo.height = height;
    framebufferCreateInfo.layers = 1;

    std::cout << "fb 1" << std::endl;
    VkResult err = vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &(swapchainData.framebuffers[0]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create framebuffer[0]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    VkImageView attachments2[2] = {swapchainData.imageViews[1],swapchainData.depthImageViews[1]};
    framebufferCreateInfo.pAttachments = attachments2;
    std::cout << "fb 2" << std::endl;
    err = vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &(swapchainData.framebuffers[1]) );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create framebuffer[1]! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
}
