#pragma once

#include <Gui/nGuiComponent/GuiSlugViewer.h>

void GuiSlugViewerView::view()
{
	ImGui::Begin("Slug Viewer");
	
	if (!model->slugMesh->obj)
	{
		if (ImGui::Button("Load SlugMesh"))
		{
			model->loadSlugMesh();
		}
	}
	else {
		ImGui::Text("Slug Mesh Name : "); ImGui::SameLine();
		ImGui::Text(model->slugMesh->obj->getName().c_str());
	}

	ImGui::End();
}