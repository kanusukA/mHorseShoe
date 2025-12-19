#include "ToastTabComponent.h"

void ToastTabViewComponent::view()
{
	ImGui::Begin("Notifications");

	for (int i = 0; i < toastModel->notifications->size(); i++)
	{
		ImGui::Text(toastModel->notifications->at(i).c_str());
	}


	ImGui::End();

}
