#pragma once

#include <GDHandler/GDContext.h>


class Case : public CaseResource , public GDBuilderImpli{

public:
	Case(GDBuilderContext* builder_p) : CaseResource(ResourceHandler::GetInstance(), "") , GDBuilderImpli(builder_p){

	}
	Case(GDBuilderContext* builder_p, std::string name_p) : CaseResource(ResourceHandler::GetInstance(), name_p), GDBuilderImpli(builder_p) {

	}

	int getSceneCount() {
		return this->getScenesIdInCase()->size();
	}

};
