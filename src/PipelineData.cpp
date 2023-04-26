#include <PipelineData.h>

//std
#include <fstream>
#include <vector>

#include <Component.h>

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void createPipeline( PipelineData &pipelineData, VkDevice device, DescriptorSetsData &descriptorSetsData, WindowData &windowData ) {

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetsData.setLayout;
        VkPushConstantRange VkPushConstantRange = {};
        VkPushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        VkPushConstantRange.offset = 0;
        VkPushConstantRange.size = sizeof(Transform);
    pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pipelineLayoutCreateInfo.pPushConstantRanges = &VkPushConstantRange;
    
    std::cout << "create pipeline" << std::endl;
    VkResult err = vkCreatePipelineLayout( device, &pipelineLayoutCreateInfo, nullptr, &pipelineData.pipelineLayout );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create pipelineLayout! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkAttachmentDescription colorAttachmentDescription = {};
    colorAttachmentDescription.flags = 0;
    colorAttachmentDescription.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachmentDescription = {};
    depthAttachmentDescription.flags = 0;
    depthAttachmentDescription.format = VK_FORMAT_D32_SFLOAT;
    depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    VkAttachmentDescription attachmentDescriptions[2] = {colorAttachmentDescription,depthAttachmentDescription};

    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentReference = {};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
    subpassDescription.flags = 0;
    subpassDescription.pipelineBindPoint  = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pResolveAttachments = nullptr;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = 2;
    renderPassCreateInfo.pAttachments = attachmentDescriptions;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
        // VkSubpassDependency dependency;
        // dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        // dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        // dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    renderPassCreateInfo.pDependencies = nullptr;

    std::cout << "create render pass" << std::endl;
    err = vkCreateRenderPass( device, &renderPassCreateInfo, nullptr, &pipelineData.renderPass );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create renderPass! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.pNext = nullptr;
    graphicsPipelineCreateInfo.flags = 0;
    graphicsPipelineCreateInfo.stageCount = 2;
        VkPipelineShaderStageCreateInfo shaderStages[2] = {};
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].flags = 0;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
                auto vertShaderCode = readFile("../src/shaders/vert.spv");
                auto fragShaderCode = readFile("../src/shaders/frag.spv");
            VkShaderModuleCreateInfo vertexModuleCreateInfo = {};
            vertexModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            vertexModuleCreateInfo.pNext = nullptr;
            vertexModuleCreateInfo.flags = 0;
            vertexModuleCreateInfo.codeSize = vertShaderCode.size();
            vertexModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderCode.data());
            err = vkCreateShaderModule ( device, &vertexModuleCreateInfo, nullptr, &pipelineData.vertexModule );
            if ( err != VK_SUCCESS ) {
                std::string s = "Failed to create vertex module! VK Error: " + std::to_string(static_cast<int>(err));
                throw std::runtime_error( s.c_str() );
            }
        shaderStages[0].module = pipelineData.vertexModule;
        shaderStages[0].pName = "main";
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].flags = 0;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            VkShaderModuleCreateInfo fragmentModuleCreateInfo = {};
            fragmentModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            fragmentModuleCreateInfo.pNext = nullptr;
            fragmentModuleCreateInfo.flags = 0;
            fragmentModuleCreateInfo.codeSize = fragShaderCode.size();
            fragmentModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderCode.data());
            err = vkCreateShaderModule ( device, &fragmentModuleCreateInfo, nullptr, &pipelineData.fragmentModule );
            if ( err != VK_SUCCESS ) {
                std::string s = "Failed to create fragment module! VK Error: " + std::to_string(static_cast<int>(err));
                throw std::runtime_error( s.c_str() );
            }
        shaderStages[1].module = pipelineData.fragmentModule;
        shaderStages[1].pName = "main";
        shaderStages[1].pSpecializationInfo = nullptr;
    graphicsPipelineCreateInfo.pStages = shaderStages;
        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.pNext = nullptr;
        vertexInputState.flags = 0;
        vertexInputState.vertexBindingDescriptionCount = 1;
            VkVertexInputBindingDescription vertexInputBindingDescription = {};
            vertexInputBindingDescription.binding = 0;
            vertexInputBindingDescription.stride = sizeof(Vertex);
            vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        vertexInputState.pVertexBindingDescriptions = &vertexInputBindingDescription;
        vertexInputState.vertexAttributeDescriptionCount = 2;
            VkVertexInputAttributeDescription  vertexInputAttributeDescription[2];
            vertexInputAttributeDescription[0].location = 0;
            vertexInputAttributeDescription[0].binding = 0;
            vertexInputAttributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexInputAttributeDescription[0].offset = offsetof(Vertex,pos);
            vertexInputAttributeDescription[1].location = 1;
            vertexInputAttributeDescription[1].binding = 0;
            vertexInputAttributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexInputAttributeDescription[1].offset = offsetof(Vertex,color);
        vertexInputState.pVertexAttributeDescriptions = vertexInputAttributeDescription;
    graphicsPipelineCreateInfo.pVertexInputState = &vertexInputState;
        VkPipelineInputAssemblyStateCreateInfo  inputAssemblerState = {};
        inputAssemblerState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblerState.pNext = nullptr;
        inputAssemblerState.flags = 0;
        inputAssemblerState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblerState.primitiveRestartEnable = false;
    graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblerState;
        VkPipelineTessellationStateCreateInfo  tesselationState = {};
        tesselationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        tesselationState.pNext = nullptr;
        tesselationState.flags = 0;
        tesselationState.patchControlPoints = 1;
    graphicsPipelineCreateInfo.pTessellationState = &tesselationState;
            pipelineData.viewport.x = 0;
            pipelineData.viewport.y = 0;
            pipelineData.viewport.width = windowData.currentSurfaceFramebufferWidth;
            pipelineData.viewport.height = windowData.currentSurfaceFramebufferHeight;
            pipelineData.viewport.minDepth = 0.0f;
            pipelineData.viewport.maxDepth = 1.0f;
            pipelineData.scissor.extent.width = windowData.currentSurfaceFramebufferWidth;
            pipelineData.scissor.extent.height = windowData.currentSurfaceFramebufferHeight;
            pipelineData.scissor.offset.x = 0;
            pipelineData.scissor.offset.y = 0;
        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = nullptr;
        viewportState.flags = 0;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &pipelineData.viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &pipelineData.scissor;
    graphicsPipelineCreateInfo.pViewportState = &viewportState;
        VkPipelineRasterizationStateCreateInfo rasterizationState = {};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.pNext = nullptr;
        rasterizationState.flags = 0;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.rasterizerDiscardEnable = VK_FALSE;
        rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationState.depthBiasEnable = VK_FALSE;
        rasterizationState.depthBiasConstantFactor = 0.0f;
        rasterizationState.depthBiasClamp = 0.0f;
        rasterizationState.depthBiasSlopeFactor = 0.0f;
        rasterizationState.lineWidth = 1.0f;
    graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
        VkPipelineMultisampleStateCreateInfo multisampleState =  {};
        multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleState.pNext = nullptr;
        multisampleState.flags = 0;
        multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleState.sampleShadingEnable = VK_FALSE;
        multisampleState.minSampleShading = 1.0f;
        multisampleState.pSampleMask = nullptr;
        multisampleState.alphaToCoverageEnable = VK_FALSE;
        multisampleState.alphaToOneEnable = VK_FALSE;
    graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
        VkPipelineDepthStencilStateCreateInfo  depthStencilState = {};
        depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilState.pNext = nullptr;
        depthStencilState.flags = 0;
        depthStencilState.depthTestEnable = VK_TRUE;
        depthStencilState.depthWriteEnable = VK_TRUE;
        depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilState.depthBoundsTestEnable = VK_FALSE;
        depthStencilState.stencilTestEnable = VK_FALSE;
        depthStencilState.front = {};
        depthStencilState.back = {};
        depthStencilState.minDepthBounds = 0.0f;
        depthStencilState.maxDepthBounds = 1.0f;
    graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilState;
        VkPipelineColorBlendStateCreateInfo colorBlendState = {};
        colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendState.pNext = nullptr;
        colorBlendState.flags = 0;
        colorBlendState.logicOpEnable = VK_FALSE;
        colorBlendState.logicOp = VK_LOGIC_OP_COPY;
        colorBlendState.attachmentCount = 1;
            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendState.pAttachments = &colorBlendAttachment;
        colorBlendState.blendConstants[0] = 0.0f;
        colorBlendState.blendConstants[1] = 0.0f;
        colorBlendState.blendConstants[2] = 0.0f;
        colorBlendState.blendConstants[3] = 0.0f;
    graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
    graphicsPipelineCreateInfo.pDynamicState = nullptr;
    graphicsPipelineCreateInfo.layout = pipelineData.pipelineLayout;
    graphicsPipelineCreateInfo.renderPass = pipelineData.renderPass;
    graphicsPipelineCreateInfo.subpass = 0;
    graphicsPipelineCreateInfo.basePipelineHandle = nullptr;
    graphicsPipelineCreateInfo.basePipelineIndex = 0;
    
    VkPipelineCache pipelineCache;
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheCreateInfo.pNext = nullptr;
    pipelineCacheCreateInfo.flags = 0;
    pipelineCacheCreateInfo.initialDataSize = 0;
    pipelineCacheCreateInfo.pInitialData = nullptr;
    std::cout << "create pipe cache" << std::endl;
    err = vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create Pipeline Cache! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    std::cout << "create graph pipe" << std::endl;
    err = vkCreateGraphicsPipelines(device, pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &pipelineData.pipeline);
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to create graphicsPipeline! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
}