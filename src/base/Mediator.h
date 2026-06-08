#pragma once

#ifndef MEDIATOR_H
#define MEDIATOR_H

//Local
#include<Gui/GuiConsts.h>
#include <GDHandler/Connector/MasterB.h>
#include <Gui/GuiComponents/ToastComponent.h>

// Fonts
#include <defaultFonts/Ubuntu_bold.h>
#include <defaultFonts/Ubuntu_Regular.h>

//Third


//STL
//#include <string>
#include <ShObjIdl.h>
#include <atlbase.h>


enum mFontType
{
	TTITLE48,
	TTITLE32,
	TTITLE16,
	BODY48,
	BODY32,
	BODY16
};

// Used to propogate fonts to view components
struct mFontSet {
	ImFont* title32Font = nullptr;
	ImFont* title48Font = nullptr;
	ImFont* title16Font = nullptr;
	ImFont* body48Font = nullptr;
	ImFont* body32Font = nullptr;
	ImFont* body16Font = nullptr;
};

// Update Events - implies which variable has changed

enum GUIUpdateEvent
{
	CASE_UPDATE,
	SCENE_UPDATE,
	OBJECT_UPDATE,
	MATERIAL_UPDATE,
	RESOURCE_UPDATE

};



// GuiFRAMEWORK IMPLEMENTATION
// HERE ARE THE CLASS USED TO CREATE VIEW AND MODEL COMPONENTS FOR THE GUI

class GuiFramework;
class GDSource;

// ViewComponent is used to create elements of UI, typically a window of ImGui
// name/id are used to access the derived class from the main Gui class
// use the view function to write renderable ImGui Functions
class ViewComponent {
private:
	ViewComponent();

	

protected:

	GuiFramework* guiFramework = nullptr;

	bool show = true;
	int id = 0;
	const char* name = "";

	mFontSet* fonts = nullptr;
	
public:

	//GUIFRAMEWORK IS ALWAYS NULL UNTIL SETFRAMEWORK()!
	// USE THAT FUNCTION IS FRAMEWORK IS REQUIRED
	ViewComponent(const char* name_p,mFontSet* fonts_p = nullptr ,GuiFramework* framework = nullptr) {
		name = name_p;
		guiFramework = framework;

		fonts = fonts_p;
		/*titleFont = *ImGui::GetFont();
		titleFont->FontSize = 28;*/
	}

	// FRAMEWORK METHOD! NOT TO BE USED OUTSIDE
	// USE THIS FUNCTION AS A INIT REPLACEMENT
	void setFramework(GuiFramework* framework, mFontSet* fonts_p = nullptr) {
		guiFramework = framework;
		fonts = fonts_p;
		//loadFonts();
	}

	virtual void view() {};

	

	// HELPING WIDGETS
	void ImVariableText(const char* text,mFontType type) {

		if (!fonts)
		{
			ImGui::Text(text);
			return;
		}
		switch (type) {
		case mFontType::TTITLE16:
			if (fonts->title16Font)
			{
				ImGui::PushFont(fonts->title16Font);
				ImGui::Text(text);
				ImGui::PopFont();
			}
			else {
				ImGui::Text(text);
			}
			break;
		case mFontType::TTITLE32:
			if (fonts->title32Font)
			{
				ImGui::PushFont(fonts->title32Font);
				ImGui::Text(text);
				ImGui::PopFont();
			}
			else {
				ImGui::Text(text);
			}
			break;
		case mFontType::TTITLE48:
			if (fonts->title48Font)
			{
				ImGui::PushFont(fonts->title48Font);
				ImGui::Text(text);
				ImGui::PopFont();
			}
			else {
				ImGui::Text(text);
			}
			break;
		default:
			ImGui::Text(text);
		}
		
		/*if(titleFont)
		{
			ImGui::PushFont(titleFont);
			ImGui::Text(text);
			ImGui::PopFont();
		}
		else {
			ImGui::Text(text);
		}*/
		
	}
};

// ModelComponent is the mediator layer between ViewComponent and GDSource
// It is a base class and stores data and functions relative to the view its attached with.
// A single ViewComponent may have multiple ModelComponents and ModelComponents can be shared among ViewComponents to provide
// data consistancy.

// SELECTED VARIABLES (AN IMPLEMENTATION OF DOUBLE POINTERS)

struct SelectedCase
{
	std::weak_ptr<Case> selCase;
};
struct SelectedScene
{
	std::weak_ptr<Scene> selScene;
};
struct SelectedObject
{
	std::weak_ptr<Object> selObject;
};
struct SelectedMaterial
{
	std::weak_ptr<Material> selMaterial;
};


// USED TO HANDLE FILE DIALOG 
struct ComInit
{
	ComInit() { CoInitialize(nullptr); }
	~ComInit() { CoUninitialize(); }
};


// GDSource - IT IS THE CONNECTOR CLASS FOR THE GUI FRAMEWORK.
// IT PROVIDES ACCESS TO OTHER PARTS OF THE SOFTWARE/
// THIS CLASS SHOULD NOT BE DIRCETLY CONNECTED WITH THE VIEW CLASS AS IT PROVIDES LIMITLESS ACCESS OF OTHER CLASSES
// TYPICALLY MODELCOMPONENT MUST BE USED IN BETWEEN TO FILTER AND MASK DATA.
class GDSource {

private:
	GDSource() {};

	
protected:
	CaseHandler* scnHandler = nullptr;
	ResourceHandler* resourceHandler = nullptr;
	RSUS* shaderHandler = nullptr;
	GDSystem* system = nullptr;

	// FILE DIALOG
	CComPtr<IFileDialog> fileDialog;

	//PlayerObserver* playerObserver = nullptr;

	


public:

	GDSource(CaseHandler* casehan,  
		ResourceHandler* resourceHan,
		RSUS* rsus,GDSystem* system_p) {
		this->scnHandler = casehan;
		this->resourceHandler = resourceHan;
		//this->feel = feelhan;
		this->shaderHandler = rsus;
		this->system = system_p;

		//loadFonts();

		// initalize the file dialog COM library
		ComInit com;
		
		fileDialog.CoCreateInstance(CLSID_FileOpenDialog);

		FILEOPENDIALOGOPTIONS opt{};
		
		fileDialog->GetOptions(&opt);
		fileDialog->SetOptions(opt | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM);

		// SETS DEFUALT FOLDER TO THE SOURCE DIR.
		CComPtr<IShellItem> psi;

		SHCreateItemFromParsingName(this->resourceHandler->SourceDir.wstring().c_str(), nullptr, IID_PPV_ARGS(&psi));
		fileDialog->SetFolder(psi);

	};

	//void loadFonts();

	CaseHandler* getCaseHandler() {
		return scnHandler;
	}

	ResourceHandler* getResourceHandler() {
		return resourceHandler;
	}


	RSUS* getShaderHandler() {
		return shaderHandler;
	}
	GDSystem* getGdSystem() {
		return system;
	}

	std::string openFolderSelectionDialog() {
		
		if (SUCCEEDED( fileDialog.p->Show(nullptr) ))
		{
			

			CComPtr<IShellItem> pSelectedItem;
			fileDialog->GetResult(&pSelectedItem);

			CComHeapPtr<wchar_t> pPath;
			pSelectedItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);
			
			size_t size = wcstombs(nullptr, pPath.m_pData, 0);
			std::vector<char> buffer(size + 1);
			// Perform conversion
			wcstombs(buffer.data(), pPath.m_pData, size);
			std::string str(buffer.data());
			

			std::wcout << L"Selected folder: " << pPath.m_pData << std::endl;

			return str;

		}

		return "";

	}
};


// MODEL_COMPONENT CLASS USES STATIC VARIABLES TO PROVIDE SINGLE AND TRUE VALUES TO DIFFERENT VIEW COMPONENTS THAT ARE ALMOST REQUIRED BY ALL
class ModelComponent {
private:
	ModelComponent();
protected:
	GuiFramework* guiFramework;
	GDSource* gdSource;

	const char* name;

public:

	bool buttonLock = false; // used to lock buttons when a process is running to prevent multiple clicks and process overlapping

	// INDEPENDENT RESOURCE DATA - These vectors are generated during runtime!
//	static	std::vector<std::shared_ptr<Case>>* caseVec;

	//CASE IS DIRECTLY TAKEN FROM THE CASEHANDLER.

	static SelectedScene* selectedScene;
	static SelectedObject* selectedObject;
	static SelectedMaterial* selectedMaterial;

	// DEPENDENT RESOURCE DATA - These vectors are fetched from the resourcehandler!

	static std::vector<std::filesystem::path>* meshDpVec;
	static std::vector<std::filesystem::path>* materialDpVec;
	static std::vector<std::filesystem::path>* shaderDpVec;

	static std::vector<std::filesystem::path>* textureDpVec;
	static std::vector<Ogre::TexturePtr>* imageTextures;

	static std::vector<std::filesystem::path>* savedCaseFiles;

	SunWindowSize* windowSize;

	ModelComponent(const char* name_p) {
		name = name_p;

		selectedScene = new SelectedScene();
		selectedObject = new SelectedObject();
		selectedMaterial = new SelectedMaterial();

	}

	void setFramework(GuiFramework* framework) {
		guiFramework = framework;
	}
	void setSource(GDSource* source) {
		gdSource = source;

		// SETUP DEPENDENT RESOURCE
		meshDpVec = gdSource->getResourceHandler()->getRenderMeshLoaded();
		materialDpVec = gdSource->getResourceHandler()->getMaterialsLoaded();
		shaderDpVec = gdSource->getResourceHandler()->getShadersLoaded();
		textureDpVec = gdSource->getResourceHandler()->getTexturesLoaded();
		
		refreshImageTextures();

		//caseVec = gdSource->getCaseHandler()->caseVec;

		savedCaseFiles = gdSource->getResourceHandler()->getSavedCaseFiles();

	}

	std::string openFolderSelection() {
		return gdSource->openFolderSelectionDialog();
	}

	// SETTERS
	// A CASE CAN NO LOGNER BE 'SELECTED' AND MUST BE LOADED TO ENSURE VALID SCENES.
	/*void selectCase(const int index) {
		gdSource->getCaseHandler()->setSelectedCase(index);
		update(GUIUpdateEvent::CASE_UPDATE);
	}*/

	void selectScene(const std::weak_ptr<Scene>& scene_p) {
		selectedScene->selScene = scene_p;
		update(GUIUpdateEvent::SCENE_UPDATE);
	}

	void selectObject(const std::weak_ptr<Object>& object_p) {
		selectedObject->selObject = object_p;
		if (!object_p.lock()->getwMaterial().expired())
		{
			this->selectMaterial(object_p.lock()->getwMaterial());
		}
		else {
			this->popMaterial();
		}
		update(GUIUpdateEvent::OBJECT_UPDATE);
	}

	void selectMaterial(const std::weak_ptr<Material>& material_p) {
		selectedMaterial->selMaterial = material_p;
		update(GUIUpdateEvent::MATERIAL_UPDATE);
	}

	void popMaterial() {
		selectedMaterial->selMaterial.reset();
	}

	void refreshImageTextures();

	const char* getName() {
		return name;
	}

	// USED TO INITALIZE PREDEFINED DATA AND VARIABLE OF THE COMPONENT
	// USE THIS INSTED OF CONSTRUCTOR AS GDSOURCE IS INITALIZED AFTER THE OBJECT IS CREATED
	virtual void init() {};

	// Run when a ResourceUpdates!
	virtual void update(GUIUpdateEvent event) {};

};





class GuiFramework : public GDSource {
protected:

	std::vector<ModelComponent*> Models = std::vector<ModelComponent*>();
	std::vector<ViewComponent*> Views = std::vector<ViewComponent*>();

	

	ModelComponent* getModelByName(std::string name) {
		for (int i = 0; i < Models.size(); i++)
		{
			if (Models[i]->getName() == name)
			{
				return Models[i];
			}
		}
		return nullptr;
	};
	ModelComponent* getModelById(int id) {

	};

public:

	GuiFramework(CaseHandler* casehan, ResourceHandler* resourcehan, RSUS* rsus, GDSystem* system_p) : 
		GDSource(casehan,resourcehan,rsus,system_p) {
		//resourcehan->setGuiRegen(this);
	}

	// GUI FUNCTIONS
	

	// CONVERTING OGRE::VECTORS TO FLOAT* 
	float* convertOgreVec2(Ogre::Vector2 vec2_p) {
		float float2[2] = {vec2_p[0],vec2_p[1]};
		return float2;
	}
	float* convertOgreVec3(Ogre::Vector3 vec3_p) {
		float float3[3] = { vec3_p[0],vec3_p[1],vec3_p[2]};
		return float3;
	}
	float* convertOgreVec4(Ogre::Vector4 vec4_p) {
		float float4[4] = { vec4_p[0],vec4_p[1],vec4_p[2],vec4_p[3]};
		return float4;
	}
	// SET OGRE::VECTOR VALUE TO FLOAT*
	void setOgreVec2(float* float2,Ogre::Vector2 vec2_p) {
		float2[0] = vec2_p[0];
		float2[1] = vec2_p[1];
	}
	void setOgreVec3(float* float3,Ogre::Vector3 vec3_p) {
		float3[0] = vec3_p[0];
		float3[1] = vec3_p[1];
		float3[2] = vec3_p[2];
	}
	void setOgreVec4(float* float4,Ogre::Vector4 vec4_p) {
		float4[0] = vec4_p[0];
		float4[1] = vec4_p[1];
		float4[2] = vec4_p[2];
		float4[3] = vec4_p[3];
	}

	// RUNS AT INIT, USED DURING THE CREATION OF VIEW COMPONENTS
	// MUST NOT BE RUN FROM OUTSIDE
	// VIEW COMPONENTS MUST BE ADDED TO BE RENDERED!
	virtual void addViewComponent(ViewComponent* viewComponent) {};

	// RUNS AT INIT, USED DURING THE CREATION OF MODEL COMPONENTS
	// MUST NOT BE RUN FROM OUTSIDE
	// MODEL COMPONENTS MUST BE ADDED TO BE RENDERED
	virtual void addModelComponent(ModelComponent* modelComponent) {};

	// Notifier functions
	void updateLoadCase() {
		ModelComponent* model = getModelByName(GD_SCENE_TAB_MODEL_COMP_NAME);
		if (model) {
		//	model->selectCase(0);
		}
		else {
			ToastComponent::GetInstance()->addMessage("Unable to fecth GUI_Model : " + std::string( GD_SCENE_TAB_MODEL_COMP_NAME));
		}

	}

		

};


// HELLPING WIDGETS




// DEPRICATED STUFF ----------------------------------------------------------------------------------------------------------------


// MAIN EVENT ENUM - CONTAINS ALL THE VIEW COMPONENETS
enum InitEvents
{
	SCENE_TAB_COMP

};

// COMPONENETS ENUMS - CONTAINS ENUM SPECIFIC FUNCTIONS
enum SceneTabComp
{
	REFRESH,
	SAVE_CASE,
	LOAD_CASE
};


// GUI -> GDHandler
// GDMediator Events
#define GDEVENT_ADD_OBJECT "gd_event_add_object"
#define GDEVENT_SHOW_PHYSX_VISUAL_DEBUG "show_physx_visual_debug"
#define GDEVENT_SHOW_RENDERING "show_ogre_render"
#define GDEVENT_DELETE_SELECTED_OBJ "delete_selected_obj"
#define GDEVENT_SHOW_PHYSX_RENDERING "show_physx_render"
#define GDEVENT_ADD_MDRL "add_main_directional_light"
#define GDEVENT_ADD_LIGHT "add_light_to_scene"
#define GDEVENT_SET_HEIGHTMAP "set_height_map"
#define GDEVENT_SEND_MAT_RSUS "send_material_to_rsus"
#define GDEVENT_GENERATE_TERRAIN "generate_terrain"
#define GDEVENT_GETSELECTEDOBJFRAGMENT "get_selected_obj_fragment_shader"

class MediatorComponent;

class Mediator {

public:
	virtual void Notify(MediatorComponent* component, std::string event) const = 0;
	virtual void NotifyView(InitEvents componentType, int compFuncEvent) const = 0;
};

class MediatorComponent {
protected:
	Mediator* _mediator;

public:

	MediatorComponent(Mediator* mediator = nullptr) : _mediator(mediator) {}

	void setMediator(Mediator* mediator) {
		this->_mediator = mediator;
	}

};

#endif // !MEDIATOR




