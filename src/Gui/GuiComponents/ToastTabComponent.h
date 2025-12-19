#pragma once
#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>


class ToastTabModelComponent : public ModelComponent
{
	

public:

	std::vector<std::string>* notifications = new std::vector<std::string>();

	ToastTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {
		notifications = ToastComponent::GetInstance()->getMessages();
	}

};

class ToastTabViewComponent : public ViewComponent {

private:
	ToastTabModelComponent* toastModel;

public:

	ToastTabViewComponent(const char* name_p, ToastTabModelComponent* model_p) : ViewComponent(name_p) {
		toastModel = model_p;
	}

	void view() override;


};

