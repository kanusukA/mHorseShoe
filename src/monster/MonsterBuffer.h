#include <monster/VulkanStats.h>

class MonsterBufferManager {
private:
	VulkanStatus* vkMonsterStats;

public:
	VulkanMemAlloc vkMemAlloc = VulkanMemAlloc();

	void InitMonsterBufferManager(VulkanStatus* vkMonsterStats_p) {
		vkMonsterStats = vkMonsterStats_p;
	}

	void createVertexBuffer();
	uint32_t createVertexBuffer(std::vector<vulkanUtils::Vertex> vertices);
	void createIndexBuffer();
	uint32_t createIndexBuffer(std::vector<uint16_t> indices);
	void createUniformBuffers();
	uint32_t createUniformBuffers(vk::DeviceSize bufferSize);
	void createMonsterBuffer(vk::DeviceSize bufferSize, MonsterBuffer* buffer);

	void copyBuffer(
		vk::Buffer srcBuffer,
		vk::Buffer dstBuffer,
		vk::DeviceSize bufferSize
	);

	std::pair<vk::Buffer, VmaAllocation> createBuffer(
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		VmaAllocationCreateFlags allocFlags,
		VmaMemoryUsage allocUsage,
		VmaAllocationInfo* allocationInfo = nullptr
	);

	vk::raii::CommandBuffer begineSingleTimeCommands();
	void endSingleTimeCommands(vk::raii::CommandBuffer&& commandBuffer);

};