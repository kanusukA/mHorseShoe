#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>


class StatusTabModelComponent : public ModelComponent
{
private:
	//PlayerObserver* playerObserver;

public:

	StatusTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}
	void init() override{
		//this->playerObserver = this->gdSource->getPlayerObserver();
	}

	bool isPlayerObserverSet() {
		/*if (playerObserver)
		{
			return true;
		}
		else {
			return false;
		}*/
	}

	Ogre::Vector3 getPlayerPosition() {
		//return playerObserver->getPlayerPosition();
	}

	Ogre::Quaternion getPlayerOrientation() {
		//return playerObserver->getPlayerRotation();
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

