#include <Gui/nGuiComponent/GuiObjectViewer.h>


void shaderViewer(Shader& shader) {
	if (shader.fragShaderParameters && !shader.fragShaderParameters->empty())
	{
		for (size_t i = 0; i < shader.fragShaderParameters->size(); i++)
		{
			ShaderVar var = shader.fragShaderParameters->at(i);
			switch (var.varType) {
			case ShaderVarType::FLOAT0:
				ImGui::DragFloat(var.varName.c_str(), var.varFloat);
				break;
			case ShaderVarType::FLOAT2:
				ImGui::DragFloat2(var.varName.c_str(), var.varFloat2);
				break;
			case ShaderVarType::FLOAT3:
				ImGui::DragFloat3(var.varName.c_str(), var.varFloat3);
				break;
			case ShaderVarType::FLOAT4:
				ImGui::DragFloat4(var.varName.c_str(), var.varFloat4);
				break;
			default:
				ImGui::Text(var.varName.c_str());
			}
		}
	}
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
				shaderViewer(*model->selectedObject->mesh->shader);
			}

		}
		else {
			ImGui::Text("NO MESH!");
		}



		ImGui::End();
	}

}