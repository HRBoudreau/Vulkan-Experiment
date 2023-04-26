#include <DescriptorSetsData.h>

void createDescriptorSetLayout(DescriptorSetsData &descriptorSetsData, VkDevice device) {
    //
    VkDescriptorSetLayoutBinding bindings[1];
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    bindings[0].pImmutableSamplers = nullptr;
    #ifndef N_DEBUG
        std::cout << "desc set" << std::endl;
    #endif
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = bindings;
    VkResult err = vkCreateDescriptorSetLayout( device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetsData.setLayout );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create descriptorSetLayout! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
}

void allocateDescriptorSets(DescriptorSetsData &descriptorSetsData, VkDevice device, uint32_t graphicsFamily) {
    VkDescriptorPoolSize descriptorPoolSize = {};
    descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount = 1;
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; 
    descriptorPoolCreateInfo.pNext = nullptr; 
    descriptorPoolCreateInfo.flags = 0; 
    descriptorPoolCreateInfo.maxSets = 1; 
    descriptorPoolCreateInfo.poolSizeCount = 1; 
    descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize; 

    #ifndef N_DEBUG
        std::cout << "desc pool" << std::endl;
    #endif
    VkResult err = vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &descriptorSetsData.pool);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create descriptorPool! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    

    VkDescriptorSetAllocateInfo descriptorSetAllocateinfo = {};
    descriptorSetAllocateinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateinfo.pNext = nullptr;
    descriptorSetAllocateinfo.descriptorPool = descriptorSetsData.pool;
    descriptorSetAllocateinfo.descriptorSetCount = 1;
    descriptorSetAllocateinfo.pSetLayouts = &descriptorSetsData.setLayout;

    #ifndef N_DEBUG
        std::cout << "alloc desc sets" << std::endl;
    #endif
    err = vkAllocateDescriptorSets(device, &descriptorSetAllocateinfo, descriptorSetsData.sets );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to alloacte descriptorSet! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

}


void writeDescriptorSets(DescriptorSetsData &descriptorSetsData, VkDevice device, uint32_t set, uint32_t binding, BufferData &bufferData ) {
    VkDescriptorBufferInfo descriptorBufferInfo = {};
    descriptorBufferInfo.buffer = bufferData.buffer;
    descriptorBufferInfo.offset = 0;
    descriptorBufferInfo.range = bufferData.size;

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = descriptorSetsData.sets[set];
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
    writeDescriptorSet.pTexelBufferView = nullptr;

    #ifndef N_DEBUG
        std::cout << "update desc sets" << std::endl;
    #endif
    vkUpdateDescriptorSets( device, 1, &writeDescriptorSet, 0, nullptr );
}