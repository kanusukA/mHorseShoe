#include "EntityObject.h"




void ObjectDeleter(Object* object_p)
{
	//object_p->destroyObject();
	delete object_p;
}

