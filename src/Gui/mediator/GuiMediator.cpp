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

void GuiComponent::saveRsusObj()
{
	std::string filename = this->rsusParam->rsusObj->shaderName;
	std::cout << "saving " + filename  << std::endl;
	ResourceHandler::GetInstance()->clearFile(filename);
	ShaderVar shaVar = rsusParam->rsusObj->variables.at(0);
	
	for (int i = 0; i < rsusParam->rsusObj->variables.size(); i++)
	{
		ShaderVar shaVar = rsusParam->rsusObj->variables.at(i);
		switch (shaVar.varType)
		{
		case ShaderVarType::INTEGER :
			ResourceHandler::GetInstance()->writeToFile(shaVar.varName, std::to_string(shaVar.varType) + " " + 
				std::to_string(*shaVar.varInt),filename );
			break;
		case ShaderVarType::FLOAT0:
			ResourceHandler::GetInstance()->writeToFile(shaVar.varName, std::to_string(shaVar.varType) + " " + 
				std::to_string(*shaVar.varFloat), filename);
			break;
		case ShaderVarType::FLOAT2:
			ResourceHandler::GetInstance()->writeToFile(shaVar.varName, std::to_string(shaVar.varType) + " " + 
				std::to_string(shaVar.varFloat2[0]) + " " + 
				std::to_string(shaVar.varFloat2[1]), 
				filename);
			break;
		case ShaderVarType::FLOAT3:
			ResourceHandler::GetInstance()->writeToFile(shaVar.varName, std::to_string(shaVar.varType) + " " + 
				std::to_string(shaVar.varFloat3[0]) + " " + 
				std::to_string(shaVar.varFloat3[1]) + " " + 
				std::to_string(shaVar.varFloat3[2]), 
				filename);
			break;
		case ShaderVarType::FLOAT4:
			ResourceHandler::GetInstance()->writeToFile(shaVar.varName, std::to_string(shaVar.varType) + " " + 
				std::to_string(shaVar.varFloat4[0]) + " " + 
				std::to_string(shaVar.varFloat4[1]) + " " + 
				std::to_string(shaVar.varFloat4[2]) + " " + 
				std::to_string(shaVar.varFloat4[3]), 
				filename);
			break;
		default:
			std::cout << "Error filling value " << std::endl;
			break;
		}
		
	}

}
