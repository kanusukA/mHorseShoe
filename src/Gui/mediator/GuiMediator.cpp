#include "GuiMediator.h"


void GuiComponent::addObject()
{
	this->_mediator->Notify(this, GDEVENT_ADD_OBJECT);
}

void GuiComponent::showRenderMeshes()
{
	this->_mediator->Notify(this, GDEVENT_SHOW_RENDERING);
}

void GuiComponent::deleteSelectedObj()
{
	this->_mediator->Notify(this, GDEVENT_DELETE_SELECTED_OBJ);
}

void GuiComponent::showDebugLines()
{
	this->_mediator->Notify(this, GDEVENT_SHOW_PHYSX_VISUAL_DEBUG);
}

void GuiComponent::setHeightMap()
{
	this->_mediator->Notify(this, GDEVENT_SET_HEIGHTMAP);
}

void GuiComponent::showGuiTab(bool visibility, GuiTabs tab)
{
	switch (tab)
	{
	case GUI_STATUS_TAB:
		this->guiTabs->statusTab = visibility;
		break;
	case GUI_ADD_OBJECT_TAB:
		this->guiTabs->addObjectTab = visibility;
		break;
	case GUI_RESOURCES_TAB:
		this->guiTabs->resourcesTab = visibility;
		break;
	case GUI_LIGHT_TAB:
		this->guiTabs->lightTab = visibility;
		break;
	case GUI_ALL_TABS:
		this->guiTabs->allTabs = visibility;
	default:
		break;
	}
}



// Saved as Object name - contains vertex program, fragment program, textures
void GuiComponent::saveRsusObj()
{
	std::string filename = "";
	std::vector<SaveData>* savedata = new std::vector<SaveData>();
	// if saved file is for a object
	if (getSelectedObject()->selectingName->empty()) {
		filename = this->rsusParam->rsusObj->fragShaderFileName;
	}
	else {
		filename = getSelectedObject()->selectingName->c_str();
	}


	std::cout << "saving " + filename  << std::endl;
	ResourceHandler::GetInstance()->clearFile(filename);
	ShaderVar shaVar = rsusParam->rsusObj->fragVariables.at(0);

	if (rsusParam->rsusObj->fragVariables.empty()) {
		std::cout << "Nothing to save in fragment program!" << std::endl;
	}
	else {


		for (int i = 0; i < rsusParam->rsusObj->fragVariables.size(); i++)
		{
			shaVar = rsusParam->rsusObj->fragVariables.at(i);
			SaveData data = SaveData();
			data.section = SECTION_FRAGMNET_SHADER;
			switch (shaVar.varType)
			{
			case ShaderVarType::INTEGER:
				data.key = shaVar.varName;
				data.value = std::to_string(shaVar.varType) + " " + std::to_string(*shaVar.varInt);

				savedata->push_back(data);
				break;
			case ShaderVarType::FLOAT0:
				data.key = shaVar.varName;
				data.value = std::to_string(shaVar.varType) + " " + std::to_string(*shaVar.varFloat);
				
				savedata->push_back(data);
				break;
			case ShaderVarType::FLOAT2:
				data.key = shaVar.varName;
				data.value = std::to_string(shaVar.varType) + " " +
					std::to_string(shaVar.varFloat2[0]) + " " +
					std::to_string(shaVar.varFloat2[1]);
			
				savedata->push_back(data);
				break;
			case ShaderVarType::FLOAT3:
				data.key = shaVar.varName;
				data.value = std::to_string(shaVar.varType) + " " +
					std::to_string(shaVar.varFloat3[0]) + " " +
					std::to_string(shaVar.varFloat3[1]) + " " +
					std::to_string(shaVar.varFloat3[2]);
				
				savedata->push_back(data);
				break;
			case ShaderVarType::FLOAT4:
				data.key = shaVar.varName;
				data.value = std::to_string(shaVar.varType) + " " +
					std::to_string(shaVar.varFloat4[0]) + " " +
					std::to_string(shaVar.varFloat4[1]) + " " +
					std::to_string(shaVar.varFloat4[2]) + " " +
					std::to_string(shaVar.varFloat4[3]);

				savedata->push_back(data);
				break;
			default:
				std::cout << "Error filling value " << std::endl;
				break;

			}
			
		}

		ResourceHandler::GetInstance()->writeToFile(savedata, filename);

	}
	

	filename = this->rsusParam->rsusObj->vertShaderName;
	std::cout << "saving " + filename << std::endl;
	ResourceHandler::GetInstance()->clearFile(filename);
	
	if (rsusParam->rsusObj->vertVariables.empty()) {
		std::cout << "Nothing to save in vertex program!" << std::endl;
		return;
	}

	shaVar = rsusParam->rsusObj->vertVariables.at(0);
	savedata->clear();

	for (int i = 0; i < rsusParam->rsusObj->vertVariables.size(); i++)
	{
		shaVar = rsusParam->rsusObj->vertVariables.at(i);
		SaveData data = SaveData();
		data.section = SECTION_VERTEX_SHADER;
		switch (shaVar.varType)
		{
		case ShaderVarType::INTEGER:
			data.key = shaVar.varName;
			data.value = std::to_string(shaVar.varType) + " " + std::to_string(*shaVar.varInt);

			savedata->push_back(data);
			break;
		case ShaderVarType::FLOAT0:
			data.key = shaVar.varName;
			data.value = std::to_string(shaVar.varType) + " " + std::to_string(*shaVar.varFloat);

			savedata->push_back(data);
			break;
		case ShaderVarType::FLOAT2:
			data.key = shaVar.varName;
			data.value = std::to_string(shaVar.varType) + " " +
				std::to_string(shaVar.varFloat2[0]) + " " +
				std::to_string(shaVar.varFloat2[1]);

			savedata->push_back(data);
			break;
		case ShaderVarType::FLOAT3:
			data.key = shaVar.varName;
			data.value = std::to_string(shaVar.varType) + " " +
				std::to_string(shaVar.varFloat3[0]) + " " +
				std::to_string(shaVar.varFloat3[1]) + " " +
				std::to_string(shaVar.varFloat3[2]);

			savedata->push_back(data);
			break;
		case ShaderVarType::FLOAT4:
			data.key = shaVar.varName;
			data.value = std::to_string(shaVar.varType) + " " +
				std::to_string(shaVar.varFloat4[0]) + " " +
				std::to_string(shaVar.varFloat4[1]) + " " +
				std::to_string(shaVar.varFloat4[2]) + " " +
				std::to_string(shaVar.varFloat4[3]);

			savedata->push_back(data);
			break;
		default:
			std::cout << "Error filling value " << std::endl;
			break;

		}
	}

	ResourceHandler::GetInstance()->writeToFile(savedata, filename);
	savedata->clear();


	// saveing textures
	// only object specific to PBR shader textures
	if (this->rsusParam->Diffuse) {
		ResourceHandler::GetInstance()->writeToFile(DIFFUSE_TEX_NAME,
			ResourceHandler::GetInstance()->images->at(this->rsusParam->selectedTextureDiffuseImage).string(),
			SECTION_TEXTURE,
			filename
		);
	}

	if (this->rsusParam->Normal) {
		ResourceHandler::GetInstance()->writeToFile(NORMAL_TEX_NAME,
			ResourceHandler::GetInstance()->images->at(this->rsusParam->selectedTextureNormalImage).string(),
			SECTION_TEXTURE,
			filename
		);
	}

	if (this->rsusParam->Roughness) {
		ResourceHandler::GetInstance()->writeToFile(ROUGH_TEX_NAME,
			ResourceHandler::GetInstance()->images->at(this->rsusParam->selectedTextureRoughnessImage).string(),
			SECTION_TEXTURE,
			filename
		);
	}

	if (this->rsusParam->Parallax) {
		ResourceHandler::GetInstance()->writeToFile(PARALLAX_TEX_NAME,
			ResourceHandler::GetInstance()->images->at(this->rsusParam->selectedTextureParallaxImage).string(),
			SECTION_TEXTURE,
			filename
		);
	}


}

void GuiComponent::setTextures()
{

	if (rsusParam->Diffuse) {
		RSUS::GetInstance()->setDiffuseTexture(rsusParam->Diffuse.get());
	}
	if (rsusParam->Normal) {
		RSUS::GetInstance()->setNormalTexture(rsusParam->Normal.get());
	}
	if (rsusParam->Roughness) {
		RSUS::GetInstance()->setRoughnessTexture(rsusParam->Roughness.get());
	}
	if (rsusParam->Parallax) {
		RSUS::GetInstance()->setParallaxTexture(rsusParam->Parallax.get());
	}
}
