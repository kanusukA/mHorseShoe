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

		ogreMaterials = this->gdSource->getResourceHandler()->OgreMaterials;
		rsusObj = this->gdSource->getShaderHandler()->rsusObj;
		images = this->gdSource->getResourceHandler()->images;

	}

	void sendMaterialRsus() {
		//this->gdSource->getShaderHandler()->readMaterial(ogreMaterials->at(selectedMaterial));
	}
	
	void getMaterials() {
		this->gdSource->getResourceHandler()->updateOgreMaterials();
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

	/*void updateFRsusInt(Ogre::String varName, int val) {
		this->gdSource->getShaderHandler()->updateFragParameterInt(varName,val);
	}
	void updateFragRsusFloat(Ogre::String varName, float* val) {
		this->gdSource->getShaderHandler()->updateFragParameterFloat(varName, val);
	}
	void updateFragRsusFloat2(Ogre::String varName, float val1, float val2) {
		this->gdSource->getShaderHandler()->updateFragParameterFloat2(varName, Ogre::Vector2(val1, val2));
	}
	void updateFragRsusFloat3(Ogre::String varName, float val1, float val2, float val3) {
		this->gdSource->getShaderHandler()->updateFragParameterFloat3(varName, Ogre::Vector3(val1, val2, val3));
	}
	void updateFragRsusFloat4(Ogre::String varName, float val1, float val2, float val3, float val4) {
		this->gdSource->getShaderHandler()->updateFragParameterFloat4(varName, Ogre::Vector4(val1, val2, val3, val4));
	}

	void updateVertRsusInt(Ogre::String varName, int val) {
		this->gdSource->getShaderHandler()->updateVertParameterInt(varName, val);
	}
	void updateVertRsusFloat(Ogre::String varName, float* val) {
		this->gdSource->getShaderHandler()->updateVertParameterFloat(varName, val);
	}
	void updateVertRsusFloat2(Ogre::String varName, float val1, float val2) {
		this->gdSource->getShaderHandler()->updateVertParameterFloat2(varName, Ogre::Vector2(val1, val2));
	}
	void updateVertRsusFloat3(Ogre::String varName, float val1, float val2, float val3) {
		this->gdSource->getShaderHandler()->updateVertParameterFloat3(varName, Ogre::Vector3(val1, val2, val3));
	}
	void updateVertRsusFloat4(Ogre::String varName, float val1, float val2, float val3, float val4) {
		this->gdSource->getShaderHandler()->updateVertParameterFloat4(varName, Ogre::Vector4(val1, val2, val3, val4));
	}*/

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

