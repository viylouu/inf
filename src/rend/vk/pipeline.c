// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

static void _vk_createPipeline(inf_pipeline* pln) {
    vk_pipelineData* pd = pln->data;
    vk_shaderData* vdata = pln->desc.vertex->data;
    vk_shaderData* fdata = pln->desc.fragment->data;

    VkPipelineShaderStageCreateInfo vertstageinfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = NULL,
        
            .flags = 0,

            .stage = VK_SHADER_STAGE_VERTEX_BIT,

            .module = vdata->module,
            .pName = "main",

            .pSpecializationInfo = NULL,
        };
    VkPipelineShaderStageCreateInfo fragstageinfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = NULL,
        
            .flags = 0,

            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,

            .module = fdata->module,
            .pName = "main",

            .pSpecializationInfo = NULL,
        };
    VkPipelineShaderStageCreateInfo stages[] = { 
        vertstageinfo, 
        fragstageinfo,
        };

    VkDynamicState dstates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };
    VkPipelineDynamicStateCreateInfo dstate = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .dynamicStateCount = sizeof(dstates)/sizeof(dstates[0]),
            .pDynamicStates = dstates,
        };

    VkPipelineVertexInputStateCreateInfo vertexinputinfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = NULL,
            
            .flags = 0,

            .vertexBindingDescriptionCount = 0,
            .pVertexBindingDescriptions = NULL,

            .vertexAttributeDescriptionCount = 0,
            .pVertexAttributeDescriptions = NULL,
        };

    VkPipelineInputAssemblyStateCreateInfo inputassembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = false,
        };

    VkPipelineViewportStateCreateInfo viewportstate = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .viewportCount = 1,
            .scissorCount = 1,

            // aparrently this works with dynamic state?
            .pViewports = NULL,
            .pScissors = NULL,
        };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .depthClampEnable = false,

            .rasterizerDiscardEnable = false,

            .polygonMode = VK_POLYGON_MODE_FILL,

            .lineWidth = 1,

            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,

            .depthBiasEnable = false,
            .depthBiasConstantFactor = 0,
            .depthBiasClamp = 0,
            .depthBiasSlopeFactor = 0,
        };

    VkPipelineMultisampleStateCreateInfo multisampling = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .sampleShadingEnable = false,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1,
            .pSampleMask = NULL,
            .alphaToCoverageEnable = false,
            .alphaToOneEnable = false,
        };

    VkPipelineColorBlendAttachmentState colorblendattach = {
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,

            .blendEnable = false,

            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .colorBlendOp = VK_BLEND_OP_ADD,

            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
        };
    VkPipelineColorBlendStateCreateInfo colorblend = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_COPY,

            .attachmentCount = 1,
            .pAttachments = &colorblendattach,

            .blendConstants[0] = 0,
            .blendConstants[1] = 0,
            .blendConstants[2] = 0,
            .blendConstants[3] = 0,
        };

    VkPipelineLayoutCreateInfo layoutinfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .setLayoutCount = 0,
            .pSetLayouts = NULL,

            .pushConstantRangeCount = 0,
            .pPushConstantRanges = NULL,
        };

    if (vkCreatePipelineLayout(s.device, &layoutinfo, NULL, &pd->layout) != VK_SUCCESS) {
        inf_err_msg("failed to create shader pipeline layout!");
        exit(1);
    }
}

static void _vk_deletePipeline(inf_pipeline* pln) {
    vk_pipelineData* pd = pln->data;

    vkDestroyPipelineLayout(s.device, pd->layout, NULL);

    free(pln->data);
}
