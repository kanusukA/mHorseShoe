#include "RSUSTabComponent.h"

inline int ImageComboView(int index, const char* selectedImage) {
	int selValue = -1;
	if(!ModelComponent::imageTextures->empty()){
		if(ModelComponent::imageTextures->at(0) == nullptr){
			ImGui::Text("No images found.");
			return selValue;
		}
		else {
			if (ImGui::BeginCombo(("Images##" + std::to_string(index)).c_str(), selectedImage))
			{
				for (int imageIndex = 0; imageIndex < ModelComponent::imageTextures->size(); imageIndex++)
				{

					ImGui::Image((ImTextureID)ModelComponent::imageTextures->at(imageIndex)->getHandle(), ImVec2(25, 25));
					ImGui::SameLine();

					if (ImGui::Selectable(ModelComponent::imageTextures->at(imageIndex)->getName().c_str(), false))
					{
						selValue = imageIndex;
					}
				}
				ImGui::EndCombo();
			}
		}
		
	}
	return selValue;
}

inline int textureWrapModeComboView(int index, const char* currentMode) {
	int selValue = -1;
	if (ImGui::BeginCombo(("Wrap Mode##" + std::to_string(index)).c_str(), currentMode))
	{
		if (ImGui::Selectable("Wrap"))
		{
			selValue = 0;
		}
		if (ImGui::Selectable("Mirror"))
		{
			selValue = 1;
		}
		if (ImGui::Selectable("Clamp"))
		{
			selValue = 2;
		}
		if (ImGui::Selectable("Border"))
		{
			selValue = 3;
		}

		ImGui::EndCombo();
		
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
	ImGui::SetNextWindowPos(ImVec2(*model->windowSize->width - 400, 0));

	ImGui::Begin("RSUS");
	
	if(!model->selectedMaterial.expired()){

		ImGui::Text("Material : "); ImGui::SameLine();
		ImGui::Text(model->selectedMaterial.lock()->getName().c_str());

		ImGui::Spacing();

		if(model->selectedMaterial.lock()->getVertexShader()) {
			ImGui::Text("Vertex Shader : "); ImGui::SameLine();
			ImGui::Text(model->selectedMaterial.lock()->getVertexShader()->getShaderName().c_str());

			
			for (int i = 0; i < model->selectedMaterial.lock()->getVertexShader()->getShaderVars()->size(); i++)
			{
				editableShaderVarViewComponent(&model->selectedMaterial.lock()->getVertexShader()->getShaderVars()->at(i),ShaderType::Vertex, model);
			}

		}
		else {
			ImGui::Text("No Vertex Shader Found!");
		}

		if (model->selectedMaterial.lock()->getFragmentShader()) {

			ImGui::Text("Fragment Shader : "); ImGui::SameLine();
			ImGui::Text(model->selectedMaterial.lock()->getFragmentShader()->getName().c_str());

			
			
			for (int i = 0; i < model->selectedMaterial.lock()->getFragmentShader()->getShaderVars()->size(); i++)
			{
				editableShaderVarViewComponent(&model->selectedMaterial.lock()->getFragmentShader()->getShaderVars()->at(i), ShaderType::Fragment, model);
			}
			


		}
		else {
			ImGui::Text("No Fragment Shader Found!");
		}

		

		ImGui::Text("Texture");
		if (ImGui::Button("Refresh Textures"))
		{
			model->refreshTexture();
		}
		if (model->selectedMaterial.lock()->textures && !model->selectedMaterial.lock()->textures->empty())
		{
			for (int texIndex = 0; texIndex < model->selectedMaterial.lock()->textures->size(); texIndex++)
			{
				if (model->selectedMaterial.lock()->textures->at(texIndex).texture)
				{
					ImGui::Text(model->selectedMaterial.lock()->textures->at(texIndex).textureName.c_str());
					ImGui::Image((ImTextureID)model->selectedMaterial.lock()->textures->at(texIndex).texture->getHandle(), ImVec2(250, 250));

					int selectedImage = ImageComboView(texIndex, model->selectedMaterial.lock()->textures->at(texIndex).textureName.c_str());
					if (selectedImage >= 0) {
						model->selectedMaterial.lock()->setTexture(model->selectedMaterial.lock()->textures->at(texIndex).texturePosition,
							ModelComponent::imageTextures->at(selectedImage));
					}

					int selectedWrapMode = textureWrapModeComboView(texIndex,model->wrapModeEnumToname(model->selectedMaterial.lock()->textures->at(texIndex).addressingMode));
					if (selectedWrapMode > 0)
					{
						model->selectedMaterial.lock()->setTextureWrapMode(
							model->selectedMaterial.lock()->textures->at(texIndex).texturePosition, Ogre::TextureUnitState::TextureAddressingMode(selectedWrapMode)
						);
					}

			
				}
				else {

					

					ImGui::Text("No Texture Found!");
					int selectedImage = ImageComboView(texIndex, model->selectedMaterial.lock()->textures->at(texIndex).textureName.c_str());
					if (selectedImage >= 0) {
						model->selectedMaterial.lock()->setTexture(model->selectedMaterial.lock()->textures->at(texIndex).texturePosition,
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

	


	ImGui::End();
}
