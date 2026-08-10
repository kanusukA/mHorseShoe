
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <memory>


struct SDLStats {

	uint32_t windowWidth;
	uint32_t windowHeight;

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