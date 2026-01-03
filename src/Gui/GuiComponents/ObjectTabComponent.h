#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>


class ObjectTabModelComponent : public ModelComponent {
	
public:

	SunWindowSize* windowSize;

	Object* selectedObj;

	ObjectTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {
		selectedObj = this->gdSource->getCaseHandler()->getSelectedObject();
		windowSize = this->gdSource->getGdSystem()->getWindowSize();
	}

	void getFragShader() {
		// INTIGRATE AFTER SETTING RSUS
		
		this->gdSource->getResourceHandler()->loadImgToTex();
	}

	void deleteObject() {
		//this->gdSource->getStuffHandler()->deleteSelectedObj();
	}

};

class ObjectTabComponent : public ViewComponent
{
private:
	ObjectTabModelComponent* objectModel;
public:
	ObjectTabComponent(const char* name_p, ObjectTabModelComponent* objectModel_p) : ViewComponent(name_p) {
		objectModel = objectModel_p;
	}

	void view() override;

};

