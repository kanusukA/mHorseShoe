#pragma once

#include <base/Mediator.h>

class GuiVulkanUtilsComponent : public ModelComponent 
{
public:

	std::vector<std::shared_ptr<hRes::Mesh>>* vulkanMeshes = nullptr;
	GuiVulkanUtilsComponent(const char* name_p) : ModelComponent(name_p) {}

	void init() override {
		vulkanMeshes = &this->gdSource->mVulkan->importedMeshes;
	}
};


class GuiVulkanUtilsView : public ViewComponent {

private:
	GuiVulkanUtilsView();

	// MODELS
	GuiVulkanUtilsComponent* model;

public:

	GuiVulkanUtilsView(const char* name_p,
		GuiVulkanUtilsComponent* vulkanComponent
	) : ViewComponent(name_p) {
		model = vulkanComponent;
	};

	void view() override;

};