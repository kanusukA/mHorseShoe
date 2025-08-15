#include "Gui.h"

void Gui::_terrainTab() {

	ImGui::Begin("Terrain");


	

	if (guiComponent->terrainTab->selectedHeightMap < 0)
	{
		if (ImGui::Button("Set up height"))
		{
			guiComponent->terrainTab->selectedHeightMap = 0;

			if (!resourceHandler->images->empty()) {
				Ogre::TextureManager::getSingleton().load(resourceHandler->images->at(0).filename().string().c_str(), "Images");
				Ogre::Texture* imgTex = Ogre::TextureManager::getSingleton().getByName(resourceHandler->images->at(0).filename().string().c_str(), "Images").get();
				hdl = imgTex->getHandle();
			}
			
		}
	}
	else {
		if (resourceHandler->images->empty())
		{
			ImGui::Text("No Images loaded");
		}
		else {

			if (ImGui::BeginCombo("Height Maps", resourceHandler->images->at(guiComponent->terrainTab->selectedHeightMap).filename().string().c_str())) {

				for (int i = 0; i < resourceHandler->images->size(); i++)
				{

					if (ImGui::Selectable(resourceHandler->images->at(i).filename().string().c_str(), guiComponent->terrainTab->selectedHeightMap == i))
					{
						guiComponent->terrainTab->selectedHeightMap = i;

						Ogre::TextureManager::getSingleton().load(resourceHandler->images->at(i).filename().string().c_str(), "Images");
						Ogre::Texture* imgTex = Ogre::TextureManager::getSingleton().getByName(resourceHandler->images->at(i).filename().string().c_str(), "Images").get();
						hdl = imgTex->getHandle();
					}

				}

				ImGui::EndCombo();

			}

			ImGui::Image((ImTextureID)hdl, ImVec2(250, 250));


			ImGui::SliderFloat("Displacement", &guiComponent->terrainTab->displacement, 0.0f, 1.0f);
			ImGui::SliderFloat("Scale", &guiComponent->terrainTab->scale, 1.0f, 10.0f);



		}
		

	}
	
	if (resourceHandler->OgreMaterials) {

		if (ImGui::BeginCombo("Materials", resourceHandler->OgreMaterials->at(guiComponent->terrainTab->selectedMaterial).c_str())) {

			for (int i = 0; i < resourceHandler->OgreMaterials->size(); i++)
			{

				if (ImGui::Selectable(resourceHandler->OgreMaterials->at(i).c_str(), guiComponent->terrainTab->selectedMaterial == i))
				{
					guiComponent->terrainTab->selectedMaterial = i;
				}

			}
			ImGui::EndCombo();

		}
	}

	if (guiComponent->terrainTab->selectedHeightMap < 0) {
		ImGui::SliderInt("Terrain Size", &guiComponent->terrainTab->terrainSize, 1, 1000);
	}
	

	ImGui::SliderInt("Terrain blocks", &guiComponent->terrainTab->blocks,1,50);

	if (ImGui::Button("Generate Terrain")) {
		guiComponent->createTerrain();
	}


	ImGui::End();

}