
#if defined (_WIN32)
	#define NOMINMAX 
	#include <Windows.h>
#endif

#if defined (_WIN32)
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <memory>

#include <stdexcept>

struct SDLStats {

	int windowWidth;
	int windowHeight;

	bool windowGrabed = false;
	bool windowFullscreen = false;

	bool framebufferResized = false;

	SDL_Window* window;

};

class MonsterSDL {
public:

	std::unique_ptr<SDLStats> sdlStats = std::make_unique<SDLStats>();

	void InitSDLWindow(uint16_t windowHeight, uint16_t windowWidth);

	void frameBufferResized();

	void windowGrab(bool state);

	void ShutdownSDL();

};