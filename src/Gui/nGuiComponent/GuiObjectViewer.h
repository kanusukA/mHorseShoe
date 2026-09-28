#pragma once

#include <base/Mediator.h>

class ObjectViewerModel : public ModelComponent {


	ObjectViewerModel(const char* name_p) : ModelComponent(name_p) {}

	void init() override {

	}



};


class ObjectViewerView : public ViewComponent {

	ObjectViewerModel* model;


	ObjectViewerView(const char* name_p, ObjectViewerModel* model_p) : ViewComponent(name_p){
		model = model_p;
	}

	void view() override;

};