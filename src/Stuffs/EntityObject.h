#pragma once

#include <Stuffs/RenderMesh.h>

// clean each object file with new object creation standard!
// use default destructors or change a newer implementation if modern C++ has something as such!


class Object : public ObjectResource {

protected:

	
public:

	GDBuilderContext* GDBuilderCxt;

	std::shared_ptr<RenderMesh> mesh;


	Object(GDBuilderContext* GDBuilderCxt_p, std::string name_p, std::shared_ptr<RenderMesh> renderMesh_p ) :
		ObjectResource(ResourceHandler::GetInstance(), name_p, renderMesh_p.get() ) {
		GDBuilderCxt = GDBuilderCxt_p;

		mesh = renderMesh_p;
		
		
	}


	

	// DELETOR
	~Object() {
		destroyObject();
	}

	void destroyObject() {
		
	}

};


