#pragma once

#ifndef KINT_H
#define KINT_H

#include <PxPhysicsAPI.h>
#include <PxPhysics.h>
#include <cooking/PxCooking.h>
#include <stdio.h>
#include <iostream>

// PHYSX VARIABLES
using namespace physx;

static PxDefaultAllocator pAllocator;
static PxDefaultErrorCallback pErrorCallback;
static PxDefaultCpuDispatcher* pDispatcher = NULL;
static PxPhysics* pPhysics = NULL;
static PxFoundation* pFoundation = NULL;
static PxMaterial* pMaterial = NULL;
static PxScene* pScene;

// THE PHYSICS CLASS
class Kint
{
public:

	void InitPhysics();
	void Shutdown();

	static bool updatePhysics(float deltaTime);

	// Creates and adds rigidbody to the physics scene.
	// transform - position of the rigidbody in scene.
	// mass - mass of the rigidbody.
	// geo - geometry used for making the shape.
	// mat - physics Material.

	PxRigidDynamic* createRigidBody(std::string name, PxTransform transfrom, PxReal mass, PxGeometry* geo);
	PxRigidStatic* createStaticBody(std::string name, PxTransform transfrom, PxGeometry* geo);

	// Triangle Mesh creation
	PxTriangleMesh* createTriangleCollisionMesh(int nbVert, PxVec3* vertices, int nbIndices,unsigned long* indices);

	// Convex Mesh creation
	PxConvexMesh* createConvexCollisionMesh(int nbverts, PxVec3* vertices);

	PxActor** getUpdatedActors(PxU32& nbActors);

	// VISUALIZAION
	const PxDebugLine* getDebugLines();
	PxU32 getNbDebugLine();
	const PxDebugTriangle* getDebugTriangles();
	PxU32 getNbDebugTriangles();
	void setGlobalVisualization(bool value);

};

#endif // !KINT_H


