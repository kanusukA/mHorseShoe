
#include <Ogre.h>
#include <iostream>

#define TERRAIN_MOBJ_NAME "terrain_manual_object"
#define TERRAIN_SCN_NODE_NAME "terrain_scn_node"

class Terrain {

private:

	Ogre::SceneNode* terrainScnNode;

	Ogre::ManualObject* terrainMObj;
	
	Ogre::SceneManager* oScnManager;

	bool hasInitialized = false;

public:

	Terrain(Ogre::SceneManager* scnManager);

	void createTerrain(int terrainSize, int blocks, Ogre::MaterialPtr mat);

	void createHightMapTerrain(Ogre::String hightmap, Ogre::MaterialPtr mat, int blocks, float displacement, float scale);
	

};