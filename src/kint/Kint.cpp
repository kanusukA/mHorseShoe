#include "Kint.h"

PxReal pAccumulator = 0.0f;
PxReal pStepSize = 1.0f / 60.0f;

void Kint::InitPhysics() {

	pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pAllocator, pErrorCallback);

	pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pFoundation, PxTolerancesScale(), true);


	PxSceneDesc sceneDesc(pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	

	pScene = pPhysics->createScene(sceneDesc);

	pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	//pScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
	setGlobalVisualization(true);
	

}

void Kint::Shutdown() {
	pPhysics->release();
	pFoundation->release();
}

bool Kint::updatePhysics(float deltaTime) {

	pAccumulator += deltaTime;

	if (pAccumulator < pStepSize) {
		return false;
	}
		
	pAccumulator -= pStepSize;

	pScene->simulate(pStepSize);
	pScene->fetchResults(true);

	

}


PxRigidDynamic* Kint::createRigidBody(std::string name,PxTransform transfrom, PxReal mass, PxGeometry* geo) {

	PxMaterial* mat = pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	PxRigidDynamic* dyn = pPhysics->createRigidDynamic(transfrom);

	dyn->setMass(mass);

	PxShape* shape = PxRigidActorExt::createExclusiveShape(*dyn, *geo, *mat);

	shape->setFlag(PxShapeFlag::eVISUALIZATION, true);

	dyn->setName(name.c_str());
	pScene->addActor(*dyn);

	//shape->release();

	return dyn;

	

	

}



PxRigidStatic* Kint::createStaticBody(std::string name,PxTransform transfrom, PxGeometry* geo ) {
	PxMaterial* mat = pPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	
	

	std::cout << "position : " << transfrom.p.x << " " << transfrom.p.y << " " << transfrom.p.z << std::endl;

	PxRigidStatic* sta = pPhysics->createRigidStatic(transfrom);
	PxShape* shape = PxRigidActorExt::createExclusiveShape(*sta, *geo, *mat);
	shape->setFlag(PxShapeFlag::eVISUALIZATION, true);
	sta->setName(name.c_str());
	sta->setGlobalPose(transfrom);
	
	std::cout << "out position : " << sta->getGlobalPose().p.x << " " << sta->getGlobalPose().p.y << " " << sta->getGlobalPose().p.z << std::endl;

	pScene->addActor(*sta);

	//shape->release();

	return sta;
}

PxTriangleMesh* Kint::createTriangleCollisionMesh(int nbVert, PxVec3* vertices, int nbIndices, unsigned long* indices)
{
	// Triangle Data

	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = nbVert;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = vertices;

	meshDesc.triangles.count = nbIndices / 3;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = indices;

	PxTolerancesScale scale;
	PxCookingParams params(scale);

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;


	// i only fear the unknown
	// SDF
	PxSDFDesc sdfDesc;
	sdfDesc.numThreadsForSdfConstruction = 4;
	std::cout << "sdf validation: " << sdfDesc.isValid() << std::endl;
	meshDesc.sdfDesc = &sdfDesc;

	PxTriangleMesh* mesh = PxCreateTriangleMesh(params, meshDesc, pPhysics->getPhysicsInsertionCallback());

	if (mesh) {
		std::cout << "triangles : " << mesh->getNbTriangles() << std::endl << "vertices : " << mesh->getNbVertices();
		return mesh;
	}
	else {
		std::cout << "Mesh creation failed";
		return  nullptr;
	}

}

PxConvexMesh* Kint::createConvexCollisionMesh(int nbverts, PxVec3* vertices)
{
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = nbverts;
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = vertices;
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PxTolerancesScale scale;
	PxCookingParams params(scale);

	PxConvexMesh* convexMesh = PxCreateConvexMesh(params, convexDesc,pPhysics->getPhysicsInsertionCallback());

	if (convexMesh) {
		return convexMesh;
	}
	else {
		std::cout << "Error creating convex mesh";
		return nullptr;
	}


}

PxActor** Kint::getUpdatedActors(PxU32& nbActors)
{
	return pScene->getActiveActors(nbActors);
}

const PxDebugLine* Kint::getDebugLines()
{
	return pScene->getRenderBuffer().getLines();
}

PxU32 Kint::getNbDebugLine()
{
	return pScene->getRenderBuffer().getNbLines();
	
}

const PxDebugTriangle* Kint::getDebugTriangles()
{
	return pScene->getRenderBuffer().getTriangles();
}

PxU32 Kint::getNbDebugTriangles()
{
	return pScene->getRenderBuffer().getNbTriangles();
}

void Kint::setGlobalVisualization(bool value)
{
	if (value)
	{
		// SETS VISUALIZATION OF GLOBAL OBJECTS
		// USE CULLING_BOX TO SET A SMALLER RADIUS OF VISUALIZATION
		pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		// GET COLLISION EDGES OF ACTIVE OBJECTS
		pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}
	else {
		// POSITIVE VALUE - TURE , NEGATIVE VALUE - FALSE
		pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, -1.0f);
	}
	

}

