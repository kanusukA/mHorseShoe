#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

// The value for Rsus Input is not correct check once again

class ObjectTabModelComponent : public ModelComponent {
	
public:

	SunWindowSize* windowSize;

	std::weak_ptr<Object> selectedObj;

	std::vector<MaterialResource*>* materials;
	int selectedMaterial = 0;

	ObjectTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {
		//selectedObj = this->gdSource->getCaseHandler()->getSelectedObject();
		windowSize = this->gdSource->getGdSystem()->getWindowSize();
		//materials = this->gdSource->getResourceHandler()->getAllMaterial();
	}

	void getFragShader() {
		// INTIGRATE AFTER SETTING RSUS
		
		this->gdSource->getResourceHandler()->loadImgToTex();
	}

	void deleteObject() {
		//this->gdSource->getStuffHandler()->deleteSelectedObj();
	}

	void setMaterial() {
		//selectedObj->get()->setMaterial(materials->at(selectedMaterial)->getHigherRef());
	}

	void selectShader(Shader* vertShader,Shader* fragShader) {
		/*this->gdSource->getShaderHandler()->setShader(selectedObj->get()->getMaterial()->getName(),
			fragShader->getShaderName(),
			vertShader->getShaderName(),
			fragShader->getShaderVars(),
			vertShader->getShaderVars(),
			fragShader->getShader(),
			vertShader->getShader()
		);*/
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

