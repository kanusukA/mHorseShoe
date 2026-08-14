#pragma once
#include <monster/VulkanStats.h>
#include <GDHandler/ResourceHandler.h>


class MonsterBuffer {
	VulkanMemAlloc* vkMemAlloc;
	VulkanStatus* vkMonsterStats;
public:

	

	void InitMonsterBuffer(vk::raii::Device* device_p,VulkanMemAlloc* alloc_p, VulkanStatus* vkMonsterStats_p) {
		vkMemAlloc = alloc_p;
		vkMonsterStats = vkMonsterStats_p;
	}

	std::pair<VkBuffer, VmaAllocation> createBuffer(
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		VmaAllocationCreateFlags allocFlags,
		VmaMemoryUsage allocUsage,
		VmaAllocationInfo* allocationInfo = nullptr
	);
	std::pair<vk::raii::Image, VmaAllocation> createImage(
		uint32_t width,
		uint32_t height,
		vk::Format format,
		vk::ImageTiling tiling,
		vk::ImageUsageFlags usage,
		VmaAllocationCreateFlags allocFlags,
		VmaMemoryUsage allocUsage
	);

	void copyBuffer(
		vk::Buffer srcBuffer,
		vk::Buffer dstBuffer,
		vk::DeviceSize bufferSize
	);

	void copyBufferToImage(
		vk::raii::CommandBuffer& commandBuffer,
		VkBuffer buffer,
		vk::raii::Image& image,
		uint32_t width,
		uint32_t height
	);


	vk::raii::CommandBuffer begineSingleTimeCommands();
	void endSingleTimeCommands(vk::raii::CommandBuffer&& commandBuffer);

};