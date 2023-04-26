#include <BufferData.h>

#include <string>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <glm/stb_image.h>

void createBufferData(VkDevice device, uint32_t queueFamily, VkBufferUsageFlags usage, BufferData &bufferData) {
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = bufferData.size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 1;
    const uint32_t qFamIndice = queueFamily;
    bufferCreateInfo.pQueueFamilyIndices = &(qFamIndice);

    #ifndef N_DEBUG
        std::cout << "createbufferData: crea buff" << std::endl;
    #endif
    VkResult err = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &bufferData.buffer );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create buffer! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    VkMemoryRequirements  memRequirements;
    vkGetBufferMemoryRequirements( device, bufferData.buffer, &memRequirements);
    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = 2;

    #ifndef N_DEBUG
        std::cout << "createbufferData: alloc mem" << std::endl;
    #endif
    err = vkAllocateMemory ( device, &memoryAllocateInfo, nullptr, &bufferData.deviceMemory );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to allocate bufferMemory! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    #ifndef N_DEBUG
        std::cout << "createbufferData: bind buff" << std::endl;
    #endif
    err = vkBindBufferMemory( device, bufferData.buffer, bufferData.deviceMemory, 0 );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to bind bufferMemory! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
}

void writeDeviceMemory(VkDevice device, BufferData &bufferStruct, void* copyData) {
    void* data;
    VkResult err = vkMapMemory(device, bufferStruct.deviceMemory, 0, bufferStruct.size, 0, &data);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to map memory! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
        memcpy(data, copyData, bufferStruct.size);
    vkUnmapMemory(device, bufferStruct.deviceMemory);
}


void createBufferDataWithData(VkDevice device, uint32_t queueFamily, VkBufferUsageFlags usage, BufferData &bufferStruct, void* copyData) {
    createBufferData(device,queueFamily,usage,bufferStruct);
    writeDeviceMemory(device, bufferStruct, copyData);
}


void createImageData(VkDevice device, uint32_t queueFamily, std::string fileName, ImageData &imageData) {
    int width, height, channels;
    stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if ( pixels == nullptr ) {
        std::string s = "Failed to load image!";
        throw std::runtime_error( s.c_str() );
    }
    BufferData stbRaw;
    stbRaw.size = width*height*4;

    createBufferDataWithData(device,queueFamily,VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stbRaw, pixels);
    stbi_image_free(pixels);

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageCreateInfo.extent.width = static_cast<uint32_t>(width);
    imageCreateInfo.extent.height = static_cast<uint32_t>(height);
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 1;
    imageCreateInfo.pQueueFamilyIndices = &queueFamily;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkResult err = vkCreateImage(device, &imageCreateInfo, nullptr, &imageData.image);
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, imageData.image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = 2;
    err = vkAllocateMemory(device, &allocInfo, nullptr, &imageData.bufferData.deviceMemory);
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, imageData.image, imageData.bufferData.deviceMemory, 0 );
    
}