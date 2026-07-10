#include "Monster.h"

void Monster::InitVulkan() {


	vk::ApplicationInfo appInfo{};
	appInfo.pEngineName = "mHorseShoee";
	appInfo.pApplicationName = "Psycho";
	appInfo.applicationVersion = 10000;
	appInfo.engineVersion = 10001;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = 0;
	createInfo.ppEnabledExtensionNames = nullptr;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	vkInstance = vk::raii::Instance(Context, createInfo);


	//-------------------------------------------------------------------------------------

	// PICK GPU
	auto physicalDevices = vkInstance.enumeratePhysicalDevices();
	std::multimap<int, vk::PhysicalDevice> suitableDevices;
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

	



}