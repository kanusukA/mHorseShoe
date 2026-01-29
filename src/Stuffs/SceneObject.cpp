#include "SceneObject.h"




void SceneDeleter(Scene* scene_p)
{
	scene_p->destroyScene();
	delete scene_p;
}
