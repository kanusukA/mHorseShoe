#pragma once

#include <Stuffs/RenderMesh.h>



class Object : public ObjectResource {

protected:

	
	

public:

	/*static void customDeleter(Ogre::Entity* entity_p) {
		if (entity_p) {
			GDBuilderCxt->monDeleteEntity(entity_p);
			delete entity_p;
		}
	}*/

	GDBuilderContext* GDBuilderCxt;

	//std::unique_ptr < Ogre::Entity, void(*)(Ogre::Entity*) > entity;
	std::shared_ptr<Ogre::Entity> entity;

	std::shared_ptr<Material> sMaterial;

	Object(GDBuilderContext* GDBuilderCxt_p, Ogre::Entity* entity_p, std::string name_p, PhysXType objType_p,std::filesystem::path meshPath_p) : 
		ObjectResource(ResourceHandler::GetInstance(), name_p, objType_p,meshPath_p) {
		GDBuilderCxt = GDBuilderCxt_p;

		this->renderMeshName = entity_p->getName();
		
		auto deleter = [this](Ogre::Entity* entity) {
			GDBuilderCxt->monDeleteEntity(entity);
			};

		entity.reset(entity_p, deleter);
		
	}

	// materialName must differ from materialPath_p. Else exception will be created!
	bool setMaterial(std::filesystem::path materialPath_p, std::string materialName_p) {
		Material* newMat = GDBuilderCxt->CreateMaterial(materialPath_p, materialName_p);
		if (newMat)
		{
			sMaterial.reset(newMat);
			entity->setMaterial(sMaterial->getMaterialPtr());

			return true;
		}
		return false;
		
		
	}

	const std::weak_ptr<Material> getwMaterial() {
		return sMaterial;
	}

	const std::string getMeshName() {
		return entity->getMesh().get()->getName();
	}

	const std::string getMeshMaterialName() {
		return entity->getMesh().get()->getSubMesh(0)->getMaterialName();
	}


	void destroyObject() {
		GDBuilderCxt->monDeleteEntity(entity.get());
	}

};


