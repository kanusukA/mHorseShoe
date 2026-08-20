#pragma once

#include "MonsterVulkan.h"


//#define VMA_IMPLEMENTATION
////#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



const std::vector<vulkanUtils::Vertex> p_vertices = {
	{{-0.5f, -0.5f, 0.5f}, {1.0f,1.0f,1.0f},{1.0f, 0.0f} },
	{{0.5f, -0.5f,0.5f}, {0.0f,1.0f,0.0f}, {0.0f, 0.0f} },
	{{0.5f, 0.5f,0.5f}, {0.0f,0.0f,1.0f}, {0.0f, 1.0f} },
	{{-0.5f, 0.5f,0.5f}, {1.0f,1.0f,1.0f},{1.0f, 1.0f} }
};

const std::vector<uint16_t> p_indices = {
	0, 1, 2, 2, 3, 0
};

void MonsterVulkan::renderVulkanFrame(ImDrawData* drawData) {

	// WAIT FOR SIGNAL FROM GPU THAT INDICATE RENDERING HAS FINISHED
	auto fenceResult = vkMonsterStats.device.waitForFences(*vkSyncStats.inFlightFences[vkMonsterStats.frameIndex], true, UINT64_MAX);
	if (fenceResult != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to wait for fence!");
	}

	// ACQUIRE NEW SWAPCHAIN IMAGE VIEW TO WHICH WE WILL ADD COLOR
	auto [result, imageIndex] = vkMonsterStats.swapChain.acquireNextImage(UINT64_MAX, *vkSyncStats.presentCompleteSemaphores[vkMonsterStats.frameIndex], nullptr);

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || sdlStats->framebufferResized)
	{
		sdlStats->framebufferResized = false;
		recreateSwapChain();
		return;
	}
	if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
	{
		throw std::runtime_error("failed to acquire swapchain");
	}

	// SET FENCES BACK TO NORMAL STATE (HAS TO BE DONE MANUALLY)
	vkMonsterStats.device.resetFences(*vkSyncStats.inFlightFences[vkMonsterStats.frameIndex]);

	// UPDATE BUFFERS
	updateUniformBuffer(vkMonsterStats.frameIndex);

	// RESET COMMAND BUFFERS
	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].reset();
	//vkMonsterStats.commandBuffers[MAX_FRAMES_IN_FLIGHT + 1].reset();

	// RECORD COMMANDS. SEE MORE
	recordCommandBuffer(imageIndex, drawData);


	// submitting command buffer
	vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

	const vk::SubmitInfo submitInfo{
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &*vkSyncStats.presentCompleteSemaphores[vkMonsterStats.frameIndex], // waits until this semaphore is triggered
		.pWaitDstStageMask = &waitDestinationStageMask,
		.commandBufferCount = 1,
		.pCommandBuffers = &*vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex],
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &*vkSyncStats.renderFinishedSemaphores[vkMonsterStats.frameIndex] // triggers this semaphore
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
		.pWaitSemaphores = &*vkSyncStats.renderFinishedSemaphores[vkMonsterStats.frameIndex], // waits for this
		.swapchainCount = 1,
		.pSwapchains = &*vkMonsterStats.swapChain,
		.pImageIndices = &imageIndex,

	};

	auto presentResult = vkMonsterStats.graphicsQueue.presentKHR(presentInfoKHR);

	if ((presentResult == vk::Result::eSuboptimalKHR) || (presentResult == vk::Result::eErrorOutOfDateKHR) || sdlStats->framebufferResized)
	{
		sdlStats->framebufferResized = false;
		recreateSwapChain();
	}

	vkMonsterStats.frameIndex = (vkMonsterStats.frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

}




void MonsterVulkan::InitVulkan(uint16_t windowWidth, uint16_t windowHeight) {

	InitSDLWindow(windowWidth, windowHeight);
	

	createVulkanInstance();
	setupDebugMessenger();
	createVulkanSurface();
	pickVulkanPhysicalDevice();
	createVulkanDevice();
	createVulkanMemAllocator();
	createSwapchain();
	createImageView();

	InitMonsterSlang(&vkMonsterStats, &vkDescriptors, &vkMemAlloc);
	InitMonsterBuffer( &vkMemAlloc, &vkMonsterStats);
	createDepthResources();
	createGraphicsPipeline();
	createCommandPool();
	createCommandBuffer();
	//createTextureImage();
	//createTextureImageView();
	//createTextureSampler();
	//createVertexBuffer();
	//createIndexBuffer();
	//createUniformBuffers();
	createDescriptorPool();
	//createDescriptorSets();
	createSyncObjects();




}

void MonsterVulkan::ShutdownVulkan()
{
	vkMonsterStats.device.waitIdle();
	vmaDestroyAllocator(vkMemAlloc.vmaAllocator);
}



void MonsterVulkan::createVulkanInstance() {

	uint32_t major = VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE);
	uint32_t minor = VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE);
	uint32_t patch = VK_HEADER_VERSION; // Alternately: VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)

	

	// 2. Print the version string
	std::cout << "Vulkan SDK Header Version: "
		<< major << "."
		<< minor << "."
		<< patch << std::endl;

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

	for (const auto& layer : layerProperties)
	{
		std::cout << "LAYER : " << layer.layerName << std::endl;
	}

	auto availExt = vkMonsterStats.Context.enumerateInstanceExtensionProperties();
	for (const auto& ext: availExt)
	{
		std::cout << "EXTENSION : " << ext.extensionName << std::endl;
	}
	

	int count_extensions = extensionCount + 1;
	const char** extensions = static_cast<const char**>(SDL_malloc(count_extensions * sizeof(const char*)));
	extensions[0] = vk::EXTDebugUtilsExtensionName;
	//extensions[1] = vk::KHRGetDisplayProperties2ExtensionName;
	//extensions[1] = vk::KHRBufferDeviceAddressExtensionName;
	//extensions[2] = vk::KHRShaderUntypedPointersExtensionName;
	/*extensions[2] = vk::EXTDescriptorBufferExtensionName;*/
	SDL_memcpy(&extensions[1], instance_extensions, extensionCount * sizeof(const char*));
	
	

	for (size_t i = 0; i < count_extensions; i++)
	{
		std::cout << "VULKAN Extension : " << extensions[i] << std::endl;
	}


	vk::ApplicationInfo appInfo{};
	appInfo.pEngineName = "mHorseShoee";
	appInfo.pApplicationName = "Psycho";
	appInfo.applicationVersion = 10000;
	appInfo.engineVersion = 10001;
	appInfo.apiVersion = VK_API_VERSION_1_4;

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = count_extensions;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
	createInfo.ppEnabledLayerNames = requiredLayers.data();

	vkMonsterStats.vkInstance = vk::raii::Instance(vkMonsterStats.Context, createInfo);


}

void MonsterVulkan::setupDebugMessenger()
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

void MonsterVulkan::createVulkanSurface() {

	VkSurfaceKHR surface;

	if (!SDL_Vulkan_CreateSurface(sdlStats->window, *vkMonsterStats.vkInstance, nullptr, &surface)) {
		std::cerr << "SDL ERROR : " << SDL_GetError() << std::endl;
		throw std::runtime_error(SDL_GetError());
	}

	vkMonsterStats.surface = vk::raii::SurfaceKHR(vkMonsterStats.vkInstance, surface);

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

void MonsterVulkan::pickVulkanPhysicalDevice() {
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


void MonsterVulkan::createVulkanDevice() {

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
		vk::KHRSwapchainExtensionName,
		vk::EXTDescriptorBufferExtensionName
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

	// EXT FUNCTIONS
	vkGetBufferDeviceAddressMON = (PFN_vkGetBufferDeviceAddressEXT)vkGetDeviceProcAddr(*vkMonsterStats.device, "vkGetBufferDeviceAddressEXT");
	vkWriteResourceDescriptorsMON = (PFN_vkWriteResourceDescriptorsEXT)vkGetDeviceProcAddr(*vkMonsterStats.device, "vkWriteResourceDescriptorsEXT");

}

void MonsterVulkan::createVulkanMemAllocator() {
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


void MonsterVulkan::createSwapchain() {
	auto surfaceCapabilities = vkMonsterStats.gpuDevice.getSurfaceCapabilitiesKHR(*vkMonsterStats.surface);

	// supported surface formats
	std::vector<vk::SurfaceFormatKHR> availableFormats = vkMonsterStats.gpuDevice.getSurfaceFormatsKHR(vkMonsterStats.surface);

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
		[](const auto& format) {return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; });

	vkMonsterStats.swapChainSurfaceFormat = formatIt != availableFormats.end() ? *formatIt : availableFormats[0];

	// Present mode
	// There are foure possible modes in vulkan
	// - eImmediate - transfered to screen right away, may cause tearing
	// - eFifo - similar to vertical sync, queue is set and is changed when display is refreshed
	// - eFifoRelaxed - if the queue is empty it's shown right away, may cause tearing
	// - eMailBox - also known as 'triple buffer, replaces item in queue if it's full. Is more energy demanding.

	// eFifo is always present.
	assert(std::ranges::any_of(availablePresentModes, [](auto presentMode == ) {return vk::PresentModeKHR::eFifo; }));
	vk::PresentModeKHR presentMode = std::ranges::any_of(availablePresentModes, [](const vk::PresentModeKHR value) {return vk::PresentModeKHR::eMailbox == value; }) ?
		vk::PresentModeKHR::eMailbox :
		vk::PresentModeKHR::eFifo;

	// Swap extent

	if (surfaceCapabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)()) {
		vkMonsterStats.swapChainExtent = surfaceCapabilities.currentExtent;
	}

	int width = windowWidth;
	int height = windowHeight;
	


	vkMonsterStats.swapChainExtent = vk::Extent2D(
		std::clamp<uint32_t>(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
		std::clamp<uint32_t>(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
	);

	// image count - how many images to store in swapchain
	auto minImageCount = (std::max)(3u, surfaceCapabilities.minImageCount);
	if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount)) {
		minImageCount = surfaceCapabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR swapChainCreateInfo{
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

void MonsterVulkan::createImageView() {

	assert(vkMonsterStats.swapChainImages.empty());

	vkMonsterStats.swapChainImageViews.reserve(vkMonsterStats.swapChainImages.size());
	for (const auto& image : vkMonsterStats.swapChainImages)
	{
		vkMonsterStats.swapChainImageViews.emplace_back(createImageView(image, vkMonsterStats.swapChainSurfaceFormat.format, vk::ImageAspectFlagBits::eColor));
	}
}

vk::raii::ImageView MonsterVulkan::createImageView(vk::Image const& image, vk::Format format, vk::ImageAspectFlags flags)
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

//void MonsterVulkan::createDescriptiorSetLayout()
//{
//
//	std::array<vk::DescriptorSetLayoutBinding, 2> bindings{
//		{{
//		.binding = 0,
//		.descriptorType = vk::DescriptorType::eUniformBuffer,
//		.descriptorCount = 1,
//		.stageFlags = vk::ShaderStageFlagBits::eVertex
//		},{
//		.binding = 1,
//		.descriptorType = vk::DescriptorType::eCombinedImageSampler,
//		.descriptorCount = 1,
//		.stageFlags = vk::ShaderStageFlagBits::eFragment
//		}}
//	};
//
//	vk::DescriptorSetLayoutCreateInfo layoutInfo{
//		.bindingCount = static_cast<uint32_t>(bindings.size()),
//		.pBindings = bindings.data()
//	};
//	vkDescriptors.descriptorSetLayout = vk::raii::DescriptorSetLayout(vkMonsterStats.device, layoutInfo);
//}


void MonsterVulkan::createGraphicsPipeline() {
	std::vector<char> shaderCode = std::vector<char>();
	ResourceHandler::GetInstance()->readFileContents("../../../src/monster/shaders/triangle.spv", &shaderCode);

	vk::raii::ShaderModule shaderModule = createShaderModule(shaderCode);

	//vkMonsterStats.graphicsPipeline = createGraphicsPipeline(*shaderModule, *shaderModule,"vertMain","fragMain");
}

std::shared_ptr<vk::raii::Pipeline> MonsterVulkan::createGraphicsPipeline(
	const vk::ShaderModule& vertShaderModule, // function name must start with vertMain
	const vk::ShaderModule& fragShaderModule, // function name must start with fragMain
	const std::string& entryPointVert,
	const std::string& entryPointFrag,
	const vk::DescriptorSetLayout& descriptorLayouts,
	vk::PolygonMode polygonMode ,
	vk::CullModeFlags cullingModes ,
	vk::FrontFace frontFace ,
	float lineWidth 
) {

	/*std::vector<char> shaderCode = std::vector<char>();
	ResourceHandler::GetInstance()->readFileContents("../../../src/monster/shaders/triangle.spv", &shaderCode);*/

	//vk::raii::ShaderModule shaderModule = createShaderModule(shaderCode);

	vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = vertShaderModule, .pName = entryPointVert.c_str()};

	vk::PipelineShaderStageCreateInfo fragShaderStageCreateInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = fragShaderModule, .pName = entryPointFrag.c_str()};

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
	auto bindingDescription = vulkanUtils::Vertex::getBindingDescription();
	auto attributeDescription = vulkanUtils::Vertex::getAttributeDescriptions();
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
		.polygonMode = polygonMode,
		.cullMode = cullingModes,
		.frontFace = frontFace,
		.depthBiasEnable = false,
		.lineWidth = lineWidth
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
		.pSetLayouts = &descriptorLayouts,
		.pushConstantRangeCount = 0,

	};

	vkDescriptors.pipelineLayout = vk::raii::PipelineLayout(vkMonsterStats.device, pipelineLayoutCreateInfo);

	// DESCRIPTOR HEAP
	/*VkDescriptorSetAndBindingMappingEXT bufferBindings{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_AND_BINDING_MAPPING_EXT,
		.descriptorSet = 0,
		.firstBinding = 0,
		.bindingCount = 1,
		.resourceMask = VK_SPIRV_RESOURCE_TYPE_UNIFORM_BUFFER_BIT_EXT,
		.source = VK_DESCRIPTOR_MAPPING_SOURCE_HEAP_WITH_CONSTANT_OFFSET_EXT,
		.sourceData = {
			.constantOffset = {
				.heapArrayStride = static_cast<uint32_t>(bufferDescriptorSize)
			}
		}
	};

	VkShaderDescriptorSetAndBindingMappingInfoEXT shaderDescriptorInfo{
		.mappingCount = 1,
		.pMappings = &bufferBindings
	};

	ShaderStages[0].pNext = &shaderDescriptorInfo;
	ShaderStages[1].pNext = &shaderDescriptorInfo;*/


	vk::PipelineRenderingCreateInfo renderingCreateInfo{
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &vkMonsterStats.swapChainSurfaceFormat.format,
		.depthAttachmentFormat = depthTexture->imgFormat 
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
		.layout = *vkDescriptors.pipelineLayout,
		.renderPass = nullptr,

	};

	vk::PipelineCreateFlags2CreateInfo pipelineCreateFlags{
		.pNext = &renderingCreateInfo,
		.flags = vk::PipelineCreateFlagBits2::eDescriptorHeapEXT
	};

	graphicsPipelineCreateInfo.pNext = &pipelineCreateFlags;

	vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
		graphicsPipelineCreateInfo,
		renderingCreateInfo
	};

	std::shared_ptr<vk::raii::Pipeline> pipe = std::make_shared<vk::raii::Pipeline>(vk::raii::Pipeline(vkMonsterStats.device, nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>()));
	pipes.push_back(std::move(pipe));

	return pipes.back();

}

void MonsterVulkan::createCommandPool() {
	vk::CommandPoolCreateInfo poolInfo{
		.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = vkMonsterStats.queueIndex
	};
	vkMonsterStats.commandPool = vk::raii::CommandPool(vkMonsterStats.device, poolInfo);

}

void MonsterVulkan::createCommandBuffer() {

	vk::CommandBufferAllocateInfo allocInfo{
		.commandPool = vkMonsterStats.commandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = MAX_FRAMES_IN_FLIGHT + 1 // For IMGUI
	};

	vkMonsterStats.commandBuffers = std::move(vk::raii::CommandBuffers(vkMonsterStats.device, allocInfo));

}
void MonsterVulkan::createDepthResources()
{
	/*
	vk::Format::eD32Sfloat: 32-bit float for depth

	vk::Format::eD32SfloatS8Uint: 32-bit signed float for depth and 8 bit stencil component

	vk::Format::eD24UnormS8Uint: 24-bit float for depth and 8 bit stencil component
	*/
	depthTexture = new VulkanTexture();

	depthTexture->imgFormat = findDepthFormat();

	std::tie(depthTexture->texture, depthTexture->alloc) = createImage(
		vkMonsterStats.swapChainExtent.width,
		vkMonsterStats.swapChainExtent.height,
		depthTexture->imgFormat,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		0,
		VMA_MEMORY_USAGE_AUTO
	);
	depthTexture->textureView = createImageView(depthTexture->texture, depthTexture->imgFormat, vk::ImageAspectFlagBits::eDepth);


}



//void MonsterVulkan::createTextureImage()
//{
//	std::filesystem::path filepath = std::filesystem::path("../../../src/vktest/textures/praise_the_sun.png");
//	sampleTexture = MonsterSlang::createTextureImage(filepath).get();
//	MonsterSlang::createTextureImageView(sampleTexture);
//	MonsterSlang::createTextureSampler(sampleTexture);
//}


void MonsterVulkan::transition_image_layout(
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

vk::Format MonsterVulkan::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
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

vk::Format MonsterVulkan::findDepthFormat()
{
	return findSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

void MonsterVulkan::recordCommandBuffer(uint32_t imageIndex, ImDrawData* drawData) {

	// COMMAND BUFFERS ARE RECORDED IN ORDER BUT MAY NOT RUN IN THAT ORDER FOR OPTIMIZATION , HENCE BARRIERS ARE USED

	// INITALIZE RECODRING OF COMMAND BUFFER
	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].begin({});

	// MAKE SURE COLOR HAS BEEN PRINTED
	transition_image_layout(
		imageIndex,
		vk::ImageLayout::eUndefined, // Image can be in any layout in input
		vk::ImageLayout::eColorAttachmentOptimal, // must be converted into a color attachment 
		{}, // No read/write is required prior to this step
		vk::AccessFlagBits2::eColorAttachmentWrite, // must be in color Write before continuing
		vk::PipelineStageFlagBits2::eColorAttachmentOutput, // must be in Color ouput mode
		vk::PipelineStageFlagBits2::eColorAttachmentOutput, // continue in color output mode
		vk::ImageAspectFlagBits::eColor
	);

	// Similar transition for Depth Attachment
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
		.imageView = depthTexture->textureView,
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
		.pDepthAttachment = &depthAttachmentInfo,

	};

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].beginRendering(renderingInfo);

	// ORDER INCOMING MESHES BY THE PIPELINE THEY USE

	// add descriptor single time per objects
	/*for (size_t bufferDescripIdx = 0; bufferDescripIdx < bufferHeapAddress.size(); bufferDescripIdx++)
	{
		vk::BindHeapInfoEXT bindHeapInfo{
		.heapRange = {
			.address = bufferHeapAddress[bufferDescripIdx],
			.size = bufferHeapSize[bufferDescripIdx]
		},
		.reservedRangeOffset = bufferHeapSize[bufferDescripIdx] - descriptorHeapProperties.minResourceHeapReservedRange,
		.reservedRangeSize = descriptorHeapProperties.minResourceHeapReservedRange

		};

		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindResourceHeapEXT(bindHeapInfo);
	}*/


	int32_t instance = 0;
	for (const auto& passObj: importedMeshes)
	{
		//hRes::Mesh* passObj = importedMeshes[passObjIndex];
		if (!passObj->shader)
		{
			throw std::runtime_error("OBJECT HAS NO SHADER!!!!");
			continue;
		}

		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics,  *passObj->shader->graphicsPipeline);

		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(vkMonsterStats.swapChainExtent.width), static_cast<float>(vkMonsterStats.swapChainExtent.height), 0.0f, 1.0f));
		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vkMonsterStats.swapChainExtent));

		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindDescriptorSets(
			vk::PipelineBindPoint::eGraphics, vkDescriptors.pipelineLayout, 0, *passObj->shader->descriptorSets->at(vkMonsterStats.frameIndex), nullptr
		);

		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindVertexBuffers(0, *passObj->vertexBuffer->uboBuffer.back(), {0});
		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].bindIndexBuffer(passObj->indexBuffer->uboBuffer.back(), 0, vk::IndexType::eUint16);


		vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].drawIndexed(passObj->indices.size(), instance, 0, 0, 0);

		//vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].draw(3, 1, 0, 0);
		instance++;
	}


	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].endRendering();

	vk::RenderingAttachmentInfo imguiColorInfo = {
		.imageView = vkMonsterStats.swapChainImageViews[imageIndex],
		.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eLoad,
		.storeOp = vk::AttachmentStoreOp::eStore,

	};

	vk::RenderingInfo renderingImguiInfo = {
		.renderArea = {.offset = {0, 0}, .extent = vkMonsterStats.swapChainExtent},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &imguiColorInfo,

	};

	vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex].beginRendering(renderingImguiInfo);

	ImGui_ImplVulkan_RenderDrawData(drawData, *vkMonsterStats.commandBuffers[vkMonsterStats.frameIndex], *vkMonsterStats.imguiPipeline);

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

void MonsterVulkan::createSyncObjects() {

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkSyncStats.presentCompleteSemaphores.emplace_back(vkMonsterStats.device, vk::SemaphoreCreateInfo());
		vkSyncStats.renderFinishedSemaphores.emplace_back(vkMonsterStats.device, vk::SemaphoreCreateInfo());
		vkSyncStats.inFlightFences.emplace_back(vkMonsterStats.device, vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
	}

}

void MonsterVulkan::cleanupSwapChain() {
	vkMonsterStats.swapChainImages.clear();
	vkMonsterStats.swapChainImageViews.clear();
	vkMonsterStats.swapChain = nullptr;
}

void MonsterVulkan::recreateSwapChain() {
	vkMonsterStats.device.waitIdle();
	cleanupSwapChain();
	createSwapchain();
	createImageView();
	createDepthResources();

}

//void MonsterVulkan::createVertexBuffer() {
//	createVertexBuffer(p_vertices);
//}

//uint32_t MonsterVulkan::createVertexBuffer(std::vector<vulkanUtils::Vertex> vertices) {
//	vkMemAlloc.vertices.push_back(vertices.size());
//	vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
//	// Create a staging buffer (stored in the CPU for quick access and change)
//	auto [stagingBuffer, stagingBufferAlloc] = createBuffer(
//		bufferSize,
//		vk::BufferUsageFlagBits::eTransferSrc,
//		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
//		VMA_MEMORY_USAGE_AUTO
//	);
//
//	// add data to the staging buffer
//	vmaCopyMemoryToAllocation(vkMemAlloc.vmaAllocator, vertices.data(), stagingBufferAlloc, 0, bufferSize);
//
//	// create the device_local(graphics crad memory) buffer
//	auto [vkBuffer, vkBufferAlloc] = createBuffer(
//		bufferSize,
//		vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
//		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
//		VMA_MEMORY_USAGE_AUTO
//	);
//	vkMemAlloc.vertexBuffer.push_back(std::move(vk::raii::Buffer(vkMonsterStats.device, vkBuffer)));
//	vkMemAlloc.vertexBufferAlloc.push_back(std::move(vkBufferAlloc));
//
//	copyBuffer(stagingBuffer, vkBuffer, bufferSize);
//
//	return vkMemAlloc.vertexBuffer.size() - 1;
//}
//
////void MonsterVulkan::createIndexBuffer() {
////	createIndexBuffer(p_indices);
////}
//
//uint32_t MonsterVulkan::createIndexBuffer(std::vector<uint16_t> indices) {
//
//	vkMemAlloc.indexes.push_back(indices.size()); // Used during recordCommandBuffer()
//
//	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
//	// Create a staging buffer (stored in the CPU for quick access and change)
//	auto [stagingBuffer, stagingBufferAlloc] = createBuffer(
//		bufferSize,
//		vk::BufferUsageFlagBits::eTransferSrc,
//		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
//		VMA_MEMORY_USAGE_AUTO
//	);
//
//	// add data to the staging buffer
//	vmaCopyMemoryToAllocation(vkMemAlloc.vmaAllocator, indices.data(), stagingBufferAlloc, 0, bufferSize);
//
//	// create the device_local(graphics crad memory) buffer
//	auto [vkBuffer, vkBufferAlloc] = createBuffer(
//		bufferSize,
//		vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
//		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
//		VMA_MEMORY_USAGE_AUTO
//	);
//	vkMemAlloc.indexBuffer.push_back(std::move(vk::raii::Buffer(vkMonsterStats.device, vkBuffer)));
//	vkMemAlloc.indexBufferAlloc.push_back(std::move(vkBufferAlloc));
//
//	copyBuffer(stagingBuffer, vkBuffer, bufferSize);
//
//	return vkMemAlloc.indexBuffer.size() - 1;
//}

//void MonsterVulkan::createUniformBuffers()
//{
//	UniformBufferObject obj = UniformBufferObject();
//	MonsterSlang::createUniformBuffers(obj);
//}

void MonsterVulkan::createDescriptorPool()
{

	std::array<vk::DescriptorPoolSize, 2>poolSize{ {
		{.type = vk::DescriptorType::eUniformBuffer,
		.descriptorCount = MAX_FRAMES_IN_FLIGHT * MAX_OBJECT
		},
		{
		.type = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = MAX_FRAMES_IN_FLIGHT * MAX_OBJECT
		}
	} };

	vk::DescriptorPoolCreateInfo poolInfo{
		.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
		.poolSizeCount = static_cast<uint32_t>(poolSize.size()),
		.pPoolSizes = poolSize.data()
	};

	for (const auto& pool : poolSize)
	{
		poolInfo.maxSets += pool.descriptorCount;
	}

	vkDescriptors.descriptorPool = vk::raii::DescriptorPool(vkMonsterStats.device, poolInfo);

}

//void MonsterVulkan::createDescriptorSets()
//{
//	// set layout for descriptor Sets
//	std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *vkDescriptors.descriptorSetLayout);
//	vk::DescriptorSetAllocateInfo allocInfo{
//		.descriptorPool = vkDescriptors.descriptorPool,
//		.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
//		.pSetLayouts = layouts.data()
//	};
//
//	// Allocate Descriptor sets
//	vkDescriptors.descriptorSets = vkMonsterStats.device.allocateDescriptorSets(allocInfo);
//
//	//configure allocated descriptor sets
//	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT * MAX_OBJECT; i++)
//	{
//		vk::DescriptorBufferInfo bufferInfo{
//			.buffer = vkMemAlloc.uniformBuffers[i],
//			.offset = 0,
//			.range = sizeof(UniformBufferObject)
//		};
//
//		vk::DescriptorImageInfo imageInfo{
//			.sampler = vkTextures.textureSampler,
//			.imageView = vkTextures.textureImageView,
//			.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
//		};
//
//		/*
//		vk::WriteDescriptorSet descriptorWrite{
//			.dstSet = vkDescriptors.descriptorSets[i],
//			.dstBinding = 0,
//			.dstArrayElement = 0,
//			.descriptorCount = 1,
//			.descriptorType = vk::DescriptorType::eUniformBuffer,
//			.pBufferInfo = &bufferInfo
//		};
//		*/
//		std::array<vk::WriteDescriptorSet, 2>descriptorWrites{ {
//			{
//			.dstSet = vkDescriptors.descriptorSets[i],
//			.dstBinding = 0,
//			.dstArrayElement = 0,
//			.descriptorCount = 1,
//			.descriptorType = vk::DescriptorType::eUniformBuffer,
//			.pBufferInfo = &bufferInfo
//			},
//			{
//			.dstSet = vkDescriptors.descriptorSets[i],
//			.dstBinding = 1,
//			.dstArrayElement = 0,
//			.descriptorCount = 1,
//			.descriptorType = vk::DescriptorType::eCombinedImageSampler,
//			.pImageInfo = &imageInfo
//			}
//		} };
//
//		vkMonsterStats.device.updateDescriptorSets(descriptorWrites, {});
//
//	}
//
//
//}


void MonsterVulkan::updateUniformBuffer(uint32_t currentImage)
{

	for (auto& passObj: passObjects)
	{
		if (passObj->shader && passObj->shader->buffers)
		{
			passObj->updateTransformBuffer(
				camera->position, 
				camera->front, camera->up, 
				static_cast<float>(vkMonsterStats.swapChainExtent.width), 
				static_cast<float>(vkMonsterStats.swapChainExtent.height)
			);
		}

	}
	//static auto startTime = std::chrono::high_resolution_clock::now();

	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//UniformBufferObject ubo{};

	//ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//ubo.view = glm::lookAt(camera->position, camera->position + camera->front, camera->up);

	//ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(vkMonsterStats.swapChainExtent.width) / static_cast<float>(vkMonsterStats.swapChainExtent.height), 0.1f, 1000.0f);
	////ubo.proj[1][1] *= -1;
	//for (auto& mBuffer : vkMemAlloc.uniformBuffers)
	//{
	//	if (mBuffer && mBuffer->buffers.size() > 0)
	//	{
	//		// FIRST BUFFER IS OF VERTEX MVP BUFFER;
	//		VmaAllocationInfo allocInfo;
	//		vmaGetAllocationInfo(vkMemAlloc.vmaAllocator, mBuffer->bufferAlloc.at(0), &allocInfo);
	//		memcpy(allocInfo.pMappedData, &ubo, sizeof(ubo));
	//	}
	//	
	//}
	
}

void MonsterVulkan::loadMeshToVulkan(std::weak_ptr<hRes::Mesh> mesh)
{

	/*mesh.lock()->vertexBufferIndex = createVertexBuffer(mesh.lock()->vertices);
	mesh.lock()->indexBufferIndex = createIndexBuffer(mesh.lock()->indices);*/

	// VALIDATION - size of index, vertex, vertex buffer, index buffer must be all same!
	if (
		(static_cast<int32_t>(vkMemAlloc.indexes.size()) != static_cast<int32_t>(vkMemAlloc.vertexBuffer.size())) || 
		(static_cast<int32_t>(vkMemAlloc.indexBuffer.size()) != static_cast<int32_t>(vkMemAlloc.vertices.size()))
	)
	{
		throw std::runtime_error("ERROR IN VERTEX AND INDEX BUFFER CREATION!!!");
	}

	

}

void MonsterVulkan::loadMeshShaders(std::weak_ptr<hRes::Mesh> mesh)
{
	//hRes::Mesh* mesh = importedMeshes[meshIndex];

	// read shader files
	/*if (importedMeshes[meshIndex]->shaders.vertShaderFilePath)
	{
		std::vector<char> vertshadercode;
		ResourceHandler::GetInstance()->readFileContents(*importedMeshes[meshIndex]->shaders.vertShaderFilePath, &vertshadercode);
		importedMeshes[meshIndex]->shaders.vertexShader = createShaderModule(vertshadercode);
	}
	else {
		throw std::runtime_error("NO VERTEX SHADER FOUND");
	}
	if (importedMeshes[meshIndex]->shaders.fragShaderFilePath)
	{
		std::vector<char> fragshadercode;
		ResourceHandler::GetInstance()->readFileContents(*importedMeshes[meshIndex]->shaders.fragShaderFilePath, &fragshadercode);
		importedMeshes[meshIndex]->shaders.fragmentShader = createShaderModule(fragshadercode);
	}
	else {
		throw std::runtime_error("NO FRAGMENT SHADER FOUND");
	}*/

	//std::weak_ptr<vulkanUtils::Shader> shader = mesh.lock()->shader;

	//// load graphics pipeline
	//pipes.push_back(std::move(createGraphicsPipeline(shader.lock()->vertexShader, shader.lock()->fragmentShader, "main", "main")));

	//mesh.lock()->graphicsPipelineIndex = pipes.size() - 1;

	//if (!mesh.lock()->vertices.empty() && !mesh.lock()->indices.empty())
	//{
	//	mesh.lock()->isMeshVkLoaded = true;
	//}

	
}

void MonsterVulkan::loadMesh(std::shared_ptr<hRes::Mesh> mesh)
{
	loadMeshToVulkan(mesh);
	loadMeshShaders(mesh);

	if (mesh->isMeshVkLoaded)
	{
		std::shared_ptr<hRes::Mesh> sharedMesh = mesh;
		loadedMeshes.push_back(std::move(sharedMesh));
	}

}



std::shared_ptr<hRes::Mesh> MonsterVulkan::createNewMesh()
{
	importedMeshes.push_back(std::make_shared<hRes::Mesh>());
	return importedMeshes.back();
}

void MonsterVulkan::loadAllMeshes()
{
	for (auto& mesh : importedMeshes)
	{
		loadMesh(mesh);
	}
}

void MonsterVulkan::loadMeshToPassObject()
{
	// check pipeline index
	
	for (const auto& impMesh: importedMeshes)
	{
		if (impMesh->isMeshVkLoaded)
		{
			std::shared_ptr<hRes::Mesh> sharedMesh = impMesh;
			passObjects.push_back(std::move(sharedMesh));
		}
	}

	/*for (size_t pipeIndex = 0; pipeIndex < pipes.size(); pipeIndex++)
	{
		for (size_t VertBufIndex = 0; VertBufIndex < vkMemAlloc.vertexBuffer.size(); VertBufIndex++)
		{
			for (auto mesh: loadedMeshes)
			{
				
				if (mesh->vertexBufferIndex == VertBufIndex && pipeIndex == mesh->graphicsPipelineIndex)
				{
					std::shared_ptr<hRes::Mesh> sharedMesh = mesh;
					passObjects.push_back(std::move(sharedMesh));
				}
			}
		}
		
	}*/
}

void MonsterVulkan::updateObjectTransforms()
{
	for (auto& passObj : passObjects)
	{
		passObj->updateTransformBuffer(camera->position, camera->front, camera->up, static_cast<float>(vkMonsterStats.swapChainExtent.width), static_cast<float>(vkMonsterStats.swapChainExtent.height));
	}
}













