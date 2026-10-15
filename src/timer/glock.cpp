#include "glock.h"


// Multi-Thread shit
Glock* Glock::pinstance_{ nullptr };
std::mutex Glock::mutex_;


Glock* Glock::GetInstance() {

	// locks storage 
	std::lock_guard<std::mutex> lock(mutex_);
	if (pinstance_ == nullptr) {
		pinstance_ = new Glock();
	}

	return pinstance_;

}


double getCurrentTime() {

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());

	return ms.count();

}

std::string getTimeStamp() {
	return "";
}

int getFPS(int framesPerMilli) {
	return (float) 1000 / framesPerMilli;
}
