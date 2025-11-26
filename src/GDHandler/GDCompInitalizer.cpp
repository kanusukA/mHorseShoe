#include <GDHandler/GDHandler.h>

void GDMediator::initGuiComponents(MediatorComponent* component, InitEvents events) {
	GuiComponent* guiComp = static_cast<GuiComponent*>(component);

	guiComp->scnComponent.CasesNames = SceneHandler::GetInstance()->getCases();

}