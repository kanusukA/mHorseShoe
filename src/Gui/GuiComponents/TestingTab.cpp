#include "TestingTab.h"



void TestingTabComponent::view()
{
	ImGui::Begin("TESTING");

	// MASTER LIST
	if (testModel->masterList)
	{
		for (int i = 0; i < testModel->masterList->size(); i++)
		{
			ImGui::Text((std::to_string(i) + " : " + std::to_string(testModel->masterList->at(i))).c_str());
		}
	}

	ImGui::Spacing();

	// CASE RESOURCE

	ImGui::Text("Cases : ");

	if (testModel->caseResources)
	{
		for (int i = 0; i < testModel->caseResources->size(); i++)
		{
			ImGui::Text(testModel->caseResources->at(i)->getName().c_str()); // Why this is coming in as null!! fix this
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->caseResources->at(i)->getId())).c_str());

			ImGui::SameLine();
			if (ImGui::Button(("Scenes##" + std::to_string(i)).c_str()))
			{
				testModel->getScenesInCase(testModel->caseResources->at(i)->getId());
			}
		}
	}

	if (testModel->scnInCaseResource)
	{
		if (testModel->scnInCaseResource->size() > 0)
		{
			if (ImGui::BeginCombo("Scenes In Case","Click to see Ids"))
			{
				for (int i = 0; i < testModel->scnInCaseResource->size(); i++)
				{
					ImGui::Text(std::to_string(testModel->scnInCaseResource->at(i)).c_str());
				}
				ImGui::EndCombo();
			}
		}
		else {
			ImGui::Text("Case has No Scenes Attached!");
		}
	}

	ImGui::Spacing();

	ImGui::InputText("Case Name",testModel->caseName);
	
	if (ImGui::Button("Add Case"))
	{
		testModel->addCase();
	}

	ImGui::Spacing();
	
	// SCENE RESOURCE

	ImGui::Text("Scenes : ");

	if (testModel->sceneResources)
	{
		for (int i = 0; i < testModel->sceneResources->size(); i++)
		{
			ImGui::Text(testModel->sceneResources->at(i)->getName().c_str()); // Why this is coming in as null!! fix this
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->sceneResources->at(i)->getId())).c_str());
			ImGui::SameLine();
			
			if (ImGui::SmallButton(("see##" + std::to_string(i)).c_str()))// Fix Id issue
			{
				testModel->getSceneDetails(testModel->sceneResources->at(i)->getId());
			}
		}
	}

	ImGui::Spacing();

	ImGui::InputText("Scene Name", &testModel->scnName);

	ImGui::InputFloat3("Position", testModel->scn_pos);

	ImGui::InputFloat4("Orientation", testModel->scn_orientation);

	ImGui::InputFloat3("Scale", testModel->scn_scale);

	ImGui::Spacing();

	ImGui::Text("Scene Type");

	if (ImGui::RadioButton("Static##1", testModel->scnType == SceneType::STATIC)) {
		testModel->scnType = SceneType::STATIC;
	}

	if (ImGui::RadioButton("Dynamic", testModel->scnType == SceneType::DYNAMIC)) {
		testModel->scnType = SceneType::DYNAMIC;
	}

	if (ImGui::RadioButton("Mesh", testModel->scnType == SceneType::MESH)) {
		testModel->scnType = SceneType::MESH;
	}

	if (ImGui::Button("Add Scene"))
	{
		testModel->addScene();
	}

	if (testModel->caseResources)
	{
		if (testModel->caseResources->size() > 0)
		{
			if (ImGui::BeginCombo("Put in Case",testModel->caseResources->at(testModel->selectedCaseInSceneTab)->getName().c_str()))
			{
				for (int i = 0; i < testModel->caseResources->size(); i++)
				{
					if (ImGui::Selectable(testModel->caseResources->at(i)->getName().c_str(),i == testModel->selectedCaseInSceneTab))
					{
						testModel->selectedCaseInSceneTab = i;
					}
				}
				ImGui::EndCombo();
			}
		}
	}

	if (ImGui::Button("Add Scene to Case"))
	{
		if (testModel->selectedScnID)
		{
			testModel->AddSceneToCase(testModel->caseResources->at(testModel->selectedCaseInSceneTab)->getId());
		}
		
		
	}

	// Shader
	ImGui::Spacing();

	ImGui::Text("Shader");

	if (testModel->shaderResources)
	{
		for (int i = 0; i < testModel->shaderResources->size(); i++)
		{
			ImGui::Text(testModel->shaderResources->at(i)->getName().c_str());
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->shaderResources->at(i)->getId())).c_str());
		}
	}

	ImGui::InputText("ShaderName", testModel->shaderName);

	ImGui::InputText("ShaderFileName", testModel->shaderFileName);

	ImGui::Text("Shader Type");

	if (ImGui::RadioButton("Vertex", testModel->shaderType == ShaderType::Vertex)) {
		testModel->shaderType = ShaderType::Vertex;
	}

	if (ImGui::RadioButton("Fragment", testModel->shaderType == ShaderType::Fragment)) {
		testModel->shaderType = ShaderType::Fragment;
	}
	// test parameters

	ImGui::Spacing();

	ImGui::InputText("VarName", &testModel->shadervar->varName);


	if (ImGui::Button("Add Shader"))
	{
		testModel->addShader();
	}



	// Objects
	ImGui::Spacing();

	ImGui::Text("Objects");

	if (testModel->objectResources)
	{
		for (int i = 0; i < testModel->objectResources->size(); i++)
		{
			ImGui::Text(testModel->objectResources->at(i)->getName().c_str());
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->objectResources->at(i)->getId())).c_str());
		}
	}

	ImGui::Spacing();

	ImGui::InputText("Object Name", testModel->objName);

	if (ImGui::RadioButton("Static", testModel->physXType == PhysXType::Static)) {
		testModel->physXType = PhysXType::Static;
	}

	if (ImGui::RadioButton("Dynamic##2", testModel->physXType == PhysXType::Dynamic)) {
		testModel->physXType = PhysXType::Dynamic;
	}

	if (ImGui::RadioButton("Kinematic", testModel->physXType == PhysXType::Kinematic)) {
		testModel->physXType = PhysXType::Kinematic;
	}

	ImGui::InputFloat("Mass", &testModel->mass);

	if (ImGui::Button("add Object"))
	{
		testModel->addObject();
	}

	ImGui::Spacing();

	ImGui::Text("Render");

	if (testModel->renderResource)
	{
		for (int i = 0; i < testModel->renderResource->size(); i++)
		{
			ImGui::Text(testModel->renderResource->at(i)->getName().c_str());
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->renderResource->at(i)->getId())).c_str());
		}
	}

	ImGui::InputText("Render name", testModel->renderName);

	if (ImGui::Button("Add render mesh"))
	{
		testModel->addRender();
	}

	ImGui::Spacing();

	ImGui::Text("Collider");

	if (testModel->colliderResource)
	{
		for (int i = 0; i < testModel->colliderResource->size(); i++)
		{
			ImGui::Text(testModel->colliderResource->at(i)->getName().c_str());
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->colliderResource->at(i)->getId())).c_str());
		}
	}

	ImGui::InputText("Collider name", testModel->colliderName);

	if (ImGui::Button("Add Collider mesh"))
	{
		testModel->addCollider();
	}


	ImGui::Spacing();

	ImGui::Text("Image");

	if (testModel->imageResource)
	{
		for (int i = 0; i < testModel->imageResource->size(); i++)
		{
			ImGui::Text(testModel->imageResource->at(i)->getName().c_str());
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->imageResource->at(i)->getId())).c_str());
		}
	}


	if (ImGui::Button("Add Image mesh"))
	{
		testModel->addImage();
	}


	ImGui::End();

}
