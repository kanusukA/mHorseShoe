#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

// The value for Rsus Input is not correct check once again

class ObjectTabModelComponent : public ModelComponent {
	
public:

	SunWindowSize* windowSize;
	
	std::weak_ptr<Material> material;
	std::weak_ptr<Object> object;

	std::string* materialName = new std::string();

	int selectedMaterial = 0;

	ObjectTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {
		windowSize = this->gdSource->getGdSystem()->getWindowSize();
		material = ModelComponent::selectedMaterial->selMaterial;
		object = ModelComponent::selectedObject->selObject;
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

	void update(GUIUpdateEvent event) override {
		switch (event) {
		case GUIUpdateEvent::OBJECT_UPDATE:
			object = ModelComponent::selectedObject->selObject;
			material = ModelComponent::selectedMaterial->selMaterial;
			if (!ModelComponent::selectedObject->selObject.expired())
			{
				materialName->assign(ModelComponent::selectedObject->selObject.lock()->getMeshMaterialName());
			}
			else {
				materialName->assign("");
			}
			break;
		default:
			break;
		}
	}

};

class ObjectTabComponent : public ViewComponent
{
private:
	ObjectTabModelComponent* model;
public:
	ObjectTabComponent(const char* name_p, ObjectTabModelComponent* objectModel_p) : ViewComponent(name_p) {
		model = objectModel_p;
	}

	void view() override;

};

