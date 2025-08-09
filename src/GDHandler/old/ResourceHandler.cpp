#include "ResourceHandler.h"

void ResourceHandler::initResourceCheck()
{
	ResourceHandler::currentDirctory();

	this->renderMeshLocation = fs::absolute(RENDERMESH_LOCATION).string().c_str();
	this->colliderMeshLocation = fs::absolute(COLLIDERMESH_LOCATION).string().c_str();

	// Render Meshes
	for (const auto& entry : fs::directory_iterator(RENDERMESH_LOCATION)) {
		if(entry.path().extension().string() == ".mesh")
		{
			this->resources_mesh.emplace_back(entry.path().filename().string().c_str(), HorseResourceType::RENDER_MESH);
			std::cout << "obj added : " << entry.path().filename().string().c_str() << std::endl;
		}
	}
	for (const auto& entry : fs::directory_iterator(COLLIDERMESH_LOCATION)) {
		if (entry.path().extension().string() == ".mesh")
		{
			this->resources_mesh.emplace_back(entry.path().filename().string().c_str(), HorseResourceType::COLLIDER_MESH);
			std::cout << "obj added : " << entry.path().filename().string().c_str()  << std::endl;
		}
	}


	//currentDirctory();
	//std::cout << "\n[Horse]Reading Meshes To Load";

	//// FIX THIS SHIT
	//std::ifstream meshFile("C:/Users/Yato/source/repos/mHorseShoe0_0_1/x64/Release/meshs.cfg");

	//if (meshFile.is_open())
	//{
	//	std::cout << "\nFile opened";

	//	std::string line;
	//	std::string TypeName = "";
	//	std::string FileLocation;
	//	std::string FileName;
	//	while (std::getline(meshFile, line)) {

	//		if (line.empty()) {
	//			continue;
	//		}

	//		if (!TypeName.empty()) {
	//			FileLocation = line.substr(line.find('=') + 1);
	//			std::cout << "\nFileLocation : " << FileLocation;
	//			FileName = line.substr(line.find_last_of('/') + 1);
	//			std::cout << "\nFileName : " << FileName;
	//			HorseMeshResource* hmr = new HorseMeshResource();

	//			if (TypeName == "RenderMeshes") {
	//				hmr->rType = HorseResourceType::RENDER_MESH;
	//			}
	//			else if (TypeName == "ColliderMeshes")
	//			{
	//				hmr->rType = HorseResourceType::COLLIDER_MESH;
	//			}
	//			else {
	//				std::cout << "\nInValid Resource Type Set";
	//				continue;
	//			}
	//			hmr->Name = FileName;
	//			hmr->FileLoc = FileLocation;
	//			resources_mesh.push_back(hmr);
	//		}

	//		TypeName.clear();

	//		if (line[0] == '[') {
	//			TypeName = line.substr(1, line.length() - 2);
	//			std::cout << "\nType : " << TypeName;
	//		}
	//		
	//		
	//	}
	//}
	//else {
	//	std::cout << "\nCan not find meshFile";
	//}

}

void ResourceHandler::currentDirctory()
{
	std::cout << "Current Directory \n";
	std::string dir = "../";
	for (const auto & entry : fs::directory_iterator(dir))
	{
		std::cout << entry.path() << std::endl;
	}
}

std::vector<HorseMeshResource>& ResourceHandler::getMeshResources()
{
	return resources_mesh;
}
