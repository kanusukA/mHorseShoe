#pragma once

#include <Stuffs/RenderMesh.h>


class Object : public ObjectResource {
public:
	GDBuilderContext* GDBuilderCxt;

	std::unique_ptr<Ogre::Entity> entity;

	std::shared_ptr<Material> sMaterial;

	Object(GDBuilderContext* GDBuilderCxt_p, Ogre::Entity* entity_p, std::string name_p, PhysXType objType_p) : ObjectResource(ResourceHandler::GetInstance(), name_p, objType_p) {
		GDBuilderCxt = GDBuilderCxt_p;
		entity.reset(entity_p);

	}

	// materialName must differ from materialPath_p. Else exception will be created!
	void setMaterial(std::filesystem::path materialPath_p, std::string materialName_p) {
		Material* newMat = GDBuilderCxt->CreateMaterial(materialPath_p, materialName_p);
		sMaterial.reset(newMat);
		
	}

	const std::weak_ptr<Material> const getwMaterial() {
		return sMaterial;
	}

	const std::string const getMeshName() {
		return entity->getMesh().get()->getName();
	}

	const std::string const getMeshMaterialName() {
		return entity->getMesh().get()->getSubMesh(0)->getMaterialName();
	}




};


