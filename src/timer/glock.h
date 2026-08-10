#pragma once

#ifndef GLOCK_H
#define GLOCK_H

#include<chrono>
#include<stdio.h>
#include<iostream>

double getCurrentTime();

int getFPS(int framesPerMilli);

#define MS_PRE_FRAME 16.0f


class Glock {
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
public:

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

	void setSleep(float duration) {
		Sleep(duration);
	}



};


#endif // !GLOCK_H

