#pragma once

#include <base/Mediator.h>

class ObjectViewerModel : public ModelComponent {

public:



	ObjectViewerModel(const char* name_p) : ModelComponent(name_p) {}

	void init() override {

	}



};


class ObjectViewerView : public ViewComponent {

	ObjectViewerModel* model;

public:


	ObjectViewerView(const char* name_p, ObjectViewerModel* model_p) : ViewComponent(name_p){
		model = model_p;
	}

	void view() override;

};