#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class SceneTabComponent : public ViewComponent {

private:
	SceneTabComponent();
public:

	SceneTabComponent(const char* name_p) : ViewComponent(){
		this->name = name_p;
	};

	void view() override;


	// Buttons
	void refresh();
	void saveCase();
	void loadCase();

};

