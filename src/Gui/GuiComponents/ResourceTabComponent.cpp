#include "ResourceTabComponent.h"


void pathViewComponent(
	ResourceTabModelComponent* model
) {

	if (ImGui::CollapsingHeader("Paths")) {



		for (int i = 0; i < model->paths->size(); i++)
		{
			if (model->edit == i)
			{
				if (ImGui::InputText("", model->inputPath, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue)) {
					model->setPath();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set"))
				{
					model->setPath();
				}
			}
			else {
				ImGui::Text(model->paths->at(i).c_str(), 500.0);
				ImGui::SameLine();
				if (ImGui::Button(("Edit##" + std::to_string(i)).c_str()))
				{
					model->editPath(i);
				}
			}


		}
	}

}

void addLoadPath(ResourceTabModelComponent* model) {

	if (ImGui::CollapsingHeader("Load Paths")) {

		if (model->emptyLoadPaths)
		{
			for (int i = 0; i < model->emptyLoadPaths->size(); i++)
			{
				ImGui::Text("Path Name");
				ImGui::InputText(("##pathNames" + std::to_string(i)).c_str(), &model->emptyLoadPaths->at(i).pathGroupName);
				if (model->emptyLoadPaths->at(i).paths)
				{
					ImGui::Text("Paths");
					for (int j = 0; j < model->emptyLoadPaths->at(i).paths->size(); j++)
					{
						ImGui::InputText(("##loadPaths" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &model->emptyLoadPaths->at(i).paths->at(j));
					}
				}
				if (model->emptyLoadPaths->at(i).extensions)
				{
					ImGui::Text("Extensions");
					for (int j = 0; j < model->emptyLoadPaths->at(i).extensions->size(); j++)
					{
						ImGui::InputText(("##loadExtensions" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &model->emptyLoadPaths->at(i).extensions->at(j));
					}
				}

			}
		}

		if (ImGui::Button("Add Load Path"))
		{
			model->addLoadPath();
		}

		if (ImGui::Button("Save"))
		{
			model->saveLoadPaths();
		}

		/*for (int i = 0; i < model->loadPaths->size(); i++)
		{

			switch (ResourceLoaderEnums::ResourceLoadPaths(i))
			{
			case ResourceLoaderEnums::Objects:
				ImGui::Text("Objects : ");
				break;
			case ResourceLoaderEnums::Shaders:
				ImGui::Text("Shaders : ");
				break;
			case ResourceLoaderEnums::MaterialPath:
				ImGui::Text("Material : ");
				break;
			case ResourceLoaderEnums::MaterialTexture:
				ImGui::Text("MaterialTexture : ");
				break;
			case ResourceLoaderEnums::RenderMeshPath:
				ImGui::Text("RenderMeshPath : ");
				break;
			case ResourceLoaderEnums::ImagePath:
				ImGui::Text("Images : ");
				break;
			default:
				ImGui::Text(("Unknown : " + std::to_string(i)).c_str());
				break;
			}
			ImGui::SameLine();

			if (model->editLoadPathpos == i)
			{

				if (ImGui::InputText(("##" + std::to_string(i)).c_str(), model->inputPath, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue)) {
					model->setLoadPath();
				}

				ImGui::SameLine();

				if (ImGui::Button("Set"))
				{
					model->setLoadPath();
				}
			}
			else {

				ImGui::Text(model->loadPaths->at(i).c_str(), 500.0);

				ImGui::SameLine();

				if (ImGui::Button(("EditLoadPath##" + std::to_string(i)).c_str()))
				{
					model->editLoadPath(i);
				}

			}


		}*/
	}
}

void ResourceTabComponent::view()
{
	ImGui::Begin("Resources");

	pathViewComponent(resourceTabModel);

	addLoadPath(resourceTabModel);


	if (ImGui::CollapsingHeader("Materials"))
	{
		for (int i = 0; i < resourceTabModel->materials->size(); i++)
		{
			ImGui::Text(resourceTabModel->materials->at(i).filename().string().c_str());
		}
	}
	if (ImGui::CollapsingHeader("Render Meshs"))
	{
		for (int i = 0; i < resourceTabModel->renderMeshes->size(); i++)
		{
			ImGui::Text(resourceTabModel->renderMeshes->at(i).filename().string().c_str());
		}
	}
	if (ImGui::CollapsingHeader("Images"))
	{
		for (int i = 0; i < resourceTabModel->images->size(); i++)
		{
			ImGui::Text(resourceTabModel->images->at(i).filename().string().c_str());
		}
	}
	if (ImGui::CollapsingHeader("Shaders"))
	{
		for (int i = 0; i < resourceTabModel->shaders->size(); i++)
		{
			ImGui::Text(resourceTabModel->shaders->at(i).filename().string().c_str());
		}
	}


	ImGui::End();
}
