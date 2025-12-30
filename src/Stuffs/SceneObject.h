#pragma once

#include <Stuffs/EntityObject.h>


// The Scene class connects SceneResource with Ogre::SceneManager. As such using SceneResource to initalize is not recommened as it may lead to complications.
class Scene : public SceneResource {
private:
	GDBuilderContext* builderCxt;

	Ogre::SceneNode* scene;

	std::vector<Scene*>* attachedScenes;
	std::vector<Object*>* attachedObject;

public:
	Scene(GDBuilderContext* builderCxt_p,SceneType scnType, std::string name_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, Ogre::Vector3(), Ogre::Vector4(), Ogre::Vector3()) {
		builderCxt = builderCxt_p;
		scene = builderCxt->createScene(name_p);
	}
	Scene(GDBuilderContext* builderCxt_p, SceneType scnType, std::string name_p, Ogre::Vector3 pos_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, pos_p, orientation_p, scale_p) {
		builderCxt = builderCxt_p;
		scene = builderCxt->createScene(name_p);
		scene->setPosition(pos_p);
		scene->setOrientation(Vec4toQuaternion(orientation_p));
		scene->setScale(scale_p);

	}

	// Object
	void addObject(Object* obj_p);

	void removeObjectById(ResID objId);

	void removeObjectByIndex(int index);

	
	//Scene
	void addScene(Scene* scene_p);

	void removeSceneById(ResID sceneId);

	void removeSceneByIndex(int index);



	void setPosition(Ogre::Vector3 pos_p) {
		this->position = pos_p;
		scene->setPosition(pos_p);
	}

	void setOrientation(Ogre::Quaternion orientation_p) {
		this->orientation = QuaternionToVec4(orientation_p);
		scene->setOrientation(orientation_p);

	}

	void setScale(Ogre::Vector3 scale_p) {
		this->scale = scale_p;
		scene->setScale(scale_p);
	}

	Ogre::Vector3 getPosition() {
		return scene->getPosition();
	}

	Ogre::Vector3 getScale() {
		return scene->getScale();
	}

	Ogre::Quaternion getOrientation() {
		return scene->getOrientation();
	}


	std::vector<Scene*>* getAttachedScenes() {
		return attachedScenes;
	}

	std::vector<Object*>* getObjects() {
		return attachedObject;
	}

	

};


