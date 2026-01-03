#pragma once

#include <Gui/GuiComponents/ToastComponent.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <filesystem>


class ResourceReader {

private:

	std::ifstream inStream;

public:

	std::string cleanWord(std::string word);

	// MATERIAL FUNCS
	std::string readMaterialName(std::filesystem::path mat_path_p);


};