#include "glock.h"


double getCurrentTime() {

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());

	return ms.count();
}

int getFPS(int framesPerMilli) {
	return (float) 1000 / framesPerMilli;
}
