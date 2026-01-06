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

		// fix position!!
	}

	// Object
	void addObject(Object* obj_p);

	void removeObjectById(ResID objId);

	void removeObjectByIndex(int index);

	
	//Scene
	void addScene(Scene* scene_p);

	void removeSceneById(ResID sceneId);

	void removeSceneByIndex(int index);

	Ogre::Vector3 getPosition() {
		return scene->getPosition();
	}

	Ogre::Quaternion getOrientation() {
		return scene->getOrientation();
	}

	Ogre::Vector3 getScale() {
		return scene->getScale();
	}

	void setPosition(Ogre::Vector3 pos_p) {
		this->position[0] = pos_p[0];
		this->position[1] = pos_p[1];
		this->position[2] = pos_p[2];
		scene->setPosition(pos_p);
	}

	void setOrientation(Ogre::Quaternion orientation_p) {
		this->orientation[0] = orientation_p[0];
		this->orientation[1] = orientation_p[1];
		this->orientation[2] = orientation_p[2];
		this->orientation[3] = orientation_p[3];
		scene->setOrientation(orientation_p);

	}

	void setScale(Ogre::Vector3 scale_p) {
		this->scale[0] = scale_p[0];
		this->scale[1] = scale_p[1];
		this->scale[2] = scale_p[2];
		scene->setScale(scale_p);
	}

	void updatePosition() override {
		scene->setPosition(Ogre::Vector3(this->position[0], this->position[1], this->position[2]));
	}
	void updateScale() override {
		scene->setScale(Ogre::Vector3(this->scale[0], this->scale[1], this->scale[2]));
	}
	void updateOrientation() override {
		Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Quaternion(this->orientation[0], this->orientation[1], this->orientation[2], this->orientation[3]));
		quat.normalise();
		this->orientation[0] = quat.w;
		this->orientation[1] = quat.x;
		this->orientation[2] = quat.y;
		this->orientation[3] = quat.z;
		scene->setOrientation(quat);
	}
	


	std::vector<Scene*>* getAttachedScenes() {
		return attachedScenes;
	}

	std::vector<Object*>* getObjects() {
		return attachedObject;
	}

	~Scene() {
		if(attachedScenes){
			for (int i = 0; i < attachedScenes->size(); i++)
			{
				delete attachedScenes->at(i);
			}
			delete attachedScenes;
		}
		if(attachedObject){
			for (int j = 0; j < attachedObject->size(); j++)
			{
				delete attachedObject->at(j);
			}

			delete attachedObject;
		}

		scene->destroyAllChildrenAndObjects();
		builderCxt->removeSceneNode(scene);
		//delete scene;
		
	}

};


