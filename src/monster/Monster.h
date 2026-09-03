#pragma once

#ifndef MONSTER_H
#define MONSTER_H

#ifndef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#define VK_EXT_DEBUG_REPORT_EXTENSION_NAME "VK_EXT_debug_report"
#endif


// STL headers
#if defined (_WIN32)
	#include <Windows.h>
#endif


#include <monster/MonsterImgui.h>

#include <feel/feel.h>





class Monster : public FeelPollEventExtension, public MonsterImgui
{

private:

	uint32_t triangleShaderIndex;

	void pollEvent(SDL_Event& event) override;
	void keyHit(MappedKey key) override;

	// EXAMPLE
	std::shared_ptr<hRes::SkyMesh> skyMesh = std::make_shared<hRes::SkyMesh>();
	std::shared_ptr<hRes::SkyTexMesh> skyTexMesh = std::make_shared<hRes::SkyTexMesh>();
	std::shared_ptr<hRes::SampleCube> sampleCube = std::make_shared<hRes::SampleCube>();

public:

	void InitMonster();


	void updateMonster(glm::vec3 cameraPosition, glm::vec2 cameraRotation, float deltaTime, float shaderTime);

	/*Mesh loadMeshObj(std::filesystem::path& path);*/

	void createRequiredShaders();

	void loadSkyBox();
	void skyBoxImguiMenu();

	void loadOtherMesh();


	void Shutdown();

};




#endif // !MONSTER_H



