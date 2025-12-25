#pragma once


// Local
#include <GDHandler/stuff/Stuff.h>
#include <feel/Feel.h>
// Third

// STL



class Scene;
class Case;

class ResourceHandler;
class Monster;
class Kint;
class Feel;



// Primary Class.
// Used to build object with all required contexts and initializations
class GDBuilderContext
{	
protected:

	ResourceHandler* resourceHandler;
	Monster* monster;
	Kint* kint;
	Feel* feel;

	void startEngine();


public:

	GDBuilderContext(ResourceHandler* resourceHan_p, Monster* monster_p, Kint* kint_p, Feel* feel_p) {
		resourceHandler = resourceHan_p;
		monster = monster_p;
		kint = kint_p;
		feel = feel_p;
	}

	virtual void update(float deltaTime) {}

	// Create Objects
	virtual Scene* createScene() { return nullptr; }
	virtual Case* createCase() { return nullptr; }

};

class GDBuilderImpli {
private:
	GDBuilderContext* builderCxt;
public:
	GDBuilderImpli(GDBuilderContext* builderCxt_p) {
		builderCxt = builderCxt_p;
	}
};



