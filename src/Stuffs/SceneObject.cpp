#include "SceneObject.h"

void Scene::addObject(Object* obj_p)
{
	if (!this->attachedObject)
	{
		this->attachedObject = new std::vector<Object*>();
	}
	SceneResource::_addObject(obj_p->getId());
	this->attachedObject->push_back(obj_p);
	scene->attachObject(obj_p->getEntity());

}

void Scene::removeObjectById(ResID objId)
{
	SceneResource::_removeObjectById(objId);

	for (int i = 0; i < this->attachedObject->size(); i++)
	{
		Object* obj = attachedObject->at(i);
		if (obj)
		{
			attachedObject->erase(attachedObject->begin() + i);
			delete obj;
			break;
		}
	}
}

void Scene::removeObjectByIndex(int index)
{
	if (!this->attachedObject)
	{
		attachedObject = new std::vector<Object*>();
	}
	SceneResource::_removeObjectByIndex(index);
	Object* obj = attachedObject->at(index);
	this->attachedObject->erase(attachedObject->begin() + index);
	delete obj;

}

void Scene::addScene(Scene* scene_p)
{
	if (!this->attachedScenes)
	{
		attachedScenes = new std::vector<Scene*>();
	}
	SceneResource::_attachScene(scene_p->getId());
	this->attachedScenes->push_back(scene_p);

}

void Scene::removeSceneById(ResID sceneId)
{
	SceneResource::_removeObjectById(sceneId);
	for (int i = 0; i < this->attachedScenes->size(); i++)
	{
		if (attachedScenes->at(i))
		{
			attachedScenes->erase(attachedScenes->begin() + i);
			break;
		}
	}
}

void Scene::removeSceneByIndex(int index)
{

	SceneResource::_removeSceneByIndex(index);
	this->attachedScenes->erase(attachedScenes->begin() + index);

}





