#include <Gui/nGuiComponent/GuiObjectViewer.h>


bool shaderViewer(std::vector<ShaderVar>* parameters) {
	bool update = false;
	if (parameters && !parameters->empty())
	{
		for (size_t i = 0; i < parameters->size(); i++)
		{
			ShaderVar& var = parameters->at(i);
			switch (var.varType) {
			case ShaderVarType::FLOAT0:
				if (ImGui::DragFloat(var.varName.c_str(), var.varFloat, 0.0001f, -1.0f, 1.0f))
				{
					update = true;
				}
				break;
			case ShaderVarType::FLOAT2:
				if (ImGui::DragFloat2(var.varName.c_str(), glm::value_ptr(var.varFloat2), 0.0001f, -1.0f, 1.0f))
				{
					update = true;
				}
				break;
			case ShaderVarType::FLOAT3:
				if (ImGui::DragFloat3(var.varName.c_str(), glm::value_ptr(var.varFloat3), 0.0001f, -1.0f, 1.0f))
				{
					update = true;
				}
				break;
			case ShaderVarType::FLOAT4:
				if (ImGui::DragFloat4(var.varName.c_str(), glm::value_ptr(var.varFloat4), 0.0001f, -1.0f, 1.0f))
				{
					update = true;
				}
				break;
			default:
				ImGui::Text(var.varName.c_str());
			}
		}
	}
	return update;
}


void ObjectViewerView::view() {

	if (model->selectedObject)
	{
		ImGui::Begin("Object Viewer");

		ImGui::Text("Name : "); ImGui::SameLine();
		ImGui::Text(model->selectedObject->getName().c_str());

		ImGui::Text("Render Mesh : "); ImGui::SameLine();
		if (model->selectedObject->mesh)
		{
			ImGui::Text(model->selectedObject->mesh->getName().c_str());

			ImGui::Text("Shader : "); ImGui::SameLine();
			if (model->selectedObject->mesh->shader)
			{
				ImGui::Text(model->selectedObject->mesh->shader->getName().c_str());
				if (shaderViewer(&model->selectedObject->mesh->fragParameters))
				{
					model->selectedObject->mesh->updateBuffer(model->selectedObject->mesh->fragParameters);
				}
			}

		}
		else {
			ImGui::Text("NO MESH!");
		}



		ImGui::End();
	}

}