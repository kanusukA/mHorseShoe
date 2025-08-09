include(FetchContent)
FetchContent_Declare(
    sdl3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.18
)
FETCHCONTENT_MAKEAVAILABLE(sdl3)


find_file(
	sdlBuilder
	NAME sdlbuilder.py
	PATHS "${CMAKE_SOURCE_DIR}"
	RESULT_VARIABLE result
)


SET(SDL_SCRIPT "${sdlBuilder}")
SET(SDL_ARGS "${sdl3_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
message("Running sdl builder ${sdl3_SOURCE_DIR}")
find_package(Python3 COMPONENTS Interpreter REQUIRED)
execute_process(COMMAND ${Python3_EXECUTABLE} ${SDL_SCRIPT} ${SDL_ARGS}
	RESULT_VARIABLE script_result
	OUTPUT_VARIABLE script_output
	)
message("sdl build output - ${script_output}")
if (NOT script_result EQUAL 0)
	
	message(FATAL_ERROR "SDL WAS NOT BUILT")
endif()
message("SDL WAS BUILT")