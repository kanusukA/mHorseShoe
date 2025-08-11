#include "playerMovement.h"

const int MOUSESENS = 200;

const float MOUSEROLLNUM = 0.9975;

float PLAYERSPEED = 1;

double mouseX = 0.0f;
double mouseY = 0.0f;

// Rotation
double xRotW = 1.0f;
double xRotY = 0.0f;

double yRotW = 1.0f;
double yRotX = 0.0f;

Ogre::Quaternion verQ;
Ogre::Quaternion horQ;

Ogre::Math mathunDa = Ogre::Math();

void updatePosition(float deltaTime , PlayerSubject* playerSub)
{
	IKEYS* inputKeys = InputHandler::GetInstance()->getInputKeys();

	if (inputKeys->SPRINT_KEY) {
		PLAYERSPEED = 3;
	}
	else {
		PLAYERSPEED = 1;
	}
	
	Ogre::Radian Yaxis = playerSub->getPlayerRotation().getYaw();

	double Zcos = (mathunDa.Cos(Yaxis) * PLAYERSPEED) * deltaTime;
	double XSin = (mathunDa.Sin(Yaxis) * PLAYERSPEED) * deltaTime;

	double YSin = (playerSub->getCamera()->getRealDirection().y * PLAYERSPEED) * deltaTime;

	// PICASSO !!!!!!!!!!!!
	//double Pitch = ((playerSub->getPlayerRotation().yAxis().x * mathunDa.Sin(Yaxis)) + (playerSub->getPlayerRotation().yAxis().z * mathunDa.Cos(Yaxis))) / PLAYERSPEED * deltaTime;


	if (inputKeys->S_UP_KEY) {
		playerSub->setPlayerPosition(
			playerSub->getPlayerPosition() + Ogre::Vector3(-XSin, YSin, -Zcos)
		);
	}

	else if (inputKeys->S_DOWN_KEY) {
		playerSub->setPlayerPosition(
			playerSub->getPlayerPosition() + Ogre::Vector3(XSin, -YSin, Zcos)
		);
	}

	if (inputKeys->S_LEFT_KEY) {
		playerSub->setPlayerPosition(
			playerSub->getPlayerPosition() + Ogre::Vector3(Zcos, 0, -XSin)
		);
	}

	else if (inputKeys->S_RIGHT_KEY) {
		playerSub->setPlayerPosition(
			playerSub->getPlayerPosition() + Ogre::Vector3(-Zcos, 0, XSin)
		);
	}

}

void updateRotation(float deltaTime, PlayerSubject* playerSub)
{
	MOUSEIN* mInput = InputHandler::GetInstance()->getMouseInput();

	// If Mouse moved
	if (mInput->MouseXrel != 0 || mInput->MouseYrel != 0) {

		// collect values
		mouseX -= (mInput->MouseXrel / MOUSESENS) * deltaTime;
		mouseY -= (mInput->MouseYrel / MOUSESENS) * deltaTime;

		//RollOver
		if (mouseX <= -1) {
			mouseX = MOUSEROLLNUM;
		}
		else if (mouseX >= 1) {
			mouseX = -MOUSEROLLNUM;
		}

		if (mouseY > 0.25 ) {
			mouseY = 0.25;
		}
		else if (mouseY < -0.25) {
			mouseY = -0.25;
		}


		

		if (mouseX > 0) {
			xRotW = 1 - ((mouseX) * 2);
		}
		else {
			xRotW = -(1 + ((mouseX) * 2));
		}

		if (mouseY > 0) {
			yRotW = 1 - ((mouseY) * 2);
		}
		else {
			yRotW = -(1 + ((mouseY) * 2));
		}

		xRotY = 1 - Ogre::Math::Abs(xRotW);
		yRotX = 1 - Ogre::Math::Abs(yRotW);

		//
		verQ = Ogre::Quaternion(yRotW, yRotX, 0, 0);
		//
		horQ = Ogre::Quaternion(xRotW, 0, xRotY, 0);

		verQ.normalise();
		horQ.normalise();

		playerSub->setPlayerRotation(horQ * verQ);

	}
}
