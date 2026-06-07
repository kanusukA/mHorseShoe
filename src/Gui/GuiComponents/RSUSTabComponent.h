#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class RSUSTabModelComponent: public ModelComponent
{

public:

	RSUShader* rsusObj;

//	Material* selectedMaterial;

	std::vector<std::filesystem::path>* images;


	Ogre::TexturePtr Diffuse;
	Ogre::TexturePtr Normal;
	Ogre::TexturePtr Roughness;
	Ogre::TexturePtr Parallax;


	int selectedDiffuseIndex = 0;
	int selectedNormalIndex = 0;
	int selectedRoughnessIndex = 0;
	int selectedParallaxIndex = 0;

	Ogre::StringVector* ogreMaterials;
	int selectedMaterialIndex = 0;


	RSUSTabModelComponent(const char* name_p) : ModelComponent(name_p) {

	}

	void init() override {


	}

	void sendMaterialRsus() {
		//this->gdSource->getShaderHandler()->readMaterial(ogreMaterials->at(selectedMaterial));
	}
	
	void getMaterials() {
		//this->gdSource->getResourceHandler()->updateOgreMaterials();
	}

	void setTexture() {
		if (Diffuse)
		{
			this->gdSource->getShaderHandler()->setDiffuseTexture(Diffuse.get());
		}
		if (Normal)
		{
			this->gdSource->getShaderHandler()->setNormalTexture(Normal.get());
		}
		if (Roughness)
		{
			this->gdSource->getShaderHandler()->setRoughnessTexture(Roughness.get());
		}
		if (Parallax)
		{
			this->gdSource->getShaderHandler()->setParallaxTexture(Parallax.get());
		}
	}

	void saveRsusObj() {
		// SET SAVING FUNCTION IN RSUS ONLY
	}

	void updateShader() {
		sendMaterialRsus();
	}

	void updateShaderVar(ShaderVar var, ShaderType shaderType) {
		this->gdSource->getShaderHandler()->updateShaderVar(var, shaderType);
	}

	void refreshTexture() {
		ModelComponent::selectedMaterial->selMaterial.lock()->readTextures();
	}

	

	const char* wrapModeEnumToname(Ogre::TextureUnitState::TextureAddressingMode mode) {
		switch (mode)
		{
		case Ogre::TextureUnitState::TAM_WRAP:
			return "Wrap";
			break;
		case Ogre::TextureUnitState::TAM_MIRROR:
			return "Mirror";
			break;
		case Ogre::TextureUnitState::TAM_CLAMP:
			return "Clamp";
			break;
		case Ogre::TextureUnitState::TAM_BORDER:
			return "Border";
			break;
		default:
			return "Select Wrap Mode";
			break;
		}
	}

};


class RSUSTabComponent : public ViewComponent
{
	RSUSTabModelComponent* RSUSModel;
public:

	RSUSTabComponent(const char* name_p,RSUSTabModelComponent* RSUSModel_p) : ViewComponent(name_p) {
		RSUSModel = RSUSModel_p;
	}

	void view() override;

};

