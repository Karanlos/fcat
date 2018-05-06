/*
 * Copyright (c) 2015-2017 Valve Corporation
 * Copyright (c) 2015-2017 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

#include "comp_shader.hpp"

#include "layer.h"
#include <sstream>

void createCommandPool(VkDevice device, VkCommandPool* pCommandPool)
{
    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = 0;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    vkCreateCommandPool(device, &createInfo, nullptr, pCommandPool);
}

void allocCommandBuffer(VkDevice device, VkCommandPool pool, VkCommandBuffer* pCommandBuffer)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    allocInfo.commandPool = pool;

    vkAllocateCommandBuffers(device, &allocInfo, pCommandBuffer);
}

void deallocCommandBuffer(VkDevice device, VkCommandPool pool, VkCommandBuffer* pCommandBuffer)
{
    vkFreeCommandBuffers(device, pool, 1, pCommandBuffer);
}

void createShaderModule(VkDevice device, VkShaderModule* pShaderModule)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = compShader_len;
    createInfo.pCode = reinterpret_cast<uint32_t*>(compShader);

    vkCreateShaderModule(device, &createInfo, nullptr, pShaderModule);
}

void createDescriptorPool(VkDevice device, VkDescriptorPool* pDescriptorPool)
{
    VkDescriptorPoolSize sizes = {};
    sizes.descriptorCount = 10;
    sizes.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

    VkDescriptorPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets = 10;
    createInfo.poolSizeCount = 1;
    createInfo.pPoolSizes = &sizes;

    vkCreateDescriptorPool(device, &createInfo, nullptr, pDescriptorPool);
}

void createDescriptorSetLayout(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout* pDescriptorSetLayout)
{
    VkDescriptorSetLayoutBinding binding = {};
    binding.binding = 0;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    binding.descriptorCount = 1;
    binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayout = {};
    descriptorSetLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayout.bindingCount = 1;
    descriptorSetLayout.pBindings = &binding;

    vkCreateDescriptorSetLayout(device, &descriptorSetLayout, nullptr, pDescriptorSetLayout);
}

void allocDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet* pDescriptorSet)
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    vkAllocateDescriptorSets(device, &allocInfo, pDescriptorSet);
}

void createComputePipeline(VkDevice device, VkShaderModule shaderModule, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkPipelineLayout* pPipelineLayout, VkPipeline* pPipeline)
{
    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(uint32_t);
    pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

    vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, pPipelineLayout);

    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
    shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo.module = shaderModule;
    shaderStageCreateInfo.pName = "main";
    shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;

    VkComputePipelineCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    createInfo.layout = *pPipelineLayout;
    createInfo.stage = shaderStageCreateInfo;

    vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &createInfo, nullptr, pPipeline);
}

void createSemaphore(VkDevice device, VkSemaphore* pSemaphore)
{
    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    vkCreateSemaphore(device, &info, nullptr, pSemaphore);
}

void createFence(VkDevice device, VkFence* pFence)
{
    VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    vkCreateFence(device, &createInfo, nullptr, pFence);
}

void createImageView(VkDevice device, VkImage image, VkImageView* pImageView)
{
    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.layerCount = 1;
    createInfo.subresourceRange.levelCount = 1;

    vkCreateImageView(device, &createInfo, nullptr, pImageView);
}

void destroyImageView(VkDevice device, VkImageView imageView)
{
    vkDestroyImageView(device, imageView, nullptr);
}

void updateDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet, VkImageView imageView)
{
    VkDescriptorImageInfo info = {};
    info.imageView = imageView;
    info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    write.dstSet = descriptorSet;
    write.dstBinding = 0;
    write.pImageInfo = &info;

    vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
}

VkResult FcatLayer::PostCallCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDevice * pDevice)
{
    device_ = *pDevice;

    return VK_SUCCESS;
}

void FcatLayer::PreCallDestroyDevice(VkDevice device, const VkAllocationCallbacks * pAllocator)
{
    for (auto& fence : fences_) {
        vkDestroyFence(device_, fence, nullptr);
    }
    for (auto& semaphore : semaphores_) {
        vkDestroySemaphore(device_, semaphore, nullptr);
    }
    for (auto& imageView : swapchain_image_views_) {
        vkDestroyImageView(device_, imageView, nullptr);
    }
    //vkFreeDescriptorSets(device_, descriptor_pool_, descriptor_sets_.size(), descriptor_sets_.data());
    vkDestroyDescriptorPool(device_, descriptor_pool_, nullptr);
    vkDestroyDescriptorSetLayout(device_, descriptor_layout_, nullptr);
    vkFreeCommandBuffers(device_, command_pool_, command_buffers_.size(), command_buffers_.data());
    vkDestroyCommandPool(device_, command_pool_, nullptr);
    vkDestroyPipeline(device_, pipeline_, nullptr);
    vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
    vkDestroyShaderModule(device_, compute_shader_module_, nullptr);
}

VkResult FcatLayer::PostCallGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t * pSwapchainImageCount, VkImage * pSwapchainImages)
{
    if (pipeline_ == VK_NULL_HANDLE) {
        createShaderModule(device_, &compute_shader_module_);
        createDescriptorPool(device_, &descriptor_pool_);
        createDescriptorSetLayout(device_, descriptor_pool_, &descriptor_layout_);
        createComputePipeline(device_, compute_shader_module_, descriptor_pool_, descriptor_layout_, &pipeline_layout_, &pipeline_);
    }

    for (int i = 0; i < swapchain_image_views_.size(); i++) {
        destroyImageView(device, swapchain_image_views_[i]);
        swapchain_image_views_[i] = VK_NULL_HANDLE;
    }

    images_count_ = *pSwapchainImageCount;
    if (command_buffers_.size() < images_count_) {
        command_buffers_.resize(images_count_);
        semaphores_.resize(images_count_);
        swapchain_images_.resize(images_count_);
        swapchain_image_views_.resize(images_count_);
        fences_.resize(images_count_);
        descriptor_sets_.resize(images_count_);
    }

    if (pSwapchainImages != nullptr) {
        for (uint32_t i = 0; i < *pSwapchainImageCount; i++) {
            swapchain_images_[i] = pSwapchainImages[i];
            createImageView(device, pSwapchainImages[i], &swapchain_image_views_[i]);

            if (descriptor_sets_[i] == VK_NULL_HANDLE) {
                allocDescriptorSet(device_, descriptor_pool_, descriptor_layout_, &descriptor_sets_[i]);
                updateDescriptorSet(device_, descriptor_sets_[i], swapchain_image_views_[i]);
            }
        }
    }

    return VK_SUCCESS;
}

void FcatLayer::PostCallGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    queue_index_map_[*pQueue] = queueIndex;
}

VkResult FcatLayer::PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo)
{
    if (command_pool_ == VK_NULL_HANDLE) {
        createCommandPool(device_, &command_pool_);
    }

    auto& cmdBuf = command_buffers_[*pPresentInfo->pImageIndices];
    auto& semaphore = semaphores_[*pPresentInfo->pImageIndices];
    auto& descriptorSet = descriptor_sets_[*pPresentInfo->pImageIndices];
    auto& fence = fences_[*pPresentInfo->pImageIndices];
    if (cmdBuf == VK_NULL_HANDLE) {
        allocCommandBuffer(device_, command_pool_, &cmdBuf);
        createSemaphore(device_, &semaphore);
        createFence(device_, &fence);
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkImageSubresourceRange range = {};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseArrayLayer = 0;
    range.layerCount = 1;
    range.baseMipLevel = 0;
    range.levelCount = 1;

    current_frame_ = (current_frame_ + 1) % 16;

    vkWaitForFences(device_, 1, &fence, true, UINT64_MAX);
    vkResetFences(device_, 1, &fence);

    vkBeginCommandBuffer(cmdBuf, &beginInfo);
    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline_);
    vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline_layout_, 0, 1, &descriptorSet, 0, nullptr);
    vkCmdPushConstants(cmdBuf, pipeline_layout_, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(uint32_t), &current_frame_);
    vkCmdDispatch(cmdBuf, 128 / 16, 1440 / 16, 1);
    vkEndCommandBuffer(cmdBuf);

    VkPipelineStageFlags waitDst = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = pPresentInfo->waitSemaphoreCount;
    submitInfo.pWaitSemaphores = pPresentInfo->pWaitSemaphores;
    submitInfo.pWaitDstStageMask = &waitDst;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &semaphores_[*pPresentInfo->pImageIndices];
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    const_cast<VkPresentInfoKHR*>(pPresentInfo)->waitSemaphoreCount = 1;
    const_cast<VkPresentInfoKHR*>(pPresentInfo)->pWaitSemaphores = &semaphores_[*pPresentInfo->pImageIndices];

    vkQueueSubmit(queue, 1, &submitInfo, fence);

    return VK_SUCCESS;
}

FcatLayer fcat_layer;
