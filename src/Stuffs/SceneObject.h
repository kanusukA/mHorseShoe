#pragma once

#include <Stuffs/EntityObject.h>

void SceneDeleter(Scene* scene_p);


// The Scene class connects SceneResource with Ogre::SceneManager. As such using SceneResource to initalize is not recommened as it may lead to complications.
class Scene : public SceneResource {
private:
	GDBuilderContext* GDBuilderCxt;

	Ogre::SceneNode* scene;

	std::vector<std::shared_ptr<Scene>> sceneVec;
	std::vector<std::shared_ptr<Object>> objVec;


public:
	Scene(GDBuilderContext* GDBuilderCxt_p,SceneType scnType, std::string name_p,Ogre::SceneNode* sceneNode_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, Ogre::Vector3(0,0,0), Ogre::Vector4(0,0,0,0), Ogre::Vector3(0,0,0)) {
		GDBuilderCxt = GDBuilderCxt_p;
		scene = sceneNode_p;
		
	}
	Scene(GDBuilderContext* GDBuilderCxt_p, SceneType scnType, std::string name_p, Ogre::SceneNode* sceneNode_p , Ogre::Vector3 pos_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, pos_p, orientation_p, scale_p) {
		GDBuilderCxt = GDBuilderCxt_p;
		scene = sceneNode_p;
		scene->setPosition(pos_p);
		scene->setOrientation(Vec4toQuaternion(orientation_p));
		scene->setScale(scale_p);
	}

	// Object
	void attachNewObject(const std::string objectName_p, std::filesystem::path meshPath_p, PhysXType type) {
		Object* newObject = GDBuilderCxt->CreateObject(objectName_p, meshPath_p, type);
		std::shared_ptr<Object> sObject(newObject);
		scene->attachObject(sObject->entity.get());
		objVec.push_back(std::move(sObject));

	}

	void removeObjectByIndex(int index) {
		objVec.erase(objVec.begin() + index);
	}

	
	//Scene
	void attachNewScene(const std::string sceneName_p, const SceneType sceneType_p) {
		Scene* newScene = GDBuilderCxt->CreateScene(sceneName_p, sceneType_p,scene);
		std::shared_ptr<Scene> sScene(newScene, SceneDeleter);
		sceneVec.push_back(std::move(sScene));
	}

	void removeSceneByIndex(int index) {
		sceneVec.erase(sceneVec.begin() + index);
	}

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
	


	const std::vector<std::shared_ptr<Scene>>* getAttachedScenes() {
		return &sceneVec;
	}

	const std::vector <std::shared_ptr<Object>>* getObjects() {
		return &objVec;
	}

	Ogre::SceneNode* getSceneNode() {
		return scene;
	}

	// Used my Shared_ptr's custom deleter
	void destroyScene() {
		GDBuilderCxt->monDeleteSceneNode(scene);
	}


};


