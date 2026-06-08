#include "Monster.h"

// Multi-Thread shit
RSUS* RSUS::pinstance_{ nullptr };
std::mutex RSUS::mutex_;

 Ogre::MovableObject* Monster::RayCastFromPoint()
{
	// create a ray (of hope)
	Ogre::Ray ray = Ogre::Ray(cam->getRealPosition(), cam->getRealDirection());
	
	// null cheak
	if (mRayScnQuery) {

		// set Ray
		mRayScnQuery->setRay(ray);

		// get a vector of hits
		Ogre::RaySceneQueryResult result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResultEntry resultEntry;
		
		if (result.size() > 0) { 
			// congrats hit!!
			resultEntry = result[0];
			for (int i = 0; i < result.size(); i++)
			{
				if (result[i].distance < resultEntry.distance) {
					
					resultEntry = result[i];
					
				}
			}
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

Monster::Monster()
{
	

	
	
}



void Monster::InitMonster(Ogre::Root* root, Ogre::RenderWindow* rWin, Ogre::OverlaySystem* overlay, Ogre::ImGuiOverlay* imguiOverlay_p) {

	oRoot = root;

	renderWindow = rWin;

	oScnManager = root->createSceneManager();

	_setupRTShader();

	oScnManager->setAmbientLight(Ogre::ColourValue(0.53, 0.2, 0.12));

	oScnManager->addRenderQueueListener(overlay);


	// raycast setup
	mRayScnQuery = oScnManager->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

	mRayScnQuery->setQueryMask(~QueryMask::SKY & ~QueryMask::GRID);

	imguiOverlay = imguiOverlay_p;
	
	
	_setupSDL3(1600,900,"Psycho");

	SDL_GetWindowSize(sdlWindow, windowProp->window_width, windowProp->window_height);

	// Add Camera
	this->addCamera(MAIN_CAMERA_NAME, Ogre::Vector3(5, 5, 5));

	
}

void Monster::setShadowTechnique()
{

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(OGRE_MATERIAL_GROUP)
		&& Ogre::MaterialManager::getSingleton().resourceExists("ShadowCasterV5", OGRE_MATERIAL_GROUP)
		&& Ogre::MaterialManager::getSingleton().resourceExists("ShadowReceiverV5", OGRE_MATERIAL_GROUP)
		)
	{
		ToastComponent::GetInstance()->addMessage("ShadowCaster and ShadowReceiver found. Initalizing ShadowTexture...");

		Ogre::MaterialPtr shadowCastMat = Ogre::MaterialManager::getSingleton().getByName("ShadowCasterV5");
		Ogre::MaterialPtr shadowReceiverMat = Ogre::MaterialManager::getSingleton().getByName("ShadowReceiverV5");

		oScnManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);

		oScnManager->setShadowTextureCasterMaterial(shadowCastMat);
		oScnManager->setShadowTextureReceiverMaterial(shadowReceiverMat);
		// TODO RESOURCES ARE SETUP LATER THAN MONSTER's INIT. MAKE RESOURCE LOAD FIRST IN THE PIPELINE.

		ToastComponent::GetInstance()->addMessage("Initalized ShadowTexture.");

	}
	else {
		ToastComponent::GetInstance()->addMessage("Error setting up shadow texture Technique. using fallback.");

		oScnManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

	}

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



Ogre::SceneNode* Monster::loadMeshScnNode(
	Ogre::String scnNodeType,
	Ogre::String scnNodeName,
	Ogre::String objectname, 
	Ogre::String meshName, 
	Ogre::String groupName, 
	bool castShadow
)
{

	std::cout << "Mesh : " << meshName << std::endl;
	
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().load(meshName, groupName);

	

	//msh.get()->buildTangentVectors();

	Ogre::SceneNode* scnNode;

	if (oScnManager->hasSceneNode(scnNodeName))
	{
		std::cout << "SceneNode Not found" << std::endl;
		
	}

	scnNode = oScnManager->getSceneNode(scnNodeName);

	Ogre::Entity* ent = oScnManager->createEntity(objectname, msh);
	//ent->setMaterial(Ogre::MaterialManager::getSingletonPtr()->getByName("monke_blue"));
	ent->setCastShadows(castShadow);
	scnNode->attachObject(ent);
	

	return scnNode;
}


Ogre::SceneNode* Monster::loadMeshScnNode(
	Ogre::String scnNodeType,
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
	if (oScnManager->hasLight(lightName)) {
		std::cout << "Light exists" << std::endl;
		return;
	}

	Ogre::Light* light = oScnManager->createLight(lightName);
	light->setType(type);	
	light->setPowerScale(powerScale);
	light->setCastShadows(true);
	Ogre::SceneNode* lightNode = oScnManager->getRootSceneNode()->createChildSceneNode(lightName + "_scn");
	lightNode->attachObject(light);
	lightNode->setDirection(direction);
	std::cout << " position: " << position.x << " " << position.y << " " << position.z << std::endl;
	std::cout << " direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
	std::cout << "type : " << (int)type << std::endl;

}

void Monster::addMainDirectionalLight(std::string lightName, Ogre::Vector3 dir , int power)
{

	if (mdrl->directionalLight && mdrl->lightScnNode) {

		mdrl->directionalLight->setPowerScale(power);
		mdrl->lightScnNode->setDirection(dir);

	}
	else {

		Ogre::Light* light = oScnManager->createLight(lightName);

		light->setType(Ogre::Light::LT_SPOTLIGHT);
		light->setCastShadows(true);

		light->setPowerScale(power);
		

		Ogre::SceneNode* mainLightScnNode = oScnManager->getRootSceneNode()->createChildSceneNode(lightName + "_scn");

		mainLightScnNode->attachObject(light);
		mainLightScnNode->setDirection(dir);
		mainLightScnNode->setPosition(0, 25, -10);

		mdrl->directionalLight = light;
		mdrl->lightScnNode = mainLightScnNode;
	}

	std::cout << "LIGHT DIRECTION : " << mdrl->directionalLight->getDerivedDirection().x << " " << mdrl->directionalLight->getDerivedDirection().y << " "
		<< mdrl->directionalLight->getDerivedDirection().z << std::endl;


}

void Monster::setupFonts()
{
	
	/*ImGuiIO& io = ImGui::GetIO();
	std::vector<std::filesystem::path>* fontsPath = ResourceHandler::GetInstance()->fetchResourceGroupVecByIndex(ResourceGroup::RESOURCE_MASTER_GROUP_INDEX::FONT);
	if (fontsPath)
	{
		for (size_t i = 0; i < fontsPath->size(); i++)
		{
			if (fontsPath->at(i).filename() == "UbuntuMono-Bold.ttf")
			{
				fonts->title = io.Fonts->AddFontFromFileTTF(fontsPath->at(i).string().c_str(), 28.0);
				IM_ASSERT(this->titleFont != nullptr);
				fonts->title->FontSize = 28;

			}

		}
	}*/
}


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



void Monster::addResourceGroup(std::string groupName)
{
	if (!this->resourceGroupExists(groupName))
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(groupName);
	}
}

void Monster::addOgreResourceLocation(std::string path_p, std::string OgreResourceGroup)
{
	if (std::filesystem::exists(path_p))
	{
		ToastComponent::GetInstance()->addMessage("Invalid Path was being added to Resource Group : " + OgreResourceGroup + " : " + path_p);
	}
	if (!this->resourceGroupExists(OgreResourceGroup))
	{
		Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(OgreResourceGroup);
	}
	if (!this->resourceLocationExists(path_p))
	{
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(path_p, "FileSystem", OgreResourceGroup);

	}
	
}

void Monster::initalizeResourceGroup(std::string OgreResourceGroup)
{
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceGroupExists(OgreResourceGroup))
	{
		Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(OgreResourceGroup);
	}
	Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(OgreResourceGroup);
}

bool Monster::resourceExists(std::string resName, std::string groupName)
{
	return Ogre::ResourceGroupManager::getSingleton().resourceExists(groupName,resName);
}

bool Monster::resourceGroupExists(std::string groupName)
{
	return Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(groupName);
}

bool Monster::resourceLocationExists(std::string path_p)
{
	return Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(path_p);
}

bool Monster::resourceEntityExists(std::string entityName)
{
	return oScnManager->hasEntity(entityName);
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
	oRoot->renderOneFrame();

	if (skySphere) {
		skySphere->setPosition(CameraNode->getPosition().x,CameraNode->getPosition().y - 2500, CameraNode->getPosition().z);
		
	}
	if (skyHighNode) {
		skyHighNode->setPosition(CameraNode->getPosition().x, CameraNode->getPosition().y - 2500 , CameraNode->getPosition().z);
		
	}



	
}

void Monster::deleteScnNode(Ogre::SceneNode* scnNode)
{
	scnNode->removeAndDestroyAllChildren();
	oScnManager->destroySceneNode(scnNode);

}


//THE SETUP SDL3 Now Works with vulkan!
void Monster::_setupSDL3(INT64 windowWidth, INT64 windowHeight, Ogre::String windowName) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	HWND window = 0;

	window = FindWindowA(NULL, INSTANCE_NAME);

	if (!window) {
		std::cout << std::endl << "Window was not found" << std::endl;
		return;
	}

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, window);
	SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, windowName.c_str());


	// SDL WINDOW
	sdlWindow = SDL_CreateWindowWithProperties(props);
	SDL_SetWindowSize(sdlWindow, windowWidth, windowHeight);

}


void Monster::_setupRTShader() {
	Ogre::RTShader::ShaderGenerator* rtGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	rtGen->setTargetLanguage("glslang");
	rtGen->addSceneManager(oScnManager);
	
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


// OLDER FUNCTIONS


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









