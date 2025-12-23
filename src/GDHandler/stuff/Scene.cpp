#include "Stuff.h"




void loadScenes() {



}

std::string _nodeVectorToStr(Ogre::Vector3 pos) {
	return std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z);
}
std::string _nodeVectorToStr(Ogre::Vector4 pos) {
	return  std::to_string(pos.w) + "," + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z);
}
