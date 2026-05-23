#pragma once


// Local
#include <feel/playerMovement.h>
#include <GDHandler/GDSystem.h>

// Third

// STL



class Scene;
class Case;
class Shader;
class Object;
class Material;

class ResourceHandler;
class Monster;
class Kint;



// Primary Class.
// Used to build object with all required contexts and initializations
class GDBuilderContext : public GDSystem
{	
protected:

	ResourceHandler* resourceHandler;
	Monster* monster;

	void startEngine();


public:

	GDBuilderContext(ResourceHandler* resourceHan_p, Monster* monster_p) : GDSystem(monster_p) {
		resourceHandler = resourceHan_p;
		monster = monster_p;

		startEngine();
		
	}

	virtual void update(float deltaTime) {};

	//Ogre
	void loadMaterialsDpToOgre();

	// Monster functions
	void monSetLocation(std::filesystem::path meshParentPath_p, std::string OgreGroup);

	Ogre::MeshPtr monGetMesh(std::string meshName_p);
	Ogre::Entity* monCreateEntity(std::string name_p, Ogre::MeshPtr mesh_p);

	Ogre::TexturePtr monGetTexture(std::string textureName);

	Ogre::MaterialPtr monCreateMaterial(std::string name_p);
	Ogre::MaterialPtr monCreateNewMaterial(std::string name_p);

	Ogre::SceneNode* monCreateSceneNode(std::string name_p, Ogre::SceneNode* parentNode_p);
	void monDeleteSceneNode(Ogre::SceneNode* scene_p);
	void monDeleteEntity(Ogre::Entity* entity_p);

	RSUS* monProvideRsus();
	ImguiFonts* monProvideFonts();

	void monSetShaderVars(std::vector<ShaderVar> vars_p, Ogre::GpuProgramParametersPtr shaderPrgm);

	// Check Functions
	bool sceneExists(std::string scnName_p);
	bool objectExists(std::string objName_p);

	// CASEHANDLER Functions
	virtual Scene* CreateScene(std::string sceneName, SceneType type, Ogre::SceneNode* parentNode_p) { return nullptr; };
	virtual Scene* CreateScene(std::string sceneName, SceneType type) { return nullptr; };
	virtual Object* CreateObject(std::string objName_p, std::filesystem::path meshPath_p, PhysXType type) { return nullptr; };

	virtual Ogre::MeshPtr fetchMeshByName(std::filesystem::path meshPath_p) { return nullptr; };
	virtual Ogre::MeshPtr fetchMeshById(ResID meshID_p) { return nullptr; };

	virtual Material* CreateMaterial(std::filesystem::path materialPath_p, std::string materialName) { return nullptr; };
	



};



Ogre::Quaternion Vec4toQuaternion(Ogre::Vector4 orientation);

Ogre::Vector4 QuaternionToVec4(Ogre::Quaternion quat);