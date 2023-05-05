
#define N_DEBUG

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

//std
#include <iostream>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <fstream>

//architecture
#include <InstanceData.h>
#include <Keyboard.h>
#include <Component.h>

BufferData indexBufferStruct;
BufferData vertexBufferStruct;
BufferData indexBufferStruct2;
BufferData vertexBufferStruct2;
VkShaderModule vertexModule;
VkShaderModule fragmentModule;
VkFence fence;
VkSemaphore semaphore;

std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, -4.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f, -4.0f}, {0.0f, 0.0f, 1.0f}},
    {{0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 1.0f}}
};

std::vector<uint32_t> indices = {
    0,1,2,0,3,1
};

std::vector<Vertex> vertices2 = {
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    
    {{0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, -0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}}
};

std::vector<uint32_t> indices2 = {
    0,1,2, 2,1,3, 1,4,3, 3,4,5,
    2,6,0, 2,7,6, 0,6,4, 0,4,1,
    2,3,7, 7,3,5, 5,4,7, 7,4,6
};

bool Update ( InstanceData &instanceData );
void Render( InstanceData &instanceData );

ScriptType testScript = [](Entity t){
    Transform* trans = getComponent<Transform>(t);
    if(trans->position.y < 3.0f) {
        trans->position.y += .0001f;
    }
    else {
        trans->position.y = -1.0f;
    }
    //std::cout << trans->position.y << std::endl;
};

void App() {
    
    InstanceData instanceData;
    createInstanceData(instanceData);
    InitKeyboard(instanceData);
    // buffers
    indexBufferStruct2.size = sizeof(indices2[0])*indices2.size();
    createBufferDataWithData(instanceData.device, instanceData.graphicsFamily,VK_BUFFER_USAGE_INDEX_BUFFER_BIT,indexBufferStruct2, (void*)indices2.data());
    vertexBufferStruct2.size = vertices2.size()*sizeof(vertices2[0]);
    createBufferDataWithData(instanceData.device, instanceData.graphicsFamily, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferStruct2, (void*)vertices2.data() );
    
    Entity t = createEntity();
    addComponent<Mesh>(t);
    Mesh* M = getComponent<Mesh>(t);
    M->indices = indexBufferStruct2;
    M->vertices = vertexBufferStruct2;
    M->indicesCount = indices2.size();
    Transform* trans = getComponent<Transform>(t);
    trans->position = {1.0f,0.0f,-2.0f,1.0f};
    addComponent<Script>(t);
    Script* S = getComponent<Script>(t);
    *S = &testScript;

    bool exit = false;
    while ( exit == false ) {
        Render( instanceData );
        exit = Update( instanceData );
    }

    #ifndef N_DEBUG
        std::cout << "end APP" << std::endl;
    #endif
}


bool Update ( InstanceData &instanceData ) {
    bool ret = ProcessInputs(instanceData);
    for ( std::pair<Entity, Script> pair : ScriptList ) {
        ScriptType currentScript = *(pair.second);
        currentScript(pair.first);
    }
    return ret;
}



void Render( InstanceData &instanceData ) {

    VkRect2D renderArea = {};
    renderArea.offset.x = 0;
    renderArea.offset.y = 0;
    renderArea.extent.width = instanceData.windowData.currentSurfaceFramebufferWidth;
    renderArea.extent.height = instanceData.windowData.currentSurfaceFramebufferHeight;
    
    vkResetFences(instanceData.device, 1, &instanceData.fences[instanceData.semaphoreValue%2]);
    uint32_t indic;
    vkAcquireNextImageKHR(instanceData.device, instanceData.swapchainData.swapchain, 0, NULL, instanceData.fences[instanceData.semaphoreValue%2], &indic);
        vkWaitForFences(instanceData.device, 1, &instanceData.fences[instanceData.semaphoreValue%2], VK_TRUE, UINT64_MAX );
    vkResetCommandBuffer( instanceData.commandBuffers[indic], 0 );
    //std::cout << "INDIC: " << indic << std::endl;
    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = instanceData.pipelineData.renderPass;
    renderPassBeginInfo.framebuffer = instanceData.swapchainData.framebuffers[indic];
    renderPassBeginInfo.renderArea = renderArea;
    renderPassBeginInfo.clearValueCount =2;
    VkClearValue clearValues[2];
    clearValues[0].color = { {0.2f,0.0f,0.7f,1.0f} };
    clearValues[1].depthStencil = {1.0f,0};
    renderPassBeginInfo.pClearValues = clearValues;

    uint32_t dynamicOffset = 0;
    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = nullptr;

    glm::mat4x4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(instanceData.camera.position));
    translate = glm::inverse(translate);
    float aspect = (float)instanceData.windowData.currentSurfaceFramebufferWidth / (float)instanceData.windowData.currentSurfaceFramebufferHeight;
    //instanceData.fov += .001f*(3.14159265f/180.0f);
    //instanceData.perspectiveMatrix = glm::perspective(instanceData.fov, aspect, .1f, 100.0f);
    glm::mat4x4 writeUBO[3] = {instanceData.perspectiveMatrix,translate,instanceData.camera.rotation};
    writeDeviceMemory( instanceData.device, instanceData.UBO, (void*)writeUBO);
    
    VkResult err = vkBeginCommandBuffer ( instanceData.commandBuffers[indic], &commandBufferBeginInfo );
    if ( err != VK_SUCCESS ) {
        std::string s = "Failed to begin commandBuffer! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
    
    vkCmdBeginRenderPass( instanceData.commandBuffers[indic], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( instanceData.commandBuffers[indic], VK_PIPELINE_BIND_POINT_GRAPHICS, instanceData.pipelineData.pipeline);
    vkCmdBindDescriptorSets(instanceData.commandBuffers[indic], VK_PIPELINE_BIND_POINT_GRAPHICS, instanceData.pipelineData.pipelineLayout, 0, 1,
        instanceData.descriptorSetsData.sets, 0, &dynamicOffset );
    VkDeviceSize offsets[] = {0};

    for ( int i = 0; i < MAX_ENTITIES; ++i ) {
        if ( components[i] & COMPONENT_MESH_BIT ) {
            Mesh* m = getComponent<Mesh>(i);
            //std::cout << "MESH: " << i << " indexCount: " << m->indicesCount << std::endl;
            vkCmdBindVertexBuffers(instanceData.commandBuffers[indic], 0, 1, &m->vertices.buffer, offsets);
            vkCmdBindIndexBuffer(instanceData.commandBuffers[indic], m->indices.buffer, 0, VK_INDEX_TYPE_UINT32);
            Transform* t = getComponent<Transform>(i);
            //t->rotation = glm::mat4_cast(glm::rotate(glm::quat_cast(t->rotation),.001f,glm::vec3(0,1,0)));

            vkCmdPushConstants(instanceData.commandBuffers[indic], instanceData.pipelineData.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Transform), (void*)t);
            vkCmdDrawIndexed(instanceData.commandBuffers[indic], static_cast<uint32_t>(m->indicesCount), 1, 0, 0, 0);
        }
    }

    vkCmdEndRenderPass( instanceData.commandBuffers[indic] );
    err = vkEndCommandBuffer( instanceData.commandBuffers[indic] );
    if ( err != VK_SUCCESS ) {
        std::string s = "Failed to begin commandBuffer! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }


    uint64_t waitValue = instanceData.semaphoreValue;
    instanceData.semaphoreValue += 1;
    uint64_t signalValue = instanceData.semaphoreValue;
    VkTimelineSemaphoreSubmitInfo timelineInfo;
    timelineInfo.sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
    timelineInfo.pNext = NULL;
    timelineInfo.waitSemaphoreValueCount = 1;
    timelineInfo.pWaitSemaphoreValues = &waitValue;
    timelineInfo.signalSemaphoreValueCount = 1;
    timelineInfo.pSignalSemaphoreValues = &signalValue;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = &timelineInfo;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &instanceData.semaphore;
    VkPipelineStageFlags stageFlags[1];
    stageFlags[0] = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    submitInfo.pWaitDstStageMask = stageFlags;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &instanceData.commandBuffers[indic];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &instanceData.semaphore;

    

    err = vkQueueSubmit(instanceData.queue, 1, &submitInfo, NULL );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to submit to graphics queue! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }

    VkSemaphoreWaitInfo semaphoreWaitInfo = {};
    semaphoreWaitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
    semaphoreWaitInfo.pNext = nullptr;
    semaphoreWaitInfo.flags = 0;
    semaphoreWaitInfo.semaphoreCount = 1;
    semaphoreWaitInfo.pSemaphores = &instanceData.semaphore;
    semaphoreWaitInfo.pValues = &instanceData.semaphoreValue;
    vkWaitSemaphores(instanceData.device, &semaphoreWaitInfo, 500 );
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 0;
    presentInfo.pWaitSemaphores = nullptr;
    presentInfo.pNext = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &instanceData.swapchainData.swapchain;
    presentInfo.pImageIndices = &indic;
    presentInfo.pResults = nullptr;
    err = vkQueuePresentKHR( instanceData.queue, &presentInfo );
    if( VK_SUCCESS != err ) {
        std::string s = "Failed to submit to present! VK Error: " + std::to_string(static_cast<int>(err));
        throw std::runtime_error( s.c_str() );
    }
}
