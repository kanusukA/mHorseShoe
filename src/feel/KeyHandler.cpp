#include "KeyHandler.h"

InputHandler* InputHandler::pInstance_{ nullptr };
std::mutex InputHandler::mutex_;

InputHandler* InputHandler::GetInstance()
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (pInstance_ == nullptr) {
		pInstance_ = new InputHandler();
	}
	return pInstance_;

}
