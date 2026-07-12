#include "Monster.h"




void Monster::InitVulkan() {

	createVulkanInstance();
	createVulkanSurface();
	pickVulkanPhysicalDevice();
	createVulkanDevice();

}

void Monster::createVulkanInstance() {
	// Retrieve Extensions required by the Window System (SDL3 in this case)
	uint32_t extensionCount = 0;
	const char* const* instance_extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

	if (instance_extensions == NULL)
	{
		throw std::exception("SDL FAILED TO PROVIDE EXTENSION DETAILS TO VULKAN");
	}

	int count_extensions = extensionCount + 1;
	const char** extensions = static_cast<const char**>(SDL_malloc(count_extensions * sizeof(const char*)));
	extensions[0] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	SDL_memcpy(&extensions[1], instance_extensions, extensionCount * sizeof(const char*));



	vk::ApplicationInfo appInfo{};
	appInfo.pEngineName = "mHorseShoee";
	appInfo.pApplicationName = "Psycho";
	appInfo.applicationVersion = 10000;
	appInfo.engineVersion = 10001;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = count_extensions;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	vkMonsterStats.vkInstance = vk::raii::Instance(vkMonsterStats.Context, createInfo);
}

void Monster::createVulkanSurface() {

	SDL_PropertiesID properties = SDL_GetWindowProperties(sdlWindow);

	void* hwnd = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

	if (!hwnd)
	{
		throw std::exception("FAILED TO GET WINDOW HWND FROM SDL");
	}

	vk::Win32SurfaceCreateInfoKHR createInfo{
		.hinstance = GetModuleHandle(nullptr),
		.hwnd = static_cast<HWND>(hwnd)
	};

	vkMonsterStats.surface = vkMonsterStats.vkInstance.createWin32SurfaceKHR(createInfo);
	


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
		throw std::exception("No Suitable Device Found!");
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
		vk::PhysicalDeviceVulkan11Features,
		vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

	vkMonsterStats.supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
		features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
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

	
	// Define device Features to be used - as in InitVulkan
	// using StructrueChain - refer to Docs https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/04_Logical_device_and_queues.html

	vk::StructureChain<
		vk::PhysicalDeviceFeatures2,
		vk::PhysicalDeviceVulkan11Features,
		vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
	> featureChain = {
		{},
		{.shaderDrawParameters = true },
		{.dynamicRendering = true},
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