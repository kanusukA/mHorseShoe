#include "SceneObject.h"




void SceneDeleter(Scene* scene_p)
{
	std::cout << "Deleting scene : " << scene_p->getName() << std::endl;
	scene_p->destroyScene();
	delete scene_p;
}
