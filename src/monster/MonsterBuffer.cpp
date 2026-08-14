
#include <monster/MonsterBuffer.h>

std::pair<VkBuffer, VmaAllocation> MonsterBuffer::createBuffer(
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

	auto result = vmaCreateBuffer(memAlloc->vmaAllocator, bufferInfo, &allocInfo, &buffer, &allocation, allocationInfo);

	if (result != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("Unbale to create vertex buffer");
	}

	return { std::move(buffer),std::move(allocation) };
}

std::pair<vk::raii::Image, VmaAllocation> MonsterBuffer::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, VmaAllocationCreateFlags allocFlags, VmaMemoryUsage allocUsage)
{
	vk::ImageCreateInfo imageInfo{
		.imageType = vk::ImageType::e2D,
		.format = format,
		.extent = {width, height,1},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = vk::SampleCountFlagBits::e1,
		.tiling = tiling,
		.usage = usage,
		.sharingMode = vk::SharingMode::eExclusive
	};

	VmaAllocationCreateInfo allocInfo{
		.flags = allocFlags,
		.usage = allocUsage

	};

	VmaAllocation allocation;

	VkImage vkImage;


	vmaCreateImage(memAlloc->vmaAllocator, imageInfo, &allocInfo, &vkImage, &allocation, nullptr);

	return {
		std::move(vk::raii::Image(vkMonsterStats->device,vkImage)),
		std::move(allocation)
	};

}


void MonsterBuffer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize bufferSize)
{

	vk::raii::CommandBuffer commandCopyBuffer = begineSingleTimeCommands();
	commandCopyBuffer.copyBuffer(srcBuffer, dstBuffer, vk::BufferCopy{ .size = bufferSize });
	endSingleTimeCommands(std::move(commandCopyBuffer));

}

void MonsterBuffer::copyBufferToImage(vk::raii::CommandBuffer& commandBuffer, VkBuffer buffer, vk::raii::Image& image, uint32_t width, uint32_t height)
{
	vk::BufferImageCopy region{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = {.aspectMask = vk::ImageAspectFlagBits::eColor, .mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1},
		.imageOffset = {0,0,0},
		.imageExtent = {width, height, 1}
	};

	commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

}

vk::raii::CommandBuffer MonsterBuffer::begineSingleTimeCommands()
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

void MonsterBuffer::endSingleTimeCommands(vk::raii::CommandBuffer&& commandBuffer)
{
	commandBuffer.end();

	vk::SubmitInfo submitInfo{
		.commandBufferCount = 1,
		.pCommandBuffers = &*commandBuffer
	};

	vkMonsterStats->graphicsQueue.submit(submitInfo, nullptr);
	vkMonsterStats->graphicsQueue.waitIdle();

}