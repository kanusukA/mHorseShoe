#pragma once

#ifndef GLOCK_H
#define GLOCK_H

#include<chrono>
#include<stdio.h>
#include<iostream>
#include <mutex>

#include <glm/glm.hpp>

double getCurrentTime();

int getFPS(int framesPerMilli);

#define MS_PRE_FRAME 16.0f


class Glock {
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
	std::chrono::steady_clock::time_point initTime;

	static Glock* pinstance_;
	static std::mutex mutex_;

protected:
	Glock() {
		initTime = std::chrono::steady_clock::now();
	};
	~Glock() {

	};

public:

	static Glock* GetInstance();

	// Class should not be cloneable
	Glock(Glock& copy) = delete;

	// Class should not be assignable
	void operator=(const Glock&) = delete;

	double deltaTime = 0;


	void setStartTime() {
		startTime = std::chrono::steady_clock::now();
	}

	void setEndTime() {
		endTime = std::chrono::steady_clock::now();
	}

	double getDelta() {
		deltaTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
		return deltaTime;
	}

	float getShaderTime() {
		return glm::fract(std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - initTime).count() / 100000);
		 
	}

	void setSleep(float duration) {
//		Sleep(duration);
	}



};


#endif // !GLOCK_H

