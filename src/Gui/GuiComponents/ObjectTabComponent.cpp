#include "ObjectTabComponent.h"

void ObjectTabComponent::view()
{
	ImGui::SetNextWindowSize(ImVec2(350,400));
	ImGui::SetNextWindowPos(ImVec2(0, 600));
	ImGui::Begin("Object");

	// Refresh when object is deleted and setup grid !!
	if (!model->object.expired())
	{
		ImGui::Text("Object Name : ");
		ImGui::SameLine();
		ImGui::Text(model->object.lock()->getName().c_str());

		ImGui::Spacing();

		// Mesh
		ImGui::Text("Mesh : ");
		ImGui::SameLine();
		ImGui::Text(model->object.lock()->getMeshName().c_str());
			
		// Materials
		ImGui::InputText("Material Name",model->materialName);
		if(ModelComponent::materialDpVec && !ModelComponent::materialDpVec->empty()) {
			if (ImGui::BeginCombo("Materials", ModelComponent::materialDpVec->at(model->selectedMaterial).filename().string().c_str()))
			{
				for (int i = 0; i < ModelComponent::materialDpVec->size(); i++)
				{
					if (ImGui::Selectable(ModelComponent::materialDpVec->at(i).filename().string().c_str(),model->selectedMaterial == i))
					{
						model->selectedMaterial = i;
					}
				}

				ImGui::EndCombo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Set"))
		{
			model->setMaterial();
		}

		ImGui::Text("Mesh Material : "); ImGui::SameLine();
		ImGui::Text(model->object.lock()->getMeshMaterialName().c_str());
		ImGui::Text("Material : ");
		ImGui::SameLine();
		if (!model->material.expired()) {

			ImGui::Text(model->material.lock()->getName().c_str());
			
			ImGui::Text("Vertex Shader : ");
			ImGui::SameLine();
			ImGui::Text(model->material.lock()->getVertexShader()->getName().c_str());
			ImGui::Text("Fragment Shader : ");
			ImGui::SameLine();
			ImGui::Text(model->material.lock()->getFragmentShader()->getName().c_str());

			if (ImGui::Checkbox("WireFrame Mode", &model->material.lock()->getWireFrameMode()))
			{
				model->material.lock()->setWireFrameMode(!model->material.lock()->getWireFrameMode());
			}

			ImGui::Text("Culling Mode");

			if (ImGui::RadioButton("Anti-Clockwise", model->material.lock()->getCullingMode() == Ogre::CullingMode::CULL_ANTICLOCKWISE))
			{
				model->material.lock()->setCullingMode(Ogre::CullingMode::CULL_ANTICLOCKWISE);
			}
			if (ImGui::RadioButton("Clockwise", model->material.lock()->getCullingMode() == Ogre::CullingMode::CULL_CLOCKWISE))
			{
				model->material.lock()->setCullingMode(Ogre::CullingMode::CULL_CLOCKWISE);
			}
			if (ImGui::RadioButton("None", model->material.lock()->getCullingMode() == Ogre::CullingMode::CULL_NONE))
			{
				model->material.lock()->setCullingMode(Ogre::CullingMode::CULL_NONE);
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
