#include "terrain.h"

Terrain::Terrain(Ogre::SceneManager* scnManager)
{
	oScnManager = scnManager;

	terrainMObj = oScnManager->createManualObject(TERRAIN_MOBJ_NAME);

	terrainScnNode = oScnManager->getRootSceneNode()->createChildSceneNode(TERRAIN_SCN_NODE_NAME);

	terrainScnNode->attachObject(terrainMObj);


}

void Terrain::createTerrain(int terrainBlocks, int blockSize , Ogre::MaterialPtr mat)
{
	if (hasInitialized) {
		terrainMObj->beginUpdate(0);
	}
	else {
		terrainMObj->begin(mat);
	}
	

	for (int x = 0; x < terrainBlocks; x++)
	{
		for (int y = 0; y < terrainBlocks; y++)
		{

			int terrainX = x * blockSize;
			int terrainZ = y * blockSize;

			// terrain triangle 1
			terrainMObj->position(terrainX, 0, terrainZ);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(0, 0);

			terrainMObj->position(terrainX, 0, terrainZ + blockSize);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			terrainMObj->position(terrainX + blockSize, 0, terrainZ);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			// terrain triangle 2
			terrainMObj->position(terrainX, 0, terrainZ + blockSize);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			terrainMObj->position(terrainX + blockSize, 0, terrainZ + blockSize);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			terrainMObj->position(terrainX + blockSize, 0, terrainZ);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

		}
	}

	terrainMObj->end();
	hasInitialized = true;
	terrainScnNode->setPosition(Ogre::Vector3(-(terrainBlocks / 2), 0, -(terrainBlocks / 2)));

}

void Terrain::createHightMapTerrain(Ogre::String hightmap, Ogre::MaterialPtr mat, int blocks, float displacement, float scale)
{

	std::cout << "creating height map terrain" << std::endl;

	Ogre::Image* imgManager = new Ogre::Image();
	imgManager->load(hightmap, "Images");

	// setup terrain
	if (hasInitialized) {
		terrainMObj->beginUpdate(0);
	}
	else {
		terrainMObj->clear();
		terrainMObj->begin(mat, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	}
	
	float inDisplacement = (displacement + 1.0f) *scale;

	float dis = 0.0f;

	for (int width = 0; width + blocks < imgManager->getWidth(); width++)
	{
		for (int height = 0; height + blocks < imgManager->getHeight(); height++)
		{
			dis = imgManager->getColourAt(width, height, 0).r * inDisplacement;
			//std::cout << "Displacement : " << dis << std::endl;

			int terrainX = width * blocks;
			int terrainZ = height * blocks;

			// terrain triangle 1
			terrainMObj->position(terrainX, dis, terrainZ);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(0, 0);

			dis = imgManager->getColourAt(width, height + blocks, 0).r * inDisplacement;

			terrainMObj->position(terrainX, dis, terrainZ + blocks);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			dis = imgManager->getColourAt(width + blocks, height, 0).r * inDisplacement;

			terrainMObj->position(terrainX + blocks, dis, terrainZ);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			// terrain triangle 2

			dis = imgManager->getColourAt(width, height + blocks, 0).r * inDisplacement;

			terrainMObj->position(terrainX, dis, terrainZ + blocks);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			dis = imgManager->getColourAt(width + blocks, height + blocks, 0).r * inDisplacement;

			terrainMObj->position(terrainX + blocks, dis, terrainZ + blocks);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);

			dis = imgManager->getColourAt(width + blocks, height, 0).r * inDisplacement;

			terrainMObj->position(terrainX + blocks, dis, terrainZ);
			terrainMObj->normal(0, 1, 0);
			terrainMObj->textureCoord(1, 1);


		}

	}

	terrainMObj->end();
	hasInitialized = true;

	terrainScnNode->setPosition(Ogre::Vector3(-((int)imgManager->getWidth() / 2), 0, -((int)imgManager->getHeight() / 2)));


}
