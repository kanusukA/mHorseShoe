#include "Monster.h"

// Multi-Thread shit
RSUS* RSUS::pinstance_{ nullptr };
std::mutex RSUS::mutex_;

//Monster::Monster(Ogre::String name)
//{
//	// Basic Ogre setup
//	OgreBites::ApplicationContext ctx("mOgreCtx");
//	ctx.initApp();
//
//	renderWindow = ctx.getRenderWindow();
//	oRoot = ctx.getRoot();
//	oScnManager = oRoot->createSceneManager();
//
//	// RTShader System
//	_setupRTShader();
//
//	// Overlay | TraySystem Setup
//	Ogre::OverlaySystem* overSys = ctx.getOverlaySystem();
//	oScnManager->addRenderQueueListener(overSys);
//
//	oTrayMan = new OgreBites::TrayManager(TRAY_SYSTEM_NAME, ctx.getRenderWindow());
//	ctx.addInputListener(oTrayMan);
//
//}


 Ogre::MovableObject* Monster::RayCastFromPoint()
{
	// create a ray (of hope)
	Ogre::Ray ray = Ogre::Ray(cam->getRealPosition(), cam->getRealDirection());
	//std::cout << "Direction " << ray.getDirection().x << " " << ray.getDirection().y << " " << ray.getDirection().z << std::endl;
	//std::cout << "origin " << ray.getOrigin().x << " " << ray.getOrigin().y << " " << ray.getOrigin().z << std::endl;
	// null cheak
	if (mRayScnQuery) {

		// set Ray
		mRayScnQuery->setRay(ray);

		// get a vector of hits
		Ogre::RaySceneQueryResult result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResultEntry resultEntry;
		
		if (result.size() > 0) { // Starts from 2 to compensate for skybox
			// congrats hit!!
			resultEntry = result[0];
			for (int i = 0; i < result.size(); i++)
			{

				//std::cout << "Selectable : " << result.at(i).movable->getName() << " Dist : " << result[i].distance << std::endl;
				//std::cout << "Hit : " << result[i].movable->getParentSceneNode()->getName() << std::endl;
				if (result[i].distance < resultEntry.distance) {
					
					resultEntry = result[i];
					
				}
			}
			//std::cout << "Selected : " << resultEntry.movable->getName() << std::endl;
			if(resultEntry.movable){
				return resultEntry.movable;
			}
			return nullptr;
		}
		else {
			
			return nullptr;
		}

	}
	else {
		return nullptr;
	}
}

Monster::Monster(Ogre::Root* root, Ogre::RenderWindow* rWin, Ogre::OverlaySystem* overlay)
{
	oRoot = root;
	
	renderWindow = rWin;

	oScnManager = root->createSceneManager();

	oScnManager->setAmbientLight(Ogre::ColourValue(0.53, 0.2, 0.12));
	oScnManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_MODULATIVE);

	Ogre::MaterialPtr casterMat = Ogre::MaterialManager::getSingleton().getByName("mShadowCast");
	Ogre::MaterialPtr receiverMat = Ogre::MaterialManager::getSingleton().getByName("mShadowReciv");

	oScnManager->setShadowTextureCasterMaterial(casterMat);
	oScnManager->setShadowTextureReceiverMaterial(receiverMat);


	//oScnManager->setShadowTextureSelfShadow(true);
	
	oScnManager->setShowDebugShadows(true);

	_setupRTShader();

	oScnManager->addRenderQueueListener(overlay);
	
	
	

	// raycast setup
	mRayScnQuery =  oScnManager->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

	mRayScnQuery->setQueryMask(~QueryMask::SKY & ~QueryMask::GRID);
	

	inputkeys = InputHandler::GetInstance()->getInputKeys();

	
	
}



void Monster::InitMonster() {

	// DOESN'T WORK WITH VULKAN RENDERER
	
	_setupSDL3(1600,900,"Psycho");

	SDL_GetWindowSize(sdlWindow, windowProp->window_width, windowProp->window_height);
	//SDL_SetWindowFullscreen(sdlWindow, true);

	// Add Camera
	this->addCamera(MAIN_CAMERA_NAME, Ogre::Vector3(5, 5, 5));

	
}


Ogre::SceneNode* Monster::addCamera(Ogre::String camName, Ogre::Vector3 startPos)
{
	Ogre::SceneNode* camNode = oScnManager->getRootSceneNode()->createChildSceneNode("CAM_NODE");

	cam = oScnManager->createCamera(camName);
	cam->setAutoAspectRatio(true);
	cam->setNearClipDistance(0.5);
	

	

	camNode->attachObject(cam);
	camNode->setPosition(startPos);

	renderWindow->addViewport(cam);

	this->CameraNode = camNode;
	

	return camNode;
}

Ogre::Entity* Monster::getMeshEntity(Ogre::String mshname, Ogre::String groupName)
{
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(mshname, groupName);
	return oScnManager->createEntity(msh);
}

Ogre::Entity* Monster::getMeshEntity(Ogre::String entityname, Ogre::String mshname, Ogre::String groupName)
{
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(mshname, groupName);
	return oScnManager->createEntity(entityname,msh);
}

Ogre::SceneNode* Monster::addToScnNode(Ogre::String meshName, Ogre::SceneNode* toScnNode)
{
	Ogre::SceneNode* scnNode = toScnNode->createChildSceneNode();
	scnNode->attachObject(getMeshEntity(meshName));
	return scnNode;
}

Ogre::SceneNode* Monster::loadMeshScnNodeFromEnt(Ogre::String scnNodeName, Ogre::Entity* ent)
{
	Ogre::SceneNode* scnNode = oScnManager->getRootSceneNode()->createChildSceneNode(scnNodeName);
	scnNode->attachObject(ent);
	return scnNode;
}

Ogre::SceneNode* Monster::loadMeshScnNodeFromEnt(Ogre::Entity* ent)
{
	Ogre::SceneNode* scnNode = oScnManager->getRootSceneNode()->createChildSceneNode();
	scnNode->attachObject(ent);
	return scnNode;
}


Ogre::SceneNode* Monster::loadMeshScnNode(Ogre::String scnNodeName, Ogre::String meshName, Ogre::String groupName, bool castShadow)
{
	// if mesh exists
	std::cout << "mesh loading : " << meshName << std::endl;
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(meshName, groupName);
	
	std::cout << "has edgeList : " << msh.get()->isEdgeListBuilt() << std::endl;

	Ogre::SceneNode* scnNode = oScnManager->getRootSceneNode()->createChildSceneNode(scnNodeName);
	Ogre::Entity* ent = oScnManager->createEntity(msh);
	ent->setCastShadows(castShadow);
	scnNode->attachObject(ent);

	return scnNode;
}


Ogre::SceneNode* Monster::loadMeshScnNode(
	Ogre::String scnNodeName,
	Ogre::String meshName,
	size_t& vertex_count,
	size_t& index_count,
	Ogre::Vector3*& vertices,
	unsigned long*& indices,
	Ogre::String groupName,
	bool castShadow
)
{
	// if mesh exists
	std::cout << "mesh loading : " << meshName << std::endl;
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(meshName, groupName);

	Ogre::Entity* ent = oScnManager->createEntity(msh);
	ent->setCastShadows(castShadow);

	Ogre::SceneNode* scnNode = oScnManager->getRootSceneNode()->createChildSceneNode(scnNodeName);
	scnNode->attachObject(ent);
	
	getMeshInformation(msh.get(), vertex_count, vertices, index_count, indices, scnNode->getPosition(), scnNode->getOrientation(), scnNode->getScale());

	return scnNode;
}

Ogre::SceneNode* Monster::addManualObject(Ogre::String scn_name, Ogre::ManualObject* mo)
{
	Ogre::SceneNode* scnNode = oScnManager->getRootSceneNode()->createChildSceneNode(scn_name);
	scnNode->attachObject(mo);
	return scnNode;
}

void Monster::addLight(
	std::string lightName, 
	Ogre::Light::LightTypes type, 
	int powerScale,
	Ogre::Vector3 position,
	Ogre::Vector3 direction
)
{
	Ogre::Light* light = oScnManager->createLight(lightName);
	light->setType(type);	
	light->setPowerScale(powerScale);
	light->setCastShadows(true);
	Ogre::SceneNode* lightNode = oScnManager->getRootSceneNode()->createChildSceneNode(lightName + "_scn");
	lightNode->attachObject(light);
	lightNode->setDirection(direction);

}

void Monster::addMainDirectionalLight(std::string lightName, Ogre::Vector3 dir , int power)
{

	if (mdrl->directionalLight && mdrl->lightScnNode) {
		mdrl->directionalLight->setPowerScale(power);
		mdrl->lightScnNode->setDirection(dir);
	}
	else {
		Ogre::Light* light = oScnManager->createLight(lightName);
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setCastShadows(true);

		light->setPowerScale(power);
		Ogre::SceneNode* mainLightScnNode = oScnManager->getRootSceneNode()->createChildSceneNode(lightName + "_scn");
		mainLightScnNode->attachObject(light);
		mainLightScnNode->setDirection(dir);
		mainLightScnNode->setPosition(100, 200, 0);

		mdrl->directionalLight = light;
		mdrl->lightScnNode = mainLightScnNode;
	}

}




//void Monster::showDebugLines(bool value)
//{
//	if (debugLineScnNode)
//	{
//		debugLineScnNode->setVisible(value);
//	}
//}

//void Monster::initDebugLines()
//{
//	debugLineScnNode = oScnManager->getRootSceneNode()->createChildSceneNode("Gizmoz");
//
//	visualManualObj = new Ogre::ManualObject("This_Object");
//	visualManualObj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
//	visualManualObj->position(0, 0, 0);
//	visualManualObj->position(0, 1, 0);
//	visualManualObj->end();
//
//	debugLineScnNode->attachObject(visualManualObj);
//
//}
//
//void Monster::updateDebugLines(const physx::PxDebugLine* dbgLines, int nbLines)
//{
//	visualManualObj->beginUpdate(0);
//
//	for (int i = 0; i < nbLines; i++) {
//		//First Vertex
//		visualManualObj->position(dbgLines[i].pos0.x, dbgLines[i].pos0.y, dbgLines[i].pos0.z);
//		// Second Vertex
//		visualManualObj->position(dbgLines[i].pos1.x, dbgLines[i].pos1.y, dbgLines[i].pos1.z);
//	}
//	
//	visualManualObj->end();
//}
//
//void Monster::initDebugTriangles()
//{
//	debugTriangleScnNode = oScnManager->getRootSceneNode()->createChildSceneNode();
//	
//	debugTriangleObj = new Ogre::ManualObject("Triangle_Object");
//	debugTriangleObj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
//	debugTriangleObj->position(0, 0, 0);
//	debugTriangleObj->position(1, 2, 0);
//	debugTriangleObj->position(2, 0, 0);
//	debugTriangleObj->end();
//
//	debugTriangleScnNode->attachObject(debugTriangleObj);
//}
//
//void Monster::showDebugTriangles()
//{
//}
//
//void Monster::updateDebugTriangles(const physx::PxDebugTriangle* dbgTriangle, int nbTriangles)
//{
//	debugTriangleObj->beginUpdate(0);
//	for (int i = 0; i < nbTriangles; i++) {
//		debugTriangleObj->position(dbgTriangle[i].pos0.x, dbgTriangle[i].pos0.y, dbgTriangle[i].pos0.z);
//		debugTriangleObj->position(dbgTriangle[i].pos1.x, dbgTriangle[i].pos1.y, dbgTriangle[i].pos1.z);
//		debugTriangleObj->position(dbgTriangle[i].pos2.x, dbgTriangle[i].pos2.y, dbgTriangle[i].pos2.z);
//	}
//	debugTriangleObj->end();
//}

void Monster::addResourceLocation(const char* loc)
{
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(loc, "FileSystem");
	}
	catch (const std::exception& exception)
	{
		std::cout << "Failed to load Monster ResourceLocation : " << exception.what();
	}
	
}

Ogre::SceneNode* Monster::getPlayerSceneNode()
{
	return this->CameraNode;
}

void Monster::getMeshInfoFromColName(
	std::string colliderName,
	size_t& vertex_count,
	size_t& index_count,
	Ogre::Vector3*& vertices,
	unsigned long*& indices,
	const Ogre::Vector3& position,
	const Ogre::Quaternion& orient,
	const Ogre::Vector3& scale
)
{
	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load(colliderName, "Collider_Mesh");
	getMeshInformation(mesh.get(), vertex_count, vertices, index_count, indices, position, orient, scale);

}

void Monster::getMeshInformation(
	const Ogre::Mesh* const mesh, 
	size_t& vertex_count, 
	Ogre::Vector3*& vertices, 
	size_t& index_count, 
	unsigned long*& indices, 
	const Ogre::Vector3& position, 
	const Ogre::Quaternion& orient, 
	const Ogre::Vector3& scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculates How many vertices and indices are required
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {

		Ogre::SubMesh* subMesh = mesh->getSubMesh(i);

		if (subMesh->useSharedVertices) {
			if (!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			vertex_count += subMesh->vertexData->vertexCount;
		}
		// Add indices
		index_count += subMesh->indexData->indexCount;
	}

	// Allocate space for vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

	// adding data to array
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* subMesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = subMesh->useSharedVertices ? mesh->sharedVertexData : subMesh->vertexData;

		if ((!subMesh->useSharedVertices) || (subMesh->useSharedVertices && !added_shared))
		{
			if (subMesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf.get()->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf.get()->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf.get()->unlock();
			next_offset += vertex_data->vertexCount;
		}

		Ogre::IndexData* index_data = subMesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf.get()->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long* pLong = static_cast<unsigned long*>(ibuf.get()->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (subMesh->useSharedVertices) ? shared_offset : current_offset;

		if (use32bitindexes) {
			for (size_t k = 0; k < numTris * 3; ++k) {
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else {
			for (size_t k = 0; k < numTris * 3; ++k) {
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
					static_cast<unsigned long>(offset);
			}
		}

		ibuf.get()->unlock();
		current_offset = next_offset;

	}
}

void Monster::getMeshVerticesInformation(
	const Ogre::Mesh* const mesh,
	size_t& vertex_count,
	Ogre::Vector3*& vertices,
	const Ogre::Vector3& position,
	const Ogre::Quaternion& orient,
	const Ogre::Vector3& scale
	)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = 0;

	// Calculates How many vertices and indices are required
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {

		Ogre::SubMesh* subMesh = mesh->getSubMesh(i);

		if (subMesh->useSharedVertices) {
			if (!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			vertex_count += subMesh->vertexData->vertexCount;
		}

	}

	// Allocate space for vertices and indices
	vertices = new Ogre::Vector3[vertex_count];

	added_shared = false;

	// adding data to array
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* subMesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = subMesh->useSharedVertices ? mesh->sharedVertexData : subMesh->vertexData;

		if ((!subMesh->useSharedVertices) || (subMesh->useSharedVertices && !added_shared))
		{
			if (subMesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf.get()->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf.get()->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
	
			}

			vbuf.get()->unlock();
			next_offset += vertex_data->vertexCount;
		}

	}
}


void Monster::setSkyBox()
{

	


	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	//oScnManager->setSkyBox(true, "mySky",300,true,Ogre::Quaternion::IDENTITY,"Render_Mesh");
	

	Ogre::MaterialPtr skyMat = Ogre::MaterialManager::getSingleton().getByName("mySky");
	Ogre::MaterialPtr skyHighMat = Ogre::MaterialManager::getSingleton().getByName("myskyHigh");
	Ogre::GpuProgramParametersPtr skyHighParam = skyHighMat.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	Ogre::GpuProgramParametersPtr param = skyMat.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();


	skyHighParam.get()->setNamedConstant("baseColor", Ogre::Vector4(0.1, 0.01, 0.06, 1.0));

	skyHighParam.get()->setNamedConstant("highlightCol", Ogre::Vector4(0.82, 0.67, 1.0, 1.0));
	skyHighParam.get()->setNamedConstant("highlightOffset", 0.26f); // higher value = lower highlight
	skyHighParam.get()->setNamedConstant("highlightSmoothness", 0.32f);

	skyHighParam.get()->setNamedConstant("middleColor", Ogre::Vector4(0.84, 0.11, 0.29, 1.0));
	skyHighParam.get()->setNamedConstant("middleOffset", 0.36f);
	skyHighParam.get()->setNamedConstant("middleSmoothness", 0.14f);

	skyHighParam.get()->setNamedConstant("coreColor", Ogre::Vector4(1.0, 0.4, 0.19, 1.0));
	skyHighParam.get()->setNamedConstant("coreOffset", 0.45f);
	skyHighParam.get()->setNamedConstant("coreSmoothness", 0.085f);

	skyHighParam.get()->setNamedConstant("bumpOffset", 0.5f);
	skyHighParam.get()->setNamedConstant("bumpRange", 0.46f);
	skyHighParam.get()->setNamedConstant("bumpHeight", 0.05f);

	skyHighParam.get()->setNamedConstant("starsOffset", 0.005f);

	skyHighParam.get()->setNamedConstant("farFogCol", Ogre::Vector4(0.84, 0.11, 0.29, 1.0));
	skyHighParam.get()->setNamedConstant("farFogOffset", 0.4f);
	skyHighParam.get()->setNamedConstant("farFogSmoothness", 0.25f);

	skyHighParam.get()->setNamedConstant("windSpeed", 0.2f);
	
	

	// text colors
	/*param.get()->setNamedConstant("skyHigh", Ogre::Vector4(0.62, 0.0, 1.0, 1.0));
	param.get()->setNamedConstant("skyMid", Ogre::Vector4(0.0, 1.0, 0.13, 1.0));
	param.get()->setNamedConstant("skyLow", Ogre::Vector4(1.0, 0.0, 0.0, 1.0));*/

	//param.get()->setNamedConstant("skyHigh", Ogre::Vector4(1.0, 0.22, 0.0, 1.0));
	//param.get()->setNamedConstant("skyMid", Ogre::Vector4(0.96, 0.82, 0.3, 1.0));
	//param.get()->setNamedConstant("skyLow", Ogre::Vector4(0.0, 0.01, 0.14, 1.0));

	param.get()->setNamedConstant("moonSize", 0.025f);
	param.get()->setNamedConstant("moonSmoothCol", Ogre::Vector4(0.93, 0.61, 0.44, 1.0));
	param.get()->setNamedConstant("moonCoreCol", Ogre::Vector4(1.0, 1.0, 1.0, 1.0));

	param.get()->setNamedConstant("worldSpaceLightPos", Ogre::Vector3(-0.14,-0.6,0));

	//param.get()->setNamedConstant("farFogOffset", 0.35f);
	//param.get()->setNamedConstant("farFogSmoothness", 0.3f);

	//param.get()->setNamedConstant("starCol", Ogre::Vector4(0.92, 0.85, 1.0, 1.0));

	// NOTE : SMOOTHNESS IS ADDED TO THE OFFSET , USE SMALLER VALUES IN THE OFFSET

	

	//param.get()->setNamedConstant("baseHighXStartOffset", 0.2f);
	//param.get()->setNamedConstant("baseHighXStartSmoothness", 0.2f);

	//param.get()->setNamedConstant("baseHighXEndOffset", 0.6f); // NOT SMALLER THAN baseHighXStartOffset + baseHighXStartSmoothness
	//param.get()->setNamedConstant("baseHighXEndSmoothness", 0.2f);

	
	//param.get()->setNamedConstant("cloudsLowOffset", 0.3f);
	//param.get()->setNamedConstant("cloudsHighOffset", 0.5f);
	

	//param.get()->setNamedConstant("middleOffset", 0.5f); // low = more middle
	//param.get()->setNamedConstant("topOffset", 0.0f);

	//param.get()->setNamedConstant("topSmooth", 0.3f);
	//param.get()->setNamedConstant("middleSmooth", 0.2f);

	//param.get()->setNamedConstant("cloudThreshold", 0.2f); // the amount of clouds edge to smooth
	//param.get()->setNamedConstant("cloudSmoothness", 0.6f); 
	//param.get()->setNamedConstant("cloudColor", Ogre::Vector4(1.0, 1.0, 1.0, 1.0));
	//param.get()->setNamedConstant("cloudTex_ST", Ogre::Vector4(1, 1, 0, 0));

	//param.get()->setNamedConstant("panningSpeedX", 0.2f);
	//param.get()->setNamedConstant("panningSpeedY", 0.2f);

	//param.get()->setNamedConstant("starsTex_ST", Ogre::Vector4(1, 1, 1, 1));
	//param.get()->setNamedConstant("starsIntensity", 0.5f);

	//param.get()->setNamedConstant("sunSize", 0.5f);
	//param.get()->setNamedConstant("sunColor", Ogre::Vector4(1, 1, 1, 1));
	//param.get()->setNamedConstant("sunCloudIntensity", 0.5f);

	//param.get()->setNamedConstant("moonPhase", 0.5f);
	//param.get()->setNamedConstant("moonSize", 0.5f);
	//param.get()->setNamedConstant("moonColor", Ogre::Vector4(1, 1, 1, 1));

	//oScnManager->setSkyDome(true, "mySky", 12.0f, 1.0f, 4000, true, Ogre::Quaternion::IDENTITY, 16, 16, -1, "Render_Mesh");


	// clouds
	// custom object of plane
	

	
	Ogre::Entity* cloudsEnt = this->getMeshEntity("Plane.mesh");
	Ogre::MaterialPtr cloudsMat = Ogre::MaterialManager::getSingleton().getByName("clouds_material", "Mesh_Materials");
	Ogre::GpuProgramParametersPtr cloudsParam = cloudsMat.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

	cloudsParam.get()->setNamedConstant("_cloudCol", Ogre::Vector4(0.18, 0.13, 0.24, 0.75));


	cloudsEnt->setMaterial(cloudsMat);
	cloudsEnt->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_SKIES_LATE);
	
	Ogre::SceneNode* cloudsNode = oScnManager->getRootSceneNode()->createChildSceneNode();
	cloudsNode->attachObject(cloudsEnt);

	cloudsNode->setScale(Ogre::Vector3(10000,1,10000));
	cloudsNode->setPosition(0, 500, 0);
	cloudsNode->setVisible(true);

	



	// SKY BOX


	
	skyHighNode = oScnManager->getRootSceneNode()->createChildSceneNode(SKY_BOX_NAME);

	
	Ogre::Entity* ent_high = this->getMeshEntity("sky_box_mesh", "Sphere_up.mesh", "Render_Mesh");

	ent_high->setMaterial(skyHighMat);
	ent_high->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_SKIES_LATE);
	skyHighNode->attachObject(ent_high);
	skyHighNode->setPosition(0, -2500, 0);
	skyHighNode->setScale(Ogre::Vector3(9000));

	skySphere = oScnManager->getRootSceneNode()->createChildSceneNode(SKY_SPHERE_NAME);

	Ogre::Entity* ent_sky = this->getMeshEntity("sky_sphere_mesh", "Sphere.mesh", "Render_Mesh");

	ent_sky->setMaterial(skyMat);
	ent_sky->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_SKIES_EARLY);
	skySphere->attachObject(ent_sky);
	skySphere->setPosition(0, -2500, 0);
	//skySphere->setOrientation(Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_X));
	skySphere->setScale(Ogre::Vector3(8500));

	Ogre::Plane plane = Ogre::Plane();
	plane.d = 1000.0f;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

	ent_high->setQueryFlags(QueryMask::SKY);
	ent_sky->setQueryFlags(QueryMask::SKY);

	//oScnManager->setSkyPlane(true, plane, "myCloud", 1000.0f, 1.0f, true, 1.0f, 16, 16, "Render_Mesh");
	//oScnManager->setSkyBox(true, "mySky");
	
}

void Monster::setGrid()
{

	Ogre::MaterialPtr gridMat = Ogre::MaterialManager::getSingleton().getByName("myGrid");
	RSUS::GetInstance()->readMaterial("myGrid");


	Ogre::ManualObject* gridObj = oScnManager->createManualObject(WORLD_GRID_NAME);

	gridObj->begin("myGrid", Ogre::RenderOperation::OT_TRIANGLE_STRIP, "Mesh_Materials");

	gridObj->position(Ogre::Vector3(0, 0, 0));
	gridObj->normal(0, 1, 0);
	gridObj->textureCoord(0, 0);

	gridObj->position(Ogre::Vector3(0, 0, 10000));
	gridObj->normal(0, 1, 0);
	gridObj->textureCoord(0, 1);

	gridObj->position(Ogre::Vector3(10000, 0, 0));
	gridObj->normal(0, 1, 0);
	gridObj->textureCoord(1, 0);

	gridObj->position(Ogre::Vector3(10000, 0, 10000));
	gridObj->normal(0, 1, 0);
	gridObj->textureCoord(1, 1);


	gridObj->end();

	//update bounding box


	gridObj->convertToMesh("gridMesh");
	
	gridObj->setQueryFlags(QueryMask::GRID);
	

	Ogre::SceneNode* gridNode = oScnManager->getRootSceneNode()->createChildSceneNode("GridScnNode");
	gridNode->attachObject(gridObj);
	
	gridNode->setPosition(-5000, 0, -5000);

}

void Monster::createTerrain(Ogre::Vector2 size,unsigned int vertSize, unsigned int grassDensity)
{

	Ogre::ManualObject* gMO = this->_createGrassBlade(0.3, 1);
	

	unsigned int numberOfGrasses = 0;
	// terrain setup
	terrainObj = oScnManager->createManualObject("terrain");
	terrainObj->begin("myMat",Ogre::RenderOperation::OT_TRIANGLE_LIST, "Mesh_Materials");

	// Grass setup
	Ogre::StaticGeometry* mField = oScnManager->createStaticGeometry("stat_geo");
	//mField->setOrigin(Ogre::Vector3(0, 1, 0));

	Ogre::Entity* ent = this->getMeshEntity("Blade2mesh.mesh");
	ent->setCastShadows(true);
	
	//Ogre::Entity* ent = oScnManager->createEntity(Ogre::MeshManager::getSingleton().getByName("CustomGrassMesh"));
	Ogre::MaterialPtr ent_mat = Ogre::MaterialManager::getSingleton().getByName("Blade_mat");
	//Ogre::MeshPtr grassMsh = gMO->convertToMesh("gMan");
	//grassMsh.get()->buildEdgeList();
	//Ogre::Entity* ent = oScnManager->createEntity(grassMsh);


	ent->setMaterial(ent_mat);

	float radius = 0.3f;

	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{

			int terrainX = j * vertSize;
			int terrainZ = i * vertSize;

			// terrain triangle 1
			terrainObj->position(terrainX, 0, terrainZ);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(0, 0);

			terrainObj->position(terrainX, 0, terrainZ + vertSize);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			terrainObj->position(terrainX + vertSize, 0, terrainZ);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			// terrain triangle 2
			terrainObj->position(terrainX, 0, terrainZ + vertSize);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			terrainObj->position(terrainX + vertSize, 0, terrainZ + vertSize);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			terrainObj->position(terrainX + vertSize, 0, terrainZ);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);


			

			// Grass positioning
			for (int grassPerVert = 0; grassPerVert < grassDensity; grassPerVert++)
			{
				// generates number between -6.0 to 6.0 for the offset
				double randomX = 1.0 - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
				double randomY = 1.0 - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));

				//std::cout << "random " << randomX << " " << randomY << std::endl;

				double grassX = (terrainZ) + randomX;
				double grassY = (terrainX) + randomY;

				
				// Patch
				for (int patch_g = 0; patch_g < 5; patch_g++)
				{
					float rangeX = Ogre::Math::RangeRandom(-radius, radius); // fix custom grass rendering and edge list!!!
					float rangeY = Ogre::Math::RangeRandom(-radius, radius);

					float scaledY = Ogre::Math::RangeRandom(1, 3);
					Ogre::Vector3 pos = Ogre::Vector3(grassX + rangeX,(scaledY/2), grassY + rangeY);

					Ogre::Vector3 scale = Ogre::Vector3(3, scaledY, 3);
					//Ogre::Vector3 scale = Ogre::Vector3(3, 3, 3);

					Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Degree(Ogre::Math::RangeRandom(0,360)), Ogre::Vector3::UNIT_Y);

					mField->addEntity(ent, pos, quat, scale);

					numberOfGrasses += 1;

					
					

				}

				//std::cout << "position : " << grassX << " " << grassY << std::endl;
				/*for (int g = 0; g < 3; g++) {
					Ogre::Vector3 pos = Ogre::Vector3(grassX, 1, grassY);
					Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Degree(g * 60), Ogre::Vector3::UNIT_Y);

					mField->addEntity(ent, pos, quat);
					
					numberOfGrasses += 1;
				}*/
			}
			
			

		}

	}

	std::cout << "Grasses rendered" << numberOfGrasses << std::endl;
	terrainObj->end();

	mField->build();
	// X
	//for (int i = 0; i < size.x; i++)
	//{
	//	// Y
	//	for (int j = 0; j < size.y; j++)
	//	{
	//		terrainObj->position
	//	}
	//}
	terrainObj->setCastShadows(false);
	Ogre::MeshPtr msh = terrainObj->convertToMesh("white_g");
	//msh.get()->buildEdgeList();
	oScnManager->getRootSceneNode()->createChildSceneNode()->attachObject(terrainObj);
	//Ogre::MeshPtr terrainMesh = terrainObj->convertToMesh("terrain");
	//terrainScnNode->attachObject(oScnManager->createEntity(terrainMesh));

}

void Monster::setHeightMap(Ogre::String heightMapImg, Ogre::String grassMapImg, int vertSize, int displacementFac , int grassDensity, float scale)
{
	Ogre::Image* imgManager = new Ogre::Image();
	imgManager->load(heightMapImg, "Images");

	Ogre::Image* grassImg = new Ogre::Image();
	grassImg->load(grassMapImg, "Images");

	std::cout << "Size : " << imgManager->getWidth() << " " << imgManager->getWidth() << std::endl;

	// Grass repositioning setup
	Ogre::Entity* ent = this->getMeshEntity("Grass.mesh");
	Ogre::StaticGeometry* mField = oScnManager->getStaticGeometry("stat_geo");
	mField->reset();
	int numberOfGrasses = 0;
	float grassScale = 0;

	// setup terrain
	terrainObj->clear();
	terrainObj->begin("myMat", Ogre::RenderOperation::OT_TRIANGLE_LIST, "Mesh_Materials");
	
	float displacement = 0.0f;

	// progress
	int loading = 0;
	int loaded = 0;

	for (int width = 0; width + vertSize < imgManager->getWidth() ; width++)
	{
		for (int height = 0; height + vertSize < imgManager->getHeight() ; height++)
		{
			displacement = imgManager->getColourAt(width, height, 0).r * displacementFac;
			
			int terrainX = width * vertSize;
			int terrainZ = height * vertSize;

			// terrain triangle 1
			terrainObj->position(terrainX, displacement, terrainZ);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(0, 0);

			displacement = imgManager->getColourAt(width, height + vertSize, 0).r * displacementFac;

			terrainObj->position(terrainX, displacement, terrainZ + vertSize);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			displacement = imgManager->getColourAt(width + vertSize , height , 0).r * displacementFac;

			terrainObj->position(terrainX + vertSize, displacement, terrainZ);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			// terrain triangle 2

			displacement = imgManager->getColourAt(width , height + vertSize, 0).r * displacementFac;

			terrainObj->position(terrainX, displacement, terrainZ + vertSize);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			displacement = imgManager->getColourAt(width + vertSize, height + vertSize, 0).r * displacementFac;

			terrainObj->position(terrainX + vertSize, displacement, terrainZ + vertSize);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);

			displacement = imgManager->getColourAt(width + vertSize, height, 0).r * displacementFac;

			terrainObj->position(terrainX + vertSize, displacement, terrainZ);
			terrainObj->normal(0, 1, 0);
			terrainObj->textureCoord(1, 1);
			
			// grass image scale;
			if (grassImg->getWidth() > width && grassImg->getHeight() > height) {
				grassScale = (scale * ((grassImg->getColourAt(width, height, 0).r) + 1));
			}
			else {
				grassScale = 1.0f;
			}
			
			// Grass re positioning
			for (int grassPerVert = 0; grassPerVert < grassDensity; grassPerVert++)
			{
				// generates number between -6.0 to 6.0 for the offset
				// Primitive way
				//double randomX = 1.0 - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
				//double randomY = 1.0 - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));

				//std::cout << "random " << randomX << " " << randomY << std::endl;

				double grassX = (terrainX) + Ogre::Math::RangeRandom(1.0f,2.0f);
				double grassY = (terrainZ) + Ogre::Math::RangeRandom(1.0f,2.0f);

				//std::cout << "position : " << grassX << " " << grassY << std::endl;
				for (int g = 0; g < 3; g++)
				{
					Ogre::Vector3 pos = Ogre::Vector3(grassX, displacement + 1, grassY);
					Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Degree(g * 60), Ogre::Vector3::UNIT_Y);
					Ogre::Vector3 gScale = Ogre::Vector3(1.0f, Ogre::Math::RangeRandom(1.0f,scale), 1.0f);
					
					//mField->addEntity(ent, pos, quat, gScale);

					numberOfGrasses += 1;
				}
			}

			//std::cout << "Loading : " << (int)(((loading + 0.0f) / (imgManager->getWidth() * imgManager->getHeight() + 0.0f)) * 100) << "% " << std::endl;
			if ((int)(((loading + 0.0f) / (imgManager->getWidth() * imgManager->getHeight() + 0.0f)) * 100) != loaded) {
				loaded = (int)(((loading + 0.0f) / (imgManager->getWidth() * imgManager->getHeight() + 0.0f)) * 100);
				std::cout << "Loading : " << loaded << "% " << std::endl;
			}
			loading += 1;

			
		}

	}
	std::cout << "building" << std::endl;
	//mField->build();
	std::cout << "Loaded" << std::endl;
	std::cout << "number of grasses rendered : " << numberOfGrasses << std::endl;
	terrainObj->end();

	

}

void setUpGrassBlade(int numOfGrass) {

	Ogre::ManualObject obj("GrassObject");
	obj.begin("GrasssMaterial",Ogre::RenderOperation::OT_TRIANGLE_LIST, "Mesh_Materials");

	for (int blades = 0; blades < numOfGrass; blades++)
	{																										// Width
		Ogre::Vector3 vec = Ogre::Quaternion(Ogre::Degree(blades * 60), Ogre::Vector3::UNIT_Y) * Ogre::Vector3(1, 0, 0);

		for (unsigned int j = 0; j < 4; j++) // each plane has 4 vertices
		{
			vec.y = j % 2 ? 0 : 2;// height
			obj.position(j < 2 ? Ogre::Vector3(-1, 1, -1) * vec : vec);
			obj.textureCoord(j < 2 ? 0 : 1, j % 2);

			// all normals point straight up
			obj.normal(0, 1, 0);
		}

		unsigned int off = blades * 4;
		// each plane consists of 2 triangles
		obj.triangle(off + 0, off + 3, off + 1);
		obj.triangle(off + 0, off + 2, off + 3);

	}

	obj.end();
	obj.convertToMesh("grass");

}

void Monster::createGrass()
{
	//setUpGrassBlade(3);
	
	//ent->setMaterial(Ogre::MaterialManager::getSingleton().getByName("GrassMaterial"));
	//Ogre::SceneNode* scn = oScnManager->getRootSceneNode()->createChildSceneNode();
	//scn->attachObject(ent);
	 
	// static Geometry - GPU INSTANCING GRASS
	
	Ogre::StaticGeometry* mField = oScnManager->createStaticGeometry("stat_geo");
	mField->setOrigin(Ogre::Vector3(0, 1, 0));

	Ogre::Entity* ent = this->getMeshEntity("Grass.mesh");
	//Ogre::Entity* ent = oScnManager->createEntity("grass");

	// get Terrain positions
	size_t vertex_count{}, indices_count;
	Ogre::Vector3* vertices;
	unsigned long* indices;

	Ogre::Mesh* terrainMesh = Ogre::MeshManager::getSingleton().getByName("terrain").get();
	

	for (size_t vert = 0; vert < vertex_count; vert++)
	{
		
//		mField->addEntity(ent, vertices[vert]);
	}


	/*for (int x = 0; x < 100; x++)
	{
		for (int z = 0; z < 100; z++)
		{
			for (int i = 0; i < 3; i++)
			{
				Ogre::Vector3 pos = Ogre::Vector3(x, 1, z + z);
				Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Degree(i * 60), Ogre::Vector3::UNIT_Y);
				mField->addEntity(ent, pos,quat);

			}
			
		}
	}*/

	//mField->build();
	


	

	//mInstanceNodes.clear();

}


void Monster::setupTestTrack()
{
	Ogre::Light* pLight = oScnManager->createLight("the_main_light", Ogre::Light::LT_DIRECTIONAL);
	Ogre::SceneNode* pLightNode = oScnManager->getRootSceneNode()->createChildSceneNode("pLightNodeScn");
	pLightNode->attachObject(pLight);
	pLightNode->setDirection(0, -1, 0);


	//Ogre::ResourceGroupManager::getSingleton().setWorldResourceGroupName("Assets");
	//Ogre::SceneNode* scn = oScnManager->getRootSceneNode()->createChildSceneNode("test_track");
	//scn->loadChildren("test_track.scene");

}


HWND* Monster::getHWND()
{
	HWND window = 0;
	window = FindWindowA(NULL,"OGRE_HORESESHOE");
	if (!window) {
		std::cout << std::endl << "Window was not found" << std::endl;
	}
	return &window;
}

void Monster::updateMonster()
{
	if (skySphere) {
		skySphere->setPosition(CameraNode->getPosition().x,CameraNode->getPosition().y - 2500, CameraNode->getPosition().z);
		
	}
	if (skyHighNode) {
		skyHighNode->setPosition(CameraNode->getPosition().x, CameraNode->getPosition().y - 2500 , CameraNode->getPosition().z);
		
	}

	this->windowUpdate();

	
}

void Monster::deleteScnNode(Ogre::SceneNode* scnNode)
{
	scnNode->removeAndDestroyAllChildren();
	oScnManager->destroySceneNode(scnNode);
}


//DEPRICATED OGRE'S SDL INPUT SYSTEM IS USED AS VULKAN DOESN'T PROVIDE HWND
void Monster::_setupSDL3(INT64 windowWidth, INT64 windowHeight, Ogre::String windowName) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	HWND window = 0;

	window = FindWindowA(NULL, "OGRE_HORESESHOE");

	if (!window) {
		std::cout << std::endl << "Window was not found" << std::endl;
	}

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, window);
	//SDL_SetPointerProperty(props, SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER, );
	//DEPRICATED
	//SDL_SetProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, getHWND());
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, windowName.c_str());
	//NOTWORKING
	//SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowWidth);
	//SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowHeight);

	// SDL WINDOW
	sdlWindow = SDL_CreateWindowWithProperties(props);
	SDL_SetWindowSize(sdlWindow, windowWidth, windowHeight);

}

void Monster::windowUpdate()
{
	// Fullscreen

	if (inputkeys->ALT_L_KEY || inputkeys->ALT_R_KEY) {
		if (inputkeys->ENTER_KEY) {
			SDL_SetWindowFullscreen(sdlWindow, !window_fullScreen);
		}
	
		
	}
	else {
		// add full screen toggle
		auto flag = SDL_GetWindowFlags(sdlWindow);
		auto is_fullscreen = flag & SDL_WINDOW_FULLSCREEN;
		if (is_fullscreen == SDL_WINDOW_FULLSCREEN) {
			//is fullscreen
			window_fullScreen = true;
		}
		else {
			window_fullScreen = false;
		}

	}
	

}

void Monster::_setupRTShader() {
	Ogre::RTShader::ShaderGenerator* rtGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	rtGen->addSceneManager(oScnManager);
}


void Monster::_loadResource()
{

}

Ogre::ManualObject* Monster::_createGrassBlade(float scaleX , float scaleY)
{
	Ogre::ManualObject* grassObj = oScnManager->createManualObject("GrassssObj");

	grassObj->begin("Blade_mat",Ogre::RenderOperation::OT_TRIANGLE_STRIP, "Mesh_Materials");

	// first Quad
	grassObj->position(0, 0, 0);
	grassObj->normal(1, 0, 0);
	grassObj->textureCoord(0, 0);

	grassObj->position(1 * scaleX, 0, 0);
	grassObj->normal(1, 0, 0);

	grassObj->position(0, 1 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	grassObj->position(1 * scaleX, 1 * scaleY, 0);
	grassObj->normal(1, 0, 0);


	// second
	grassObj->position(0, 2 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	grassObj->position(1 * scaleX, 2 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	// third
	grassObj->position(0.05 * scaleX, 3 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	grassObj->position(0.95 * scaleX, 3 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	// fourth
	grassObj->position(0.1 * scaleX, 4 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	grassObj->position(0.9 * scaleX, 4 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	// fifth
	grassObj->position(0.3 * scaleX, 5 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	grassObj->position(0.7 * scaleX, 5 * scaleY, 0);
	grassObj->normal(1, 0, 0);

	//tip
	grassObj->position(0.5 * scaleX, 5.5 * scaleY, 0);
	grassObj->normal(1, 0, 0);
	grassObj->textureCoord(1, 1);

	
	grassObj->end();

	Ogre::MeshPtr grassMsh = grassObj->convertToMesh("thisGrassMesh");
	Ogre::Entity* ent = oScnManager->createEntity(grassMsh);
	ent->setCastShadows(false);

	oScnManager->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	
	return grassObj;

	


}

void Monster::Shutdown()
{
	
}

void setObjRotation(Ogre::SceneNode* scnNode, Ogre::Vector3& rot)
{
	scnNode->pitch(Ogre::Degree(rot.x));
	scnNode->yaw(Ogre::Degree(rot.y));
	scnNode->roll(Ogre::Degree(rot.z));

}

Ogre::Vector3 getObjRotation(Ogre::SceneNode* scnNode) {
	return Ogre::Vector3(
		scnNode->getOrientation().getPitch().valueDegrees(),
		scnNode->getOrientation().getYaw().valueDegrees(),
		scnNode->getOrientation().getRoll().valueDegrees()

	);
}

void RSUS::readMaterial(Ogre::String matName)
{


	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(matName);

	Ogre::String fragProgramName = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getName();
	Ogre::String fragProgramFileName = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgram().get()->getSourceFile();
	Ogre::GpuProgramParametersPtr fragParam = mat.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

	Ogre::String vertProgramName = mat.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getName();
	Ogre::String vertProgramFileName = mat.get()->getTechnique(0)->getPass(0)->getVertexProgram().get()->getSourceFile();
	Ogre::GpuProgramParametersPtr vertParam = mat.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters();
	
	std::cout << " Frag Program Name : " << fragProgramName << std::endl;
	std::cout << " Frag Program File Name : " << fragProgramFileName << std::endl;
	std::cout << " Vert Program Name : " << vertProgramName << std::endl;
	std::cout << " Vert Program File Name : " << vertProgramFileName << std::endl;

	// Read the shader file
	ResourceHandler::GetInstance()->readShaderFiles(mat);

	std::vector<std::string>* fragshaderVar = ResourceHandler::GetInstance()->fragShaderVariables;
	std::vector<std::string>* vertshaderVar = ResourceHandler::GetInstance()->vertShaderVariables;

	rsusObj->fragVariables =  _initShaderValue(fragParam, fragshaderVar, fragProgramName);
	rsusObj->vertVariables =  _initShaderValue(vertParam, vertshaderVar, vertProgramName);

	
	// file verified
	std::cout << "file verified" << std::endl;

	rsusObj->fragShaderName = fragProgramName;
	rsusObj->fragShaderFileName = fragProgramFileName;
	rsusObj->vertShaderName = vertProgramName;
	rsusObj->vertShaderFileName = vertProgramFileName;
	rsusObj->materialName = matName;
	rsusObj->fragProgramPtr = fragParam;
	rsusObj->vertProgramPtr = vertParam;
	

	
}

void RSUS::updateFragParameterInt(Ogre::String parameterName, int val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateFragParameterFloat(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val[0]);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val[0] << std::endl;
	}
	
}

void RSUS::updateFragParameterFloat2(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector2(val[0], val[1]));
	}
	catch (...) {
		std::cout << "Invalid Float2 Input For : " << parameterName << " Value : " << Ogre::Vector2(val[0], val[1]) << std::endl;
	}
	
}

void RSUS::updateFragParameterFloat3(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector3(val[0], val[1], val[2]));
	}
	catch (...) {
		std::cout << "Invalid Float3 Input For : " << parameterName << " Value : " << Ogre::Vector3(val[0], val[1], val[2]) << std::endl;
	}
	
}

void RSUS::updateFragParameterFloat4(Ogre::String parameterName, Ogre::Vector4 val)
{
	try {
		this->rsusObj->fragProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Float4 Input For : " << parameterName << " Value : " << val << std::endl;
	}
	
}

void RSUS::updateVertParameterInt(Ogre::String parameterName, int val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

void RSUS::updateVertParameterFloat(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, val[0]);
	}
	catch (...) {
		std::cout << "Invalid Float Input For : " << parameterName << " Value : " << val[0] << std::endl;
	}
}

void RSUS::updateVertParameterFloat2(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector2(val[0], val[1]));
	}
	catch (...) {
		std::cout << "Invalid Float2 Input For : " << parameterName << " Value : " << Ogre::Vector2(val[0], val[1]) << std::endl;
	}
}

void RSUS::updateVertParameterFloat3(Ogre::String parameterName, float* val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, Ogre::Vector3(val[0], val[1], val[2]));
	}
	catch (...) {
		std::cout << "Invalid Float3 Input For : " << parameterName << " Value : " << Ogre::Vector3(val[0], val[1], val[2]) << std::endl;
	}
}

void RSUS::updateVertParameterFloat4(Ogre::String parameterName, Ogre::Vector4 val)
{
	try {
		this->rsusObj->vertProgramPtr.get()->setNamedConstant(parameterName, val);
	}
	catch (...) {
		std::cout << "Invalid Float4 Input For : " << parameterName << " Value : " << val << std::endl;
	}
}

RSUS* RSUS::GetInstance()
{
	// locks storage 
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new RSUS();
	}
	return pinstance_;
}

ShaderVar RSUS::_putShaderValue(std::string valueStr)
{

	ShaderVar var = ShaderVar();

	std::string value = "";
	int floatPos = 0;

	std::cout << "Input : " << valueStr << std::endl;

	if (!valueStr.empty())
	{
		if (valueStr.at(0) == '0') {
			std::cout << "value int : " << valueStr.substr(2, valueStr.size()) << std::endl;
			*var.varInt = std::stoi(valueStr.substr(2, valueStr.size()));
			var.varType = ShaderVarType::INTEGER;
		}
		else if (valueStr.at(0) == '1') 
		{
			std::cout << "value float : " << valueStr.substr(2, valueStr.size()) << std::endl;
			*var.varFloat = std::stof(valueStr.substr(2, valueStr.size()));
			var.varType = ShaderVarType::FLOAT0;
		}
		else if (valueStr.at(0) == '2')
		{
			for (int i = 2; i < valueStr.size(); i++)
			{
				if (valueStr.at(i) != ' ') {
					value += valueStr.at(i);
				}
				else {
					std::cout << "value float2 : " << floatPos << " : " << value << std::endl;
					var.varFloat2[floatPos] = std::stof(value);
					floatPos += 1;
					value = "";
				}
			}
			std::cout << "value float2 : " << floatPos << " : " << value << std::endl;
			var.varFloat2[floatPos] = std::stof(value);
			var.varType = ShaderVarType::FLOAT2;
		}
		else if (valueStr.at(0) == '3')
		{
			for (int i = 2; i < valueStr.size(); i++)
			{
				if (valueStr.at(i) != ' ') {
					value += valueStr.at(i);
				}
				else {
					std::cout << "value float3 : " << floatPos << " : " << value << std::endl;
					var.varFloat3[floatPos] = std::stof(value);
					floatPos += 1;
					value = "";
				}
			}
			std::cout << "value float3 : " << floatPos << " : " << value << std::endl;
			var.varFloat3[floatPos] = std::stof(value);
			var.varType = ShaderVarType::FLOAT3;
		}
		else if (valueStr.at(0) == '4')
		{
			for (int i = 2; i < valueStr.size(); i++)
			{
				if (valueStr.at(i) != ' ') {
					value += valueStr.at(i);
				}
				else {
					std::cout << "value float4 : " << floatPos << " : " << value << std::endl;
					var.varFloat4[floatPos] = std::stof(value);
					floatPos += 1;
					value = "";
				}
			}
			std::cout << "value float4 : " << floatPos << " : " << value << std::endl;
			var.varFloat4[floatPos] = std::stof(value);
			var.varType = ShaderVarType::FLOAT4;
		}
		else {
			std::cout << "Unknown Type in Save file" << std::endl;
		}
	}
	else {
		std::cout << "Invalid save file contents" << std::endl;
		// throw exception here
		throw ShaderSaveFileInconsistent();
	}

	return var;
	
}

std::vector<ShaderVar> RSUS::_initShaderValue(Ogre::GpuProgramParametersPtr params, Ogre::StringVector* vec , Ogre::String filename)
{

	std::vector<ShaderVar> variables = std::vector<ShaderVar>();

	// check if save file is found
	bool hasSave = ResourceHandler::GetInstance()->fileExists(filename);
	bool inSave = false;

	for (int i = 0; i < vec->size(); i++) {
		std::cout << "This :: " << vec->at(i) << std::endl;
	}

	std::cout << "Graphics File : " << filename << " Found : " << hasSave << std::endl;

	

	//Validate output
	// must be not null
	std::string vari = "";
	if (vec) {
		// must be even
		if (vec->size() % 2 == 0)
		{
			for (int i = 0; i < vec->size(); i += 2)
			{
				// List if validated and correct 
				// Cross Check variables with the Parameter file
				vari = vec->at(i + 1);
				std::cout <<"Output Type : " <<  vec->at(i) << std::endl;
				std::cout << "Output name : " << vec->at(i + 1) << std::endl;

				if (vec->at(i + 1) == "cameraPos") {
					continue;
				}

				//std::cout << "Output :: " << vari << std::endl;
				ShaderVar var = ShaderVar();
				
				try
				{
					var = _putShaderValue(ResourceHandler::GetInstance()->readFromFile(vari, filename));
				}
				catch (const std::exception &e)
				{
					std::cout << "ERROR FOUND : ";
					e.what();
					hasSave = false;
				}
				

				
				// add save update
				if (hasSave) {
					
					//std::cout << "value" << var << std::endl;

					// Input values to shader
					try {
						switch (var.varType)
						{
						case ShaderVarType::INTEGER:

							params.get()->setNamedConstant(vari, *var.varInt);


							break;
						case ShaderVarType::FLOAT0:
							params.get()->setNamedConstant(vari, *var.varFloat);
							break;
						case ShaderVarType::FLOAT2:
							params.get()->setNamedConstant(vari, Ogre::Vector2(var.varFloat2[0], var.varFloat2[1]));
							break;
						case ShaderVarType::FLOAT3:
							params.get()->setNamedConstant(vari,
								Ogre::Vector3(var.varFloat3[0],
									var.varFloat3[1],
									var.varFloat3[2]
								));
							break;
						case ShaderVarType::FLOAT4:
							params.get()->setNamedConstant(vari,
								Ogre::Vector4(var.varFloat4[0],
									var.varFloat4[1],
									var.varFloat4[2],
									var.varFloat4[3]
								));
							break;
						default:
							std::cout << "Invalid type found" << std::endl;
							break;
						}

					}
					catch (const std::exception& e) {
						std::cout << filename << " : Save file invalid or old." << std::endl;
					}


				}

				else {
					try
					{

						if (vec->at(i) == "0") {
							std::cout << "int" << std::endl;
							params.get()->setNamedConstant(vari, 0);
							var.varType = ShaderVarType::INTEGER;
							var.varInt = new int(0);
						}
						else if (vec->at(i) == "1")
						{
							std::cout << "float" << std::endl;
							params.get()->setNamedConstant(vari, 0.0f);
							var.varType = ShaderVarType::FLOAT0;
							var.varFloat = new float(0.0);
						}
						else if (vec->at(i) == "2")
						{
							std::cout << "float2" << std::endl;
							params.get()->setNamedConstant(vari, Ogre::Vector2(0.0, 0.0));

							var.varType = ShaderVarType::FLOAT2;

						}
						else if (vec->at(i) == "3")
						{
							std::cout << "float3" << std::endl;
							params.get()->setNamedConstant(vari, Ogre::Vector3(0.0, 0.0, 0.0));
							var.varType = ShaderVarType::FLOAT3;

						}
						else if (vec->at(i) == "4")
						{
							std::cout << "float4" << std::endl;
							params.get()->setNamedConstant(vari, Ogre::Vector4(0.0, 0.0, 0.0, 0.0));
							var.varType = ShaderVarType::FLOAT4;

						}
					}

					catch (const std::exception& e)
					{
						std::cout << e.what() << std::endl;
						std::cout << "Found invalid Parameter : " << vec->at(i + 1) << " " << vec->at(i) << std::endl;
					}
				}

				var.varName = vec->at(i + 1);
				variables.push_back(var);

			}

		}
	}
	return variables;
}
