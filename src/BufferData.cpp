#include <BufferData.h>

#include <string>
#include <iostream>

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