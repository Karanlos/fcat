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

#pragma once

#include <vector>
#include <unordered_map>
#include "vulkan/vulkan.h"
#include "vk_layer_logging.h"
#include "layer_factory.h"

class FcatLayer : public layer_factory {
public:
    // Constructor for state_tracker
    FcatLayer() : layer_factory(this), images_count_(0), current_frame_(0){};

    VkResult PostCallCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

    void PreCallDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator);

    VkResult PostCallGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);

    void PostCallGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);

    VkResult PreCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo);

private:
    uint32_t                                current_frame_;
    uint32_t                                images_count_;

    std::vector<VkCommandBuffer>            command_buffers_;
    VkCommandPool                           command_pool_;
    VkDescriptorPool                        descriptor_pool_;
    VkDescriptorSetLayout                   descriptor_layout_;
    std::vector<VkDescriptorSet>            descriptor_sets_;
    VkDevice                                device_;
    std::vector<VkImage>                    swapchain_images_;
    std::vector<VkImageView>                swapchain_image_views_;
    std::vector<VkFence>                    fences_;
    VkPipelineLayout                        pipeline_layout_;
    VkPipeline                              pipeline_;
    std::vector<VkSemaphore>                semaphores_;
    VkShaderModule                          compute_shader_module_;
    std::unordered_map<VkQueue, uint32_t>   queue_index_map_;
};
