#include "CaseHandler.h"

void CaseHandler::saveAll() {

	
	for (int caseIndex = 0; caseIndex < caseVec->size(); caseIndex++)
	{
		if (caseVec->at(caseIndex))
		{
			// Saving case
			//resourceHandler->resetIni();
			resourceHandler->saveCase(caseVec->at(caseIndex).get());
			//resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::Cases));
			saveScenes(caseVec->at(caseIndex)->getScenes());
			// Saving Scenes
			//saveScenes(caseVec->at(caseIndex)->getScenes(), caseVec->at(caseIndex)->getId(), 0);
			

		}

	}

	resourceHandler->closeSaveFile();

}


void CaseHandler::saveScenes(const std::vector<std::shared_ptr<Scene>>* scenes)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << CASE_SCENES_KEY;
	out << YAML::Value;
	out << YAML::BeginSeq;
	for (int sceneIndex = 0; sceneIndex < scenes->size(); sceneIndex++)
	{

		resourceHandler->saveScene(scenes->at(sceneIndex).get(),out);


		if (scenes->at(sceneIndex)->getObjects())
		{
			out << YAML::Key << SCENE_OBJECTS_KEY;
			out << YAML::Value << YAML::BeginSeq;

			saveObjects(scenes->at(sceneIndex)->getObjects(), out);

			out << YAML::EndSeq;
		}

		out << YAML::EndMap; // used to end Scene Map

		/*resourceHandler->resetIni();
		resourceHandler->saveScene(scenes->at(sceneIndex).get(), caseID, parentNode);
		resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::Scenes));

		if (scenes->at(sceneIndex)->getAttachedScenes())
		{
			saveScenes(scenes->at(sceneIndex)->getAttachedScenes(), caseID, scenes->at(sceneIndex)->getId());
		}

		if (scenes->at(sceneIndex)->getObjects())
		{
			saveObjects(scenes->at(sceneIndex)->getObjects(), scenes->at(sceneIndex)->getId());
		}*/

	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	resourceHandler->writeToSaveFile(out.c_str());
}

void CaseHandler::saveObjects(const std::vector<std::shared_ptr<Object>>* objects, YAML::Emitter& out)
{
	// Save Object itself
	//resourceHandler->resetIni();
	//resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::Objects)); // Overwrites the file
	
	for (int objectIndex = 0; objectIndex < objects->size(); objectIndex++)
	{
		
		resourceHandler->saveObject(objects->at(objectIndex).get(),out);

		if (!objects->at(objectIndex)->getwMaterial().expired())
		{
			out << YAML::Key << OBJECT_MATERIAL_KEY;
			out << YAML::Value;
			resourceHandler->saveMaterial(objects->at(objectIndex)->getwMaterial().lock().get(), objects->at(objectIndex)->getwMaterial().lock()->getVertexShader().get(),
				objects->at(objectIndex)->getwMaterial().lock()->getFragmentShader().get(),out);
		}
		

		out << YAML::EndMap; // Objects map end

	}
	//resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::Objects));

	//// Save Material
	//resourceHandler->resetIni();
	//resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::MaterialPath));
	
	//resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::MaterialPath));

	// Saving Shaders
	/*resourceHandler->resetIni();
	resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::Shaders));
	for (int objectIndex = 0; objectIndex < objects->size(); objectIndex++)
	{
		if (!objects->at(objectIndex)->getwMaterial().expired())
		{	
			resourceHandler->saveShader(objects->at(objectIndex)->getwMaterial().lock()->getVertexShader().get(), ShaderType::Vertex, objects->at(objectIndex)->getwMaterial().lock()->getId());
			resourceHandler->saveShader(objects->at(objectIndex)->getwMaterial().lock()->getFragmentShader().get(), ShaderType::Fragment, objects->at(objectIndex)->getwMaterial().lock()->getId());

		}

	}
	resourceHandler->saveIni(*resourceHandler->getPath(ResourcePaths::Shaders));
	resourceHandler->resetIni();*/

	
	
}