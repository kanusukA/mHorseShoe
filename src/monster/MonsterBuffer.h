#pragma once


#include <monster/VulkanStats.h>
#include <GDHandler/ResourceHandler.h>

static VkDeviceSize alignedVkSize(VkDeviceSize value, VkDeviceSize alignment) {
	return (value + alignment - 1) & ~(alignment - 1);
}


class MonsterBuffer {
	VulkanMemAlloc* vkMemAlloc;
	VulkanStatus* vkMonsterStats;

	
	

public:

	VkPhysicalDeviceDescriptorHeapPropertiesEXT descriptorHeapProperties{};
	vk::DeviceSize bufferDescriptorSize;
	std::vector<vk::DeviceSize> bufferHeapSize = std::vector<vk::DeviceSize>();
	std::vector<vk::DeviceAddress> bufferHeapAddress = std::vector<vk::DeviceAddress>();

	
	void InitMonsterBuffer(VulkanMemAlloc* alloc_p, VulkanStatus* vkMonsterStats_p) {
		vkMemAlloc = alloc_p;
		vkMonsterStats = vkMonsterStats_p;

		// GET PHYSICAL DEVICE PROPS
		VkPhysicalDeviceProperties2 props{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
		descriptorHeapProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_HEAP_PROPERTIES_EXT;
		props.pNext = &descriptorHeapProperties;
		vkGetPhysicalDeviceProperties2(*vkMonsterStats->gpuDevice, &props);

		// Buffer descriptor Size
		bufferDescriptorSize = alignedVkSize(descriptorHeapProperties.bufferDescriptorSize, descriptorHeapProperties.bufferDescriptorAlignment);

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

	void createDescriptorHeapBuffer(
		std::shared_ptr<MBuffer> buffers
	);

	vk::raii::CommandBuffer begineSingleTimeCommands();
	void endSingleTimeCommands(vk::raii::CommandBuffer&& commandBuffer);

};