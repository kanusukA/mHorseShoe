#include "Gui.h"


void Gui::_textureComponent(
	Ogre::String name,
	Ogre::TextureUnitState* textureState,
	Ogre::TexturePtr& texture,
	int* selectedIndex
) {
	Ogre::String texturename = name + "Tex";
	if (textureState) {
		ImGui::Text(name.c_str());
		if (resourceHandler->images->size() > 0) {
			if (ImGui::BeginCombo(texturename.c_str(),
				resourceHandler->images->at(*selectedIndex).filename().string().c_str())) {

				for (int i = 0; i < resourceHandler->images->size(); i++)
				{
					if (ImGui::Selectable(resourceHandler->images->at(i).filename().string().c_str(),
						i == *selectedIndex))
					{
						*selectedIndex = i;


						

						if (Ogre::TextureManager::getSingleton().load(resourceHandler->images->at(i).filename().string().c_str(), "Images")) {
							texture = Ogre::TextureManager::getSingleton().getByName(resourceHandler->images->at(i).filename().string().c_str(), "Images");
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

void Gui::_RSUSTab()
{
	ImGui::Begin("Shaders");

	// select material to send to RSUS
	if (resourceHandler->OgreMaterials)
	{
		if (ImGui::BeginCombo("Materials", resourceHandler->OgreMaterials->at(guiComponent->getRSUSParam()->selectedMaterial).c_str())) {
			for (int i = 0; i < resourceHandler->OgreMaterials->size(); i++)
			{

				if (ImGui::Selectable(resourceHandler->OgreMaterials->at(i).c_str(), guiComponent->getRSUSParam()->selectedMaterial == i))
				{
					guiComponent->getRSUSParam()->selectedMaterial = i;
				}

			}
			ImGui::EndCombo();
		}


		if (ImGui::Button("Shade it")) {
			guiComponent->sendMaterialRsus();
		}
	}
	else {
		ImGui::Text("No Materials Found");
	}

	if (ImGui::Button("Get Materials")) {
		resourceHandler->updateOgreMaterials();
	}



	if (guiComponent->getRSUSParam()->rsusObj) {

		ImGui::Text(" Fragment Shader Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->fragShaderName.c_str());

		ImGui::Text(" Fragment Shader File Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->fragShaderName.c_str());

		RSUShader* shade = guiComponent->getRSUSParam()->rsusObj;

		// Variables
		for (int i = 0; i < guiComponent->getRSUSParam()->rsusObj->fragVariables.size(); i++)
		{

			ShaderVarType type = shade->fragVariables.at(i).varType;

			switch (type)
			{
			case INTEGER:
				ImGui::InputInt(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varInt);
				break;
			case FLOAT0:
				if (ImGui::SliderFloat(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat, -1.0f, 1.0f)) {
					guiComponent->updateFragRsusFloat(shade->fragVariables.at(i).varName, shade->fragVariables.at(i).varFloat);
				}
				break;
			case FLOAT2:
				if (ImGui::SliderFloat2(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat2, -1.0f, 1.0f)) {
					guiComponent->updateFragRsusFloat2(shade->fragVariables.at(i).varName, shade->fragVariables.at(i).varFloat2);
				}
				break;
			case FLOAT3:
				if (ImGui::SliderFloat3(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat3, -1.0f, 1.0f)) {
					guiComponent->updateFragRsusFloat3(shade->fragVariables.at(i).varName, shade->fragVariables.at(i).varFloat3);
				}
				break;
			case FLOAT4:
				if (ImGui::SliderFloat4(shade->fragVariables.at(i).varName.c_str(), shade->fragVariables.at(i).varFloat4, -1.0f, 1.0f)) {
					Ogre::Vector4 slid = Ogre::Vector4(shade->fragVariables.at(i).varFloat4[0], shade->fragVariables.at(i).varFloat4[1], shade->fragVariables.at(i).varFloat4[2], shade->fragVariables.at(i).varFloat4[3]);
					guiComponent->updateFragRsusFloat4(shade->fragVariables.at(i).varName, slid);
				}
				break;
			default:
				ImGui::Text("Invalid Type : ");
				ImGui::SameLine();
				ImGui::Text(shade->fragVariables.at(i).varName.c_str());
				break;
			}

		}

		ImGui::Spacing();

		ImGui::Text(" Vertex Shader Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->vertShaderName.c_str());

		ImGui::Text(" Vertex Shader File Name : ");
		ImGui::SameLine();
		ImGui::Text(guiComponent->getRSUSParam()->rsusObj->vertShaderName.c_str());

		// Variables
		for (int i = 0; i < guiComponent->getRSUSParam()->rsusObj->vertVariables.size(); i++)
		{

			ShaderVarType type = shade->vertVariables.at(i).varType;

			switch (type)
			{
			case INTEGER:
				ImGui::InputInt(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varInt);
				break;
			case FLOAT0:
				if (ImGui::SliderFloat(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat, -1.0f, 10.0f)) {
					guiComponent->updateVertRsusFloat(shade->vertVariables.at(i).varName, shade->vertVariables.at(i).varFloat);
				}
				break;
			case FLOAT2:
				if (ImGui::SliderFloat2(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat2, -1.0f, 10.0f)) {
					guiComponent->updateVertRsusFloat2(shade->vertVariables.at(i).varName, shade->vertVariables.at(i).varFloat2);
				}
				break;
			case FLOAT3:
				if (ImGui::SliderFloat3(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat3, -1.0f, 1.0f)) {
					guiComponent->updateVertRsusFloat3(shade->vertVariables.at(i).varName, shade->vertVariables.at(i).varFloat3);
				}
				break;
			case FLOAT4:
				if (ImGui::SliderFloat4(shade->vertVariables.at(i).varName.c_str(), shade->vertVariables.at(i).varFloat4, -1.0f, 1.0f)) {
					Ogre::Vector4 slid = Ogre::Vector4(shade->vertVariables.at(i).varFloat4[0], shade->vertVariables.at(i).varFloat4[1], shade->vertVariables.at(i).varFloat4[2], shade->vertVariables.at(i).varFloat4[3]);
					guiComponent->updateVertRsusFloat4(shade->vertVariables.at(i).varName, slid);
				}
				break;
			default:
				ImGui::Text("Invalid Type : ");
				ImGui::SameLine();
				ImGui::Text(shade->vertVariables.at(i).varName.c_str());
				break;
			}


		}

		ImGui::Spacing();
		ImGui::Text("TEXTURES");
		ImGui::Spacing();
		//TEXTURES

		_textureComponent(
			"Diffuse",
			guiComponent->getRSUSParam()->rsusObj->textures->Diffuse,
			guiComponent->getRSUSParam()->Diffuse,
			&guiComponent->getRSUSParam()->selectedTextureDiffuseImage
		);

		if (guiComponent->getRSUSParam()->Diffuse) {
			ImGui::Image((ImTextureID)guiComponent->getRSUSParam()->Diffuse.get()->getHandle(), ImVec2(250, 250));
		}

		ImGui::Spacing();


		_textureComponent(
			"Normal",
			guiComponent->getRSUSParam()->rsusObj->textures->Normal,
			guiComponent->getRSUSParam()->Normal,
			&guiComponent->getRSUSParam()->selectedTextureNormalImage
		);


		if (guiComponent->getRSUSParam()->Normal) {
			ImGui::Image((ImTextureID)guiComponent->getRSUSParam()->Normal.get()->getHandle(), ImVec2(250, 250));
		}

		ImGui::Spacing();


		_textureComponent(
			"Roughness",
			guiComponent->getRSUSParam()->rsusObj->textures->Roughness,
			guiComponent->getRSUSParam()->Roughness,
			&guiComponent->getRSUSParam()->selectedTextureRoughnessImage
		);


		if (guiComponent->getRSUSParam()->Roughness) {
			ImGui::Image((ImTextureID)guiComponent->getRSUSParam()->Roughness.get()->getHandle(), ImVec2(250, 250));
		}

		ImGui::Spacing();


		_textureComponent(
			"Parallax",
			guiComponent->getRSUSParam()->rsusObj->textures->Parallax,
			guiComponent->getRSUSParam()->Parallax,
			&guiComponent->getRSUSParam()->selectedTextureParallaxImage
		);


		if (guiComponent->getRSUSParam()->Parallax) {
			ImGui::Image((ImTextureID)guiComponent->getRSUSParam()->Parallax.get()->getHandle(), ImVec2(250, 250));
		}

	

		if (ImGui::Button("Set Texture")) {
			guiComponent->setTextures();
		}

		ImGui::Spacing();
		if (ImGui::Button("Save")) {
			guiComponent->saveRsusObj();
		}

	}

	ImGui::End();

}

