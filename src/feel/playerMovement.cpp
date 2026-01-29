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

void PlayerSubject::updatePosition(float deltatime)
{

	if (input->sprint) {
		PLAYERSPEED = 3;
	}
	else {
		PLAYERSPEED = 1;
	}
	
	Ogre::Radian Yaxis = getPlayerRotation().getYaw();

	double Zcos = (mathunDa.Cos(Yaxis) * PLAYERSPEED) * deltatime;
	double XSin = (mathunDa.Sin(Yaxis) * PLAYERSPEED) * deltatime;

	double YSin = (getCamera()->getRealDirection().y * PLAYERSPEED) * deltatime;

	// PICASSO !!!!!!!!!!!!
	//double Pitch = ((playerSub->getPlayerRotation().yAxis().x * mathunDa.Sin(Yaxis)) + (playerSub->getPlayerRotation().yAxis().z * mathunDa.Cos(Yaxis))) / PLAYERSPEED * deltaTime;


	if (input->forward) {
		setPlayerPosition(
			getPlayerPosition() + Ogre::Vector3(-XSin, YSin, -Zcos)
		);
	}

	else if (input->backward) {
		setPlayerPosition(
			getPlayerPosition() + Ogre::Vector3(XSin, -YSin, Zcos)
		);
	}

	if (input->right) {
		setPlayerPosition(
			getPlayerPosition() + Ogre::Vector3(Zcos, 0, -XSin)
		);
	}

	else if (input->left) {
		setPlayerPosition(
			getPlayerPosition() + Ogre::Vector3(-Zcos, 0, XSin)
		);
	}

}

void PlayerSubject::updateRotation(float deltaTime)
{;

	// If Mouse moved
	if (*input->MouseXrel != 0 || *input->MouseYrel != 0) {

		// collect values
		mouseX -= (*input->MouseXrel / MOUSESENS) * deltaTime;
		mouseY -= (*input->MouseYrel / MOUSESENS) * deltaTime;

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

		setPlayerRotation(horQ * verQ);

	}
}
