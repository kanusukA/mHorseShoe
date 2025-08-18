#include "Stuff.h"

// Multi-Thread shit
SceneHandler* SceneHandler::pinstance_{ nullptr };
std::mutex SceneHandler::mutex_;

SceneHandler* SceneHandler::GetInstance() {

	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new SceneHandler();

	}
	return pinstance_;

}


void loadScenes() {


}

std::string _nodeVectorToStr(Ogre::Vector3 pos) {
	return std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z);
}
std::string _nodeVectorToStr(Ogre::Vector4 pos) {
	return  std::to_string(pos.w) + "," + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z);
}

// pos - the current position of scnNodes being traversed
// scnNodes - output is stored
void SceneHandler::_travSceneNode(Ogre::SceneNode* node,int pos ,std::vector<Ogre::SceneNode*>* scnNodes, int scnType)
{
	SceneObject obj = SceneObject();

	Ogre::SceneNode::ObjectMap objs = node->getAttachedObjects();
	Ogre::SceneNode::ChildNodeMap nodes = node->getChildren();
	if (objs.size() > 0) {
		for (int i = 0; i < objs.size(); i++)
		{
			Ogre::Any any = node->getUserObjectBindings().getUserAny();
			if (scnType == 0) {
				StuffDynamic* dynObj = Ogre::any_cast<StuffDynamic*>(any);

				obj.ColliderMesh = dynObj->_getColliderMesh();
				obj.mass = dynObj->_getMass();

			}
			else if (scnType == 1) {
				StuffStatic* staObj = Ogre::any_cast<StuffStatic*>(any);

				obj.ColliderMesh = staObj->_getColliderMesh();
			}

			obj.name = objs.at(i)->getName();
			obj.material = oScnManager->getEntity(obj.name)->getMesh().get()->getSubMesh(0)->getMaterialName();
			obj.castShadow = objs.at(i)->getCastShadows()? "1" : "0";
			obj.receiveShadow = objs.at(i)->getReceivesShadows() ? "1" : "0";
			obj.RenderMesh = oScnManager->getEntity(obj.name)->getMesh().get()->getName();
			obj.position = _nodeVectorToStr(node->getPosition());
			obj.rotation = _nodeVectorToStr(Ogre::Vector4(node->getOrientation().w,
				node->getOrientation().x,
				node->getOrientation().y,
				node->getOrientation().z)
			);
			obj.PhysXType = std::to_string(scnType);

			ResourceHandler::GetInstance()->saveSceneObject(node->getName() + ".ini", obj, scnType);

		}
	}
	if (nodes.size() > 0)
	{
		for (int j = 0; j < nodes.size(); j++)
		{
			ResourceHandler::GetInstance()->saveScene(nodes.at(j)->getName(), node->getName() + ".ini",scnType);
			scnNodes->insert(scnNodes->begin() + pos + 1, oScnManager->getSceneNode(nodes.at(j)->getName()));
		}
	}
	if (scnNodes->size() > pos) {
		scnNodes->erase(scnNodes->begin() + pos);
	}


}

void SceneHandler::setSceneManager(Ogre::SceneManager* scnManager)
{

	oScnManager = scnManager;
	oScnManager->getRootSceneNode()->createChildSceneNode(STATIC_SCN_NODE);
	oScnManager->getRootSceneNode()->createChildSceneNode(DYNAMIC_SCN_NODE);
	oScnManager->getRootSceneNode()->createChildSceneNode(MESH_SCN_NODE);

}

void SceneHandler::saveScene(std::string scnName)
{
	std::vector<Ogre::SceneNode*>* remainingScns = new std::vector<Ogre::SceneNode*>();
	// DYNAMIC SCENES SAVE
	if (this->DynamicScenes.size() > 0) {
		for (int i = 0; i < DynamicScenes.size(); i++)
		{

			ResourceHandler::GetInstance()->saveScene(DynamicScenes.at(i)->getName(), DYNAMIC_NODES_FILE, 0);
			remainingScns->push_back(DynamicScenes.at(i));


			for (int j = 0; j < remainingScns->size(); j++)
			{
				_travSceneNode(remainingScns->at(j), j, remainingScns, 0);
			}
		}
	}
	remainingScns->clear();
	if (this->StaticScenes.size() > 0) {
		for (int i = 0; i < StaticScenes.size(); i++)
		{

			ResourceHandler::GetInstance()->saveScene(StaticScenes.at(i)->getName(), STATIC_NODES_FILE, 1);
			remainingScns->push_back(StaticScenes.at(i));

			for (int j = 0; j < remainingScns->size(); j++)
			{
				_travSceneNode(remainingScns->at(j), j, remainingScns, 1);
			}
		}
	}

	//Loading scenes

	remainingScns->clear();
	if (this->MeshScenes.size() > 0) {
		for (int i = 0; i < MeshScenes.size(); i++)
		{

			ResourceHandler::GetInstance()->saveScene(MeshScenes.at(i)->getName(), MESH_NODES_FILE, 2);
			remainingScns->push_back(MeshScenes.at(i));


			for (int j = 0; j < remainingScns->size(); j++)
			{
				_travSceneNode(remainingScns->at(j), j, remainingScns, 2);
			}
		}
	}


}

void SceneHandler::CreateScene(SceneType typ, std::string scnName)
{
	Ogre::SceneNode* scnNode;
	Ogre::SceneNode* objNode;
	switch (typ)
	{
	case STATIC:
		scnNode = oScnManager->getSceneNode(STATIC_SCN_NODE);
		objNode = scnNode->createChildSceneNode(scnName);
		this->StaticScenes.push_back(objNode);
		break;
	case DYNAMIC:
		scnNode = oScnManager->getSceneNode(DYNAMIC_SCN_NODE);
		objNode =  scnNode->createChildSceneNode(scnName);
		this->DynamicScenes.push_back(objNode);
		break;
	case MESH:
		scnNode = oScnManager->getSceneNode(MESH_SCN_NODE);
		objNode =  scnNode->createChildSceneNode(scnName);
		this->MeshScenes.push_back(objNode);
		break;
	default:
		std::cout << "Invalid SCENE TYPE" << std::endl;
		break;
	}
}

