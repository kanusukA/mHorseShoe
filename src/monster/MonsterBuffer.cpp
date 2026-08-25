#include <monster/MonsterBuffer.h>

const std::vector<vulkanUtils::Vertex> p_vertices = {
	{{-0.5f, -0.5f, 0.5f}, {1.0f,1.0f,1.0f},{1.0f, 0.0f} },
	{{0.5f, -0.5f,0.5f}, {0.0f,1.0f,0.0f}, {0.0f, 0.0f} },
	{{0.5f, 0.5f,0.5f}, {0.0f,0.0f,1.0f}, {0.0f, 1.0f} },
	{{-0.5f, 0.5f,0.5f}, {1.0f,1.0f,1.0f},{1.0f, 1.0f} }
};

const std::vector<uint16_t> p_indices = {
	0, 1, 2, 2, 3, 0
};

void MonsterBufferManager::createVertexBuffer() {
	createVertexBuffer(p_vertices);
}

uint32_t MonsterBufferManager::createVertexBuffer(std::vector<vulkanUtils::Vertex> vertices) {
	vkMemAlloc.vertices.push_back(vertices.size());
	vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	// Create a staging buffer (stored in the CPU for quick access and change)
	auto [stagingBuffer, stagingBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VMA_MEMORY_USAGE_AUTO
	);

	// add data to the staging buffer
	vmaCopyMemoryToAllocation(vkMemAlloc.vmaAllocator, vertices.data(), stagingBufferAlloc, 0, bufferSize);

	// create the device_local(graphics crad memory) buffer
	auto [vkBuffer, vkBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		VMA_MEMORY_USAGE_AUTO
	);
	vkMemAlloc.vertexBuffer.push_back(std::move(vk::raii::Buffer(vkMonsterStats->device, vkBuffer)));
	vkMemAlloc.vertexBufferAlloc.push_back(std::move(vkBufferAlloc));

	copyBuffer(stagingBuffer, vkBuffer, bufferSize);

	return vkMemAlloc.vertexBuffer.size() - 1;
}

void MonsterBufferManager::createIndexBuffer() {
	createIndexBuffer(p_indices);

}

uint32_t MonsterBufferManager::createIndexBuffer(std::vector<uint16_t> indices) {

	vkMemAlloc.indexes.push_back(indices.size()); // Used during recordCommandBuffer()

	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	// Create a staging buffer (stored in the CPU for quick access and change)
	auto [stagingBuffer, stagingBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VMA_MEMORY_USAGE_AUTO
	);

	// add data to the staging buffer
	vmaCopyMemoryToAllocation(vkMemAlloc.vmaAllocator, indices.data(), stagingBufferAlloc, 0, bufferSize);

	// create the device_local(graphics crad memory) buffer
	auto [vkBuffer, vkBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		VMA_MEMORY_USAGE_AUTO
	);
	vkMemAlloc.indexBuffer.push_back(std::move(vk::raii::Buffer(vkMonsterStats->device, vkBuffer)));
	vkMemAlloc.indexBufferAlloc.push_back(std::move(vkBufferAlloc));

	copyBuffer(stagingBuffer, vkBuffer, bufferSize);

	return vkMemAlloc.indexBuffer.size() - 1;
}

void MonsterBufferManager::createUniformBuffers()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
		VmaAllocationInfo info;

		auto [buffer, alloc] = createBuffer(bufferSize,
			vk::BufferUsageFlagBits::eUniformBuffer,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO,
			&info);
		vkMemAlloc.uniformBuffers.emplace_back(std::move(vk::raii::Buffer(vkMonsterStats->device, buffer)));
		vkMemAlloc.uniformBufferAlloc.emplace_back(std::move(alloc));
		vkMemAlloc.uniformBuffersMapped.emplace_back(std::move(info.pMappedData));

	}
}

uint32_t MonsterBufferManager::createUniformBuffers(vk::DeviceSize bufferSize)
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VmaAllocationInfo info;

		auto [buffer, alloc] = createBuffer(bufferSize,
			vk::BufferUsageFlagBits::eUniformBuffer,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO,
			&info);
		vkMemAlloc.uniformBuffers.emplace_back(std::move(vk::raii::Buffer(vkMonsterStats->device, buffer)));
		vkMemAlloc.uniformBufferAlloc.emplace_back(std::move(alloc));
		vkMemAlloc.uniformBuffersMapped.emplace_back(std::move(info.pMappedData));

	}

	return vkMemAlloc.uniformBuffers.size() - MAX_FRAMES_IN_FLIGHT;
}

void MonsterBufferManager::createMonsterBuffer(vk::DeviceSize bufferSize, MonsterBuffer* buffer)
{
	VmaAllocationInfo info;
	std::tie(buffer->buffer, buffer->alloc) = createBuffer(bufferSize,
		vk::BufferUsageFlagBits::eUniformBuffer,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VMA_MEMORY_USAGE_AUTO,
		&info);
	buffer->bufferSize = bufferSize;
	buffer->bufferMapped = info.pMappedData;

}

void MonsterBufferManager::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize bufferSize)
{

	vk::raii::CommandBuffer commandCopyBuffer = begineSingleTimeCommands();
	commandCopyBuffer.copyBuffer(srcBuffer, dstBuffer, vk::BufferCopy{ .size = bufferSize });
	endSingleTimeCommands(std::move(commandCopyBuffer));

}

vk::raii::CommandBuffer MonsterBufferManager::begineSingleTimeCommands()
{
	vk::CommandBufferAllocateInfo allocInfo{
		.commandPool = vkMonsterStats->commandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1
	};
	vk::raii::CommandBuffer commandBuffer = std::move(vk::raii::CommandBuffers(vkMonsterStats->device, allocInfo).front());

	vk::CommandBufferBeginInfo beginInfo{ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
	commandBuffer.begin(beginInfo);

	return std::move(commandBuffer);
}

void MonsterBufferManager::endSingleTimeCommands(vk::raii::CommandBuffer&& commandBuffer)
{
	commandBuffer.end();

	vk::SubmitInfo submitInfo{
		.commandBufferCount = 1,
		.pCommandBuffers = &*commandBuffer
	};

	vkMonsterStats->graphicsQueue.submit(submitInfo, nullptr);
	vkMonsterStats->graphicsQueue.waitIdle();

}

std::pair<vk::Buffer, VmaAllocation> MonsterBufferManager::createBuffer(
	vk::DeviceSize size,
	vk::BufferUsageFlags usage,
	VmaAllocationCreateFlags allocFlags,
	VmaMemoryUsage allocUsage,
	VmaAllocationInfo* allocationInfo
) {

	vk::BufferCreateInfo bufferInfo{
		.size = size,
		.usage = usage,
		.sharingMode = vk::SharingMode::eExclusive
	};

	VmaAllocationCreateInfo allocInfo{
		.flags = allocFlags,
		.usage = allocUsage

	};

	// ASSIGN Memory to that buffer
	VkBuffer buffer;
	VmaAllocation allocation;
	//VmaAllocationInfo allocationInfo = {};

	auto result = vmaCreateBuffer(vkMemAlloc.vmaAllocator, bufferInfo, &allocInfo, &buffer, &allocation, allocationInfo);

	if (result != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("Unbale to create vertex buffer");
	}

	return { std::move(buffer),std::move(allocation) };
}