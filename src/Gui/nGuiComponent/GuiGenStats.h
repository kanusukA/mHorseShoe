#pragma once

#include <base/Mediator.h>

class GuiGenStatsModelComponent : public ModelComponent
{
public:
	ImguiDebugStats* debugStats;
	GuiGenStatsModelComponent(const char* name_p) : ModelComponent(name_p) {};

	void init() override {
		debugStats = this->gdSource->_debugStats;
	}
};


class GuiGenStatsView : public ViewComponent
{
	GuiGenStatsModelComponent* model;

	GuiGenStatsView();
public:
	GuiGenStatsView(const char* name_p, GuiGenStatsModelComponent* model_p) : ViewComponent(name_p) {
		model = model_p;
	}

	void view() override;
};