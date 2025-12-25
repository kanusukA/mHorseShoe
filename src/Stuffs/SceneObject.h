#pragma once

#include <GDHandler/GDContext.h>

// The Scene class connects SceneResource with Ogre::SceneManager. As such using SceneResource to initalize is not recommened as it may lead to complications.
class Scene : public SceneResource {
private:
	GDBuilderContext* builderCxt;

public:
	Scene(GDBuilderContext* builderCxt_p, SceneType scnType) :
		SceneResource(ResourceHandler::GetInstance(), "", scnType, Ogre::Vector3(), Ogre::Vector4(), Ogre::Vector3()) {
		builderCxt = builderCxt_p;
	}
	Scene(GDBuilderContext* builderCxt_p,SceneType scnType, std::string name_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, Ogre::Vector3(), Ogre::Vector4(), Ogre::Vector3()) {
		builderCxt = builderCxt_p;
	}
	Scene(GDBuilderContext* builderCxt_p, SceneType scnType, std::string name_p, Ogre::Vector3 pos_p, Ogre::Vector4 orientation_p, Ogre::Vector3 scale_p) :
		SceneResource(ResourceHandler::GetInstance(), name_p, scnType, pos_p, orientation_p, scale_p) {
		builderCxt = builderCxt_p;
	}

	void build() {
		
	}


};


