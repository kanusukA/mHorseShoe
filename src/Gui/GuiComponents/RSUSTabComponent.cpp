#include "RSUSTabComponent.h"

inline int ImageComboView(int index) {
	int selValue = -1;
	if(!ModelComponent::imageTextures->empty()){
		if (ImGui::BeginCombo(("Images##" + std::to_string(index)).c_str(), ModelComponent::imageTextures->at(0)->getName().c_str()))
		{
			for (int imageIndex = 0; imageIndex < ModelComponent::imageTextures->size(); imageIndex++)
			{
				if (ImGui::Selectable(ModelComponent::imageTextures->at(imageIndex)->getName().c_str(), false))
				{
					selValue = imageIndex;
				}
			}
			ImGui::EndCombo();
		}
		
	}
	return selValue;
}

inline void textureViewComponent(
	Ogre::String name,
	Ogre::TextureUnitState* textureState,
	Ogre::TexturePtr& texture,
	int* selectedIndex,
	RSUSTabModelComponent* model
) {
	Ogre::String texturename = name + "Tex";

	if (textureState) {

		ImGui::Text(name.c_str());
		if (model->images->size() > 0) {

			if (ImGui::BeginCombo(texturename.c_str(), model->images->at(*selectedIndex).filename().string().c_str())) {

				for (int i = 0; i < model->images->size(); i++)
				{
					// Load Image to Textures in Ogre in memory safe fashion
					Ogre::TexturePtr previewImg = Ogre::TextureManager::getSingleton().getByName(model->images->at(i).filename().string().c_str(), "Images");
					if (previewImg)
					{
						ImGui::Image((ImTextureID)previewImg.get()->getHandle(), ImVec2(25, 25));
						ImGui::SameLine();
					}


					if (ImGui::Selectable(model->images->at(i).filename().string().c_str(), i == *selectedIndex))
					{
						*selectedIndex = i;

						if (Ogre::TextureManager::getSingleton().load(model->images->at(i).filename().string().c_str(), "Images")) {

							texture = Ogre::TextureManager::getSingleton().getByName(model->images->at(i).filename().string().c_str(), "Images");
							std::cout << "Texture" << texture.get()->getName() << std::endl;
						}
						else {
							std::cout << "unable to load texture" << std::endl;
						}

						std::cout << "End" << std::endl;

					}

				}
				ImGui::EndCombo();
			}
			
		}
		else { // No Images to select
			ImGui::Text("No images found.");
		}
	}

}

inline void editableShaderVarViewComponent(ShaderVar* shaderVar, ShaderType type ,RSUSTabModelComponent* model) {

	switch (shaderVar->varType)
	{
	case INTEGER:
		if (ImGui::InputInt(shaderVar->varName.c_str(),shaderVar->varInt))
		{
			model->updateShaderVar(*shaderVar, type);
		}
		break;
	case FLOAT0:
		if (ImGui::SliderFloat(shaderVar->varName.c_str(), shaderVar->varFloat,-1.0f,1.0f))
		{
			model->updateShaderVar(*shaderVar, type);
		}
		break;
	case FLOAT2:
		if (ImGui::SliderFloat2(shaderVar->varName.c_str(), shaderVar->varFloat2,-1.0f,1.0f))
		{
			model->updateShaderVar(*shaderVar, type);
		}
		break;
	case FLOAT3:
		if (ImGui::SliderFloat3(shaderVar->varName.c_str(), shaderVar->varFloat3, -1.0f, 1.0f))
		{
			model->updateShaderVar(*shaderVar, type);
		}
		break;
	case FLOAT4:
		if (ImGui::SliderFloat4(shaderVar->varName.c_str(), shaderVar->varFloat4, -1.0f, 1.0f))
		{
			model->updateShaderVar(*shaderVar, type);
		}
		break;
	default:
		ImGui::Text("Invalid Type!");
		break;
	}
}


void RSUSTabComponent::view()
{

	ImGui::SetNextWindowSize(ImVec2(400, 650));
	ImGui::SetNextWindowPos(ImVec2(*RSUSModel->windowSize->width - 400, 0));

	ImGui::Begin("RSUS");
	
	if(!ModelComponent::selectedMaterial->selMaterial.expired()){

		ImGui::Text("Material : "); ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getName().c_str());

		ImGui::Spacing();

		if(ModelComponent::selectedMaterial->selMaterial.lock()->getVertexShader()){

			ImGui::Text("Vertex Shader : "); ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getVertexShader()->getShaderName().c_str());

			
			for (int i = 0; i < ModelComponent::selectedMaterial->selMaterial.lock()->getVertexShader()->getShaderVars()->size(); i++)
			{
				editableShaderVarViewComponent(&ModelComponent::selectedMaterial->selMaterial.lock()->getVertexShader()->getShaderVars()->at(i),ShaderType::Vertex, RSUSModel);
			}

		}
		else {
			ImGui::Text("No Vertex Shader Found!");
		}

		if (ModelComponent::selectedMaterial->selMaterial.lock()->getFragmentShader()) {

			ImGui::Text("Fragment Shader : "); ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getFragmentShader()->getName().c_str());

			
			
			for (int i = 0; i < ModelComponent::selectedMaterial->selMaterial.lock()->getFragmentShader()->getShaderVars()->size(); i++)
			{
				editableShaderVarViewComponent(&ModelComponent::selectedMaterial->selMaterial.lock()->getFragmentShader()->getShaderVars()->at(i), ShaderType::Fragment, RSUSModel);
			}
			


		}
		else {
			ImGui::Text("No Fragment Shader Found!");
		}

		

		ImGui::Text("Texture");
		if (ImGui::Button("Refresh Textures"))
		{
			RSUSModel->refreshTexture();
		}
		if (ModelComponent::selectedMaterial->selMaterial.lock()->textures && !ModelComponent::selectedMaterial->selMaterial.lock()->textures->empty())
		{
			for (int texIndex = 0; texIndex < ModelComponent::selectedMaterial->selMaterial.lock()->textures->size(); texIndex++)
			{
				if (ModelComponent::selectedMaterial->selMaterial.lock()->textures->at(texIndex).texture)
				{
					ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->textures->at(texIndex).textureName.c_str());
					ImGui::Image((ImTextureID)ModelComponent::selectedMaterial->selMaterial.lock()->textures->at(texIndex).texture->getHandle(), ImVec2(250, 250));

					int selectedImage = ImageComboView(texIndex);
					if (selectedImage >= 0) {
						ModelComponent::selectedMaterial->selMaterial.lock()->setTexture(ModelComponent::selectedMaterial->selMaterial.lock()->textures->at(texIndex).texturePosition,
							ModelComponent::imageTextures->at(selectedImage));
					}
				}

				

			}
			
		}
		else {
			ImGui::Text("No Textures found");
		}

	}
	else {
		ImGui::Text("No Material Selected!");
	}

	/*if (RSUSModel->ogreMaterials)
	{
		if(RSUSModel->ogreMaterials->size() > 0){
			if (ImGui::BeginCombo("Materials", RSUSModel->ogreMaterials->at(RSUSModel->selectedMaterial).c_str()))
			{
				for (int i = 0; i < RSUSModel->ogreMaterials->size(); i++)
				{
					if (ImGui::Selectable(RSUSModel->ogreMaterials->at(i).c_str(),RSUSModel->selectedMaterial == i))
					{
						RSUSModel->selectedMaterial = i;
					}
				}
				ImGui::EndCombo();
			}
			
		}

		if (ImGui::Button("Shade it"))
		{
			RSUSModel->sendMaterialRsus();
		}
	}
	else {
		ImGui::Text("No Material Found!");
	}

	if (ImGui::Button("Get Matrials"))
	{
		RSUSModel->getMaterials();
	}*/

	// SHADER SELECTED

	//if (RSUSModel->rsusObj)
	//{
	//	ImGui::Text("Fragment Shader Name : ");
	//	ImGui::SameLine();
	//	ImGui::Text(RSUSModel->rsusObj->fragShaderName.c_str());

	//	ImGui::Text("Fragment Shader File Name : ");
	//	ImGui::Text(RSUSModel->rsusObj->fragShaderFileName.c_str());


	//	for (int i = 0; i < RSUSModel->rsusObj->fragVariables.size(); i++)
	//	{
	//		editableFragShaderVarViewComponent(RSUSModel->rsusObj->fragVariables.at(i), RSUSModel);
	//	}

	//	ImGui::Spacing();

	//	ImGui::Text("Vertex Shader Name : ");
	//	ImGui::SameLine();
	//	ImGui::Text(RSUSModel->rsusObj->vertShaderName.c_str());

	//	ImGui::Text("Vertex Shader File Name : ");
	//	ImGui::SameLine();
	//	ImGui::Text(RSUSModel->rsusObj->vertShaderFileName.c_str());

	//	for (int i = 0; i < RSUSModel->rsusObj->vertVariables.size(); i++)
	//	{
	//		editableVertShaderVarViewComponent(RSUSModel->rsusObj->fragVariables.at(i), RSUSModel);
	//	}

	//	// TEXTURES

	//	ImGui::Spacing();
	//	ImGui::Text("Textures");
	//	ImGui::Spacing();

	//	textureViewComponent(
	//		"Diffuse",
	//		RSUSModel->rsusObj->textures->Diffuse,
	//		RSUSModel->Diffuse,
	//		&RSUSModel->selectedDiffuseIndex,
	//		RSUSModel
	//	);

	//	if (RSUSModel->Diffuse)
	//	{
	//		ImGui::Image((ImTextureID)RSUSModel->Diffuse.get()->getHandle(), ImVec2(250, 250));
	//	}

	//	ImGui::Spacing();

	//	textureViewComponent(
	//		"Normal",
	//		RSUSModel->rsusObj->textures->Normal,
	//		RSUSModel->Normal,
	//		&RSUSModel->selectedNormalIndex,
	//		RSUSModel
	//	);

	//	if (RSUSModel->Normal)
	//	{
	//		ImGui::Image((ImTextureID)RSUSModel->Normal.get()->getHandle(), ImVec2(250, 250));
	//	}

	//	ImGui::Spacing();

	//	textureViewComponent(
	//		"Roughness",
	//		RSUSModel->rsusObj->textures->Roughness,
	//		RSUSModel->Roughness,
	//		&RSUSModel->selectedRoughnessIndex,
	//		RSUSModel
	//	);

	//	if (RSUSModel->Roughness)
	//	{
	//		ImGui::Image((ImTextureID)RSUSModel->Roughness.get()->getHandle(), ImVec2(250, 250));
	//	}

	//	ImGui::Spacing();

	//	textureViewComponent(
	//		"Parallax",
	//		RSUSModel->rsusObj->textures->Parallax,
	//		RSUSModel->Parallax,
	//		&RSUSModel->selectedParallaxIndex,
	//		RSUSModel
	//	);

	//	if (RSUSModel->Parallax)
	//	{
	//		ImGui::Image((ImTextureID)RSUSModel->Parallax.get()->getHandle(), ImVec2(250, 250));
	//	}

	//	ImGui::Spacing();

	//	if (ImGui::Button("Set Texture"))
	//	{
	//		RSUSModel->setTexture();
	//	}

	//	ImGui::Spacing();

	//	if (ImGui::Button("Save Parameters"))
	//	{
	//		// TODO CREATE A BETTER RSUS SAVE AND LOAD SYSTEM!!
	//	}

	//	ImGui::Spacing();

	//	if (ImGui::Button("update shader"))
	//	{
	//		RSUSModel->updateShader();
	//	}
	//}


	ImGui::End();
}
