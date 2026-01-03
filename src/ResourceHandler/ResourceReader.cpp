#include "ResourceReader.h"

std::string ResourceReader::cleanWord(std::string word)
{
	std::string output = "";
	for (int i = 0; i < word.size(); i++)
	{
		if ((word.at(i) < 91 && word.at(i) > 64) || (word.at(i) < 123 && word.at(i) > 96))
		{
			output.push_back(word.at(i));
		}
	}
	return output;
}

std::string ResourceReader::readMaterialName(std::filesystem::path mat_path_p)
{
    inStream.open(mat_path_p);

	if (inStream.is_open())
	{

		std::string word;

		bool materialFound = false;

		while (inStream >> word) { // Not reading properly
			if (materialFound)
			{
				inStream.close();
				return cleanWord(word);
			}
			else {
				if (word == "material")
				{
					materialFound = true;
				}
			}
		}

	}
	else {
		ToastComponent::GetInstance()->addMessage("Failed to open inStream file");
	}

	inStream.close();

}
