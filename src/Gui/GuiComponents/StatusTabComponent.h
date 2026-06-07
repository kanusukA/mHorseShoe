#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>


class StatusTabModelComponent : public ModelComponent
{
private:

public:

	StatusTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}
	void init() override{
	}

	bool isPlayerObserverSet() {

	}

	Ogre::Vector3 getPlayerPosition() {
		
	}

	Ogre::Quaternion getPlayerOrientation() {
		
	}
	

};


class StatusTabComponent : public ViewComponent
{

private:
	StatusTabModelComponent* statusModel;


public:

	StatusTabComponent(const char* name_p,StatusTabModelComponent* statusModel_p) :
		ViewComponent(name_p) {
		statusModel = statusModel_p;
	}

	void view() override;

};

