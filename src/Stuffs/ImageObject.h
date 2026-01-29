#pragma once

#include <GDHandler/GDContext.h>


class Image : public ImageResource {
public:
	Image(std::filesystem::path imgPath_p) : ImageResource(ResourceHandler::GetInstance(), imgPath_p) {

	}

};