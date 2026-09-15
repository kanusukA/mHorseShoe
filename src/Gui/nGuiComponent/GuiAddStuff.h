#pragma once

#include <base/Mediator.h>


class GuiAddStuffModel : public ModelComponent {

public:
	CaseHandler* caseHandler;

	bool showAddCase = false;

	GuiAddStuffModel(const char* name_p) : ModelComponent(name_p) {}

	void init() override {
		caseHandler = this->gdSource->getCaseHandler();
	}

	void addCase(std::string caseName, std::string fileName) {
		if (caseName.empty())
		{
			ToastComponent::GetInstance()->addMessage("Case Name Empty!");
			return;
		}
		caseHandler->createCase(caseName, fileName);
		showAddCase = false;
	}

};

class GuiAddOverView : public ViewComponent {
public:
	GuiAddStuffModel* model;

	GuiAddOverView(const char* name_p, GuiAddStuffModel* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;

};


class GuiAddCaseView : public ViewComponent {
public:
	GuiAddStuffModel* model;

	std::string caseName = "";
	std::string caseFileName = "";

	GuiAddCaseView(const char* name_p, GuiAddStuffModel* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;


};