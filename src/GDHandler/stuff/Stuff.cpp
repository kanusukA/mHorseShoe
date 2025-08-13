#include "Stuff.h"



void Stuff::setSceneNode(Ogre::SceneNode* scnNode) {
	this->scnNode = scnNode;
}

Ogre::MovableObject* Stuff::getEntity()
{
	return this->scnNode->getAttachedObject(0);
}

void Stuff::setName(Ogre::String nm) {
	name = nm;
}

void Stuff::showRenderer(bool value)
{
	this->scnNode->setVisible(value);
}

void Stuff::setCastShadow(bool value)
{
	getEntity()->setCastShadows(value);
}

void Stuff::moveStuffBy(Ogre::Vector3 addPosition)
{
	// not defined 
}



PxVec3* mtkVectors(Ogre::Vector3* vec, int vec_size)
{
	PxVec3* physxVec = new PxVec3[vec_size];

	for (int i = 0; i < vec_size; i++)
	{
		physxVec[i] = mtkPositionVec3(vec[i]);
	}

	return physxVec;

}

Ogre::Vector3 ktmPositionVec3(PxVec3 position) {
	return Ogre::Vector3(position.x, position.y, position.z);
}

Ogre::Quaternion ktmOrientation(PxQuat quat) {
	return Ogre::Quaternion(quat.w, quat.x, quat.y, quat.z);
}

PxVec3 mtkPositionVec3(Ogre::Vector3 position)
{
	return PxVec3(position.x,position.y,position.z);
}

PxQuat mtkOrientation(Ogre::Quaternion quat)
{
	return PxQuat(quat.w,quat.x,quat.y,quat.z);
}



void StuffStatic::setPxRigidStatic(PxRigidStatic* staActor) {
	sActor = staActor;
}

void StuffStatic::setRotation(Ogre::Vector3 rot)
{
	// do quaternions
	setObjRotation(this->scnNode, rot);

	this->sActor->setGlobalPose(PxTransform(mtkOrientation(this->scnNode->getOrientation())));

}

void StuffStatic::setPosition(Ogre::Vector3 pos)
{
	this->sActor->setGlobalPose(PxTransform(mtkPositionVec3(pos), this->sActor->getGlobalPose().q));
	this->scnNode->setPosition(pos);
}

void StuffStatic::moveStuffBy(Ogre::Vector3 addPos)
{
	this->setPosition(this->scnNode->getPosition() + addPos);
}

void StuffDynamic::setPxRigidDynamic(PxRigidDynamic* dynActor) {
	sActor = dynActor;
}

bool StuffDynamic::update(float deltaTime) {

	scnNode->setPosition(ktmPositionVec3(this->sActor->getGlobalPose().p));
	scnNode->setOrientation(ktmOrientation(this->sActor->getGlobalPose().q));

	return true;
}

void StuffDynamic::setRotation(Ogre::Vector3 rot)
{
	this->sActor->setGlobalPose(PxTransform(this->sActor->getGlobalPose().p, mtkOrientation(this->scnNode->getOrientation())));
	this->update(0.1f);
}

void StuffDynamic::setPosition(Ogre::Vector3 pos)
{
	this->sActor->setGlobalPose(PxTransform(mtkPositionVec3(pos),this->sActor->getGlobalPose().q));
	this->update(0.1f);
}

void StuffDynamic::moveStuffBy(Ogre::Vector3 addPos)
{
	this->setPosition(this->scnNode->getPosition() + addPos);
}

void StuffMesh::setRotation(Ogre::Vector3 rot)
{
	setObjRotation(this->scnNode, rot);
}

void StuffMesh::setPosition(Ogre::Vector3 pos)
{
	this->scnNode->setPosition(pos);
}

void StuffMesh::moveStuffBy(Ogre::Vector3 addPos)
{
	this->setPosition(this->scnNode->getPosition() + addPos);
}


void StuffHandler::updateVisualDebug()
{

	
	const PxDebugLine* lines = kint->getDebugLines();
	int nbLines = kint->getNbDebugLine();
	if(nbLines > 0) {
		this->debugLines->beginUpdate(0);
		
		for (int i = 0; i < nbLines -1; i++)
		{
			this->debugLines->position(ktmPositionVec3(lines[i].pos0));
			this->debugLines->position(ktmPositionVec3(lines[i].pos1));
			
		}
		this->debugLines->end();
	}
	
}

// Add StuffMesh Object
void StuffHandler::_addObject(std::string objName, std::string meshName, Ogre::Vector3 position, Ogre::Vector3 rotation, bool castShadows)
{

	// Doesn't have physx as its mesh only
	StuffMesh* stuffMesh = new StuffMesh();

	// Add Resource in Ogre and Add to Stuff
	Ogre::SceneNode* monsterNode = this->monster->loadMeshScnNode(objName, meshName,"Render_Mesh",castShadows);
	monsterNode->getUserObjectBindings().setUserAny(stuffMesh);

	
	stuffMesh->setName(objName);
	stuffMesh->setSceneNode(monsterNode);
	stuffMesh->showRenderer(true);

	stuffMesh->setPosition(position);
	stuffMesh->setRotation(rotation);

	stuffMesh->setName(objName);

	stuffMesh->indexId = stuffMeshes->size();
	stuffMeshes->push_back(stuffMesh);


}

void StuffHandler::_addObjectDynamic(std::string objName, std::string meshName, std::string colliderName, Ogre::Vector3 position, Ogre::Vector3 rotation, StuffType physicsType, float mass, Ogre::Vector3 colliderSize)
{

	// Create StuffDynamic
	StuffDynamic* stuffDynamic = new StuffDynamic();

	PxGeometry* geo;

	size_t vertex_count, index_count;
	Ogre::Vector3* vertices;
	unsigned long* indices;

	Ogre::SceneNode* monsterNode = this->monster->loadMeshScnNode(objName, meshName);
	monsterNode->getUserObjectBindings().setUserAny(stuffDynamic);

	if (colliderName == "box") {
		geo = new PxBoxGeometry(mtkPositionVec3(colliderSize));
	}
	else {
		this->monster->getMeshInfoFromColName(colliderName,
			vertex_count,
			index_count,
			vertices,
			indices,
			monsterNode->getPosition(),
			monsterNode->getOrientation(),
			monsterNode->getScale()
		);

		PxConvexMesh* convexMesh = kint->createConvexCollisionMesh(vertex_count, mtkVectors(vertices,vertex_count));
		geo = new PxConvexMeshGeometry(convexMesh);

		/*PxTriangleMesh* triMesh = kint->createTriangleCollisionMesh(vertex_count, mtkVectors(vertices, vertex_count), index_count, indices);
		geo = new PxTriangleMeshGeometry(triMesh);*/
	}


	physx::PxRigidDynamic* dynamic = this->kint->createRigidBody(objName, PxTransform(PxVec3(0,0,0)), mass, geo);

	
	stuffDynamic->setSceneNode(monsterNode);
	stuffDynamic->setPxRigidDynamic(dynamic);

	stuffDynamic->setPosition(position);
	stuffDynamic->setRotation(rotation);

	stuffDynamic->setName(objName);

	stuffDynamic->indexId = stuffDynamics->size();
	this->stuffDynamics->push_back(stuffDynamic);

	

}

void StuffHandler::_addObjectStatic(std::string objName, std::string meshName, std::string colliderName, Ogre::Vector3 position, Ogre::Vector3 rotation, StuffType physicsType, Ogre::Vector3 colliderSize)
{

	StuffStatic* stuffStatic = new StuffStatic();
	Ogre::SceneNode* monsterNode = this->monster->loadMeshScnNode(objName, meshName);
	monsterNode->getUserObjectBindings().setUserAny(stuffStatic);

	PxGeometry* geo;
	size_t vertex_count, index_count;
	Ogre::Vector3* vertices;
	unsigned long* indices;

	if (colliderName == "box") {
		geo = new PxBoxGeometry(mtkPositionVec3(colliderSize));
	}
	else {
		this->monster->getMeshInfoFromColName(colliderName,
			vertex_count,
			index_count,
			vertices,
			indices,
			monsterNode->getPosition(),
			monsterNode->getOrientation(),
			monsterNode->getScale()
		);

		PxConvexMesh* convexMesh = kint->createConvexCollisionMesh(vertex_count, mtkVectors(vertices, vertex_count));
		geo = new PxConvexMeshGeometry(convexMesh);

		/*PxTriangleMesh* triMesh = kint->createTriangleCollisionMesh(vertex_count, mtkVectors(vertices, vertex_count), index_count, indices);
		geo = new PxTriangleMeshGeometry(triMesh);*/
	}
	
	physx::PxRigidStatic* pxStatic = this->kint->createStaticBody(objName, PxTransform(mtkPositionVec3(position), physx::PxQuat()), geo);

	stuffStatic->setPxRigidStatic(pxStatic);
	stuffStatic->setSceneNode(monsterNode);

	stuffStatic->setPosition(position);
	stuffStatic->setRotation(rotation);

	stuffStatic->setName(objName);

	stuffStatic->indexId = stuffStatics->size();
	this->stuffStatics->push_back(stuffStatic);

	std::cout << "position : " << stuffStatic->sActor->getGlobalPose().p.x << " " << stuffStatic->sActor->getGlobalPose().p.y << " " << stuffStatic->sActor->getGlobalPose().p.z << std::endl;

	

}

void StuffHandler::deleteObj(Stuff* stuff)
{

	monster->deleteScnNode(stuff->scnNode);

	switch (stuff->type) {

	case STUFF_DYNAMIC:
		this->stuffDynamics->at(stuff->indexId)->sActor->release();
		
		this->stuffDynamics->erase(stuffDynamics->begin() + stuff->indexId);
		break;
	case  STUFF_MESH_ONLY:
		this->stuffMeshes->erase(stuffMeshes->begin() + stuff->indexId);
		break;
	case STUFF_STATIC:
		this->stuffStatics->erase(stuffStatics->begin() + stuff->indexId);
		break;

	}
}


void StuffHandler::addObject(std::string objName, 
	std::string meshName, 
	std::string colliderName,
	Ogre::Vector3 position, 
	Ogre::Vector3 rotation, 
	StuffType physicsType, 
	float mass, 
	Ogre::Vector3 colliderSize,
	bool duplicate,
	bool castShadows
)
{
	if (!duplicate) {
		lastObject->objName = objName;
	}
	lastObject->meshName = meshName;
	lastObject->colliderName = colliderName;
	lastObject->position = position;
	lastObject->rotation = rotation;
	lastObject->physicsType = physicsType;
	lastObject->mass = mass;
	lastObject->colliderSize = colliderSize;

	switch (physicsType)
	{
	case STUFF_DYNAMIC:
		this->_addObjectDynamic(objName, meshName, colliderName, position, rotation, physicsType, mass, colliderSize);
		break;
	case STUFF_STATIC:
		this->_addObjectStatic(objName, meshName, colliderName, position, rotation, physicsType, colliderSize);
		break;
	case STUFF_MESH_ONLY:
		this->_addObject(objName, meshName, position, rotation,castShadows);
		break;
	default:
		break;
	}
}

void StuffHandler::addGrass(Ogre::Real numOfGrass)
{
	StuffMesh* stuffMesh = new StuffMesh;
	Ogre::Entity* ent = monster->getMeshEntity("Grass.mesh");
	ent->setMaterial(Ogre::MaterialManager::getSingleton().getByName("GrasssMaterial"));
	int ref = 0;
	
	for (int i = 0; i < numOfGrass; i++)
	{
		for (int j = 0; j < numOfGrass; j++)
		{
			Ogre::SceneNode* parentNode = monster->loadMeshScnNode("Grass " + std::to_string(ref),"Grass.mesh");
			parentNode->setPosition(i, 2, j + j);
			ref += 1;
		}
	}
	
	//stuffMesh->scnNode = parentNode;
	//this->stuffMeshes->push_back(stuffMesh);

}

void StuffHandler::addLastObject()
{
	std::string name = lastObject->objName +  std::to_string(stuffDynamics->size() + stuffMeshes->size() + stuffStatics->size());

	if(lastObject){
		this->addObject(
			name,
			lastObject->meshName,
			lastObject->colliderName,
			lastObject->position,
			lastObject->rotation,
			lastObject->physicsType,
			lastObject->mass,
			lastObject->colliderSize,
			true
		);
	}
}

void StuffHandler::setHeightMap(std::string loc, std::string grassMapLoc, float displacement, int vertSize , int grassDensity, float scale)
{
	monster->setHeightMap(loc,grassMapLoc, vertSize, displacement,grassDensity, scale);
}

void StuffHandler::addMDRL(std::string name, Ogre::Vector3& direction, int powerScale)
{
	monster->addMainDirectionalLight(name, direction, powerScale);
}

void StuffHandler::addLight(std::string name, Ogre::Vector3& position, Ogre::Vector3& direction, int powerScale, Ogre::Light::LightTypes type)
{
	monster->addLight(name, type, powerScale, position, direction);
}

void StuffHandler::showDebugPhysxMeshes()
{

	

	showingColliderDebug = !showingColliderDebug;
	kint->setGlobalVisualization(showingColliderDebug);

	const PxDebugLine* lines = kint->getDebugLines();

	

	if(debugLines){
		this->debugScnNode->setVisible(showingColliderDebug);
	}
	else {
		debugLines = new Ogre::ManualObject("mo");
		debugLines->begin("Blade_mat", Ogre::RenderOperation::OT_LINE_LIST, "Render_Mesh");
		
		for (int i = 0; i < kint->getNbDebugLine(); i++)
		{
			debugLines->position(ktmPositionVec3(lines[i].pos0));
			debugLines->normal(0, 1, 0);
			debugLines->textureCoord(0, 0);

			debugLines->position(ktmPositionVec3(lines[i].pos1));
			debugLines->normal(0, 1, 0);
			debugLines->textureCoord(1, 1);
		}
		debugLines->end();

		this->debugScnNode = monster->addManualObject("this_moze", debugLines);
		
		//std::cout << "lines : " << kint->getNbDebugLine();
	}

}

void StuffHandler::showOgreRendering()
{	
	showingOgreRendering = !showingOgreRendering;
	for (int i = 0; i < this->stuffDynamics->size(); i++)
	{
		this->stuffDynamics->at(i)->scnNode->setVisible(showingOgreRendering);
	}
	for (int i = 0; i < this->stuffStatics->size(); i++)
	{
		this->stuffStatics->at(i)->scnNode->setVisible(showingOgreRendering);
	}
	for (int i = 0; i < this->stuffMeshes->size(); i++)
	{
		this->stuffMeshes->at(i)->scnNode->setVisible(showingOgreRendering);
	}
}

void StuffHandler::update(float deltaTime)
{

	this->updateSelectedPos();


	if (InputHandler::GetInstance()->getInputKeys()->SPACE_KEY)
	{
		this->addLastObject();
	}

	for (int i = 0; i < this->stuffDynamics->size(); i++)
	{
		this->stuffDynamics->at(i)->update(deltaTime);
	}
	if (showingColliderDebug) {
		this->updateVisualDebug();
	}

	// update rayCast from monster
	Ogre::MovableObject* rayObj = monster->RayCastFromPoint();
	if (rayObj) {

		*this->selectedObj->selectingName = rayObj->getParentSceneNode()->getName();

		if(InputHandler::GetInstance()->getInputKeys()->MOUSE_LEFT_CLICK > 0){

			Ogre::Any any = rayObj->getParentSceneNode()->getUserObjectBindings().getUserAny();
			
			


			if (any.type() == typeid(StuffDynamic*)) {

				
				this->selectedObj->selectedStuff = Ogre::any_cast<StuffDynamic*>(any);
				this->selectedObj->type = STUFF_DYNAMIC;
				
				*this->selectedObj->shadowCasting = selectedObj->selectedStuff->getEntity()->getCastShadows();
				
			}
			else if (any.type() == typeid(StuffStatic*)) {
				
				this->selectedObj->selectedStuff = Ogre::any_cast<StuffStatic*>(any);
				this->selectedObj->type = STUFF_STATIC;
					
				
				*this->selectedObj->shadowCasting = selectedObj->selectedStuff->getEntity()->getCastShadows();
				
			}
			else if (any.type() == typeid(StuffMesh*)) {
				
				this->selectedObj->selectedStuff = Ogre::any_cast<StuffMesh*>(any);
				this->selectedObj->type = STUFF_MESH_ONLY;
				
				*this->selectedObj->shadowCasting = selectedObj->selectedStuff->getEntity()->getCastShadows();
			}
			
		}

	}
	else {
		*this->selectedObj->selectingName = "";
	}

	

}

void StuffHandler::updateSelectedPos()
{


	if (this->selectedObj->type) {
		//set type
		if (inputKeys->UP_KEY)
		{
			this->selectedObj->selectedStuff->moveStuffBy(Ogre::Vector3(0, 1 - inputKeys->ALT_L_KEY, 0 + inputKeys->ALT_L_KEY));
		}
		if (inputKeys->DOWN_KEY)
		{
			this->selectedObj->selectedStuff->moveStuffBy(Ogre::Vector3(0, -1 + inputKeys->ALT_L_KEY, 0 - inputKeys->ALT_L_KEY));
		}
		if (inputKeys->LEFT_KEY)
		{
			this->selectedObj->selectedStuff->moveStuffBy(Ogre::Vector3(1, 0, 0));
		}
		if (inputKeys->RIGHT_KEY)
		{
			this->selectedObj->selectedStuff->moveStuffBy(Ogre::Vector3(-1, 0, 0));
		}

	}
}

void StuffHandler::deleteSelectedObj()
{
	//this->deleteObj(this->selectedObj->selectedStuff);
	*this->selectedObj = GuiSelectableObject();
}

