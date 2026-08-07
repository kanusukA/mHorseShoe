#pragma once

#ifndef MONSTER_H
#define MONSTER_H

#ifndef VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#define VK_EXT_DEBUG_REPORT_EXTENSION_NAME "VK_EXT_debug_report"
#endif

#include "VulkanStats.h"

// Third-party header
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <imgui_stdlib.h>

// STL headers
#if defined (_WIN32)
	#include <Windows.h>
#endif

#include <monster/VulkanStats.h>


// Third-party header
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <imgui.h>
#include <imgui-1.92.9b/backends/imgui_impl_sdl3.h>
#include <imgui-1.92.9b/backends/imgui_impl_vulkan.h>

#include <Camera.h>






class Monster
{

private:

	//SDL
	SDLStats sdlStats = SDLStats();
	

	// IMGUI
	
	void updateSDL();

	void startImGuiFrame();
	void renderFrame();


	// INIT FUNCTIONS
	void InitSDLWindow();
	void InitVulkan();
	void InitImgui();
	bool createImGuiPipeline();
	
	void ShutdownSDL();
	void ShutdownImGui();
	void ShutdownVulkan();
	

public:

	void InitMonster();


	void Shutdown();

};




#endif // !MONSTER_H



