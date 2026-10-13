#pragma once

#include <base/Mediator.h>

class GuiSlugViewerModel : public ModelComponent {
public:

	std::shared_ptr<Observ<SlugMesh>> slugMesh;

	GuiSlugViewerModel(const char* name_p) : ModelComponent(name_p) {}

	void init() override {

		this->slugMesh = this->gdSource->getCaseHandler()->slugMesh;

	}

	void loadSlugMesh() {
		this->gdSource->getCaseHandler()->loadSlugObject("C:/Users/lenovo/source/repos/mHorseShoeeVCmake/mHorseShoe/src/slughorn/viaoda_libre.ttf");
	}
	
};

class GuiSlugViewerView : public ViewComponent {
public:
	GuiSlugViewerModel* model;
	GuiSlugViewerView(GuiSlugViewerModel* model_p,const char* name_p): ViewComponent(name_p)
	{
		model = model_p;
	}

	void view() override;
};