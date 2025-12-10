#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>
#include <imgui_stdlib.h>

class TestingTabModelComponent : public ModelComponent {

public:

	std::vector<ResID>* masterList;
	std::vector<CaseResource*>* caseResources;

	std::string* caseName = new std::string("");

	TestingTabModelComponent(const char* name_p) : ModelComponent(name_p){

	}

	void init() override {
		masterList = this->gdSource->getResourceHandler()->getMasterList();
		caseResources = this->gdSource->getResourceHandler()->getAllCase();
	}

	void addCase() {

		Case myCase = Case("example", this->gdSource->getResourceHandler());
		
	}

};

class TestingTabComponent : public ViewComponent
{
private:
	TestingTabModelComponent* testModel;

public:
	TestingTabComponent(const char* name_p,TestingTabModelComponent* testModel_p) : ViewComponent(name_p) {
		testModel = testModel_p;
	}

	void view() override;
};

