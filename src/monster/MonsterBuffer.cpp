
#include <monster/MonsterBuffer.h>


//#include <vulkan/vulkan.h>
//#include "vk_mem_alloc.h"

#define VMA_IMPLEMENTATION
//#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

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

	auto result = vmaCreateBuffer(vkMemAlloc->vmaAllocator, bufferInfo, &allocInfo, &buffer, &allocation, allocationInfo);

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


	vmaCreateImage(vkMemAlloc->vmaAllocator, imageInfo, &allocInfo, &vkImage, &allocation, nullptr);

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

void MonsterBuffer::createDescriptorHeapBuffer
(
	std::shared_ptr<MBuffer> buffers
)
{
	//vk::DeviceSize offset = 0;
	//// craete Heap buffers
	//bufferHeapSize.push_back(alignedVkSize(bufferDescriptorSize + offset + descriptorHeapProperties.minResourceHeapReservedRange, descriptorHeapProperties.resourceHeapAlignment));
	//auto [heapBuffer, heapAlloc] = createBuffer(
	//	bufferHeapSize.back(),
	//	vk::BufferUsageFlagBits::eShaderDeviceAddressEXT | vk::BufferUsageFlagBits::eDescriptorHeapEXT,
	//	VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
	//	VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
	//);

	//std::vector<VkResourceDescriptorInfoEXT> resources;
	//std::vector<VkHostAddressRangeEXT> hostAddrs;

	//VkBufferDeviceAddressInfoKHR heapAddr{
	//		.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR,
	//		.buffer = heapBuffer
	//};

	//bufferHeapAddress.push_back(vkGetBufferDeviceAddressMON(*vkMonsterStats->device, &heapAddr));

	//for (size_t i = 0; i < buffers->buffers.size(); i++)
	//{
	//	// GET BUFFER ADDRESS
	//	VkBufferDeviceAddressInfoKHR deviceAddr{
	//		.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR,
	//		.buffer =  buffers->buffers[i]
	//	};

	//	VkDeviceAddress bufferAddr = vkGetBufferDeviceAddressMON(*vkMonsterStats->device, &deviceAddr);

	//	VkDeviceAddressRangeEXT addressRange{
	//		.address = bufferAddr,
	//		.size = buffers->bufferSizes[i]
	//	};

	//	// WRITE THE RESOURCE TO DESCRIPTOR HEAP
	//	VkResourceDescriptorDataEXT resourceData{
	//		.pAddressRange = &addressRange
	//	};

	//	VkResourceDescriptorInfoEXT resourceInfo{
	//		.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	//		.data = resourceData
	//	};

	//	resources.push_back(resourceInfo);

	//	VkHostAddressRangeEXT hostAddressRange{
	//		.address = static_cast<uint8_t*>(heapAlloc->GetMappedData()) + bufferDescriptorSize * i,
	//		.size = bufferDescriptorSize
	//	};

	//	hostAddrs.push_back(hostAddressRange);

	//}

	//buffers->heapMapped = heapAlloc->GetMappedData();

	//VkResult result = vkWriteResourceDescriptorsMON(*vkMonsterStats->device, buffers->buffers.size(), resources.data(), hostAddrs.data());

	//if (result != VkResult::VK_SUCCESS)
	//{
	//	throw std::runtime_error("ERROR CREATING DESCRIPTOR HEAP!");
	//}

	// DESCRIPTOR MAPPED DATA IS CREATED NOW TRANSFER THE DATA INTO THE UNIFORM BUFFERS USING GETMAPPERDATA();

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


MBuffer* MonsterBuffer::createVertexBuffer(std::vector<vulkanUtils::Vertex> vertices) {
	MBuffer* mBuffer = new MBuffer();
	vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	// Create a staging buffer (stored in the CPU for quick access and change)
	auto [stagingBuffer, stagingBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VMA_MEMORY_USAGE_AUTO
	);

	// add data to the staging buffer
	vmaCopyMemoryToAllocation(vkMemAlloc->vmaAllocator, vertices.data(), stagingBufferAlloc, 0, bufferSize);

	// create the device_local(graphics crad memory) buffer
	VmaAllocationInfo allocInfo;
	auto [vkBuffer, vkBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		VMA_MEMORY_USAGE_AUTO,
		&allocInfo
	);
	mBuffer->uboBuffer.push_back(std::move(vk::raii::Buffer(vkMonsterStats->device, vkBuffer)));
	mBuffer->bufferAlloc.push_back(std::move(vkBufferAlloc));
	mBuffer->bufferMapped.push_back(std::move(allocInfo));
	copyBuffer(stagingBuffer, mBuffer->uboBuffer.back(), bufferSize);

	return mBuffer;
}

//void MonsterVulkan::createIndexBuffer() {
//	createIndexBuffer(p_indices);
//}

MBuffer* MonsterBuffer::createIndexBuffer(std::vector<uint16_t> indices) {

	MBuffer* mBuffer = new MBuffer();

	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	// Create a staging buffer (stored in the CPU for quick access and change)
	auto [stagingBuffer, stagingBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
		VMA_MEMORY_USAGE_AUTO
	);

	// add data to the staging buffer
	vmaCopyMemoryToAllocation(vkMemAlloc->vmaAllocator, indices.data(), stagingBufferAlloc, 0, bufferSize);

	// create the device_local(graphics crad memory) buffer
	VmaAllocationInfo allocInfo;
	auto [vkBuffer, vkBufferAlloc] = createBuffer(
		bufferSize,
		vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		VMA_MEMORY_USAGE_AUTO,
		&allocInfo
	);
	mBuffer->uboBuffer.push_back(std::move(vk::raii::Buffer(vkMonsterStats->device, vkBuffer)));
	mBuffer->bufferAlloc.push_back(std::move(vkBufferAlloc));
	mBuffer->bufferMapped.push_back(std::move(allocInfo));

	copyBuffer(stagingBuffer, mBuffer->uboBuffer.back(), bufferSize);

	return mBuffer;
}