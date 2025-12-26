#include "CaseHandler.h"

void CaseHandler::checkIntegrity()
{
	// Check Scenes in 
	Ogre::Node::ChildNodeMap nodes = oScnManager->getRootSceneNode()->getChildren();

	std::vector<SceneResource*>* scnNodes = ResourceHandler::GetInstance()->getAllScenes();
	bool match = true;
	if (nodes.size() == scnNodes->size())
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			match = false;
			for (int j = 0; j < scnNodes->size(); j++)
			{
				if (nodes.at(i)->getName() != scnNodes->at(j)->getName())
				{
					match = true;
				}
			}
			if (!match)
			{
				throw CaseErrorHandler("Ogre Nodes are inconsistent with Resource Nodes");
			}
		}
	}
	else {
		throw CaseErrorHandler("Ogre Nodes are inconsistent with Resource Nodes");
	}
	

}


Scene* CaseHandler::CreateScene(SceneType scnType, std::string scnName)
{
	Scene* new_scn = new Scene(this->builderCxt, scnType, scnName);
	return new_scn;
	//return new Scene(scnType, scnName);
}
