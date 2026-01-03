#include "ObjectTabComponent.h"

void ObjectTabComponent::view()
{
	ImGui::SetNextWindowSize(ImVec2(300,600));
	
	ImGui::SetNextWindowPos(ImVec2(*objectModel->windowSize->width-300, 0));
	ImGui::Begin("Object");


	ImGui::End();


}
