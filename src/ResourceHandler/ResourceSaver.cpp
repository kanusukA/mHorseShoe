#include "ResourceSaver.h"

bool ResourceSaver::loadIniFile(std::string filename)
{
	ini->Reset();
	SI_Error rc = ini->LoadFile(filename.c_str());
	if (rc < 0)
	{
		return false;
	}

	return true;
	
}

void ResourceSaver::saveIni(std::string filename)
{
	SI_Error rc = ini->SaveFile(filename.c_str());
	if (rc < 0)
	{
		throw ResourceHandlerIDError("Error Saving File - SaveFile() filepath invalid");
	}
	
}

void ResourceSaver::resetIni()
{
	ini->Reset();
}

void ResourceSaver::saveIniFile(std::string filename, std::string section, std::string key, std::string value)
{
	if (!loadIniFile(filename)) {
		throw ResourceHandlerIDError("Error Saving File - File not found!");
	}

	ini->SetValue(section.c_str(), key.c_str(), value.c_str());

}

void ResourceSaver::saveMasterList(std::string instanceName, std::vector<ResID>* master_p, std::string path)
{
	loadIniFile(path);

	ini->SetValue("Instances", instanceName.c_str(), NULL);

	for (int i = 0; i < master_p->size(); i++)
	{
		ini->SetValue("ID", std::to_string(master_p->at(i)).c_str(), NULL);
	}

	ini->SaveFile(path.c_str());
	ini->Reset();

}

void ResourceSaver::saveCase(CaseResource* case_p)
{
	const char* sectionName = std::to_string(case_p->getId()).c_str();

	ini->SetValue(sectionName, case_p->getName().c_str(),NULL);

	for (int i = 0; i < case_p->getScenesInCase()->size(); i++)
	{
		ini->SetValue(sectionName, std::to_string(case_p->getScenesInCase()->at(i)).c_str(), NULL);
	}

}

void ResourceSaver::saveScene(SceneResource* scene_p)
{
	const char* sectionName = std::to_string(scene_p->getId()).c_str();

	ini->SetValue(sectionName, scene_p->getName().c_str(), NULL);

	ini->SetValue(sectionName, "position", convertVec3ToString(scene_p->getPosition()).c_str());
	ini->SetValue(sectionName, "scale", convertVec3ToString(scene_p->getScale()).c_str());
	ini->SetValue(sectionName, "rotation", convertVec4ToString(scene_p->getOrientation()).c_str());

	for (int i = 0; i < scene_p->getObjects()->size(); i++)
	{
		ini->SetValue(sectionName, "Scene", std::to_string(scene_p->getObjects()->at(i)).c_str());
	}

}

void ResourceSaver::saveScnObj(std::string sectionName, ResID objectID)
{
	ini->SetValue(sectionName.c_str(), std::to_string(objectID).c_str(),NULL);
}

void ResourceSaver::saveMaterial(MaterialResource* mat_p)
{
	const char* sectionName = std::to_string(mat_p->getId()).c_str();

	ini->SetValue(sectionName, mat_p->getName().c_str(), NULL);
	ini->SetValue(sectionName, "vertexShader", std::to_string(mat_p->getVertexShader()).c_str());
	ini->SetValue(sectionName, "fragmentShader", std::to_string(mat_p->getFragmentShader()).c_str());


}



// PUBLIC FUNCTIONS

void ResourceSaver::saveCases(std::vector<CaseResource*>* case_res, std::string caseInipath)
{
	ini->Reset();
	for (int i = 0; i < case_res->size(); i++)
	{
		
		this->saveCase(case_res->at(i));
		
	}
	this->saveIni(caseInipath);
}

void ResourceSaver::saveScenes(std::vector<SceneResource*>* scene_res, std::string scnInipath, std::string scnObjInipath)
{
	
	
	for (int i = 0; i < scene_res->size(); i++)
	{
		ini->Reset();
		this->saveScene(scene_res->at(i));
		ini->SaveFile(scnInipath.c_str());
		ini->Reset();
		for (int j = 0; j < scene_res->at(i)->getObjects()->size(); j++)
		{
			this->saveScnObj(std::to_string(scene_res->at(i)->getId()), scene_res->at(i)->getObjects()->at(j));
		}
		ini->SaveFile(scnObjInipath.c_str());
			
	}

}
