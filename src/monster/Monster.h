#pragma once

#ifndef MONSTER_H
#define MONSTER_H

#ifndef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#define VK_EXT_DEBUG_REPORT_EXTENSION_NAME "VK_EXT_debug_report"
#endif

//  IMPORT THIS BEFORE OGRE.H !!!!!!!!!!!
// #include <OgreApplicationContext.h>
//#include <OgreTrays.h>

// Local Headers
//#include <monster/terrain.h>
//#include <feel/KeyHandler.h>
//#include <GDHandler/ResourceHandler.h>

#include "VulkanStats.h"

// Third-party header
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
//#include <OgreImGuiOverlay.h>
#include <imgui_stdlib.h>


//#include <imgui_spectrum.h>
//#include <PxPhysicsAPI.h>

// STL headers
#if defined (_WIN32)
	#include <Windows.h>
#endif

#include <random>
#include <cons.h>

#include <map>


//const Ogre::String TRAY_SYSTEM_NAME = "Default_Tray_Man";

#define MAIN_CAMERA_NAME "main_camera_name"

#define SKY_SPHERE_NAME "sky_sphere"
#define SKY_BOX_NAME "sky_high_box"
#define WORLD_GRID_NAME "world_grid"
#define MAIN_DIRECTIONAL_LIGHT_NAME "directional_light"

#define DYNAMIC_SCN_NODE "Dynamic"
#define STATIC_SCN_NODE "Static"
#define MESH_SCN_NODE "Mesh"

#define DIFFUSE_TEX_NAME "Diffuse"
#define ROUGH_TEX_NAME "Roughness"
#define NORMAL_TEX_NAME "NormalMap"
#define PARALLAX_TEX_NAME "DisplacementMap"


struct MainDirectionalLight
{
//	Ogre::SceneNode* lightScnNode;
//	Ogre::Light* directionalLight;
};

// Provides access to the entity and sceneNode. saves time in finding the entity object
struct MonsterObject {
//	Ogre::SceneNode* scnNode = nullptr;
//	Ogre::Entity* ent; // ERROR DEPRICATED ENTITY CAN NOT BE REFERED
};

struct WindowProperties {
	int* window_width = new int(0);
	int* window_height = new int(0);
};




enum QueryMask
{
	SKY = 1 << 0,
	GRID = 1 << 1

};

struct ImguiFonts {
//	ImFont* title;
};



struct RSUSShaderTextures {
	//Ogre::TextureUnitState* Diffuse;
	//Ogre::TextureUnitState* Normal;
	//Ogre::TextureUnitState* Roughness;
	//Ogre::TextureUnitState* Parallax;
};

struct RSUShader
{
	std::string materialName;

	//Ogre::GpuProgramParametersPtr fragProgramPtr;
	//Ogre::GpuProgramParametersPtr vertProgramPtr;

	std::string fragShaderName;
	std::string fragShaderFileName;
	std::string vertShaderName;
	std::string vertShaderFileName;
//	std::vector<ShaderVar>* fragVariables;
//	std::vector<ShaderVar>* vertVariables;

	RSUSShaderTextures* textures = new RSUSShaderTextures();

};



// Singleton Class // TODO make a part of Monster class.
/*class RSUS
{
public:

	RSUShader* rsusObj = new RSUShader();

	// NEW SHADER FUNCTIONS
	void setShader(std::string matName,
		std::string fragShaderName, 
		std::string vertShaderName, 
		std::vector<ShaderVar>* fragShaderVar, 
		std::vector<ShaderVar>* vertShaderVar, 
		Ogre::GpuProgramParametersPtr fragProgram, 
		Ogre::GpuProgramParametersPtr vertProgram);
	

	void updateShaderVar(ShaderVar var, ShaderType shaderType);

	void updateShaderVars(std::vector<ShaderVar> var, Ogre::GpuProgramParametersPtr shaderPrgm);

	
	void readMaterial(Ogre::String matName , Ogre::String objectName = "");

	void updateFragParameterInt(Ogre::String parameterName, int val);
	void updateFragParameterFloat(Ogre::String parameterName, float* val);
	void updateFragParameterFloat2(Ogre::String parameterName, float* val);
	void updateFragParameterFloat3(Ogre::String parameterName, float* val);
	void updateFragParameterFloat4(Ogre::String parameterName, float* val);
	void updateFragParameterBool(Ogre::String parameteName, bool val);

	void updateVertParameterInt(Ogre::String parameterName, int val);
	void updateVertParameterFloat(Ogre::String parameterName, float* val);
	void updateVertParameterFloat2(Ogre::String parameterName, float* val);
	void updateVertParameterFloat3(Ogre::String parameterName, float* val);
	void updateVertParameterFloat4(Ogre::String parameterName, float* val);
	void updateVertParameterBool(Ogre::String parameterName, bool val);

	// Class should not be clonable
	RSUS(RSUS& copy) = delete;

	// Class should not be assignable
	void operator=(const RSUS&) = delete;

	// Only way to initalize the class
	static RSUS* GetInstance();


	void setDiffuseTexture(Ogre::Texture* texture);
	void setNormalTexture(Ogre::Texture* texture);
	void setRoughnessTexture(Ogre::Texture* texture);
	void setParallaxTexture(Ogre::Texture* texture);


	

	// Hide the constructor and destructor of the class
protected:
	RSUS() {};
	~RSUS() {};

private:

	void readTextures(Ogre::MaterialPtr mat);

	// to be used with ResourceHandler Save
	ShaderVar _putShaderValue(std::string valueStr);


	// Multi-Thread Shit
	static RSUS* pinstance_;
	static std::mutex mutex_;
	
};*/


class Monster
{

private:
/*	Ogre::SceneNode* CameraNode;
	Ogre::Camera* cam;
	MainDirectionalLight* mdrl = new MainDirectionalLight();

	Ogre::RaySceneQuery* mRayScnQuery;

	Ogre::Vector3 result = Ogre::Vector3();
*/
	//IKEYS* inputkeys;

	// VULKAN INIT
	VulkanStatus vkMonsterStats = VulkanStatus();
	
	// vulkan Init
	void createVulkanInstance();
	void createVulkanSurface();
	void pickVulkanPhysicalDevice();
	void createVulkanDevice();

	

public:

	bool window_fullScreen = false;
	SDL_Window* sdlWindow;

/*	Ogre::MovableObject* RayCastFromPoint();

	WindowProperties* windowProp = new WindowProperties();

	Ogre::ImGuiOverlay* imguiOverlay;

	ImguiFonts* fonts = new ImguiFonts();
*/
	// INITIALIZE OGRE3D AND CREATE A RENDERWINDOW
	// name  -  NAME OF THE RENDERWINDOW
	Monster();

	// INITIALISES SDL3 WINDOW / ADDS DEFAULT CAMERAMAN / GUI SYSTEM
	//void InitMonster(Ogre::Root* root, Ogre::RenderWindow* rWin, Ogre::OverlaySystem* overlay, Ogre::ImGuiOverlay* imguiOverlay_p);

	void InitMonster();

	// SDL CLASSES
	void InitSDLWindow();
	void ShutdownSDL();

	// Vulkan
	void InitVulkan();

/*	void setShadowTechnique();

	// Use this function to add input listner to the application context
	int* FPS;

	Ogre::Root* oRoot;
	Ogre::RenderWindow* renderWindow;
	Ogre::SceneManager* oScnManager;

	Ogre::ManualObject* terrainObj;
	Ogre::SceneNode* terrainScnNode;

	Ogre::SceneNode* skySphere = nullptr;
	Ogre::SceneNode* skyHighNode = nullptr;


	Ogre::ManualObject* visualManualObj;
	Ogre::SceneNode* debugLineScnNode;

	Ogre::ManualObject* debugTriangleObj;
	Ogre::SceneNode* debugTriangleScnNode;

	// Add Subject and observer for render meshes available in Monster
	std::vector<Ogre::MeshPtr> preLoadRenderMeshes = std::vector<Ogre::MeshPtr>();

	// RENDERING RELATED 
	Ogre::SceneNode* addCamera(Ogre::String camName, Ogre::Vector3 startPos);

	Ogre::SceneNode* addToScnNode(Ogre::String meshName, Ogre::SceneNode* toScnNode);


	// MONSTER FUNCTIONS
	void addResourceGroup(std::string groupName);
	void addOgreResourceLocation(std::string path_p, std::string OgreResourceGroup);
	void initalizeResourceGroup(std::string OgreResourceGroup);


	// VALIDATION CHECKERS
	bool resourceExists(std::string resName, std::string groupName);
	bool resourceGroupExists(std::string groupName);
	bool resourceLocationExists(std::string path_p, std::string group);
	bool resourceEntityExists(std::string entityName);

	// MONSTER CREATOR FUNCTIONS
	//	CREATE FUNCTIONS
	Ogre::Entity* createMeshEntity(Ogre::String mshname, Ogre::String groupName = "Render_Mesh");
	Ogre::Entity* createMeshEntity(Ogre::String entityname, Ogre::String mshname, Ogre::String groupName = "Render_Mesh");
	Ogre::Entity* createEntity(Ogre::String entityName_p, Ogre::MeshPtr mesh_p);

	Ogre::MaterialPtr createEmptyMaterial(std::string name_p, Ogre::String groupName);

	Ogre::SceneNode* createNewScnNodeAttach(std::string scnNodeName, Ogre::SceneNode* node);

	//	FETCH FUNCTIONS
	Ogre::MeshPtr getMesh(Ogre::String meshName, Ogre::String groupName = "Render_Mesh");

	Ogre::MaterialPtr getMaterial(Ogre::String matName_p, Ogre::String groupName);

	Ogre::TexturePtr getImageTexture(std::string textureName, Ogre::String groupName);

	// MONSTER DELETE FUNCTIONS
	void rmSceneNode(std::string scnNode);
	void rmEntity(std::string entName);
	void rmMaterial(std::string matName, std::string groupName);



	// OLDER FUNCTIONS - RETAIN THEM FOR FUTURE PURPOSE
	Ogre::SceneNode* loadMeshScnNode(Ogre::String scnNodeType, Ogre::String scnNodeName, Ogre::String objectname, Ogre::String meshName, Ogre::String groupName = "Render_Mesh", bool castShadow = true);
	


	Ogre::SceneNode* loadMeshScnNode(
		Ogre::String scnNodeType,
		Ogre::String scnNodeName,
		Ogre::String meshName,
		size_t& vertex_count,
		size_t& index_count,
		Ogre::Vector3*& vertices,
		unsigned long*& indices,
		Ogre::String groupName = "Render_Mesh",
		bool castShadow = true
	);
	Ogre::SceneNode* addManualObject(Ogre::String scn_name,Ogre::ManualObject* mo);

	void addLight(std::string lightName,
		Ogre::Light::LightTypes type,
		int powerScale,
		Ogre::Vector3 position,
		Ogre::Vector3 direction);
	void addMainDirectionalLight(std::string lightName, Ogre::Vector3 dir, int power);


	// IMGUI OVERLAY
	void setupFonts();

	// adds a resource location 
	// loc - location from current working directory
	void addResourceLocation(const char* loc);

	Ogre::SceneNode* getPlayerSceneNode();

	void getMeshInfoFromColName(
		std::string colliderName,
		size_t& vertex_count,
		size_t& index_count,
		Ogre::Vector3*& vertices,
		unsigned long*& indices,
		const Ogre::Vector3& position,
		const Ogre::Quaternion& orient,
		const Ogre::Vector3& scale
	);
	void getMeshInformation(
		const Ogre::Mesh* const mesh,
		size_t& vertex_count,
		Ogre::Vector3*& vertices,
		size_t& index_count,
		unsigned long*& indices,
		const Ogre::Vector3& position,
		const Ogre::Quaternion& orient,
		const Ogre::Vector3& scale
	);
	void getMeshVerticesInformation(
		const Ogre::Mesh* const mesh,
		size_t& vertex_count,
		Ogre::Vector3*& vertices,
		const Ogre::Vector3& position,
		const Ogre::Quaternion& orient,
		const Ogre::Vector3& scale
	);

	Ogre::Camera* getCamera() { return cam; }
	Ogre::SceneNode* getCameraScnNode() { return CameraNode; }


	HWND* getHWND();
*/
	void updateMonster();

//	void deleteScnNode(Ogre::SceneNode* scnNode);

	
//	void _setupRTShader();

	// WINDOW RELATED
	
//	void _setupSDL3(INT64 windowWidth, INT64 windowHeight, Ogre::String windowName);


	void Shutdown();

};

// for preprogrammed rotations only.
/*void setObjRotation(Ogre::SceneNode* scnNode, Ogre::Vector3& rot);

Ogre::Vector3 getObjRotation(Ogre::SceneNode* scnNode);


class ShaderSaveFileInconsistent : public std::exception {
public:

	char* what() {
		std::cout << "Shader Save File is not inline with the shader script. " << std::endl;
	}

};*/



#endif // !MONSTER_H



