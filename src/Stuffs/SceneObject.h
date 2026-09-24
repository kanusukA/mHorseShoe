#pragma once

#include <Stuffs/EntityObject.h>

void SceneDeleter(Scene* scene_p);


// The Scene class connects SceneResource with Ogre::SceneManager. As such using SceneResource to initalize is not recommened as it may lead to complications.
class Scene : public SceneResource {
private:
	GDBuilderContext* GDBuilderCxt;

	std::vector<std::shared_ptr<Scene>> sceneVec{}; // linked
	std::vector<std::shared_ptr<Object>> objVec {};


public:

	Scene(GDBuilderContext* GDBuilderCxt_p, std::string name_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p) {
		GDBuilderCxt = GDBuilderCxt_p;

	}
	
	// Object
	void attachNewObject(std::shared_ptr<Object> obj_p) {
		objVec.push_back(obj_p);
	}

	void removeObjectByIndex(int index) {
		objVec.erase(objVec.begin() + index);
	}

	
	//Scene
	void attachNewScene(std::shared_ptr<Scene> scn_p) {
		sceneVec.push_back(scn_p);
	}

	void removeSceneByIndex(int index) {
		sceneVec.erase(sceneVec.begin() + index);
	}

	
	


	const std::vector<std::shared_ptr<Scene>>* getAttachedScenes() {
		return &sceneVec;
	}

	const std::vector<std::shared_ptr<Object>>* getObjects() {
		return &objVec;
	}

	
	~Scene() {
		destroyScene();
	}

	void destroyScene() {
		objVec.clear();
		sceneVec.clear();
	}


};


