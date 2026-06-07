#include "AddTabComponent.h"

void AddTabComponent::view()
{
	ImGui::Begin("Add");


	ImGui::InputText("Object Name", addModel->objectName);


	ImGui::End();

}
