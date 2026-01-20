#include "ObjectTabComponent.h"

void ObjectTabComponent::view()
{
	ImGui::SetNextWindowSize(ImVec2(350,400));
	ImGui::SetNextWindowPos(ImVec2(0, 600));
	ImGui::Begin("Object");

	// Refresh when object is deleted and setup grid !!
	if (!ModelComponent::selectedObject->selObject.expired())
	{
		ImGui::Text("Object Name : ");
		ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getName().c_str());

		

		ImGui::Spacing();

		// Mesh
		ImGui::Text("Mesh : ");
		ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getMeshName().c_str());

		// Materials
		ImGui::InputText("Material Name",objectModel->materialName);

		if(ModelComponent::materialDpVec && !ModelComponent::materialDpVec->empty()) {
			if (ImGui::BeginCombo("Materials", ModelComponent::materialDpVec->at(objectModel->selectedMaterial).filename().string().c_str()))
			{
				for (int i = 0; i < ModelComponent::materialDpVec->size(); i++)
				{
					if (ImGui::Selectable(ModelComponent::materialDpVec->at(i).filename().string().c_str(),objectModel->selectedMaterial == i))
					{
						objectModel->selectedMaterial = i;
					}
				}

				ImGui::EndCombo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Set"))
		{
			objectModel->setMaterial();
		}

		ImGui::Text("Mesh Material : "); ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getMeshMaterialName().c_str());

		/*ImGui::Text("Current Material : "); ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getwMaterial().lock()->getName().c_str());*/

		ImGui::Text("Material : ");
		ImGui::SameLine();
		if (!ModelComponent::selectedMaterial->selMaterial.expired()) {

			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getName().c_str());
			
			ImGui::Text("Vertex Shader : ");
			ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getVertexShader()->getName().c_str());

			ImGui::Text("Fragment Shader : ");
			ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getFragmentShader()->getName().c_str());

			if (ImGui::Checkbox("WireFrame Mode", &objectModel->wireframeMode))
			{
				ModelComponent::selectedMaterial->selMaterial.lock()->setWireFrameMode(objectModel->wireframeMode);
			}

			ImGui::Text("Culling Mode");

			if (ImGui::RadioButton("Anti-Clockwise", ModelComponent::selectedMaterial->selMaterial.lock()->getCullingMode() == Ogre::CullingMode::CULL_ANTICLOCKWISE))
			{
				ModelComponent::selectedMaterial->selMaterial.lock()->setCullingMode(Ogre::CullingMode::CULL_ANTICLOCKWISE);
			}
			if (ImGui::RadioButton("Clockwise", ModelComponent::selectedMaterial->selMaterial.lock()->getCullingMode() == Ogre::CullingMode::CULL_CLOCKWISE))
			{
				ModelComponent::selectedMaterial->selMaterial.lock()->setCullingMode(Ogre::CullingMode::CULL_CLOCKWISE);
			}
			if (ImGui::RadioButton("None", ModelComponent::selectedMaterial->selMaterial.lock()->getCullingMode() == Ogre::CullingMode::CULL_NONE))
			{
				ModelComponent::selectedMaterial->selMaterial.lock()->setCullingMode(Ogre::CullingMode::CULL_NONE);
			}
			


		}
		else {
			ImGui::Text("No Material Selected!");
		}


	}
	else {
		ImGui::Text("No Object Selected!");
	}


	ImGui::End();


}
