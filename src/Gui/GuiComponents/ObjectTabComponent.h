#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

// The value for Rsus Input is not correct check once again

class ObjectTabModelComponent : public ModelComponent {
	
public:

	SunWindowSize* windowSize;
	std::string* materialName = new std::string("");

	bool wireframeMode = false;

	int selectedMaterial = 0;

	ObjectTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {
		//selectedObj = this->gdSource->getCaseHandler()->getSelectedObject();
		windowSize = this->gdSource->getGdSystem()->getWindowSize();
		//materials = this->gdSource->getResourceHandler()->getAllMaterial();
	}

	void setMaterial() {
		if (!materialName->empty())
		{
			if (!ModelComponent::selectedObject->selObject.lock()->setMaterial(ModelComponent::materialDpVec->at(selectedMaterial), *materialName))
			{
				ToastComponent::GetInstance()->addMessage("Failed to create Material!");
			}
			else {
				ModelComponent::selectMaterial(ModelComponent::selectedObject->selObject.lock()->getwMaterial());
			}

		}
		
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

