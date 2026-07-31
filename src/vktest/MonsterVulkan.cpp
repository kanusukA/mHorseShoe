#include "Monster.h"


#include <cstddef>
#include <cstdint>
#include <exception>
#include <map>

#include <algorithm>
#include <limits.h>

#define VMA_IMPLEMENTATION
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// NOTES AND SUGGESTIONS FOR CUSTOM CHANGES IN THE FRAMEWORK
// USE VK_TRUE AND VK_FALSE - ENSURES CROSS_PLATFORMNESS. read more

// TESTING

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f,1.0f,1.0f},{1.0f, 0.0f} },
	{{0.5f, -0.5f}, {0.0f,1.0f,0.0f}, {0.0f, 0.0f} },
	{{0.5f, 0.5f}, {0.0f,0.0f,1.0f}, {0.0f, 1.0f} },
	{{-0.5f, 0.5f}, {1.0f,1.0f,1.0f},{1.0f, 1.0f} }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};  

// -----------------



void Monster::renderFrame() {

	auto fenceResult = vkMonsterStats.device.waitForFences(*vkSyncStats.inFlightFences[vkMonsterStats.frameIndex], true, UINT64_MAX);
	if (fenceResult != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to wait for fence!");
	}
	
	auto [result, imageIndex] = vkMonsterStats.swapChain.acquireNextImage(UINT64_MAX, *vkSyncStats.presentCompleteSemaphores[vkMonsterStats.frameIndex], nullptr);

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized)
	{
		framebufferResized = false;
		recreateSwapChain();
		return;
	}
	if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
	{
		throw std::runtime_error("failed to acquire swapchain");
	}

	vkMonsterStats.device.resetFences(*vkSyncStats.inFlightFences[vkMonsterStats.frameIndex]);

	updateUniformBuffer(vkMonsterStats.frameIndex);

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].reset();

	recordCommandBuffer(imageIndex);

	// submitting command buffer
	vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

	const vk::SubmitInfo submitInfo{
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &*vkSyncStats.presentCompleteSemaphores[vkMonsterStats.frameIndex],
		.pWaitDstStageMask = &waitDestinationStageMask,
		.commandBufferCount = 1,
		.pCommandBuffers = &*vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex],
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &*vkSyncStats.renderFinishedSemaphores[vkMonsterStats.frameIndex]
	};

	vkMonsterStats.graphicsQueue.submit(submitInfo, *vkSyncStats.inFlightFences[vkMonsterStats.frameIndex]);

	// SUBPASSES ARE OPTIONAL. GET MORE DETAIL.

	/*vk::SubpassDependency dependency{
		.srcSubpass = vk::SubpassExternal,
		.dstSubpass = 0,
		.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
		.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
		.srcAccessMask = vk::AccessFlagBits::eNone,
		.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite
	};*/

	const vk::PresentInfoKHR presentInfoKHR{
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &*vkSyncStats.renderFinishedSemaphores[vkMonsterStats.frameIndex],
		.swapchainCount = 1,
		.pSwapchains = &*vkMonsterStats.swapChain,
		.pImageIndices = &imageIndex
	};

	auto presentResult = vkMonsterStats.graphicsQueue.presentKHR(presentInfoKHR);
	if ((presentResult == vk::Result::eSuboptimalKHR) || (presentResult == vk::Result::eErrorOutOfDateKHR) || framebufferResized)
	{
		framebufferResized = false;
		recreateSwapChain();
	}

	vkMonsterStats.frameIndex = (vkMonsterStats.frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

}



void Monster::InitVulkan() {

	std::cout << "Initalizing Vulkan" << std::endl;

	createVulkanInstance();
	setupDebugMessenger();
	createVulkanSurface();
	pickVulkanPhysicalDevice();
	createVulkanDevice();
	createVulkanMemAllocator();
	createSwapchain();
	createImageView();
	createDescriptiorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createCommandBuffer();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createSyncObjects();


}

void Monster::createVulkanInstance() {

	// Validation Layers
	std::vector<char const*> requiredLayers;
	if (enableValidationLayers)
	{
		requiredLayers.assign(validationLayers.begin(), validationLayers.end());
	}

	auto layerProperties = vkMonsterStats.Context.enumerateInstanceLayerProperties();
	auto unsupportedLayerIt = std::ranges::find_if(
		requiredLayers,
		[&layerProperties](auto const& requiredLayer) {
			return std::ranges::none_of(layerProperties,
				[requiredLayer](auto const& layerProperty) {return strcmp(layerProperty.layerName, requiredLayer) == 0; });
		}
	);

	if (unsupportedLayerIt != requiredLayers.end())
	{
		throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
	}

	// Retrieve Extensions required by the Window System (SDL3 in this case)
	uint32_t extensionCount = 0;
	const char* const* instance_extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

	if (instance_extensions == NULL)
	{
		throw std::runtime_error("SDL FAILED TO PROVIDE EXTENSION DETAILS TO VULKAN");
	}

	int count_extensions = extensionCount + 1;
	const char** extensions = static_cast<const char**>(SDL_malloc(count_extensions * sizeof(const char*)));
	extensions[0] = vk::EXTDebugUtilsExtensionName;
	SDL_memcpy(&extensions[1], instance_extensions, extensionCount * sizeof(const char*));



	vk::ApplicationInfo appInfo{};
	appInfo.pEngineName = "mHorseShoee";
	appInfo.pApplicationName = "Psycho";
	appInfo.applicationVersion = 10000;
	appInfo.engineVersion = 10001;
	appInfo.apiVersion = VK_API_VERSION_1_3;

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = count_extensions;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
	createInfo.ppEnabledLayerNames = requiredLayers.data();

	vkMonsterStats.vkInstance = vk::raii::Instance(vkMonsterStats.Context, createInfo);

	


}

void Monster::setupDebugMessenger()
{
	if (enableValidationLayers)
	{
		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError

		);

		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
		);

		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{
			.messageSeverity = severityFlags,
			.messageType = messageTypeFlags,
			.pfnUserCallback = &debugCallback
		};

		debugMessenger = vkMonsterStats.vkInstance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfo);

	}
}

void Monster::createVulkanSurface() {

	VkSurfaceKHR surface;

	if(!SDL_Vulkan_CreateSurface(sdlWindow, *vkMonsterStats.vkInstance, nullptr, &surface)){
	
		throw std::runtime_error("UNABLE TO CREATE VULKAN SURFACE FROM SDLWINDOW!");
	}

	vkMonsterStats.surface = vk::raii::SurfaceKHR(vkMonsterStats.vkInstance,surface);

/*	SDL_PropertiesID properties = SDL_GetWindowProperties(sdlWindow);

	void* hwnd = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

	if (!hwnd)
	{
		throw std::runtime_error("FAILED TO GET WINDOW HWND FROM SDL");
	}

	vk::Win32SurfaceCreateInfoKHR createInfo{
		.hinstance = GetModuleHandle(nullptr),
		.hwnd = static_cast<HWND>(hwnd)
	};

	vkMonsterStats.surface = vkMonsterStats.vkInstance.createWin32SurfaceKHR(createInfo);
*/	


}

void Monster::pickVulkanPhysicalDevice() {
	// PICK GPU
	auto physicalDevices = vkMonsterStats.vkInstance.enumeratePhysicalDevices();
	std::multimap<int, vk::raii::PhysicalDevice> suitableDevices;
	for (size_t i = 0; i < physicalDevices.size(); i++)
	{

		uint32_t score = 0;

		auto deviceInfo = physicalDevices.at(i).getProperties();
		auto deviceFeature = physicalDevices.at(i).getFeatures();

		std::cout << "DEVICE NAME: " << deviceInfo.deviceName << std::endl;
		std::cout << "Device Type: ";
		switch (deviceInfo.deviceType) {
		case vk::PhysicalDeviceType::eCpu:
			std::cout << "CPU" << std::endl;
			break;
		case vk::PhysicalDeviceType::eDiscreteGpu:
			std::cout << "Discrete GPU" << std::endl;
			break;
		case vk::PhysicalDeviceType::eIntegratedGpu:
			score += 1000;
			std::cout << "Integrated GPU" << std::endl;
			break;
		case vk::PhysicalDeviceType::eVirtualGpu:
			std::cout << "Virtual GPU" << std::endl;
			break;
		case vk::PhysicalDeviceType::eOther:
			std::cout << "Other GPU" << std::endl;
			break;
		}

		score += deviceInfo.limits.maxImageDimension2D;

		std::cout << "Driver Version: " << deviceInfo.driverVersion << std::endl;

		if (!deviceFeature.geometryShader)
		{
			continue;
		}

		suitableDevices.insert(std::make_pair(score, physicalDevices.at(i)));

	}

	if (suitableDevices.empty())
	{
		throw std::runtime_error("No Suitable Device Found!");
	}

	int maxScore = 0;
	for (auto& device : suitableDevices)
	{
		if (device.first > maxScore)
		{
			vkMonsterStats.gpuDevice = device.second;
			maxScore = device.first;
		}
	}

	// API VERSION CHECK - If Vulkan 1.3 is supported by the GPU
	vkMonsterStats.supportsVulkan1_3 = vkMonsterStats.gpuDevice.getProperties().apiVersion >= vk::ApiVersion13;

	// Check Queue Family
	auto queueFamilyProperties = vkMonsterStats.gpuDevice.getQueueFamilyProperties();


	// Checks if the queue supports Graphics command (most basic of all. More checks will be required when testing is done)
	vkMonsterStats.supportsGraphic =
		std::ranges::any_of(queueFamilyProperties, [](auto const& qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });


	// Extension Check (currently only SwapchainExtension is checked)
	std::vector<const char*> requiredDeviceExtension = { vk::KHRSwapchainExtensionName };

	auto availableDeviceExtensions = vkMonsterStats.gpuDevice.enumerateDeviceExtensionProperties();

	vkMonsterStats.supportsRequiredExtensions =
		std::ranges::all_of(requiredDeviceExtension,
			[&availableDeviceExtensions](auto const& requiredDeviceExtension)
			{
				return std::ranges::any_of(availableDeviceExtensions,
					[requiredDeviceExtension](auto const& availableDeviceExtension)
					{ return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
			});


	// Required Feature Check
	auto features = vkMonsterStats.gpuDevice.template getFeatures2<vk::PhysicalDeviceFeatures2,
		vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

	vkMonsterStats.supportsRequiredFeatures = features.template get<vk::PhysicalDeviceFeatures2>().features.samplerAnisotropy &&
		features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
		features.template get<vk::PhysicalDeviceVulkan13Features>().synchronization2 &&
		features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;
}


void Monster::createVulkanDevice() {
	
	//Specifying vulkan queues
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = vkMonsterStats.gpuDevice.getQueueFamilyProperties();

	// Check if the queue family supports both graphics and presentation (surface)
	uint32_t queueIndex = ~0;
	for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++)
	{
		if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
			vkMonsterStats.gpuDevice.getSurfaceSupportKHR(qfpIndex, vkMonsterStats.surface))
		{
			queueIndex = qfpIndex;
			break;

		}
	}

	if (queueIndex == ~0)
	{
		throw std::runtime_error("Cound not find queue for graphics and surface");
	}

	//auto graphicsQueueFamilyProperty = std::ranges::find_if(queueFamilyProperties, [](auto const& qfp) {return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0); });
	//auto graphicsIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
	
	// Define Queue Priority 0.0f - 1.0f
	float queuePriority = 0.5f;
	
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo{ .queueFamilyIndex = queueIndex, .queueCount = 1, .pQueuePriorities = &queuePriority };
	vkMonsterStats.queueIndex = queueIndex;
	
	// Define device Features to be used - as in InitVulkan
	// using StructrueChain - refer to Docs https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/04_Logical_device_and_queues.html

	vk::StructureChain<
		vk::PhysicalDeviceFeatures2,
		vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
	> featureChain = {
		{.features = {.samplerAnisotropy = true}},
		{.synchronization2 = true, .dynamicRendering = true},
		{.extendedDynamicState = true}
	};

	// required extensions

	std::vector<const char*> requiredDeviceExtension = {
		vk::KHRSwapchainExtensionName
	};

	// Create Device
	vk::DeviceCreateInfo deviceCreateInfo{
		.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &deviceQueueCreateInfo,
		.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
		.ppEnabledExtensionNames = requiredDeviceExtension.data()
	};

	vkMonsterStats.device = vk::raii::Device(vkMonsterStats.gpuDevice, deviceCreateInfo);

	vkMonsterStats.graphicsQueue = vk::raii::Queue(
		vkMonsterStats.device,
		queueIndex,
		0
	);

}

void Monster::createVulkanMemAllocator() {
	VmaAllocatorCreateInfo vmaCreateInfo{
		.physicalDevice = *vkMonsterStats.gpuDevice,
		.device = *vkMonsterStats.device,
		.instance = *vkMonsterStats.vkInstance
	};

	auto result = vmaCreateAllocator(&vmaCreateInfo, &vkMemAlloc.vmaAllocator);

	if (result != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("Unable to create VMA Allocator Object!");
	}

}


void Monster::createSwapchain(){
	auto surfaceCapabilities = vkMonsterStats.gpuDevice.getSurfaceCapabilitiesKHR(*vkMonsterStats.surface);

	// supported surface formats
	std::vector<vk::SurfaceFormatKHR> availableFormats = vkMonsterStats.gpuDevice.getSurfaceFormatsKHR( vkMonsterStats.surface);

	// supported presentModes
	std::vector<vk::PresentModeKHR> availablePresentModes = vkMonsterStats.gpuDevice.getSurfacePresentModesKHR(vkMonsterStats.surface);

	// Things to determine when choosing swapchain settings
	// - Surface format (color depth)
	// - Presentations mode (conditions for swapping images to the screen)
	// - Swap extent (resolution of images in swapchain)
	

	// Surface format
	
	// vulkan recommands for BGRA 8bit 
	// if that is not found the next best is taken
	
	const auto formatIt = std::ranges::find_if(
			availableFormats,
			[](const auto &format) {return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;});
	
	vkMonsterStats.swapChainSurfaceFormat = formatIt != availableFormats.end() ? *formatIt : availableFormats[0];

	// Present mode
	// There are foure possible modes in vulkan
	// - eImmediate - transfered to screen right away, may cause tearing
	// - eFifo - similar to vertical sync, queue is set and is changed when display is refreshed
	// - eFifoRelaxed - if the queue is empty it's shown right away, may cause tearing
	// - eMailBox - also known as 'triple buffer, replaces item in queue if it's full. Is more energy demanding.
	
	// eFifo is always present.
	assert(std::ranges::any_of(availablePresentModes, [](auto presentMode == ){return vk::PresentModeKHR::eFifo; }));
	vk::PresentModeKHR presentMode = std::ranges::any_of(availablePresentModes, [](const vk::PresentModeKHR value) {return vk::PresentModeKHR::eMailbox == value; }) ? 
	vk::PresentModeKHR::eMailbox :
	vk::PresentModeKHR::eFifo;

	// Swap extent

	if (surfaceCapabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
		vkMonsterStats.swapChainExtent = surfaceCapabilities.currentExtent;
	}

	int width, height;
	SDL_GetWindowSizeInPixels(sdlWindow, &width, &height);
	

	vkMonsterStats.swapChainExtent = vk::Extent2D(
		std::clamp<uint32_t>(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
	std::clamp<uint32_t>(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
	);

	// image count - how many images to store in swapchain
	auto minImageCount = (std::max)(3u, surfaceCapabilities.minImageCount);
	if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount)){
		minImageCount = surfaceCapabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR swapChainCreateInfo {
	.surface = *vkMonsterStats.surface,
	.minImageCount = minImageCount,
	.imageFormat = vkMonsterStats.swapChainSurfaceFormat.format,
	.imageColorSpace = vkMonsterStats.swapChainSurfaceFormat.colorSpace,
	.imageExtent = vkMonsterStats.swapChainExtent,
	.imageArrayLayers = 1,
	.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
	.imageSharingMode = vk::SharingMode::eExclusive,
	.preTransform = surfaceCapabilities.currentTransform,
	.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
	.presentMode = presentMode,
	.clipped = true,
	.oldSwapchain = nullptr
	};

	// When the window is resized the swapchain must be created from scratch again
	// It's left null for now
	//swapChainCreateInfo.oldSwapchain = nullptr;
	
	vkMonsterStats.swapChain = vk::raii::SwapchainKHR(vkMonsterStats.device, swapChainCreateInfo);
	vkMonsterStats.swapChainImages = vkMonsterStats.swapChain.getImages();

}

void Monster::createImageView() {

	assert(vkMonsterStats.swapChainImages.empty());

	vkMonsterStats.swapChainImageViews.reserve(vkMonsterStats.swapChainImages.size());
	for (const auto &image : vkMonsterStats.swapChainImages)
	{
		vkMonsterStats.swapChainImageViews.emplace_back(createImageView(image, vkMonsterStats.swapChainSurfaceFormat.format, vk::ImageAspectFlagBits::eColor));
	}
}

vk::raii::ImageView Monster::createImageView(vk::Image const& image, vk::Format format, vk::ImageAspectFlags flags)
{
	vk::ImageViewCreateInfo imageViewCreateInfo{
		.image = image,
		.viewType = vk::ImageViewType::e2D,
		.format = format,
		.subresourceRange = {
			.aspectMask = flags,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1}
	};
	vk::raii::ImageView imageView = vk::raii::ImageView(vkMonsterStats.device, imageViewCreateInfo);
	return std::move(imageView);
}

void Monster::createDescriptiorSetLayout()
{

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings{
		{{
		.binding = 0,
		.descriptorType = vk::DescriptorType::eUniformBuffer,
		.descriptorCount = 1,
		.stageFlags = vk::ShaderStageFlagBits::eVertex
		},{
		.binding = 1,
		.descriptorType = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = 1,
		.stageFlags = vk::ShaderStageFlagBits::eFragment
		}}
	};

	vk::DescriptorSetLayoutCreateInfo layoutInfo{
		.bindingCount = static_cast<uint32_t>(bindings.size()),
		.pBindings = bindings.data()
	};
	vkDescriptors.descriptorSetLayout = vk::raii::DescriptorSetLayout(vkMonsterStats.device, layoutInfo);
}

[[nodiscard]] vk::raii::ShaderModule Monster::createShaderModule(const std::vector<char>& code) const {

	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = code.size() * sizeof(char),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats.device, shaderModuleCreateInfo };
	return shaderModule;

}

vk::raii::CommandBuffer Monster::begineSingleTimeCommands()
{
	vk::CommandBufferAllocateInfo allocInfo{
		.commandPool = vkMonsterStats.commandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1
	};
	vk::raii::CommandBuffer commandBuffer = std::move(vk::raii::CommandBuffers(vkMonsterStats.device, allocInfo).front());

	vk::CommandBufferBeginInfo beginInfo{.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
	commandBuffer.begin(beginInfo);

	return std::move(commandBuffer);
}

void Monster::endSingleTimeCommands(vk::raii::CommandBuffer &&commandBuffer)
{
	commandBuffer.end();

	vk::SubmitInfo submitInfo{
		.commandBufferCount = 1,
		.pCommandBuffers = &*commandBuffer
	};

	vkMonsterStats.graphicsQueue.submit(submitInfo, nullptr);
	vkMonsterStats.graphicsQueue.waitIdle();

}

void Monster::createGraphicsPipeline() {

	auto shaderCode = readShaderFile("C:/Users/lenovo/source/repos/mHorseShoeeVCmake/mHorseShoe/src/vktest/shaders/triangle.spv");

	vk::raii::ShaderModule shaderModule = createShaderModule(shaderCode);

	vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = shaderModule, .pName = "vertMain" };

	vk::PipelineShaderStageCreateInfo fragShaderStageCreateInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = shaderModule, .pName = "fragMain" };

	vk::PipelineShaderStageCreateInfo ShaderStages[] = { vertexShaderStageCreateInfo,fragShaderStageCreateInfo };


	vk::PipelineDepthStencilStateCreateInfo depthStencil{
		.depthTestEnable = vk::True,
		.depthWriteEnable = vk::True,
		.depthCompareOp = vk::CompareOp::eLess,
		.depthBoundsTestEnable = vk::False,
		.stencilTestEnable = vk::False
	};

	// Dynamic state
	std::vector<vk::DynamicState> dynamicStates{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };
	vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{ .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()), .pDynamicStates = dynamicStates.data() };

		// vertex input
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescription = Vertex::getAttributeDescriptions();
	vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo{
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &bindingDescription,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size()),
		.pVertexAttributeDescriptions = attributeDescription.data()
	};
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{ .topology = vk::PrimitiveTopology::eTriangleList };

		// viewport - screen size being rendered
		// scissor - crop on that viewport
	vk::Viewport viewport{ 0.0f,0.0f, static_cast<float>(vkMonsterStats.swapChainExtent.width), static_cast<float>(vkMonsterStats.swapChainExtent.height), 0.0f, 1.0f };
	vk::Rect2D scissor{ vk::Offset2D(0.0f,0.0f), vkMonsterStats.swapChainExtent };

	vk::PipelineViewportStateCreateInfo viewportCreateStateInfo{ .viewportCount = 1, .pViewports = &viewport, .scissorCount = 1, .pScissors = &scissor };

	
	// RASTERIZER

	vk::PipelineRasterizationStateCreateInfo rasterizer{
		.depthClampEnable = false,
		.rasterizerDiscardEnable = false,
		.polygonMode = vk::PolygonMode::eFill,
		.cullMode = vk::CullModeFlagBits::eBack,
		.frontFace = vk::FrontFace::eCounterClockwise,
		.depthBiasEnable = false,
		.lineWidth = 1.0f
	};


	// MULTISAMPLING
	vk::PipelineMultisampleStateCreateInfo multisampleCreateInfo{ .rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = false };

	// Color-blending
	vk::PipelineColorBlendAttachmentState colorBlendState{ 
		.blendEnable = false, 
		.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA 
	};


	vk::PipelineColorBlendStateCreateInfo colorBlendInfo{
		.logicOpEnable = false,
		.logicOp = vk::LogicOp::eCopy,
		.attachmentCount = 1,
		.pAttachments = &colorBlendState
	};

	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{
		.setLayoutCount = 1,
		.pSetLayouts = &*vkDescriptors.descriptorSetLayout,
		.pushConstantRangeCount = 0,

	};

	vkDescriptors.pipelineLayout = vk::raii::PipelineLayout(vkMonsterStats.device, pipelineLayoutCreateInfo);


	vk::PipelineRenderingCreateInfo renderingCreateInfo{
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &vkMonsterStats.swapChainSurfaceFormat.format,
		.depthAttachmentFormat = vkTextures.depthFormat
	};

	vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo{
		.stageCount = 2,
		.pStages = ShaderStages,
		.pVertexInputState = &vertexInputCreateInfo,
		.pInputAssemblyState = &inputAssemblyStateCreateInfo,
		.pViewportState = &viewportCreateStateInfo,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampleCreateInfo,
		.pDepthStencilState = &depthStencil,
		.pColorBlendState = &colorBlendInfo,
		.pDynamicState = &dynamicStateCreateInfo,
		.layout = vkDescriptors.pipelineLayout,
		.renderPass = nullptr,
		
	};

	vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
		graphicsPipelineCreateInfo,
		renderingCreateInfo
	};


	vkMonsterStats.graphicsPipeline = vk::raii::Pipeline(vkMonsterStats.device, nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());

}

void Monster::createCommandPool() {
	vk::CommandPoolCreateInfo poolInfo{
		.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = vkMonsterStats.queueIndex
	};
	vkMonsterStats.commandPool = vk::raii::CommandPool(vkMonsterStats.device, poolInfo);

}

void Monster::createCommandBuffer() {

	vk::CommandBufferAllocateInfo allocInfo{
		.commandPool = vkMonsterStats.commandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = MAX_FRAMES_IN_FLIGHT
	};

	vkMonsterStats.commandBuffers = std::move(vk::raii::CommandBuffers(vkMonsterStats.device, allocInfo));

}
void Monster::createDepthResources()
{
	/*
	vk::Format::eD32Sfloat: 32-bit float for depth

	vk::Format::eD32SfloatS8Uint: 32-bit signed float for depth and 8 bit stencil component

	vk::Format::eD24UnormS8Uint: 24-bit float for depth and 8 bit stencil component
	*/

	vkTextures.depthFormat = findDepthFormat();

	std::tie(vkTextures.depthImage, vkTextures.depthImageAlloc) = createImage(
		vkMonsterStats.swapChainExtent.width,
		vkMonsterStats.swapChainExtent.height,
		vkTextures.depthFormat,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		0,
		VMA_MEMORY_USAGE_AUTO
	);
	vkTextures.depthImageView = createImageView(vkTextures.depthImage, vkTextures.depthFormat, vk::ImageAspectFlagBits::eDepth);


}



void Monster::createTextureImage()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(
		"C:/Users/lenovo/source/repos/mHorseShoeeVCmake/mHorseShoe/src/vktest/textures/praise_the_sun.png",
		&texWidth,
		&texHeight,
		&texChannels,
		STBI_rgb_alpha
	);
	vk::DeviceSize imageSize = texWidth * texHeight * 4;

	

	if (!pixels)
	{
		throw std::runtime_error("Failed to load image");
	}

	// staging buffer for texture
	auto [stagingBuffer, stagingAlloc] =
		createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			VMA_MEMORY_USAGE_AUTO
		);

	void* data = nullptr;
	vmaMapMemory(vkMemAlloc.vmaAllocator, stagingAlloc, &data);
	memcpy(data, pixels, imageSize);
	vmaUnmapMemory(vkMemAlloc.vmaAllocator, stagingAlloc);

	//vmaCopyMemoryToAllocation(vkMemAlloc.vmaAllocator, pixels, stagingAlloc, 0, imageSize);

	std::tie(vkTextures.textureImage, vkTextures.textureAlloc) = createImage(
		texWidth,
		texHeight,
		vkMonsterStats.swapChainSurfaceFormat.format,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		0,
		VMA_MEMORY_USAGE_AUTO
	);

	vk::raii::CommandBuffer commandBuffer = begineSingleTimeCommands();
	transitionImageLayout(commandBuffer, vkTextures.textureImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	copyBufferToImage(
		commandBuffer,
		stagingBuffer,
		vkTextures.textureImage,
		static_cast<uint32_t>(texWidth),
		static_cast<uint32_t>(texHeight)
	);
	transitionImageLayout(commandBuffer, vkTextures.textureImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
	endSingleTimeCommands(std::move(commandBuffer));

	vmaDestroyBuffer(vkMemAlloc.vmaAllocator, stagingBuffer, stagingAlloc);

	stbi_image_free(pixels);
}

void Monster::createTextureImageView()
{

	vkTextures.textureImageView = createImageView(
		*vkTextures.textureImage,
		vk::Format::eR8G8B8A8Srgb,
		{ vk::ImageAspectFlagBits::eColor}
	);

}

void Monster::createTextureSampler()
{

	vk::PhysicalDeviceProperties properties = vkMonsterStats.gpuDevice.getProperties();

	vk::SamplerCreateInfo samplerInfo{
		.magFilter = vk::Filter::eLinear,
		.minFilter = vk::Filter::eLinear,
		.mipmapMode = vk::SamplerMipmapMode::eLinear,
		.addressModeU = vk::SamplerAddressMode::eRepeat,
		.addressModeV = vk::SamplerAddressMode::eRepeat,
		.addressModeW = vk::SamplerAddressMode::eRepeat,
		.anisotropyEnable = vk::True,
		.maxAnisotropy = properties.limits.maxSamplerAnisotropy,
		.compareEnable = vk::False,
		.compareOp = vk::CompareOp::eAlways,
		.borderColor = vk::BorderColor::eIntOpaqueBlack,
		.unnormalizedCoordinates = vk::False,
	};

	vkTextures.textureSampler = vk::raii::Sampler(vkMonsterStats.device, samplerInfo);




}


void Monster::transition_image_layout(
	uint32_t imageIndex,
	vk::ImageLayout old_layout,
	vk::ImageLayout new_layout,
	vk::AccessFlags2 src_access_mask,
	vk::AccessFlags2 dst_access_mask,
	vk::PipelineStageFlags2 src_stage_mask,
	vk::PipelineStageFlags2 dst_stage_mask,
	vk::ImageAspectFlags flags
) {
	vk::ImageMemoryBarrier2 barriar = {
		.srcStageMask = src_stage_mask,
		.srcAccessMask = src_access_mask,
		.dstStageMask = dst_stage_mask,
		.dstAccessMask = dst_access_mask,
		.oldLayout = old_layout,
		.newLayout = new_layout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = vkMonsterStats.swapChainImages[imageIndex],
		.subresourceRange = {
			.aspectMask = flags,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1} };

	vk::DependencyInfo dependency_info = {
		.dependencyFlags = {},
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barriar
	};

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].pipelineBarrier2(dependency_info);

}

vk::Format Monster::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
	for (const auto format : candidates)
	{
		vk::FormatProperties props = vkMonsterStats.gpuDevice.getFormatProperties(format);

		if (((tiling == vk::ImageTiling::eLinear) && ((props.linearTilingFeatures & features) == features)) ||
			((tiling == vk::ImageTiling::eOptimal) && ((props.optimalTilingFeatures & features) == features)))
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format");

}

vk::Format Monster::findDepthFormat()
{
	return findSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

void Monster::recordCommandBuffer(uint32_t imageIndex) {

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].begin({});

	transition_image_layout(
		imageIndex,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal,
		{},
		vk::AccessFlagBits2::eColorAttachmentWrite,
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		vk::ImageAspectFlagBits::eColor
	);

	transition_image_layout(
		imageIndex,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eDepthAttachmentOptimal,
		vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
		vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
		vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
		vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
		vk::ImageAspectFlagBits::eDepth
	);

	vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
	vk::ClearValue clearDepth = vk::ClearDepthStencilValue(1.0f, 0);

	// SWAPCHAIN ATTACHMENT
	vk::RenderingAttachmentInfo attachmentInfo = {
		.imageView = vkMonsterStats.swapChainImageViews[imageIndex],
		.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.clearValue = clearColor

	};

	vk::RenderingAttachmentInfo depthAttachmentInfo = {
		.imageView = vkTextures.depthImageView,
		.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eDontCare,
		.clearValue = clearDepth
	};

	vk::RenderingInfo renderingInfo = {
		.renderArea = {.offset = {0, 0}, .extent = vkMonsterStats.swapChainExtent},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentInfo,
		.pDepthAttachment = &depthAttachmentInfo
	};


	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].beginRendering(renderingInfo);

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, *vkMonsterStats.graphicsPipeline);

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindVertexBuffers(0, *vkMemAlloc.vertexBuffer, { 0 });
	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindIndexBuffer(*vkMemAlloc.indexBuffer, 0, vk::IndexType::eUint16);

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(vkMonsterStats.swapChainExtent.width), static_cast<float>(vkMonsterStats.swapChainExtent.height), 0.0f, 1.0f));
	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vkMonsterStats.swapChainExtent));

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindDescriptorSets(
		vk::PipelineBindPoint::eGraphics, vkDescriptors.pipelineLayout, 0, *vkDescriptors.descriptorSets[vkMonsterStats.frameIndex], nullptr
	);
	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].endRendering();

	transition_image_layout(
		imageIndex,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::ePresentSrcKHR,
		vk::AccessFlagBits2::eColorAttachmentWrite,
		{},
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		vk::PipelineStageFlagBits2::eBottomOfPipe,
		vk::ImageAspectFlagBits::eColor
	);

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].end();
}

void Monster::createSyncObjects() {

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkSyncStats.presentCompleteSemaphores.emplace_back(vkMonsterStats.device, vk::SemaphoreCreateInfo());
		vkSyncStats.renderFinishedSemaphores.emplace_back(vkMonsterStats.device, vk::SemaphoreCreateInfo());
		vkSyncStats.inFlightFences.emplace_back(vkMonsterStats.device, vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
	}

}

void Monster::cleanupSwapChain() {
	vkMonsterStats.swapChainImages.clear();
	vkMonsterStats.swapChainImageViews.clear();
	vkMonsterStats.swapChain = nullptr;
}

void Monster::recreateSwapChain() {
	vkMonsterStats.device.waitIdle();
	cleanupSwapChain();
	createSwapchain();
	createImageView();
	createDepthResources();

}

void Monster::createVertexBuffer() {
	
	vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	// Create a staging buffer (stored in the CPU for quick access and change)
	auto [stagingBuffer,stagingBufferAlloc] = createBuffer(
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
	vkMemAlloc.vertexBuffer = vk::raii::Buffer(vkMonsterStats.device, vkBuffer);
	vkMemAlloc.vertexBufferAlloc = vkBufferAlloc;

	copyBuffer(stagingBuffer, vkBuffer, bufferSize);

}

void Monster::createIndexBuffer() {
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
		vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		VMA_MEMORY_USAGE_AUTO
	);
	vkMemAlloc.indexBuffer = vk::raii::Buffer(vkMonsterStats.device, vkBuffer);
	vkMemAlloc.indexBufferAlloc = vkBufferAlloc;

	copyBuffer(stagingBuffer, vkBuffer, bufferSize);

}

void Monster::createUniformBuffers()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DeviceSize bufferSize = sizeof(UniformBufferObject);


		auto [buffer, alloc] = createBuffer(bufferSize,
			vk::BufferUsageFlagBits::eUniformBuffer,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO);
		vkMemAlloc.uniformBuffers.emplace_back(std::move(vk::raii::Buffer(vkMonsterStats.device,buffer)));
		vkMemAlloc.uniformBufferAlloc.emplace_back(std::move(alloc));
		vkMemAlloc.uniformBuffersMapped.emplace_back(std::move(alloc->GetMappedData()));
		
	}
}

void Monster::createDescriptorPool()
{

	std::array<vk::DescriptorPoolSize, 2>poolSize{ {
		{.type = vk::DescriptorType::eUniformBuffer,
		.descriptorCount = MAX_FRAMES_IN_FLIGHT
		},
		{
		.type = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = MAX_FRAMES_IN_FLIGHT
		}
	} };

	vk::DescriptorPoolCreateInfo poolInfo{
		.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
		.maxSets = MAX_FRAMES_IN_FLIGHT,
		.poolSizeCount = static_cast<uint32_t>(poolSize.size()),
		.pPoolSizes = poolSize.data()
	};

	vkDescriptors.descriptorPool = vk::raii::DescriptorPool(vkMonsterStats.device, poolInfo);

}

void Monster::createDescriptorSets()
{
	// set layout for descriptor Sets
	std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *vkDescriptors.descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo{
		.descriptorPool = vkDescriptors.descriptorPool,
		.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
		.pSetLayouts = layouts.data()
	};

	// Allocate Descriptor sets
	vkDescriptors.descriptorSets = vkMonsterStats.device.allocateDescriptorSets(allocInfo);

	//configure allocated descriptor sets
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DescriptorBufferInfo bufferInfo{
			.buffer = vkMemAlloc.uniformBuffers[i],
			.offset = 0,
			.range = sizeof(UniformBufferObject)
		};

		vk::DescriptorImageInfo imageInfo{
			.sampler = vkTextures.textureSampler,
			.imageView = vkTextures.textureImageView,
			.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
		};

		/*
		vk::WriteDescriptorSet descriptorWrite{
			.dstSet = vkDescriptors.descriptorSets[i],
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = vk::DescriptorType::eUniformBuffer,
			.pBufferInfo = &bufferInfo
		};
		*/
		std::array<vk::WriteDescriptorSet, 2>descriptorWrites{ {
			{
			.dstSet = vkDescriptors.descriptorSets[i],
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = vk::DescriptorType::eUniformBuffer,
			.pBufferInfo = &bufferInfo
			},
			{
			.dstSet = vkDescriptors.descriptorSets[i],
			.dstBinding = 1,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = vk::DescriptorType::eCombinedImageSampler,
			.pImageInfo = &imageInfo
			}
		} };

		vkMonsterStats.device.updateDescriptorSets(descriptorWrites, {});

	}


}

std::pair<VkBuffer, VmaAllocation> Monster::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, VmaAllocationCreateFlags allocFlags, VmaMemoryUsage allocUsage) {

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

	auto result = vmaCreateBuffer(vkMemAlloc.vmaAllocator, bufferInfo, &allocInfo, &buffer, &allocation, nullptr);

	if (result != VkResult::VK_SUCCESS)
	{
		throw std::runtime_error("Unbale to create vertex buffer");
	}

	return {std::move(buffer),std::move(allocation)};
}

std::pair<vk::raii::Image, VmaAllocation> Monster::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage,VmaAllocationCreateFlags allocFlags, VmaMemoryUsage allocUsage)
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


	vmaCreateImage(vkMemAlloc.vmaAllocator, imageInfo, &allocInfo, &vkImage, &allocation, nullptr);

	return {
		std::move(vk::raii::Image(vkMonsterStats.device,vkImage)),
		std::move(allocation)
	};

	

}

void Monster::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize bufferSize)
{

	vk::raii::CommandBuffer commandCopyBuffer = begineSingleTimeCommands();
	commandCopyBuffer.copyBuffer(srcBuffer, dstBuffer, vk::BufferCopy{ .size = bufferSize });
	endSingleTimeCommands(std::move(commandCopyBuffer));

}

void Monster::copyBufferToImage(vk::raii::CommandBuffer& commandBuffer, VkBuffer buffer, vk::raii::Image& image, uint32_t width, uint32_t height)
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

void Monster::updateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	
	UniformBufferObject ubo{};

	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(vkMonsterStats.swapChainExtent.width) / static_cast<float>(vkMonsterStats.swapChainExtent.height), 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	memcpy(vkMemAlloc.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));

}

void Monster::transitionImageLayout(vk::raii::CommandBuffer& commandBuffer, const vk::raii::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
	vk::ImageMemoryBarrier barrier{
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = vk::QueueFamilyIgnored,
		.dstQueueFamilyIndex = vk::QueueFamilyIgnored,
		.image = image,
		.subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1}
	};

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = {};
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {

		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;

	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, {}, nullptr, barrier);
}
