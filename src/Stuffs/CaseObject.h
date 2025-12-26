#pragma once

#include <GDHandler/GDContext.h>


class Case : public CaseResource {

public:
	Case(GDBuilderContext* builder_p) : CaseResource(ResourceHandler::GetInstance(), "") {

	}
	Case(GDBuilderContext* builder_p, std::string name_p) : CaseResource(ResourceHandler::GetInstance(), name_p) {

	}

	int getSceneCount() {
		return this->getScenesIdInCase()->size();
	}

};
