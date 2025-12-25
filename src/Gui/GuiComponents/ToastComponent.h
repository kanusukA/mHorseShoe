#pragma once


//STL Headers
#include <stdio.h>
#include <vector>
#include <string>
#include <mutex>




class ToastComponent
{

private:
	// Multi-Thread Shit
	static ToastComponent* pinstance_;
	//static std::mutex mutex_;


protected:


	ToastComponent() {
	};
	~ToastComponent() {};

	std::vector<std::string>* messages = new std::vector<std::string>();


public:

	void addMessage(std::string msg) {
		messages->push_back(msg);
	}

	std::vector<std::string>* getMessages() {
		return messages;
	}

	// Class should not be clonable
	ToastComponent(ToastComponent& copy) = delete;

	// Class should not be assignable
	void operator=(const ToastComponent&) = delete;

	// Only way to initalize the class
	static ToastComponent* GetInstance() {

		// locks storage 
		//std::lock_guard<std::mutex> lock(mutex_);
		if (pinstance_ == nullptr) {
			pinstance_ = new ToastComponent();
		}
		return pinstance_;

	};



};


//std::mutex ToastComponent::mutex_;
